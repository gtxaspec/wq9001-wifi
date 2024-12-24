/****************************
 * Include
 ****************************/
#include <linux/types.h>
#include <net/cfg80211.h>

#include "wq_ipc.h"
#include "usb.h"
#include "../../wifi_drv.h"

//#define CFG_IPC_SIM_LOOPBACK

#define WQ_IPC_WAIT_TIMEOUT     (HZ*10) //1000
#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
extern void wq_sche_pktin_work(void *wq_dev);
#endif


/****************************
 * IPC module structure
 ****************************/
enum wq_ipc_state {
    WQ_IPC_STATE_DOWN,
    WQ_IPC_STATE_UP,
    WQ_IPC_STATE_SLEEP,
    WQ_IPC_STATE_STOP,
};

#ifdef WQ_BUS_TEST
#define WQ_IPC_MAX_PIPE_NUM         7           //bulk tx x 4 + bulk rx + int tx + int rx
#define WQ_IPC_MAX_RETRY_NUM        10          //tx test

#define WQ_IPC_LB_LOG_SZ            0x10000     //0xFFFFFFFF % 0x10000 = 0xFFFF
#define WQ_IPC_LB_PKT_HLEN          8           //4(pipe id, pkt idx) + 4(sum)
#define WQ_IPC_LB_PKT_LOSS_LOG_SZ   1000
#define WQ_IPC_LB_PKT_DATA_LOG_SZ   (WQ_IPC_LB_PKT_HLEN+10)

struct wq_ipc_test {
    // test parameter
    u8 tx_pipe[WQ_IPC_MAX_PIPE_NUM];
    u8 tx_pipe_num;
    u8 pattern;     //1(fixed), 2(increasing)
    u32 loop;       //loop number, note: 0(inifinite)
    u32 loop_multiple;
    u32 pkt_len;
    u8 pgmode;      //0(fixed length), 1(increasing length)
    u8 loopback;    //0(single-direction mode), 1(loopback mode)
    u8 silent_test; //0(log mode), 1(silent mode)

    // test statistic
  	spinlock_t log_lock;

    ktime_t tx_start_time, tx_end_time, tx_diff_time;
    ktime_t rx_start_time, rx_end_time, rx_diff_time;
    u64 tx_time_total, rx_time_total;  //unit: ms

    u32 tx_result[WQ_IPC_MAX_PIPE_NUM][WQ_IPC_MAX_RETRY_NUM+2];   //succ+retry+fail
    u32 tx_done_result[WQ_IPC_MAX_PIPE_NUM][2]; //idx#1: pipe;
                                                //idx#2: 0(succ), 1(fail)
    u32 txs_pkt_total;                          //tx submit pkt count
    u32 txd_pkt_total;                          //tx complete pkt count
    u64 tx_sum_total[2];                        //0(normal mode), 1(speed mode)
    u64 tx_len_total;
    u64 rx_sum_total[2][2];                     //idx1: 0(msg-in), 1(pkt-in). for single-direction test
                                                //idx2: 0(normal mode), 1(speed mode)
    u64 rx_len_total[2];                        //0(msg-in), 1(pkt-in). for single-direction test
    u32 rx_pkt_cnt[WQ_IPC_MAX_PIPE_NUM];        //case 1: rx test
                                                //        - 0(msg-in), 1(pkt-in)
                                                //case 2: loopback test
                                                //        - pipe_id
    u32 rx_unknown_pkt_cnt[2];  //0(msg-in), 1(pkt-in), for loopback test
    u32 rx_wrong_sum1_pkt_cnt[WQ_IPC_MAX_PIPE_NUM];
    u32 rx_wrong_sum2_pkt_cnt[WQ_IPC_MAX_PIPE_NUM];

    u32 rawsum[WQ_IPC_MAX_PIPE_NUM][WQ_IPC_LB_LOG_SZ];  //idx#1: pipe;
                                                        //idx#2: pkt idx
    u32 lb_pkt_loss_cnt[WQ_IPC_MAX_PIPE_NUM];   //loopback test pkt loss count
    u32 lb_pkt_loss_idx[WQ_IPC_MAX_PIPE_NUM][WQ_IPC_LB_PKT_LOSS_LOG_SZ]; //loopback test pkt loss log
    u32 lb_next_pkt_idx[WQ_IPC_MAX_PIPE_NUM];
    u8  lb_last_rx_pkt_data[WQ_IPC_MAX_PIPE_NUM][WQ_IPC_LB_PKT_DATA_LOG_SZ];

	struct work_struct tx_work;
};
#endif

struct wq_ipc {
    void *wq_dev;   //device module handler
    void *wq_mac;   //mac module handler
    struct wq_ipc_mac_interface mac_inf;

    bool fw_ready;
    enum wq_ipc_state state;

    //todo: all sync code coverage recheck
	struct mutex op_mutex;
    //todo: replace with irq version
	spinlock_t op_lock;
	struct completion op_wait;

    u8 cmd_status;
    u16 cmd_seq;
    u16 cmd_seq_last;
    u16 pkt_seq;

    u8 *cmd_rsp_buf;
    u32 cmd_rsp_buf_sz;

#ifdef WQ_BUS_TEST
    struct wq_ipc_test test;
#endif
};

/****************************
 * IPC header definition
 ****************************/
//IPC type
#define WQ_IPC_TPE_MASK     0x3
#define WQ_IPC_TPE_SHIFT    0
#define WQ_IPC_TPE_CMD      0x1
#define WQ_IPC_TPE_EVT      0x2
#define WQ_IPC_TPE_PKT      0x3
#define WQ_IPC_TPE(flags)   (((flags) & WQ_IPC_TPE_MASK) >> WQ_IPC_TPE_SHIFT)

//IPC receiver
#define WQ_IPC_RCV_MASK     0xC
#define WQ_IPC_RCV_SHIFT    2
#define WQ_IPC_RCV(flags)   (((flags) & WQ_IPC_RCV_MASK) >> WQ_IPC_RCV_SHIFT)

//IPC get/set status
#define WQ_IPC_STS_MASK	    0xF000
#define WQ_IPC_STS_SHIFT    12
#define WQ_IPC_STS(flags)   (((flags) & WQ_IPC_STS_MASK) >> WQ_IPC_STS_SHIFT)

enum wq_ipc_status {
	WQ_IPC_STS_SUCC = 0,
	WQ_IPC_STS_ERR
};

//IPC sequence number
#define WQ_IPC_SEQ_MASK     0xFFFF0000
#define WQ_IPC_SEQ_SHIFT	16
#define WQ_IPC_SEQ(flags)   (((flags) & WQ_IPC_SEQ_MASK) >> WQ_IPC_SEQ_SHIFT)

struct wq_ipc_header {
    __le32 flags;   //[0:1] type => 1:command, 2:event, 3:pkt (0:reserved)
                    //[2:3] receiver => 0:device, 1:interface#1, 2:interface#2
                    //[12:15] command status (returned from the device)
                    //[16:31] sequence number
    union {
        __le32 cmd_id;  //command id
        __le32 evt_id;  //event id
        __le32 pkt_flags;
    } u;
    __le32 buf_len;     //buffer len
} __packed;


/****************************
 * IPC debug
 ****************************/
#define WQ_FN_ENTRY_STR "--> %s()\n", __func__

enum dbg_sev_tag
{
    DBG_SEV_NONE = 0,
    DBG_SEV_OOPS,
    DBG_SEV_ERR,
    DBG_SEV_WRN,
    DBG_SEV_INF,
    DBG_SEV_VRB,
    DBG_SEV_MAX
};

#define WQ_LOG_IPC_DBG ((wq_dbg_flag & WQ_DEBUG_IPC)? WQ_LOG_IPC_SEV : DBG_SEV_NONE)
extern void ieee80211_catch_log(char *fmt, ...);

#define IPC_DBG_VRB(fmt, ...)               \
    do {                                    \
        if (WQ_LOG_IPC_DBG > DBG_SEV_INF) {\
            printk(fmt, ##__VA_ARGS__);     \
        }                                   \
    } while (0)

#define IPC_DBG_INF(fmt, ...)               \
    do {                                    \
        if (WQ_LOG_IPC_DBG > DBG_SEV_WRN) {\
            if(wq_dbg_flag & WQ_DEBUG_LOG2FILE){            \
                ieee80211_catch_log(fmt, ##__VA_ARGS__); \
            }          \
            else       \
                printk(fmt, ##__VA_ARGS__);     \
        }                                   \
    } while (0)

#define IPC_DBG_WRN(fmt, ...)               \
    do {                                    \
        if (WQ_LOG_IPC_DBG > DBG_SEV_ERR) {\
            if(wq_dbg_flag & WQ_DEBUG_LOG2FILE){            \
                ieee80211_catch_log(fmt, ##__VA_ARGS__); \
            }          \
            else       \
                printk(fmt, ##__VA_ARGS__);     \
        }                                   \
    } while (0)

#define IPC_DBG_ERR(fmt, ...)                   \
    do {                                        \
        if (WQ_LOG_IPC_DBG > DBG_SEV_OOPS) {   \
            if(wq_dbg_flag & WQ_DEBUG_LOG2FILE){            \
                ieee80211_catch_log(fmt, ##__VA_ARGS__); \
            }          \
            else       \
                printk(fmt, ##__VA_ARGS__);         \
        }                                       \
    } while (0)

#define IPC_DBG_OOPS(fmt, ...)                  \
    do {                                        \
        if (WQ_LOG_IPC_DBG > DBG_SEV_NONE) {   \
            if(wq_dbg_flag & WQ_DEBUG_LOG2FILE){            \
                ieee80211_catch_log(fmt, ##__VA_ARGS__); \
            }          \
            else{       \
                printk(fmt, ##__VA_ARGS__);         \
                BUG();                              \
            }                                       \
        }                                       \
    } while (0)


/*****************************
 * Function
 *****************************/
#ifdef WQ_BUS_TEST
void wq_ipc_tx_test_work(struct work_struct *work);
#endif

u64 ipc_tx_seq = 0;
u64 ipc_tx_done_seq = 0;
u64 ipc_pre_check_seq = 0;

//#ifdef WQ_FW_DL
//for fw ready event simulation test
//static void
//wq_ipc_event_handler(struct wq_ipc *ipc, enum wq_ipc_rcvs receiver, enum wq_ipc_evt_ids id,
//                     u32 buf_len, struct sk_buff *skb);
//#endif

void*
wq_ipc_attach(void *wq_dev)
{
    struct wq_ipc *ipc = NULL;

    IPC_DBG_INF(WQ_FN_ENTRY_STR);

	ipc = kzalloc(sizeof(struct wq_ipc), GFP_ATOMIC);
	if (ipc) {
        ipc->wq_dev = wq_dev;
        ipc->fw_ready = false;

        mutex_init(&ipc->op_mutex);
        spin_lock_init(&ipc->op_lock);
        init_completion(&ipc->op_wait);

        ipc->cmd_seq = 0;
        ipc->pkt_seq = 0;

#ifdef WQ_BUS_TEST
        ipc->test.tx_pipe_num = 1;
        ipc->test.tx_pipe[0] = 1;
        ipc->test.pattern = 1;
        ipc->test.loop = 1;
        ipc->test.pkt_len = 100;
        ipc->test.pgmode = 0;
        ipc->test.loopback = 0;
        ipc->test.silent_test = 0;
        ipc->test.loop_multiple = 1;

        INIT_WORK(&ipc->test.tx_work, wq_ipc_tx_test_work);
        spin_lock_init(&ipc->test.log_lock);

        ipc->state = WQ_IPC_STATE_UP;
#else   //wifi mode
#ifdef WQ_FW_DL
        ipc->state = WQ_IPC_STATE_DOWN;

        //for fw ready event simulation test
        //wq_ipc_event_handler(ipc, WQ_IPC_RCV_DEV, WQ_IPC_EVT_FWREADY, 0, 0);
#else
        ipc->state = WQ_IPC_STATE_UP;
#endif
#endif
    }

    return (void *)ipc;

#if 0
fail:
    wq_ipc_detach((void *)ipc);

    return NULL;
#endif
}

void
wq_ipc_state_stop(void *wq_ipc)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;

    IPC_DBG_INF(WQ_FN_ENTRY_STR);

    if (ipc == NULL)
        return;

    complete_all(&ipc->op_wait); // maybe a dummy signal

    // stop ipc to prevent usb data to/from hawk_usb module
    IPC_DBG_INF("%s %d\n", __func__, __LINE__);
    mutex_lock(&ipc->op_mutex);
    ipc->state = WQ_IPC_STATE_STOP;
    mutex_unlock(&ipc->op_mutex);
}

void
wq_ipc_detach(void *wq_ipc)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;

    IPC_DBG_INF(WQ_FN_ENTRY_STR);

  	if (ipc == NULL)
		return;

    // free ipc resource
  	mutex_lock(&ipc->op_mutex);
	ipc->state = WQ_IPC_STATE_DOWN;
	mutex_unlock(&ipc->op_mutex);

#ifdef WQ_BUS_TEST
    cancel_work_sync(&ipc->test.tx_work);
#endif

   	kfree(ipc);
}

void
wq_ipc_mac_register(void *wq_ipc, void *wq_mac, struct wq_ipc_mac_interface *mac_inf)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;

    IPC_DBG_INF(WQ_FN_ENTRY_STR);

    ipc->wq_mac = wq_mac;
    ipc->mac_inf.wq_mac_tx_done = mac_inf->wq_mac_tx_done;
    ipc->mac_inf.wq_mac_rx_pkt = mac_inf->wq_mac_rx_pkt;
    ipc->mac_inf.wq_mac_evt_handler = mac_inf->wq_mac_evt_handler;
}

int
wq_ipc_headroom_size()
{
    return sizeof(struct wq_ipc_header);
}

void
wq_ipc_event_hdr_parse(struct sk_buff *skb, u32 *id, u32 *ipc_buf_len, u32 *ipc_hdr_len)
{
    struct wq_ipc_header *ipc_hdr = NULL;
    u32 flags = 0, type = 0;

    ipc_hdr = (struct wq_ipc_header *)(skb->data);
    flags = le32_to_cpu(ipc_hdr->flags);

    type = WQ_IPC_TPE(flags);
    WARN_ON(type != WQ_IPC_TPE_EVT);

    *id = le32_to_cpu(ipc_hdr->u.evt_id);
    *ipc_buf_len = ipc_hdr->buf_len;
    *ipc_hdr_len = sizeof(struct wq_ipc_header);

    return;
}

void wq_ipc_send_trigger_pattern(void *wq_ipc, u16 type, u16 info)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;

    //send trigger pattern
    wq_usb_send_trigger_pattern(ipc->wq_dev, type, info);
}

int wq_ipc_check_pkt_type(struct sk_buff *skb)
{
    u32 flags = 0;
    u32 type = 0;
    struct wq_ipc_header *ipc_hdr = NULL;

    ipc_hdr = (struct wq_ipc_header *)(skb->data);
    flags = le32_to_cpu(ipc_hdr->flags);
    type = WQ_IPC_TPE(flags);

    if (type == WQ_IPC_TPE_PKT) {
        return 1;
    }

    return 0;
}

u8 g_trigger_flag = 0;

#ifndef WQ_BUS_TEST
int
wq_ipc_cmd_out(void *wq_ipc, enum wq_ipc_rcvs receiver, enum wq_ipc_cmd_ids cmd_id,
           u8 *cmd_buf, u32 cmd_len, u8 *rsp_buf, u32 rsp_buf_sz)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    struct wq_ipc_header *ipc_hdr = NULL;
    struct sk_buff *skb;
    u8 *data;
    u16 headroom = sizeof(struct wq_ipc_header);
    u32 ipc_flags = 0;
    unsigned long flags = 0;
    unsigned long left_time = 0;
    int ret = 0;

    if (!ipc) {
        IPC_DBG_ERR("%s: ipc is null\n", __func__);
        return -EIO;
    }

    if (ipc->state == WQ_IPC_STATE_STOP) {
        IPC_DBG_ERR("%s: ipc is stoped\n", __func__);
        return -EIO;
    }

    skb = __dev_alloc_skb(headroom + cmd_len, WQ_MEM_GFP);
    if (!skb)
        return -ENOMEM;

    skb_reserve(skb, headroom);
    if (cmd_len != 0 && cmd_buf != NULL) {
        data = (u8 *) skb_put(skb, cmd_len);
        memcpy(data, cmd_buf, cmd_len);
    }

    mutex_lock(&ipc->op_mutex);

    if (ipc->state != WQ_IPC_STATE_UP) {
        ret = -EIO;
        goto fail;
    }

    ipc_hdr = (struct wq_ipc_header *) skb_push(skb, headroom);
    ipc_flags = WQ_IPC_TPE_CMD | ((receiver << WQ_IPC_RCV_SHIFT) & WQ_IPC_RCV_MASK);
    ipc_flags |= ((++ipc->cmd_seq << WQ_IPC_SEQ_SHIFT) & WQ_IPC_SEQ_MASK);
    ipc_hdr->flags = cpu_to_le32(ipc_flags);
    ipc_hdr->u.cmd_id = cpu_to_le32(cmd_id);
    ipc_hdr->buf_len = cpu_to_le32(cmd_len);

    spin_lock_irqsave(&ipc->op_lock, flags);
    ipc->cmd_seq_last = ipc->cmd_seq;
    ipc->cmd_rsp_buf = rsp_buf;
    ipc->cmd_rsp_buf_sz = rsp_buf_sz;
    spin_unlock_irqrestore(&ipc->op_lock, flags);

    IPC_DBG_INF("%s: cmd_id=%d, receiver=%d, seq=%d, rsp_buf_sz=%d, skb=0x%p\n", __func__,
                cmd_id, receiver, ipc->cmd_seq_last, rsp_buf_sz, skb);

    ret = wq_usb_msg_out(ipc->wq_dev, skb);
    if (ret) {
        ipc->state = WQ_IPC_STATE_DOWN;
        IPC_DBG_ERR("%s: tx urb failed, id=%d, ret=%d\n", __func__, cmd_id, ret);
        goto fail;
    }

    left_time = wait_for_completion_timeout(&ipc->op_wait, WQ_IPC_WAIT_TIMEOUT);
    if (!left_time) {
        ipc->state = WQ_IPC_STATE_DOWN;
        IPC_DBG_ERR("%s: command(id=%d) timeout!\n", __func__, cmd_id);
        ret = -ETIMEDOUT;

        //send trigger pattern
        wq_usb_send_trigger_pattern(ipc->wq_dev, WQ_USB_TRI_CMD_TIMEOUT, ipc->cmd_seq_last);

        //dump usb state
        wq_usb_dump_info(ipc->wq_dev);
    } else {
        spin_lock_irqsave(&ipc->op_lock, flags);
        ret = (ipc->cmd_status == WQ_IPC_STS_SUCC)? 0:(-EREMOTEIO);
        spin_unlock_irqrestore(&ipc->op_lock, flags);

        if (ret != 0) {
            ipc->state = WQ_IPC_STATE_DOWN;
            IPC_DBG_ERR("%s: command(id=%d) failed! (status=%d)!\n", __func__, cmd_id, ipc->cmd_status);
        }
    }

    mutex_unlock(&ipc->op_mutex);

    return ret;

fail:
    IPC_DBG_ERR("%s: failed, ipc state=%d, command id=%d, ret=%d\n",
                __func__, ipc->state, cmd_id, ret);

    mutex_unlock(&ipc->op_mutex);
    dev_kfree_skb_any(skb);

    return ret;
}

void
wq_ipc_cmd_out_cb(void *wq_ipc, u8 pipe_id, struct sk_buff *skb, int status)
{
    IPC_DBG_INF("%s: skb=0x%p, status=%d\n", __func__, skb, status);

    if (status) {
        IPC_DBG_ERR("%s: failed (status = %d)\n", __func__, status);
    }

    if (!skb)
		return;

    dev_kfree_skb_any(skb);
}

static void
wq_ipc_cmd_handler(struct wq_ipc *ipc, enum wq_ipc_rcvs receiver, enum wq_ipc_status status,
                   u32 seq, enum wq_ipc_cmd_ids id, u32 buf_len, struct sk_buff *skb)
{
    u32 len = 0;

    if ((ipc->state != WQ_IPC_STATE_UP)
        || (id >= WQ_IPC_CMD_MAX)) {
        IPC_DBG_WRN("%s: drop, state=%d, status=%d, seq=%d, id=%d\n", __func__,
                    ipc->state, status, seq, id);
        return;
    }

    spin_lock(&ipc->op_lock);
    if (seq != ipc->cmd_seq_last) {
        IPC_DBG_WRN("%s: drop, seq mismatch, id=%d, seq=%d/%d, status=%d\n",
                    __func__, id, seq, ipc->cmd_seq_last, status);
        goto fail;
    }

    ipc->cmd_status = status;
    if (status == WQ_IPC_STS_SUCC) {
        if (ipc->cmd_rsp_buf != NULL) {
            WARN_ON(ipc->cmd_rsp_buf_sz < skb->len);

            len = min(ipc->cmd_rsp_buf_sz, skb->len);
            memcpy(ipc->cmd_rsp_buf, skb->data, len);
        }
    }
    spin_unlock(&ipc->op_lock);

    complete(&ipc->op_wait);

    return;

fail:
    spin_unlock(&ipc->op_lock);
}

static void
wq_ipc_event_handler(struct wq_ipc *ipc, enum wq_ipc_rcvs receiver, enum wq_ipc_evt_ids id,
                     u32 buf_len, struct sk_buff *skb)
{
    /* 1. check input */
    if ((id != WQ_IPC_EVT_FWREADY)
        && ((ipc->state != WQ_IPC_STATE_UP) || (id >= WQ_IPC_EVT_MAX))) {
        IPC_DBG_WRN("%s: drop, id=%d, state=%d\n", __func__, id, ipc->state);
        //BUG();
        return;
    }

    /* 2. event process */
    //IPC_DBG_INF("%s: id=%d\n", __func__, id);
    switch(id)
    {
    case WQ_IPC_EVT_FWREADY:
        ipc->fw_ready = true;
        ipc->state = WQ_IPC_STATE_UP;

        wq_usb_set_dev_ready(ipc->wq_dev);
        break;
    default:
        // upload to mac module
        if (ipc->mac_inf.wq_mac_evt_handler) {
            ipc->mac_inf.wq_mac_evt_handler(ipc->wq_mac, buf_len, skb);
        } else {
            BUG();
        }
        break;
    }
}

void
wq_ipc_msg_in(void *wq_ipc, struct sk_buff *skb, bool success)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    struct wq_ipc_header *ipc_hdr = NULL;
    u32 flags = 0;
    u32 type = 0;
    u32 receiver = 0;
    u32 status = 0;
    u32 seq = 0;
    u32 id = 0;
    u32 buf_len = 0;
    u32 enqueue_skb = 0;

	if (!skb) {
        IPC_DBG_WRN("%s: skb is null\n", __func__);
		return;
	}

    if (!ipc) {
        IPC_DBG_ERR("%s: ipc is null!\n", __func__);
        goto fail;
    }

    if (ipc->state == WQ_IPC_STATE_STOP) {
        IPC_DBG_ERR("%s: ipc is stoped!\n", __func__);
        goto fail;
    }

    ipc_hdr = (struct wq_ipc_header *)(skb->data);
    flags = le32_to_cpu(ipc_hdr->flags);
    buf_len = le32_to_cpu(ipc_hdr->buf_len);

    type = WQ_IPC_TPE(flags);
    receiver = WQ_IPC_RCV(flags);
    status = WQ_IPC_STS(flags);
    seq = WQ_IPC_SEQ(flags);

    /* keep IPC header for event rx work queue */
    if (type == WQ_IPC_TPE_EVT)
        buf_len += sizeof(struct wq_ipc_header);
    else
        skb_pull(skb, sizeof(struct wq_ipc_header));

    IPC_DBG_INF("%s: type=%d, receiver=%d, status=%d, seq=%d, id=%d, len=%d, skb len=%d\n",
                __func__, type, receiver, status, seq, le32_to_cpu(ipc_hdr->u.cmd_id),
                buf_len, skb->len);

    WARN_ON(buf_len > skb->len);

    switch (type)
    {
    case WQ_IPC_TPE_CMD:
        id = le32_to_cpu(ipc_hdr->u.cmd_id);
        wq_ipc_cmd_handler(ipc, receiver, status, seq, id, buf_len, skb);
        break;
    case WQ_IPC_TPE_EVT:
        id = le32_to_cpu(ipc_hdr->u.evt_id);
        enqueue_skb = (id == WQ_IPC_EVT_FWREADY)? 0:1;
        wq_ipc_event_handler(ipc, receiver, id, buf_len, skb);
        break;
    default:
        IPC_DBG_OOPS("%s: wrong type, flags = 0x%x\n", __func__, flags);
        break;
    }

fail:
    if (!enqueue_skb) {
     	WARN_ON(skb->next);
        dev_kfree_skb_any(skb);
    }
}

/*
 * note:
 *  if return != 0, the caller should free skb
 */
int
wq_ipc_tx_pkt(void *wq_ipc, enum wq_ipc_rcvs receiver, enum wq_ipc_txq txq,
              u32 pkt_flags, void *buf, u8 *data, u32 len)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    struct wq_ipc_header *ipc_hdr = NULL;
    u16 headroom = sizeof(struct wq_ipc_header);
    u32 len_pkt = 0;
    u32 ipc_flags = 0;
    enum wq_usb_txq utxq[] = {WQ_USB_TXQ_BE, WQ_USB_TXQ_BK, WQ_USB_TXQ_VI, WQ_USB_TXQ_VO};
    int ret = 0;

    if (!ipc) {
        IPC_DBG_ERR("%s: ipc is null\n", __func__);
        return -EIO;
    }

    if (ipc->state != WQ_IPC_STATE_UP) {
        IPC_DBG_ERR("%s: ipc isn't up\n", __func__);
        return -EIO;
    }

    if (!data) {
        IPC_DBG_ERR("%s: data is null\n", __func__);
        return -ENODATA;
    }

    len_pkt = len - headroom;

    ipc_hdr = (struct wq_ipc_header *)data;
    ipc_flags = WQ_IPC_TPE_PKT | ((receiver << WQ_IPC_RCV_SHIFT) & WQ_IPC_RCV_MASK);
    ipc_flags |= ((++ipc->pkt_seq << WQ_IPC_SEQ_SHIFT) & WQ_IPC_SEQ_MASK);
    ipc_hdr->flags = cpu_to_le32(ipc_flags);
    ipc_hdr->u.pkt_flags = cpu_to_le32(pkt_flags);
    ipc_hdr->buf_len = cpu_to_le32(len_pkt);

    IPC_DBG_INF("%s: receiver=%d, txq=%d, pkt_flags=0x%x, pkt_len=%d, pkt_seq=%d\n", __func__,
                receiver, txq, pkt_flags, len_pkt, ipc->pkt_seq);

    ipc_tx_seq = ipc->pkt_seq;
    ret = wq_usb_pkt_out(ipc->wq_dev, utxq[txq], buf, data, len);
    if (ret) {
        IPC_DBG_ERR("%s: tx pkt failed, ret=%d, receiver=%d, txq=%d, pkt_flags=0x%x, pkt_len=%d\n",
                    __func__, ret, receiver, txq, pkt_flags, len_pkt);
    }

    return ret;
}

/*
 * note:
 *  skb free by mac module
 */
void
wq_ipc_tx_pkt_cb(void *wq_ipc, u8 pipe_id, void *buf, void *data, int status)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    struct wq_ipc_header *ipc_hdr = (struct wq_ipc_header *)data;
    u32 flags = le32_to_cpu(ipc_hdr->flags);

    /* 1. check input */
    if (!ipc) {
        IPC_DBG_ERR("%s: ipc is null!\n", __func__);
        goto err_oops;
    }

    IPC_DBG_INF("%s: pkt_seq=%d, status=%d\n", __func__, WQ_IPC_SEQ(flags), status);
    ipc_tx_done_seq = WQ_IPC_SEQ(flags);

    /* 2. indicate to mac module */
    if (ipc->mac_inf.wq_mac_tx_done) {
        ipc->mac_inf.wq_mac_tx_done(ipc->wq_mac, buf, status);
    } else {
        goto err_oops;
    }

    return;

err_oops:
    BUG();
}

/*
 * note:
 *  skb free by mac module, reference wq_usb_skb_free()
 */
void
wq_ipc_rx_pkt(void *wq_ipc, struct sk_buff *skb, bool success)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    struct wq_ipc_header *ipc_hdr = NULL;
    u32 flags = 0;
    u32 buf_len = 0;

    /* 1. check input */
    if (!skb) {
        IPC_DBG_ERR("%s: skb is null\n", __func__);
		goto err_oops;
	}

    if (!ipc) {
        IPC_DBG_ERR("%s: ipc is null!\n", __func__);
        goto err_release;
    }

    if (ipc->state == WQ_IPC_STATE_STOP) {
        IPC_DBG_ERR("%s: ipc is stoped!\n", __func__);
        WARN_ON(skb->next);
        dev_kfree_skb_any(skb);
        return;
    }

    /* 2. ipc header process */
    ipc_hdr = (struct wq_ipc_header *)(skb->data);
    buf_len = le32_to_cpu(ipc_hdr->buf_len);
    flags = le32_to_cpu(ipc_hdr->flags);

    if ((ipc->state != WQ_IPC_STATE_UP) || g_trigger_flag) {
        IPC_DBG_INF("%s: skb=0x%p, success=%d, buf_len=%d, seq=%d\n", __func__, skb, success, buf_len, WQ_IPC_SEQ(flags));
    }

    skb_pull(skb, sizeof(struct wq_ipc_header));

    if (skb->len != buf_len) {
        IPC_DBG_ERR("%s: len mismatch!\n", __func__);
        dev_kfree_skb_any(skb);
        return;
    }

    /* 3. upload to mac module */
    if (ipc->mac_inf.wq_mac_rx_pkt) {
        ipc->mac_inf.wq_mac_rx_pkt(ipc->wq_mac, skb, buf_len);
    } else {
        goto err_release;
    }

    return;

err_release:
    //free skb
	WARN_ON(skb->next);
    dev_kfree_skb_any(skb);

err_oops:
    BUG();
}
#else
int
wq_ipc_cmd_out(void *wq_ipc, enum wq_ipc_rcvs receiver, enum wq_ipc_cmd_ids cmd_id,
           u8 *cmd_buf, u32 cmd_len, u8 *rsp_buf, u32 rsp_buf_sz)
{
    IPC_DBG_ERR("%s: failed! In bus test mode! cmd_id=%d, receiver=%d\n", __func__,
                cmd_id, receiver);
    return 0;
}

void
wq_ipc_cmd_out_cb(void *wq_ipc, u8 pipe_id, struct sk_buff *skb, int status)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
#if 0
    u8 pipe_id;
    u32 *phd = NULL;
#endif

    /* 1. check input */
    if ((status) || (!skb) || (!wq_ipc) || (wq_usb_is_msg_out_pipe(pipe_id) == false)) {
        IPC_DBG_WRN("%s: Error! (status=%d, skb=0x%p, ipc=0x%p, pipe_id=%d)\n",
                    __func__, status, skb, wq_ipc, pipe_id);
    }

    /* 2. update statistic */
#if 0
    if (ipc && skb) {
        phd = (u32 *)(skb->data);
        pipe_id = (phd[0] >> 24) & 0xFF;
#else
    if ((ipc) && (wq_usb_is_msg_out_pipe(pipe_id) == true)) {
#endif
        if (status) {
            ipc->test.tx_done_result[pipe_id-1][1]++;
        } else {
            ipc->test.tx_done_result[pipe_id-1][0]++;
        }
     }

    /* 3. free skb */
    if (!skb)
		return;

    dev_kfree_skb_any(skb);
}

void
wq_ipc_dump_rx_info(struct wq_ipc *ipc)
{
    IPC_DBG_INF("%s: next pkt idx=%u-%u-%u-%u-%u-%u-%u\n", __func__, ipc->test.lb_next_pkt_idx[0],
                ipc->test.lb_next_pkt_idx[1], ipc->test.lb_next_pkt_idx[2],
                ipc->test.lb_next_pkt_idx[3], ipc->test.lb_next_pkt_idx[4],
                ipc->test.lb_next_pkt_idx[5], ipc->test.lb_next_pkt_idx[6]);
    return;
}

void
wq_ipc_dump_test_info(struct wq_ipc *ipc, u8 locked)
{
    u32 i=0, j=0;
    unsigned long flags = 0;
    u64 throughput[2] = {0};

    if (locked == 0) {
        spin_lock_irqsave(&ipc->test.log_lock, flags);
    }

    for (i = 0; i < WQ_IPC_MAX_PIPE_NUM; i++) {
        IPC_DBG_INF("\n\n%s: ------------------------ pipe#%d ------------------------\n", __func__, i+1);

        // tx urb submit status
        IPC_DBG_INF("%s: tx submit: succ=%u, rty=%u-%u-%u-%u-%u-%u-%u-%u-%u-%u, failed=%u\n",
                    __func__, ipc->test.tx_result[i][0], ipc->test.tx_result[i][1],
                    ipc->test.tx_result[i][2], ipc->test.tx_result[i][3],
                    ipc->test.tx_result[i][4], ipc->test.tx_result[i][5],
                    ipc->test.tx_result[i][6], ipc->test.tx_result[i][7],
                    ipc->test.tx_result[i][8], ipc->test.tx_result[i][9],
                    ipc->test.tx_result[i][10], ipc->test.tx_result[i][11]);

        // tx urb complete status
        IPC_DBG_INF("%s: tx complete: succ=%u, failed=%u, total=%u\n", __func__,
                    ipc->test.tx_done_result[i][0], ipc->test.tx_done_result[i][1],
                    (ipc->test.tx_done_result[i][0] + ipc->test.tx_done_result[i][1]));

        if (ipc->test.loopback) {
            // loopback rx pkt info
            IPC_DBG_INF("%s: rx pkt info: sum1 failed=%u, sum2 failed=%u, correct=%u\n",
                        __func__, ipc->test.rx_wrong_sum1_pkt_cnt[i],
                        ipc->test.rx_wrong_sum2_pkt_cnt[i], ipc->test.rx_pkt_cnt[i]);

            // loopback pkt loss info
            IPC_DBG_INF("%s: pkt loss count = %u\n", __func__, ipc->test.lb_pkt_loss_cnt[i]);
            for (j = 0; j < ipc->test.lb_pkt_loss_cnt[i]; j++) {
                IPC_DBG_INF("%s: pkt loss#%u: idx=%u\n", __func__, j, ipc->test.lb_pkt_loss_idx[i][j]);
                if (j  == (WQ_IPC_LB_PKT_LOSS_LOG_SZ-1)) {
                    IPC_DBG_INF("%s: reach pkt loss log limit !\n", __func__);
                    break;
                }
            }
        }
    }

    IPC_DBG_INF("\n\n%s: txs_pkt_total=%u, txd_pkt_total=%u\n", __func__,
                ipc->test.txs_pkt_total, ipc->test.txd_pkt_total);
    IPC_DBG_INF("%s: tx_sum_total = %llu(normal)/%llu(speed), tx_len_total = %llu\n", __func__,
                ipc->test.tx_sum_total[0], ipc->test.tx_sum_total[1], ipc->test.tx_len_total);

    if (ipc->test.loopback) {
        // loopback unknown pkt info
        IPC_DBG_INF("\n\n%s: rx unknown pkt - msg-in=%u, pkt-in=%u\n", __func__,
                    ipc->test.rx_unknown_pkt_cnt[0], ipc->test.rx_unknown_pkt_cnt[1]);
    } else {
        IPC_DBG_INF("\n\n%s: rx_pkt_cnt: msg-in=%u, pkt-in=%u\n", __func__, ipc->test.rx_pkt_cnt[0],
                    ipc->test.rx_pkt_cnt[1]);
        IPC_DBG_INF("%s: rx_sum_total: msg-in=%llu(normal)/%llu(speed), pkt-in=%llu(normal)/%llu(speed)\n", __func__,
                    ipc->test.rx_sum_total[0][0], ipc->test.rx_sum_total[0][1],
                    ipc->test.rx_sum_total[1][0], ipc->test.rx_sum_total[1][1]);
        IPC_DBG_INF("%s: rx_len_total: msg-in=%llu, pkt-in=%llu, total=%llu\n", __func__,
                    ipc->test.rx_len_total[0], ipc->test.rx_len_total[1],
                    (ipc->test.rx_len_total[0]+ipc->test.rx_len_total[1]));
    }

    if (ipc->test.tx_time_total) {
        throughput[0] = ipc->test.tx_len_total*8*1000/1024/1024;
        do_div(throughput[0], ipc->test.tx_time_total);
        IPC_DBG_INF("\n\n%s: tx duration = %llu ms, tx throughput = %llu Mbps\n", __func__,
                    ipc->test.tx_time_total, throughput[0]);
    }
    if (ipc->test.rx_time_total) {
        throughput[1] = (ipc->test.rx_len_total[0]+ipc->test.rx_len_total[1])*8*1000/1024/1024;
        do_div(throughput[1], ipc->test.rx_time_total);
        IPC_DBG_INF("%s: rx duration = %llu ms, rx throughput = %llu Mbps\n", __func__,
                    ipc->test.rx_time_total, throughput[1]);
    }
    if (throughput[0] && throughput[1]) {
        IPC_DBG_INF("%s: total throughput = %llu Mbps\n", __func__, throughput[0]+throughput[1]);
    }

    IPC_DBG_INF("%s: ---------------------------------------------------------\n", __func__);

    if (locked == 0) {
        spin_unlock_irqrestore(&ipc->test.log_lock, flags);
    }

    return;
}

void
wq_ipc_msg_in(void *wq_ipc, struct sk_buff *skb, bool success)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    u8 *p = NULL;
    u32 sum;
    u32 j;
    unsigned long flags = 0;

    /* 1. check input */
	if (!skb) {
        IPC_DBG_OOPS("%s: null pointer, wq_ipc=0x%p, skb=0x%p\n", __func__, wq_ipc, skb);
		return;
	} else if (!wq_ipc) {
        IPC_DBG_OOPS("%s: null pointer, wq_ipc=0x%p, skb=0x%p\n", __func__, wq_ipc, skb);
		goto free_buf;
	} else if (ipc->test.loopback) {
        if (skb->len <= WQ_IPC_LB_PKT_HLEN) {
            spin_lock_irqsave(&ipc->test.log_lock, flags);
            ipc->test.rx_unknown_pkt_cnt[0]++;
            IPC_DBG_WRN("%s: unknown pkt#%u, len=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x...\n",
                        __func__, ipc->test.rx_unknown_pkt_cnt[0], skb->len,
                        p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            wq_ipc_dump_rx_info(ipc);
            spin_unlock_irqrestore(&ipc->test.log_lock, flags);

            goto free_buf;
        }
	}

    spin_lock_irqsave(&ipc->test.log_lock, flags);

    /* 2. data process */
    if (ipc->test.loopback) {
        u32 *phd = NULL;
        u32 pkt_idx=0, len=0;
        u8 pipe_id=0;

        phd = (u32 *)(skb->data);
        pipe_id = (phd[0] >> 24) & 0xFF;
        pkt_idx = phd[0] & 0xFFFFFF;

        p = (u8 *)(skb->data);
        p += WQ_IPC_LB_PKT_HLEN;
        sum = 0;
        for (j = 0; j < (skb->len - WQ_IPC_LB_PKT_HLEN); j++) {
            sum += p[j];
        }

        if ((wq_usb_is_msg_out_pipe(pipe_id) == false)
            || (pkt_idx >= WQ_IPC_LB_LOG_SZ)) {
            ipc->test.rx_unknown_pkt_cnt[0]++;
            IPC_DBG_WRN("%s: unknown pkt#%u, pipe=%d, pkt_idx=%u, phd[1]=%u, sum=%u, len=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                        __func__, ipc->test.rx_unknown_pkt_cnt[0], pipe_id, pkt_idx, phd[1],
                        sum, skb->len, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
            wq_ipc_dump_rx_info(ipc);
        } else {
            if (sum != phd[1]) {
                ipc->test.rx_wrong_sum1_pkt_cnt[pipe_id-1]++;
                IPC_DBG_WRN("%s: pipe=%d, wrong sum1 pkt#%u, idx=%d, phd[1]=%u, sum=%u, len=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                            __func__, pipe_id, ipc->test.rx_wrong_sum1_pkt_cnt[pipe_id-1],
                            pkt_idx, phd[1], sum, skb->len,
                            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
                wq_ipc_dump_rx_info(ipc);
            } else if (sum != ipc->test.rawsum[pipe_id-1][pkt_idx]) {
                ipc->test.rx_wrong_sum2_pkt_cnt[pipe_id-1]++;
                IPC_DBG_WRN("%s: pipe=%d, wrong sum2 pkt#%u, idx=%d, sum=%u, rawsum=%u, len=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                            __func__, pipe_id, ipc->test.rx_wrong_sum2_pkt_cnt[pipe_id-1],
                            pkt_idx, sum, ipc->test.rawsum[pipe_id-1][pkt_idx], skb->len,
                            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
                wq_ipc_dump_rx_info(ipc);
            } else {
                //pkt loss check
                if (ipc->test.lb_next_pkt_idx[pipe_id-1] != 0xFFFFFFFF) {
                    if (ipc->test.lb_next_pkt_idx[pipe_id-1] != pkt_idx) {
                        if (ipc->test.lb_pkt_loss_cnt[pipe_id-1] < WQ_IPC_LB_PKT_LOSS_LOG_SZ) {
                            ipc->test.lb_pkt_loss_idx[pipe_id-1][ipc->test.lb_pkt_loss_cnt[pipe_id-1]] = pkt_idx;
                        }

                        ipc->test.lb_pkt_loss_cnt[pipe_id-1]++;
                        IPC_DBG_ERR("%s: pkt loss !! pipe=%d, pkt idx=%d/%d, loss cnt=%u, len=%d\n",
                                    __func__, pipe_id, ipc->test.lb_next_pkt_idx[pipe_id-1],
                                    pkt_idx, ipc->test.lb_pkt_loss_cnt[pipe_id-1], skb->len);

                        wq_ipc_dump_rx_info(ipc);
                    }
                }

                //save pkt data
                len = (skb->len >= WQ_IPC_LB_PKT_DATA_LOG_SZ)? WQ_IPC_LB_PKT_DATA_LOG_SZ : skb->len;
                memset((void *)(&(ipc->test.lb_last_rx_pkt_data[pipe_id-1][0])),
                        0, WQ_IPC_LB_PKT_DATA_LOG_SZ);
                memcpy((void *)(&(ipc->test.lb_last_rx_pkt_data[pipe_id-1][0])),
                       (void *)(skb->data), len);

                //update next pkt index
                ipc->test.lb_next_pkt_idx[pipe_id-1] = (pkt_idx + ipc->test.tx_pipe_num) % WQ_IPC_LB_LOG_SZ;

                //update counter
                ipc->test.rx_pkt_cnt[pipe_id-1]++;

                if (!ipc->test.silent_test) {
                    IPC_DBG_INF("%s: pipe=%d, pkt#%u, idx=%d[next=%d], sum=%d, len=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                                __func__, pipe_id, ipc->test.rx_pkt_cnt[pipe_id-1],
                                pkt_idx, ipc->test.lb_next_pkt_idx[pipe_id-1], sum, skb->len,
                                p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
                }
            }
        }
    } else {
        p = (u8 *)(skb->data);
        sum = 0;
        for (j = 0; j < skb->len; j++) {
            sum += p[j];
        }
        ipc->test.rx_sum_total[0][0] += sum;
        ipc->test.rx_sum_total[0][1] += (p[0]+p[skb->len-1]);
        ipc->test.rx_len_total[0] += skb->len;
        ++ipc->test.rx_pkt_cnt[0];

        if (!ipc->test.silent_test) {
            IPC_DBG_INF("%s: pkt#%u, len=%d, sum=%d, sum_total=%llu(normal)/%llu(speed), raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                        __func__, ipc->test.rx_pkt_cnt[0], skb->len, sum, ipc->test.rx_sum_total[0][0], ipc->test.rx_sum_total[0][1],
                        p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
        }

        switch(ipc->test.rx_pkt_cnt[0] + ipc->test.rx_pkt_cnt[1])
        {
        case 1:
            ipc->test.rx_start_time = ktime_get();
            break;
        case 30:
        case 300:
        case 3000:
        case 30000:
        case 300000:
        case 3000000:
        case 30000000:
        case 300000000:
            ipc->test.rx_end_time = ktime_get();
            ipc->test.rx_diff_time = ktime_sub(ipc->test.rx_end_time, ipc->test.rx_start_time);
            ipc->test.rx_time_total = (u64)ktime_to_ms(ipc->test.rx_diff_time);

            wq_ipc_dump_test_info(ipc, 1);
            break;
        default:
            break;
        }
    }

    spin_unlock_irqrestore(&ipc->test.log_lock, flags);

free_buf:
    /* 3. free resource */
    WARN_ON(skb->next);
    dev_kfree_skb_any(skb);
}

extern void dump_bytes(char *note, uint8_t *data, int len);

/*
 * note:
 *  if return != 0, the caller should free skb
 */
int
wq_ipc_tx_pkt(void *wq_ipc, enum wq_ipc_rcvs receiver, enum wq_ipc_txq txq,
              u32 pkt_flags, void *buf, u8 *data, u32 len)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;

    IPC_DBG_ERR("%s: failed! In bus test mode!\n", __func__);

	//dump_bytes("wq_ipc_tx_pkt", data, len);

    if (ipc->mac_inf.wq_mac_tx_done) {
        ipc->mac_inf.wq_mac_tx_done(ipc->wq_mac, buf, 0);
    }

    return 0;
}

void
wq_ipc_rx_pkt(void *wq_ipc, struct sk_buff *skb, bool success)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    u8 *p = NULL;
    u32 sum;
    u32 j;
    unsigned long flags = 0;

    /* 1. check input */
	if (!skb) {
        IPC_DBG_OOPS("%s: null pointer, wq_ipc=0x%p, skb=0x%p\n", __func__, wq_ipc, skb);
		return;
	} else if (!wq_ipc) {
        IPC_DBG_OOPS("%s: null pointer, wq_ipc=0x%p, skb=0x%p\n", __func__, wq_ipc, skb);
		goto free_buf;
	} else if (ipc->test.loopback) {
        if (skb->len <= WQ_IPC_LB_PKT_HLEN) {
            spin_lock_irqsave(&ipc->test.log_lock, flags);
            ipc->test.rx_unknown_pkt_cnt[1]++;
            IPC_DBG_WRN("%s: unknown pkt#%u, len=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x...\n",
                        __func__, ipc->test.rx_unknown_pkt_cnt[1], skb->len,
                        p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            wq_ipc_dump_rx_info(ipc);
            spin_unlock_irqrestore(&ipc->test.log_lock, flags);

            goto free_buf;
        }
	}

    spin_lock_irqsave(&ipc->test.log_lock, flags);

    /* 2. data process */
    if (ipc->test.loopback) {
        u32 *phd = NULL;
        u32 pkt_idx=0, len=0;
        u8 pipe_id=0;

        phd = (u32 *)(skb->data);
        pipe_id = (phd[0] >> 24) & 0xFF;
        pkt_idx = phd[0] & 0xFFFFFF;

        p = (u8 *)(skb->data);
        p += WQ_IPC_LB_PKT_HLEN;
        sum = 0;
        for (j = 0; j < (skb->len - WQ_IPC_LB_PKT_HLEN); j++) {
            sum += p[j];
        }

        if ((wq_usb_is_pkt_out_pipe(pipe_id) == false)
            || (pkt_idx >= WQ_IPC_LB_LOG_SZ)) {
            ipc->test.rx_unknown_pkt_cnt[1]++;
            IPC_DBG_WRN("%s: unknown pkt#%u, pipe=%d, pkt_idx=%u, phd[1]=%u, sum=%u, len=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                        __func__, ipc->test.rx_unknown_pkt_cnt[1], pipe_id, pkt_idx, phd[1],
                        sum, skb->len, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
            wq_ipc_dump_rx_info(ipc);
        } else {
            if (sum != phd[1]) {
                ipc->test.rx_wrong_sum1_pkt_cnt[pipe_id-1]++;
                IPC_DBG_WRN("%s: pipe=%d, wrong sum1 pkt#%u, idx=%d, phd[1]=%u, sum=%u, len=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                            __func__, pipe_id, ipc->test.rx_wrong_sum1_pkt_cnt[pipe_id-1],
                            pkt_idx, phd[1], sum, skb->len,
                            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
                wq_ipc_dump_rx_info(ipc);
            } else if (sum != ipc->test.rawsum[pipe_id-1][pkt_idx]) {
                ipc->test.rx_wrong_sum2_pkt_cnt[pipe_id-1]++;
                IPC_DBG_WRN("%s: pipe=%d, wrong sum2 pkt#%u, idx=%d, sum=%u, rawsum=%u, len=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                            __func__, pipe_id, ipc->test.rx_wrong_sum2_pkt_cnt[pipe_id-1],
                            pkt_idx, sum, ipc->test.rawsum[pipe_id-1][pkt_idx], skb->len,
                            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
                wq_ipc_dump_rx_info(ipc);
            } else {
                //pkt loss check
                if (ipc->test.lb_next_pkt_idx[pipe_id-1] != 0xFFFFFFFF) {
                    if (ipc->test.lb_next_pkt_idx[pipe_id-1] != pkt_idx) {
                        if (ipc->test.lb_pkt_loss_cnt[pipe_id-1] < WQ_IPC_LB_PKT_LOSS_LOG_SZ) {
                            ipc->test.lb_pkt_loss_idx[pipe_id-1][ipc->test.lb_pkt_loss_cnt[pipe_id-1]] = pkt_idx;
                        }

                        ipc->test.lb_pkt_loss_cnt[pipe_id-1]++;
                        IPC_DBG_ERR("%s: pkt loss !! pipe=%d, pkt idx=%d/%d, loss cnt=%u, len=%d\n",
                                    __func__, pipe_id, ipc->test.lb_next_pkt_idx[pipe_id-1],
                                    pkt_idx, ipc->test.lb_pkt_loss_cnt[pipe_id-1], skb->len);

                        wq_ipc_dump_rx_info(ipc);
                    }
                }

                //save pkt data
                len = (skb->len >= WQ_IPC_LB_PKT_DATA_LOG_SZ)? WQ_IPC_LB_PKT_DATA_LOG_SZ : skb->len;
                memset((void *)(&(ipc->test.lb_last_rx_pkt_data[pipe_id-1][0])),
                        0, WQ_IPC_LB_PKT_DATA_LOG_SZ);
                memcpy((void *)(&(ipc->test.lb_last_rx_pkt_data[pipe_id-1][0])),
                       (void *)(skb->data), len);

                //update next pkt index
                ipc->test.lb_next_pkt_idx[pipe_id-1] = (pkt_idx + ipc->test.tx_pipe_num) % WQ_IPC_LB_LOG_SZ;

                //update counter
                ipc->test.rx_pkt_cnt[pipe_id-1]++;

                if (!ipc->test.silent_test) {
                    IPC_DBG_INF("%s: pipe=%d, pkt#%u, idx=%d[next=%d], sum=%d, len=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                                __func__, pipe_id, ipc->test.rx_pkt_cnt[pipe_id-1],
                                pkt_idx, ipc->test.lb_next_pkt_idx[pipe_id-1], sum, skb->len,
                                p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
                }
            }
        }
    } else {
        p = (u8 *)(skb->data);
        sum = 0;
        for (j = 0; j < skb->len; j++) {
            sum += p[j];
        }
        ipc->test.rx_sum_total[1][0] += sum;
        ipc->test.rx_sum_total[1][1] += (p[0]+p[skb->len-1]);
        ipc->test.rx_len_total[1] += skb->len;
        ++ipc->test.rx_pkt_cnt[1];

        if (!ipc->test.silent_test) {
            IPC_DBG_INF("%s: pkt#%u, len=%d, sum=%d, sum_total=%llu(normal)/%llu(speed), raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                        __func__, ipc->test.rx_pkt_cnt[1], skb->len, sum, ipc->test.rx_sum_total[1][0], ipc->test.rx_sum_total[1][1],
                        p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
        }

        switch(ipc->test.rx_pkt_cnt[0] + ipc->test.rx_pkt_cnt[1])
        {
        case 1:
            ipc->test.rx_start_time = ktime_get();
            break;
        case 30:
        case 300:
        case 3000:
        case 30000:
        case 300000:
        case 3000000:
        case 30000000:
        case 300000000:
            ipc->test.rx_end_time = ktime_get();
            ipc->test.rx_diff_time = ktime_sub(ipc->test.rx_end_time, ipc->test.rx_start_time);
            ipc->test.rx_time_total = (u64)ktime_to_ms(ipc->test.rx_diff_time);

            wq_ipc_dump_test_info(ipc, 1);
            break;
        default:
            break;
        }
    }

    spin_unlock_irqrestore(&ipc->test.log_lock, flags);

free_buf:
    /* 3. free resource */
#ifndef CFG_IPC_SIM_LOOPBACK
    WARN_ON(skb->next);
    dev_kfree_skb_any(skb);
#else
    if (success) {
        WARN_ON(skb->next);
        dev_kfree_skb_any(skb);
    } else {
        dev_kfree_skb_any(skb);
    }
#endif
}

void
wq_ipc_tx_pkt_cb(void *wq_ipc, u8 pipe_id, void *buf, void *data, int status)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    struct sk_buff *skb = (struct sk_buff *)buf;
#if 0
    u8 pipe_id;
    u32 *phd = NULL;
#endif

    /* 1. check input */
    if ((status) || (!skb) || (!wq_ipc) || (wq_usb_is_pkt_out_pipe(pipe_id) == false)) {
        IPC_DBG_WRN("%s: Error! (status=%d, skb=0x%p, ipc=0x%p, pipe_id=%d)\n",
                    __func__, status, skb, wq_ipc, pipe_id);
    }

    /* 2. update statistic */
#if 0
    if (ipc && skb) {
        phd = (u32 *)(skb->data);
        pipe_id = (phd[0] >> 24) & 0xFF;
#else
    if ((ipc) && (wq_usb_is_pkt_out_pipe(pipe_id) == true)) {
#endif
        if (status) {
            ipc->test.tx_done_result[pipe_id-1][1]++;
        } else {
            ipc->test.tx_done_result[pipe_id-1][0]++;
        }
     }

    /* 3. free skb */
    if (!skb)
		return;

#ifndef CFG_IPC_SIM_LOOPBACK
    dev_kfree_skb_any(skb);
#else
    wq_ipc_rx_pkt(ipc, skb, false);
#endif
}

/*************************************************************
 * Description
 *  - start ipc tx test.
 *  - test parameter setting function
 *      - step1: wq_ipc_test_pipe()
 *      - step2: wq_ipc_test_pattern()
 *      - step3: wq_ipc_test_loop()
 *      - step4: wq_ipc_test_pktlen_mode()
 *    , this function will be called in step4 to start tx test
 *  - test pattern
 *      - 0x1: 11111..., 22222..., 33333...,...
 *      - 0x2: 12345..., 23456..., 34567...,...
 *************************************************************/
void
wq_ipc_tx_test(void *wq_ipc)
{
#define WQ_IPC_STOP_WHEN_FAILED
#define WQ_IPC_WAIT_TXDONE_MAX_CNT  300

    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    struct sk_buff *skb;
    u8 infinite, pipe_id;
    u32 len, mtu_tx = 0, headroom, sum = 0, pkt_idx;
    u8 *p = NULL;
    u32 *phd = NULL;
    u32 i=0, j=0, rty=0;
    u32 wait_cnt=0;
    int ret = 0;
    unsigned long flags = 0;


    if (!ipc) {
        IPC_DBG_ERR("%s: ipc is null\n", __func__);
        return;
    }


    /* 1. tx test preparation */
    infinite = (ipc->test.loop)? 0:1;
    headroom = (ipc->test.loopback)? WQ_IPC_LB_PKT_HLEN : 0;

    //reset test log
    ipc->test.txs_pkt_total=0;
    ipc->test.txd_pkt_total=0;
    memset(ipc->test.tx_sum_total, 0, sizeof(ipc->test.tx_sum_total));
    ipc->test.tx_len_total = 0;
    memset(ipc->test.tx_result, 0, sizeof(ipc->test.tx_result));
    memset(ipc->test.tx_done_result, 0, sizeof(ipc->test.tx_done_result));
    memset(ipc->test.rawsum, 0, sizeof(ipc->test.rawsum));
    memset(ipc->test.lb_pkt_loss_cnt, 0, sizeof(ipc->test.lb_pkt_loss_cnt));
    memset(ipc->test.lb_next_pkt_idx, 0xFF, sizeof(ipc->test.lb_next_pkt_idx));
    memset(ipc->test.lb_last_rx_pkt_data, 0, sizeof(ipc->test.lb_last_rx_pkt_data));
    memset(ipc->test.rx_unknown_pkt_cnt, 0, sizeof(ipc->test.rx_unknown_pkt_cnt));
    memset(ipc->test.rx_wrong_sum1_pkt_cnt, 0, sizeof(ipc->test.rx_wrong_sum1_pkt_cnt));
    memset(ipc->test.rx_wrong_sum2_pkt_cnt, 0, sizeof(ipc->test.rx_wrong_sum2_pkt_cnt));

    if (ipc->test.loopback) {
        //avoid reset counter when do non-loopback bi-direction test
        memset(ipc->test.rx_pkt_cnt, 0, sizeof(ipc->test.rx_pkt_cnt));
    }

    IPC_DBG_INF("%s: pipe_num=%d, pipe=%d-%d-%d-%d-%d-%d-%d, pattern=%d, loop=%d, len=%d, pgmode=%d, headroom=%d\n",
                __func__, ipc->test.tx_pipe_num, ipc->test.tx_pipe[0], ipc->test.tx_pipe[1],
                ipc->test.tx_pipe[2], ipc->test.tx_pipe[3], ipc->test.tx_pipe[4],
                ipc->test.tx_pipe[5], ipc->test.tx_pipe[6], ipc->test.pattern, ipc->test.loop,
                ipc->test.pkt_len, ipc->test.pgmode, headroom);

    /* 2. start rx */
    if (ipc->test.loopback) {
        wq_usb_submit_in_urb(ipc->wq_dev);
    }

    /* 3. start tx test */
    ipc->test.tx_start_time = ktime_get();
    for (i = 0; (infinite || (i < ipc->test.loop)); i++) {
        // select pipe id
        pipe_id = ipc->test.tx_pipe[i % ipc->test.tx_pipe_num];

        // calculate pkt length
        if (ipc->test.pgmode) {
            len = (i % (ipc->test.pkt_len)) + 1 + headroom;
        } else {
            len = ipc->test.pkt_len + headroom;
        }

        // tx mtu check
        mtu_tx = wq_usb_get_mtu(ipc->wq_dev, pipe_id);
        len = (len > mtu_tx)? mtu_tx:len;

        ipc->test.tx_len_total += len;

        // payload preparation
        skb = __dev_alloc_skb(len, WQ_MEM_GFP);
        if (!skb) {
            IPC_DBG_ERR("%s: skb is null, i=%u\n", __func__, i);
    		goto report;
        }

        p = (u8 *) skb_put(skb, len);

        p += headroom;
        switch(ipc->test.pattern)
        {
        case 1:
            memset(p, (i & 0xFF), len-headroom);
            sum = (i & 0xFF) * (len-headroom);
            break;
        default:
            sum = 0;
            for (j = 0; j < (len-headroom); j++) {
                p[j] = (i+j) & 0xFF;
                sum += p[j];
            }
            break;
        }
        ipc->test.tx_sum_total[0] += sum;
        ipc->test.tx_sum_total[1] += (p[0]+p[len-headroom-1]);

        if (headroom) {
            phd = (u32 *)(skb->data);
            pkt_idx = i % WQ_IPC_LB_LOG_SZ;

            phd[0] = (pipe_id << 24) | pkt_idx;
            phd[1] = sum;

            //update raw sum
            spin_lock_irqsave(&ipc->test.log_lock, flags);
            ipc->test.rawsum[pipe_id-1][pkt_idx] = sum;
            spin_unlock_irqrestore(&ipc->test.log_lock, flags);

            if (!ipc->test.silent_test) {
                IPC_DBG_INF("%s: pkt#%u - skb=0x%p, pipe=%d, pkt_idx=%d, sum=%d, len=%d, raw=0x%x-%d | 0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                            __func__, i+1, skb, pipe_id, pkt_idx, sum, len, phd[0], phd[1],
                            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
            }
        } else {
            if (!ipc->test.silent_test) {
                IPC_DBG_INF("%s: pkt#%u - skb=0x%p, pipe=%d, len=%d, sum=%d, raw=0x%x-%x-%x-%x-%x-%x-%x-%x-%x-%x...\n",
                            __func__, i+1, skb, pipe_id, len, sum,
                            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
            }
        }

        rty = 0;
        do {
            if (ipc->state != WQ_IPC_STATE_UP) {
                IPC_DBG_WRN("%s: stop ipc tx test by state=%d\n", __func__, ipc->state);
                dev_kfree_skb_any(skb);
                goto report;
            }

            ret = wq_usb_tx(ipc->wq_dev, pipe_id, skb);
            if (ret) {
                ++rty;
                if (!ipc->test.silent_test) {
                    IPC_DBG_WRN("%s: tx urb failed, ret=%d, rty=%d, i=%u\n",
                                __func__, ret, rty, i);
                }

                ipc->test.tx_result[pipe_id-1][rty]++;

                if (rty > WQ_IPC_MAX_RETRY_NUM) {
                    IPC_DBG_ERR("%s: reach retry limit, stop test\n", __func__);
                    dev_kfree_skb_any(skb);
#ifdef WQ_IPC_STOP_WHEN_FAILED
                    goto report;
#else
                    break;
#endif
                } else {
                    if (ipc->test.silent_test) {
                        msleep(10);
                    } else {
                        msleep(200);
                    }
                }
            } else {
                ipc->test.tx_result[pipe_id-1][0]++;
            }
        } while(ret);

        if ((infinite) && (i == 0xFFFFFFFF)) {
            IPC_DBG_INF("%s: reach test log limit !\n", __func__);
            break;
        }
    }

    IPC_DBG_INF("%s: test done !\n", __func__);

report:
    // wait for all tx/rx urb complete
    wait_cnt = 0;
    do {
        ipc->test.txs_pkt_total=0;
        ipc->test.txd_pkt_total=0;

        spin_lock_irqsave(&ipc->test.log_lock, flags);
        for (i = 0; i < WQ_IPC_MAX_PIPE_NUM; i++) {
            ipc->test.txs_pkt_total += ipc->test.tx_result[i][0];
            ipc->test.txd_pkt_total += (ipc->test.tx_done_result[i][0] + ipc->test.tx_done_result[i][1]);
        }
        spin_unlock_irqrestore(&ipc->test.log_lock, flags);

        if (ipc->test.txd_pkt_total >= ipc->test.txs_pkt_total) {
            IPC_DBG_INF("%s: tx done - wait_cnt=%u, txs_pkt_total=%u, txd_pkt_total=%u\n",
                        __func__, wait_cnt, ipc->test.txs_pkt_total, ipc->test.txd_pkt_total);
            break;
        } else {
            IPC_DBG_INF("%s: wait tx done - wait_cnt=%u, txs_pkt_total=%u, txd_pkt_total=%u\n",
                        __func__, wait_cnt, ipc->test.txs_pkt_total, ipc->test.txd_pkt_total);
            msleep(1000);
        }
    } while(wait_cnt++ < WQ_IPC_WAIT_TXDONE_MAX_CNT);

    ipc->test.tx_end_time = ktime_get();
    ipc->test.tx_diff_time = ktime_sub(ipc->test.tx_end_time, ipc->test.tx_start_time);
    ipc->test.tx_time_total = (u64)ktime_to_ms(ipc->test.tx_diff_time);

    wq_ipc_dump_test_info(ipc, 0);
    wq_usb_dump_info(ipc->wq_dev);

    return;
}

void wq_ipc_tx_test_work(struct work_struct *work)
{
  	struct wq_ipc *ipc = container_of(work, struct wq_ipc, test.tx_work);

    IPC_DBG_INF("\n\n\n!!!!!!!!!!!!!!!!!!!!!!!! START TX TEST !!!!!!!!!!!!!!!!!!!!!!!!\n\n\n");

    wq_ipc_tx_test((void *)ipc);

}

/*************************************************************
 * Description
 *  - Set tx pipe
 *
 * Input
 *  - input
 *      - [1:7] bitmap for pipe 1 - 7
 *
 *    , refer to WQ_USB_EP_XXX for the value definition
 *************************************************************/
void wq_ipc_test_pipe(void *wq_ipc, u8 input)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    unsigned long bitmap = input;
    u8 i = 0, offset = 0;

    /* 1. bitmap check */
    //keep valid bit only
    bitmap &= 0xFE;

    //set as least one pipe for test
    bitmap = (bitmap == 0)? 0x2:bitmap;

    /* 2. get test pipe info */
    offset = 0;
    ipc->test.tx_pipe_num = bitmap_weight(&bitmap, 8);
    for (i = 0; i < ipc->test.tx_pipe_num; i++, offset++) {
        offset = find_next_bit(&bitmap, 8, offset);
        WARN_ON(offset == 8);

        ipc->test.tx_pipe[i] = offset;
    }

    IPC_DBG_INF("%s: input=0x%x, pipe_num=%d, pipe=%d-%d-%d-%d-%d-%d-%d\n", __func__, input,
                ipc->test.tx_pipe_num, ipc->test.tx_pipe[0], ipc->test.tx_pipe[1],
                ipc->test.tx_pipe[2], ipc->test.tx_pipe[3], ipc->test.tx_pipe[4],
                ipc->test.tx_pipe[5], ipc->test.tx_pipe[6]);
}

/*************************************************************
 * Description
 *  - Set pattern
 *
 * Input
 *  - input: tx pattern
 *    , the value definition could refer to wq_ipc_tx_test()
 *************************************************************/
void wq_ipc_test_pattern(void *wq_ipc, u8 input)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;

    ipc->test.pattern = (input == 0x1)? 0x1 : 0x2;

    IPC_DBG_INF("%s: pattern=%d\n", __func__, ipc->test.pattern);
}

/*************************************************************
 * Description
 *  - Set the number of pkt to send
 *
 * Input
 *  - input
 *      # of pket = input / 1000
 *      [special case] when num < 1000, do infinite loop test
 *************************************************************/
void wq_ipc_test_loop(void *wq_ipc, u32 input)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;

    ipc->test.loop = (input < 1000)? 0 : (input/1000);
    ipc->test.loop *= ipc->test.loop_multiple;

    IPC_DBG_INF("%s: input=%u, multiple=%u, loop=%u\n", __func__,
                input, ipc->test.loop_multiple, ipc->test.loop);
}

/*************************************************************
 * Description
 *  - set pkt length, test mode and start the tx test
 *
 * Input
 *  - input
 *      - [0:29] (max) pkt_len
 *      - [30] progressive mode, tx_pktlen = 1 ~ pkt_len
 *      - [31] loopback test
 *              - in loopback test, pkt will add 8 byte to save loopback data
 *************************************************************/
void wq_ipc_test_pktlen_mode(void *wq_ipc, u32 input)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;

    ipc->test.pkt_len = input & 0x3FFFFFFF;
    // avoid len = 0
    ipc->test.pkt_len = (ipc->test.pkt_len)? ipc->test.pkt_len : 1;

    ipc->test.pgmode = (input >> 30) & 0x1;
    ipc->test.loopback = (input >> 31) & 0x1;

    IPC_DBG_INF("%s: loopback=%d, pgmode=%d, pkt_len=%d\n", __func__,
                ipc->test.loopback, ipc->test.pgmode, ipc->test.pkt_len);

    schedule_work(&ipc->test.tx_work);
}

void wq_ipc_test_ex(void *wq_ipc, u8 id)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;

    IPC_DBG_INF("%s: id=%d\n", __func__, id);

    switch (id)
    {
    case 0:
        IPC_DBG_INF("\n\n\n!!!!!!!!!!!!!!!!!!!!!!!! START RX TEST !!!!!!!!!!!!!!!!!!!!!!!!\n\n\n");

        //reset rx result
        memset(ipc->test.rx_pkt_cnt, 0, sizeof(ipc->test.rx_pkt_cnt));
        memset(ipc->test.rx_sum_total, 0, sizeof(ipc->test.rx_sum_total));
        memset(ipc->test.rx_len_total, 0, sizeof(ipc->test.rx_len_total));

        wq_usb_submit_in_urb(ipc->wq_dev);
        break;
    case 1:
        IPC_DBG_INF("\n\n\n!!!!!!!!!!!!!!!!!!!!!!!! Test Log !!!!!!!!!!!!!!!!!!!!!!!!\n");

        wq_ipc_dump_test_info(ipc, 0);
        wq_usb_dump_info(ipc->wq_dev);
        break;
    case 2:
        ipc->test.silent_test = 0;
        IPC_DBG_INF("%s: silent_test=%d\n", __func__, ipc->test.silent_test);
        break;
    case 3:
        ipc->test.silent_test = 1;
        IPC_DBG_INF("%s: silent_test=%d\n", __func__, ipc->test.silent_test);
        break;
    case 4:
        ipc->test.loop_multiple = 1;
        IPC_DBG_INF("%s: loop_multiple=%u\n", __func__, ipc->test.loop_multiple);
        break;
    case 5:
        ipc->test.loop_multiple *= 10;
        IPC_DBG_INF("%s: loop_multiple=%u\n", __func__, ipc->test.loop_multiple);
        break;
    }
}
#endif

#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
void wq_sche_pktin_work_ex(void * wq_ipc)
{
    struct wq_ipc *ipc = (struct wq_ipc *)wq_ipc;
    wq_sche_pktin_work(ipc->wq_dev);
}
#endif


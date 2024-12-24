/****************************
 * Include
 ****************************/
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/module.h>
#include <linux/firmware.h>
#include <linux/vmalloc.h>
#include <linux/usb.h>
#include <linux/version.h>

#include "usb.h"
#include "wq_ipc.h"

#ifdef WQ_BUS_TEST
#include "wq_usb_fw.h"
#else
#include "wq_wifi_fw.h"
#endif

#include "../wifi_drv.h"


/****************************
 * USB Configuration
 ****************************/
// EP type
#define WQ_USB_EPTYPE_CTRL  0x00
#define WQ_USB_EPTYPE_ISOC  0x01

// EP number
#define WQ_USB_NUM_OF_EP    7

#define WQ_USB_EP_RX        1
#if WQ_USB_FIFO_NUM == 5
#define WQ_USB_EP_TX_BE     3
#define WQ_USB_EP_TX_VI     1
#else
#define WQ_USB_EP_TX_BE     1
#define WQ_USB_EP_TX_VI     4
#endif
#define WQ_USB_EP_TX_BK     3
#define WQ_USB_EP_TX_VO     5
#define WQ_USB_EP_MSG_I     2
#if WQ_MSG_OUT_EP_TYPE == WQ_USB_EPTYPE_INT
#define WQ_USB_EP_MSG_O     2
#else
#define WQ_USB_EP_MSG_O     3
#endif

// URB number
#ifdef WQ_BUS_TEST
#define WQ_MSGOUT_URB_NUM   170
#define WQ_MSGIN_URB_NUM    50
#define WQ_BULKRX_URB_NUM   50
#define WQ_BULKTX_URB_NUM   1000
#else
#define WQ_MSGOUT_URB_NUM   3
#define WQ_MSGIN_URB_NUM    10
#if WQ_MSG_IN_EP_TYPE == WQ_USB_EPTYPE_INT
#define WQ_BULKRX_URB_NUM   10
#else
#define WQ_BULKRX_URB_NUM   50
#endif
#define WQ_BULKTX_URB_NUM   40
#endif

// MTU
#define WQ_USB_MTU_MSG      1024
#define WQ_USB_MTU_PKT      1664    //2048

// Vendor Command Id
#define WQ_VREQ_ID_FW_DL        0xAA    //usb vendor request id for firmware download
#define WQ_VREQ_ID_FW_DL_COMP   0xAB    //usb vendor request id for firmware download complete
#define WQ_VREQ_ID_GET_DSTATE   0xBA    //usb vendor request id for device state check
#define WQ_VREQ_ID_GET_ROM_VER  0xCA    //usb vendor request id for get rom version
#define WQ_VREQ_ID_SET_RESET    0xDA    //usb vender request id for set soc reset
#define WQ_USB_RESET_VALUE      0xABCD  //usb vender request id for set reset value
#define WQ_USB_RESET_INDEX      0x1234  //usb vender request id for set reset index

// Firmware download
#define WQ_USB_FW_DL_EPID       1
#define WQ_USB_FW_DL_MTU        10232       //size of each firmware download transfer
                                            //fw dl mtu +  sizeof(struct wq_usb_fw_dl_tag) must be
                                            //4 byte alignment. (usb dma address rule)
#define WQ_USB_FW_FILE_NAME     "wq_wifi.fw"

#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
extern int rx_skb_count;
int submit_rxurb_limit_h = 50;
int submit_rxurb_limit_l = 20;
int flag_rxurb_irq = 1;
#endif

#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
bool reset_down_flag = false;
bool wq_wifi_detach_flag = false;
bool wq_wifi_detach_end_flag = false;
#endif

enum wq_usb_vreq_dstate {
	WQ_USB_DSTATE_TRANSFER = 0,
	WQ_USB_DSTATE_READY,
	WQ_USB_DSTATE_ID_ERR,
	WQ_USB_DSTATE_CHKSUM_ERR,
	WQ_USB_DSTATE_MAX
};

struct wq_usb_ep_cfg {
    int num;    //endpoint number
    int type;   //endpoint type(0:Control, 1:Isochronous, 2:Bulk, 3:Interrupt)
    int dir;    //endpont direction(0:Out, 1:In)
};

struct wq_usb_vreq_get_dstate {
    u8 state;   //0: not ready, 1: ready
} __attribute__((__packed__));

struct wq_usb_vreq_get_rom_ver {
    u8 major;
    u8 minor;
    u8 build_hr;
    u8 build_min;
} __attribute__((__packed__));

struct wq_usb_fw_dl_tag {
    u16 id;         //index
    u16 checksum;   //checksum
} __attribute__((__packed__));


/****************************
 * USB debug
 ****************************/
extern u16 wq_dbg_flag;
extern bool wq_drv_rmmod_flag;
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

#define WQ_LOG_USB_DBG ((wq_dbg_flag & WQ_DEBUG_USB)? WQ_LOG_USB_SEV : DBG_SEV_NONE)
#define UDBG_VRB(fmt, ...)                  \
    do {                                    \
        if (WQ_LOG_USB_DBG > DBG_SEV_INF) {\
            printk(fmt, ##__VA_ARGS__);     \
        }                                   \
    } while (0)

#define UDBG_INF(fmt, ...)                  \
    do {                                    \
        if (WQ_LOG_USB_DBG > DBG_SEV_WRN) {\
            printk(fmt, ##__VA_ARGS__);     \
        }                                   \
    } while (0)

#define UDBG_WRN(fmt, ...)                  \
    do {                                    \
        if (WQ_LOG_USB_DBG > DBG_SEV_ERR) {\
            printk(fmt, ##__VA_ARGS__);     \
        }                                   \
    } while (0)

#define UDBG_ERR(fmt, ...)                      \
    do {                                        \
        if (WQ_LOG_USB_DBG > DBG_SEV_OOPS) {   \
            printk(fmt, ##__VA_ARGS__);         \
        }                                       \
    } while (0)

#define UDBG_OOPS(fmt, ...)                     \
    do {                                        \
        if (WQ_LOG_USB_DBG > DBG_SEV_NONE) {   \
            printk(fmt, ##__VA_ARGS__);         \
            BUG();                              \
        }                                       \
    } while (0)


/****************************
 * USB module structure
 ****************************/
enum wq_usb_state {
	WQ_USB_STATE_DOWN,
	WQ_USB_STATE_DL_FAIL,
	WQ_USB_STATE_DL_DONE,
	WQ_USB_STATE_UP
};

struct wq_usbreq {
	struct list_head list;
	struct wq_udev *udev;
	struct urb *urb;
	struct sk_buff *skb;
    void *buf;
    void *data;
    u8 pipe_id;
};

struct wq_udev {
    //module
	struct usb_device *usbdev;
	struct device *dev;
    void *wq_ipc;

    //urb resource
	u32 msg_mtu;
	u32 pkt_mtu;

	spinlock_t qlock;
#if WQ_MSG_IN_EP_TYPE == WQ_USB_EPTYPE_INT
    struct list_head msgin_freeq;
    struct list_head msgin_postq;
#endif
	struct list_head pktin_freeq;
	struct list_head pktin_postq;
    struct list_head pktin_workq;
	struct list_head pktout_freeq;
	struct list_head pktout_postq;

	int pktout_freecount;
    int pktin_freecount;
    int msgin_freecount;
    atomic_t msgout_cnt;

	struct wq_usbreq *msgin_reqs;
	struct wq_usbreq *pktout_reqs;
	struct wq_usbreq *pktin_reqs;

    struct usb_anchor msgout_anchor;

    //firmware resource
    const u8 *fw;
    int fw_len;

    //status, sync, work...etc
    enum wq_usb_state state;

    struct completion op_dev_ready;
	struct completion dev_init_lock;

    struct work_struct trigger_work;
    struct work_struct pkt_in_work;
    u8 trigger_cnt;
    u16 trigger_type;
    u16 trigger_info;
};

static struct wq_udev *g_udev = NULL;

/****************************
 * USB ID list
 ****************************/
static struct usb_device_id wq_usb_id_tbl[] = {
    /* support list (Vendor ID, Product ID) */
    { USB_DEVICE(0x0FFE, 0x0001) },
	{ },
};

MODULE_DEVICE_TABLE(usb, wq_usb_id_tbl);


/*****************************
 * Function
 *****************************/
extern void dump_bytes(char *note, uint8_t *data, int len);
extern int wq_mp_cali_free();
extern void set_rate_power(int8_t *power_list);
extern int8_t init_rate_pwr_list[12];
extern u32 *wq_mem_alloc(int len, int zero);
extern void indicate_wq_stop_hawk();

#if WQ_MSG_IN_EP_TYPE == WQ_USB_EPTYPE_INT
static void wq_usb_msg_in(struct wq_udev *udev, struct wq_usbreq *req);
#endif
static void wq_usb_pkt_in(struct wq_udev *udev, struct wq_usbreq *req);

void wq_usb_dump_info(void *wq_dev)
{
    struct wq_udev *udev = (struct wq_udev *)wq_dev;

    UDBG_INF("%s: pktout_freecount=%d, pktin_freecount=%d, msgin_freecount=%d, msgout_cnt=%d\n",
             __func__, udev->pktout_freecount, udev->pktin_freecount, udev->msgin_freecount,
             atomic_read(&(udev->msgout_cnt)));
}

u32 wq_usb_get_pkt_mtu(void *wq_dev)
{
    struct wq_udev *udev = (struct wq_udev *)wq_dev;

    return udev->pkt_mtu;
}

u32 wq_usb_get_msg_mtu(void *wq_dev)
{
    struct wq_udev *udev = (struct wq_udev *)wq_dev;

    return udev->msg_mtu;
}

static void
wq_usb_set_state(struct wq_udev *udev, int state)
{
	UDBG_INF("%s: cur=%d -> new=%d\n", __func__, udev->state, state);

	if (udev->state == state)
		return;

	udev->state = state;
}

void wq_usb_set_dev_ready(void *wq_dev)
{
    struct wq_udev *udev = (struct wq_udev *)wq_dev;

    complete(&udev->op_dev_ready);
}

void wq_usb_skb_free(struct sk_buff *skb)
{
	if (!skb)
		return;

	WARN_ON(skb->next);
    dev_kfree_skb_any(skb);
}

static struct wq_usbreq *
wq_usb_deq(struct wq_udev *udev, struct list_head *q, int *counter)
{
	unsigned long flags;
	struct wq_usbreq *req;

	spin_lock_irqsave(&udev->qlock, flags);

    if (list_empty(q)) {
		spin_unlock_irqrestore(&udev->qlock, flags);
		return NULL;
	}

    req = list_entry(q->next, struct wq_usbreq, list);
	list_del_init(q->next);

	if (counter)
		(*counter)--;

	spin_unlock_irqrestore(&udev->qlock, flags);

	return req;
}

static void
wq_usb_enq(struct wq_udev *udev, struct list_head *q, struct wq_usbreq *req,
           int *counter)
{
	unsigned long flags;

	spin_lock_irqsave(&udev->qlock, flags);

	list_add_tail(&req->list, q);

    if (counter)
		(*counter)++;

    spin_unlock_irqrestore(&udev->qlock, flags);
}

static void
wq_usb_del_fromq(struct wq_udev *udev, struct wq_usbreq *req)
{
	unsigned long flags;

	spin_lock_irqsave(&udev->qlock, flags);
	list_del_init(&req->list);
	spin_unlock_irqrestore(&udev->qlock, flags);
}

static void
wq_usb_pkt_in_cb(struct urb *urb)
{
	struct wq_usbreq *req = (struct wq_usbreq *)urb->context;
	struct wq_udev *udev = req->udev;
	struct sk_buff *skb;

    UDBG_VRB("%s: status=%d, actual_length=%d\n", __func__, urb->status, urb->actual_length);

	wq_usb_del_fromq(udev, req);
	skb = req->skb;
	req->skb = NULL;

	if (urb->status != 0 || !urb->actual_length) {
        UDBG_WRN("%s: Error! status=%d, actual_length=%d\n", __func__,
                 urb->status, urb->actual_length);

        if (urb->actual_length > udev->pkt_mtu) {
            udev->trigger_cnt++;
            udev->trigger_type = WQ_USB_TRI_URB_IN_LEN;
            udev->trigger_info = WQ_USB_EP_RX;

            UDBG_WRN("%s: cnt=%d, type=%d, info=%d\n", __func__,
                     udev->trigger_cnt, udev->trigger_type, udev->trigger_info);
            schedule_work(&udev->trigger_work);

            //dump_bytes("wq_usb_pkt_in_cb", skb->data, urb->actual_length);
        }

		wq_usb_skb_free(skb);
		wq_usb_enq(udev, &udev->pktin_freeq, req, &udev->pktin_freecount);

		return;
	}

	if (udev->state == WQ_USB_STATE_UP) {
        UDBG_VRB("%s: receive length=%d pkt\n", __func__, urb->actual_length);
        skb_put(skb, urb->actual_length);
        if (wq_ipc_check_pkt_type(skb)) {
            wq_ipc_rx_pkt(udev->wq_ipc, skb, true);
        } else {
            wq_ipc_msg_in(udev->wq_ipc, skb, true);
        }

        wq_usb_pkt_in(udev, req);
	} else {
        UDBG_WRN("%s: Error! dev state=%d, urb status=%d, actual_length=%d\n", __func__,
                 udev->state, urb->status, urb->actual_length);
		wq_usb_skb_free(skb);
		wq_usb_enq(udev, &udev->pktin_freeq, req, &udev->pktin_freecount);
	}

	return;
}


#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU

static void
wq_usb_pkt_in_submit(struct wq_udev *udev, struct wq_usbreq *req)
{
    struct sk_buff *skb;
    int ret;
#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
    static int submit_fail_cnt = 0;
#endif

    if (!req || !udev)
        return;

    //+1: avoid receive zero length pkt
    skb = dev_alloc_skb(udev->pkt_mtu+1);
    if (!skb) {
        wq_usb_enq(udev, &udev->pktin_freeq, req, &udev->pktin_freecount);
        return;
    }

    req->skb = skb;
    req->udev = udev;
    wq_usb_enq(udev, &udev->pktin_postq, req, NULL);

    usb_fill_bulk_urb(req->urb, udev->usbdev,
                      usb_rcvbulkpipe(udev->usbdev, WQ_USB_EP_RX),
                      skb->data, skb_tailroom(skb),
                      wq_usb_pkt_in_cb, req);

    ret = usb_submit_urb(req->urb, GFP_ATOMIC);
    if (ret) {
        wq_usb_del_fromq(udev, req);
        wq_usb_skb_free(req->skb);
        req->skb = NULL;
#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
        if (abs(ret) == 19) {
            submit_fail_cnt++;
            if (submit_fail_cnt >= 5) {
                reset_down_flag = true;
            }
            UDBG_INF("%s: usb dev removed\n", __func__);
        }
        if (abs(ret) != 19 && submit_fail_cnt != 0) {
            submit_fail_cnt = 0;
        }
#endif

        wq_usb_enq(udev, &udev->pktin_freeq, req, &udev->pktin_freecount);
    }

    return;
}

void wq_sche_pktin_work(void *wq_dev)
{
    struct wq_usbreq *req;
    struct wq_udev *udev = (struct wq_udev *)wq_dev;

    flag_rxurb_irq = 0;
#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192) 
    while ((req = wq_usb_deq(udev, &udev->pktin_freeq, &udev->pktin_freecount)) != NULL){
        wq_usb_pkt_in_submit(udev, req);
        if (rx_skb_count >= submit_rxurb_limit_h || reset_down_flag) { 
            break;
        }
    }
#else
    while ((req = wq_usb_deq(udev, &udev->pktin_freeq, &udev->pktin_freecount)) != NULL){
        wq_usb_pkt_in_submit(udev, req);
        if (rx_skb_count >= submit_rxurb_limit_h) {
            break;
        }
    }
#endif
    flag_rxurb_irq = 1;

    return;
}

static void
wq_usb_pkt_in(struct wq_udev *udev, struct wq_usbreq *req)
{
    if (!req || !udev)
        return;

    if (rx_skb_count <= submit_rxurb_limit_h && flag_rxurb_irq) {
        wq_usb_pkt_in_submit(udev, req);
    } else {
        wq_usb_enq(udev, &udev->pktin_freeq, req, &udev->pktin_freecount);
    }

    return;
}

#else

static void wq_usb_pkt_in_work(struct work_struct *work_arg)
{
    int ret;
    struct sk_buff *skb;
    struct wq_usbreq *req;
    struct wq_udev *udev = container_of(work_arg, struct wq_udev, pkt_in_work);

    while ((req = wq_usb_deq(udev, &udev->pktin_workq, NULL)) != NULL){

        if (!req || !udev){
            return;
        }

        //+1: avoid receive zero length pkt
        skb = __dev_alloc_skb(udev->pkt_mtu+1, WQ_MEM_GFP);
        if (!skb) {
            wq_usb_enq(udev, &udev->pktin_freeq, req, &udev->pktin_freecount);
            return;
        }

        req->skb = skb;
        req->udev = udev;
        wq_usb_enq(udev, &udev->pktin_postq, req, NULL);

        usb_fill_bulk_urb(req->urb, udev->usbdev,
                          usb_rcvbulkpipe(udev->usbdev, WQ_USB_EP_RX),
                          skb->data, skb_tailroom(skb),
                          wq_usb_pkt_in_cb, req);

        ret = usb_submit_urb(req->urb, WQ_MEM_GFP);
        if (ret) {
            wq_usb_del_fromq(udev, req);
            wq_usb_skb_free(req->skb);
            req->skb = NULL;

            wq_usb_enq(udev, &udev->pktin_freeq, req, &udev->pktin_freecount);
        }
    }
}


static void
wq_usb_pkt_in(struct wq_udev *udev, struct wq_usbreq *req)
{
    if (!req || !udev)
        return;

    wq_usb_enq(udev, &udev->pktin_workq, req, NULL);
    schedule_work(&udev->pkt_in_work);

	return;
}

#endif


#if WQ_MSG_IN_EP_TYPE == WQ_USB_EPTYPE_INT
static void
wq_usb_msg_in_cb(struct urb *urb)
{
	struct wq_usbreq  *req = (struct wq_usbreq *)urb->context;
	struct wq_udev *udev = req->udev;
	struct sk_buff *skb;

    UDBG_VRB("%s: status=%d, actual_length=%d\n", __func__, urb->status, urb->actual_length);

	wq_usb_del_fromq(udev, req);
	skb = req->skb;
	req->skb = NULL;

	if (urb->status != 0 || !urb->actual_length) {
        UDBG_WRN("%s: Error! status=%d, actual_length=%d\n", __func__,
                 urb->status, urb->actual_length);

        if (urb->actual_length > udev->msg_mtu) {
            udev->trigger_cnt++;
            udev->trigger_type = WQ_USB_TRI_URB_IN_LEN;
            udev->trigger_info = WQ_USB_EP_MSG_I;

            UDBG_WRN("%s: cnt=%d, type=%d, info=%d\n", __func__,
                     udev->trigger_cnt, udev->trigger_type, udev->trigger_info);
            schedule_work(&udev->trigger_work);

            //dump_bytes("wq_usb_msg_in_cb", skb->data, urb->actual_length);
        }

		wq_usb_skb_free(skb);
		wq_usb_enq(udev, &udev->msgin_freeq, req, &udev->msgin_freecount);

		return;
	}

	if (udev->state == WQ_USB_STATE_UP) {
        UDBG_VRB("%s: receive length=%d pkt\n", __func__, urb->actual_length);
        skb_put(skb, urb->actual_length);
        wq_ipc_msg_in(udev->wq_ipc, skb, true);

        wq_usb_msg_in(udev, req);
	} else {
        UDBG_WRN("%s: Error! dev state=%d, urb status=%d, actual_length=%d\n", __func__,
                 udev->state, urb->status, urb->actual_length);
		wq_usb_skb_free(skb);
		wq_usb_enq(udev, &udev->msgin_freeq, req, &udev->msgin_freecount);
	}

	return;
}

static void
wq_usb_msg_in(struct wq_udev *udev, struct wq_usbreq *req)
{
	struct sk_buff *skb;
	int ret;

	if (!req || !udev)
		return;

	skb = __dev_alloc_skb(udev->msg_mtu, WQ_MEM_GFP);
	if (!skb) {
        UDBG_WRN("%s: Error! allocate skb failed!\n", __func__);
		wq_usb_enq(udev, &udev->msgin_freeq, req, &udev->msgin_freecount);
		return;
	}

	req->skb = skb;
    req->udev = udev;
    wq_usb_enq(udev, &udev->msgin_postq, req, NULL);

	usb_fill_int_urb(req->urb, udev->usbdev,
                     usb_rcvintpipe(udev->usbdev, WQ_USB_EP_MSG_I),
                     skb->data, skb_tailroom(skb),
                     wq_usb_msg_in_cb, req, 1);

	ret = usb_submit_urb(req->urb, GFP_ATOMIC);
	if (ret) {
        UDBG_WRN("%s: Error! submit urb failed (%d)!\n", __func__, ret);
		wq_usb_del_fromq(udev, req);
		wq_usb_skb_free(req->skb);
		req->skb = NULL;

        wq_usb_enq(udev, &udev->msgin_freeq, req, &udev->msgin_freecount);
	}

	return;
}
#endif

static void
wq_usb_pkt_out_cb(struct urb *urb)
{
	struct wq_usbreq *req = (struct wq_usbreq *)urb->context;
	struct wq_udev *udev = req->udev;

    UDBG_VRB("%s: pipe_id=%d, buf=%p, status=%d\n", __func__, req->pipe_id, req->buf, urb->status);

	wq_usb_del_fromq(udev, req);

    wq_ipc_tx_pkt_cb(udev->wq_ipc, req->pipe_id, req->buf, req->data, urb->status);

    req->buf = NULL;
	wq_usb_enq(udev, &udev->pktout_freeq, req, &udev->pktout_freecount);
}

int
wq_usb_pkt_out(void *wq_dev, enum wq_usb_txq txq, void *buf, u8 *data, u32 len)
{
    struct wq_udev *udev = (struct wq_udev *)wq_dev;
    struct wq_usbreq *req;
	int ret;
    u8 ep_map[] = {WQ_USB_EP_TX_BE, WQ_USB_EP_TX_BK, WQ_USB_EP_TX_VI, WQ_USB_EP_TX_VO};
    u8 ep_id;

    UDBG_VRB("%s: txq=%d, buf=0x%p, len=%d\n", __func__, txq, buf, len);

    // 1. check input
    if ((!udev) || (!buf) || (!data)) {
        UDBG_ERR("%s: dev=0x%p, buf=0x%p, data=0x%p\n", __func__, udev, buf, data);
		ret = -EIO;
		goto fail;
	}

    if ((udev->state != WQ_USB_STATE_UP)
        || (len > wq_usb_get_pkt_mtu(wq_dev))
        || (len == 0)) {
        UDBG_ERR("%s: state=%d, len=%d\n", __func__, udev->state, len);
        ret = -EINVAL;
        goto fail;
    }

    // 2. urb preparation
	req = wq_usb_deq(udev, &udev->pktout_freeq, &udev->pktout_freecount);
	if (!req) {
#ifdef WQ_BUS_TEST
        //for throughput silent test
        return -ENOMEM;
#else
		UDBG_WRN("%s: no req to send\n", __func__);
		ret = -ENOMEM;
		goto fail;
#endif
	}

    ep_id = ep_map[txq];

	req->udev = udev;
    req->pipe_id = ep_id;
	req->buf = buf;
    req->data = data;

	usb_fill_bulk_urb(req->urb, udev->usbdev,
                      usb_sndbulkpipe(udev->usbdev, ep_id),
                      data, len,
                      wq_usb_pkt_out_cb, req);
	req->urb->transfer_flags |= URB_ZERO_PACKET;

	wq_usb_enq(udev, &udev->pktout_postq, req, NULL);

    // 3. submit urb
    ret = usb_submit_urb(req->urb, GFP_ATOMIC);
	if (ret) {
		UDBG_ERR("%s: usb_submit_urb FAILED\n", __func__);

		wq_usb_del_fromq(udev, req);
		req->buf = NULL;
		wq_usb_enq(udev, &udev->pktout_freeq, req,
			       &udev->pktout_freecount);
		goto fail;
	}

	return 0;

fail:
    UDBG_ERR("%s: send pkt failed (%d)\n", __func__, ret);

	return ret;
}

static void
wq_usb_msg_out_cb(struct urb *urb)
{
	struct wq_usbreq *req = (struct wq_usbreq *)urb->context;

    if (req) {
        struct wq_udev *udev = req->udev;

        UDBG_VRB("%s: pipe_id=%d, skb=0x%p, status=%d\n", __func__, req->pipe_id, req->skb, urb->status);

        atomic_dec(&(udev->msgout_cnt));

        wq_ipc_cmd_out_cb(udev->wq_ipc, req->pipe_id, req->skb, urb->status);

        kfree(req);
    } else {
        UDBG_OOPS("%s: urb context is null !, status=%d\n", __func__, urb->status);
    }
}

int
wq_usb_msg_out(void *wq_dev, struct sk_buff *skb)
{
    struct wq_udev *udev = (struct wq_udev *)wq_dev;
	struct urb *urb;
	struct wq_usbreq *req;
	int ret = 0;
    int submit_cnt = atomic_read(&(udev->msgout_cnt));

    UDBG_VRB("%s: skb=0x%p, skb->len=%d\n", __func__, skb, skb->len);

    // 1. check input
	if ((!udev) || (!skb)) {
        UDBG_ERR("%s: Error! dev=0x%p, skb=0x%p\n", __func__, udev, skb);
		ret = -EIO;
		goto fail;
	}

    if ((udev->state != WQ_USB_STATE_UP)
        || (skb->len > wq_usb_get_msg_mtu(wq_dev))
        || (skb->len == 0)) {
        UDBG_ERR("%s: Error! state=%d, len=%d\n", __func__, udev->state, skb->len);
        ret = -EINVAL;
        goto fail;
    }

    if (submit_cnt > WQ_MSGOUT_URB_NUM) {
#ifdef WQ_BUS_TEST
        //for throughput silent test
        return -ENOSPC;
#else
        UDBG_ERR("%s: submit_cnt=%d\n", __func__, submit_cnt);
        ret = -ENOSPC;
        goto fail;
#endif
    }

    // 2. urb preparation
	urb = usb_alloc_urb(0, GFP_ATOMIC);
	if (urb == NULL) {
        UDBG_ERR("%s: allocate urb failed !\n", __func__);
		ret = -ENOMEM;
        goto fail;
	}

	req = kzalloc(sizeof(*req), GFP_ATOMIC);
	if (req == NULL) {
		usb_free_urb(urb);

        UDBG_ERR("%s: allocate memory failed !\n", __func__);
		ret = -ENOMEM;
        goto fail;
	}

	req->skb = skb;
	req->udev = udev;
    req->pipe_id = WQ_USB_EP_MSG_O;

#if WQ_MSG_OUT_EP_TYPE == WQ_USB_EPTYPE_BULK
    usb_fill_bulk_urb(urb, udev->usbdev,
                      usb_sndbulkpipe(udev->usbdev, WQ_USB_EP_MSG_O),
                      skb->data, skb->len,
                      wq_usb_msg_out_cb, req);
    urb->transfer_flags |= URB_ZERO_PACKET;
#else
     usb_fill_int_urb(urb, udev->usbdev,
                     usb_sndintpipe(udev->usbdev, WQ_USB_EP_MSG_O),
                     skb->data, skb->len,
                     wq_usb_msg_out_cb, req, 1);
#endif

    // 3. submit urb
    atomic_inc(&(udev->msgout_cnt));
	usb_anchor_urb(urb, &udev->msgout_anchor);
	ret = usb_submit_urb(urb, GFP_ATOMIC);
	if (ret) {
        UDBG_ERR("%s: usb_submit_urb FAILED\n", __func__);
        atomic_dec(&(udev->msgout_cnt));
		usb_unanchor_urb(urb);
		kfree(req);
	}
	usb_free_urb(urb);

	return ret;

fail:
    UDBG_ERR("%s: send message failed (%d)\n", __func__, ret);

    return ret;
}

#ifdef WQ_BUS_TEST
int wq_usb_tx(void *wq_dev, u8 pipe_id, struct sk_buff *skb)
{
    int ret = 0;

    switch (pipe_id)
    {
    case WQ_USB_EP_MSG_O:
        ret = wq_usb_msg_out(wq_dev, skb);
        break;
    case WQ_USB_EP_TX_BE:
        ret = wq_usb_pkt_out(wq_dev, WQ_USB_TXQ_BE, (void *)skb, skb->data, skb->len);
        break;
    case WQ_USB_EP_TX_BK:
        ret = wq_usb_pkt_out(wq_dev, WQ_USB_TXQ_BK, (void *)skb, skb->data, skb->len);
        break;
    case WQ_USB_EP_TX_VI:
        ret = wq_usb_pkt_out(wq_dev, WQ_USB_TXQ_VI, (void *)skb, skb->data, skb->len);
        break;
    case WQ_USB_EP_TX_VO:
        ret = wq_usb_pkt_out(wq_dev, WQ_USB_TXQ_VO, (void *)skb, skb->data, skb->len);
        break;
    default:
        UDBG_ERR("%s: wrong pipe id (%d)\n", __func__, pipe_id);
        ret = -EINVAL;
        break;
    }

    return ret;
}

bool wq_usb_is_msg_out_pipe(u8 pipe_id)
{
    if (pipe_id == WQ_USB_EP_MSG_O) {
        return true;
    }

    return false;
}

bool wq_usb_is_pkt_out_pipe(u8 pipe_id)
{
    switch (pipe_id)
    {
    case WQ_USB_EP_TX_BE:
    case WQ_USB_EP_TX_BK:
    case WQ_USB_EP_TX_VI:
    case WQ_USB_EP_TX_VO:
        return true;
        break;
    }

    return false;
}

u32 wq_usb_get_mtu(void *wq_dev, u8 pipe_id)
{
    u32 ret = 0;

    switch (pipe_id)
    {
    case WQ_USB_EP_MSG_O:
        ret = wq_usb_get_msg_mtu(wq_dev);
        break;
    case WQ_USB_EP_TX_BE:
    case WQ_USB_EP_TX_BK:
    case WQ_USB_EP_TX_VI:
    case WQ_USB_EP_TX_VO:
        ret = wq_usb_get_pkt_mtu(wq_dev);
        break;
    default:
        UDBG_ERR("%s: wrong pipe id (%u)\n", __func__, pipe_id);
        ret = 0;
        break;
    }

    return ret;
}
#endif

void wq_usb_send_trigger_pattern(void *wq_dev, u16 type, u16 info)
{
    struct wq_udev *udev = (struct wq_udev *)wq_dev;
  	int err = 0;
    u16 len = 0;
    u32 i = 0;
    u32 *data = NULL;

	UDBG_INF(WQ_FN_ENTRY_STR);

    //1. preparation
    data = wq_mem_alloc(sizeof(data), 1);
    if (!data) {
        UDBG_ERR("%s: allocate data buf failed!\n", __func__);
        return;
    }

    //2a. dump usb state
    wq_usb_dump_info(udev);

#if 0
    //2b. type, info
    err = usb_control_msg(udev->usbdev,                             //dev
                usb_rcvctrlpipe(udev->usbdev, 0),                   //pipe
                0x41,                                               //request
                USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,    //requesttype
                type,                                               //value
                info,                                               //index
                data,                                               //data
                sizeof(data),                                       //size
                HZ);
    if (err < 0) {
        UDBG_ERR("%s: failed, err=%d!\n", __func__, err);
    } else {
        UDBG_INF("%s: succ\n", __func__);
    }
#endif

    //3. issue timeout pattern
    for (i = 0; i < 2; i++) {
        len = (i == 0)? type:info;
        err = usb_control_msg(udev->usbdev,                             //dev
                    usb_rcvctrlpipe(udev->usbdev, 0),                   //pipe
                    0x41,                                               //request
                    USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,    //requesttype
                    0xAAAA,                                             //value
                    0xBBBB,                                             //index
                    data,                                               //data
                    len,                                                //size
                    HZ);
        if (err < 0) {
            UDBG_ERR("%s: failed - %d (err=%d)!\n", __func__, i, err);
        } else {
            UDBG_INF("%s: succ - %d\n", __func__, i);
        }
    }

    if (data)
        kfree(data);
}

void wq_usb_trigger_work(struct work_struct *work)
{
    struct wq_udev *udev = container_of(work, struct wq_udev, trigger_work);

    UDBG_WRN("%s: cnt=%d, type=%d, info=%d\n", __func__,
             udev->trigger_cnt, udev->trigger_type, udev->trigger_info);

    //send trigger pattern
    if (udev->trigger_cnt <= 10) {
        wq_usb_send_trigger_pattern(udev, udev->trigger_type, udev->trigger_info);
    }
}

static struct wq_usbreq *
wq_usb_qinit(struct list_head *q, int qsize)
{
	int i;
	struct wq_usbreq *req, *reqs;

	reqs = kcalloc(qsize, sizeof(struct wq_usbreq), GFP_ATOMIC);
	if (reqs == NULL)
		return NULL;

	req = reqs;

	for (i = 0; i < qsize; i++) {
		req->urb = usb_alloc_urb(0, GFP_ATOMIC);
		if (!req->urb)
			goto fail;

		INIT_LIST_HEAD(&req->list);
		list_add_tail(&req->list, q);
		req++;
	}

	return reqs;

fail:
	UDBG_ERR("%s: fail!\n", __func__);

	while (!list_empty(q)) {
		req = list_entry(q->next, struct wq_usbreq, list);
		if (req)
			usb_free_urb(req->urb);

		list_del(q->next);
	}

	return NULL;
}

void
wq_usb_submit_in_urb(void *wq_dev)
{
    struct wq_udev *udev = (struct wq_udev *)wq_dev;
	struct wq_usbreq *req;

    //1. preparation
	if (!udev) {
		UDBG_ERR("%s: Error! null udev!\n", __func__);
		return;
	}

	if (udev->state != WQ_USB_STATE_UP) {
		UDBG_ERR("%s: Error! USB is not up (%d)\n", __func__, udev->state);
		return;
	}

    //2. submit IN urb
#if WQ_MSG_IN_EP_TYPE == WQ_USB_EPTYPE_INT
    //message
    while ((req = wq_usb_deq(udev, &udev->msgin_freeq, &udev->msgin_freecount)) != NULL)
        wq_usb_msg_in(udev, req);
#endif

    //packet
#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
    while ((req = wq_usb_deq(udev, &udev->pktin_freeq, &udev->pktin_freecount)) != NULL){
        wq_usb_pkt_in(udev, req);
        if (reset_down_flag)
            break;
    }
#else
    while ((req = wq_usb_deq(udev, &udev->pktin_freeq, &udev->pktin_freecount)) != NULL)
        wq_usb_pkt_in(udev, req);
#endif
}

static void
wq_usb_kill_urbs(struct list_head *q, bool cancel)
{
	struct wq_usbreq *req, *next;
	int i = 0;

	list_for_each_entry_safe(req, next, q, list) {
		if (!req->urb) {
			UDBG_ERR("%s: bad req\n", __func__);
			break;
		}

		i++;

        if (cancel) {
			usb_kill_urb(req->urb);
		} else {
			usb_free_urb(req->urb);
			list_del_init(&req->list);
		}
	}
}

static void
wq_usb_cancel_all_urbs(struct wq_udev *udev)
{
	usb_kill_anchored_urbs(&udev->msgout_anchor);

	wq_usb_kill_urbs(&udev->pktout_postq, true);
	wq_usb_kill_urbs(&udev->pktin_postq, true);
    wq_usb_kill_urbs(&udev->pktin_workq, true);
#if WQ_MSG_IN_EP_TYPE == WQ_USB_EPTYPE_INT
   	wq_usb_kill_urbs(&udev->msgin_postq, true);
#endif
}

static int
wq_usb_up(struct wq_udev *udev)
{
	UDBG_INF(WQ_FN_ENTRY_STR);

	if (udev->state == WQ_USB_STATE_UP)
		return 0;

	//1. resource allocate
	init_usb_anchor(&udev->msgout_anchor);

	//2. state update
	wq_usb_set_state(udev, WQ_USB_STATE_UP);

    //3. fill rx urb
#ifndef WQ_BUS_TEST
	wq_usb_submit_in_urb(udev);
#endif

	return 0;
}

static void
wq_usb_down(struct wq_udev *udev)
{
    UDBG_INF(WQ_FN_ENTRY_STR);

	if (udev == NULL)
		return;

	if (udev->state == WQ_USB_STATE_DOWN)
		return;

	wq_usb_set_state(udev, WQ_USB_STATE_DOWN);

	wq_usb_cancel_all_urbs(udev);
}

static void
wq_usb_detach(struct wq_udev *udev)
{
    int loop_cnt = 0, urbs_back = 0;

    UDBG_INF("%s: udev=0x%p\n", __func__, udev);

	//1. free urb/queue
    loop_cnt = 0;
    do {
        loop_cnt++;

        if (udev->pktin_freecount == WQ_BULKRX_URB_NUM) {
            urbs_back = 1;
            wq_usb_kill_urbs(&udev->pktin_freeq, false);
        } else {
            urbs_back = 0;
            UDBG_INF("%s: wait pktin urb (cur=%d)\n", __func__, udev->pktin_freecount);
            msleep(20);
        }
    } while((urbs_back == 0) && (loop_cnt < 10));


    loop_cnt = 0;
    do {
        loop_cnt++;

        if (udev->pktout_freecount == WQ_BULKTX_URB_NUM) {
            urbs_back = 1;
            wq_usb_kill_urbs(&udev->pktout_freeq, false);
        } else {
            urbs_back = 0;
            UDBG_INF("%s: wait pktout urb (cur=%d)\n", __func__, udev->pktout_freecount);
            msleep(20);
        }
    } while((urbs_back == 0) && (loop_cnt < 10));

#if WQ_MSG_IN_EP_TYPE == WQ_USB_EPTYPE_INT
    loop_cnt = 0;
    do {
        loop_cnt++;

        if (udev->msgin_freecount == WQ_MSGIN_URB_NUM) {
            urbs_back = 1;
            wq_usb_kill_urbs(&udev->msgin_freeq, false);
        } else {
            urbs_back = 0;
            UDBG_INF("%s: wait msgin urb (cur=%d)\n", __func__, udev->msgin_freecount);
            msleep(20);
        }
    } while((urbs_back == 0) && (loop_cnt < 10));
#endif

	kfree(udev->pktout_reqs);
	kfree(udev->pktin_reqs);
	kfree(udev->msgin_reqs);

    //2. free others
    cancel_work_sync(&udev->trigger_work);
#if WQ_USB_RX_TYPE == WQ_USB_RX_DMA
    cancel_work_sync(&udev->pkt_in_work);
#endif
}

static
int wq_usb_attach(struct wq_udev *udev,
                  int n_msgin_urb, int n_bulkrx_urb, int n_bulktx_urb)
{
	//UDBG_INF(WQ_FN_ENTRY_STR);
    UDBG_INF("%s: msg in ep type=%d\n", __func__, WQ_MSG_IN_EP_TYPE);
    UDBG_INF("%s: msg out ep type=%d\n", __func__, WQ_MSG_OUT_EP_TYPE);

    //1. status, sync, work...etc
	udev->state = WQ_USB_STATE_DOWN;

    init_completion(&udev->op_dev_ready);
    INIT_WORK(&udev->trigger_work, wq_usb_trigger_work);
#if WQ_USB_RX_TYPE == WQ_USB_RX_DMA
    INIT_WORK(&udev->pkt_in_work, wq_usb_pkt_in_work);
#endif


    //2. urb
    udev->msg_mtu = WQ_USB_MTU_MSG;
    udev->pkt_mtu = WQ_USB_MTU_PKT;

	spin_lock_init(&udev->qlock);

#if WQ_MSG_IN_EP_TYPE == WQ_USB_EPTYPE_INT
	INIT_LIST_HEAD(&udev->msgin_freeq);
	INIT_LIST_HEAD(&udev->msgin_postq);
#endif

	INIT_LIST_HEAD(&udev->pktin_freeq);
	INIT_LIST_HEAD(&udev->pktin_postq);
    INIT_LIST_HEAD(&udev->pktin_workq);

	INIT_LIST_HEAD(&udev->pktout_freeq);
	INIT_LIST_HEAD(&udev->pktout_postq);

#if WQ_MSG_IN_EP_TYPE == WQ_USB_EPTYPE_INT
	udev->msgin_reqs = wq_usb_qinit(&udev->msgin_freeq, n_msgin_urb);
	if (!udev->msgin_reqs)
		goto error;
    udev->msgin_freecount = n_msgin_urb;
#endif

	udev->pktin_reqs = wq_usb_qinit(&udev->pktin_freeq, n_bulkrx_urb);
	if (!udev->pktin_reqs)
		goto error;
	udev->pktin_freecount = n_bulkrx_urb;

	udev->pktout_reqs = wq_usb_qinit(&udev->pktout_freeq, n_bulktx_urb);
	if (!udev->pktout_reqs)
		goto error;
	udev->pktout_freecount = n_bulktx_urb;

    atomic_set(&(udev->msgout_cnt), 0);

	return 0;

error:
	UDBG_ERR("%s: failed!\n", __func__);

	wq_usb_detach(udev);

	return -ENODEV;
}

static int
wq_usb_bus_setup(struct wq_udev *udev)
{
	int ret;
    unsigned long __attribute__((__unused__)) left_time = 0;

    //1. attach IPC module
    udev->wq_ipc = wq_ipc_attach((void *)udev);
    if (udev->wq_ipc == NULL) {
        UDBG_ERR("%s: ipc attach failed\n", __func__);
        return -EIO;
    }

    //2. enable usb data i/o
	ret = wq_usb_up(udev);
	if (ret) {
		UDBG_ERR("%s: wq_usb_up failed(%d)\n", __func__, ret);
		goto fail;
	}

#ifdef WQ_FW_DL
#ifndef WQ_BUS_TEST
    //3. wait device ready (4sec: wait until rf completes the maximum calibration)
    left_time = wait_for_completion_timeout(&udev->op_dev_ready , HZ*4);
    if (!left_time) {
        UDBG_ERR("%s: wait device ready timeout!\n", __func__);
        ret = -ETIMEDOUT;
        goto fail;
    }
#endif
#endif

	//4. attach wifi module
	ret = wq_wifi_attach((void *)udev->wq_ipc);
	if (ret) {
		UDBG_ERR("%s: wq_wifi_attach failed(%d)\n", __func__, ret);
		goto fail;
	}

	return 0;

fail:
	//wq_wifi_detach();
    wq_usb_down(udev);

    wq_ipc_detach((void *)udev->wq_ipc);
    udev->wq_ipc = NULL;

	return ret;
}

#ifdef WQ_FW_DL
#if (WQ_USB_FW_DL_EPID == 0)    //fw download through ep0
static int
wq_usb_fw_download(struct wq_udev *udev)
{
	int err = 0, loop_cnt = 0;
    u8 *data = NULL;
    u8 *buf = NULL;
    u16 id = 0;
    u32 i = 0, checksum = 0;
    int len = 0, len_xfer = 0;
    struct wq_usb_vreq_get_dstate *dstate = NULL;

	UDBG_INF(WQ_FN_ENTRY_STR);

    //1. preparation
	if (udev == NULL)
		return -ENODEV;

	if (!udev->fw) {
		UDBG_ERR("%s: No firmware!\n", __func__);
		return -ENOENT;
	}

    buf = (u8 *)wq_mem_alloc(WQ_USB_FW_DL_MTU, 1);
    if (!buf)
        return -ENOMEM;

    //2.download firmware
    data = (u8 *)udev->fw;
    len = udev->fw_len;

    UDBG_INF("%s: start fw downloading, len=%d\n", __func__, udev->fw_len);
    id = 0;
    while (len) {
        len_xfer = (len < WQ_USB_FW_DL_MTU)? len : WQ_USB_FW_DL_MTU;
		memcpy(buf, data, len_xfer);

        //calc checksum
        checksum = 0;
        for (i = 0; i < len_xfer; i++) {
            checksum += buf[i];
        }

        err = usb_control_msg(udev->usbdev,                                     //dev
                              usb_sndctrlpipe(udev->usbdev, 0),                 //pipe
                              WQ_VREQ_ID_FW_DL,                                 //request
                              USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE, //requesttype
                              (u16)(checksum&0xffff),                           //value
                              id,                                               //index
                              buf,                                              //data
                              len_xfer,                                         //size
                              2000);                                            //timeout

        if (err < 0) {
            UDBG_ERR("%s: firmware download id=0x%x failed (err=%d), checksum=0x%x\n", __func__, id, err, (u16)(checksum&0xffff));
            goto done;
        } else {
            UDBG_INF("%s: firmware download id=0x%x completed, checksum=0x%x\n", __func__, id, (u16)(checksum&0xffff));
        }

        len -= len_xfer;
        data += len_xfer;
        id++;
    }

    //3. issue firmware download complete request to device
    //calc checksum
    checksum = 0;
    for (i = 0; i < udev->fw_len; i++) {
        checksum += udev->fw[i];
    }

	err = usb_control_msg(udev->usbdev,                                     //dev
                          usb_sndctrlpipe(udev->usbdev, 0),                 //pipe
                          WQ_VREQ_ID_FW_DL_COMP,                            //request
                          USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE, //requesttype
                          (u16)(checksum&0xffff),                           //value
                          0,                                                //index
                          NULL,                                             //data
                          0,                                                //size
                          2000);                                            //timeout
    if (err < 0) {
        UDBG_ERR("%s: send firmware download complete request failed (%d), checksum=0x%x!\n",
                 __func__, err, (u16)(checksum&0xffff));
        goto done;
    } else {
        UDBG_INF("%s: fw download complete, checksum=0x%x\n", __func__, (u16)(checksum&0xffff));
    }

#if 1
    //4. check device state
    dstate = wq_mem_alloc(sizeof(struct wq_usb_vreq_get_dstate), 1);
    if (!dstate) {
        UDBG_ERR("%s: allocate dstate buf failed!\n", __func__);
        err = -ENOMEM;
        goto done;
    }

    do {
        loop_cnt++;
        msleep(100);
    	err = usb_control_msg(udev->usbdev,                             //dev
                      usb_rcvctrlpipe(udev->usbdev, 0),                 //pipe
                      WQ_VREQ_ID_GET_DSTATE,                            //request
                      USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,  //requesttype
                      0,                                                //value
                      0,                                                //index
                      dstate,                                           //data
                      sizeof(struct wq_usb_vreq_get_dstate),            //size
                      2000);                                            //timeout
        if (err < 0) {
            UDBG_ERR("%s: get device state failed (%d)!\n", __func__, err);
            goto done;
        } else {
            UDBG_INF("%s: get device state=%d, loop_cnt=%d\n", __func__, dstate->state, loop_cnt);

            if (dstate->state != WQ_USB_DSTATE_READY) {
                err = -ENXIO;
            }
        }
    } while ((dstate->state != WQ_USB_DSTATE_READY) && (loop_cnt < 10));
#endif

done:
    //5. update state
    if (err >= 0) {
        err = 0;
        wq_usb_set_state(udev, WQ_USB_STATE_DL_DONE);
    } else {
        err = -ENXIO;
        wq_usb_set_state(udev, WQ_USB_STATE_DL_FAIL);

        //send trigger pattern
        wq_usb_send_trigger_pattern(udev, WQ_USB_TRI_FW_DL, 0);
    }

    if (buf)
        kfree(buf);

    if (dstate)
        kfree(dstate);

	return err;
}
#else   //fw download through ep1
static int
wq_usb_fw_download(struct wq_udev *udev)
{
	int err = 0, loop_cnt = 0;
    int actual_length = 0;
    u8 send_zeropkt = 0;
    u8 *data = NULL;
    u8 *buf = NULL;
    u16 id = 0;
    u32 i = 0, checksum = 0;
    int len = 0, len_xfer = 0;
    struct wq_usb_vreq_get_rom_ver *rom_ver = NULL;
    struct wq_usb_vreq_get_dstate *dstate = NULL;
    u16 tagroom = sizeof(struct wq_usb_fw_dl_tag);
    struct wq_usb_fw_dl_tag *fwdl_tag = NULL;

	UDBG_INF(WQ_FN_ENTRY_STR);

    //1. preparation
	if (udev == NULL)
		return -ENODEV;

	if (!udev->fw) {
		UDBG_ERR("%s: No firmware!\n", __func__);
		return -ENOENT;
	}

    if ((udev->fw_len % 4) != 0) {
		UDBG_ERR("%s: wrong firmware length (len=%d) !\n", __func__, udev->fw_len);
		return -ENOENT;
	}


    //2. get rom version
    rom_ver = (struct wq_usb_vreq_get_rom_ver *)wq_mem_alloc(sizeof(struct wq_usb_vreq_get_rom_ver), 1);
    if (!rom_ver) {
        UDBG_ERR("%s: allocate rom version buf failed!\n", __func__);
        err = -ENOMEM;
        goto done;
    }

    err = usb_control_msg(udev->usbdev,                             //dev
                  usb_rcvctrlpipe(udev->usbdev, 0),                 //pipe
                  WQ_VREQ_ID_GET_ROM_VER,                           //request
                  USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,  //requesttype
                  0,                                                //value
                  0,                                                //index
                  rom_ver,                                          //data
                  sizeof(struct wq_usb_vreq_get_rom_ver),           //size
                  2000);                                            //timeout
    if (err < 0) {
        UDBG_ERR("%s: get rom version failed (%d)!\n", __func__, err);
        goto done;
    } else {
        UDBG_INF("%s: rom version = %d.%d, build time = %d:%d\n", __func__,
                 rom_ver->major, rom_ver->minor, rom_ver->build_hr, rom_ver->build_min);
    }


    //3. download firmware
    UDBG_INF("%s: start fw downloading, len=%d, tagroom=%d\n", __func__, udev->fw_len, tagroom);

    //send fw download start command
    err = usb_control_msg(udev->usbdev,                                     //dev
                          usb_sndctrlpipe(udev->usbdev, 0),                 //pipe
                          WQ_VREQ_ID_FW_DL,                                 //request
                          USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE, //requesttype
                          (((udev->fw_len) >> 16) & 0xffff),                //value
                          ((udev->fw_len) & 0xffff),                        //index
                          NULL,                                             //data
                          0,                                                //size
                          2000);                                            //timeout

    if (err < 0) {
        UDBG_ERR("%s: start command failed (err=%d)\n", __func__, err);
        goto done;
    } else {
        UDBG_INF("%s: start command completed\n", __func__);
    }

    //send fw raw data to device
    buf = (u8 *)wq_mem_alloc(WQ_USB_FW_DL_MTU + tagroom, 1);
    if (!buf) {
        UDBG_ERR("%s: allocate download buf failed!\n", __func__);
        err = -ENOMEM;
        goto done;
    }
    data = (u8 *)udev->fw;
    len = udev->fw_len;
    id = 0;
    while (len) {
        //raw data preparation
        len_xfer = (len < WQ_USB_FW_DL_MTU)? len : WQ_USB_FW_DL_MTU;
		memcpy(buf, data, len_xfer);

        //calc checksum
        checksum = 0;
        for (i = 0; i < len_xfer; i++) {
            checksum += buf[i];
        }

        //update fw download tag
        fwdl_tag = (struct wq_usb_fw_dl_tag *)(buf + len_xfer);
        fwdl_tag->id = id;
        fwdl_tag->checksum = (u16)(checksum&0xffff);

        //submit urb out
        err = usb_bulk_msg(udev->usbdev,                                    //dev
                           usb_sndbulkpipe(udev->usbdev, WQ_USB_FW_DL_EPID),  //pipe
                           buf,                                             //data
                           (len_xfer + tagroom),                            //len
                           &actual_length,                                  //actual length
                           2000);                                           //timeout

        //check result
        if (err < 0) {
            UDBG_ERR("%s: firmware download id=0x%x failed (err=%d), checksum=0x%x, len=%d\n", __func__, id, err, (u16)(checksum&0xffff), actual_length);
            goto done;
        } else {
            UDBG_INF("%s: firmware download id=0x%x completed, checksum=0x%x, len=%d\n", __func__, id, (u16)(checksum&0xffff), actual_length);
        }

        //update data
        len -= len_xfer;
        data += len_xfer;
        id++;
    }

    //send zero length pkt
    send_zeropkt = ((len_xfer + tagroom)%512)? 0:1;
    if (send_zeropkt == 1) {
        err = usb_bulk_msg(udev->usbdev,                                    //dev
                           usb_sndbulkpipe(udev->usbdev, WQ_USB_FW_DL_EPID),  //pipe
                           NULL,                                            //data
                           0,                                               //len
                           &actual_length,                                  //actual length
                           2000);                                           //timeout

        //check result
        if (err < 0) {
           UDBG_ERR("%s: send zero pkt failed (err=%d), len=%d\n", __func__, err, actual_length);
           goto done;
        } else {
           UDBG_INF("%s: send zero pkt completed, len=%d\n", __func__, actual_length);
        }
    }


    //4. issue firmware download complete request to device
    //calc checksum
    checksum = 0;
    for (i = 0; i < udev->fw_len; i++) {
        checksum += udev->fw[i];
    }

	err = usb_control_msg(udev->usbdev,                                     //dev
                          usb_sndctrlpipe(udev->usbdev, 0),                 //pipe
                          WQ_VREQ_ID_FW_DL_COMP,                            //request
                          USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE, //requesttype
                          (u16)(checksum&0xffff),                           //value
                          0,                                                //index
                          NULL,                                             //data
                          0,                                                //size
                          2000);                                            //timeout
    if (err < 0) {
        UDBG_ERR("%s: send firmware download complete request failed (%d), checksum=0x%x!\n",
                 __func__, err, (u16)(checksum&0xffff));
        goto done;
    } else {
        UDBG_INF("%s: fw download complete, checksum=0x%x\n", __func__, (u16)(checksum&0xffff));
    }

#if 1
    //5. check device state
    dstate = (struct wq_usb_vreq_get_dstate *)wq_mem_alloc(sizeof(struct wq_usb_vreq_get_dstate), 1);

    if (!dstate) {
        UDBG_ERR("%s: allocate dstate buf failed!\n", __func__);
        err = -ENOMEM;
        goto done;
    }

    do {
        loop_cnt++;
        msleep(100);
    	err = usb_control_msg(udev->usbdev,                             //dev
                      usb_rcvctrlpipe(udev->usbdev, 0),                 //pipe
                      WQ_VREQ_ID_GET_DSTATE,                            //request
                      USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,  //requesttype
                      0,                                                //value
                      0,                                                //index
                      dstate,                                           //data
                      sizeof(struct wq_usb_vreq_get_dstate),            //size
                      2000);                                            //timeout
        if (err < 0) {
            UDBG_ERR("%s: get device state failed (%d)!\n", __func__, err);
            goto done;
        } else {
            UDBG_INF("%s: get device state=%d, loop_cnt=%d\n", __func__, dstate->state, loop_cnt);

            if (dstate->state != WQ_USB_DSTATE_READY) {
                err = -ENXIO;
            }
        }
    } while ((dstate->state != WQ_USB_DSTATE_READY) && (loop_cnt < 10));
#endif

done:
    //6. update state
    if (err >= 0) {
        err = 0;
        wq_usb_set_state(udev, WQ_USB_STATE_DL_DONE);
    } else {
        err = -ENXIO;
        wq_usb_set_state(udev, WQ_USB_STATE_DL_FAIL);

        //send trigger pattern
        wq_usb_send_trigger_pattern(udev, WQ_USB_TRI_FW_DL, 0);
    }

    if (buf)
        kfree(buf);

    if (dstate)
        kfree(dstate);

    if (rom_ver){
        kfree(rom_ver);
    }

	return err;
}
#endif

#if (WQ_FW_SRC == 1)
static void
wq_usb_request_fw_cb(const struct firmware *fw, void *wq_dev)
{
    struct wq_udev *udev = (struct wq_udev *)wq_dev;
    int ret = 0;

    UDBG_INF(WQ_FN_ENTRY_STR);

    //1. check
    if (!fw) {
		ret = -ENOENT;
        goto fail;
	}

    //2. firmware download
    udev->fw = fw->data;
    udev->fw_len = fw->size;

    ret = wq_usb_fw_download(udev);
    release_firmware(fw);
    if (ret) {
        UDBG_ERR("%s: fw download failed (dev=%s, err=%d)\n", __func__, dev_name(udev->dev), ret);
        goto fail;
    }


    //3. establish host/target communication
	ret = wq_usb_bus_setup(udev);
	if (ret) {
        UDBG_ERR("%s: establish communication failed (dev=%s, err=%d)\n", __func__, dev_name(udev->dev), ret);
		goto fail;
	}

    complete_all(&udev->dev_init_lock);
	return;

fail:
    UDBG_ERR("%s: failed: dev=%s, err=%d\n", __func__, dev_name(udev->dev), ret);

    complete_all(&udev->dev_init_lock);
    device_release_driver(udev->dev);
}
#endif
#endif

static int
wq_usb_probe_ex(struct wq_udev *udev)
{
	int ret;

#ifdef WQ_FW_DL
#if (WQ_FW_SRC == 1)
    char fw_name[] = WQ_USB_FW_FILE_NAME;
#endif
#endif

	UDBG_INF(WQ_FN_ENTRY_STR);

    //1. usb resource preparation
	ret = wq_usb_attach(udev, WQ_MSGIN_URB_NUM, WQ_BULKRX_URB_NUM, WQ_BULKTX_URB_NUM);
	if (ret) {
        UDBG_ERR("%s: usb attach failed (%d)\n", __func__, ret);
		return -ENODEV;
	}


#ifdef WQ_FW_DL
#if (WQ_FW_SRC == 0)
    //2. firmware download
#if (WQ_CONFIG_64 == 1)
    udev->fw = (u8*)wq_fw_data_bulk_64;
    udev->fw_len = wq_fw_data_bulk_64_len;
    UDBG_INF("wq_fw_data_bulk_64_len=%d\n", wq_fw_data_bulk_64_len);
#else
#if (WQ_MSG_IN_EP_TYPE == WQ_USB_EPTYPE_INT && WQ_MSG_OUT_EP_TYPE == WQ_USB_EPTYPE_INT)
    udev->fw = (u8 *)wq_fw_data;
    udev->fw_len = wq_fw_data_len;
    UDBG_INF("wq_fw_data_len=%d\n", wq_fw_data_len);
#else
    udev->fw = (u8 *)wq_fw_data_bulk;
    udev->fw_len = wq_fw_data_bulk_len;
    UDBG_INF("wq_fw_data_bulk_len=%d\n", wq_fw_data_bulk_len);
#endif
#endif

    ret = wq_usb_fw_download(udev);
    if (ret) {
        UDBG_ERR("%s: fw download failed (dev=%s, err=%d)\n", __func__, dev_name(udev->dev), ret);
        goto fail;
    }
#else
    //2. load firmware
    ret = request_firmware_nowait(THIS_MODULE, true, fw_name, udev->dev, WQ_MEM_GFP, udev,
				                  wq_usb_request_fw_cb);
    if (ret) {
        UDBG_ERR("firmware request failed: %d\n", ret);
        goto fail;
    } else {
        return 0;
    }
#endif
#endif


    //3. establish host/target communication
	ret = wq_usb_bus_setup(udev);
	if (ret) {
        UDBG_ERR("%s: establish communication failed (dev=%s, err=%d)\n", __func__, dev_name(udev->dev), ret);
		goto fail;
	}

	return 0;

fail:
	//release resource
	wq_usb_detach(udev);

	return ret;
}

static int
wq_usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
 	struct usb_device *usb = interface_to_usbdev(intf);
    struct wq_udev *udev;
    struct usb_interface_descriptor	*desc;
	struct usb_endpoint_descriptor *endpoint;
    int ret = 0;
	u8 endpoint_num, ep;
    struct wq_usb_ep_cfg epcfg[WQ_USB_NUM_OF_EP] = {{1, 2, 1},
                                                    {1, 2, 0},
                                                    {2, 3, 1},
                                                    {2, 3, 0},
                                                    {3, 2, 0},
                                                    {4, 2, 0},
                                                    {5, 2, 0}};

    wq_dbg_flag = WQ_DEBUG_ALL;
    UDBG_INF("%s: VID=0x%04x, PID=0x%04x, dev class=0x%x, cfg num=%d\n", __func__,
             id->idVendor, id->idProduct,
             usb->descriptor.bDeviceClass, usb->descriptor.bNumConfigurations);

    //1. device info preparation
  	udev = kzalloc(sizeof(*udev), GFP_ATOMIC);
	if (udev == NULL)
		return -ENOMEM;
    g_udev = udev;
    udev->usbdev = usb;
	udev->dev = &usb->dev;

    //protect whole probe phase
    init_completion(&udev->dev_init_lock);

	usb_set_intfdata(intf, udev);


    //2. check device enumeration result
    desc = &intf->altsetting[0].desc;
    UDBG_INF("%s: interface num=%d, class=0x%x, subclass=0x%x, protocol=0x%x, ep num=%d\n",
             __func__, desc->bInterfaceNumber, desc->bInterfaceClass, desc->bInterfaceSubClass,
             desc->bInterfaceProtocol, desc->bNumEndpoints);

    //check endpoint number
    if (desc->bNumEndpoints != WQ_USB_NUM_OF_EP) {
        UDBG_ERR("%s: endpoint number mismatch!\n", __func__);
        ret = -ENODEV;
        goto fail;
    }

    //check endpoint type/dir/addr
	for (ep = 0; ep < desc->bNumEndpoints; ep++) {
		endpoint = &intf->altsetting[0].endpoint[ep].desc;
		endpoint_num = usb_endpoint_num(endpoint);

        UDBG_INF("%s: ep[%d] num=%d, type=%d, dir=%s\n", __func__, ep, endpoint_num,
                 usb_endpoint_type(endpoint), usb_endpoint_dir_in(endpoint)? "IN":"OUT");

        if ((epcfg[ep].num != endpoint_num)
            || (epcfg[ep].type != usb_endpoint_type(endpoint))
            || (epcfg[ep].dir != ((usb_endpoint_dir_in(endpoint))? 1:0))) {
            UDBG_ERR("%s: endpoint configuration mismatch!\n", __func__);
            ret = -ENODEV;
            goto fail;
        }
	}

    //usb speed
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0)
   	if (usb->speed == USB_SPEED_SUPER_PLUS)
		UDBG_WRN("%s: USB3.1 detected\n", __func__);
	else if (usb->speed == USB_SPEED_SUPER)
		UDBG_WRN("%s: USB3.0 detected\n", __func__);
#else
    if (usb->speed == USB_SPEED_SUPER)
		UDBG_WRN("%s: USB3.0 detected\n", __func__);
#endif
	else if (usb->speed == USB_SPEED_HIGH)
		UDBG_INF("%s: USB2.0 detected\n", __func__);
	else
		UDBG_WRN("%s: USB device speed = %d\n", __func__, usb->speed);


    //3. establish the communication with device
    ret = wq_usb_probe_ex(udev);
    if (ret)
		goto fail;


  	//4. device initialize success
#ifndef WQ_FW_DL
    complete_all(&udev->dev_init_lock);
#else
#if (WQ_FW_SRC == 0)
    complete_all(&udev->dev_init_lock);
#endif
#endif

    wq_mp_cali_free();
    set_rate_power(init_rate_pwr_list);
	return 0;

fail:
    //set NULL to avoid free udev in disconnect handler
	usb_set_intfdata(intf, NULL);
   	complete_all(&udev->dev_init_lock);
	kfree(udev);
    g_udev = NULL;
	return ret;
}

extern void wq_wifi_detached_stop_wdrv(void);

void
wq_notify_hawk_reset(struct wq_udev *udev)
{
    int err;
    err = usb_control_msg(udev->usbdev,                             //dev
                usb_rcvctrlpipe(udev->usbdev, 0),                   //pipe
                WQ_VREQ_ID_SET_RESET,                               //request
                USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,   //requesttype
                WQ_USB_RESET_VALUE,                                 //value
                WQ_USB_RESET_INDEX,                                 //index
                NULL,                                               //data
                0,                                                  //size
                2000                                                //timeout
            );
    if (err < 0)
        UDBG_ERR("%s: reset failed (%d)!\n", __func__, err);
    else
        UDBG_INF("%s: reset success!\n", __func__);
}

void wq_usb_reset_chip(void)
{
    if (g_udev) {
        wq_notify_hawk_reset(g_udev);
    }
}

static void
wq_usb_disconnect_ex(struct wq_udev *udev)
{
    UDBG_INF("%s: udev=0x%p\n", __func__, udev);

    if (!udev)
        return;

    wq_ipc_state_stop((void *)udev->wq_ipc);
    wq_wifi_detach();
#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
        while(!wq_wifi_detach_end_flag)
        {
            UDBG_INF("%s: wait wifi detach finish\n", __func__);
            msleep(50);
        }
#endif
    wq_wifi_detached_stop_wdrv();
    wq_usb_down(udev);
    wq_usb_detach(udev);
    wq_usb_dump_info(udev);

    wq_ipc_detach((void *)udev->wq_ipc);
    udev->wq_ipc = NULL;
}

static void
wq_usb_disconnect(struct usb_interface *intf)
{
    struct wq_udev *udev;

    if (!wq_drv_rmmod_flag)
        wq_drv_rmmod_flag = true;

    wq_dbg_flag = WQ_DEBUG_ALL;
    UDBG_INF(WQ_FN_ENTRY_STR);

	udev = (struct wq_udev *)usb_get_intfdata(intf);
	if (udev) {
		wait_for_completion(&udev->dev_init_lock);
        //check interface data again for probe failed case
		if (!usb_get_intfdata(intf))
			goto done;

		wq_usb_disconnect_ex(udev);
        UDBG_INF("%s: to do reset!\n", __func__);
        msleep(500);
        wq_notify_hawk_reset(udev);
		kfree(udev);
		g_udev = NULL;
	}

done:
	UDBG_INF("%s: exit, udev=0x%p\n", __func__, udev);
}

static struct usb_driver wq_usbdrvr = {
	.name = KBUILD_MODNAME,
	.probe = wq_usb_probe,
	.disconnect = wq_usb_disconnect,
	.id_table = wq_usb_id_tbl,
	//.soft_unbind = 1,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
	.disable_hub_initiated_lpm = 1,
#endif
};

int wq_usb_register(void)
{
	UDBG_INF(WQ_FN_ENTRY_STR);

	return usb_register(&wq_usbdrvr);
}

void wq_usb_exit(void)
{
    UDBG_INF(WQ_FN_ENTRY_STR);
    wq_drv_rmmod_flag = true;
    wq_wifi_detach();
    indicate_wq_stop_hawk();
    usb_deregister(&wq_usbdrvr);
}

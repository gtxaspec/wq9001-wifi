#ifndef _WQ_IPC_H
#define _WQ_IPC_H

//Command ID
enum wq_ipc_cmd_ids {
	WQ_IPC_CMD,
	WQ_IPC_CMD_MAX
};

//Event ID
enum wq_ipc_evt_ids {
	WQ_IPC_EVT_FWREADY = 0,
	WQ_IPC_EVT_BMISS,
	WQ_IPC_EVT_DISCONNECT,
	WQ_IPC_EVT_MIC_ERROR,
	WQ_IPC_EVT_ACK_RETURN,
    WQ_IPC_EVT_FW_LOG,
    WQ_IPC_EVT_PACKET_DUMP,
    WQ_IPC_EVT_TRIGGER_PATTERN,
    WQ_IPC_EVT_WRITE_CALI,
    WQ_IPC_EVT_READ_CALI,
	WQ_IPC_EVT_MAX
};

//IPC receiver
enum wq_ipc_rcvs {
	WQ_IPC_RCV_DEV = 0,
	WQ_IPC_RCV_INTF1,
	WQ_IPC_RCV_INTF2,
	WQ_IPC_RCV_MAX
};

//TX pkt queue
enum wq_ipc_txq {
	WQ_IPC_TXQ_BE = 0,
	WQ_IPC_TXQ_BK,
	WQ_IPC_TXQ_VI,
	WQ_IPC_TXQ_VO,
	WQ_IPC_TXQ_MAX
};

struct wq_ipc_mac_interface {
    void (*wq_mac_tx_done)(void *wq_mac, void *buf, int status);
    void (*wq_mac_rx_pkt)(void *wq_mac, struct sk_buff *skb, u32 buf_len);
    void (*wq_mac_evt_handler)(void *wq_mac, u32 buf_len, struct sk_buff *skb);
};

void wq_ipc_state_stop(void *wq_ipc);
void *wq_ipc_attach(void *wq_dev);
void wq_ipc_detach(void *wq_ipc);

void wq_ipc_mac_register(void *wq_ipc, void *wq_mac, struct wq_ipc_mac_interface *mac_inf);
int wq_ipc_headroom_size();

int wq_ipc_cmd_out(void *wq_ipc, enum wq_ipc_rcvs receiver, enum wq_ipc_cmd_ids cmd_id,
               u8 *cmd_buf, u32 cmd_len, u8 *rsp_buf, u32 rsp_buf_sz);
void wq_ipc_cmd_out_cb(void *wq_ipc, u8 pipe_id, struct sk_buff *skb, int status);

void wq_ipc_msg_in(void *wq_ipc, struct sk_buff *skb, bool success);

int wq_ipc_tx_pkt(void *wq_ipc, enum wq_ipc_rcvs receiver, enum wq_ipc_txq txq,
                  u32 pkt_flags, void *buf, u8 *data, u32 len);
void wq_ipc_tx_pkt_cb(void *wq_ipc, u8 pipe_id, void *buf, void *data, int status);

void wq_ipc_rx_pkt(void *wq_ipc, struct sk_buff *skb, bool success);
void wq_ipc_event_hdr_parse(struct sk_buff *skb, u32 *id, u32 *ipc_buf_len, u32 *ipc_hdr_len);

void wq_ipc_send_trigger_pattern(void *wq_ipc, u16 type, u16 info);
int wq_ipc_check_pkt_type(struct sk_buff *skb);

#ifdef WQ_BUS_TEST
void wq_ipc_test_pipe(void *wq_ipc, u8 input);
void wq_ipc_test_pattern(void *wq_ipc, u8 input);
void wq_ipc_test_loop(void *wq_ipc, u32 input);
void wq_ipc_test_pktlen_mode(void *wq_ipc, u32 input);
void wq_ipc_test_ex(void *wq_ipc, u8 id);
#endif


#endif /* _WQ_IPC_H */


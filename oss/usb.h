#ifndef _WQ_USB_H
#define _WQ_USB_H

enum wq_usb_txq {
	WQ_USB_TXQ_BE = 0,
	WQ_USB_TXQ_BK,
	WQ_USB_TXQ_VI,
	WQ_USB_TXQ_VO,
	WQ_USB_TXQ_MAX
};

enum wq_usb_trigger_type {
	WQ_USB_TRI_FW_DL = 1,
	WQ_USB_TRI_CMD_TIMEOUT,
	WQ_USB_TRI_EVENT,
	WQ_USB_TRI_TXPKT_CHK,
	WQ_USB_TRI_URB_IN_LEN,
	WQ_USB_TRI_MAX
};

#define WQ_MEM_GFP  in_interrupt() ? GFP_ATOMIC : GFP_KERNEL
#define WQ_USB_EPTYPE_BULK  0x02
#define WQ_USB_EPTYPE_INT   0x03
#define WQ_USB_RX_DMA 1
#define WQ_USB_RX_CPU 2

int wq_usb_register(void);
void wq_usb_exit(void);

int wq_usb_msg_out(void *wq_dev, struct sk_buff *skb);
int wq_usb_pkt_out(void *wq_dev, enum wq_usb_txq txq, void *buf, u8 *data, u32 len);
void wq_usb_send_trigger_pattern(void *wq_dev, u16 type, u16 info);

void wq_usb_set_dev_ready(void *wq_dev);
void wq_usb_dump_info(void *wq_dev);

#ifdef WQ_BUS_TEST
void wq_usb_submit_in_urb(void *wq_dev);
int wq_usb_tx(void *wq_dev, u8 pipe_id, struct sk_buff *skb);
bool wq_usb_is_msg_out_pipe(u8 pipe_id);
bool wq_usb_is_pkt_out_pipe(u8 pipe_id);
u32 wq_usb_get_mtu(void *wq_dev, u8 pipe_id);
#endif

#endif /* _WQ_USB_H */


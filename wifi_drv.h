#ifndef _WQ_WIFI_DRV_H
#define _WQ_WIFI_DRV_H
#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#define MP_SOC_INFO_LEN    9
#define MP_DPD_GAIN_INFO_LEN    12  //multiple of 4
#define SKB_MAX_LEN    3893                //max size of skb
#define M_AMSDU    0x80

/* for TYPE_ADDIE*/
#define IEEE80211_ADDIE_PROBE_REQ        0x00
#define IEEE80211_ADDIE_AUTH_REQ         0x10
#define IEEE80211_ADDIE_ASOC_REQ         0x20

enum {
    WQ_DEBUG_NONE      = 0x0000,
    WQ_DEBUG_IPC       = 0x0001,   /* wuqi ipc log flag */
    WQ_DEBUG_USB       = 0x0002,   /* wuqi usb log flag */
    WQ_DEBUG_RX_DATA_DUMP = 0x0004,   /* wuqi RX DUMP flag */
    WQ_DEBUG_RX_LIMIT  = 0x0008,   /* wuqi RX flag */
    WQ_DEBUG_IEEE80211 = 0x0010,   /* wuqi IEEE80211 DEBUG flag */
    WQ_DEBUG_MWL       = 0x0020,   /* wuqi MWL DEBUG flag */
    WQ_DEBUG_CCA       = 0x0040,   /* wuqi cca flag  */
    WQ_DEBUG_PKT_XMIT  = 0X0080,   /*wuqi pkt flag*/
    WQ_DEBUG_RX_MGMT_DUMP = 0x0100,   /* wuqi MGMT PKT DUMP flag */
    WQ_DEBUG_PKT_STATISTICAL  = 0x0200,   /* wuqi pkt stat flag*/
    WQ_DEBUG_LOG2FILE     = 0X1000,   /* wuqi debug for tp */
    WQ_DEBUG_PING         = 0x2000,   /* wuqi debug for ping */
    WQ_DEBUG_PS           = 0x4000,   /* wuqi debug for ps */
    WQ_DEBUG_RX_PROBE_RSP    = 0x8000,   /* wuqi debug for RX probe rsp*/
    WQ_DEBUG_ANY       = 0xffff
};

enum {
    WQ_PROC_DBG    = 0x01,
    WQ_PROC_FWDBG  = 0x02,
    WQ_PROC_REGDBG = 0x03,
    WQ_PROC_MAX    = 0xFF
};

#define WQ_DEBUG_ALL (WQ_DEBUG_IPC | WQ_DEBUG_USB | WQ_DEBUG_RX_DATA_DUMP | WQ_DEBUG_RX_LIMIT | \
                      WQ_DEBUG_IEEE80211 | WQ_DEBUG_MWL | WQ_DEBUG_CCA | WQ_DEBUG_PKT_XMIT | \
                      WQ_DEBUG_RX_MGMT_DUMP)

extern u16 wq_dbg_flag;
extern u8 monitor_tx_flag;
extern atomic_t krn_drv_pkt_cnt;
extern atomic_t drv_fw_pkt_cnt;
extern atomic_t fw_ack_cnt;
extern atomic_t drop_pkt_cnt;
extern int sgi_rx_enable;
extern int sgi_tx_enable;

int wq_wifi_attach(void *wq_ipc);
void wq_wifi_detach(void);

void wq_ch_power_set(struct net_device *dev, int *freq, uint8_t *power, int ch_nbr);

struct wuqi_mptest_efuse_ctx{
    uint8_t efuse_info[16];
    uint8_t rsp_efuse_info[16];
    uint8_t info_len;
};

#endif  /* _WQ_WIFI_DRV_H */

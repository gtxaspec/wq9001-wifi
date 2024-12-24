
#ifndef HTC_DESC_H
#define HTC_DESC_H

/* Descriptor of Host/Target Data path communication */
#define HTC_DATA_DESC_LEN    sizeof(struct htc_data_desc)
#define HTC_MSG_DESC_LEN    sizeof(struct htc_msg_desc)
#define HTC_CFM_DESC_LEN    sizeof(struct htc_cfm_desc)

#define MAX_BCN_LEN     470
#define MAX_KEYINFO_LEN 80
#define MAX_CALI_LEN    256
#define MAX_MP_PARAM_LEN    25

enum desc_msg_type {
    MSG_TYPE_REG_READ = 0,
    MSG_TYPE_REG_WRITE,
    MSG_TYPE_RC_NODE,
    MSG_TYPE_BCN_UPDATE,
    MSG_TYPE_TIM_NOTIFY,
    MSG_TYPE_PS_NOTIFY,
    MSG_TYPE_TX_AMPDU_NOTIFY,
    MSG_TYPE_CH_BW_NOTIFY,
    MSG_TYPE_NODE_NOTIFY,
    MSG_TYPE_STATUS_NOTIFY,
    MSG_TYPE_MVAP_NOTIFY,
    MSG_TYPE_KEY_NOTIFY,
    MSG_TYPE_FW_INFO,
    MSG_TYPE_DEV_INFO,
    MSG_TYPE_CALI_INFO,
    MSG_TYPE_MP_SET_NOTIFY,
    MSG_TYPE_MP_GET_NOTIFY,
    MSG_TYPE_AMPDU_AGGR_NOTIFY,
};

enum desc_frame_type {
    FRAME_TYPE_MGMT = 0,
    FRAME_TYPE_DATA
};

enum ampdu_tx_msg {
    AMPDU_TX_START = 0,
    AMPDU_TX_DONE,
    AMPDU_TX_TIMEOUT,
    AMPDU_TX_STOP
};

enum node_info_type {
    NODE_ALLOC = 0,
    NODE_FREE = 1,
    NODE_UPDATE
};

enum status_info_type {
    WQ_INIT = 0,
    WQ_STOP,
    WQ_USB_RESET,
    WQ_SCAN_START,
    WQ_SCAN_END,
    WQ_KEEPALIVE,
};

enum mvp_info_type {
    MVAP_ALLOC = 0,
    MVAP_FREE
};

enum cfm_result {
    CFM_FAIL = -1,
    CFM_SUCC = 0,
};

enum key_info_type {
    KEY_ADD = 0,
    KEY_DELETE
};

enum desc_cfm_type {
    CFM_TYPE_NODE = 0,
    CFM_TYPE_MVAP,
    CFM_TYPE_CH_BW,
};

//ACK return to Host
enum wq_ack_type {
    WQ_ACK_INACT_RELOAD = 0,
    WQ_ACK_MAX
};

enum dev_info_type {
    AP_MAC_INFO = 0,
    MP_FIXED_RATE_INFO
};

enum mp_notify_type {
    MP_TONE_NOTIFY = 0,
    MP_FREQ_DEV_NOTIFY = 1,
    MP_POWER_NOTIFY = 2,
    MP_MAC_ADDR_NOTIFY = 3,
    MP_EFUSE_NOTIFY = 4,
    MP_CRC_ERR_NOTIFY = 5,
    MP_POWER_CTRL_NOTIFY = 6,
    MP_EFUSE_CALIB_NOTIFY = 7,
    MP_DPD_CTRL_NOTIFY = 8,
    MP_DIGIT_GAIN_NOTIFY = 9,
    MP_STATUS_NOTIFY = 10,
    MP_KFREE_CALI_NOTIFY = 11,
    MP_CALI_REMAIN_TIMES_NOTIFY = 12,
    MP_RATE_PWR_NOTIFY = 13,
    MP_SOC_INFO_NOTIFY = 14,
    MP_GAIN_INFO_NOTIFY = 15,
    MP_GAIN_CTRL_INFO_NOTIFY = 16,
    MP_SET_REF_GAIN_NOTIFY = 17,
    MP_RATE_PWR_SRRC_NOTIFY = 18,
};

enum ampdu_agg_msg {
    AMPDU_AGG_GET = 0,
    AMPDU_AGG_SET,
};

struct ack_payload {
    uint8_t    type;
    uint8_t    succ;
    uint8_t    mnid;
};

struct htc_msg_desc {
    uint16_t msg_idx;
    uint16_t len;

    union {
        struct htc_msg_desc *next;
        STAILQ_ENTRY(htc_msg_desc) listp;
    };
};

struct reg_cfg {
    uint32_t addr;
    uint32_t value;
    uint32_t len;
};

struct fw_info {
    uint32_t reserved;
};

struct ni_rc_info {
    uint8_t         ni_mnid;
    uint16_t        ni_capinfo;
    struct ieee80211_rateset ni_rates;
    uint16_t        ni_htcap;
    uint8_t         ni_chw;
    struct ieee80211_htrateset ni_htrates;
    uint8_t         ni_erp;
};

struct ampdu_tx_info {
    uint8_t type;
    uint8_t mnid;
    uint8_t ac;
    uint8_t code;
};

struct ch_bw_info {
    uint16_t ic_freq; /* primary centre frequency in MHz */
    uint8_t ic_ieee; /* IEEE channel number */
    int8_t ic_maxregpower; /* maximum regulatory tx power in dBm */
    uint8_t sw_ch_cali; /* 0: switch channel + cali;  1: switch channel only */
};

struct ch_bw_cfm {
    char result;
    uint8_t ic_ieee; /* IEEE channel number */
};

struct node_info {
    uint8_t type;
    uint8_t mnid;
    uint8_t erp;
};

struct status_info {
    uint8_t type;
};

struct reg_cfm {
    char result;
    uint8_t len;
};

struct fw_info_cfm {
    uint8_t ipc_ver;
	uint8_t hw_ver;
};

struct node_cfm {
    char result;
    uint8_t mnid;
    uint32_t node_ptr;
};

struct key_info {
    uint8_t type;
    uint8_t mnid;
    uint8_t key_info[MAX_KEYINFO_LEN];
    uint8_t mac[IEEE80211_ADDR_LEN];
};

struct mvap_info {
    uint8_t type;
    uint8_t macid;
    uint8_t opmode;
};

struct mvap_cfm {
    char result;
    uint8_t macid;
};

struct beacon_info {
    uint8_t macid;
    uint8_t mnid;
    uint16_t bcn_len;
    uint8_t bcn_data[MAX_BCN_LEN];
};

struct tim_info {
    uint8_t macid;
    uint8_t pm;
    uint16_t aid;
};

struct ps_info {
    uint8_t macid;
    int nsta;
};

struct dev_info{
    uint8_t type;
    uint8_t ap_mac[IEEE80211_ADDR_LEN];
    uint8_t param;
};

struct cali_info{
    uint16_t cali_len;
    uint8_t cali_data[MAX_CALI_LEN];
};

struct mp_info {
    uint8_t type;
    uint16_t param_len;
    uint8_t param[MAX_MP_PARAM_LEN];
    uint16_t rsp_len;
};

struct mp_get_cfm {
    char result;
};

struct ampdu_agg_info {
    uint8_t type;
    uint8_t num;
};

struct ampdu_agg_cfg
{
    uint8_t result;
    uint8_t num;
};

struct htc_data_desc {
    uint8_t sta_idx;
    uint8_t macid;
    uint16_t len;
    uint32_t data_start;

    union {
        /* info for tx data */
        struct {
            uint32_t flags: 16,
                     sw_dur: 1,
                     wait_txc: 1,
                     no_ack: 1,
                     self_cts: 1,
                     rsvd: 12;
            uint32_t ether_vtag: 16,
                    is_addba_req: 1,
                    is_mcast: 1,
                    is_protect: 1,
                    padding: 5,
                    keyid: 4,
                    frame_type: 3,
                    qos_node: 1;
        } tx;

        /* info for rx data */
        struct {
            uint32_t nf: 16,
                    rssi:16;
            uint32_t rate: 8,
                    is_next:1,
                    chain_seq:2, //valid chain_seq value is 1-3
                    rsvd: 21;
        } rx;
    } info;

    // for test
    union {
        struct htc_data_desc *next;
        STAILQ_ENTRY(htc_data_desc) listp;
    };
    void *m;
} __packed;

struct htc_cfm_desc {
    uint16_t cfm_type;
    uint16_t len;
};

struct htc_data_desc *ieee80211_host_desc_gen(struct mbuf *m, struct ieee80211_node *ni);
void htc_mp_set_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t *param, uint16_t param_len);
void htc_mp_set_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t *param, uint16_t param_len);
int htc_mp_get_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t *param, uint16_t param_len, uint8_t *buf, uint16_t buf_len);
#endif

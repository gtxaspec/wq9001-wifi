#ifndef WUQI_CFG80211_PORT_H
#define WUQI_CFG80211_PORT_H

#include <linux/nl80211.h>

#define IMAX_SSID_LEN 32
#define MAC_RATESET_LEN 12
#define IMAX_PASSWORD_LEN 32
#define WUQI_MAX_CHANNELS 20

#define VIF_STATE_IDLE          0x0
#define VIF_STATE_SCANNING      0x1
#define VIF_STATE_CONNECTING    0x2
#define VIF_STATE_CONNECTED     0x4
#define VIF_STATE_AP_CONNECTED  0x8

#define ERR_CHAN_INFO 0x01
#define ERR_TXQ_INFO 0x02
#define CACHE_LOG_LEN    20000


void wuqi_scan_req(void *param, char ssid[], uint8_t len, u16 scanreq_channels_conf[WUQI_MAX_CHANNELS], u16 scanreq_n_channels);
void * wuqi_vap_create(char *name, enum nl80211_iftype type, void *vif_ctx, uint8_t mac[6]);
void mmal_info(void);
void dump_timer_record();
void wuqi_vap_delete(void *param);
void wuqi_vap_setmac(void *param, u8 myaddr[ETH_ALEN]);
int wuqi_set_key(void * vap_param,
        u8 key_idx, bool pairwise, const u8 *mac_addr,
        const u8 *key, const u8 *seq, int key_len, int seq_len,u32 cipher);
void wuqi_set_deftxkey(void * vap_param,u8 key_idx);
int wuqi_del_key(void * vap_param, u8 key_idx, bool pairwise, const u8 *mac_addr);
int wuqi_del_station(void *vap_param, int reason, const u8 *mac);
int wuqi_config_ap_basic(void *vap_param,const u8 *ssid, int ssid_len, int hidden_ssid, int beacon_int,int dtim_period, const u8 *appie, int appie_len);
int wuqi_config_ap_security(void *vap_param,int privacy, int auth_type,int wpa_version, int cipher_group, int n_ciphers_pairwise);
int wuqi_set_txq_params(void *vap_params, void *ac_params);
void wuqi_uptate_txq_params(void *vap_params);

int wuqi_config_channel(void *vap_param, int chan_freq);
extern void dump_bytes(char *note, uint8_t *data, int len);
int wuqi_vap_ap_start(void *vap_param);
int wuqi_vap_ap_stop(void *vap_param);
int wuqi_get_station_info(void *sinfo_params, void *vap_params, const u8 *mac);
int wuqi_get_channel_params(void *vap_params, uint32_t vif_ctx_state, void *channel_info);
int wuqi_vap_config_appie(void *vap_param, int16_t appie_type, const u8 *ie, int ie_len);
int wuqi_config_channel_data_only(void *vap_param, int chan_freq);
void wuqi_disable_ht_cap(void *vap_param);
void wuqi_enable_ht_cap(void *vap_param);
int wuqi_config_shared_key(void *vap_param,int auth_type,int ken_len, int key_idx, const u8 *key,u32 cipher);
int wuqi_config_auth_open(void *vap_param);
void wuqi_config_privacy(void *vap_param, int privacy);
void wuqi_set_extap(void *vap_param, u8 enable);
void wuqi_set_fixed_rate(void *vap_param, u8 fixed_rate);
int wuqi_config_sta_security(void *vap_param, int wpa_version);

struct wuqi_cfg80211_beacon
{
    u8 *head;
    u8 *tail;
    u8 *ies;
    size_t head_len;
    size_t tail_len;
    size_t ies_len;
    size_t tim_len;
    size_t len;
    u8 dtim;
};

struct wuqi_cfg80211_chan
{
    u8 band;
    u8 type;
    u16 center_freq;
    u32 center_freq1;
    u32 center_freq2;
    int8_t tx_power;
    u8 flags;
};

struct wuqi_mac_rateset
{
    u8 length;
    u8 rate_array[MAC_RATESET_LEN];
};

struct wuqi_station_info
{
    u64 rx_bytes;
    u64 tx_bytes;
    u32 rx_packets;
    u32 tx_packets;
};

struct wuqi_chan_info
{
    u8 width;
    u16 center_freq;
    u8 chnum;
    enum nl80211_band band;
};

struct wuqi_ac_txq_params
{
    u16 txop;
    u16 cwmin;
    u16 cwmax;
    u8 aifs;
    u8 acm;
};

struct log_2_file {
    struct file* fp1;
    struct file* fp2;
    mm_segment_t old_fs;
    char write_flag;
    char write_to_fp2_flag;
};

int wuqi_set_edca_params(void *vap_param , struct wuqi_ac_txq_params *edca);
void wuqi_get_edca_params(void *vap_param);
void wuqi_set_nav(uint16_t nav);
void wuqi_get_nav(void);
void wuqi_set_sgi_enable(void *ieee80211vap, bool enable);
bool wuqi_get_sgi_enable(void *ieee80211vap);
void wuqi_set_ampdu_agg_num(void *vap_param, uint8_t  num);
int wuqi_get_ampdu_agg_num(void *vap_param);


#endif

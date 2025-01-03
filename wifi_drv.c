#include <linux/module.h>

#include <net/cfg80211.h> 
#include <linux/skbuff.h>

#include <linux/workqueue.h> /* work_struct */
#include <linux/semaphore.h>
#include <linux/version.h>
#include <linux/netdevice.h>
#include <linux/wireless.h>
#include <linux/if_arp.h>
#include <net/iw_handler.h>
#include <linux/proc_fs.h>

#include "usb.h"
#include "wq_ipc.h"
#include "wuqi_cfg80211_port.h"
#include "wifi_drv.h"
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/etherdevice.h>


#define WIPHY_NAME "phy0"
#define NDEV_NAME "hawk%d"


#ifdef TASKNAME_PRINT
 #define iot_printf(fmt, ...)               \
     do {                                    \
             pr_cont("[%d][%s] ",current->pid,current->comm); \
             pr_cont(fmt, ##__VA_ARGS__);     \
     } while (0)

#else
#define iot_printf printk
#endif

extern int wq_get_desc_size(void);

//For netdevic tx packet headroom/tailroom configuration
#define TXPKT_MAC_HEADROOM_NEEDED       (32 + wq_get_desc_size())  //sizeof(DESC) + 8(IV) + 24(3->11 header, 36-12)
#define TXPKT_MAC_TAILROOM_NEEDED       12  //8(MIC) + 4(ICV)
#define SSID_MAX_LEN 32
#define WQ_MEM_GFP  in_interrupt() ? GFP_ATOMIC : GFP_KERNEL

static struct wuqi_vif_context *g_vif_ctx = NULL;

static struct wuqi_vif_context *wuqi_create_context(void *wq_ipc);
static struct wuqi_vif_context *wuqi_create_context_vif(const char *name, struct wiphy *wiphy, enum nl80211_iftype type, uint8_t mac[6]);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)) 
static struct wireless_dev *wuqi_create_dummy_ndev(const char *name,struct wiphy *wiphy);
#else
static struct net_device *wuqi_create_dummy_ndev(const char *name,struct wiphy *wiphy);
#endif
static int wuqi_free_dummy_ndev(struct net_device *ndev);
extern void oss_start(void);
extern void oss_stop(void);
extern void wproto_start(void);
extern void wdrv_start(void *);
extern void wdrv_stop(void);
extern void wq_set_fw_dbg(uint32_t fw_dbg);

extern int rtnl_is_locked(void);
extern void rtnl_lock(void);
extern void rtnl_unlock(void);
extern void wuqi_set_shortslottime_flag(void *vap_param);
extern void wuqi_vap_change_mode(void *, uint8_t);
extern void wuqi_send_probereq(void *vap_params, uint8_t *addr, uint8_t *ssid, size_t ssidlen);
extern void wuqi_dhcp_debug_msg(struct ethhdr *eh);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,20,0)
    extern void wuqi_config_chan_power(void *vap_params, uint32_t *freq, int *pwr, uint16_t ch_nbr);
#else
    extern void wuqi_config_chan_power(void *vap_params, uint16_t *freq, int *pwr, uint16_t ch_nbr);
#endif
extern void wq_mp_tone_notify(uint8_t enable, uint16_t freq, uint8_t num, uint16_t amp);
extern int wq_mp_freq_dev_notify(uint8_t set, uint16_t *freq_dev);
extern int wq_mp_power_notify(uint8_t set, uint8_t rate_idx, int8_t *power);
extern int wq_mp_mac_notify(uint8_t set, uint8_t *mac_addr);
extern int wq_mp_crc_err_notify(uint32_t *crc_err);
extern int wq_mp_power_ctrl_notify(uint8_t *power_ctrl);
extern int wq_mp_cali_efuse_notify(int set, uint8_t *cali_efuse, uint8_t cal_efuse_len);
extern int wq_mp_dpd_ctrl_notify(uint8_t *dpd_ctrl);
extern int wq_mp_digit_gain_notify(uint8_t mode, uint16_t freq);
extern int wq_mp_status_notify(uint8_t status);
extern int wq_mp_kfree_cali_notify(char *ver, char *cont);
extern int wuqi_config_channel_dpd(void *vap_param, int chan_freq, int chan_info, uint8_t sw_ch_cali);
extern void wuqi_user_chan_power(void *vap_params, int *freq, uint8_t *power, int ch_nbr);
extern bool wuqi_chan_power_valid(void *vap_params, uint16_t freq);
extern int wq_mp_cali_remain_times_notify(uint8_t *remain_times);
extern int wq_rate_pwr_notify(int8_t *pwr_list);
extern int wq_rate_pwr_srrc_notify(int8_t *pwr_list);
extern void wuqi_chan_freq_set(void *vap_params, u32 first_freq, u32 last_freq);
extern int wq_mp_socinfo_notify(uint8_t *soc_info);
extern int wq_mp_gain_notify(uint8_t gain_info);
extern int wq_mp_gain_ctrl_notify(uint8_t gain_ctrl_info);
extern int wq_mp_get_dpdgain_notify(uint8_t *dpd_gain_info);
extern int wq_mp_set_gain_notify(uint32_t *ref_gain);

#ifndef WQ_BUS_TEST
extern uint32_t WQ_MAC_READ(uint32_t addr);
extern void WQ_MAC_WRITE(uint32_t addr, uint32_t val);
#endif

extern void wq_proc_tx_monitor(long tx_count, int phy_perf_flag, int pkt_len, int tx_ms, int tx_us);
extern void vap_attach_to_new_ic(void *vap_param);
extern int wuqi_change_channel_dpd();

#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
extern int submit_rxurb_limit_h;
extern int submit_rxurb_limit_l;
extern int rx_skb_count_limit;
extern int rx_done_count_wk_limit;
#endif

#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
extern bool wq_wifi_detach_flag;
extern bool wq_wifi_detach_end_flag;
#endif

int rx_start = 0;
uint32_t rxcount = 0;
int8_t init_rate_pwr_list[12] = {
    0, //OFDM_MCS0
    0, //OFDM_MCS1
    0, //OFDM_MCS2
    0, //OFDM_MCS3
    0, //OFDM_MCS4
    0, //OFDM_MCS5
    0, //OFDM_MCS6
    0, //OFDM_MCS7
    0, //DSSS_1M
    0, //DSSS_2M
    0, //DSSS_5M
    0, //DSSS_11M
};

int inject_other_types = 0;
u16 wq_dbg_flag = WQ_DEBUG_ALL;
bool wq_drv_rmmod_flag = false;
module_param(wq_dbg_flag, short, S_IRUGO);
MODULE_PARM_DESC(wq_dbg_flag, "dbg_flag");
char *country;
module_param(country, charp, S_IRUGO);
MODULE_PARM_DESC(country, "country code");
int sgi_rx_enable=1, sgi_tx_enable=1;
module_param(sgi_rx_enable, int, S_IRUGO);
module_param(sgi_tx_enable, int, S_IRUGO);

u8 monitor_tx_flag = 0;
static struct net_device * g_wlan1_mon_ndev=NULL;
static struct net *g_wlan1_mon_net=NULL;
int send_keepalive_flag =1;

struct log_2_file log2file;
char *cache_log_buf1;
char *cache_log_buf2;

struct wuqi_mptest_context{
    uint32_t rx_ce_start_cnt;
    uint32_t rx_ce_stop_cnt;
    uint32_t rx_crc_err_cnt;
    int rx_reset_flag;
    uint8_t mac_addr[ETH_ALEN];
    struct wuqi_mptest_efuse_ctx efuse_ctx;
};

//statistic pkt of krn_drv, drv_fw and fw ack
atomic_t krn_drv_pkt_cnt ;
atomic_t drv_fw_pkt_cnt;
atomic_t fw_ack_cnt;
atomic_t drop_pkt_cnt;

struct wuqi_mptest_context mp_ctxt;

struct wuqi_vif_context {
    struct wiphy *wiphy;
    struct net_device *ndev;

    uint32_t vif_ctx_state;
    uint32_t if_up;
    struct semaphore sem;
    struct work_struct ws_connect;
    struct work_struct ws_set_bitrate;
    struct work_struct ws_set_txq_params;
	struct ieee80211_txq_params txq_params;

    u8 connecting_bssid[ETH_ALEN];
    char connecting_ssid[SSID_MAX_LEN];
    struct work_struct ws_disconnect;
    u16 disconnect_reason_code;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,20,0)
    u32 scanreq_channels_conf[WUQI_MAX_CHANNELS];
#else
    u16 scanreq_channels_conf[WUQI_MAX_CHANNELS];
#endif
    int scanreq_channels_pwr[WUQI_MAX_CHANNELS];
    u16 scanreq_n_channels;
    struct work_struct ws_scan;
    struct delayed_work ws_scan_delayed;
    struct cfg80211_scan_request *scan_request;
    void *vap;
    u16 mgmt_rx_reg;
    u8 mac_addr[ETH_ALEN];
    u8 bssid[ETH_ALEN];

    /*ap params*/
    char ap_ssid_len;
    char ap_ssid[IMAX_SSID_LEN];
    int dtim_period;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
    enum nl80211_hidden_ssid hidden_ssid;
#endif
    u32 wpa_versions;
    u32 cipher_group;
    int n_ciphers_pairwise;
    enum nl80211_auth_type auth_type;
    bool privacy;
    struct wuqi_cfg80211_chan chan;
    struct wuqi_mac_rateset rateset;
    struct wuqi_cfg80211_beacon bcn;
    char password[IMAX_PASSWORD_LEN];

    /* statistics */
    struct iw_statistics iwstats;
};


struct wuqi_wiphy_priv_context {
    void *wq_ipc;
    struct wuqi_vif_context  *vif_ctx;
};

struct wuqi_ndev_priv_context {
    struct wuqi_vif_context *vif_ctx;
    struct wireless_dev wdev;
};

static struct wuqi_vif_context *g_vif_ctx0=NULL, *g_vif_ctx1 = NULL;
/* helper function that will retrieve main context from "priv" data of the wiphy */
static struct wuqi_wiphy_priv_context *
wiphy_get_vif_context(struct wiphy *wiphy) { return (struct wuqi_wiphy_priv_context *) wiphy_priv(wiphy); }

/* helper function that will retrieve main context from "priv" data of the network device */
static struct wuqi_ndev_priv_context *
ndev_get_vif_context(struct net_device *ndev) { return (struct wuqi_ndev_priv_context *) netdev_priv(ndev); }

static const u32 wuqi_cipher_suites[] = {
    WLAN_CIPHER_SUITE_WEP40,
    WLAN_CIPHER_SUITE_WEP104,
    WLAN_CIPHER_SUITE_TKIP,
    WLAN_CIPHER_SUITE_CCMP,
    /* Keep as last entry: */
    WLAN_CIPHER_SUITE_AES_CMAC
};

static const struct ieee80211_txrx_stypes
wuqi_txrx_stypes[NUM_NL80211_IFTYPES] = {
    [NL80211_IFTYPE_STATION] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
            BIT(IEEE80211_STYPE_DISASSOC >> 4) |
            BIT(IEEE80211_STYPE_AUTH >> 4) |
            BIT(IEEE80211_STYPE_DEAUTH >> 4) |
            BIT(IEEE80211_STYPE_ACTION >> 4)
    },
    [NL80211_IFTYPE_AP] = {
        .tx = BIT(IEEE80211_STYPE_ACTION >> 4) |
              BIT(IEEE80211_STYPE_PROBE_RESP >> 4),
        .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
            BIT(IEEE80211_STYPE_DISASSOC >> 4) |
            BIT(IEEE80211_STYPE_AUTH >> 4) |
            BIT(IEEE80211_STYPE_DEAUTH >> 4) |
            BIT(IEEE80211_STYPE_ACTION >> 4)
    },
    [NL80211_IFTYPE_P2P_CLIENT] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
    },
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
    [NL80211_IFTYPE_P2P_DEVICE] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
    },
#endif
    [NL80211_IFTYPE_P2P_GO] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
            BIT(IEEE80211_STYPE_DISASSOC >> 4) |
            BIT(IEEE80211_STYPE_AUTH >> 4) |
            BIT(IEEE80211_STYPE_DEAUTH >> 4) |
            BIT(IEEE80211_STYPE_ACTION >> 4)
    }
};

void wq_free_skb(struct sk_buff *skb)
{
    dev_kfree_skb_any(skb);
}

u32 *wq_mem_alloc(int len, int zero)
{
    gfp_t gfp;
    u32 *addr_mem = NULL;

    gfp = in_interrupt() ? GFP_ATOMIC : GFP_KERNEL;
    if (zero)
        addr_mem = kzalloc(len, gfp);
    else
        addr_mem = kmalloc(len, gfp);
    return addr_mem;
}

struct sk_buff *wq_alloc_skb(unsigned int length)
{
    return __dev_alloc_skb(length, WQ_MEM_GFP);
}

int wuqi_tx_mgmt_status(void *vap_ctx_param, char *buf, int size)
{
    struct wuqi_vif_context *vif_ctx = (struct wuqi_vif_context *)vap_ctx_param;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)
    struct wuqi_ndev_priv_context *ndev_data = ndev_get_vif_context(vif_ctx->ndev);

    cfg80211_mgmt_tx_status(&ndev_data->wdev, 0, buf, size, 1, WQ_MEM_GFP);
#else
    cfg80211_mgmt_tx_status(vif_ctx->ndev, 0, buf, size, 1, WQ_MEM_GFP);
#endif
    iot_printf("[%s:%d]\n",__func__,__LINE__);

    return 0;
}

#if 0
int wuqi_rx_mgmt(void *vap_ctx_param, char *buf, int size)
{
    struct wuqi_vif_context *vif_ctx = (struct wuqi_vif_context *)vap_ctx_param;
    struct wuqi_ndev_priv_context *ndev_data = ndev_get_vif_context(vif_ctx->ndev);
    struct wuqi_chan_info channel_info;

    iot_printf("[%s:%d]\n",__func__,__LINE__);

    dump_bytes("wuqi_rx_mgmt",buf,size);
    wuqi_get_channel_params(vif_ctx->vap, vif_ctx->vif_ctx_state, &channel_info);

    cfg80211_rx_mgmt(&ndev_data->wdev,channel_info.center_freq,0,buf,size,0);


    return 0;
}
#else
int wuqi_rx_mgmt(void *vap_ctx_param, char *buf, int size)
{
    struct sk_buff *skb_for_route=NULL;
    u8 rtap_hdr[] = {
        0x00, 0x00, /* radiotap version */
        0x0e, 0x00, /* radiotap length */
        0x00, 0x00, 0x00, 0x00, /* bmap: flags, tx and rx flags */
        0x8, /* F_FRAG (fragment if required) */
        0x00,       /* padding */
        0x00, 0x00, /* RX and TX flags to indicate that */
        0x00, 0x00, /* this is the injected frame directly */
    };

    iot_printf("[%s:%d]\n",__func__,__LINE__);
    if(!g_wlan1_mon_ndev)
        return 0;

    skb_for_route=wq_alloc_skb(1500);
    skb_for_route->dev = g_wlan1_mon_ndev;
    memcpy(skb_for_route->data,rtap_hdr,sizeof(rtap_hdr));
    memcpy(skb_for_route->data+sizeof(rtap_hdr),buf,size);

    skb_for_route->len=sizeof(rtap_hdr)+size;

    skb_reset_mac_header(skb_for_route);
    skb_for_route->ip_summed = CHECKSUM_UNNECESSARY;
    skb_for_route->pkt_type = PACKET_OTHERHOST;
    skb_for_route->protocol = htons(ETH_P_802_2);
    memset(skb_for_route->cb, 0, sizeof(skb_for_route->cb));

    dump_bytes("skb_for_route->data",skb_for_route->data,skb_for_route->len);

    netif_receive_skb(skb_for_route);

    return 0;
}

int wuqi_rx_monitor(void *vap_ctx_param, void *buf, int size, uint32_t flags)
{
    struct wuqi_vif_context *vif_ctx = (struct wuqi_vif_context *)vap_ctx_param;
    struct sk_buff *skb=NULL;

    if (flags & M_AMSDU)
        skb = wq_alloc_skb(SKB_MAX_LEN);
    else
        skb = wq_alloc_skb(1800);

    if(skb == NULL) {
        iot_printf("wuqi_rx_monitor : no buff\n");
        return 0;
    }

    skb->dev = vif_ctx->ndev;

    memcpy(skb->data, buf, size);
    skb->len=size;

    skb_reset_mac_header(skb);
    memset(skb->cb, 0, sizeof(skb->cb));

    //dump_bytes("skb->data", skb->data, skb->len);
    netif_receive_skb(skb);

    return 0;
}

#endif


int wuqi_mic_fail_notify(void *vap_ctx_param, char *mac, int key_id)
{
    struct wuqi_vif_context *vif_ctx = (struct wuqi_vif_context *)vap_ctx_param;

    iot_printf("[%s:%d]\n",__func__,__LINE__);
    cfg80211_michael_mic_failure(vif_ctx->ndev,mac,NL80211_KEYTYPE_PAIRWISE ,key_id, NULL, WQ_MEM_GFP);

    return 0;
}

int wuqi_new_sta_notify(void *vap_ctx_param, char *mac, char *ies, int ies_len)
{
    struct wuqi_vif_context *vif_ctx = (struct wuqi_vif_context *)vap_ctx_param;
    struct station_info sinfo={0};

    iot_printf("%s:%d\n",__func__,__LINE__);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
    sinfo.assoc_req_ies = ies;
    sinfo.assoc_req_ies_len = ies_len;
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0) && LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0))
    sinfo.filled |= STATION_INFO_ASSOC_REQ_IES;
#endif

    cfg80211_new_sta(vif_ctx->ndev,mac, &sinfo, WQ_MEM_GFP);

    return 0;
}

int wuqi_del_sta_notify(void *vap_ctx_param, char *mac)
{
    struct wuqi_vif_context *vif_ctx = (struct wuqi_vif_context *)vap_ctx_param;
    iot_printf("%s:%d\n",__func__,__LINE__);

    cfg80211_del_sta(vif_ctx->ndev, mac, WQ_MEM_GFP);
    return 0;
}

#define NUM_OF_CHANNEL_OF_MICRO_SCAN 4

void build_scan_channel_list(struct wuqi_vif_context *vif_ctx, u16 *chan_list, u16 *n_chan)
{
	int i;
	
	*n_chan = 0;
	
	for (i=0; i<NUM_OF_CHANNEL_OF_MICRO_SCAN; i++) {
		if (vif_ctx->scanreq_n_channels > 0)
		{
			chan_list[i] = vif_ctx->scanreq_channels_conf[vif_ctx->scanreq_n_channels-1];
			(*n_chan)++;
			vif_ctx->scanreq_n_channels--;
		}
		else
			break;
        //iot_printf("build_scan_channel_list %d:%d\n", *n_chan, chan_list[i]);
	}
}

static void wuqi_scan_routine(struct work_struct *w)
{
    struct wuqi_vif_context *vif_ctx = container_of(w, struct wuqi_vif_context, ws_scan);
	u16 chan_list[WUQI_MAX_CHANNELS];
	u16 n_chan;

    iot_printf("[wuqi_scan_routine:%p]\n", vif_ctx->vap);

    if(vif_ctx->vap){

		if(vif_ctx->scan_request == NULL)
		{
			iot_printf("in disconnecting, already report scanning done!\n");
			return;
		}
		
        build_scan_channel_list(vif_ctx, chan_list, &n_chan);

        wuqi_scan_req(vif_ctx->vap, vif_ctx->scan_request->ssids->ssid,
                          vif_ctx->scan_request->ssids->ssid_len,
						  chan_list, n_chan);
    }
    else{

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
        static struct cfg80211_scan_info info = {
            /* if scan was aborted by user(calling cfg80211_ops->abort_scan) or by any driver/hardware issue - field should be set to "true"*/
            .aborted = false,
        };
#endif

        msleep(100);

        /* inform with dummy BSS */
        //inform_dummy_bss(vif_ctx);

        if(down_interruptible(&vif_ctx->sem)) {
            return;
        }

        /* finish scan */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
        cfg80211_scan_done(vif_ctx->scan_request, &info);
#else
        cfg80211_scan_done(vif_ctx->scan_request, false);
#endif
        vif_ctx->scan_request = NULL;
        up(&vif_ctx->sem);
    }
}


static void wuqi_scan_delay_routine(struct work_struct *w)
{
    struct wuqi_vif_context *vif_ctx = container_of(w, struct wuqi_vif_context, ws_scan_delayed.work);
    u16 chan_list[WUQI_MAX_CHANNELS];
	u16 n_chan;
	

    if((vif_ctx != NULL) && (vif_ctx->vap != NULL) &&
        (vif_ctx->scan_request != NULL)){
        //iot_printf("wuqi_scan_delay_routine:%p\n", vif_ctx->vap);

		build_scan_channel_list(vif_ctx, chan_list, &n_chan);
		
        wuqi_scan_req(vif_ctx->vap, vif_ctx->scan_request->ssids->ssid,
                          vif_ctx->scan_request->ssids->ssid_len,
						  chan_list, n_chan);
    }
    else{
        iot_printf("wuqi_scan_delay_routine: fatal error\n");
    }
}

void update_scan_cancel_info(void *vif_ctx_cxt)
{

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
    static struct cfg80211_scan_info info = {
        /* if scan was aborted by user(calling cfg80211_ops->abort_scan) or by any driver/hardware issue - field should be set to "true"*/
        .aborted = false,
    };
#endif

    struct wuqi_vif_context *vif_ctx = vif_ctx_cxt;
    
    if (vif_ctx->vif_ctx_state & VIF_STATE_SCANNING)
    {
         iot_printf("In scanning, report scanning cancel!\n");
        /* finish scan */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
        cfg80211_scan_done(vif_ctx->scan_request, &info);
#else
        cfg80211_scan_done(vif_ctx->scan_request, false);
#endif
        vif_ctx->vif_ctx_state &= (~VIF_STATE_SCANNING);
        vif_ctx->scanreq_n_channels = 0;
        vif_ctx->scan_request = NULL;
    }	

}

void update_bss_info(void *vif_ctx_cxt, char *bssid, char *ie, int ie_len, int chan, uint16_t bcn_interval, uint16_t cap, int last, int signal)
{
    struct cfg80211_bss *bss = NULL;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0)
        struct cfg80211_inform_bss data = {
        .scan_width = NL80211_BSS_CHAN_WIDTH_20,
        .signal = signal*100,
    };
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
    static struct cfg80211_scan_info info = {
        /* if scan was aborted by user(calling cfg80211_ops->abort_scan) or by any driver/hardware issue - field should be set to "true"*/
        .aborted = false,
    };
#endif
    struct wuqi_vif_context *vif_ctx;

    vif_ctx = vif_ctx_cxt;
    if (last == 1)
    {
        iot_printf("call cfg80211_scan_done %p\n", vif_ctx->scan_request);

        if (vif_ctx->scan_request != NULL)
        {
            if (vif_ctx->vif_ctx_state & VIF_STATE_SCANNING)
            {
                /* finish scan */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
                cfg80211_scan_done(vif_ctx->scan_request, &info);
#else
                cfg80211_scan_done(vif_ctx->scan_request, false);
#endif
                vif_ctx->vif_ctx_state &= (~VIF_STATE_SCANNING);
            }
            else
            {
                iot_printf("Error : vif_ctx_state not in VIF_STATE_SCANNING\n");
            }

            vif_ctx->scan_request = NULL;
        }
        return;
    }

    if ((chan >= 1) && (chan <= 14))
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0)
        data.chan = &vif_ctx->wiphy->bands[NL80211_BAND_2GHZ]->channels[chan-1];



        //iot_printf("update_bss_info %02x:%02x:%02x:%02x:%02x:%02x, ie_len=%d\n", (u_char)bssid[0], (u_char)bssid[1], (u_char)bssid[2], (u_char)bssid[3], (u_char)bssid[4], (u_char)bssid[5], ie_len);
        //for (i=0; i<ie_len; i++)
        //	iot_printf("%02x ", ie[i]);

        /* also it posible to use cfg80211_inform_bss() instead of cfg80211_inform_bss_data() */
        bss = cfg80211_inform_bss_data(vif_ctx->wiphy, &data, CFG80211_BSS_FTYPE_UNKNOWN, bssid, 0, cap, bcn_interval,
                ie, ie_len, WQ_MEM_GFP);

#else
        //channel = vif_ctx->wiphy->bands[NL80211_BAND_2GHZ]->channels[chan-1];
        //iot_printf("update_bss_info %02x:%02x:%02x:%02x:%02x:%02x, ie_len=%d\n", (u_char)bssid[0], (u_char)bssid[1], (u_char)bssid[2], (u_char)bssid[3], (u_char)bssid[4], (u_char)bssid[5], ie_len);
        //for (i=0; i<ie_len; i++)
        //	iot_printf("%02x ", ie[i]);

        /* also it posible to use cfg80211_inform_bss() instead of cfg80211_inform_bss_data() */
        bss = cfg80211_inform_bss(vif_ctx->wiphy, &(vif_ctx->wiphy->bands[NL80211_BAND_2GHZ]->channels[chan-1]), bssid, 0, cap, bcn_interval,
                ie, ie_len, signal*100, WQ_MEM_GFP);
#endif

        /* free, cfg80211_inform_bss_data() returning cfg80211_bss structure refcounter of which should be decremented if its not used. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0)
        cfg80211_put_bss(vif_ctx->wiphy, bss);
#else
        cfg80211_put_bss(bss);
#endif
    }

}

void update_connection_info(void *vif_ctx_ctx, uint8_t *bssid, uint8_t *essid, uint8_t essid_len)
{
    struct wuqi_vif_context *vif_ctx;

    vif_ctx = vif_ctx_ctx;

    iot_printf("in update_connection_info, call cfg80211_connect_result bssid=%02x-%02x-%02x-%02x-%02x-%02x\n", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);

    if (vif_ctx->vif_ctx_state & VIF_STATE_CONNECTING)
    {
        vif_ctx->vif_ctx_state &= (~VIF_STATE_CONNECTING);
        vif_ctx->vif_ctx_state |= VIF_STATE_CONNECTED;
        memcpy(vif_ctx->bssid,bssid,ETH_ALEN);
        vif_ctx->ap_ssid_len=essid_len;
        memcpy(vif_ctx->ap_ssid,essid,essid_len);
        cfg80211_connect_result(vif_ctx->ndev, bssid, NULL, 0, NULL, 0, 0, WQ_MEM_GFP);

        netif_carrier_on(vif_ctx->ndev);
        netif_wake_queue(vif_ctx->ndev);
    }
    else
    {
        iot_printf("Error : 1. vif_ctx_state not in VIF_STATE_CONNECTING\n");
    }
}

void update_connection_fail_info(void *vif_ctx_ctx)
{
    struct wuqi_vif_context *vif_ctx;

    vif_ctx = vif_ctx_ctx;

    iot_printf("in update_connection_fail_info, call cfg80211_connect_result\n");

    if (vif_ctx->vif_ctx_state & VIF_STATE_CONNECTING)
    {
        vif_ctx->vif_ctx_state &= (~VIF_STATE_CONNECTING);
        cfg80211_connect_result(vif_ctx->ndev, NULL, NULL, 0, NULL, 0, WLAN_STATUS_UNSPECIFIED_FAILURE, WQ_MEM_GFP);
    }
    else
    {
        iot_printf("Error : 2. vif_ctx_state not in VIF_STATE_CONNECTING\n");
    }
}



void update_disconnection_info(void *vif_ctx_ctx)
{
    struct wuqi_vif_context *vif_ctx;
    vif_ctx = vif_ctx_ctx;

    iot_printf("call cfg80211_disconnected\n");

    if (vif_ctx->vif_ctx_state & VIF_STATE_CONNECTED)
    {
		update_scan_cancel_info(vif_ctx_ctx);

        vif_ctx->vif_ctx_state &= (~VIF_STATE_CONNECTED);
        memset(vif_ctx->bssid,0,ETH_ALEN);
        vif_ctx->ap_ssid_len=0;
        memset(vif_ctx->ap_ssid,0,IMAX_SSID_LEN);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0)
        cfg80211_disconnected(vif_ctx->ndev, vif_ctx->disconnect_reason_code, NULL, 0, true, WQ_MEM_GFP);

#else
        cfg80211_disconnected(vif_ctx->ndev, vif_ctx->disconnect_reason_code, NULL, 0, WQ_MEM_GFP);
#endif
        netif_carrier_off(vif_ctx->ndev);
        netif_stop_queue(vif_ctx->ndev);
    }
    else
    {
        iot_printf("Error : vif_ctx_state not in VIF_STATE_CONNECTED\n");
    }

    vif_ctx->disconnect_reason_code = 0;
}


bool wq_vif_state_is_idle(void *vif_ctx_ctx)
{	
  	return (((struct wuqi_vif_context*)vif_ctx_ctx)->vif_ctx_state) == VIF_STATE_IDLE;
}

void wq_update_rssi_nf(void *vap_ctx_param, uint8_t rssi, uint8_t nf)
{
    struct wuqi_vif_context *vif_ctx = (struct wuqi_vif_context *)vap_ctx_param;
    //agv_rssi-0.5 if last_rssi<agv_rssi, agv_rssi+0.5 if last_rssi>agv_rssi
    if ((int8_t) rssi > (int8_t) vif_ctx->iwstats.qual.level) {
        vif_ctx->iwstats.qual.level=((int8_t) vif_ctx->iwstats.qual.level*7+(int8_t) rssi+4)/8;
    }
    if ((int8_t) rssi < (int8_t) vif_ctx->iwstats.qual.level) {
        vif_ctx->iwstats.qual.level=((int8_t) vif_ctx->iwstats.qual.level*7+(int8_t) rssi-4)/8;
    }
    //iot_printf("agv:%d, last:%d\n",(int8_t) vif_ctx->iwstats.qual.level, (int8_t) rssi);
    vif_ctx->iwstats.qual.noise=(vif_ctx->iwstats.qual.noise*7+nf+4)/8;
    vif_ctx->iwstats.qual.updated=IW_QUAL_LEVEL_UPDATED|IW_QUAL_NOISE_UPDATED|IW_QUAL_DBM;
}



void wq_net_if_input(void *vap_ctx_param, struct sk_buff *skb, uint8_t rssi, uint8_t nf)
{
    struct wuqi_vif_context *vif_ctx = (struct wuqi_vif_context *)vap_ctx_param;
    struct wuqi_ndev_priv_context * ndev_data;
    skb->dev = vif_ctx->ndev;
    ndev_data = ndev_get_vif_context(skb->dev);

    if (((vif_ctx->vif_ctx_state & VIF_STATE_CONNECTED) == VIF_STATE_CONNECTED)
		|| ((vif_ctx->vif_ctx_state & VIF_STATE_AP_CONNECTED) == VIF_STATE_AP_CONNECTED)
		|| ndev_data->wdev.iftype == NL80211_IFTYPE_MONITOR)
    {
		if (vif_ctx->ndev->flags & IFF_UP)
		{
			vif_ctx->ndev->stats.rx_packets++;
		    vif_ctx->ndev->stats.rx_bytes += (skb->len - 14);//payload only

			skb->protocol = eth_type_trans(skb, skb->dev);
			netif_rx_ni(skb);
		}
		else
		{
			dev_kfree_skb_any(skb);
		}
	}
    else
    {
        dev_kfree_skb_any(skb);
    }
}

extern void inject_packets(void *vap_ptr,int types);

static void set_bitrate_routine(struct work_struct *w)
{
    struct wuqi_vif_context *vif_ctx = container_of(w, struct wuqi_vif_context, ws_set_bitrate);

    iot_printf("[set_bitrate_routine:%p] inject_other_types:0x%x\n",
        vif_ctx->vap,inject_other_types);

    if(inject_other_types){
        inject_packets(vif_ctx->vap,inject_other_types);
        inject_other_types=0;
    }else{
        inject_packets(vif_ctx->vap,0xff);
    }
}

static void set_txq_params_routine(struct work_struct *w)
{
    struct wuqi_vif_context *vif_ctx = container_of(w, struct wuqi_vif_context, ws_set_txq_params);

    iot_printf("[set_txq_params_routine] vif_ctx->vap:%p\n",vif_ctx->vap);
    wuqi_uptate_txq_params(vif_ctx->vap);
}


static void wuqi_connect_routine(struct work_struct *w)
{
    struct wuqi_vif_context *vif_ctx = container_of(w, struct wuqi_vif_context, ws_connect);
    int ret;

    iot_printf("[wuqi_connect_routine:%p]\n", vif_ctx->vap);

    if(down_interruptible(&vif_ctx->sem)) {
        iot_printf("down_interruptible failed\n");
        return;
    }

    if(vif_ctx->vap){
        extern int setmlme_assoc_sta(void *vap,	const uint8_t mac[6], int ssid_len, const uint8_t ssid[32]);

        //uint8_t mac[6] = {0x00, 0x48, 0x41, 0x57, 0x4b, 0x46};
        uint8_t ssid[33] = {};

        int ssid_len = strlen(vif_ctx->connecting_ssid);
        strcpy(ssid, vif_ctx->connecting_ssid);

        if(memcmp(vif_ctx->connecting_bssid, "\x00\x00\x00\x00\x00\x00", ETH_ALEN) != 0)
        {
            ret = setmlme_assoc_sta(vif_ctx->vap, vif_ctx->connecting_bssid, ssid_len, ssid);
        }
        else
        {
            ret = setmlme_assoc_sta(vif_ctx->vap, NULL, ssid_len, ssid);
        }

        if (ret != 0)
        {
            cfg80211_connect_result(vif_ctx->ndev, NULL, NULL, 0, NULL, 0, WLAN_STATUS_UNSPECIFIED_FAILURE, WQ_MEM_GFP);
        }
    }
    else{
        //cfg80211_connect_timeout(vif_ctx->ndev, NULL, NULL, 0, GFP_KERNEL, NL80211_TIMEOUT_SCAN);
        cfg80211_connect_result(vif_ctx->ndev, NULL, NULL, 0, NULL, 0, WLAN_STATUS_UNSPECIFIED_FAILURE, WQ_MEM_GFP);
    }

    vif_ctx->connecting_ssid[0] = 0;

    up(&vif_ctx->sem);
}

extern int setmlme_disconnect(void *vap, const uint8_t mac[6], int reason);


static void wuqi_disconnect_routine(struct work_struct *w)
{

    struct wuqi_vif_context *vif_ctx = container_of(w, struct wuqi_vif_context, ws_disconnect);
    //uint8_t mac[6] = {0x00, 0x48, 0x41, 0x57, 0x4b, 0x46};

    iot_printf("[wuqi_disconnect_routine] vif_ctx->vap:%p\n",vif_ctx->vap);


    if(down_interruptible(&vif_ctx->sem)) {
        iot_printf("down_interruptible failed\n");
        return;
    }

    if(vif_ctx->vap)
        setmlme_disconnect(vif_ctx->vap, NULL, 0);
    //cfg80211_disconnected(vif_ctx->ndev, vif_ctx->disconnect_reason_code, NULL, 0, true, GFP_KERNEL);

    //vif_ctx->disconnect_reason_code = 0;

    up(&vif_ctx->sem);
}

/* callback that called by the kernel when user decided to scan.
 * This callback should initiate scan routine(through work_struct) and exit with 0 if everything ok.
 * Scan routine should be finished with cfg80211_scan_done() call. */
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)) 
static int wuqi_cfg80211_scan(struct wiphy *wiphy, struct cfg80211_scan_request *request)
#else
static int wuqi_cfg80211_scan(struct wiphy *wiphy, struct net_device *ndev,
            struct cfg80211_scan_request *request)
#endif
{
    //struct wuqi_vif_context *vif_ctx = wiphy_get_vif_context(wiphy)->vif_ctx;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(request->wdev->netdev)->vif_ctx;
#else
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(ndev)->vif_ctx;
#endif
    u32 scan_first_freq;
    u32 scan_last_freq;

    if (wq_drv_rmmod_flag){
        iot_printf("%s: rmmod is being executed, return!\n", __func__);
        return -ERESTARTSYS;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,20,0)
    memset(vif_ctx->scanreq_channels_conf, 0, WUQI_MAX_CHANNELS*sizeof(u32));
#else
    memset(vif_ctx->scanreq_channels_conf, 0, WUQI_MAX_CHANNELS*sizeof(u16));
#endif
   // iot_printf("wuqi_cfg80211_scan n_channels=%d, %p %s\n", vif_ctx->scanreq_n_channels, request->wdev->netdev, request->wdev->netdev->name);

    if(!vif_ctx->ndev){
        iot_printf("%s Error!, ndev is NULL\n",__func__);
        return -ERESTARTSYS;
    }

    if(down_interruptible(&vif_ctx->sem)) {
        return -ERESTARTSYS;
    }

    if (vif_ctx->scan_request != NULL) {
        up(&vif_ctx->sem);
        return -EBUSY;
    }

    vif_ctx->vif_ctx_state |= VIF_STATE_SCANNING;
    vif_ctx->scan_request = request;

    if(request->ie_len) {
        wuqi_vap_config_appie(vif_ctx->vap, IEEE80211_ADDIE_PROBE_REQ, request->ie, request->ie_len);
    }

    //Scan requested channels
    if ((request->n_channels > 0) && (request->n_channels < WUQI_MAX_CHANNELS)) {
        u8 i, ch_cnt = 0;
        vif_ctx->scanreq_n_channels = request->n_channels;
        //iot_printf("wuqi_cfg80211_scan: n_channels=%d\n", vif_ctx->scanreq_n_channels);
#if  LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
        for (i = 0; i < vif_ctx->scanreq_n_channels; i++) {
            vif_ctx->scanreq_channels_conf[vif_ctx->scanreq_n_channels-i-1] = request->channels[i]->center_freq;
            vif_ctx->scanreq_channels_pwr[vif_ctx->scanreq_n_channels-i-1] = request->channels[i]->max_reg_power;
        }
#else
        for (i = 0; i < vif_ctx->scanreq_n_channels; i++) {
            vif_ctx->scanreq_channels_conf[vif_ctx->scanreq_n_channels-i-1] = request->channels[i]->center_freq;
            vif_ctx->scanreq_channels_pwr[vif_ctx->scanreq_n_channels-i-1] = request->channels[i]->max_power;
        }
#endif

        /* update reg_power */
        wuqi_config_chan_power(vif_ctx->vap, vif_ctx->scanreq_channels_conf, vif_ctx->scanreq_channels_pwr, vif_ctx->scanreq_n_channels);

        /* since power may be configured by user, remove channels without power configured */
        for (i = 0; i < request->n_channels; i++) {
            if (wuqi_chan_power_valid(vif_ctx->vap, request->channels[i]->center_freq)) {
                ch_cnt++;
            }
        }
        vif_ctx->scanreq_n_channels = ch_cnt;
#if  LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
        for (i = 0; i < request->n_channels; i++) {
            if (wuqi_chan_power_valid(vif_ctx->vap, request->channels[i]->center_freq)) {
                vif_ctx->scanreq_channels_conf[ch_cnt - 1] = request->channels[i]->center_freq;
                vif_ctx->scanreq_channels_pwr[ch_cnt - 1] = request->channels[i]->max_reg_power;
                ch_cnt--;
            }
        }
#else
        for (i = 0; i < request->n_channels; i++) {
            if (wuqi_chan_power_valid(vif_ctx->vap, request->channels[i]->center_freq)) {
                vif_ctx->scanreq_channels_conf[ch_cnt - 1] = request->channels[i]->center_freq;
                vif_ctx->scanreq_channels_pwr[ch_cnt - 1] = request->channels[i]->max_power;
                ch_cnt--;
            }
        }
#endif
        scan_first_freq = vif_ctx->scanreq_channels_conf[vif_ctx->scanreq_n_channels - 1];
        scan_last_freq = vif_ctx->scanreq_channels_conf[0];
        wuqi_chan_freq_set(vif_ctx->vap, scan_first_freq, scan_last_freq);
    }
    else
    {
        //unlikely to happen, but scan channel 1-6-11 by default
        iot_printf("Error : invalid cfg80211 scan request %d\n", request->n_channels);
        vif_ctx->scanreq_n_channels = 3;
        vif_ctx->scanreq_channels_conf[0] = 2462;
        vif_ctx->scanreq_channels_conf[1] = 2437;
        vif_ctx->scanreq_channels_conf[2] = 2412;
        vif_ctx->scanreq_channels_pwr[0] = 20;
        vif_ctx->scanreq_channels_pwr[1] = 20;
        vif_ctx->scanreq_channels_pwr[2] = 20;

        /* update reg_power */
        wuqi_config_chan_power(vif_ctx->vap, vif_ctx->scanreq_channels_conf, vif_ctx->scanreq_channels_pwr, vif_ctx->scanreq_n_channels);
    }

    up(&vif_ctx->sem);

    if (!schedule_work(&vif_ctx->ws_scan)) {
        return -EBUSY;
    }

    return 0; /* OK */
}

int schedule_scan(void *vif_ctx_cxt)
{
	struct wuqi_vif_context *vif_ctx;
	unsigned long delay = 0;

    vif_ctx = vif_ctx_cxt;
	iot_printf("schedule_scan\n");
	
	
	if (g_vif_ctx0->vif_ctx_state == VIF_STATE_AP_CONNECTED)
	{
		//if STA+AP mode, delay 200ms between each micro-scan for AP beaconing
	    delay = msecs_to_jiffies(200);
	}
	else if (g_vif_ctx1->vif_ctx_state == VIF_STATE_CONNECTED)
	{
		//if STA mode and connected, delay 100ms between each micro-scan for sta to send and receive data
	    delay = msecs_to_jiffies(100);
	}
	
    if (!schedule_delayed_work(&vif_ctx->ws_scan_delayed, delay)) {
        iot_printf("schedule_scan fail\n");
        return -1;
    }
	return 0;
}

int is_cfg80211_scan_done(void *vif_ctx_cxt)
{
	struct wuqi_vif_context *vif_ctx;

    vif_ctx = vif_ctx_cxt;

    if(vif_ctx->scan_request == NULL || vif_ctx->scanreq_n_channels == 0)
		return 1;

	return 0;
}

#ifdef WQ_ADD_AUTH_ASOC_IE
/*
sme->ie format:
ie_id_1+ie_len_1+ie_data1[] + ie_id_2+ie_len_2+ie_data2[]

ie_id:
 1:auth vendor ie
 2:asoc vendor ie
*/
int wuqi_parse_sme_ie(void *vap_param, const u8 *ie, int ie_len)
{
    struct ieee80211vap *vap = (struct ieee80211vap *)vap_param;
    
	if (ie_len == 0) {
		wuqi_vap_config_appie(vap,IEEE80211_ADDIE_AUTH_REQ,ie,ie_len);
		wuqi_vap_config_appie(vap,IEEE80211_ADDIE_ASOC_REQ,ie,ie_len);
		return 0;
	}
	
    dump_bytes("wuqi_parse_sme_ie:",(u8 *) ie,ie_len);
    while (ie_len > 0) {
        if (*ie == 0x01) {
            //ie_id == 0x01, for auth vendor ie
            wuqi_vap_config_appie(vap,IEEE80211_ADDIE_AUTH_REQ,ie+2,*(ie+1));
        } else if(*ie == 0x02) {
            //ie_id == 0x02, for asoc vendor ie
            wuqi_vap_config_appie(vap,IEEE80211_ADDIE_ASOC_REQ,ie+2,*(ie+1));
        } else {
            iot_printf("%s error ie id\n",__func__);
            return EINVAL;
        }
        ie_len = ie_len - (*(ie+1)+2);
        ie = ie + (*(ie+1)+2);
        iot_printf("%s unproce ie_len:%d\n",__func__,ie_len);
    }

    return 0;
}
#endif


/* callback that called by the kernel when there is need to "connect" to some network.
 * It inits connection routine through work_struct and exits with 0 if everything ok.
 * connect routine should be finished with cfg80211_connect_bss()/cfg80211_connect_result()/cfg80211_connect_done() or cfg80211_connect_timeout(). */
static int wuqi_cfg80211_connect(struct wiphy *wiphy, struct net_device *dev,
        struct cfg80211_connect_params *sme)
{
    //struct wuqi_vif_context *vif_ctx = wiphy_get_vif_context(wiphy)->vif_ctx;
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(dev)->vif_ctx;
    static int auth_toggle=0;
    size_t ssid_len = (sme->ssid_len>=SSID_MAX_LEN)? SSID_MAX_LEN : sme->ssid_len;

    if (wq_drv_rmmod_flag){
        iot_printf("%s: rmmod is being executed, return!\n", __func__);
        return -ERESTARTSYS;
    }

    if(vif_ctx == g_vif_ctx)
    {
        iot_printf("use the fake network card\n");
        return -ENODEV;
    }

    if(down_interruptible(&vif_ctx->sem)) {
        iot_printf("down_interruptible failed\n");
        return -ERESTARTSYS;
    }

    vif_ctx->vif_ctx_state |= VIF_STATE_CONNECTING;

    memcpy(vif_ctx->connecting_ssid, sme->ssid, ssid_len);
    vif_ctx->connecting_ssid[ssid_len] = 0;

    if(sme->bssid != NULL)
    {
        memcpy(vif_ctx->connecting_bssid, sme->bssid, ETH_ALEN);
    }
    else
    {
        memset(vif_ctx->connecting_bssid, 0x00, ETH_ALEN);
    }

    up(&vif_ctx->sem);

#ifdef WQ_ADD_AUTH_ASOC_IE
    wuqi_parse_sme_ie(vif_ctx->vap, sme->ie, sme->ie_len);
#else
    wuqi_vap_config_appie(vif_ctx->vap, IEEE80211_ADDIE_ASOC_REQ, sme->ie, sme->ie_len);
#endif
   
    iot_printf("cipher_pairwise:0x%x, supported unicast cipher number:%d, wpa_versions:%d \n",
        sme->crypto.ciphers_pairwise[0], sme->crypto.n_ciphers_pairwise, sme->crypto.wpa_versions);

    if(sme->crypto.n_ciphers_pairwise)
    {
        switch(sme->crypto.ciphers_pairwise[0])
        {
             case WLAN_CIPHER_SUITE_WEP40:
             case WLAN_CIPHER_SUITE_WEP104:
             case WLAN_CIPHER_SUITE_TKIP:
                 wuqi_disable_ht_cap(vif_ctx->vap);
                 break;
             case WLAN_CIPHER_SUITE_CCMP:
                 wuqi_enable_ht_cap(vif_ctx->vap);
                 break;
              default:
                 iot_printf("%s: invalid cipher pairwise=0x%x\n", __func__, sme->crypto.ciphers_pairwise[0]);
                 return -EINVAL;
        }

        if(sme->auth_type==NL80211_AUTHTYPE_SHARED_KEY){
            iot_printf("%s %d shared key auth\n",__func__,sme->auth_type);
            wuqi_config_shared_key(vif_ctx->vap,sme->auth_type, sme->key_len,sme->key_idx,sme->key,sme->crypto.ciphers_pairwise[0]);
        }else if(sme->auth_type==NL80211_AUTHTYPE_OPEN_SYSTEM){
            iot_printf("%s %d open auth\n",__func__,sme->auth_type);
            wuqi_config_auth_open(vif_ctx->vap);
        }else {
            //auth config both SHARED OPEN
            auth_toggle=(auth_toggle+1)%2;
            iot_printf("%s %d toggle auth to:%d\n",__func__,sme->auth_type,auth_toggle);
            switch(auth_toggle){
                case NL80211_AUTHTYPE_OPEN_SYSTEM:
                    wuqi_config_auth_open(vif_ctx->vap);
                    break;
                case NL80211_AUTHTYPE_SHARED_KEY:
                    wuqi_config_shared_key(vif_ctx->vap,auth_toggle, sme->key_len,sme->key_idx,sme->key,sme->crypto.ciphers_pairwise[0]);
            }
        }

        if ((sme->crypto.ciphers_pairwise[0] == WLAN_CIPHER_SUITE_TKIP) ||
            (sme->crypto.ciphers_pairwise[0] == WLAN_CIPHER_SUITE_CCMP)) {
            wuqi_config_sta_security(vif_ctx->vap, sme->crypto.wpa_versions);
        } else {
            wuqi_config_sta_security(vif_ctx->vap, 0);
        }
    }
    else
    {
        iot_printf("open mode\n");
        wuqi_config_sta_security(vif_ctx->vap, 0);
        wuqi_config_auth_open(vif_ctx->vap);
        wuqi_enable_ht_cap(vif_ctx->vap);
    }

    wuqi_config_privacy(vif_ctx->vap,sme->privacy);

    if (!schedule_work(&vif_ctx->ws_connect)) {
        return -EBUSY;
    }
    return 0;
}
/* callback that called by the kernel when there is need to "diconnect" from currently connected network.
 * It inits disconnect routine through work_struct and exits with 0 if everything ok.
 * disconnect routine should call cfg80211_disconnected() to inform the kernel that disconnection is complete. */
static int wuqi_cfg80211_disconnect(struct wiphy *wiphy, struct net_device *dev,
        u16 reason_code)
{
    //struct wuqi_vif_context *vif_ctx = wiphy_get_vif_context(wiphy)->vif_ctx;
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(dev)->vif_ctx;

    if (wq_drv_rmmod_flag){
        iot_printf("%s: rmmod/usb_disconnect is being executed, return!\n", __func__);
        return -ERESTARTSYS;
    }

    iot_printf("%s, reason:%d\n",__func__,reason_code);
    if(down_interruptible(&vif_ctx->sem)) {
        iot_printf("down_interruptible failed\n");
        return -ERESTARTSYS;
    }

    vif_ctx->disconnect_reason_code = reason_code;

    up(&vif_ctx->sem);

    if (!schedule_work(&vif_ctx->ws_disconnect)) {
        return -EBUSY;
    }
    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,12,0)
static struct wireless_dev *wuqi_cfg80211_add_iface(struct wiphy *wiphy,
        const char *name,
        unsigned char name_assign_type,
        enum nl80211_iftype type,
        struct vif_params *params)
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0) && (LINUX_VERSION_CODE < KERNEL_VERSION(4,12,0)))
static struct wireless_dev *wuqi_cfg80211_add_iface(struct wiphy *wiphy,
        const char *name,
        unsigned char name_assign_type,
        enum nl80211_iftype type,
        u32 *flags,
        struct vif_params *params)
#elif ((LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(4,1,0)))
static struct wireless_dev *wuqi_cfg80211_add_iface(struct wiphy *wiphy,
        const char *name,
        enum nl80211_iftype type,
        u32 *flags,
        struct vif_params *params)
#elif ((LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)))
static struct wireless_dev *wuqi_cfg80211_add_iface(struct wiphy *wiphy,
        char *name,
        enum nl80211_iftype type,
        u32 *flags,
        struct vif_params *params)
#else
static struct net_device *wuqi_cfg80211_add_iface(struct wiphy *wiphy,
        char *name,
        enum nl80211_iftype type,
        u32 *flags,
        struct vif_params *params)
#endif
{
    struct wuqi_vif_context *vif_ctx=NULL;
    struct wuqi_vif_context **g_vif_ctx_pp=NULL;
    uint8_t mac[6] = {0x00, 0x00, 0x11, 0x22, 0x44, 0x44};
    int ret;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,72)
    mac[4] = get_random_long();
    mac[5] = get_random_long();

#elif ((LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,12)))
    mac[4] = get_random_int();
    mac[5] = get_random_int();
#endif

    if (WQ_MAC_FROM_EFUSE) {
        /* try to retrieve mac addr from efuse */
        iot_printf("WQ_MAC_FROM_EFUSE\n");
        ret = wq_mp_mac_notify(0, mp_ctxt.mac_addr);
        /* use this addr if it's valid and not empty */
        if (ret == 0) {
            int i;
            for (i = 0; i < ETH_ALEN; i++) {
                // check whether mac_addr from efuse is multicast
                if (mp_ctxt.mac_addr[0] & 0x01) {
                    break;
                }
                if (mp_ctxt.mac_addr[i]) {
                    memcpy(mac, mp_ctxt.mac_addr, ETH_ALEN);
                    break;
                }
            }
        }
    }

    iot_printf("wuqi_cfg80211_add_iface:mac address %pM\n", mac);

    iot_printf("wuqi_cfg80211_add_iface:Enter Adding vif_ctx \"%s\" ,type:%d\n", name,type);
    if(strcmp(name, MON_VIF_NAME)==0){
        return wuqi_create_dummy_ndev(name, wiphy);
    }
    else if(type==NL80211_IFTYPE_STATION || type==NL80211_IFTYPE_MONITOR) {
        g_vif_ctx_pp=&g_vif_ctx1;
        if(g_vif_ctx1){
            iot_printf("[%s:%d] g_vif_ctx1:0x%p\n",__func__,__LINE__,g_vif_ctx1);
            goto vif_ctx_created;
        }
    }
    else{
        type=NL80211_IFTYPE_AP;
        iot_printf("Force  \"%s\" ,type to:%d\n", name,type);
        g_vif_ctx_pp=&g_vif_ctx0;
        iot_printf("[%s:%d] g_vif_ctx0:0x%p\n",__func__,__LINE__,g_vif_ctx0);
        if(g_vif_ctx0)
            goto vif_ctx_created;
    }

    if(type!=NL80211_IFTYPE_STATION &&
        type!=NL80211_IFTYPE_AP && type!=NL80211_IFTYPE_MONITOR) {
        iot_printf("[%s:%d] type %d not support yet!\n",__func__,__LINE__,type);
        return NULL;
    }

    //TODO
    //get HW mac address from chip

    //enable local bit for AP vif_ctx
    if (type == NL80211_IFTYPE_AP)
        mac[0] |= 0x2;

    vif_ctx = wuqi_create_context_vif(name, wiphy, type, mac);
    if(vif_ctx==NULL){
        iot_printf("[%s:%d] create vif_ctx fail!\n",__func__,__LINE__);
        return ERR_PTR(-EINVAL);
    }

    *g_vif_ctx_pp = vif_ctx;
    vif_ctx->vap=wuqi_vap_create((char*)name, type, vif_ctx, mac);

    /* always start extap support for sta interface */
    if (type == NL80211_IFTYPE_STATION) {
        wuqi_set_extap(vif_ctx->vap, 1);
        vif_ctx->ndev->priv_flags &= ~IFF_DONT_BRIDGE;
    }

    /*DEMO*/
    sema_init(&vif_ctx->sem, 1);
    INIT_WORK(&vif_ctx->ws_connect, wuqi_connect_routine);
    vif_ctx->connecting_ssid[0] = 0;
    INIT_WORK(&vif_ctx->ws_disconnect, wuqi_disconnect_routine);
    vif_ctx->disconnect_reason_code = 0;
    INIT_WORK(&vif_ctx->ws_set_bitrate, set_bitrate_routine);
    INIT_WORK(&vif_ctx->ws_set_txq_params, set_txq_params_routine);

    INIT_WORK(&vif_ctx->ws_scan, wuqi_scan_routine);
    INIT_DELAYED_WORK(&vif_ctx->ws_scan_delayed, wuqi_scan_delay_routine);
    vif_ctx->scan_request = NULL;

    iot_printf("[%s:%d] Exit!\n",__func__,__LINE__);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)) 
    return vif_ctx->ndev->ieee80211_ptr;
#else
    return vif_ctx->ndev;
#endif
vif_ctx_created:
    iot_printf("[%s:%d] %s is existed, creation failed!\n",__func__,__LINE__,name);
    if((*g_vif_ctx_pp)->vap)
    {
        /*attach twice, vap->iv_ic should point to the new ic*/
        vap_attach_to_new_ic((*g_vif_ctx_pp)->vap);
    }
    return ERR_PTR(-EINVAL);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)
static
int wuqi_cfg80211_del_iface(struct wiphy *wiphy, struct wireless_dev *wdev)
{
    struct wuqi_ndev_priv_context *priv=container_of(wdev, struct wuqi_ndev_priv_context, wdev);
#else
static
int wuqi_cfg80211_del_iface(struct wiphy *wiphy, struct net_device *wdev)
{
    struct wuqi_ndev_priv_context *priv=ndev_get_vif_context(wdev);
#endif
    struct wuqi_vif_context *vif_ctx = priv->vif_ctx;
    //struct wuqi_wiphy_priv_context *wiphy_data = NULL;

	//going to delete iface
	//notify cfg80211 the disconnection if required
	if ((vif_ctx->vif_ctx_state & VIF_STATE_CONNECTED) == VIF_STATE_CONNECTED)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0)
        cfg80211_disconnected(vif_ctx->ndev, 0, NULL, 0, true, WQ_MEM_GFP);
#else
        cfg80211_disconnected(vif_ctx->ndev, 0, NULL, 0, WQ_MEM_GFP);
#endif
	//and move vif state to IDLE to prevent calling cfg80211 after this
	vif_ctx->vif_ctx_state = VIF_STATE_IDLE;

    netif_stop_queue(vif_ctx->ndev);

    if (netif_carrier_ok(vif_ctx->ndev))
        netif_carrier_off(vif_ctx->ndev);

    iot_printf("%s:%d vif_ctx:0x%p, vif_ctx->vap;0x%p\n",__func__,__LINE__,vif_ctx,vif_ctx->vap);
    if (vif_ctx->ndev->reg_state == NETREG_REGISTERED) {
        if(strcmp(vif_ctx->ndev->name, MON_VIF_NAME) != 0) {
            cancel_work_sync(&vif_ctx->ws_connect);
            cancel_work_sync(&vif_ctx->ws_disconnect);
            cancel_work_sync(&vif_ctx->ws_scan);
            cancel_delayed_work_sync(&vif_ctx->ws_scan_delayed);
            cancel_work_sync(&vif_ctx->ws_set_bitrate);
            cancel_work_sync(&vif_ctx->ws_set_txq_params);
        }

        iot_printf("%s:%d, unregister dev 0x%p\n",__func__,__LINE__,vif_ctx->ndev);
        if(rtnl_is_locked()){
                unregister_netdevice(vif_ctx->ndev);
        }else{
                unregister_netdev(vif_ctx->ndev);
        }
    }
    return 0;
}

void wuqi_free_netdev(struct net_device *dev)
{
    struct wuqi_ndev_priv_context * ndev_data = ndev_get_vif_context(dev);
    enum nl80211_iftype type = ndev_data->wdev.iftype;
	struct wuqi_vif_context  *vif_ctx;

    iot_printf("[%s:%d] type:0x%x, dev->reg_state=%d\n",__func__,__LINE__,type,dev->reg_state);

    wuqi_vap_delete(ndev_data->vif_ctx->vap);
    ndev_data->vif_ctx->vap = NULL;
	vif_ctx = ndev_data->vif_ctx;

    free_netdev(dev);

	if (g_vif_ctx1 == vif_ctx)
	{
	    iot_printf("wuqi_free_netdev: free g_vif_ctx1\n");
        kfree(g_vif_ctx1);
        g_vif_ctx1=NULL;
	}
	else if (g_vif_ctx0 == vif_ctx)
	{
		iot_printf("wuqi_free_netdev: free g_vif_ctx0\n");
		kfree(g_vif_ctx0);
        g_vif_ctx0=NULL;
	}
	else
	{
		iot_printf("wuqi_free_netdev: error, could not free g_vif_ctxN\n");
	}

    mmal_info();
    iot_printf("[%s:%d] end\n",__func__,__LINE__);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,12,0)
static s32 wuqi_cfg80211_change_iface(struct wiphy *wiphy, struct net_device *ndev,
        enum nl80211_iftype type,
        struct vif_params *params)
#else
static s32 wuqi_cfg80211_change_iface(struct wiphy *wiphy, struct net_device *ndev,
        enum nl80211_iftype type, u32 *flags,
        struct vif_params *params)
#endif
{
    struct wuqi_ndev_priv_context * ndev_data;
    ndev_data= ndev_get_vif_context(ndev);

    ndev->ieee80211_ptr->iftype = type;
    ndev->type = (type==NL80211_IFTYPE_MONITOR)? ARPHRD_IEEE80211 : ARPHRD_ETHER;
    iot_printf("%s:%d change ndev:%p iftype to %d(ndev->type:%d)\n",__func__,__LINE__, ndev, type, ndev->type);
    switch(type) {
        case NL80211_IFTYPE_MONITOR:
            wuqi_vap_change_mode(ndev_data->vif_ctx->vap, 5); //monitor mode
            break;
        case NL80211_IFTYPE_STATION:
            wuqi_vap_change_mode(ndev_data->vif_ctx->vap, 1); //station
            wuqi_config_channel(ndev_data->vif_ctx->vap, 0);
            break;
        case NL80211_IFTYPE_AP:
            wuqi_vap_change_mode(ndev_data->vif_ctx->vap, 4); //ap
            break;
        default :
            break;
    }
    return 0;
}

static int wuqi_cfg80211_set_bitrate(struct wiphy *wiphy,
        struct net_device *dev,
        const u8 *addr,
        const struct cfg80211_bitrate_mask *mask)
{
    //for debug purpose only
    iot_printf("wuqi_cfg80211_set_bitrate legacy=0x%x\n", mask->control[NL80211_BAND_2GHZ].legacy);

    if (mask->control[NL80211_BAND_2GHZ].legacy == 2)
    {
        struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(dev)->vif_ctx;

        if (!schedule_work(&vif_ctx->ws_set_bitrate)) {
            return -EBUSY;
        }
    }else if (mask->control[NL80211_BAND_2GHZ].legacy == 0x10)
    {
        struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(dev)->vif_ctx;
        //Inject DISASSOC
        inject_other_types=0x80;
        if (!schedule_work(&vif_ctx->ws_set_bitrate)) {
            return -EBUSY;
        }
    }
    else
    {
        mmal_info();
        dump_timer_record();
    }
    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,8,0)
static void
wuqi_cfg80211_mgmt_frame_register(struct wiphy *wiphy,
        struct wireless_dev *wdev,
        struct mgmt_frame_regs *upd)
{
    //when enable wpa_supplicant, disable IFF_DONT_BRIDGE
    if (wdev->iftype == NL80211_IFTYPE_STATION && (wdev->netdev->priv_flags & IFF_DONT_BRIDGE)) {
        wdev->netdev->priv_flags &= ~IFF_DONT_BRIDGE;
    }
}
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0) && LINUX_VERSION_CODE < KERNEL_VERSION(5,8,0))
static void
wuqi_cfg80211_mgmt_frame_register(struct wiphy *wiphy,
        struct wireless_dev *wdev,
        u16 frame_type, bool reg)
{
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(wdev->netdev)->vif_ctx;
    u16 mgmt_type;

    iot_printf("%s, frame_type %04x, reg=%d\n", __func__,frame_type, reg);

    mgmt_type = (frame_type & IEEE80211_FCTL_STYPE) >> 4;

    if (reg)
        vif_ctx->mgmt_rx_reg |= BIT(mgmt_type);
    else
        vif_ctx->mgmt_rx_reg &= ~BIT(mgmt_type);
    //when enable wpa_supplicant, disable IFF_DONT_BRIDGE
    if (wdev->iftype == NL80211_IFTYPE_STATION && (wdev->netdev->priv_flags & IFF_DONT_BRIDGE)) {
        wdev->netdev->priv_flags &= ~IFF_DONT_BRIDGE;
    }
}
#else
static void
wuqi_cfg80211_mgmt_frame_register(struct wiphy *wiphy,
		struct net_device *wdev,
		u16 frame_type, bool reg)
{
	struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(wdev)->vif_ctx;
	struct wuqi_ndev_priv_context *ndev_data = ndev_get_vif_context(wdev);
	u16 mgmt_type;

	iot_printf("%s, frame_type %04x, reg=%d\n", __func__,frame_type, reg);

	mgmt_type = (frame_type & IEEE80211_FCTL_STYPE) >> 4;

	if (reg)
		vif_ctx->mgmt_rx_reg |= BIT(mgmt_type);
	else
		vif_ctx->mgmt_rx_reg &= ~BIT(mgmt_type);
    //when enable wpa_supplicant, disable IFF_DONT_BRIDGE
    if (ndev_data->wdev.iftype == NL80211_IFTYPE_STATION && (wdev->priv_flags & IFF_DONT_BRIDGE)) {
        wdev->priv_flags &= ~IFF_DONT_BRIDGE;
    }
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,14,0)
static int
wuqi_cfg80211_mgmt_tx(struct wiphy *wiphy, struct wireless_dev *wdev,
        struct cfg80211_mgmt_tx_params *params, u64 *cookie)
#elif ((LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(3,14,0)))
static int
wuqi_cfg80211_mgmt_tx(struct wiphy *wiphy, struct wireless_dev *wdev,
              struct ieee80211_channel *chan, bool offchan,
              unsigned int wait, const u8 *buf, size_t len,
              bool no_cck, bool dont_wait_for_ack, u64 *cookie)
#elif ((LINUX_VERSION_CODE >= KERNEL_VERSION(3,3,0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0)))
static int
wuqi_cfg80211_mgmt_tx(struct wiphy *wiphy, struct net_device *dev,
             struct ieee80211_channel *chan, bool offchan,
             enum nl80211_channel_type channel_type,
             bool channel_type_valid, unsigned int wait,
             const u8 *buf, size_t len, bool no_cck,
             bool dont_wait_for_ack, u64 *cookie)
#elif((LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0)))
static int
wuqi_cfg80211_mgmt_tx(struct wiphy *wiphy, struct net_device *dev,
			  struct ieee80211_channel *chan, bool offchan,
			  enum nl80211_channel_type channel_type,
			  bool channel_type_valid, unsigned int wait,
			  const u8 *buf, size_t len, bool no_cck,
			  u64 *cookie)
#else
static int
wuqi_cfg80211_mgmt_tx(struct wiphy *wiphy, struct net_device *dev,
            struct ieee80211_channel *chan, bool offchan,
            enum nl80211_channel_type channel_type,
            bool channel_type_valid, unsigned int wait,
            const u8 *buf, size_t len, u64 *cookie)
#endif
{
    //struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(wdev->netdev)->vif_ctx;
    //TODO:needs implement
    iot_printf("%s, needs implemet\n", __func__);

    return 0;
}

    static s32
wuqi_cfg80211_add_key(struct wiphy *wiphy, struct net_device *ndev,
        u8 key_idx, bool pairwise, const u8 *mac_addr,
        struct key_params *params)
{
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(ndev)->vif_ctx;

    iot_printf("%s, key_idx:%d, pairwise:%d, mac_addr:%pM\n",
            __func__,key_idx,pairwise,mac_addr);

    return wuqi_set_key(vif_ctx->vap,key_idx,pairwise,mac_addr,
            params->key,params->seq,params->key_len,params->seq_len,
            params->cipher);
}

static s32
wuqi_cfg80211_del_key(struct wiphy *wiphy, struct net_device *ndev, u8 key_idx, bool pairwise, const u8 *mac_addr)
{
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(ndev)->vif_ctx;
    return wuqi_del_key(vif_ctx->vap,key_idx,pairwise,mac_addr);
}

static s32
wuqi_cfg80211_get_key(struct wiphy *wiphy, struct net_device *ndev, u8 key_idx,
        bool pairwise, const u8 *mac_addr, void *cookie,
        void (*callback)(void *cookie,
            struct key_params *params))
{
    //TODO:To be implemented
    iot_printf("%s, key_idx:%d, pairwise:%d, To be implemented\n",
            __func__,key_idx,pairwise);

    return 0;
}

static s32
wuqi_cfg80211_config_default_key(struct wiphy *wiphy, struct net_device *ndev, u8 key_idx, bool unicast, bool multicast)
{
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(ndev)->vif_ctx;
    iot_printf("%s, key_idx:%d unicast:%d,multicast:%d\n",
            __func__,key_idx,unicast,multicast);

    wuqi_set_deftxkey(vif_ctx->vap,key_idx);
    return 0;
}

static s32
wuqi_cfg80211_config_default_mgmt_key(struct wiphy *wiphy,
        struct net_device *ndev, u8 key_idx)
{
    iot_printf("%s,key_idx:%d, Not supported \n", __func__, key_idx);

    return 0;
}

void wq_wifi_dbg_set(u16 flag)
{
    wq_dbg_flag = flag;
}

#ifndef WQ_BUS_TEST
static int wuqi_cfg80211_set_wiphy_params(struct wiphy *wiphy, u32 changed)
{
    //wifi ipc/usb dbg log display
    if (changed & WIPHY_PARAM_COVERAGE_CLASS) {
        //  coverage_class:
        //  type: u8
        //  valid value: 0 ~ 255
        iot_printf("%s: coverage_class=%d (temporary debugging flag)\n", __func__, wiphy->coverage_class);
        wq_wifi_dbg_set(wiphy->coverage_class);
    }

    return 0;
}
#else
/*********************************************************************************
 * WQ_BUS_TEST case
 *  - tx test method
 *      step1: set tx pipe and pattern
 *          - sudo iw <phyname> set retry short <num1> long <num2>
 *              - num1
 *                  - [1:7] bitmap for pipe 1 - 7
 *              - num2
 *                  - tx pattern
 *                      - 0x1: 11111..., 22222..., 33333...,...
 *                      - 0x2: 12345..., 23456..., 34567...,...
 *          - e.g.
 *              - sudo iw phy0 set retry short  10 long 1 (pipe=1,3        ; pattern=1)
 *              - sudo iw phy0 set retry short 253 long 2 (pipe=2,3,4,5,6,7; pattern=2)
 *      step2: set tx pkt number
 *          - sudo iw <phyname> set frag <num>
 *              - num
 *                  - tx pkt number x 1000
 *                  - special case: when num < 1000, do infinite loop test
 *          - e.g.
 *              - sudo iw phy0 set frag 10000 (tx 10 pkt)
 *              - sudo iw phy0 set frag 999   (infinite loop test)
 *      step3: set pkt length, test mode and start the tx test
 *          - sudo iw <phyname> set rts <num>
 *              - num
 *                  - [0:29] (max) pkt_len
 *                  - [30] progressive mode, tx_pktlen = 1 ~ pkt_len
 *                  - [31] loopback test
 *                      - in loopback test, pkt will add 8 byte to save loopback data
 *          - e.g.
 *              - sudo iw phy0 set rts 1500 (pkt length = 1500, non-progressive, non-loopback)
 *              - sudo iw phy0 set rts 3221227172 (pkt length = 1700, progressive, loopback)
 *  - rx test method
 *      - sudo iw <phyname> set coverage <num>
 *          - num = unused now
 *      - e.g.
 *          - sudo iw phy0 set coverage 0
 *********************************************************************************/
static int wuqi_cfg80211_set_wiphy_params(struct wiphy *wiphy, u32 changed)
{
    struct wuqi_wiphy_priv_context *p = (struct wuqi_wiphy_priv_context *)wiphy_priv(wiphy) ;

    iot_printf("%s: changed=0x%x\n", __func__, changed);

    if (changed & WIPHY_PARAM_RETRY_SHORT) {
        //changed:
        //  type: u8
        //  valid value: 1 ~ 255
        iot_printf("%s: retry_short=%d\n", __func__, wiphy->retry_short);
        wq_ipc_test_pipe(p->wq_ipc, wiphy->retry_short);
    }
    if (changed & WIPHY_PARAM_RETRY_LONG) {
        //changed:
        //  type: u8
        //  valid value: 1 ~ 255
        iot_printf("%s: retry_long=%d\n", __func__, wiphy->retry_long);
        wq_ipc_test_pattern(p->wq_ipc, wiphy->retry_long);
    }
    if (changed & WIPHY_PARAM_FRAG_THRESHOLD) {
        //changed:
        //  type: u32
        //  valid value: 256 ~ 0xFFFFFFFE (even only)
        iot_printf("%s: frag_threshold=%d\n", __func__, wiphy->frag_threshold);
        wq_ipc_test_loop(p->wq_ipc, wiphy->frag_threshold);
    }
    if (changed & WIPHY_PARAM_RTS_THRESHOLD) {
        //changed:
        //  type: u32
        //  valid value: 0 ~ 0xFFFFFFFF
        iot_printf("%s: rts_threshold=%d\n", __func__, wiphy->rts_threshold);
        wq_ipc_test_pktlen_mode(p->wq_ipc, wiphy->rts_threshold);
    }
    if (changed & WIPHY_PARAM_COVERAGE_CLASS) {
        //changed:
        //  type: u8
        //  valid value: 0 ~ 255
        iot_printf("%s: coverage_class=%d\n", __func__, wiphy->coverage_class);
        wq_ipc_test_ex(p->wq_ipc, wiphy->coverage_class);
    }

    return 0;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
static int
wuqi_cfg80211_del_station(struct wiphy *wiphy, struct net_device *ndev,
        struct station_del_parameters *params)
{
    const u8* mac = params->mac;
    int reason = params->reason_code;
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(ndev)->vif_ctx;
#else
static int
wuqi_cfg80211_del_station(struct wiphy *wiphy, struct net_device *dev,
                       u8 *mac_params)
{
    u8* mac = mac_params;
    int reason = WLAN_REASON_DEAUTH_LEAVING;
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(dev)->vif_ctx;
#endif
    int ret = 0;

    if (mac == NULL || (memcmp(mac, "\xff\xff\xff\xff\xff\xff", ETH_ALEN) == 0))
    {
        iot_printf("%s: del_station_mac is NULL or broadcast addr, return!\n", __func__);
        return ret;
    }
    iot_printf("%s: del_station_mac is %x:%x:%x:%x:%x:%x\n", __func__, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    ret = wuqi_del_station(vif_ctx->vap, reason, mac);
    return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0)
static int
wuqi_cfg80211_change_station(struct wiphy *wiphy, struct net_device *ndev,
        const u8 *mac, struct station_parameters *params)
#else
static int
wuqi_cfg80211_change_station(struct wiphy *wiphy, struct net_device *ndev,
        u8 *mac, struct station_parameters *params)
#endif
{
    //TODO:To be implemented
    iot_printf("%s,To be implemented\n", __func__);

    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
static s32
wuqi_cfg80211_change_beacon(struct wiphy *wiphy, struct net_device *ndev,
        struct cfg80211_beacon_data *info)
{
    //TODO:To be implemented
    iot_printf("%s,To be implemented\n", __func__);

    return 0;
}
#endif

enum wuqi_mac_chan_flags
{
    CHAN_NO_IR = BIT(0),
    CHAN_DISABLED = BIT(1),
    CHAN_RADAR = BIT(2),
};

u8 wuqi_get_chan_flags(u32 flags)
{
    u8 chan_flags = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,14,0)
    if(flags & IEEE80211_CHAN_NO_IR)
#else
    if(flags & IEEE80211_CHAN_PASSIVE_SCAN)
#endif
        chan_flags |= CHAN_NO_IR;
    if(flags & IEEE80211_CHAN_RADAR)
        chan_flags |= CHAN_RADAR;
    return chan_flags;
}

u8 wuqi_chan_to_fw_pwr(int power)
{
    return power > 127 ? 127 : (u8)power;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0)
void wuqi_cfg80211_set_chan(struct cfg80211_chan_def *chandef, struct wuqi_cfg80211_chan *chan)
{
    chan->band = chandef->chan->band;
    chan->type = cfg80211_get_chandef_type(chandef);
    chan->center_freq = chandef->chan->center_freq;
    chan->center_freq1 = chandef->center_freq1;
    chan->center_freq2 = chandef->center_freq2;
    chan->flags = wuqi_get_chan_flags(chandef->chan->flags);
    chan->tx_power = wuqi_chan_to_fw_pwr(chandef->chan->max_power);
}
#elif LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0)
void wuqi_cfg80211_set_chan(struct wiphy *wiphy, struct net_device *dev,
			       struct ieee80211_channel *chan,
			       enum nl80211_channel_type channel_type)
{
    struct wuqi_cfg80211_chan *chan_drv = NULL;
    chan_drv->band = chan->band;
    chan_drv->type = channel_type;
    chan_drv->center_freq = chan->center_freq;
    chan_drv->flags = wuqi_get_chan_flags(chan->flags);
    chan_drv->tx_power = wuqi_chan_to_fw_pwr(chan->max_power);
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
static s32 wuqi_cfg80211_start_ap(struct wiphy *wiphy, struct net_device *ndev,
        struct cfg80211_ap_settings *settings)
{
    u8 appie[255]={0};
    int appie_len=0;

    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(ndev)->vif_ctx;

    /*build the chan*/
    wuqi_cfg80211_set_chan(&settings->chandef, &vif_ctx->chan);
    wuqi_config_chan_power(vif_ctx->vap, &settings->chandef.chan->center_freq, &settings->chandef.chan->max_reg_power, 1);

    /*save another params */
    vif_ctx->ap_ssid_len=settings->ssid_len;
    memcpy(vif_ctx->ap_ssid, settings->ssid, settings->ssid_len);
    vif_ctx->hidden_ssid = settings->hidden_ssid;
    vif_ctx->wpa_versions = settings->crypto.wpa_versions;
    vif_ctx->cipher_group = settings->crypto.cipher_group;
    vif_ctx->n_ciphers_pairwise = settings->crypto.n_ciphers_pairwise;
    vif_ctx->auth_type = settings->auth_type;
    vif_ctx->privacy = settings->privacy;

    //Find SSID in beacon head
    if(!vif_ctx->ap_ssid_len){
        const u8 *ssid_ie=cfg80211_find_ie(WLAN_EID_SSID,settings->beacon.head+36,settings->beacon.head_len);
#if (WQ_CONFIG_64 == 1)
        iot_printf("%s:%d ssid_ie %ld, ssid_id[1]:%d\n",__func__,__LINE__,(long)ssid_ie,(int)ssid_ie[1]);
#else
        iot_printf("%s:%d ssid_ie %d, ssid_id[1]:%d\n",__func__,__LINE__,(int)ssid_ie,(int)ssid_ie[1]);
#endif
        if(ssid_ie && ssid_ie[1]){
            vif_ctx->ap_ssid_len=ssid_ie[1];
            memcpy(vif_ctx->ap_ssid, ssid_ie+2, vif_ctx->ap_ssid_len);
        }
    }

    //Find RSN IE(WLAN_EID_RSN)/WPA(WLAN_EID_VENDOR_SPECIFIC)
    if(settings->beacon.tail && settings->beacon.tail_len){
        const u8 *wpa_ie=NULL, *rsn_ie=NULL;


        rsn_ie=cfg80211_find_ie(WLAN_EID_RSN,
                settings->beacon.tail,
                settings->beacon.tail_len);
        if(rsn_ie && rsn_ie[1]){
            appie_len+=rsn_ie[1]+2;
            memcpy(appie,rsn_ie,appie_len);
            //For hostapd 0.7.3
            vif_ctx->privacy = 1;
            vif_ctx->wpa_versions=2;
            vif_ctx->auth_type=NL80211_AUTHTYPE_OPEN_SYSTEM;
            vif_ctx->n_ciphers_pairwise=WLAN_CIPHER_SUITE_CCMP;
        }

        wpa_ie=cfg80211_find_vendor_ie(WLAN_OUI_MICROSOFT,
                WLAN_OUI_TYPE_MICROSOFT_WPA,
                settings->beacon.tail,
                settings->beacon.tail_len);
        if(wpa_ie && wpa_ie[1]){
            appie_len+=wpa_ie[1]+2;
            memcpy(appie+appie_len,wpa_ie,appie_len);
        }
    }
    wuqi_config_ap_basic(vif_ctx->vap,vif_ctx->ap_ssid,vif_ctx->ap_ssid_len, settings->hidden_ssid,
            settings->beacon_interval,settings->dtim_period, appie, appie_len);

    wuqi_config_ap_security(vif_ctx->vap,vif_ctx->privacy,vif_ctx->auth_type,
            vif_ctx->wpa_versions,vif_ctx->cipher_group,vif_ctx->n_ciphers_pairwise);

    if(g_vif_ctx1!=NULL){ //concurrent mode
        struct wuqi_chan_info channel_info;
        int res;

        iot_printf("%s\n",__func__);
        if(!g_vif_ctx1->vap)
        {
            iot_printf("%s Error! v_vif_ctx1 vap is NULL\n",__func__);
            return -ENODEV;
        }

        res=wuqi_get_channel_params(g_vif_ctx1->vap, g_vif_ctx1->vif_ctx_state, &channel_info);
        if(res!=0){
            iot_printf("concurrent, home channel not ready! Use AP setting:%d\n",vif_ctx->chan.center_freq);
            wuqi_config_channel(vif_ctx->vap,vif_ctx->chan.center_freq);
        }else{
            iot_printf("concurrent, use sta chan:%d\n",channel_info.center_freq);
            wuqi_config_channel_data_only(vif_ctx->vap,channel_info.center_freq);
        }
    }else{
        wuqi_config_channel(vif_ctx->vap,vif_ctx->chan.center_freq);
    }

    wuqi_set_shortslottime_flag(vif_ctx->vap);

    wuqi_vap_ap_start(vif_ctx->vap);

    vif_ctx->vif_ctx_state = VIF_STATE_AP_CONNECTED;

    //Now AP could be carrier on
    netif_carrier_on(vif_ctx->ndev);
    netif_wake_queue(vif_ctx->ndev);

    if(g_wlan1_mon_ndev){
        netif_carrier_on(g_wlan1_mon_ndev);
        netif_wake_queue(g_wlan1_mon_ndev);
    }
    return 0;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
static int wuqi_cfg80211_stop_ap(struct wiphy *wiphy, struct net_device *ndev)
{

	struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(ndev)->vif_ctx;

    iot_printf("%s\n", __func__);

    vif_ctx->vif_ctx_state = VIF_STATE_IDLE;

    wuqi_vap_ap_stop(vif_ctx->vap);

    return 0;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)
static int wuqi_cfg80211_set_txq_params(struct wiphy *wiphy, struct net_device *dev,
        struct ieee80211_txq_params *params)
{
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(dev)->vif_ctx;
#else
static int wuqi_cfg80211_set_txq_params(struct wiphy *wiphy,
        struct ieee80211_txq_params *params)
{
    struct wuqi_vif_context *vif_ctx = wiphy_get_vif_context(wiphy)->vif_ctx;
#endif
    struct wuqi_ac_txq_params ac_txq_params;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
    iot_printf("%s:params_acm:%d,params_aifsn:%d,params_txop:%d,params_cwmin:%d,params_cwmax:%d\n",
                __func__,params->ac,params->aifs,params->txop, params->cwmin, params->cwmax);

    ac_txq_params.acm = params->ac;
#else
    iot_printf("%s:params_acm:%d,params_aifsn:%d,params_txop:%d,params_cwmin:%d,params_cwmax:%d\n",
                __func__,params->queue,params->aifs,params->txop, params->cwmin, params->cwmax);

    ac_txq_params.acm = params->queue;
#endif
    ac_txq_params.txop = params->txop;
    ac_txq_params.cwmin = params->cwmin;
    ac_txq_params.cwmax = params->cwmax;
    ac_txq_params.aifs = params->aifs;

    if(wuqi_set_txq_params(vif_ctx->vap ,&ac_txq_params))
    {
        return -ELNRNG;
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
    if(params->ac == 3)
#else
    if(params->queue == 3)
#endif
    {
        if (!schedule_work(&vif_ctx->ws_set_txq_params))
        {
            return -EBUSY;
        }
    }
    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0)
int wuqi_cfg80211_get_station(struct wiphy *wiphy, struct net_device *dev,
        const u8 *mac, struct station_info *sinfo)
#else
int wuqi_cfg80211_get_station(struct wiphy *wiphy, struct net_device *dev,
        u8 *mac, struct station_info *sinfo)
#endif
{
    struct wuqi_station_info sinfo_params;
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(dev)->vif_ctx;

    wuqi_get_station_info(&sinfo_params, vif_ctx->vap, mac);
    sinfo->rx_packets = sinfo_params.rx_packets;
    sinfo->filled |= BIT(NL80211_STA_INFO_RX_PACKETS);
    sinfo->tx_packets = sinfo_params.tx_packets;
    sinfo->filled |= BIT(NL80211_STA_INFO_TX_PACKETS);
    sinfo->signal = vif_ctx->iwstats.qual.level;
    sinfo->filled |= BIT(NL80211_STA_INFO_SIGNAL);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0)
    sinfo->rx_bytes = sinfo_params.rx_bytes;
    sinfo->filled |= BIT(NL80211_STA_INFO_RX_BYTES64);
    sinfo->tx_bytes = sinfo_params.tx_bytes;
    sinfo->filled |= BIT(NL80211_STA_INFO_TX_BYTES64);
    iot_printf("wuqi_cfg80211_get_station:%d,%lld,%d,%lld,signal:%d\n",sinfo->rx_packets, sinfo->rx_bytes,
            sinfo->tx_packets, sinfo->tx_bytes,sinfo->signal);
#else
    sinfo->rx_bytes = sinfo_params.rx_bytes;
    sinfo->filled |= BIT(NL80211_STA_INFO_RX_BYTES);
    sinfo->tx_bytes = sinfo_params.tx_bytes;
    sinfo->filled |= BIT(NL80211_STA_INFO_TX_BYTES);
    iot_printf("wuqi_cfg80211_get_station:%d,%d,%d,%d,signal:%d\n",sinfo->rx_packets, sinfo->rx_bytes,
            sinfo->tx_packets, sinfo->tx_bytes,sinfo->signal);
#endif

    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0)
int wuqi_cfg80211_get_channel(struct wiphy *wiphy, struct wireless_dev *wdev,
        struct cfg80211_chan_def *chandef)
{
    struct wuqi_vif_context *vif_ctx = ndev_get_vif_context(wdev->netdev)->vif_ctx;
    struct wuqi_chan_info channel_info;

    iot_printf("%s\n",__func__);
    if(!vif_ctx->vap)
    {
        return -ENODEV;
    }

    if(wuqi_get_channel_params(vif_ctx->vap, vif_ctx->vif_ctx_state, &channel_info))
    {
        return -ENODATA;
    }

    chandef->chan = ieee80211_get_channel(wiphy, channel_info.center_freq);
    chandef->width = channel_info.width;
    chandef->center_freq1 = ieee80211_channel_to_frequency(channel_info.chnum, channel_info.band);
    chandef->center_freq2 = 0;

    iot_printf("width:%d, chandef->chan->center_freq:%d,chandef->center_freq1:%d chan:%p\n",chandef->width, chandef->chan->center_freq,chandef->center_freq1,chandef->chan);
    return 0;
}
#else
int wuqi_cfg80211_get_channel(struct wiphy *wiphy)
{
    struct wuqi_vif_context *vif_ctx = wiphy_get_vif_context(wiphy)->vif_ctx;
    struct wuqi_chan_info channel_info;

    iot_printf("%s\n",__func__);
    if(!vif_ctx->vap)
    {
        return -ENODEV;
    }

    if(wuqi_get_channel_params(vif_ctx->vap, vif_ctx->vif_ctx_state, &channel_info))
    {
        return -ENODATA;
    }
     return 0;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0)
int wuqi_cfg80211_set_monitor_channel(struct wiphy *wiphy,
        struct cfg80211_chan_def *chandef)
{
    int chan_info;
    struct wuqi_vif_context *vif_ctx = g_vif_ctx1; // vif of monitor, the same as station mode
    if(vif_ctx)
    {
        //iot_printf("%s: vif_ctx->vap = %p, chandef->center_freq1=%d\n", __func__, vif_ctx->vap, chandef->center_freq1);
        if (chandef->center_freq1 < 2412 || chandef->center_freq1 > 2484) {
            return -ENODATA;
        }
        chan_info = ieee80211_frequency_to_channel(chandef->center_freq1);
        wuqi_config_channel_dpd(vif_ctx->vap , chandef->center_freq1, chan_info, 1);
    }
    return 0;
}
#else
int wuqi_cfg80211_set_monitor_channel(struct wiphy *wiphy,
            struct ieee80211_channel *chan,
            enum nl80211_channel_type channel_type)
{
    int chan_info;
    struct wuqi_vif_context *vif_ctx = g_vif_ctx1; // vif of monitor, the same as station mode
    
    if(vif_ctx)
    {
    //iot_printf("%s: vif_ctx->vap = %p, chandef->center_freq1=%d\n", __func__, vif_ctx->vap, chandef->center_freq1);
        if (chan->center_freq < 2412 || chan->center_freq > 2484) 
        {
            return -ENODATA;
        }
        chan_info = ieee80211_frequency_to_channel(chan->center_freq);
        wuqi_config_channel_dpd(vif_ctx->vap , chan->center_freq, chan_info, 1);
    }
    return 0;
}
#endif

/* Structure of functions for cfg80211 drivers */
static struct cfg80211_ops wuqi_cfg_ops = {
    .add_virtual_intf = wuqi_cfg80211_add_iface,
    .del_virtual_intf = wuqi_cfg80211_del_iface,
    .change_virtual_intf = wuqi_cfg80211_change_iface,
    .scan = wuqi_cfg80211_scan,
    .connect = wuqi_cfg80211_connect,
    .disconnect = wuqi_cfg80211_disconnect,
    .set_bitrate_mask = wuqi_cfg80211_set_bitrate,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,8,0)
    .update_mgmt_frame_registrations = wuqi_cfg80211_mgmt_frame_register,
#else
    .mgmt_frame_register = wuqi_cfg80211_mgmt_frame_register,
#endif
    .mgmt_tx = wuqi_cfg80211_mgmt_tx,
    .add_key = wuqi_cfg80211_add_key,
    .del_key = wuqi_cfg80211_del_key,
    .get_key = wuqi_cfg80211_get_key,
    .set_default_key = wuqi_cfg80211_config_default_key,
    .set_default_mgmt_key = wuqi_cfg80211_config_default_mgmt_key,
    .set_wiphy_params = wuqi_cfg80211_set_wiphy_params,
    .del_station = wuqi_cfg80211_del_station,
    .change_station = wuqi_cfg80211_change_station,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
    .change_beacon = wuqi_cfg80211_change_beacon,
    .start_ap = wuqi_cfg80211_start_ap,
    .stop_ap = wuqi_cfg80211_stop_ap,
#endif
    .set_txq_params = wuqi_cfg80211_set_txq_params,
    .get_station = wuqi_cfg80211_get_station,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,3,0)
    .get_channel = wuqi_cfg80211_get_channel,
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
    .set_monitor_channel = wuqi_cfg80211_set_monitor_channel,
#endif
};

extern int net_if_output(void *vap, struct sk_buff *skb);
extern void dump_bytes(char *note, uint8_t *data, int len);

static netdev_tx_t wq_ndo_start_xmit(struct sk_buff *skb,
        struct net_device *dev)
{
    struct wuqi_ndev_priv_context *ndev_data;
	struct sk_buff *skb_for_tx;
	int ret;
	int len;

    ndev_data = ndev_get_vif_context(dev);

    //statistic pkt number of krn to drv
    atomic_add(1,&krn_drv_pkt_cnt);

	//copy non-linear skb to linear one
	if ((skb->next != NULL) || (skb_cloned(skb) && (dev->priv_flags & IFF_BRIDGE_PORT)) ||
        skb_shared(skb) || (skb_headroom(skb) < dev->needed_headroom) || (skb_tailroom(skb) < dev->needed_tailroom))
	{
		if(wq_dbg_flag & WQ_DEBUG_PKT_XMIT) {
			iot_printf("copy non-linear skb to linear one\n");
		}
		skb_for_tx = skb_copy_expand(skb, dev->needed_headroom, dev->needed_tailroom, GFP_ATOMIC);
		if (!skb_for_tx) {
			return NETDEV_TX_BUSY;
		}
		dev_kfree_skb_any(skb);
	}
	else
	{
		skb_for_tx = skb;
	}
    if(wq_dbg_flag & WQ_DEBUG_PKT_XMIT)
    {
        iot_printf("skb tx buf len:%d\n", skb_for_tx->len);
    }
    //dump_bytes("wq_ndo_start_xmit", skb_for_tx->data, (skb_for_tx->len>0x20)?0x20:skb_for_tx->len);

    wuqi_dhcp_debug_msg((struct ethhdr *)skb_for_tx->data);

    if(strcmp(dev->name,MON_VIF_NAME)==0){
        char radio_ap[]={0x00,0x00,0x0e,0x00,0x02,0xc0,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00};
        int ethertype_offset;

        //dump_bytes("wq_ndo_start_xmit mon if", skb_for_tx->data, skb_for_tx->len);
        if (skb_for_tx->data[sizeof(radio_ap)]&IEEE80211_STYPE_QOS_DATA) {
            ethertype_offset=sizeof(radio_ap)+30+2;
        } else
            ethertype_offset=sizeof(radio_ap)+30;

        //iot_printf("%s ethertype_offset:%d\n",__func__,ethertype_offset);

        if((ethertype_offset < (skb_for_tx->len-1)) && skb_for_tx->data[ethertype_offset]==0x88 &&
                skb_for_tx->data[ethertype_offset+1]==0x8E){

            //remove radio AP and 802.11 hdr ,transfer to 802.3 format
            memcpy(&skb_for_tx->data[ethertype_offset-ETH_ALEN*2], &skb->data[18], ETH_ALEN);
            memcpy(&skb_for_tx->data[ethertype_offset-ETH_ALEN], &skb->data[24], ETH_ALEN);
            skb_pull(skb_for_tx,ethertype_offset-12);

            len = skb->len - 14; //payload only
            if ((ret = net_if_output(g_vif_ctx0->vap, skb_for_tx)) == -1)
            {
                iot_printf("%s: drop frame, len=%d\n", __func__, skb_for_tx->len);
                //free skb if fail
                dev_kfree_skb_any(skb);
                dev->stats.tx_dropped++;
            } else {
                dev->stats.tx_packets++;
                dev->stats.tx_bytes += len;//payload only
            }
        }else {
            //iot_printf("frame to mon_if dropped..\n");
            dev_kfree_skb_any(skb);
        }

        return NETDEV_TX_OK;
    }

    len = skb->len - 14; //payload only
    if ((ret = net_if_output(ndev_data->vif_ctx->vap, skb_for_tx)) == -1)
    {
        iot_printf("%s: drop frame, len=%d\n", __func__, skb_for_tx->len);
        //free skb if fail
        dev_kfree_skb_any(skb);
		dev->stats.tx_dropped++;
    }
	else
	{
	    dev->stats.tx_packets++;
		dev->stats.tx_bytes += len;//payload only
	}

    /* Dont forget to cleanup skb, as its ownership moved to xmit callback. */
    //dev_kfree_skb_any(skb);

    return NETDEV_TX_OK;
}

static int wq_ndo_open(struct net_device *dev)
{
    struct wuqi_ndev_priv_context *ndev_data;

    ndev_data = ndev_get_vif_context(dev);
    ndev_data->vif_ctx->if_up = 1;
    //ndev_data->vif_ctx->vap;

    //disable IFF_DONT_BRIDGE when interface up
    if (ndev_data->wdev.iftype == NL80211_IFTYPE_STATION && (dev->priv_flags & IFF_DONT_BRIDGE)) {
        dev->priv_flags &= ~IFF_DONT_BRIDGE;
    }

    iot_printf("interface up (vap:%p)", ndev_data->vif_ctx->vap);

#if 0
    if (connected) {
        netif_carrier_on(dev);
        netif_wake_queue(dev);
    } else {
        netif_carrier_off(dev);
    }
#else
    netif_carrier_off(dev);
#endif

    return 0;

}


static int wq_ndo_stop(struct net_device *dev)
{
    struct wuqi_ndev_priv_context *ndev_data;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
    static struct cfg80211_scan_info info = {
        /* if scan was aborted by user(calling cfg80211_ops->abort_scan) or by any driver/hardware issue - field should be set to "true"*/
        .aborted = false,
    };
#endif

    ndev_data = ndev_get_vif_context(dev);
    //ndev_data->vif_ctx->vap;
    ndev_data->vif_ctx->if_up = 0;

    iot_printf("interface %s down  (vap:%p)", dev->name, ndev_data->vif_ctx->vap);

    netif_stop_queue(dev);

    //TODO stop wproto

    if (ndev_data->vif_ctx->vap && ndev_data->vif_ctx->scan_request != NULL) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
        cfg80211_scan_done(ndev_data->vif_ctx->scan_request, &info);
#else
        cfg80211_scan_done(ndev_data->vif_ctx->scan_request, false);
#endif
        ndev_data->vif_ctx->scanreq_n_channels = 0;
        ndev_data->vif_ctx->scan_request = NULL;
        ndev_data->vif_ctx->vif_ctx_state &= (~VIF_STATE_SCANNING);
    }
    return 0;
}


static int wq_ndo_set_mac_address(struct net_device *ndev, void *addr)
{
    struct wuqi_ndev_priv_context *ndev_data;
    struct wuqi_vif_context *vif_ctx = NULL;
    struct sockaddr *sa = (struct sockaddr *)addr;
    ndev_data = ndev_get_vif_context(ndev);
    vif_ctx = ndev_data->vif_ctx;
    memcpy(vif_ctx->mac_addr, sa->sa_data, ETH_ALEN);
    memcpy(vif_ctx->ndev->dev_addr, vif_ctx->mac_addr, ETH_ALEN);
    iot_printf("mac address updated to %pM\n", vif_ctx->ndev->dev_addr);
    wuqi_vap_setmac(vif_ctx->vap, vif_ctx->mac_addr);

    return 0;
}

/* network devices structure */
static struct net_device_ops wuqi_ndev_ops = {
    .ndo_start_xmit = wq_ndo_start_xmit,
    .ndo_open = wq_ndo_open,
    .ndo_stop = wq_ndo_stop,
    .ndo_set_mac_address = wq_ndo_set_mac_address,
};


/* supported channels in 2ghz band */
static struct ieee80211_channel supported_channels_2ghz[] = {
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 1,
        .center_freq = 2412,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 2,
        .center_freq = 2417,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 3,
        .center_freq = 2422,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 4,
        .center_freq = 2427,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 5,
        .center_freq = 2432,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 6,
        .center_freq = 2437,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 7,
        .center_freq = 2442,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 8,
        .center_freq = 2447,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 9,
        .center_freq = 2452,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 10,
        .center_freq = 2457,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 11,
        .center_freq = 2462,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 12,
        .center_freq = 2467,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 13,
        .center_freq = 2472,
    },
    {
        .band = NL80211_BAND_2GHZ,
        .hw_value = 14,
        .center_freq = 2484,
    }
};

#define RATETAB_ENT(_rate, _rateid, _flags) {   \
    .bitrate    = (_rate),                  \
    .flags      = (_flags),                 \
    .hw_value   = (_rateid),                \
}
/* supported rates */
static struct ieee80211_rate supported_rates_2ghz[] = {
    RATETAB_ENT(10, 0x1, 0),
    RATETAB_ENT(20, 0x2, 0),
    RATETAB_ENT(55, 0x4, 0),
    RATETAB_ENT(110, 0x8, 0),
    RATETAB_ENT(60, 0x10, 0),
    RATETAB_ENT(90, 0x20, 0),
    RATETAB_ENT(120, 0x40, 0),
    RATETAB_ENT(180, 0x80, 0),
    RATETAB_ENT(240, 0x100, 0),
    RATETAB_ENT(360, 0x200, 0),
    RATETAB_ENT(480, 0x400, 0),
    RATETAB_ENT(540, 0x800, 0),
};

/* Structure that describes supported band of 2ghz. */
static struct ieee80211_supported_band nf_band_2ghz = {
    .ht_cap.cap = IEEE80211_HT_CAP_SGI_20, /* add other band capabilities if needed, like 40 width etc. */
    .ht_cap.ht_supported = true,

    .channels = supported_channels_2ghz,
    .n_channels = ARRAY_SIZE(supported_channels_2ghz),

    .bitrates = supported_rates_2ghz,
    .n_bitrates = ARRAY_SIZE(supported_rates_2ghz),
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)
static struct wireless_dev *wuqi_create_dummy_ndev(const char *name,struct wiphy *wiphy)
#else
static struct net_device *wuqi_create_dummy_ndev(const char *name,struct wiphy *wiphy)
#endif
{
    struct wuqi_vif_context *ret = NULL;
    //struct wuqi_wiphy_priv_context *wiphy_data = NULL;
    struct wuqi_ndev_priv_context *ndev_data = NULL;
    int err;

    /* allocate for vif_ctxfly context*/
    ret = (struct wuqi_vif_context *)wq_mem_alloc(sizeof(*ret), 0);
    if (!ret) {
        goto l2_error;
    }

    memset(ret, 0, sizeof(*ret));

    ret->wiphy = wiphy;
    /* allocate network device context. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,0)
    ret->ndev = alloc_netdev(sizeof(*ndev_data), name, NET_NAME_ENUM, ether_setup);
#else
    ret->ndev = alloc_netdev(sizeof(*ndev_data), name, ether_setup);
#endif
    if (ret->ndev == NULL) {
        goto l2_error_alloc_ndev;
    }

    g_wlan1_mon_ndev=ret->ndev;
    g_wlan1_mon_net=dev_net(g_wlan1_mon_ndev);

    /* fill private data of network context.*/
    ndev_data = ndev_get_vif_context(ret->ndev);
    ndev_data->vif_ctx = ret;
    ndev_data->vif_ctx->if_up = 0;

    /* fill wireless_dev context.
     * wireless_dev with net_device can be represented as inherited class of single net_device. */
    ndev_data->wdev.wiphy = ret->wiphy;
    ndev_data->wdev.netdev = ret->ndev;
    ndev_data->wdev.iftype = NL80211_IFTYPE_MONITOR;
    ret->ndev->ieee80211_ptr = &ndev_data->wdev;

    /* set network device hooks. It should implement ndo_start_xmit() at least. */
    ret->ndev->netdev_ops = &wuqi_ndev_ops;
    ret->ndev->needed_headroom += (TXPKT_MAC_HEADROOM_NEEDED + wq_ipc_headroom_size());
    ret->ndev->needed_tailroom += TXPKT_MAC_TAILROOM_NEEDED;

    err = register_netdevice(ret->ndev);

    if (err !=0 ) {
        goto l2_error_ndev_register;
    }
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)) 
    return ret->ndev->ieee80211_ptr;
#else
    return ret->ndev;
#endif

l2_error_ndev_register:
    free_netdev(ret->ndev);
l2_error_alloc_ndev:
    //    wiphy_unregister(ret->wiphy);
    //l2_error_wiphy_register:
    //    wiphy_free(ret->wiphy);
l2_error:
    return NULL;
}


static int wuqi_free_dummy_ndev(struct net_device *ndev)
{
    iot_printf("%s\n",__func__);

    netif_carrier_off(ndev);
    netif_stop_queue(ndev);
    unregister_netdev(ndev);
    free_netdev(ndev);
    return 0;
}


static struct wuqi_vif_context *wuqi_create_context_vif(const char *name,struct wiphy *wiphy,enum nl80211_iftype type, uint8_t mac[6])
{
    struct wuqi_vif_context *ret = NULL;
    struct wuqi_ndev_priv_context *ndev_data = NULL;
    int err;

    /* allocate for vif_ctxfly context*/
    ret = (struct wuqi_vif_context *)wq_mem_alloc(sizeof(*ret), 0);
    if (!ret) {
        goto l2_error;
    }

    memset(ret, 0, sizeof(*ret));
    ret->vap=NULL;

    ret->wiphy = wiphy;

    ret->wiphy->bands[NL80211_BAND_2GHZ] = &nf_band_2ghz;

    /* scan - if ur device supports "scan" u need to define max_scan_ssids at least. */
    ret->wiphy->max_scan_ssids = 69;
	ret->wiphy->max_scan_ie_len = 1024;
    ret->vif_ctx_state = VIF_STATE_IDLE;

    /* allocate network device context. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,0)
    ret->ndev = alloc_netdev(sizeof(*ndev_data), name, NET_NAME_ENUM, ether_setup);
#else
    ret->ndev = alloc_netdev(sizeof(*ndev_data), name, ether_setup);
#endif
    if (ret->ndev == NULL) {
        goto l2_error_alloc_ndev;
    }

    /* [Monitor mode] Change link-type to IEEE802_11 (802.11) for tcpdump capturing in correct format */
    if (type == NL80211_IFTYPE_MONITOR) {
        ret->ndev->type = ARPHRD_IEEE80211;
    }

    /* fill private data of network context.*/
    ndev_data = ndev_get_vif_context(ret->ndev);
    ndev_data->vif_ctx = ret;
    ndev_data->vif_ctx->if_up = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,9)
    ret->ndev->priv_destructor=wuqi_free_netdev;
#else
    ret->ndev->destructor=wuqi_free_netdev;
#endif
    /* fill wireless_dev context */
    ndev_data->wdev.wiphy = ret->wiphy;
    ndev_data->wdev.netdev = ret->ndev;
    ndev_data->wdev.iftype = type;
    ret->ndev->ieee80211_ptr = &ndev_data->wdev;

    /* set device object for net_device */
    /* SET_NETDEV_DEV(ret->ndev, wiphy_dev(ret->wiphy)); */

    /* set network device hooks. */
    ret->ndev->netdev_ops = &wuqi_ndev_ops;

    /* net_device initialization. */
    ret->ndev->needed_headroom += (TXPKT_MAC_HEADROOM_NEEDED + wq_ipc_headroom_size());
    ret->ndev->needed_tailroom += TXPKT_MAC_TAILROOM_NEEDED;
    memcpy(ret->ndev->dev_addr, mac, 6);

    /* register network device */
    err = register_netdevice(ret->ndev);

    if (err !=0 ) {
        goto l2_error_ndev_register;
    }

    return ret;
l2_error_ndev_register:
    free_netdev(ret->ndev);
l2_error_alloc_ndev:
    //    wiphy_unregister(ret->wiphy);
    //l2_error_wiphy_register:
    //    wiphy_free(ret->wiphy);
l2_error:
    return NULL;
}

/* interface combination */
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(3,16,0)))
static const struct ieee80211_iface_limit if_limits[2] = {
    {.max = 2,
     .types = BIT(NL80211_IFTYPE_STATION)|
              BIT(NL80211_IFTYPE_MONITOR)
    },
    {.max = 1,
     .types = BIT(NL80211_IFTYPE_AP)}
};

static const struct ieee80211_iface_combination if_comb = {
    .limits = if_limits,
    .n_limits = ARRAY_SIZE(if_limits),
    .max_interfaces = 3,
    .num_different_channels = 2,
    .beacon_int_infra_match = true,
};
#endif

/* creates wiphy context and net_device with wireless_dev */
static struct wuqi_vif_context *wuqi_create_context(void *wq_ipc)
{
    struct wuqi_vif_context *ret = NULL;
    struct wuqi_wiphy_priv_context *wiphy_data = NULL;
    struct wuqi_ndev_priv_context *ndev_data = NULL;
    char conutry_code[3] = {0};

    /* allocate for vif_ctxfly context*/
    ret = (struct wuqi_vif_context *)wq_mem_alloc(sizeof(*ret), 0);
    if (!ret) {
        goto l_error;
    }

    memset(ret, 0, sizeof(*ret));
    ret->vap=NULL;

    /* allocate wiphy context */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0)
    ret->wiphy = wiphy_new_nm(&wuqi_cfg_ops, sizeof(struct wuqi_wiphy_priv_context), WIPHY_NAME);
#else
    ret->wiphy = wiphy_new(&wuqi_cfg_ops, sizeof(struct wuqi_wiphy_priv_context));
#endif
    if (ret->wiphy == NULL) {
        goto l_error_wiphy;
    }

    ret->wiphy->mgmt_stypes = wuqi_txrx_stypes;
    ret->wiphy->signal_type = CFG80211_SIGNAL_TYPE_MBM;
    ret->wiphy->cipher_suites = wuqi_cipher_suites;
    ret->wiphy->n_cipher_suites = ARRAY_SIZE(wuqi_cipher_suites);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,3,0)
    ret->wiphy->flags |= WIPHY_FLAG_HAVE_AP_SME;
#endif
    /* save vif_ctxfly context in wiphy private data. */
    wiphy_data = wiphy_get_vif_context(ret->wiphy);
    wiphy_data->wq_ipc = wq_ipc;
    wiphy_data->vif_ctx = ret;

    /* set device object as wiphy "parent", I dont have any device yet. */
    /* set_wiphy_dev(ret->wiphy, dev); */

    /* wiphy type */
    ret->wiphy->interface_modes = BIT(NL80211_IFTYPE_STATION) | BIT(NL80211_IFTYPE_AP) | BIT(NL80211_IFTYPE_MONITOR);

#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(3,16,0)))
    ret->wiphy->iface_combinations = &if_comb;
    ret->wiphy->n_iface_combinations = 1;
#endif

    ret->wiphy->bands[NL80211_BAND_2GHZ] = &nf_band_2ghz;

    /* scan - if ur device supports "scan" u need to define max_scan_ssids at least. */
    ret->wiphy->max_scan_ssids = 69;
	ret->wiphy->max_scan_ie_len = 1024;
    /* register wiphy */
    if (wiphy_register(ret->wiphy) < 0) {
        goto l_error_wiphy_register;
    }

    /* allocate network device context. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,0)
    ret->ndev = alloc_netdev(sizeof(*ndev_data), NDEV_NAME, NET_NAME_ENUM, ether_setup);
#else
    ret->ndev = alloc_netdev(sizeof(*ndev_data), NDEV_NAME, ether_setup);
#endif
    if (ret->ndev == NULL) {
        goto l_error_alloc_ndev;
    }

    /* fill private data of network context.*/
    ndev_data = ndev_get_vif_context(ret->ndev);
    ndev_data->vif_ctx = ret;

    /* fill wireless_dev context.
     * wireless_dev with net_device can be represented as inherited class of single net_device. */
    ndev_data->wdev.wiphy = ret->wiphy;
    ndev_data->wdev.netdev = ret->ndev;
    ndev_data->wdev.iftype = NL80211_IFTYPE_STATION;
    ret->ndev->ieee80211_ptr = &ndev_data->wdev;

    /* set device object for net_device */
    /* SET_NETDEV_DEV(ret->ndev, wiphy_dev(ret->wiphy)); */

    /* set network device hooks. It should implement ndo_start_xmit() at least. */
    ret->ndev->netdev_ops = &wuqi_ndev_ops;

    /* Add here proper net_device initialization. */
    ret->ndev->needed_headroom += (TXPKT_MAC_HEADROOM_NEEDED + wq_ipc_headroom_size());
    ret->ndev->needed_tailroom += TXPKT_MAC_TAILROOM_NEEDED;

    /* reg domain setting */
    do {
        if (country) {
            if (strlen(country) == 2) {
                strncpy(conutry_code, country, sizeof(conutry_code));
                break;
            }
        }
        conutry_code[0] = 'C';
        conutry_code[1] = 'N';
        conutry_code[2] = '\0';
    } while(0);
    printk("country: %s\n", conutry_code);
    regulatory_hint(ret->wiphy, conutry_code);

    /* register network device */
    //if (register_netdev(ret->ndev)) {
    //    goto l_error_ndev_register;
    //}

    return ret;
//l_error_ndev_register:
   // free_netdev(ret->ndev);
l_error_alloc_ndev:
    wiphy_unregister(ret->wiphy);
l_error_wiphy_register:
    wiphy_free(ret->wiphy);
l_error_wiphy:
    kfree(ret);
l_error:
    return NULL;
}

static void wuqi_ctx_free(struct wuqi_vif_context *ctx) {
    if (ctx == NULL) {
        return;
    }

#ifdef WQ_SIM_HOST_ONLY
    {
        struct wuqi_wiphy_priv_context *p =
            (struct wuqi_wiphy_priv_context *)wiphy_priv(ctx->wiphy) ;
        wq_ipc_detach(p->wq_ipc);
    }
#endif

    //unregister_netdev(ctx->ndev);
    free_netdev(ctx->ndev);
    wiphy_unregister(ctx->wiphy);
    wiphy_free(ctx->wiphy);
    kfree(ctx);
}

static struct task_struct *log2file_task = NULL;
int thread_end_flag = 0;

int log2file_func(void *data){
        while(1){
            set_current_state(TASK_UNINTERRUPTIBLE);
            if(kthread_should_stop())
                break;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,12,0)
if(log2file.write_flag){
                if(log2file.write_to_fp2_flag){
                    kernel_write(log2file.fp2, cache_log_buf2, strlen(cache_log_buf2), &(log2file.fp2->f_pos));
                }
                else
                    kernel_write(log2file.fp1, cache_log_buf2, strlen(cache_log_buf2), &(log2file.fp1->f_pos));
                log2file.write_flag = 0;
            }

#else
            if(log2file.write_flag){
                if(log2file.write_to_fp2_flag){
                    vfs_write(log2file.fp2, cache_log_buf2, strlen(cache_log_buf2), &(log2file.fp2->f_pos));
                }
                else
                    vfs_write(log2file.fp1, cache_log_buf2, strlen(cache_log_buf2), &(log2file.fp1->f_pos));
                log2file.write_flag = 0;
            }
#endif
            else{
                schedule_timeout(10);
            }
            if(thread_end_flag){
                return 0;
            }
        }

        return 0;
}

static int create_thread()
{
    int err;

    log2file_task = kthread_create(log2file_func, NULL, "log2file_task");
    if(IS_ERR(log2file_task)){
        printk("Unable to start kernel thread. ");
        err = PTR_ERR(log2file_task);
        log2file_task = NULL;
        return err;
    }

    cache_log_buf1 = (char *)wq_mem_alloc(CACHE_LOG_LEN, 0);
    cache_log_buf2 = (char *)wq_mem_alloc(CACHE_LOG_LEN, 0);
    if(!cache_log_buf1 || !cache_log_buf2){
        iot_printf("chache buf malloc fail\n");
        return -ENOMEM;
    }
    wake_up_process(log2file_task);
    return 0;
}


static void hawk_proc_show_usage(uint8_t proc_var)
{
    char dbg[] = "driver/hawk_usb/dbg";
    char fwdbg[] = "driver/hawk_usb/fwdbg";
    char regdbg[] = "driver/hawk_usb/regdbg";
    char pkt_dump[]  = "Tx/Rx packet dump";
    char reg_dump[]  = "dump register on hawk console";
    char cpu_usage[] = "show cpu usage on hawk console";

    printk("Example of usage:\n");
    switch (proc_var) {
        case WQ_PROC_DBG:
            printk("echo \"0x%04x\" > %s (set wq_dbg_flag)\n", WQ_DEBUG_NONE, dbg);
            printk("echo \"0x%04x\" > %s (set wq_dbg_flag)\n", WQ_DEBUG_ALL, dbg);
            break;
        case WQ_PROC_FWDBG:
            printk("echo \"0x10000\" > %s (enable %s)\n", fwdbg, pkt_dump);
            printk("echo \"0x10001\" > %s (disalbe %s)\n",fwdbg, pkt_dump);
            printk("echo \"0x20000\" > %s (%s)\n", fwdbg, reg_dump);
            printk("echo \"0x30000\" > %s (disable %s)\n", fwdbg, cpu_usage);
            printk("echo \"0x30002\" > %s (enable %s)\n",  fwdbg, cpu_usage);
            break;
        case WQ_PROC_REGDBG:
            printk("echo \"0x41000024\" > %s (read register)\n", regdbg);
            printk("echo \"0x41000024 0x000f007f\" > %s (write register with value)\n", regdbg);
            break;
        default:
            break;
    }
}

static ssize_t hawk_proc_write(struct file *file, const char __user *buffer,
			      size_t count, loff_t *pos)
{
    char tmp_buf[8];
    long hawk_proc_dbg = 0;
    int ret;

    if (!count)
        return 0;

    if (count != 7) {
        hawk_proc_show_usage(WQ_PROC_DBG);
        return -EINVAL;
    }

    if (copy_from_user(tmp_buf, buffer, count))
        return -EFAULT;

    tmp_buf[7] = 0;
    //printk("hawk_proc_dbg str=%s %d\n", tmp_buf, count);

    if ((ret = kstrtol(tmp_buf, 0, &hawk_proc_dbg)) == 0)
    {
        wq_dbg_flag = hawk_proc_dbg;
        printk("wq_dbg_flag = 0x%04x\n", (unsigned int)wq_dbg_flag);
        if(wq_dbg_flag & WQ_DEBUG_LOG2FILE){
            log2file.fp1 = filp_open("/tmp/log2file_1.txt", O_CREAT | O_RDWR, 0666);
            log2file.fp2 = filp_open("/tmp/log2file_2.txt", O_CREAT | O_RDWR, 0666);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,12,0)
            set_fs(KERNEL_DS);
#endif
            /*create thread to process log2file*/
            create_thread();
        }

    }

    return count;
}

#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
//debug for param testing if usb_rx using cpu(not dma)
static ssize_t hawk_proc_usbrx_cpu_write(struct file *file, const char __user *buffer,
                size_t count, loff_t *pos)
{
    char tmp_buf[32];
    int ret;

    if (!count)
        return 0;

    if (copy_from_user(tmp_buf, buffer, count))
        return -EFAULT;

    tmp_buf[count] = 0;
    //rx_skb_count_limit: max pkts of driver hold
    //rx_done_count_wk_limit: check whether tx pkts to be sent every N rx pkts
    //submit_rxurb_limit_h: upper threshold of dynamic rx urb algorithm
    //submit_rxurb_limit_l: lower threshold of dynamic rx urb algorithm
    ret = sscanf(tmp_buf, "%d %d %d %d", &rx_skb_count_limit, &rx_done_count_wk_limit, 
    &submit_rxurb_limit_h, &submit_rxurb_limit_l);
    if (ret != 4) {
        printk("error parameter\n");
        return count;
    }
    iot_printf("rx_skb_count_limit:%d, rx_done_count_wk_limit= %d, submit_rxurb_limit_h=%d,submit_rxurb_limit_l=%d\n",
    rx_skb_count_limit, rx_done_count_wk_limit, submit_rxurb_limit_h,submit_rxurb_limit_l);

    return count;
}
#endif

static ssize_t hawk_proc_fwdbg_write(struct file *file, const char __user *buffer,
			      size_t count, loff_t *pos)
{
    char tmp_buf[12];
    long fwdbg = 0;
    int ret;

    if (!count)
        return 0;

    if (count != 8) {
        hawk_proc_show_usage(WQ_PROC_FWDBG);
        return -EINVAL;
    }

    if (copy_from_user(tmp_buf, buffer, count))
        return -EFAULT;

    tmp_buf[8] = 0;
    //printk("hawk_proc_dbg str=%s %d\n", tmp_buf, count);

    if ((ret = kstrtol(tmp_buf, 0, &fwdbg)) == 0)
    {
        printk("fwdbg = 0x%x\n", (unsigned int)fwdbg);

        wq_set_fw_dbg(fwdbg);
    }

    return count;
}

void wq_ch_power_set(struct net_device *dev, int *freq, uint8_t *power, int ch_nbr)
{
    struct wuqi_ndev_priv_context * ndev_data = ndev_get_vif_context(dev);
	struct wuqi_vif_context  *vif_ctx = ndev_data->vif_ctx;

    wuqi_user_chan_power(vif_ctx->vap, freq, power, ch_nbr);
}

void set_rate_power(int8_t *power_list)
{
    wq_rate_pwr_notify(power_list);
}

void set_rate_power_srrc(int8_t *power_list)
{
    wq_rate_pwr_srrc_notify(power_list);
}

extern int wq_mp_efuse_notify(uint8_t, struct wuqi_mptest_efuse_ctx *efuse_ctx);
extern void wq_usb_reset_chip(void);

static ssize_t hawk_proc_mp_write(struct file *file, const char __user *buffer,
    size_t count, loff_t *pos)
{
    char tmp_buf[100];
    long chan_info = 0;
    int ret;
    int chan_freq;
    long rate_info = 0;
    long tx_count = 100;
    int phy_perf_flag = 0; //for phy performance test
    uint32_t crc_err_temp;
    uint8_t sw_ch_cali;
    int pkt_len = 478;
    int tx_ms = 0;
    int tx_us = 0;
    long gain_info = 0;
    long gain_ctrl_info = 0;

    if ((count > 100) || (count == 0)) {
        return 0;
    }

    if (copy_from_user(tmp_buf, buffer, count))
        return -EFAULT;
    tmp_buf[count] = '\0';
    printk("[mp_cmd]:%s\n",tmp_buf);

    if (!memcmp(tmp_buf, "chip_rst", strlen("chip_rst")))
    {
        wq_usb_reset_chip();  
    }
    
    if(!memcmp(tmp_buf, "alive_stop", strlen("alive_stop")))
    {
        send_keepalive_flag = 0;
    }
    if(!memcmp(tmp_buf, "alive_start", strlen("alive_start")))
    {
        send_keepalive_flag = 1;
    }

    if(!memcmp(tmp_buf, "channel", strlen("channel")))
    {
        if (strstr(tmp_buf, "dpd")) {
            ret = kstrtol(tmp_buf + strlen("channel_dpd") + 1, 0, &chan_info);
            sw_ch_cali = 0;
        } else {
            ret = kstrtol(tmp_buf + strlen("channel") + 1, 0, &chan_info);
            sw_ch_cali = 1;
        }
        if (ret != 0)
        {
            printk("error command\n");
            return -EINVAL;
        } else if(chan_info < 1 || chan_info > 14){
            printk("error parameter: chan 1 - 14\n");
            return -EINVAL;
        } else {
            printk("chan_info = %d\n", (unsigned int)chan_info);
            chan_freq = ieee80211_channel_to_frequency(chan_info, NL80211_BAND_2GHZ);
            wuqi_config_channel_dpd(g_vif_ctx1->vap , chan_freq, chan_info, sw_ch_cali);
        }
    }

    if(!memcmp(tmp_buf, "rx_reset", strlen("rx_reset")))
    {
        rxcount = 0;
        if (rx_start == 0)
        {
            mp_ctxt.rx_ce_start_cnt = 0;
            mp_ctxt.rx_ce_stop_cnt = 0;
        } else {
            ret = wq_mp_crc_err_notify(&crc_err_temp);
            if (ret) {
                printk("get crc err from fw failed!\n");
                crc_err_temp = 0;
            }
            mp_ctxt.rx_ce_start_cnt = crc_err_temp;
        }
        mp_ctxt.rx_reset_flag = 1;
    }

    if(!memcmp(tmp_buf, "rx_start", strlen("rx_start")))
    {
        if (rx_start == 0)
        {
            if (mp_ctxt.rx_ce_start_cnt == 0 || mp_ctxt.rx_reset_flag == 1)
            {
                ret = wq_mp_crc_err_notify(&crc_err_temp);
                if (ret) {
                    printk("get crc err from fw failed!\n");
                    crc_err_temp = 0;
                }
                mp_ctxt.rx_ce_start_cnt = crc_err_temp;
            }
        }
        mp_ctxt.rx_reset_flag = 0;
        rx_start = 1;
    }

    if(!memcmp(tmp_buf, "rx_stop", strlen("rx_stop")))
    {
        if (rx_start == 1)
        {
            if (mp_ctxt.rx_reset_flag == 0)
            {
                ret = wq_mp_crc_err_notify(&crc_err_temp);
                if (ret) {
                    printk("get crc err from fw failed!\n");
                    crc_err_temp = 0;
                }
                mp_ctxt.rx_ce_stop_cnt = crc_err_temp;
            } else {
                mp_ctxt.rx_ce_stop_cnt = 0;
            }
        }
        rx_start = 0;
    }

    if(!memcmp(tmp_buf, "init", strlen("init")))
    {
        wuqi_vap_change_mode(g_vif_ctx1->vap, 5); //monitor mode
        wq_mp_status_notify(1); //mp start
        WQ_MAC_WRITE(0x42100184, 0x00002915); //disable cca
    }

    if(!memcmp(tmp_buf, "deinit", strlen("deinit")))
    {
        wuqi_vap_change_mode(g_vif_ctx1->vap, 1); //sta mode
        wuqi_config_channel(g_vif_ctx1->vap, 0);
        WQ_MAC_WRITE(0x42100184, 0x00002910); //enable cca
    }

    if(!memcmp(tmp_buf, "rate", strlen("rate")))
    {
        if ((ret = kstrtol(tmp_buf + strlen("rate") + 1, 0, &rate_info)) == 0)
        {
            if(((rate_info >> 4 == 0x08) && (!(rate_info  << 4 & 0x80))) ||
                ((rate_info >> 4 == 0x0a) && (!(rate_info << 4 & 0x80))) ||
                ((rate_info >> 4 == 0) && (!(rate_info << 4 & 0xc0))) ||
                (rate_info == 0xe7))
            {
                printk("set rate correct:%02x\n", (uint8_t)rate_info);
                wuqi_set_fixed_rate(g_vif_ctx1->vap, rate_info);
            }
            else
            {
                printk("set rate error:%02x\n", (uint8_t)rate_info);
                return -EINVAL;
            }
        }
    }
    //enable tx under monitor mode cmd: echo "tx_start [tx_count]" > /proc/driver/hawk_usb/mp_test
    //                                  tx_count: number of packets sent(0 means continuously sending)
    //enable tx under monitor mode for phy performance test cmd: echo "phy tx_start [tx_count]" > /proc/driver/hawk_usb/mp_test
    //disable tx under monitor mode cmd: echo "tx_stop" > /proc/driver/hawk_usb/mp_test
    if (strstr(tmp_buf, "tx"))
    {
        if (strstr(tmp_buf, "start"))
        {
            monitor_tx_flag = 1;
            printk("monitor_tx_flag = %d\n", (int) monitor_tx_flag);
            if (strstr(tmp_buf, "phy tx")) { //send pkt of length 63 every usleep_range(50, 100)
                phy_perf_flag = 1;
                ret = kstrtol(tmp_buf + strlen("phy tx_start") + 1, 0, &tx_count);
            } else if(strstr(tmp_buf, "phy2 tx")) { //50%+ duty ratio
                phy_perf_flag = 2;
                ret = kstrtol(tmp_buf + strlen("phy2 tx_start") + 1, 0, &tx_count);
            } else if(strstr(tmp_buf, "phy3 tx")) { // user define
                phy_perf_flag = 3;
                ret = sscanf(tmp_buf + strlen("phy3 tx_start") + 1, "%d %d %dms %dus", (int *)&tx_count, &pkt_len, &tx_ms, &tx_us);
                if (ret != 4) {
                    printk("error parameter\n");
                    return count;
                }
                if (pkt_len < 63 || pkt_len > 980 || (((tx_ms < 1 || tx_ms > 50) && tx_us == 0)|| ((tx_us < 50 || tx_us > 950) && tx_ms == 0))){
                    printk("error parameter: pkt_len:63-980, tx_ms:1-50 && tx_us==0  ||  tx_us:50-950 && tx_ms==0\n");
                    return count;
                }
            } else {
                ret = kstrtol(tmp_buf + strlen("tx_start") + 1, 0, &tx_count);
            }
            printk("tx_count = %d\n", (unsigned int) tx_count);
            wq_proc_tx_monitor(tx_count, phy_perf_flag, pkt_len, tx_ms, tx_us);
        }
        if (strstr(tmp_buf, "stop"))
        {
            monitor_tx_flag = 0;
            printk("monitor_tx_flag = %d\n", (int) monitor_tx_flag);
        }
    }

    if (strstr(tmp_buf, "tone")) {
        uint8_t enable = 0;
        int freq, num, amp;

        if (strstr(tmp_buf, "start")) {
            int ret;
            enable = 1;
            ret = sscanf(tmp_buf + strlen("tone start") + 1, "%d %d %d", &freq, &num, &amp);
            if (ret != 3) {
                printk("error command\n");
                return count;
            }
            if ((freq < 2412) || (freq > 2484)) {
                printk("error parameter freq 2412 - 2484");
                return count;
            }
            if ((amp < 0) || (amp > 2047)) {
                printk("error parameter amp 0 - 2047");
                return count;
            }
            if ((num < -128) || (num > 127)) {
                printk("error parameter num -128 - 127");
                return count;
            }
            printk("phy tone gen start on %d MHz num:%d amp:%d\n", freq, num, amp);
        } else if (strstr(tmp_buf, "stop")) {
            enable = 0;
            printk("phy tone gen stop\n");
        } else {
            printk("error command\n");
            return count;
        }

        wq_mp_tone_notify(enable, (uint16_t)freq, (uint8_t)num, (uint16_t)amp);
    }

    if (strstr(tmp_buf, "freq_dev")) {
        int set, ret;
        uint16_t freq_dev;

        if (strstr(tmp_buf, "set")) {
            int freq;
            set = 1;
            ret = sscanf(tmp_buf + strlen("freq_dev set") + 1, "%d", &freq);
            if (ret != 1) {
                printk("error command %d\n", ret);
                return count;
            }
            if ((freq < 0) || (freq > 127)) {
                printk("error parameter 0-127\n");
                return count;
            }
            freq_dev = (uint16_t)freq;
            printk("set freq deviation:%d\n", freq_dev);
            wq_mp_freq_dev_notify(set, &freq_dev);
        } else if (strstr(tmp_buf, "get")) {
            set = 0;
            ret = wq_mp_freq_dev_notify(set, &freq_dev);
            if (ret) {
                printk("get freq deviation failed!\n");
            } else {
                printk("get freq deviation: %d\n", freq_dev);
            }
        } else {
            printk("error command\n");
            return count;
        }
    }

    if(!memcmp(tmp_buf, "power_ctrl", strlen("power_ctrl"))) {
        int ret;
        uint8_t power_ctrl;

        if (strstr(tmp_buf, "set")) {
            int power;
            ret = sscanf(tmp_buf + strlen("ctrl_power set") + 1, "%d", &power);
            if (ret != 1) {
                printk("error command %d\n", ret);
                return count;
            }
            if ((power != 0) && (power != 1)) {
                printk("error parameter 0/1\n");
                return count;
            }
            power_ctrl = (uint8_t)power;
            printk("set power_ctrl: %d\n", power_ctrl);
            wq_mp_power_ctrl_notify(&power_ctrl);
        } 
        else {
            printk("error command\n");
            return count;
        }
    }
    else if (strstr(tmp_buf, "power")) {
        int set, ret, rate_idx;

        if (strstr(tmp_buf, "set")) {
            int power;
            int8_t rate_power;
            set = 1;
            ret = sscanf(tmp_buf + strlen("power set") + 1, "%d %d", &rate_idx, &power);
            if (ret != 2) {
                printk("error command %d\n", ret);
                return count;
            }
            rate_power = (int8_t)power;
            printk("set rate_idx:%d power:%d\n", rate_idx, rate_power);
            wq_mp_power_notify(set, (uint8_t)rate_idx, &rate_power);
        } else if (strstr(tmp_buf, "get")) {
            int8_t power;
            set = 0;
            ret = sscanf(tmp_buf + strlen("power get") + 1, "%d", &rate_idx);
            if (ret != 1) {
                printk("error command %d\n", ret);
                return count;
            }
            ret = wq_mp_power_notify(set, (uint8_t)rate_idx, &power);
            if (ret) {
                printk("get power of rate_idx %d failed!\n", rate_idx);
            } else {
                printk("get rate_idx:%d power:%d\n", rate_idx, power);
            }
        } else {
            printk("error command\n");
            return count;
        }
    }

    if(!memcmp(tmp_buf, "macaddr", strlen("macaddr")))
    {
        int set, mac[ETH_ALEN], ret;
        uint8_t i;

        if (strstr(tmp_buf, "set")) {
            set = 1;
            ret = sscanf(tmp_buf + strlen("macaddr set") + 1, "%x:%x:%x:%x:%x:%x",
            &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
            if (ret != ETH_ALEN) {
                printk("error mac addr format\n");
                return count;
            }
            for (i = 0; i < ETH_ALEN; i++) {
                mp_ctxt.mac_addr[i] = (uint8_t)mac[i];
            }
        } else if (strstr(tmp_buf, "get")) {
            set = 0;
        } else {
            printk("error command\n");
            return count;
        }

        ret = wq_mp_mac_notify(set, mp_ctxt.mac_addr);
        if (ret) {
            printk("set mac addr failed\n");
        } else {
            if (set == 1) {
                printk("set mac addr %pM success\n", mp_ctxt.mac_addr);
            } else {
                printk("get mac addr %pM\n", mp_ctxt.mac_addr);
            }
        }
    }

    if(!memcmp(tmp_buf, "efuse", strlen("efuse")))
    {
        int set, ret, efuse_info[16], offset, space_len;
        uint8_t i = 0;
        char buf_info[32], tmp_info[2];
        struct wuqi_mptest_efuse_ctx *efuse_ctx = (struct wuqi_mptest_efuse_ctx *)&mp_ctxt.efuse_ctx;

        if(strstr(tmp_buf, "set")) {
            set = 1;
            memset(efuse_ctx->efuse_info, '\0', sizeof(efuse_ctx->efuse_info));
            memset(buf_info, '\0', sizeof(buf_info));
            if (strstr(tmp_buf, "user")) {
                sscanf(tmp_buf + strlen("efuse user set") + 1, "%d %s", &offset, buf_info);
                if(offset < 90 || offset > 127) {
                    printk("error offset(90-127)\n");
                    return count;
                }
            } else {
                sscanf(tmp_buf + strlen("efuse set") + 1, "%d %s", &offset, buf_info);
            }
            if(offset <= 127)
            {
                space_len = 128 - offset;
                if((strlen(buf_info) % 2) != 0 || strlen(buf_info) == 0 || (strlen(buf_info) / 2) > space_len) {
                    printk("error info lenth\n");
                    return count;
                }
                efuse_ctx->info_len = (uint8_t)(strlen(buf_info) / 2) + 2;
                efuse_ctx->efuse_info[0] = (uint8_t)offset;

                while(i < efuse_ctx->info_len - 2) {
                    sscanf(buf_info + 2 * i, "%2s", tmp_info);
                    sscanf(tmp_info, "%x", &efuse_info[i]);
                    efuse_ctx->efuse_info[i + 1] = (uint8_t)efuse_info[i];
                    i++;
                }
            }
            else {
                printk("error offset(0-127)\n");
                return count;
            }
         } else if(strstr(tmp_buf, "get")) {
            set = 0;
            memset(efuse_ctx->rsp_efuse_info, '\0', sizeof(efuse_ctx->rsp_efuse_info));
            memset(efuse_ctx->efuse_info, '\0', sizeof(efuse_ctx->efuse_info));
            sscanf(tmp_buf + strlen("efuse get") + 1, "%d %d", &offset, &ret);
            if(offset <= 127 )
            {
                space_len = 128 - offset;
                if(ret > 0 && ret <= 16 && ret <= space_len) {
                    efuse_ctx->efuse_info[0] = (uint8_t) offset;
                    efuse_ctx->info_len = (uint8_t) ret;
                } else {
                    printk("error rsp_len\n");
                    return count;
                }
            }
            else {
                printk("error offset(0-127)\n");
                return count;
           }
         } else {
            printk("error command\n");
            return count;
         }

        ret = wq_mp_efuse_notify(set, efuse_ctx);
        if(ret) {
            printk("set efuse fail\n");
        } else {
            if(set == 1) {
                printk("set efuse to %d success\n", efuse_ctx->efuse_info[0]);
            } else {
                for(i = 0; i < efuse_ctx->info_len; i++) {
                    printk("get efuse %x from %d\n", efuse_ctx->rsp_efuse_info[i], offset + i);
               }
           }
       }
    }

    if (strstr(tmp_buf, "calib set")) {
        wq_mp_cali_efuse_notify(1, NULL, 0);
    }

    if(strstr(tmp_buf, "dpd_ctrl set")) {
        int ret, dpd;
        uint8_t dpd_ctrl;

        ret = sscanf(tmp_buf + strlen("dpd_ctrl set") + 1, "%d", &dpd);
        if (ret != 1) {
            printk("error command %d\n", ret);
            return count;
        }
        if (dpd != 0) {
            printk("error parameter: 0-disable\n");
            return count;
        }
        dpd_ctrl = (uint8_t)dpd;
        printk("set dpd_ctrl: %d\n", dpd_ctrl);
        wq_mp_dpd_ctrl_notify(&dpd_ctrl);
    }

    if(strstr(tmp_buf, "set_digit_gain")) {
        int ret, freq, mode;
        ret = sscanf(tmp_buf + strlen("set_digit_gain") + 1, "%d %d", &mode, &freq);
        if(ret != 2) {
            printk("error command %d\n", ret);
            return count;
        }
        if((mode < 0) || (mode > 2)) {
            printk("error parameter mode: 0-2\n");
            return count;
        }
        if ((freq < 2412) || (freq > 2484)) {
            printk("error parameter freq 2412 - 2484");
            return count;
        }
        printk("set digit_gain: mode = %d, freq = %d\n", mode, freq);
        wq_mp_digit_gain_notify((uint8_t)mode, (uint16_t)freq);
    }

    if(strstr(tmp_buf, "set_rate_pwr")) {
        int8_t pwr[12] = {0};
        ret = sscanf(tmp_buf + strlen("set_rate_pwr") + 1, "%hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd",
            &pwr[0], &pwr[1], &pwr[2], &pwr[3], &pwr[4], &pwr[5], &pwr[6], &pwr[7], &pwr[8], &pwr[9], &pwr[10], &pwr[11]);
        if(ret != 12) {
            printk("error command %d\n", ret);
            return count;
        }
        set_rate_power(pwr);
    }

    
    if(strstr(tmp_buf, "set_srrc_rate_pwr")) {
        int8_t pwr[24] = {0};
        ret = sscanf(tmp_buf + strlen("set_srrc_rate_pwr") + 1,
        "%hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd", 
            &pwr[0], &pwr[1], &pwr[2], &pwr[3], &pwr[4], &pwr[5], &pwr[6], &pwr[7], &pwr[8], &pwr[9], &pwr[10], &pwr[11],
            &pwr[12], &pwr[13], &pwr[14], &pwr[15], &pwr[16], &pwr[17], &pwr[18], &pwr[19], &pwr[20], &pwr[21], &pwr[22], &pwr[23]);
        if(ret != 24) {
            printk("error command %d\n", ret);
            return count;
        }
        set_rate_power_srrc(pwr);
    }
    /* config reference base gain for different rf matching */
    /* cmd  echo "gain 30" > /proc/driver/hawk_usb/mp_test   */
    if(!memcmp(tmp_buf, "gain", strlen("gain")))
    {
        ret = kstrtol(tmp_buf + strlen("gain") + 1, 0, &gain_info);

        if (ret != 0)
        {
            printk("error command\n");
            return -EINVAL;
        }
        else
        {
            printk("gain_info = %d\n", (uint8_t)gain_info);
            wq_mp_gain_notify((uint8_t)gain_info);
        }
    }

    /* config tx gain control mode */
    /* cmd  echo "ctrl_gain 0/1/2" > /proc/driver/hawk_usb/mp_test   */
    /* 0: power control off; 1: efuse gain control; 2: efuse gain control + tpc*/
    if(!memcmp(tmp_buf, "ctrl_gain", strlen("ctrl_gain")))
    {
        ret = kstrtol(tmp_buf + strlen("ctrl_gain") + 1, 0, &gain_ctrl_info);

        if (ret != 0)
        {
            printk("error command\n");
            return -EINVAL;
        }
        else if(gain_ctrl_info <0 || gain_ctrl_info >2)
        {
            printk("Invalid value!\n");
            return -EINVAL;
        }
        else
        {
            printk("gain_ctrl_info = %d\n", (uint8_t)gain_ctrl_info);
            wq_mp_gain_ctrl_notify((uint8_t)gain_ctrl_info);
        }
    }

    /* cmd  echo "set_ref_gain [ref_gain1] [ref_gain2] [ref_gain3]" > /proc/driver/hawk_usb/mp_test   */
    if (!memcmp(tmp_buf, "set_ref_gain", strlen("set_ref_gain")))
    {
        uint32_t reg_gain[3];
        ret = sscanf(tmp_buf+strlen("set_ref_gain")+1, "%d %d %d", &reg_gain[0], &reg_gain[1], &reg_gain[2]);
        if (ret != 3)
        {
            printk("error command\n");
            return -EINVAL;
        }
        wq_mp_set_gain_notify(reg_gain);
    }

    return count;
}

static ssize_t hawk_proc_mp_socinfo_read(struct file *file, char __user *buffer,
    size_t count, loff_t *pos)
{
    char numstr[64];
    char tmp_buf[4];
    int ret, i;
    uint8_t soc_info[MP_SOC_INFO_LEN];

    if(*pos> 0){
        return 0;
    }
    printk("[mp_cmd]:%s\n",__func__);

    ret = wq_mp_socinfo_notify(soc_info);
    if (ret) {
        printk("read soc info fail\n");
        return -1;
    }
    memset(numstr, '\0', sizeof(numstr));
    memset(tmp_buf, '\0', sizeof(tmp_buf));

    for(i = 0; i < MP_SOC_INFO_LEN; i++) {
        snprintf(tmp_buf, 4, "%02x ", soc_info[i]);
        strncat(numstr, tmp_buf, 3);
        //printk("soc_info[%d] 0x%x, tmp_buf:%s, numstr:%s\n", i, soc_info[i], tmp_buf, numstr);
    }
    numstr[strlen(numstr)]='\n';

    if(!copy_to_user(buffer, numstr, strlen(numstr) + 1))
    {
        *pos = 1;
        return strlen(numstr) + 1;
    }
    else
        return -1;

}

//cat /proc/driver/hawk_usb/mp_dpd_gain
static ssize_t hawk_proc_mp_dpdgain_read(struct file *file, char __user *buffer,
    size_t count, loff_t *pos)
{
    char numstr[64];
    char tmp_buf[4];
    int ret, i;
    uint8_t gain_info[MP_DPD_GAIN_INFO_LEN];

    if(*pos> 0){
        return 0;
    }
    printk("[mp_cmd]:%s\n",__func__);

    ret = wq_mp_get_dpdgain_notify(gain_info);
    if (ret) {
        printk("read dpd gain info fail\n");
        return -1;
    }
    wuqi_change_channel_dpd();
    memset(numstr, '\0', sizeof(numstr));
    memset(tmp_buf, '\0', sizeof(tmp_buf));

    for(i = 0; i < MP_DPD_GAIN_INFO_LEN; i+=4) {
        snprintf(tmp_buf, 4, "%d", *(int32_t *)(gain_info+i));
        strncat(numstr, tmp_buf, 3);
        strncat(numstr, " ", 1);
    }
    numstr[strlen(numstr)]='\n';

    if(!copy_to_user(buffer, numstr, strlen(numstr) + 1))
    {
        *pos = 1;
        return strlen(numstr) + 1;
    }
    else
        return -1;

}


static ssize_t hawk_proc_mp_cali_read(struct file *file, char __user *buffer,
    size_t count, loff_t *pos)
{
    char numstr[64];
    int ret;
    uint8_t cali[5];

    if(*pos> 0){
        return 0;
    }
    printk("[mp_cmd]:%s\n",__func__);

    ret = wq_mp_cali_efuse_notify(0, cali, sizeof(cali));
    if (ret) {
        printk("read efuse fail\n");
        return -1;
    }
    snprintf(numstr, sizeof(numstr), "cali efuse: 0x%02x%02x%02x%02x%02x\n", cali[4], cali[3], cali[2], cali[1], cali[0]);
    if(!copy_to_user(buffer, numstr, strlen(numstr) + 1))
    {
        *pos = 1;
        return strlen(numstr) + 1;
    }
    else
        return -1;

}

static ssize_t hawk_proc_version_read(struct file *file, char __user *buffer,
    size_t count, loff_t *pos)
{
    char numstr[64];

    if(*pos> 0) {
        return 0;
    }
    printk("[version_cmd]: %s\n", __func__);

    if(BUILD_VERSION == 0) {
        snprintf(numstr, sizeof(numstr), "[HAWK] no build version info\n");
    }
    else {
        snprintf(numstr, sizeof(numstr), "[HAWK] build version: 7.%d.0.%d\n", BRANCH_VERSION, BUILD_VERSION);
    }
    if(!copy_to_user(buffer, numstr, strlen(numstr) + 1))
    {
        *pos = 1;
        return strlen(numstr) + 1;
    }
    else
        return -1;
}

static ssize_t hawk_proc_remain_cali_times_read(struct file *file, char __user *buffer,
    size_t count, loff_t *pos)
{
    char numstr[64];
    int ret;
    uint8_t remain_times;

    if(*pos> 0) {
        return 0;
    }
    printk("[mp_cmd]:%s\n",__func__);

    ret = wq_mp_cali_remain_times_notify(&remain_times);
    if (ret) {
        printk("read remain_calibration_times fail\n");
        return -1;
    }

    snprintf(numstr, sizeof(numstr), "remain calibration times:%d\n", remain_times);

    if(!copy_to_user(buffer, numstr, strlen(numstr) + 1))
    {
        *pos = 1;
        return strlen(numstr) + 1;
    }
    else
        return -1;
}

static ssize_t hawk_proc_efuse_reserve_read(struct file *file, char __user *buffer,
    size_t count, loff_t *pos)
{
    char numstr[85];
    char tmp_buf[4];
    int ret, i, x;
    int offset_len[2][2]={{111,16},{127,1}};
    struct wuqi_mptest_efuse_ctx *efuse_ctx = (struct wuqi_mptest_efuse_ctx *)&mp_ctxt.efuse_ctx;

    if(*pos> 0) {
        return 0;
    }
    printk("[mp_cmd]:%s\n",__func__);

    memset(numstr, '\0', sizeof(numstr));
    memset(tmp_buf, '\0', sizeof(tmp_buf));
    strncat(numstr, "efuse bytes 111--127:", strlen("efuse bytes 111--127:"));

    for (x = 0; x < sizeof(offset_len)/sizeof(offset_len[0]); x++){
        memset(efuse_ctx->rsp_efuse_info, '\0', sizeof(efuse_ctx->rsp_efuse_info));
        memset(efuse_ctx->efuse_info, '\0', sizeof(efuse_ctx->efuse_info));
        efuse_ctx->efuse_info[0] = offset_len[x][0]; //offset
        efuse_ctx->info_len = offset_len[x][1]; //length

        ret = wq_mp_efuse_notify(0, efuse_ctx);
        if(ret) {
            printk("get efuse fail\n");
            return -1;
        } else {
            for(i = 0; i < efuse_ctx->info_len; i++) {
                printk("get efuse %02x from %d\n", efuse_ctx->rsp_efuse_info[i], efuse_ctx->efuse_info[0] + i);
            }
        }

        for (i = 0; i < efuse_ctx->info_len; i++) {
            snprintf(tmp_buf, 4, "%02x ", efuse_ctx->rsp_efuse_info[i]);
            strncat(numstr, tmp_buf, 3);
            //iot_printf("numstr:|%s|\n", numstr);
        }

    }

    numstr[strlen(numstr)]='\n';
#if (WQ_CONFIG_64 == 1)
    iot_printf("numstr:|%s|, len=%ld", numstr,strlen(numstr));
#else
    iot_printf("numstr:|%s|, len=%d", numstr,strlen(numstr));
#endif

    if(!copy_to_user(buffer, numstr, strlen(numstr) + 1))
    {
        *pos = 1;
        return strlen(numstr) + 1;
    }
    else
        return -1;
}


static ssize_t hawk_proc_mp_read(struct file *file, char __user *buffer,
    size_t count, loff_t *pos)
{
    char numstr[64];
    uint32_t crc_err_temp;
    int ret;

    if(*pos> 0){
        return 0;
    }
    printk("[mp_cmd]:%s\n",__func__);

    if (rx_start == 1)
    {
        ret = wq_mp_crc_err_notify(&crc_err_temp);
        if (ret) {
            printk("get crc err from fw failed!\n");
            crc_err_temp = 0;
        }
        mp_ctxt.rx_crc_err_cnt = crc_err_temp - mp_ctxt.rx_ce_start_cnt;
    } else {
        mp_ctxt.rx_crc_err_cnt = mp_ctxt.rx_ce_stop_cnt - mp_ctxt.rx_ce_start_cnt;
    }

    if ((int)mp_ctxt.rx_crc_err_cnt <0){
        mp_ctxt.rx_crc_err_cnt = 0;
    }

    snprintf(numstr, sizeof(numstr), "rx: success=%u, crc error=%u\n", rxcount, mp_ctxt.rx_crc_err_cnt);
    if(!copy_to_user(buffer, numstr, strlen(numstr) + 1))
    {
        *pos = 1;
        return strlen(numstr) + 1;
    }
    else
        return -1;

}

static ssize_t hawk_proc_mp_freq_dev_read(struct file *file, char __user *buffer,
    size_t count, loff_t *pos)
{
    char numstr[64];
    uint16_t freq_dev;
    int ret;

    if(*pos> 0){
        return 0;
    }
    printk("[mp_cmd]:%s\n",__func__);
    ret = wq_mp_freq_dev_notify(0, &freq_dev);
    if (ret) {
        printk("get freq deviation from fw failed!\n");
        freq_dev = 0;
    }

    snprintf(numstr, sizeof(numstr), "freq_dev=%d\n", freq_dev);
    if(!copy_to_user(buffer, numstr, strlen(numstr) + 1))
    {
        *pos = 1;
        return strlen(numstr) + 1;
    }
    else
        return -1;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
const struct proc_ops hawk_proc_fops = {
     .proc_write   = hawk_proc_write,
     .proc_lseek   = noop_llseek,
};

const struct proc_ops hawk_proc_fwdbg_fops = {
    .proc_write    = hawk_proc_fwdbg_write,
    .proc_lseek    = noop_llseek,
};

const struct proc_ops hawk_proc_mp = {
    .proc_read     = hawk_proc_mp_read,
    .proc_write    = hawk_proc_mp_write,
    .proc_lseek    = noop_llseek,
};
const struct proc_ops hawk_proc_mp_freq_dev = {
    .proc_read     = hawk_proc_mp_freq_dev_read,
    .proc_lseek    = noop_llseek,
};

const struct proc_ops hawk_proc_mp_cali = {
    .proc_read     = hawk_proc_mp_cali_read,
    .proc_lseek    = noop_llseek,
};

const struct proc_ops hawk_proc_mp_socinfo = {
    .proc_read     = hawk_proc_mp_socinfo_read,
    .proc_lseek    = noop_llseek,
};

const struct proc_ops hawk_proc_mp_dpdgain = {
    .proc_read     = hawk_proc_mp_dpdgain_read,
    .proc_lseek    = noop_llseek,
};

const struct proc_ops hawk_proc_version_fops = {
    .proc_read     = hawk_proc_version_read,
    .proc_lseek    = noop_llseek,
};

const struct proc_ops hawk_proc_remain_cali_times = {
    .proc_read     = hawk_proc_remain_cali_times_read,
    .proc_lseek    = noop_llseek,
};

const struct proc_ops hawk_proc_efuse_reserve = {
    .proc_read     = hawk_proc_efuse_reserve_read,
    .proc_lseek    = noop_llseek,
};
#else
static const struct file_operations hawk_proc_fops = {
	.owner		= THIS_MODULE,
	.write		= hawk_proc_write,
	.llseek		= noop_llseek,
};

#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
static const struct file_operations hawk_proc_usbrx_cpu_fops = {
    .owner      = THIS_MODULE,
    .write      = hawk_proc_usbrx_cpu_write,
    .llseek     = noop_llseek,
};
#endif

static const struct file_operations hawk_proc_fwdbg_fops = {
	.owner		= THIS_MODULE,
	.write		= hawk_proc_fwdbg_write,
	.llseek		= noop_llseek,
};

static const struct file_operations hawk_proc_mp = {
    .owner      = THIS_MODULE,
    .read       = hawk_proc_mp_read,
    .write      = hawk_proc_mp_write,
    .llseek     = noop_llseek,
};

static const struct file_operations hawk_proc_mp_freq_dev = {
    .owner      = THIS_MODULE,
    .read       = hawk_proc_mp_freq_dev_read,
    .llseek     = noop_llseek,
};

static const struct file_operations hawk_proc_mp_cali = {
    .owner      = THIS_MODULE,
    .read       = hawk_proc_mp_cali_read,
    .llseek     = noop_llseek,
};

static const struct file_operations hawk_proc_mp_socinfo = {
    .owner      = THIS_MODULE,
    .read       = hawk_proc_mp_socinfo_read,
    .llseek     = noop_llseek,
};

static const struct file_operations hawk_proc_mp_dpdgain = {
    .owner      = THIS_MODULE,
    .read       = hawk_proc_mp_dpdgain_read,
    .llseek     = noop_llseek,
};

static const struct file_operations hawk_proc_version_fops = {
    .owner      = THIS_MODULE,
    .read       = hawk_proc_version_read,
    .llseek     = noop_llseek,
};

static const struct file_operations hawk_proc_remain_cali_times = {
    .owner      = THIS_MODULE,
    .read       = hawk_proc_remain_cali_times_read,
    .llseek     = noop_llseek,
};

static const struct file_operations hawk_proc_efuse_reserve = {
    .owner      = THIS_MODULE,
    .read       = hawk_proc_efuse_reserve_read,
    .llseek     = noop_llseek,
};
#endif

#ifndef WQ_BUS_TEST
static ssize_t hawk_proc_regdbg_write(struct file *file, const char __user *buffer,
                  size_t count, loff_t *pos)
{
    char tmp_buf[24] = {0};
    unsigned int regaddr = 0, regval = 0;
    uint32_t tmp, space_idx = 10;
    int ret;

    if (!count)
        return 0;
    if (count != 11 && count != 22) {
        hawk_proc_show_usage(WQ_PROC_REGDBG);
        return -EINVAL;
    }
    if (copy_from_user(tmp_buf, buffer, count))
        return -EFAULT;

    if (count == 11) {
        if ((ret = kstrtouint(tmp_buf, 0, &regaddr)) == 0)
        {
            tmp  = WQ_MAC_READ((unsigned int)regaddr);
            printk("Read 0x%x, val=0x%x\n", (unsigned int)regaddr, tmp);
        }
    } else {
        tmp_buf[space_idx] = '\0';
        if (((ret = kstrtouint(tmp_buf, 0, &regaddr)) == 0) &&
            ((ret = kstrtouint(tmp_buf+space_idx+1, 0, &regval))==0))
        {
            WQ_MAC_WRITE(regaddr, regval);
            printk("Write 0x%x with 0x%x\n", (unsigned int)regaddr, (unsigned int)regval);
        }
    }

    return count;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
const struct proc_ops hawk_proc_regdbg_fops = {
    .proc_write     = hawk_proc_regdbg_write,
    .proc_lseek     = noop_llseek,
};
#else
static const struct file_operations hawk_proc_regdbg_fops = {
	.owner		= THIS_MODULE,
	.write		= hawk_proc_regdbg_write,
	.llseek		= noop_llseek,
};
#endif
#endif


int wq_wifi_attach(void *wq_ipc)
{
#if 0
    //extern void osal_unit_test(void);
    //    osal_unit_test();
    //extern void timer_unit_test(void);
    //	timer_unit_test();
#else
    printk(KERN_ALERT "wq_dbg_flag = %d\n", wq_dbg_flag);
    if(BUILD_VERSION == 0) {
        printk("[HAWK] no build version info\n");
    }
    else {
        printk("[HAWK] build version: 7.%d.0.%d\n", BRANCH_VERSION, BUILD_VERSION);
    }
    wq_wifi_dbg_set(wq_dbg_flag);
    oss_start();
    wproto_start();
    wdrv_start(wq_ipc);
    wq_drv_rmmod_flag = false;
#endif

#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
    wq_wifi_detach_flag = false;
    wq_wifi_detach_end_flag = false;
#endif

    g_vif_ctx = wuqi_create_context(wq_ipc);
    if (g_vif_ctx != NULL) {
        /*DEMO*/
        sema_init(&g_vif_ctx->sem, 1);
        INIT_WORK(&g_vif_ctx->ws_connect, wuqi_connect_routine);
        g_vif_ctx->connecting_ssid[0] = 0;
        INIT_WORK(&g_vif_ctx->ws_disconnect, wuqi_disconnect_routine);
        g_vif_ctx->disconnect_reason_code = 0;
        INIT_WORK(&g_vif_ctx->ws_scan, wuqi_scan_routine);
        INIT_DELAYED_WORK(&g_vif_ctx->ws_scan_delayed, wuqi_scan_delay_routine);
        g_vif_ctx->scan_request = NULL;
        INIT_WORK(&g_vif_ctx->ws_set_bitrate, set_bitrate_routine);
        INIT_WORK(&g_vif_ctx->ws_set_txq_params, set_bitrate_routine);

    }

#if 1
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,12,0)
    rtnl_lock();
    wuqi_cfg80211_add_iface(g_vif_ctx->wiphy,
        STA_VIF_NAME,
        0,
        NL80211_IFTYPE_STATION,
        NULL);

    wuqi_cfg80211_add_iface(g_vif_ctx->wiphy,
        SOFTAP_VIF_NAME,
        0,
        NL80211_IFTYPE_AP,
        NULL);
    rtnl_unlock();
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0) && (LINUX_VERSION_CODE < KERNEL_VERSION(4,12,0)))
    rtnl_lock();
    wuqi_cfg80211_add_iface(g_vif_ctx->wiphy,
        STA_VIF_NAME,
        0,
        NL80211_IFTYPE_STATION,
        NULL,
        NULL);

	wuqi_cfg80211_add_iface(g_vif_ctx->wiphy,
        SOFTAP_VIF_NAME,
        0,
        NL80211_IFTYPE_AP,
        NULL,
        NULL);
	rtnl_unlock();
#else
    rtnl_lock();
    wuqi_cfg80211_add_iface(g_vif_ctx->wiphy,
        STA_VIF_NAME,
        NL80211_IFTYPE_STATION,
        NULL,
        NULL);

	wuqi_cfg80211_add_iface(g_vif_ctx->wiphy,
        SOFTAP_VIF_NAME,
        NL80211_IFTYPE_AP,
        NULL,
        NULL);
	rtnl_unlock();
#endif
#endif

    return g_vif_ctx == NULL;
}

int hawk_module_exist()
{
    return (g_vif_ctx != NULL);
}

void wq_wifi_detached_stop_wdrv(void)
{
    if(g_vif_ctx != NULL) {
        printk("%s:%d error!! g_vif_ctx is not NULL\n", __func__, __LINE__);
        return;
    }

    wdrv_stop();
    oss_stop();
    mmal_info();
    dump_timer_record();
}

void wq_wifi_detach(void)
{
    if(g_vif_ctx == NULL) {
        return;
    }

#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
    if (wq_wifi_detach_flag){
        return;
    }
    else{
        wq_wifi_detach_flag = true;
    }
#endif

    regulatory_hint(g_vif_ctx->wiphy, "00");

    rtnl_lock();
    if(g_vif_ctx0 && g_vif_ctx0->ndev)
    {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)) 
        struct wireless_dev *wireless_dev = g_vif_ctx0->ndev->ieee80211_ptr;
#else
        struct net_device *wireless_dev = g_vif_ctx0->ndev;
#endif
        wuqi_cfg80211_del_iface(g_vif_ctx0->wiphy, wireless_dev);
    }
    rtnl_unlock();

    rtnl_lock();
    if(g_vif_ctx1 && g_vif_ctx1->ndev)
    {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)) 
        struct wireless_dev *wireless_dev = g_vif_ctx1->ndev->ieee80211_ptr;
#else
        struct net_device *wireless_dev = g_vif_ctx1->ndev;
#endif
        wuqi_cfg80211_del_iface(g_vif_ctx1->wiphy, wireless_dev);
    }
    rtnl_unlock();

    /* make sure that no work is queued */
    cancel_work_sync(&g_vif_ctx->ws_connect);
    cancel_work_sync(&g_vif_ctx->ws_disconnect);
    cancel_work_sync(&g_vif_ctx->ws_scan);
	cancel_delayed_work_sync(&g_vif_ctx->ws_scan_delayed);
    cancel_work_sync(&g_vif_ctx->ws_set_bitrate);
    cancel_work_sync(&g_vif_ctx->ws_set_txq_params);



    if(g_wlan1_mon_net && g_wlan1_mon_ndev && dev_get_by_name(g_wlan1_mon_net, MON_VIF_NAME)){
        printk("%s:%d found mon.wlan1, free it \n",__func__,__LINE__);
        dev_put(g_wlan1_mon_ndev);
        wuqi_free_dummy_ndev(g_wlan1_mon_ndev);
    }

    wuqi_ctx_free(g_vif_ctx);
    g_vif_ctx = NULL;
#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
    wq_wifi_detach_end_flag = true;
#endif
}

char *califile_read_line(struct file *fp, char *buf, int buf_len)
{
    int ret;
    int i = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,12,0)
    ret = kernel_read(fp, buf, buf_len, &(fp->f_pos));
#else
    mm_segment_t fs;

    fs=get_fs();
    set_fs(KERNEL_DS);
    ret = vfs_read(fp, buf, buf_len, &(fp->f_pos));
    set_fs(fs);
#endif
    if (ret <= 0)
        return NULL;

    while(buf[i++] != '\n' && i < ret);

    if(i < ret) {
        fp->f_pos += i-ret;
    }

    if(i < buf_len) {
        buf[i] = 0;
    }
    return buf;
}

int delete_space(char *src) {
    char *pTmp = src;

    while (*src != '\0') {
        if (*src != ' ') {
            *pTmp++ = *src;
        }
        src++;
    }

    *pTmp = '\0';

    return 0;
}

/* mptest cali-free mode */
int wq_mp_cali_free() {
    int ret;
    uint8_t cali[5];
    char buf_line[50], buf_pn[50], buf_ver[50], buf_con[50];
    struct file* fp_cali_efuse;
    char *pos;

#ifndef WQ_LOAD_CALI_FROM_FILE
    ret = wq_mp_cali_efuse_notify(0, cali, sizeof(cali));
    if (ret) {
        iot_printf("read efuse cali fail\n");
    }else{
        iot_printf("cali efuse: 0x%02x%02x%02x%02x%02x\n", cali[4], cali[3], cali[2], cali[1], cali[0]);

        if (cali[0]==0 && cali[1]==0 && cali[2]==0 && cali[3]==0 && cali[4]==0) {
#endif
            //read efuse from file
            fp_cali_efuse = filp_open("/etc/WQ9001_WiFi_Mapfile.dat", O_RDONLY, 00700);
            if (IS_ERR(fp_cali_efuse)){
                iot_printf("open WQ9001_WiFi_Mapfile.dat error\n");
            } else {
                //iot_printf("open WQ9001_WiFi_Mapfile.dat success\n");

                while (califile_read_line(fp_cali_efuse, buf_line, 50) != NULL) {
                    //iot_printf("%s|\n", buf_line);
                    if (!memcmp(buf_line, "$PN", strlen("$PN"))) {
                        sscanf(buf_line + strlen("$PN: "), "%s", buf_pn);
                        pos = skip_spaces(buf_pn);
                        if (sysfs_streq(buf_pn, "WQ9001")) {
                            iot_printf("$PN: WQ9001\n");
                        }
                    }
                    if (!memcmp(buf_line, "$VER", strlen("$VER"))) {
                        sscanf(buf_line + strlen("$VER: "), "%s", buf_ver);
                        pos = skip_spaces(buf_ver);
                        if (sysfs_streq(buf_ver, "2.0")) {
                            iot_printf("$VER: 2.0\n");
                        }
                    }
                    if (!memcmp(buf_line, "$Content: ", strlen("$Content: "))) {
                        if(strrchr(buf_line, '\n')) {
                            strlcpy(buf_con, &buf_line[strlen("$Content: ")], strlen(buf_line)-strlen("$Content: "));
                        }else{
                            strlcpy(buf_con, &buf_line[strlen("$Content: ")], strlen(buf_line)-strlen("$Content: ")+1);
                        }
                        buf_con[strlen(buf_con)] = '\0';
                       // delete_space(buf_con);
#if (WQ_CONFIG_64 == 1)
                        iot_printf("%s|,len=%ld\n", buf_con,strlen(buf_con));
#else
                        iot_printf("%s|,len=%d\n", buf_con,strlen(buf_con));
#endif
                    }
                }

                filp_close(fp_cali_efuse, NULL);

                if (sysfs_streq(buf_pn, "WQ9001") && sysfs_streq(buf_ver, "2.0") && strlen(buf_con)==14) {
                    wq_mp_kfree_cali_notify(buf_ver, buf_con);
                }
                else {
                    iot_printf("not WQ9001 or ver1.0 or invalid cali len\n");
                }
            }
#ifndef WQ_LOAD_CALI_FROM_FILE
        }

    }
#endif
    return 0;
}

#ifdef WQ_SIM_HOST_ONLY
static int __init wifi_init(void)
{
    return wq_wifi_attach(wq_ipc_attach(NULL));
}

static void __exit wifi_exit(void)
{
    wq_wifi_detach();
}

module_init(wifi_init);
module_exit(wifi_exit);
#else
static int __init wq_module_init(void)
{

    proc_mkdir("driver/hawk_usb", NULL);

    proc_create_data("driver/hawk_usb/dbg", 0666, NULL, &hawk_proc_fops, NULL);
#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
    proc_create_data("driver/hawk_usb/usb_rx_cpu", 0666, NULL, &hawk_proc_usbrx_cpu_fops, NULL);
#endif
    proc_create_data("driver/hawk_usb/fwdbg", 0666, NULL, &hawk_proc_fwdbg_fops, NULL);
    proc_create_data("driver/hawk_usb/mp_test", 0666, NULL, &hawk_proc_mp, NULL);
    proc_create_data("driver/hawk_usb/mp_test_freq_dev", 0666, NULL, &hawk_proc_mp_freq_dev, NULL);
    proc_create_data("driver/hawk_usb/mp_test_cali", 0666, NULL, &hawk_proc_mp_cali, NULL);
    proc_create_data("driver/hawk_usb/mp_soc_info", 0666, NULL, &hawk_proc_mp_socinfo, NULL);
    proc_create_data("driver/hawk_usb/mp_dpd_gain", 0666, NULL, &hawk_proc_mp_dpdgain, NULL);
    proc_create_data("driver/hawk_usb/version", 0666, NULL, &hawk_proc_version_fops, NULL);
    proc_create_data("driver/hawk_usb/remain_cali_times", 0666, NULL, &hawk_proc_remain_cali_times, NULL);
    proc_create_data("driver/hawk_usb/efuse_reserve", 0666, NULL, &hawk_proc_efuse_reserve, NULL);

#ifndef WQ_BUS_TEST
    proc_create_data("driver/hawk_usb/regdbg", 0666, NULL, &hawk_proc_regdbg_fops, NULL);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,12,0)
    log2file.old_fs = get_fs();
#endif
    /* register the bus */
    if (wq_usb_register() < 0) {
        iot_printf("usb register failed!\n");
        return -ENODEV;
    }

    return 0;
}

static void __exit wq_module_exit(void)
{
    int ret;

    if (log2file_task){
        ret = kthread_stop(log2file_task);
        log2file_task = NULL;
        thread_end_flag = 1;
        printk("thread function has run %d\n", ret);
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,12,0)
    kfree(cache_log_buf1);
    cache_log_buf1 = NULL;
    kfree(cache_log_buf2);
    cache_log_buf2 = NULL;
#endif
    wq_dbg_flag = WQ_DEBUG_ALL;
    wq_usb_exit();

    remove_proc_entry("driver/hawk_usb/dbg", NULL);
#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
    remove_proc_entry("driver/hawk_usb/usb_rx_cpu", NULL);
#endif
    remove_proc_entry("driver/hawk_usb/fwdbg", NULL);
    remove_proc_entry("driver/hawk_usb/mp_test", NULL);
    remove_proc_entry("driver/hawk_usb/mp_test_freq_dev", NULL);
    remove_proc_entry("driver/hawk_usb/mp_test_cali", NULL);
    remove_proc_entry("driver/hawk_usb/mp_soc_info", NULL);
    remove_proc_entry("driver/hawk_usb/mp_dpd_gain", NULL);
    remove_proc_entry("driver/hawk_usb/version", NULL);
    remove_proc_entry("driver/hawk_usb/remain_cali_times", NULL);
    remove_proc_entry("driver/hawk_usb/efuse_reserve", NULL);

    if(log2file.fp1){
        filp_close(log2file.fp1, NULL);
    }

    if(log2file.fp2){
        filp_close(log2file.fp2, NULL);
    }
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,12,0)
    set_fs(log2file.old_fs);
#endif

#ifndef WQ_BUS_TEST
    remove_proc_entry("driver/hawk_usb/regdbg", NULL);
#endif

    remove_proc_entry("driver/hawk_usb", NULL);

}

module_init(wq_module_init);
module_exit(wq_module_exit);
#endif

MODULE_LICENSE("GPL v2");

MODULE_DESCRIPTION("Hawk 802.11bgn WLAN driver.");

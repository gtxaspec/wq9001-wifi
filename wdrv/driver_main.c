#include "oss.h"
#include "if_hawk_port.h"
#include <net80211/ieee80211_var.h>
#include <linux/nl80211.h>

#include "if_hawkvar.h"
#include "wq_ipc.h"
#include "wuqi_cfg80211_port.h"
#include "wq_htc_desc.h"
#include "net80211/ieee80211_extap.h"

#define	DEVICE_HAWK            0x0001

struct wq_softc *wdrv_sc = NULL;
extern struct ieee80211vap *hawk_vap0; //Support up to 2 VAP only
extern struct ieee80211vap *hawk_vap1; //Support up to 2 VAP only

extern struct ieee80211vap *wlan0_vap;
extern struct ieee80211vap *wlan1_vap;
extern struct ieee80211com *net80211_ic;
extern struct ieee80211vap *wq_vap_create(struct ieee80211com *,
        const char [IFNAMSIZ], int, enum ieee80211_opmode, int,
        const uint8_t [IEEE80211_ADDR_LEN],
        const uint8_t [IEEE80211_ADDR_LEN]);
extern void hawk_clone_destroy(struct ifnet *ifp);
extern int wq_hal_setmac(struct wq_hal_vap *, const uint8_t addr[6]);
extern int
ieee80211_ioctl_setkey(struct ieee80211vap *vap, struct ieee80211req *ireq);
extern void
ieee80211_crypto_set_deftxkey(struct ieee80211vap *vap, ieee80211_keyix kid);
extern int ieee80211_ioctl_delkey(struct ieee80211vap *vap, struct ieee80211req *ireq);
extern void wq_mac_tx_done(void *wq_mac, void *buf, int status);
extern void wq_mac_rx_pkt(void *wq_mac, struct sk_buff *skb, u32 buf_len);
extern void wq_mac_event_handle(void *wq_mac, u32 buf_len, struct sk_buff *skb);
extern int ieee80211_ioctl_setappie(struct ieee80211vap *vap,
        const struct ieee80211req *ireq);
extern int
ieee80211_ioctl_setcurchan(struct ieee80211vap *vap,
        const struct ieee80211req *ireq);
extern void ieee80211_syncflag_ht(struct ieee80211vap *vap, int flag);
extern int ieee80211_send_probereq(struct ieee80211_node *ni,
	const uint8_t sa[IEEE80211_ADDR_LEN],
	const uint8_t da[IEEE80211_ADDR_LEN],
	const uint8_t bssid[IEEE80211_ADDR_LEN],
	const uint8_t *ssid, size_t ssidlen);
extern void wq_notify_hawk_stop(struct wq_softc *sc);
extern void wq_newstate_setup(struct ieee80211vap *vap);
extern void start_rx_promiscuous(void);
extern void stop_rx_promiscuous(void);
void htc_fw_info(struct wq_softc *sc, struct fw_info_cfm *cfm, uint32_t fw_dbg);
extern void htc_dev_info_notify(struct wq_softc *sc, uint8_t type, uint8_t *mac_addr, uint8_t fixed_rate);
extern void htc_status_notify(struct wq_softc *sc, uint8_t msg_type);
extern u32 *wq_mem_alloc(int len, int zero);
extern int htc_ch_bw_notify(struct wq_softc *sc, uint8_t ch_num, uint16_t freq, int8_t power, uint8_t sw_ch_cali);
extern void htc_ampdu_agg_set_notify(struct wq_softc *sc, uint8_t agg_num);
extern uint8_t htc_get_ampdu_aggr_num(struct wq_softc *sc);

void vap_create_test(void)
{
    struct ieee80211com *ic;
    char name[IFNAMSIZ] = {'h', 'a', 'w', 'k'};
    const uint8_t bssid[IEEE80211_ADDR_LEN] = {00, 01, 02, 03, 04, 05};
    uint8_t mac0[IEEE80211_ADDR_LEN] = {0x0, 'h', 'a', 'w', 'k', 'A'};
    enum ieee80211_opmode opmode;

    //opmode = IEEE80211_M_HOSTAP;
    opmode = IEEE80211_M_STA;

    iot_printf("driver_vap_create %s\n", (opmode == IEEE80211_M_STA)?"STA":"AP");
    if (opmode == IEEE80211_M_STA)
    {
        mac0[IEEE80211_ADDR_LEN-1] = 'S';
    }

    ic = &wdrv_sc->sc_ic;


    //iot_printf("driver_vap_create %p %p\n", wlan0_vap, wlan1_vap);

    if ((ic->ic_vap_create) != NULL)
    {

        wlan0_vap = ic->ic_vap_create(ic, name, 0,
                opmode,
                0,
                bssid,
                mac0);

        //  wlan1_vap = ic->ic_vap_create(ic, name, 1,
        //    opmode,
        //    0,
        //    bssid,
        //    mac0);

        IEEE80211_LOCK(ic);
        ieee80211_start_locked(wlan0_vap);
        IEEE80211_UNLOCK(ic);


    }
    else
    {
        iot_printf("driver_vap_create fail : ic->ic_vap_create = NULL\n");
    }


}

void wuqi_vap_setmac(void *param, u8 myaddr[IEEE80211_ADDR_LEN])
{
    struct ieee80211vap *vap=param;
    struct wq_vap *mvp = WQ_VAP(vap);
    struct wq_hal_vap *hvap = mvp->mv_hvap;
    memcpy(vap->iv_myaddr, myaddr, IEEE80211_ADDR_LEN);
    memcpy(vap->iv_ifp->if_addr.ifa_addr, myaddr, IEEE80211_ADDR_LEN);
    wq_hal_setmac(hvap, vap->iv_myaddr);
}

void wuqi_vap_change_mode(void *param, u8 nmode)
{
    struct ieee80211vap *vap=param;
    struct ieee80211com *ic = vap->iv_ic;
    struct wq_softc *sc = (struct wq_softc *) ic->ic_softc;
    enum ieee80211_opmode opmode= vap->iv_opmode;

    if(opmode == nmode)
        return;

    if(opmode == IEEE80211_M_MONITOR) {
        stop_rx_promiscuous();
        htc_status_notify(sc, WQ_SCAN_END); //for enable phy dpd timer
    }

    iot_printf("[%s:%d] iv_opmode : %d -> %d\n",__func__,__LINE__, opmode, nmode);
    vap->iv_opmode = nmode;
    ieee80211_proto_change_vattach(vap);
    wq_newstate_setup(vap);

    if(nmode == IEEE80211_M_MONITOR) {
        start_rx_promiscuous();
        htc_status_notify(sc, WQ_SCAN_START); //for disable phy dpd timer
    }

}

void wuqi_vap_delete(void *param)
{
    struct ieee80211vap *vap=param;
    struct ifnet *ifp = vap->iv_ifp;

    iot_printf("[%s:%d] ifp:0x%p\n",__func__,__LINE__,ifp);
    hawk_clone_destroy(ifp);
}

void *wuqi_vap_create(char *name, enum nl80211_iftype type, void *vif_ctx, uint8_t mac[6])
{
    enum ieee80211_opmode opmode;
    int unit=0;
    uint8_t bssid[6]={0}; //TODO
    //uint8_t mac[6]={0, 'h', 'a', 'w', 'k', 'x'};  //TODO
    struct ieee80211vap **vap = NULL;

    switch(type){
        case NL80211_IFTYPE_AP:
            opmode=IEEE80211_M_HOSTAP;
            unit=0;
            vap=(struct ieee80211vap **)&hawk_vap0;
            break;
        case NL80211_IFTYPE_STATION:
            opmode=IEEE80211_M_STA;
            vap=(struct ieee80211vap **)&hawk_vap1;
            unit=1;
            break;
        case NL80211_IFTYPE_MONITOR:
            opmode=IEEE80211_M_MONITOR;
            vap=(struct ieee80211vap **)&hawk_vap1;
            unit=0;
            break;
        default:
            iot_printf("%s: mode %d not support\n",__func__,type);
            return NULL;
    }

    *vap=wq_vap_create(&wdrv_sc->sc_ic,name,unit,opmode,0,bssid,mac); //Flag:TODO

    (*vap)->vif_ctx = vif_ctx;
    (*vap)->iv_scandone_flags = 1;

    if ((type == NL80211_IFTYPE_STATION)|| (type == NL80211_IFTYPE_MONITOR))
	{
		IEEE80211_LOCK((*vap)->iv_ic);
		ieee80211_start_locked(*vap);
		IEEE80211_UNLOCK((*vap)->iv_ic);
	}

    return *vap;
}


extern int ieee80211_scanreq(struct ieee80211vap *, struct ieee80211_scan_req *);

u16 channels_conf[WUQI_MAX_CHANNELS] = {0}; //channel list to be scanned by wproto 
u16 n_channels = 0; //number channel list to be scanned by wproto

void wuqi_scan_req(void *param, char ssid[], uint8_t len, u16 scanreq_channels_conf[WUQI_MAX_CHANNELS], u16 scanreq_n_channels)
{
    struct ieee80211vap *vap=param;
    struct ieee80211_scan_req sr;
    struct ifnet *ifp = vap->iv_ifp;
    int i;
    //u_int8_t mac[6], tmp[3] = {0};
    uint32_t once;

    //scan req
    sr.sr_duration = 5000;
    sr.sr_mindwell = 40;
    sr.sr_maxdwell = 110;
    sr.sr_nssid = 1;
    sr.sr_ssid[0].len = len;
    strncpy(sr.sr_ssid[0].ssid, ssid, (len > IEEE80211_NWID_LEN ? IEEE80211_NWID_LEN:len));
    sr.sr_flags = IEEE80211_IOC_SCAN_ACTIVE;
    once = 1;
    if(once == 0){
        iot_printf("[%s] active + continuous scan\n", __func__);
    }else if(once == 1){
        sr.sr_flags |= IEEE80211_IOC_SCAN_ONCE;
        iot_printf("[%s] active + scan once\n", __func__);
    }else if(once == 2){
        sr.sr_flags = 0;
        sr.sr_flags |= IEEE80211_IOC_SCAN_ONCE;
        iot_printf("[%s] passive + scan once\n", __func__);
    }else{
        iot_printf("[%s] Do nothing!\n", __func__);
        return;
    }
#if 0
    if(macStr && iot_strlen(macStr) == 17){
        for(i = 0; i < 6; i++){
            os_mem_cpy(tmp, macStr+(i*3), 2);
            mac[i] = iot_strtol((char *)tmp, NULL, 16);
        }
        ifp->if_directedaddr = mac;
    }
#endif
    ifp->if_directedaddr = NULL;
	

    n_channels = scanreq_n_channels;
	memset(channels_conf, 0, sizeof(u16)*WUQI_MAX_CHANNELS);
	
	for (i=0; i<n_channels; i++) {
        channels_conf[i] = scanreq_channels_conf[i];
	}
	
	//iot_printf("wuqi_scan_req %d\n", n_channels);
	
    ieee80211_scanreq(vap, &sr);


}

int wuqi_send_probereq(void *vap_params, u8 *addr, u8 *ssid, size_t ssidlen)
{
    struct ieee80211_node *ni;
    struct ieee80211vap *vap = (struct ieee80211vap *)vap_params;
    ni = vap->iv_bss;

    ieee80211_send_probereq(ni,
        vap->iv_myaddr,
        addr,
        addr,
        ssid, ssidlen);
    return 0;
}

void scan_req_test(void)
{
    struct ieee80211_scan_req sr;
    struct ifnet *ifp = wlan0_vap->iv_ifp;
    //int i;
    //u_int8_t mac[6], tmp[3] = {0};
    uint32_t once;

    //scan req
    sr.sr_duration = 5000;
    sr.sr_mindwell = 40;
    sr.sr_maxdwell = 100;
    sr.sr_nssid = 1;
    sr.sr_ssid[0].len = 4;
    sr.sr_ssid[0].ssid[0] = 't';
    sr.sr_ssid[0].ssid[1] = 'e';
    sr.sr_ssid[0].ssid[2] = 's';
    sr.sr_ssid[0].ssid[3] = 't';

    sr.sr_flags = IEEE80211_IOC_SCAN_ACTIVE;
    once = 1;
    if(once == 0){
        iot_printf("[%s] active + continuous scan\n", __func__);
    }else if(once == 1){
        sr.sr_flags |= IEEE80211_IOC_SCAN_ONCE;
        iot_printf("[%s] active + scan once\n", __func__);
    }else if(once == 2){
        sr.sr_flags = 0;
        sr.sr_flags |= IEEE80211_IOC_SCAN_ONCE;
        iot_printf("[%s] passive + scan once\n", __func__);
    }else{
        iot_printf("[%s] Do nothing!\n", __func__);
        return;
    }
#if 0
    if(macStr && iot_strlen(macStr) == 17){
        for(i = 0; i < 6; i++){
            os_mem_cpy(tmp, macStr+(i*3), 2);
            mac[i] = iot_strtol((char *)tmp, NULL, 16);
        }
        ifp->if_directedaddr = mac;
    }
#endif
    ifp->if_directedaddr = NULL;

    ieee80211_scanreq(wlan0_vap, &sr);


}

void wdrv_start(void *wq_ipc)
{
    struct wq_ipc_mac_interface mac_inf = {
        .wq_mac_tx_done = wq_mac_tx_done,
        .wq_mac_rx_pkt = wq_mac_rx_pkt,
        .wq_mac_evt_handler = wq_mac_event_handle,
    };
    struct fw_info_cfm cfm;

    wdrv_sc = (struct wq_softc *)wq_mem_alloc(sizeof(struct wq_softc), 0);
    //wdrv_sc = mmal_malloc(sizeof(struct wq_softc));
    //wdrv_sc = mmal_pool_alloc(MEMPOOL_TYPE_NORMAL,
    //                      0,
    //                      sizeof(struct wq_softc),
    //                      1,
    //                      0);
#if (WQ_CONFIG_64 == 1)
        iot_printf("wdrv_sc=0x%p size=%ld\n", wdrv_sc, sizeof(struct wq_softc));
#else
        iot_printf("wdrv_sc=0x%p size=%d\n", wdrv_sc, sizeof(struct wq_softc));
#endif

    memset(wdrv_sc, 0, sizeof(struct wq_softc));

    wdrv_sc->sc_wq_ipc = wq_ipc; // set it before wq_attach() for usb communication

	WQ_LOCK_INIT(wdrv_sc);

    wq_attach(DEVICE_HAWK, wdrv_sc);
#if 0 //test code
    msleep(100);

    vap_create_test();

    msleep(100);

    scan_req_test();
#endif
    wq_ipc_mac_register(wq_ipc, (void *)wdrv_sc, &mac_inf);


    htc_fw_info(wdrv_sc, &cfm, 0);
	iot_printf("ipc_ver:%d.%d, hw_ver:%d\n", cfm.ipc_ver/10, cfm.ipc_ver%10, cfm.hw_ver);
    //ipc version
	//0.1 : initial version
    //hw version
	//3 : HAWK-USB v3 FPGA

    return;
}

void wq_set_fw_dbg(uint32_t fw_dbg)
{
	struct fw_info_cfm cfm;

	if (wdrv_sc != NULL)
	    htc_fw_info(wdrv_sc, &cfm, fw_dbg);
}

void
indicate_wq_stop_hawk()
{
    wq_notify_hawk_stop(wdrv_sc);
}

void wdrv_stop(void)
{
    if (wdrv_sc == NULL) {
        return;
    }

    wq_detach(wdrv_sc);

    kfree(wdrv_sc);
    wdrv_sc = NULL;

    return;
}

int wuqi_set_key(void * vap_param,
        u8 key_idx, bool pairwise, const u8 *mac_addr,
        const u8 *key, const u8 *seq, int key_len, int seq_len,u32 cipher)
{
    struct ieee80211req_key wk;
    struct ieee80211req ireq;
    struct ieee80211vap *vap=vap_param;
    int res;

    iot_printf("%s, key_idx:%d, pairwise:%d, mac_addr %pM, key_len:%d, seq_len:%d, cipher:0x%x\n",
            __func__,key_idx,pairwise,mac_addr,key_len,seq_len,cipher);

    memset(&wk, 0, sizeof(wk));
    switch (cipher) {
        case 0x000FAC01:// WLAN_CIPHER_SUITE_WEP40:refer to include/linux/ieee80211.h
        case 0x000FAC05://WLAN_CIPHER_SUITE_WEP104:
            vap->iv_flags |= IEEE80211_F_PRIVACY;
            wk.ik_type = IEEE80211_CIPHER_WEP;
            break;
        case 0x000FAC02://WLAN_CIPHER_SUITE_TKIP:
            vap->iv_flags |= IEEE80211_F_PRIVACY;
            wk.ik_type = IEEE80211_CIPHER_TKIP;
            break;
        case 0x000FAC04://WLAN_CIPHER_SUITE_CCMP:
            vap->iv_flags |= IEEE80211_F_PRIVACY;
            wk.ik_type = IEEE80211_CIPHER_AES_CCM;
            break;
        default:
            iot_printf("%s: unknown cipher=0x%x", __func__, cipher);
            return -1;
    }

    wk.ik_flags = IEEE80211_KEY_RECV;

    if (mac_addr == NULL) {
        memset(wk.ik_macaddr, 0xff, IEEE80211_ADDR_LEN);
        wk.ik_keyix = key_idx;
        wk.ik_flags |= IEEE80211_KEY_GROUP;
    } else {
        memcpy(wk.ik_macaddr, mac_addr, IEEE80211_ADDR_LEN);
        /*
         * Deduce whether group/global or unicast key by checking
         * the address (yech).  Note also that we can only mark global
         * keys default; doing this for a unicast key is an error.
         */
        if (IEEE80211_IS_MULTICAST(mac_addr)) {
            wk.ik_flags |= IEEE80211_KEY_GROUP;
            wk.ik_keyix = key_idx;
        } else {
            wk.ik_keyix = key_idx == 0 ? IEEE80211_KEYIX_NONE :
                key_idx;
            if(vap->iv_opmode == IEEE80211_M_STA) {
                vap->iv_flags_ext |= IEEE80211_UNICAST_KEY;
            }
        }
        wk.ik_flags |= IEEE80211_KEY_XMIT;
    }

    //driver design, if CIPHER is WEP, no need to set flags.
    //will set both unicast/group key.
    if(wk.ik_type == IEEE80211_CIPHER_WEP) {
        wk.ik_flags = 0;
    }
#if 0
    if (wk.ik_keyix != IEEE80211_KEYIX_NONE && set_tx)
        wk.ik_flags |= IEEE80211_KEY_DEFAULT;

    /*
     * Ignore replay failures in IBSS and AHDEMO mode.
     */
    if (drv->opmode == IEEE80211_M_IBSS ||
            drv->opmode == IEEE80211_M_AHDEMO)
        wk.ik_flags |= IEEE80211_KEY_NOREPLAY;
#endif
    if (wk.ik_keyix != IEEE80211_KEYIX_NONE) //TODO:set_tx ?
    {
        wk.ik_flags |= IEEE80211_KEY_DEFAULT;
    }

    wk.ik_keylen = key_len;
    if (seq) {
#ifdef WORDS_BIGENDIAN
        /*
         * wk.ik_keyrsc is in host byte order (big endian), need to
         * swap it to match with the byte order used in WPA.
         */
        int i;
        u8 *keyrsc = (u8 *) &wk.ik_keyrsc;
        for (i = 0; i < seq_len; i++)
            keyrsc[WPA_KEY_RSC_LEN - i - 1] = seq[i];
#else /* WORDS_BIGENDIAN */
        memcpy(&wk.ik_keyrsc, seq, seq_len);
#endif /* WORDS_BIGENDIAN */
    }
    memcpy(wk.ik_keydata, key, key_len);

    memset(&ireq, 0, sizeof(ireq));
    ireq.i_data = (void *) &wk;
    ireq.i_len = sizeof(wk);

    /* wait if eapol is pending */
    wait_for_completion_timeout(&vap->iv_wait, hz);
    res=ieee80211_ioctl_setkey(vap,&ireq);
    vap->iv_wait.done = 1;

    return (0-res);
}

void wuqi_set_deftxkey(void * vap_param,u8 key_idx)
{
    struct ieee80211vap *vap=vap_param;

    ieee80211_crypto_set_deftxkey(vap,key_idx);
}

int wuqi_del_key(void * vap_param, u8 key_idx, bool pairwise, const u8 *mac_addr)
{
    struct ieee80211req_del_key dk;
    struct ieee80211req ireq;
    struct ieee80211vap *vap=vap_param;
    int res=0;

    if (vap == NULL) {
        iot_printf("%s vap is NULL, return!!\n", __func__);
        return -EINVAL;
    }

    if((vap->iv_opmode == IEEE80211_M_STA) && (vap->iv_flags_ext & IEEE80211_UNICAST_KEY)) {
        vap->iv_flags_ext &= ~IEEE80211_UNICAST_KEY;
    }

    iot_printf("%s, key_idx:%d, pairwise:%d, mac_addr %pM\n",
            __func__,key_idx,pairwise,mac_addr);
    memset(&dk, 0, sizeof(dk));
    if (mac_addr == NULL){
        dk.idk_keyix = key_idx;
    }else{
        memcpy(dk.idk_macaddr, mac_addr, IEEE80211_ADDR_LEN);
        dk.idk_keyix = (u8) IEEE80211_KEYIX_NONE;
    }
    memset(&ireq, 0, sizeof(ireq));
    ireq.i_data = (void *) &dk;
    ireq.i_len = sizeof(dk);
    res=ieee80211_ioctl_delkey(vap, &ireq);

    return (0-res);
}


int wuqi_config_ap_basic(void *vap_param, const u8 *ssid, int ssid_len, int hidden_ssid,
        int beacon_int, int dtim_period, const u8 *appie, int appie_len)
{
    struct ieee80211vap *vap=vap_param;
    struct ieee80211com *ic = vap->iv_ic;

    iot_printf("%s :ssid:%s, beacon int:%d, dtim period:%d\n",
            __func__,ssid,beacon_int,dtim_period);

    //set ssid
    if(ssid && ssid_len){
        os_mem_set(vap->iv_des_ssid[0].ssid, 0, IEEE80211_NWID_LEN);
        vap->iv_des_ssid[0].len = ssid_len;
        os_mem_cpy(vap->iv_des_ssid[0].ssid, ssid, ssid_len);
        vap->iv_des_nssid = (ssid_len > 0);
    }

    //set hidden_ssid
    if (hidden_ssid) {
        vap->iv_flags |= IEEE80211_F_HIDESSID;
    } else {
        vap->iv_flags &= ~IEEE80211_F_HIDESSID;
    }

    //set dtim_period
    vap->iv_dtim_period=dtim_period;

    //set beacon interval
    if (IEEE80211_BINTVAL_MIN <= beacon_int &&
            beacon_int <= IEEE80211_BINTVAL_MAX) {
        ic->ic_bintval = beacon_int;
    }

    //set appie
    if(appie && appie_len){
        struct ieee80211req ireq;
        memset(&ireq, 0, sizeof(ireq));
        ireq.i_type=IEEE80211_IOC_APPIE;
        ireq.i_val=IEEE80211_APPIE_WPA;
        ireq.i_data = (void *) appie;
        ireq.i_len = appie_len;
        return (0-ieee80211_ioctl_setappie(vap, &ireq));
    }

    //config ap wme params
    ieee80211_wme_initparams(vap);

    return 0;
}

int wuqi_config_shared_key(void *vap_param,int auth_type,int key_len, int key_idx, const u8 *key,u32 cipher)
{
    struct ieee80211vap *vap=vap_param;
    int authmode;
    const struct ieee80211_authenticator *auth;

    if(key_len == 0)
        return 0;

    authmode = IEEE80211_AUTH_SHARED;

    auth = ieee80211_authenticator_get(authmode);
    if (auth == NULL)
        return -EINVAL;

    vap->iv_flags |= IEEE80211_F_PRIVACY;
    /* NB: authenticator attach/detach happens on state change */
    vap->iv_bss->ni_authmode = authmode;
    /* XXX mixed/mode/usage? */
    vap->iv_auth = auth;

    wuqi_set_key(vap, key_idx, 0, 0, key, 0, key_len, 0, cipher);
    return 0;
}

int wuqi_config_auth_open(void *vap_param)
{
    struct ieee80211vap *vap=vap_param;
    int authmode;
    const struct ieee80211_authenticator *auth;


    authmode = IEEE80211_AUTH_OPEN;

    auth = ieee80211_authenticator_get(authmode);
    if (auth == NULL)
        return -EINVAL;

    /* NB: authenticator attach/detach happens on state change */
    vap->iv_bss->ni_authmode = authmode;
    /* XXX mixed/mode/usage? */
    vap->iv_auth = auth;

    return 0;
}

int wuqi_config_ap_security(void *vap_param,int privacy, int auth_type,int wpa_version, int cipher_group, int n_ciphers_pairwise )
{
    struct ieee80211vap *vap=vap_param;
    const struct ieee80211_authenticator *auth;
    uint32_t flags;
    int authmode;

    iot_printf("%s privacy:%d,auth_type:%d, wpa_version:%d, chipher_group:0x%x, n_ciphers_pairwise:%d\n",
            __func__,privacy,auth_type,wpa_version,cipher_group,n_ciphers_pairwise);

    //set auth mode
    if (auth_type == NL80211_AUTHTYPE_AUTOMATIC) //hostapd-0.7.3 will pass 5.
        authmode = IEEE80211_AUTH_OPEN;
    else if ((auth_type & NL80211_AUTHTYPE_OPEN_SYSTEM) && (auth_type & NL80211_AUTHTYPE_SHARED_KEY))
        authmode = IEEE80211_AUTH_AUTO;
    else if (auth_type & NL80211_AUTHTYPE_SHARED_KEY)
        authmode = IEEE80211_AUTH_SHARED;
    else
        authmode = IEEE80211_AUTH_OPEN;

    if(wpa_version)
        authmode=IEEE80211_AUTH_WPA;

    auth = ieee80211_authenticator_get(authmode);
    if (auth == NULL)
        return -EINVAL;

    switch (authmode) {
        case IEEE80211_AUTH_WPA:    /* WPA w/ 802.1x */
            vap->iv_flags |= IEEE80211_F_PRIVACY;
            break;
        case IEEE80211_AUTH_OPEN:   /* open */
            vap->iv_flags &= ~(IEEE80211_F_WPA|IEEE80211_F_PRIVACY);
            break;
        case IEEE80211_AUTH_SHARED: /* shared-key */
        case IEEE80211_AUTH_8021X:  /* 802.1x */
            vap->iv_flags &= ~IEEE80211_F_WPA;
            /* both require a key so mark the PRIVACY capability */
            vap->iv_flags |= IEEE80211_F_PRIVACY;
            break;
        case IEEE80211_AUTH_AUTO:   /* auto */
            vap->iv_flags &= ~IEEE80211_F_WPA;
            /* XXX PRIVACY handling? */
            /* XXX what's the right way to do this? */
            break;
    }
    /* NB: authenticator attach/detach happens on state change */
    vap->iv_bss->ni_authmode = authmode;
    /* XXX mixed/mode/usage? */
    vap->iv_auth = auth;

    //set WPA version
    if (wpa_version > 3)
        return -EINVAL;
    /* XXX verify ciphers available */
    flags = vap->iv_flags & ~IEEE80211_F_WPA;
    switch (wpa_version) {
        case 0:
            /* wpa_supplicant calls this to clear the WPA config */
            break;
        case 1:
            if (!(vap->iv_caps & IEEE80211_C_WPA1))
                return -EOPNOTSUPP;
            flags |= IEEE80211_F_WPA1;
            break;
        case 2:
            if (!(vap->iv_caps & IEEE80211_C_WPA2))
                return -EOPNOTSUPP;
            flags |= IEEE80211_F_WPA2;
            break;
        case 3:
            if ((vap->iv_caps & IEEE80211_C_WPA) != IEEE80211_C_WPA)
                return -EOPNOTSUPP;
            flags |= IEEE80211_F_WPA1 | IEEE80211_F_WPA2;
            break;
        default:	/*  Can't set any -> error */
            return -EOPNOTSUPP;
    }
    vap->iv_flags = flags;

    //privacy
    if (privacy) {
        /* XXX check for key state? */
        vap->iv_flags |= IEEE80211_F_PRIVACY;
    } else
        vap->iv_flags &= ~IEEE80211_F_PRIVACY;

    //HTCONF, only support 20MHz
    ieee80211_syncflag_ht(vap, IEEE80211_FHT_HT);

    iot_printf("%s vap iv_flags:0x%x, authmode:%d\n",__func__,vap->iv_flags,authmode);
    return 0;
}

int wuqi_config_sta_security(void *vap_param, int wpa_version)
{
    struct ieee80211vap *vap = vap_param;

    /* reset wpa flag in vap->iv_caps */
    vap->iv_caps &= ~IEEE80211_F_WPA;

    switch (wpa_version) {
        case 0:
            /* wpa_supplicant calls this to clear the WPA config */
            break;
        case 1:
            vap->iv_flags |= IEEE80211_F_WPA1;
            break;

        case 2:
            vap->iv_flags |= IEEE80211_F_WPA2;
            break;

        case 3:
            vap->iv_flags |= IEEE80211_F_WPA1 | IEEE80211_F_WPA2;
            break;

        default:    /*  Can't set any -> error */
            return -EOPNOTSUPP;
    }

    return 0;
}

uint8_t wuqi_wmeparams2log2(uint16_t input)
{
    uint16_t val;
    uint8_t output = 0;

    val = input + 1;

    while((val >> 1) != 0)
    {
        val = val >> 1;
        output++;
    }
    return output;
}

void map_hostapd_params_to_driver(uint8_t ac, struct ieee80211vap *vap, struct wuqi_ac_txq_params *ac_txq_params)
{
    struct ieee80211com *ic = vap->iv_bss->ni_ic;
    //struct ieee80211com *ic = vap->iv_ic;
    struct chanAccParams *chp = &(ic->ic_wme.wme_chanParams);
    struct chanAccParams *chp2 = &(ic->ic_wme.wme_bssChanParams);
    struct wmeParams *wmeparams = chp->cap_wmeParams;

    wmeparams[ac].wmep_acm = ac_txq_params->acm;
    wmeparams[ac].wmep_aifsn = ac_txq_params->aifs;
    wmeparams[ac].wmep_txopLimit = ac_txq_params->txop;
    wmeparams[ac].wmep_logcwmax = wuqi_wmeparams2log2(ac_txq_params->cwmax);
    wmeparams[ac].wmep_logcwmin = wuqi_wmeparams2log2(ac_txq_params->cwmin);

    iot_printf("update broadcast wmm params\n");
    wmeparams = chp2->cap_wmeParams;
    wmeparams[ac].wmep_acm = ac_txq_params->acm;
    wmeparams[ac].wmep_aifsn = ac_txq_params->aifs;
    wmeparams[ac].wmep_txopLimit = ac_txq_params->txop;
    wmeparams[ac].wmep_logcwmax = wuqi_wmeparams2log2(ac_txq_params->cwmax);
    wmeparams[ac].wmep_logcwmin = wuqi_wmeparams2log2(ac_txq_params->cwmin);
}

int wuqi_set_txq_params(void *vap_params,void *ac_params)
{
    struct ieee80211vap *vap = (struct ieee80211vap *)vap_params;
    struct wuqi_ac_txq_params *ac_txq_params = (struct wuqi_ac_txq_params *)ac_params;

    switch(ac_txq_params->acm)
    {
        case WME_AC_BE:
            map_hostapd_params_to_driver(WME_AC_VO, vap, ac_txq_params);
            break;
        case WME_AC_BK:
            map_hostapd_params_to_driver(WME_AC_VI, vap, ac_txq_params);
            break;
        case WME_AC_VI:
            map_hostapd_params_to_driver(WME_AC_BE, vap, ac_txq_params);
            break;
        case WME_AC_VO:
            map_hostapd_params_to_driver(WME_AC_BK, vap, ac_txq_params);
            break;
        default:
            iot_printf("error ac queue\n");
            return ERR_TXQ_INFO;
    }

    return 0;
}

void wuqi_uptate_txq_params(void *vap_params)
{
    struct ieee80211vap *vap = (struct ieee80211vap *)vap_params;
    struct ieee80211com *ic = vap->iv_ic;

    /*update the txq params*/
    ic->ic_wme.wme_update(ic);
}


int wuqi_get_station_info(void *sinfo_params, void *vap_params, const u8 *mac)
{
    struct ieee80211_node *ni;
    struct wuqi_station_info *sinfo;

    struct ieee80211vap *vap = (struct ieee80211vap *)vap_params;
    struct ieee80211_node_table *nt = &vap->iv_ic->ic_sta;
    sinfo = (struct wuqi_station_info *)sinfo_params;

    ni = vap->iv_bss;
    if (vap->iv_opmode == IEEE80211_M_STA)
    {
        if(memcmp(mac, ni->ni_bssid, sizeof(ni->ni_bssid)) != 0)
        {
            iot_printf("no such device\n");
            return -ENOENT;
        }
    }
    if (vap->iv_opmode == IEEE80211_M_HOSTAP)
    {
        ni = ieee80211_find_node(nt, mac);
        if (ni == NULL)
        {
            iot_printf("no such station\n");
            return -ENOENT;
        }
    }

    sinfo->tx_packets = ni->ni_stats.ns_tx_data;
    sinfo->tx_bytes = ni->ni_stats.ns_tx_bytes;

    sinfo->rx_packets = ni->ni_stats.ns_rx_data;
    sinfo->rx_bytes = ni->ni_stats.ns_rx_bytes;

    if (vap->iv_opmode == IEEE80211_M_HOSTAP && ni != NULL)
        ieee80211_free_node(ni);
    return 0;
}

int wuqi_get_channel_params(void *vap_params, uint32_t vif_ctx_state, void *channel_params)
{
    struct ieee80211vap *vap = (struct ieee80211vap *)vap_params;
    struct ieee80211com *iv_ic = vap->iv_ic;
    struct wuqi_chan_info *channel_info = (struct wuqi_chan_info *)channel_params;

    if(vap->iv_opmode == IEEE80211_M_STA)
    {
        iot_printf("sta mode\n");
        if(!(vif_ctx_state & VIF_STATE_CONNECTED))
        {
            iot_printf("not connected to ap yet, have no effective info\n");
            return -ERR_CHAN_INFO;
        }
    }

    else if(vap->iv_opmode == IEEE80211_M_HOSTAP)
    {
        iot_printf("ap mode\n");
    }

    else
    {
        iot_printf("mode not support yet\n");
        return -ERR_CHAN_INFO;
    }

    channel_info->center_freq = iv_ic->ic_curchan->ic_freq;
    channel_info->chnum = iv_ic->ic_curchan->ic_ieee;

    /*get channel band*/
    if(iv_ic->ic_curchan->ic_flags & IEEE80211_CHAN_2GHZ)
    {
        channel_info->band = NL80211_BAND_2GHZ;
    }
    else if(iv_ic->ic_curchan->ic_flags & IEEE80211_CHAN_5GHZ)
    {
        channel_info->band = NL80211_BAND_5GHZ;
    }
    else
    {
        iot_printf("band not support yet\n");
        return -ERR_CHAN_INFO;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0)
    /*get channel width*/
    if(iv_ic->ic_curchan->ic_flags & IEEE80211_CHAN_HT20)
    {
        channel_info->width = NL80211_CHAN_WIDTH_20;
    }
    else if(iv_ic->ic_curchan->ic_flags & IEEE80211_CHAN_HT40)
    {
        channel_info->width = NL80211_CHAN_WIDTH_40;
    }
    else
    {
        /* hawk station will not get IE 45 (HT capability) from AP with WEP/TKIP mode */
        if (vap->iv_opmode == IEEE80211_M_STA && (vif_ctx_state & VIF_STATE_CONNECTED))
        {
            channel_info->width = NL80211_CHAN_WIDTH_20;
            return 0;
        }
        iot_printf("width not support yet\n");
        return -ERR_CHAN_INFO;
    }
#endif

    return 0;
}


#define	IS_UP_AUTO(_vap) \
    (IFNET_IS_UP_RUNNING((_vap)->iv_ifp))

int wuqi_config_channel_data_only(void *vap_param, int chan_freq)
{
    struct ieee80211vap *vap=vap_param;
    struct ieee80211com *ic = vap->iv_ic;
    struct ieee80211_channel *c;
    uint32_t ic_flags=IEEE80211_CHAN_HT20|IEEE80211_CHAN_DYN|IEEE80211_CHAN_2GHZ;

    c = ieee80211_find_channel(ic, chan_freq, ic_flags);

    iot_printf("%s:%d chan_freq:%d\n",__func__,__LINE__,chan_freq);
    if(c)
        hawk_vap0->iv_bss->ni_chan = hawk_vap0->iv_des_chan=c;
    else
        return -ENODATA;

    return 0;
}

int wuqi_config_channel_dpd(void *vap_param, int chan_freq, int chan_info, uint8_t sw_ch_cali)
{
    struct ieee80211vap *vap=vap_param;
    struct ieee80211com *ic = vap->iv_ic;
    struct ieee80211_channel *c;
    uint32_t flags;

    flags = IEEE80211_CHAN_HT20|IEEE80211_CHAN_DYN|IEEE80211_CHAN_2GHZ;
    c = ieee80211_find_channel(ic, chan_freq, flags);
    if(c == NULL)
    {
        printk("%s: target channel not found!!\n", __func__);
        return 0;
    }

    ic->ic_curchan = c;
    ic->ic_bsschan = c;
    ic->sw_ch_cali = sw_ch_cali;
    ic->ic_set_channel(ic);
    ic->sw_ch_cali = 0;

    return 0;
}

int wuqi_change_channel_dpd()
{
    struct ieee80211com *ic = &wdrv_sc->sc_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct ieee80211_channel *chan = ic->ic_curchan;
    int ret = 0;


    if(chan->ic_maxregpower == 0)
        chan->ic_maxregpower = 20;
    iot_printf("%s: channel set %d\n", __func__, chan->ic_ieee);
    ret = htc_ch_bw_notify(sc, chan->ic_ieee, chan->ic_freq, chan->ic_maxregpower, 0);

    return 0;
}

int wuqi_config_channel(void *vap_param, int chan_freq)
{
    struct ieee80211vap *vap=vap_param;
    struct ieee80211_channel channel={0};
    struct ieee80211req ireq;

    iot_printf("%s:%d chan_freq:%d\n",__func__,__LINE__,chan_freq);
    channel.ic_freq = chan_freq;
    channel.ic_flags=IEEE80211_CHAN_HT20|IEEE80211_CHAN_DYN|IEEE80211_CHAN_2GHZ;

    memset(&ireq, 0, sizeof(ireq));
    ireq.i_type=IEEE80211_IOC_CURCHAN;
    ireq.i_val=0;
    ireq.i_data = &channel;
    ireq.i_len = sizeof(channel);

    return (0-ieee80211_ioctl_setcurchan(vap, &ireq));
}

bool wuqi_chan_power_valid(void *vap_params, uint16_t freq)
{
    struct ieee80211vap *vap = (struct ieee80211vap *)vap_params;
    struct ieee80211com *ic = vap->iv_ic;
    int i;

    for (i = 0; i < ic->ic_nchans; i++) {
        if (freq == ic->ic_channels[i].ic_freq) {
            /* update power */
            if (ic->ic_channels[i].ic_maxregpower) {
                return true;
            }
        }
    }

    return false;
}

void wuqi_chan_freq_set(void *vap_params, u32 first_freq, u32 last_freq)
{
    struct ieee80211vap *vap = (struct ieee80211vap *)vap_params;

    vap->scan_first_freq = first_freq;
    vap->scan_last_freq = last_freq;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,20,0)
void wuqi_config_chan_power(void *vap_params, uint32_t *freq, int *pwr, uint16_t ch_nbr)
{
#else
void wuqi_config_chan_power(void *vap_params, uint16_t *freq, int *pwr, uint16_t ch_nbr)
{
#endif
    int i, j;
    struct ieee80211vap *vap = (struct ieee80211vap *)vap_params;
    struct ieee80211com *ic = vap->iv_ic;

    /* do not touch power table if controlled by user */
    if (ic->ic_txpowctrl == 1) {
        return;
    }

    /* clear power table if null args */
    if ((freq == NULL) || (pwr == NULL)) {
        for (i = 0; i < ic->ic_nchans; i++) {
            /* clear power */
            ic->ic_channels[i].ic_maxregpower = 0;
        }
        return;
    }

    for (i = 0; i < ch_nbr; i++) {
        for (j = 0; j < ic->ic_nchans; j++) {
            if (freq[i] == ic->ic_channels[j].ic_freq) {
                /* update power */
                ic->ic_channels[j].ic_maxregpower = pwr[i];
            }
        }
    }

    //ieee80211_announce_channels(ic);
}

void wuqi_user_chan_power(void *vap_params, int *freq, uint8_t *power, int ch_nbr)
{
    struct ieee80211com *ic = net80211_ic;
    int i, j;

    if ((freq == NULL) || (power == NULL)) {
        printk("%s invalid param\n", __func__);
        return;
    }

    /* write power table */
    for (i = 0; i < ch_nbr; i++) {
        for (j = 0; j < ic->ic_nchans; j++) {
            if (freq[i] == ic->ic_channels[j].ic_freq) {
                ic->ic_channels[j].ic_maxregpower = power[i];
                printk("%s chan:%d power:%d\n", __func__, freq[i], power[i]);
            }
        }
    }

    ic->ic_txpowctrl = 1;
}

int wuqi_vap_ap_start(void *vap_param)
{
    struct ieee80211vap *vap=vap_param;

    //if(IS_UP_AUTO(vap))
    //    ieee80211_init(vap);
	IEEE80211_LOCK(vap->iv_ic);
	ieee80211_start_locked(vap);
	IEEE80211_UNLOCK(vap->iv_ic);

    return 0;
}

int wuqi_vap_ap_stop(void *vap_param)
{
    struct ieee80211vap *vap=vap_param;

	IEEE80211_LOCK(vap->iv_ic);
	ieee80211_stop_locked(vap);
	IEEE80211_UNLOCK(vap->iv_ic);

    return 0;
}

int wuqi_vap_config_appie(void *vap_param, int16_t appie_type, const u8 *ie, int ie_len)
{
    struct ieee80211vap *vap = (struct ieee80211vap *)vap_param;
	struct ieee80211req ireq;

    if (ie != NULL)
    	dump_bytes("wuqi_vap_config_appie->wuqi_parse_sme_ie:",(u8 *) ie,ie_len);
    	
    memset(&ireq, 0, sizeof(ireq));
    ireq.i_type=IEEE80211_IOC_APPIE;
    switch (appie_type)
    {
        case IEEE80211_ADDIE_PROBE_REQ:
            ireq.i_val=IEEE80211_FC0_SUBTYPE_PROBE_REQ;
            break;
        case IEEE80211_ADDIE_AUTH_REQ:
            ireq.i_val=IEEE80211_FC0_SUBTYPE_AUTH;
            break;
        case IEEE80211_ADDIE_ASOC_REQ:
            ireq.i_val=IEEE80211_FC0_SUBTYPE_ASSOC_REQ;
            break;
        default:
            return 0;
    }
    ireq.i_data = (void *) ie;
    ireq.i_len = ie_len;
    return (0-ieee80211_ioctl_setappie(vap, &ireq));
}

void wuqi_config_privacy(void *vap_param, int privacy)
{
    struct ieee80211vap *vap=vap_param;
    iot_printf("privacy:%d\n",privacy);

    if (privacy) {
        /* XXX check for key state? */
        vap->iv_flags |= IEEE80211_F_PRIVACY;
    } else
        vap->iv_flags &= ~IEEE80211_F_PRIVACY;
}

void wuqi_disable_ht_cap(void *vap_param)
{
    ieee80211_syncflag_ht((struct ieee80211vap *)vap_param, -IEEE80211_FHT_HT);
}

void wuqi_enable_ht_cap(void *vap_param)
{
    ieee80211_syncflag_ht((struct ieee80211vap *)vap_param, IEEE80211_FHT_HT);
}

void wuqi_set_shortslottime_flag(void *vap_param)
{
    struct ieee80211vap *vap=vap_param;
    struct ieee80211com *ic =vap->iv_ic;

    ieee80211_set_shortslottime(ic,
    IEEE80211_IS_CHAN_A(ic->ic_curchan) ||
    IEEE80211_IS_CHAN_HT(ic->ic_curchan) ||
    (IEEE80211_IS_CHAN_ANYG(ic->ic_curchan) && ic->ic_opmode == IEEE80211_M_HOSTAP &&
    (ic->ic_caps & IEEE80211_C_SHSLOT)));
}

void wuqi_set_extap(void *vap_param, u8 enable)
{
    struct ieee80211vap *vap = vap_param;

    if (enable) {
        vap->iv_caps |= IEEE80211_C_EXTAP;
    } else {
        vap->iv_caps &= ~IEEE80211_C_EXTAP;
    }
}

void wuqi_set_fixed_rate(void *vap_param, u8 fixed_rate)
{
    struct ieee80211vap *vap = vap_param;
    struct ieee80211com *ic = vap->iv_ic;
    struct wq_softc *sc = ic->ic_softc;
    htc_dev_info_notify(sc, MP_FIXED_RATE_INFO, NULL, fixed_rate);
}

void wuqi_dhcp_debug_msg(struct ethhdr *eh)
{
    if(htons(eh->h_proto) == ETH_P_IP){
        struct iphdr *iph = (struct iphdr *)(eh + 1);
        if(iph->protocol == PROTO_UDP){
            struct udphdr *udph = (struct udphdr *)(iph + 1);
            if(udph->dest == htons(DHCP_PORT))
            {
                net_dhcphdr_t *dhcph = (net_dhcphdr_t *)(udph + 1);
                iot_printf("[DHCP]transaction id: 0x%x\n", dhcph->dhcp_transc_id);
            }
        }
    }
}

int wuqi_ping_debug_msg(struct ethhdr *eh, struct mbuf *m)
{
    if(htons(eh->h_proto) == ETH_P_IP)
    {
        struct iphdr *iph = (struct iphdr *)(eh + 1);
        if(iph->protocol == PROTO_ICMP)
        {
            struct icmphdr *icmph = (struct icmphdr *)(iph + 1);
            m->m_ping_seq = htons(icmph->un.echo.sequence);
            /* ping reply frame */
            if(icmph->type == 0) {
                m->m_flags_ext |= M_IS_PING_REPLY;
                if(wq_dbg_flag & WQ_DEBUG_PING) {
                    iot_printf("%s: Ping reply sequence is 0x%04x\n", __func__, m->m_ping_seq);
                }
            }
            /* ping request frame */
            else if(icmph->type == 8) {
                m->m_flags_ext |= M_IS_ICMP;
                if(wq_dbg_flag & WQ_DEBUG_PING) {
                    iot_printf("%s: Ping sequence is 0x%04x\n", __func__, htons(icmph->un.echo.sequence));
                }
            }
            return 1;
        }
    }
    return 0;
}

int wuqi_del_station(void *vap_param, int reason, const u8* mac)
{
    struct ieee80211vap *vap = vap_param;
    struct ieee80211_node_table *nt = &vap->iv_ic->ic_sta;
    struct ieee80211_node *ni;

    if(nt == NULL)
    {
        iot_printf("%s, The node table is NULL, return!\n", __func__);
        return 0;
    }
    IEEE80211_DISCON_LOCK(vap);
    ni = ieee80211_find_node(nt, mac);
    if(ni == NULL)
    {
        iot_printf("%s: The node not exist!\n",__func__);
        IEEE80211_DISCON_UNLOCK(vap);
        return 0;
    }
    if(ni->ni_associd >0 && ni->ni_recv_eapol)
    {
        IEEE80211_SEND_MGMT(ni, IEEE80211_FC0_SUBTYPE_DEAUTH, reason);
        ieee80211_node_leave(ni);
        iot_printf("%s: delete node succss!\n", __func__);
    }
    ieee80211_free_node(ni);
    IEEE80211_DISCON_UNLOCK(vap);
    return 0;
}

void vap_attach_to_new_ic(void *vap_param)
{
    struct ieee80211vap *vap = vap_param;
    vap->iv_ic = &wdrv_sc->sc_ic;
}

int wuqi_set_edca_params(void *vap_param, struct wuqi_ac_txq_params *edca)
{
#define WQ_EXPONENT_TO_VALUE(v)    ((1<<v)-1)
    int ret = -1;
    struct ieee80211vap *vap = vap_param;
    struct wq_softc *sc;
    struct wq_hal *mh ;
    uint32_t cwmax,cwmin;


    if (!vap || !vap->iv_ic || !vap->iv_ic->ic_softc)
        return ret;

    sc = vap->iv_ic->ic_softc;
    mh = sc->sc_mh;

    if(!mh)
        return ret;

    iot_printf("%s(), iv_opmode:%d \n", __func__, vap->iv_opmode);


    switch(vap->iv_opmode){
        case IEEE80211_M_HOSTAP:
            if (wuqi_set_txq_params(vap, edca)) {
                iot_printf("set txq failed\n");
                return ret;
            }
            wuqi_uptate_txq_params(vap);

            iot_printf("%s in softap mode, need beacon notify\n", __func__);
            ieee80211_beacon_notify(vap, IEEE80211_BEACON_WME);

            break;
        case IEEE80211_M_STA:
            cwmin = wuqi_wmeparams2log2(edca->cwmin);
            cwmax = wuqi_wmeparams2log2(edca->cwmax);

            ret = wq_hal_setedcaparams(mh, edca->acm, WQ_EXPONENT_TO_VALUE(cwmin), WQ_EXPONENT_TO_VALUE(cwmax),
                    edca->aifs,edca->txop, vap->iv_ic->ic_flags & IEEE80211_F_SHSLOT?9:20);
            break;
        default:
            iot_printf("Current Mode is Not Support.\n");
            break;
    }

    return ret;

}

void wuqi_get_edca_params(void *vap_param)
{
    struct ieee80211vap *vap = vap_param;
    int slottime;

    if (!vap || !vap->iv_ic)
        return;

    switch (vap->iv_opmode){
        case IEEE80211_M_HOSTAP:
            if(!vap->iv_bss || !vap->iv_bss->ni_ic) {
                return;
            }

             slottime = vap->iv_bss->ni_ic->ic_flags & IEEE80211_F_SHSLOT?9:20;
             break;
        case IEEE80211_M_STA:
            slottime = vap->iv_ic->ic_flags & IEEE80211_F_SHSLOT?9:20;
            break;
        default:
            iot_printf("Current Mode is Not Support.\n");
            return;
    }



    iot_printf("%s(), slottime:%d iv_opmode:%d\n", __func__, slottime, vap->iv_opmode);

    iot_printf("WMM_BE\n");
    iot_printf("cw max:0x%x\n", mac_txq0_cw_sw_txq0_cw_max_get());
    iot_printf("cw min:0x%x\n", mac_txq0_cw_sw_txq0_cw_min_get());
    iot_printf("txop limit num:0x%x\n", mac_txq0_period_sw_txq0_txop_get()/32);
    iot_printf("eifs:0x%x\n", mac_txq0_ifs_ctrl_sw_txq0_eifs_get());
    iot_printf("aifs num:0x%x\n", (mac_txq0_ifs_ctrl_sw_txq0_difs_get() - 10)/slottime);
    iot_printf("========================================\n");
    iot_printf("WMM_BK\n");
    iot_printf("cw max:0x%x\n", mac_txq1_cw_sw_txq1_cw_max_get());
    iot_printf("cw min:0x%x\n", mac_txq1_cw_sw_txq1_cw_min_get());
    iot_printf("txop limit num:0x%x\n", mac_txq1_period_sw_txq1_txop_get()/32);
    iot_printf("eifs:0x%x\n", mac_txq1_ifs_ctrl_sw_txq1_eifs_get());
    iot_printf("aifs num:0x%x\n", (mac_txq1_ifs_ctrl_sw_txq1_difs_get() - 10)/slottime);
    iot_printf("========================================\n");
    iot_printf("WMM_VI\n");
    iot_printf("cw max:0x%x\n", mac_txq2_cw_sw_txq2_cw_max_get());
    iot_printf("cw min:0x%x\n", mac_txq2_cw_sw_txq2_cw_min_get());
    iot_printf("txop limit num:0x%x\n", mac_txq2_period_sw_txq2_txop_get()/32);
    iot_printf("eifs:0x%x\n", mac_txq2_ifs_ctrl_sw_txq2_eifs_get());
    iot_printf("aifs num:0x%x\n", (mac_txq2_ifs_ctrl_sw_txq2_difs_get() - 10)/slottime);
    iot_printf("========================================\n");
    iot_printf("WMM_VO\n");
    iot_printf("cw max:0x%x\n", mac_txq3_cw_sw_txq3_cw_max_get());
    iot_printf("cw min:0x%x\n", mac_txq3_cw_sw_txq3_cw_min_get());
    iot_printf("txop limit num:0x%x\n", mac_txq3_period_sw_txq3_txop_get()/32);
    iot_printf("eifs:0x%x\n", mac_txq3_ifs_ctrl_sw_txq3_eifs_get());
    iot_printf("aifs num:0x%x\n", (mac_txq3_ifs_ctrl_sw_txq3_difs_get() - 10)/slottime);

    return;
}

void wuqi_set_nav_enable(bool enable)
{
    if(enable) {
        mac_nav_set_sw_nav_force_en_set(0);
    } else {
        mac_nav_set_sw_nav_force_en_set(1);
        mac_nav_set_sw_nav_force_val_set(0);
    }

    return;
}

void wuqi_get_nav_enable(void)
{
    iot_printf("sw_nav_force_en:%d\n", mac_nav_set_sw_nav_force_en_get());
    iot_printf("sw_nav_force_val:%d\n", mac_nav_set_sw_nav_force_val_get());

    return;
}

void wuqi_set_sgi_enable(void *vap_param, bool enable)
{
    struct ieee80211vap *vap = vap_param;

    if(!vap)
        return;

    iot_printf("%s(), iv_opmode:%d\n", __func__, vap->iv_opmode);

    switch(vap->iv_opmode){
        case IEEE80211_M_HOSTAP:
            if (enable) {
                vap->iv_bss->ni_vap->iv_flags_ht |= IEEE80211_FHT_SHORTGI20;
            } else {
                vap->iv_bss->ni_vap->iv_flags_ht &= ~IEEE80211_FHT_SHORTGI20;
            }

            iot_printf("%s in softap mode, need beacon notify\n", __func__);
            ieee80211_beacon_notify(vap, IEEE80211_BEACON_WME);
            break;
        case IEEE80211_M_STA:
            if(enable) {
                //vap->iv_htcaps |= IEEE80211_HTCAP_SHORTGI20;
                vap->iv_flags_ht |= IEEE80211_FHT_SHORTGI20;

            } else {
                //vap->iv_htcaps &= ~IEEE80211_HTCAP_SHORTGI20;
                vap->iv_flags_ht &= ~IEEE80211_FHT_SHORTGI20;
            }
            break;
        default:
            iot_printf("Current Mode is Not Support.\n");
            break;
    }

    return;
}

bool wuqi_get_sgi_enable(void *vap_param)
{
    bool ret = false;
    struct ieee80211vap *vap = vap_param;

    if(!vap)
        return ret;
    iot_printf("%s(), iv_opmode:%d\n", __func__, vap->iv_opmode);

    switch(vap->iv_opmode){
        case IEEE80211_M_HOSTAP:
            if(!vap->iv_bss || !vap->iv_bss->ni_vap) {
                iot_printf("set bss short gi fialed\n");
                return ret;
            }

            if (vap->iv_bss->ni_vap->iv_flags_ht & IEEE80211_FHT_SHORTGI20)
                ret = true;
            break;
        case IEEE80211_M_STA:
            //if (vap->iv_htcaps & IEEE80211_HTCAP_SHORTGI20)
            if (vap->iv_flags_ht & IEEE80211_FHT_SHORTGI20)
                ret = true;
            break;
        default:
            iot_printf("Not Support Current Mode:%d\n", vap->iv_opmode);
            break;
    }

    return ret;
}

void wuqi_set_ampdu_agg_num(void *vap_param, uint8_t  num)
{
    struct ieee80211vap *vap = vap_param;
    struct wq_softc *sc;

    if (!vap || !vap->iv_ic || !vap->iv_ic->ic_softc)
        return ;

    sc = vap->iv_ic->ic_softc;
    if(!sc)
        return;

    htc_ampdu_agg_set_notify(sc, num);

    return;
}

int wuqi_get_ampdu_agg_num(void *vap_param)
{
    int ret = -1;
    struct ieee80211vap *vap = vap_param;
    struct wq_softc *sc;

    if (!vap || !vap->iv_ic || !vap->iv_ic->ic_softc)
        return ret;

    sc = vap->iv_ic->ic_softc;
    if(!sc)
        return ret;

    ret = htc_get_ampdu_aggr_num(sc);

    return ret;
}

/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2003-2009 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * IEEE 802.11 support (FreeBSD-specific code)
 */
#include <net80211/ieee80211_var.h>
#include <net80211/ieee80211_input.h>
#include <net80211/ieee80211_extap.h>


//void ieee80211_vht_init(void);
void ieee80211_phy_init(void);
void ieee80211_auth_setup(void);
void ieee80211_ht_init(void);
void* net80211_thread(void* argu);
//void ic_list_lock_init(void);
extern const struct ieee80211_authenticator xauth;
extern const struct ieee80211_cipher wep;
extern const struct ieee80211_cipher tkip;
extern const struct ieee80211_cipher ccmp;
uint32_t get_ni_from_idx(struct ieee80211com *ic, int node_idx);
extern int wuqi_new_sta_notify(void *vap_ctx, char *mac, char *ies, int ies_len);
extern int wuqi_del_sta_notify(void *vap_ctx, char *mac);
extern int wuqi_mic_fail_notify(void *vap_ctx, char *mac,int key_id);
extern int wuqi_ping_debug_msg(struct ethhdr *eh, struct mbuf *m);

#if 0 //ifndef RISCV
struct timespec bootime;

void time_init()
{
    //unsigned int tick;
    //struct timespec curr_ts;
    clock_gettime(CLOCK_REALTIME, &bootime);

}

int tick_get()
{
    struct timespec curr_ts;
    unsigned int tick;

    clock_gettime(CLOCK_REALTIME, &curr_ts);
    tick=(curr_ts.tv_sec-bootime.tv_sec)*1000+(curr_ts.tv_nsec-bootime.tv_nsec)/1000000;

    return tick;
}
#endif

void wproto_start(void)
{

    iot_printf("net80211_init : initialize net80211\n");
    //time_init();
    //Initialize net80211

    //SYSINIT(wlan_phy, SI_SUB_DRIVERS, SI_ORDER_FIRST, ieee80211_phy_init, NULL);
    ieee80211_phy_init();
    //SYSINIT(wlan_ht, SI_SUB_DRIVERS, SI_ORDER_FIRST, ieee80211_ht_init, NULL);
    ieee80211_ht_init();
    //SYSINIT(wlan_auth, SI_SUB_DRIVERS, SI_ORDER_FIRST, ieee80211_auth_setup, NULL);
    ieee80211_auth_setup();
    //SYSINIT(wlan_vht, SI_SUB_DRIVERS, SI_ORDER_FIRST, ieee80211_vht_init, NULL);
    //ieee80211_vht_init();
    
    //ic_list_lock_init();

    ieee80211_authenticator_register(IEEE80211_AUTH_8021X, &xauth);
    ieee80211_authenticator_register(IEEE80211_AUTH_WPA, &xauth);
    
    ieee80211_crypto_register(&ccmp);
    ieee80211_crypto_register(&tkip);
    ieee80211_crypto_register(&wep);


    return;
}


//function to be called by NET80211--------------------------------------------

/*
 * Allocate and setup a management frame of the specified
 * size.  We return the mbuf and a pointer to the start
 * of the contiguous data area that's been reserved based
 * on the packet length.  The data area is forced to 32-bit
 * alignment and the buffer length to a multiple of 4 bytes.
 * This is done mainly so beacon frames (that require this)
 * can use this interface too.
 */
struct mbuf *
ieee80211_getmgtframe(uint8_t **frm, int headroom, int pktlen)
{
	struct mbuf *m;
	u_int len;

	/*
	 * NB: we know the mbuf routines will align the data area
	 *     so we don't need to do anything special.
	 */
	len = roundup(headroom + pktlen, 4);
	KASSERT(len <= MBUF_ALLOC_SIZE, ("802.11 mgt frame too large: %u", len));
	
	m = m_gethdr(M_NOWAIT, MT_DATA);
	if (m != NULL) {
	    
		//m->pkt.data += headroom;
		//m->pkt.end=m->pkt.data;
		//m->m_data = (uint32_t)m->pkt.data;
		m->m_data += headroom;
		*frm = (uint8_t *)m->m_data;
	}
	return m;
}

int
ieee80211_add_callback(struct mbuf *m, struct ieee80211_node *ni,
	void (*func)(struct ieee80211_node *, void *, int), void *arg)
{
	IOT_ASSERT(m->cbfunc == NULL);
	m->cbfunc = (void (*)(void *, void *, int))func;
	m->cbarg = arg;
	m->m_flags |= M_TXCB;
	m->cbni = (void *)ni;
	return 1;
}

int
ieee80211_add_rx_callback(struct mbuf *m,
	void (*func)(struct ieee80211_node *, void *, int), void *arg)
{
	m->cbfunc = (void (*)(void *, void *, int))func;
	m->cbarg = arg;
	m->m_flags |= M_RXCB;
	return 1;
}

FAST_ATTR void
ieee80211_process_callback(struct ieee80211_node *ni,
	struct mbuf *m, int status)
{
    if (m->cbfunc) {
		m->cbfunc(ni, m->cbarg, status);
    }
    m->cbfunc = NULL;
}


/*
 * Transmit a frame to the parent interface.
 */
int
FAST_ATTR ieee80211_parent_xmitpkt(struct ieee80211com *ic, struct mbuf *m)
{
	int error;

	/*
	 * Assert the IC TX lock is held - this enforces the
	 * processing -> queuing order is maintained
	 */
	IEEE80211_TX_LOCK_ASSERT(ic);
	if (m->m_flags_ext & M_DIRECT_FRM) {
		/* for direct frames from supplicant, handover to wdrv
		 * task by ic_raw_xmit to avoid race condition on driver tx
		 */
		error = ic->ic_raw_xmit((struct ieee80211_node *)m->m_pkthdr.rcvif, m);
	} else {
		error = ic->ic_transmit(ic, m);
	}
	if (error) {
		struct ieee80211_node *ni;

		ni = (struct ieee80211_node *)m->m_pkthdr.rcvif;

		/* XXX number of fragments */
		net_if_inc_counter(ni->ni_vap->iv_ifp, IFCOUNTER_OERRORS, 1);
        iot_printf("%s: drop frame, len=%d\n", __func__, m->m_len);
		ieee80211_free_node(ni);
		ieee80211_free_mbuf(m);
	}
	return (error);
}

/* mbufq for wifi tx */
#define WIFI_MAX_MBUFQLEN           30
struct mbufq {
    STAILQ_HEAD(, mbuf)     mq_head;
    int                     mq_len;
    int                     mq_maxlen;
};

struct wifi_tx_ctx {
    struct wuqi_work_struct  wifi_tx_task;
    struct osal_mutex_t wifi_tx_mtx;
    struct mbufq mq;
};

struct wifi_tx_ctx wifi_ctx;
struct mbuf *wifi_tx_dequeue(struct mbufq *mq);

FAST_ATTR void wifi_tx_proc(struct work_struct * work)
{
    struct ifnet *ifp = (struct ifnet *)WORK_ARGV(work);
    struct mbuf *m;

    while (1) {
        m = wifi_tx_dequeue(&wifi_ctx.mq);
        if (m) {
            ifp->if_transmit(ifp, m);
        } else {
            break;
        }
    }
}

FAST_ATTR void wifi_tx_work_start(struct ieee80211com *ic, struct ieee80211vap *vap)
{
    struct ifnet *ifp = vap->iv_ifp;
    wk_queue_insert_ex(ic->ic_wdrvq, &wifi_ctx.wifi_tx_task, (void *)ifp);
}

void wifi_tx_attach()
{
    wk_init(&wifi_ctx.wifi_tx_task, wifi_tx_proc, NULL);
    osal_mutex_create(&wifi_ctx.wifi_tx_mtx, 0);

    /* init mq */
    STAILQ_INIT(&wifi_ctx.mq.mq_head);
    wifi_ctx.mq.mq_maxlen = WIFI_MAX_MBUFQLEN;
    wifi_ctx.mq.mq_len = 0;
}

void wifi_tx_detach()
{
    wk_deinit(&wifi_ctx.wifi_tx_task);
    osal_mutex_destory(&wifi_ctx.wifi_tx_mtx);
}

#ifdef AMPDU_TX_DEBUG
int wifi_tx_queue_len()
{
    return wifi_ctx.mq.mq_len;
}
#endif

FAST_ATTR struct mbuf *wifi_tx_dequeue(struct mbufq *mq)
{
    struct mbuf *m;

    osal_mutex_acquire(&wifi_ctx.wifi_tx_mtx);
    m = STAILQ_FIRST(&mq->mq_head);
    if (m) {
        STAILQ_REMOVE_HEAD(&mq->mq_head, m_stailqpkt);
        m->m_nextpkt = NULL;
        mq->mq_len--;
    }
    osal_mutex_release(&wifi_ctx.wifi_tx_mtx);
    return m;
}

FAST_ATTR int wifi_tx_enqueue(struct mbufq *mq, struct mbuf *m)
{
    osal_mutex_acquire(&wifi_ctx.wifi_tx_mtx);
    if (mq->mq_len >= mq->mq_maxlen) {
        osal_mutex_release(&wifi_ctx.wifi_tx_mtx);
        return (ENOBUFS);
    }
    STAILQ_INSERT_TAIL(&mq->mq_head, m, m_stailqpkt);
    mq->mq_len++;
    osal_mutex_release(&wifi_ctx.wifi_tx_mtx);

    return 0;
}

/*
 * Transmit a frame to the VAP interface.
 */
int
FAST_ATTR ieee80211_vap_xmitpkt(struct ieee80211vap *vap, struct mbuf *m)
{
    struct ieee80211com *ic = vap->iv_ic;


    /* enqueue this mbuf and handover to wifi_tx_task
     * for further processing
     */
    if (wifi_tx_enqueue(&wifi_ctx.mq, m) != 0) {
        /* drop due to mbufq full */
        m_freem(m);
    }

    /* start tx work if flow control is not activated */
    if ((ic->ic_flowctrl == 0)) {
        wifi_tx_work_start(ic, vap);
    }

    return 0;
}

int
ieee80211_node_dectestref(struct ieee80211_node *ni)
{
	/* XXX need equivalent of atomic_dec_and_test */
	//atomic_subtract_int(&ni->ni_refcnt, 1);
	//return atomic_cmpset_int(&ni->ni_refcnt, 0, 1);
	osal_atomic_dec((struct osal_atomic_t *)&(ni->ni_refcnt));
	return osal_atomic_check_set((struct osal_atomic_t *)&ni->ni_refcnt, 0, 1);
}

void
ieee80211_load_module(const char *modname)
{
	iot_printf("%s: load %s module.\n", __func__, modname);
}


/*
 * Add RX parameters to the given mbuf.
 *
 * Returns 1 if OK, 0 on error.
 */
int
ieee80211_add_rx_params(struct mbuf *m, const struct ieee80211_rx_stats *rxs)
{
	//m->c_pktflags = rxs->c_pktflags;
	m->c_nf = rxs->c_nf;
	m->c_rssi = rxs->c_rssi;
	//m->c_freq = rxs->c_freq;

	return (1);
}


int
ieee80211_get_rx_params(struct mbuf *m, struct ieee80211_rx_stats *rxs)
{
  
    //rxs->c_pktflags = m->c_pktflags;
	rxs->c_nf = m->c_nf;
	rxs->c_rssi = m->c_rssi;
	//rxs->c_freq = m->c_freq;
  	
	return (0);
}
#if 0
uint32_t
ieee80211_get_rx_params_ptr(struct mbuf *m)
{
    IOT_ASSERT(m);
    return (m->c_pktflags);
}
#endif

//Do not suport TOA
int
ieee80211_get_toa_params(struct mbuf *m, struct ieee80211_toa_params *p)
{
  return (0);
}

//function NET80211 call to notify others--------------------------------------------


/*
 * Helper function for events that pass just a single mac address.
 */
static void
notify_macaddr(struct ifnet *ifp, int op, const uint8_t mac[IEEE80211_ADDR_LEN])
{
	struct ieee80211_join_event iev;

	//CURVNET_SET(ifp->if_vnet);
	os_mem_set(&iev, 0, sizeof(iev));
	IEEE80211_ADDR_COPY(iev.iev_addr, mac);
	route_ieee80211msg(ifp, op, &iev, sizeof(iev));
	//CURVNET_RESTORE();
}

void
ieee80211_notify_node_auth(struct ieee80211_node *ni)
{
	struct ieee80211vap *vap = ni->ni_vap;
	struct ifnet *ifp = vap->iv_ifp;

	iot_printf("%s", "node auth");

	notify_macaddr(ifp, RTM_IEEE80211_AUTH, ni->ni_macaddr);
}


void
ieee80211_notify_csa(struct ieee80211com *ic,
	const struct ieee80211_channel *c, int mode, int count)
{
    struct ieee80211_csa_event iev;
    struct ieee80211vap *vap;
    struct ifnet *ifp;

    os_mem_set(&iev, 0, sizeof(iev));
    iev.iev_flags = c->ic_flags;
    iev.iev_freq = c->ic_freq;
    iev.iev_ieee = c->ic_ieee;
    iev.iev_mode = mode;
    iev.iev_count = count;
    TAILQ_FOREACH(vap, &ic->ic_vaps, iv_next) {
        ifp = vap->iv_ifp;
        //CURVNET_SET(ifp->if_vnet);
        route_ieee80211msg(ifp, RTM_IEEE80211_CSA, &iev, sizeof(iev));
        //CURVNET_RESTORE();
    }
}


void
ieee80211_notify_country(struct ieee80211vap *vap,
	const uint8_t bssid[IEEE80211_ADDR_LEN], const uint8_t cc[2])
{
	struct ifnet *ifp = vap->iv_ifp;
	struct ieee80211_country_event iev;

	os_mem_set(&iev, 0, sizeof(iev));
	IEEE80211_ADDR_COPY(iev.iev_addr, bssid);
	iev.iev_cc[0] = cc[0];
	iev.iev_cc[1] = cc[1];
	//CURVNET_SET(ifp->if_vnet);
	route_ieee80211msg(ifp, RTM_IEEE80211_COUNTRY, &iev, sizeof(iev));
	//CURVNET_RESTORE();
}

extern void update_connection_info(void *vif_ctx, uint8_t *bssid, uint8_t *essid, uint8_t essid_len);
extern void update_bss_info(void *vif_cxt, char *bssid, char *ie, int ie_len, int chan, uint16_t bcn_interval, uint16_t cap, int last,int signal);


void
ieee80211_notify_node_join(struct ieee80211_node *ni, int newassoc)
{
    struct ieee80211vap *vap = ni->ni_vap;
    //struct ifnet *ifp = vap->iv_ifp;
    //int i;

    //CURVNET_SET_QUIET(ifp->if_vnet);
    IEEE80211_NOTE(vap, IEEE80211_MSG_NODE, ni, "%snode join",
            (ni == vap->iv_bss) ? "bss " : "");


    //iot_printf("ic_ieee=%d, ni_intval=%d, ni_capinfo=0x%x, ie_len=%d\n", ni->ni_chan->ic_ieee, ni->ni_intval, ni->ni_capinfo, ni->ni_ies.len);
    //for (i=0; i<ni->ni_ies.len; i++)
    //	iot_printf("%02x ", ni->ni_ies.data[i]);

    if (ni == vap->iv_bss) {
        update_bss_info(vap->vif_ctx, ni->ni_bssid, ni->ni_ies.data, ni->ni_ies.len, ni->ni_chan->ic_ieee, ni->ni_intval, ni->ni_capinfo, 0,ni->ni_avgrssi);
        update_connection_info(vap->vif_ctx, ni->ni_bssid, ni->ni_essid, ni->ni_esslen);
    }else{
        wuqi_new_sta_notify(vap->vif_ctx, ni->ni_macaddr,ni->ni_ies.data, ni->ni_ies.len);
    }
#if 0
    if (ni == vap->iv_bss) {
        notify_macaddr(ifp, newassoc ?
                RTM_IEEE80211_ASSOC : RTM_IEEE80211_REASSOC, ni->ni_bssid);
        net_if_link_state_change(ifp, LINK_STATE_UP);
    } else {
        notify_macaddr(ifp, newassoc ?
                RTM_IEEE80211_JOIN : RTM_IEEE80211_REJOIN, ni->ni_macaddr);
    }
#endif
    //CURVNET_RESTORE();
}

extern void update_disconnection_info(void *vif_ctx);


void
ieee80211_notify_node_leave(struct ieee80211_node *ni)
{
	struct ieee80211vap *vap = ni->ni_vap;
	struct ifnet *ifp = vap->iv_ifp;

	//CURVNET_SET_QUIET(ifp->if_vnet);
	IEEE80211_NOTE(vap, IEEE80211_MSG_NODE, ni, "%snode leave",
	    (ni == vap->iv_bss) ? "bss " : "");

	if (ni == vap->iv_bss) {
		//route_ieee80211msg(ifp, RTM_IEEE80211_DISASSOC, NULL, 0);
		//net_if_link_state_change(ifp, LINK_STATE_DOWN);
		update_disconnection_info(vap->vif_ctx);
	} else {
		/* fire off wireless event station leaving */
		wuqi_del_sta_notify(vap->vif_ctx, ni->ni_macaddr);
		notify_macaddr(ifp, RTM_IEEE80211_LEAVE, ni->ni_macaddr);
	}
	//CURVNET_RESTORE();
}


void
ieee80211_route_auth(struct ieee80211vap *vap, void *data, int data_len)
{
	struct ifnet *ifp = vap->iv_ifp;

	IEEE80211_DPRINTF(vap, IEEE80211_MSG_INPUT, "%s\n", "ieee80211_route_auth to supplicant");

	/* dispatch AUTH*/
	//CURVNET_SET(ifp->if_vnet);
	route_ieee80211msg(ifp, RTM_IEEE80211_AUTH, data, data_len);
	//CURVNET_RESTORE();
}

extern int wuqi_rx_mgmt(void *vap_ctx_param, char *buf, int size);

void
ieee80211_route_rxmgmt(struct ieee80211vap *vap, void *data, int data_len)
{
    struct ifnet *ifp = vap->iv_ifp;

    IEEE80211_DPRINTF(vap, IEEE80211_MSG_INPUT, "%s\n", "ieee80211_route_rxmgmt to supplicant");

    /* dispatch RxMGMT*/
    //CURVNET_SET(ifp->if_vnet);
    route_ieee80211msg(ifp, RTM_IEEE80211_RXMGMT, data, data_len);
    wuqi_rx_mgmt(vap->vif_ctx,data,data_len);
    //CURVNET_RESTORE();
}


//uint8_t scan_buf[4096];

extern int ieee80211_ioctl_getscanresults(struct ieee80211vap *vap, struct ieee80211req *ireq);
extern void update_bss_info(void *vif_cxt, char *bssid, char *ie, int ie_len, int chan, uint16_t bcn_interval, uint16_t cap, int last, int signal);

extern int schedule_scan(void *vif_ctx_cxt);
extern int is_cfg80211_scan_done(void *vif_ctx_cxt);

void
ieee80211_notify_scan_done(struct ieee80211vap *vap)
{
	//struct ifnet *ifp = vap->iv_ifp;
    struct ieee80211req ireq;
    char bssid[6];
    uint32_t rest;
    uint8_t *pos;
    struct ieee80211req_scan_result *sr;
	uint8_t *scan_buf;
	uint32_t buf_size;

    if (!is_cfg80211_scan_done(vap->vif_ctx))
	{
		//continue micro-scan
		schedule_scan(vap->vif_ctx);
		return; 
	}
    vap->iv_scandone_flags = 1;
	
	//query scan result size
	ireq.i_len = 0;
	ireq.i_data = NULL;
	ieee80211_ioctl_getscanresults(vap, &ireq);
	if (ireq.i_val > 32768)
		ireq.i_val = 32768;
	buf_size = (ireq.i_val>2048)?ireq.i_val:2048;

	//IEEE80211_DPRINTF(vap, IEEE80211_MSG_SCAN, "%s result=%d\n", "notify scan done", buf_size);

    //alloc needed buffer size
    scan_buf = (uint8_t *)wq_mem_alloc(buf_size, 0);
	if (scan_buf == NULL)
	{
        iot_printf("ieee80211_notify_scan_done kmalloc failed\n");
		update_bss_info(vap->vif_ctx, NULL, NULL, 0, 0, 0, 0, 1,0);
		return;
	}
	
	//query scan result
	ireq.i_len = buf_size;
	ireq.i_data = scan_buf;
	ieee80211_ioctl_getscanresults(vap, &ireq);


    //indidate scan result to kernel
    pos = ireq.i_data;
    rest = ireq.i_len;
    while (rest >= sizeof(struct ieee80211req_scan_result)) {
                sr = (struct ieee80211req_scan_result*)pos;
                //bssid
                os_mem_cpy(bssid, sr->isr_bssid, ETH_ALEN);
                //ssid
                //os_mem_cpy(ap_info_tmp->ssid, ((uint8_t*)sr) + sr->isr_ie_off, sr->isr_ssid_len);
                //ap_info_tmp->ssid_len = sr->isr_ssid_len;
                //bw / pri_channel / sec_channel
                //ap_info_tmp->bandwidth = WIFI_BW_20MHZ;
                //ap_info_tmp->pri_channel = ieee80211_mhz2ieee(sr->isr_freq, sr->isr_flags);
                //rssi
                //ap_info_tmp->rssi = sr->isr_rssi;
				update_bss_info(vap->vif_ctx, bssid, ((uint8_t *)sr) + sr->isr_ie_off + sr->isr_ssid_len, sr->isr_ie_len, ieee80211_mhz2ieee(sr->isr_freq, sr->isr_flags), sr->isr_intval, sr->isr_capinfo, 0, sr->isr_rssi);

                pos += sr->isr_len;
                rest -= sr->isr_len;
				
    }	
	update_bss_info(vap->vif_ctx, NULL, NULL, 0, 0, 0, 0, 1,0);
	
	kfree(scan_buf);
}



void
ieee80211_notify_replay_failure(struct ieee80211vap *vap,
	const struct ieee80211_frame *wh, const struct ieee80211_key *k,
	u_int64_t rsc, int tid)
{
	struct ifnet *ifp = vap->iv_ifp;

	IEEE80211_NOTE_MAC(vap, IEEE80211_MSG_CRYPTO, wh->i_addr2,
	    "%s replay detected tid %d <rsc %llu, csc %llu, keyix %u rxkeyix %u>",
	    k->wk_cipher->ic_name, tid, rsc,
	    k->wk_keyrsc[tid],
	    k->wk_keyix, k->wk_rxkeyix);

	if (ifp != NULL) {		/* NB: for cipher test modules */
		struct ieee80211_replay_event iev;

		IEEE80211_ADDR_COPY(iev.iev_dst, wh->i_addr1);
		IEEE80211_ADDR_COPY(iev.iev_src, wh->i_addr2);
		iev.iev_cipher = k->wk_cipher->ic_cipher;
		if (k->wk_rxkeyix != IEEE80211_KEYIX_NONE)
			iev.iev_keyix = k->wk_rxkeyix;
		else
			iev.iev_keyix = k->wk_keyix;
		iev.iev_keyrsc = k->wk_keyrsc[tid];
		iev.iev_rsc = rsc;
		//CURVNET_SET(ifp->if_vnet);
		route_ieee80211msg(ifp, RTM_IEEE80211_REPLAY, &iev, sizeof(iev));
		//CURVNET_RESTORE();
	}
}

void
ieee80211_notify_michael_failure(struct ieee80211vap *vap,
	const struct ieee80211_frame *wh, u_int keyix)
{
    struct ifnet *ifp = vap->iv_ifp;

    IEEE80211_NOTE_MAC(vap, IEEE80211_MSG_CRYPTO, wh->i_addr2,
            "michael MIC verification failed <keyix %u>", keyix);
    //vap->iv_stats.is_rx_tkipmic++;

    if (ifp != NULL) {		/* NB: for cipher test modules */
        struct ieee80211_michael_event iev;

        IEEE80211_ADDR_COPY(iev.iev_dst, wh->i_addr1);
        IEEE80211_ADDR_COPY(iev.iev_src, wh->i_addr2);
        iev.iev_cipher = IEEE80211_CIPHER_TKIP;
        iev.iev_keyix = keyix;
        //CURVNET_SET(ifp->if_vnet);
        route_ieee80211msg(ifp, RTM_IEEE80211_MICHAEL, &iev, sizeof(iev));
        wuqi_mic_fail_notify(vap->vif_ctx,(char *)wh->i_addr2,keyix);
        //CURVNET_RESTORE();
    }
}


//test command--------------------------------------------
struct ieee80211vap *wlan0_vap = NULL;
struct ieee80211vap *wlan1_vap = NULL;
extern struct ieee80211vap *hawk_vap0;
struct ieee80211com *net80211_ic;

extern int ieee80211_scanreq(struct ieee80211vap *, struct ieee80211_scan_req *);

void iffup_test_cmd(void)
{
	wlan0_vap->iv_ifp->if_flags |= IFF_UP;

	ieee80211_ioctl(wlan0_vap->iv_ifp, SIOCSIFFLAGS, NULL);
}

#if 0
void scan_req_test_cmd(uint32_t once, char *macStr)
{
    struct ieee80211_scan_req sr;
    struct ifnet *ifp = hawk_vap0->iv_ifp;
    int i;
    u_int8_t mac[6], tmp[3] = {0};

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

    if(macStr && iot_strlen(macStr) == 17){
        for(i = 0; i < 6; i++){
            os_mem_cpy(tmp, macStr+(i*3), 2);
            mac[i] = iot_strtol((char *)tmp, NULL, 16);
        }
        ifp->if_directedaddr = mac;
    }

    ieee80211_scanreq(hawk_vap0, &sr);
    os_delay(3000); //after 3sec, reset ifp->if_direcatedaddr

    if(ifp->if_directedaddr)
        ifp->if_directedaddr = NULL;
}
#endif


void vap_create_test_cmd(enum ieee80211_opmode opmode)
{
    struct ieee80211com *ic;
    char name[IFNAMSIZ] = {'h', 'a', 'w', 'k'};
    const uint8_t bssid[IEEE80211_ADDR_LEN] = {00, 01, 02, 03, 04, 05};
    uint8_t mac0[IEEE80211_ADDR_LEN] = {0x0, 'h', 'a', 'w', 'k', 'A'};
    
    if (opmode == IEEE80211_M_HOSTAP)
        opmode = IEEE80211_M_HOSTAP;
    else
        opmode = IEEE80211_M_STA;

    iot_printf("driver_vap_create %s\n", (opmode == IEEE80211_M_STA)?"STA":"AP");
    if (opmode == IEEE80211_M_STA)
    {
        mac0[IEEE80211_ADDR_LEN-1] = 'S';
	}
	  
    ic = net80211_ic;

	iot_printf("driver_vap_create %p %p\n", wlan0_vap, wlan1_vap);

	  if ((ic->ic_vap_create) != NULL)
	  {
	  	  if (wlan0_vap == NULL)
	  	  {	  	      
	  	      wlan0_vap = ic->ic_vap_create(ic, name, 0,
                opmode,
    		    0,
    		    bssid,
    		    mac0);
	  	  }
	  	  else if (wlan1_vap == NULL)
	  	  {	  	      
	  	      wlan1_vap = ic->ic_vap_create(ic, name, 1,
                opmode,
    		    0,
    		    bssid,
    		    mac0);
	  	  }	  
	  	  else
	  	      IOT_ASSERT(0);

	  }
	  else
	  {
	      iot_printf("driver_vap_create fail : ic->ic_vap_create = NULL\n");
      }
    

}

static const char hawkname[] = "hawk";
struct ieee80211vap *hawk_vap0 = NULL; //Support up to 2 VAP only
struct ieee80211vap *hawk_vap1 = NULL; //Support up to 2 VAP only

uint8_t arp_req_eth_frame[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 'h', 
    'a',  'w',  'k',  'Z',  0x08, 0x06, 0x00, 0x01,
    0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 'h',
    'a',  'w',  'k',  'z',  0xc0, 0xa8, 0x01, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8,
    0x01, 0x01
};

uint8_t test_eth_frame[] = {
    0x00, 0x00, 0x11, 0x22, 0x33, 0x33, 0x00, 'h', 
    'a',  'w',  'k',  'Z',  0x08, 0x06, 0x00, 0x01,
    0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 'T',  'x',
    'T',  'e',  's',  't',  0xc0, 0xa8, 0x01, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8,
    0x01, 0x01
};

#ifdef CERT_TEST //Test case 5.2.14 Multicast
uint8_t test_ip_frame[] = {
    0x01, 0x00, 0x5e, 0x00, 0x00, 0x05, 0x00, 'h',
    'a',  'w',  'k',  'Z',  0x08, 0x00, 0x45, 0xA0,
    0x03, 0x4E, 0x09, 0x7c, 0x00, 0x00, 0x32, 0x06,
    0xCE, 0xF9, 0x48, 0x15, 0x5B, 0x1D, 0xe0, 0x00,
    0x00, 0x05
};

#else //For test cases 5.2.31 Test ACM Bit Conformance, this packet will use VI txq
      //               5.2.37 ADDBA REQ use TID 5
uint8_t test_ip_frame[] = {
    0x00, 0x11, 0x22, 0x33, 0x33, 0x33, 0x00, 'h', 
    'a',  'w',  'k',  'Z',  0x08, 0x00, 0x45, 0xA0,
    0x03, 0x4E, 0x09, 0x7c, 0x00, 0x00, 0x32, 0x06,
    0xCE, 0xF9, 0x48, 0x15, 0x5B, 0x1D, 0x0A, 0x00,
    0x00, 0x03
};
#endif

FAST_ATTR void hawk_dma_done(void* dst, uint32_t length, uint32_t checksum, void *arg)
{
    struct mbuf *m = (struct mbuf *)arg;

    m->m_payload_cksum = ~(inet_htons(checksum));
    /* content copy on this mbuf is done */
    m->m_dma &= ~M_DMA_PENDING;
}

#ifdef RISCV
#define ETHERTYPE_IP    0x0800
#define ETHERTYPE_IPV6  0x86DD
#define PROTO_TCP       0x6
#define PROTO_UDP       0x11
#define PROTO_ICMP      0x1

#define WMM_THRESHOLD   4
uint32_t wmm_pending_cnt[4];        /* pending count of each AC */
uint32_t wmm_pending_cnt_max[4];    /* for debug only */

int wmm_classify(uint8_t *hdr)
{
    const struct ether_header *eh = NULL;
    int ac = WME_AC_BE;

    eh = (struct ether_header *)hdr;

    if (eh && eh->ether_type == htons(ETHERTYPE_IP)) {
        uint8_t tos;
        /*
         * IP frame, map the DSCP bits from the TOS field.
         */
        tos = *((uint8_t*)(hdr + sizeof(struct ether_header) + 1));
        tos >>= 3;		/* NB: ECN + low 3 bits of DSCP */
        ac = TID_TO_WME_AC(tos);
    } else {
        if (eh && eh->ether_type == htons(ETHERTYPE_IPV6)) {
            uint8_t tos;
            /*
             * IPv6 frame, map the DSCP bits from the traffic class field.
             */
            tos = *((uint8_t*)(hdr + sizeof(struct ether_header)));
            tos = (tos >> 1) & 0x7;
            ac = TID_TO_WME_AC(tos);
        } else {
            ac = WME_AC_BE;
        }
    }

	return ac;
}

void wmm_update_buf_status(struct ieee80211_node *ni, void *arg, int status)
{
    int ac = (int)arg;
    osal_atomic_dec((struct osal_atomic_t *)&wmm_pending_cnt[ac]);
}

int is_wmm_congested(struct ieee80211vap *vap, int ac)
{
    if (is_wmm_hi_ac(vap, ac)) {
        return 0;
    }
    if (wmm_pending_cnt[ac] > WMM_THRESHOLD) {
        return 1;
    }

    return 0;
}

void proto_frame_parse(uint8_t *hdr, int *is_udp, int *is_tcp, int *ip_header_len)
{
    const struct ether_header *eh = NULL;
    eh = (struct ether_header *)hdr;

    if (eh && eh->ether_type == htons(ETHERTYPE_IP)) {
        uint8_t *ip = hdr + sizeof(struct ether_header);
        uint8_t *proto = ip + 9;
        if (*proto == PROTO_UDP) {
            *is_udp = 1;
        } else if (*proto == PROTO_TCP) {
            *is_tcp = 1;
        }
        *ip_header_len = (*ip & 0xf) * 4;
    }
}

uint16_t proto_header_cksum(uint8_t *ip_hdr, uint8_t *payload, int len)
{
    uint32_t acc = 0;

    uint16_t *src_ip, *dst_ip, addr;
    uint16_t proto, hdr_len, tot_len, payload_len;
    uint16_t *data_payload;

    src_ip = (uint16_t *)(ip_hdr + 12);
    dst_ip = (uint16_t *)(ip_hdr + 16);

    proto = inet_htons(ip_hdr[9]);
    hdr_len = (*ip_hdr & 0xf) * 4;
    tot_len = inet_htons((*(ip_hdr + 2) << 8 | *(ip_hdr + 3)) - hdr_len);

    /* gen pseudo header checksum */
    addr = (*src_ip);
    acc += (addr & 0xffffUL);
    addr = *(src_ip + 1);
    acc += (addr & 0xffffUL);
    addr = (*dst_ip);
    acc += (addr & 0xffffUL);
    addr = *(dst_ip + 1);
    acc += (addr & 0xffffUL);
    acc += proto;
    acc += tot_len;

    data_payload = (uint16_t *)payload;
    payload_len = len;
    while (payload_len > 1) {
        acc += (*data_payload);
        data_payload++;
        payload_len -= 2;
    }
    if (payload_len) {
        acc += (payload[len - 1]);
    }
     /* add deferred carry bits */
    acc = (acc >> 16) + (acc & 0x0000ffffUL);
    if ((acc & 0xffff0000UL) != 0) {
        acc = (acc >> 16) + (acc & 0x0000ffffUL);
    }

    return acc;
}

/* tx function for lwip */
void wifi_send(int idx, uint8_t *hdr, int hdr_len, uint8_t *data, int data_len)
{
    struct ieee80211vap *vap = NULL;
    struct mbuf *m;
    uint8_t ret;
    int ac = -1, is_udp = 0, is_tcp = 0, ip_header_len = 0;

    if (idx == 1) {
        vap = hawk_vap0;
    } else if (idx == 2) {
        vap = hawk_vap1;
    } else {
        IOT_ASSERT(0);
    }

    if (vap == NULL) {
        return;
    }

    /* retrieve fields for wmm and hw cksum offload */
    if ((uint32_t)(hdr_len + data_len) > (sizeof(struct ether_header) + 34)) {
        proto_frame_parse(hdr, &is_udp, &is_tcp, &ip_header_len);
    }

    /* check if wmm is running. If wmm is running and queued frames exceed
     * the threshold, wait until queued frame consumed.
     */
    if (is_wmm_running(vap)) {
        ac = wmm_classify(hdr);
        if (is_wmm_congested(vap, ac) && is_udp) {
            /* drop this frame if wmm congested and it's udp */
            return;
        }
    }

    m = m_gethdr(0, MT_DATA);
    while (m == NULL) {
        os_delay(5);
        m = m_gethdr(0, MT_DATA);
    }

    if (ac >= 0) {
        osal_atomic_inc((struct osal_atomic_t *)&wmm_pending_cnt[ac]);
        if (wmm_pending_cnt[ac] > wmm_pending_cnt_max[ac]) {
            wmm_pending_cnt_max[ac] = wmm_pending_cnt[ac];
        }
        ieee80211_add_callback(m, NULL, wmm_update_buf_status, (void *)ac);
    }

    /* reserve headroom */
    m->m_data = m->m_data + (vap->iv_ic->ic_headroom + sizeof(struct ieee80211_qosframe));

    /* copy header by cpu, this is needed for 80211 header
     * encap; pkt content is copied by dma and can be completed
     * before assigning this pkt to hw
     */
    if (hdr_len && hdr) {
        os_mem_cpy(m->m_data, hdr, hdr_len);
    }

#ifdef HW_PLATFORM_FPGA
    /* hw tcp/udp checksum offload - pseudo header cksum is calculate by sw,
     * data payload is calculated by hw swdma.
     */
    if (is_tcp || is_udp) {
        uint8_t *payload;
        int payload_offset = 0;

        /* adjust hdr and data since their length varies from lwip.
         * if payload_offset > 0, some tcp/udp payload is in hdr
         * otherwise, adjust data to contains tcp/udp payload only
         */
        payload_offset = hdr_len - sizeof(struct ether_header) - ip_header_len;
        if (payload_offset > 0) {
            payload = hdr + (sizeof(struct ether_header) + ip_header_len);
            m->m_header_cksum = proto_header_cksum(hdr + sizeof(struct ether_header),
                                                   payload, payload_offset);
        } else {
            os_mem_cpy(m->m_data, hdr, sizeof(struct ether_header) + ip_header_len);
            m->m_header_cksum = proto_header_cksum(hdr + sizeof(struct ether_header),
                                                   NULL, 0);
            data -= payload_offset;
            data_len += payload_offset;
            hdr_len -= payload_offset;
        }

        /* address for checksum */
        if (is_tcp) {
            m->m_cksum_addr = (uint32_t)(m->m_data + sizeof(struct ether_header) + ip_header_len + 16);
        } else {
            m->m_cksum_addr = (uint32_t)(m->m_data + sizeof(struct ether_header) + ip_header_len + 6);
        }
        m->m_dma |= M_DMA_CKSUM;
    }
#endif

    m->m_dma |= M_DMA_PENDING;
    ret = iot_dma_memcpy(m->m_data + hdr_len, data,
                         data_len, hawk_dma_done,
                         (void *)m, 1);

    if (ret != RET_OK) {
        IOT_ASSERT(0);
    }
    m->m_len = hdr_len + data_len;

    ieee80211_vap_xmitpkt(vap, m);
}
#endif

void wifi_free_mbuf(void *m)
{
    IOT_ASSERT(m);
    m_freem((struct mbuf *)m);
}

struct ieee80211_node *tx_find_vap_node(struct ieee80211vap *vap, uint8_t *mac)
{
    struct ieee80211_node_table *nt;
    struct ieee80211_node *ni;

    if (vap == NULL) {
        return NULL;
    }

    nt = &vap->iv_ic->ic_sta;
    ni = ieee80211_find_vap_node(nt, vap, mac);
    if (ni) {
        ieee80211_node_decref(ni);
    }

    return ni;
}

extern void wq_set_aggr_cnt(int en , int ampdu_cnt);
void ampdu_set_cmd(int en, int ampdu_cnt)
{
#define HW_AMPDU_MAX_SUBFRAME_NUM 16
    struct ieee80211_node *ni = hawk_vap1->iv_bss;

    if (ampdu_cnt > HW_AMPDU_MAX_SUBFRAME_NUM) {
        iot_printf("ampdu count exceed max value (16)\n");
        goto print_err;
    }

    if(ni){
        iot_printf("ampdu_set : %d, ampdu_cnt : %d\n", en, ampdu_cnt);
        wq_set_aggr_cnt(en, ampdu_cnt);
        return;
    } else {
        iot_printf("sta not connected\n");
    }

print_err:
    iot_printf("ampdu_set_cmd failed\n");
    return;
}

#if 0
void tx_test_cmd(int argc, char **argv)
{
    struct mbuf *m;
    struct ieee80211vap *vap = NULL;
    int count = 1;
    int delay_ms = 0;
    int i;
    uint8_t ret;

    if (argc >= 3)
    {
        count = iot_atoi(argv[2]);
        if (argc >= 4)
        {
            delay_ms = iot_atoi(argv[3]);
        }
    }
 
    if (argc >= 2)
    {
        int value=iot_strtol(argv[1],NULL,16);
        test_eth_frame[4] = value>>8;
        test_eth_frame[5] = value&0xFF;
    }

    if(tx_find_vap_node(hawk_vap0, test_eth_frame)) {
        vap = hawk_vap0;
    } else if (tx_find_vap_node(hawk_vap1, test_eth_frame)) {
        vap = hawk_vap1;
    } else {
        iot_printf("Tx test failed: cannot find node\n");
        return;
    }

    for (i=0; i<count; i++)
    {
#ifdef RISCV
        char ch;
        extern int uart_e_getc(int port);
        ch=uart_e_getc(0);
        if( (ch=='x') || (ch=='X')){
            iot_printf("tx_test:receive %c, break at %d/%d\n",ch,i,count);
            break;
        }
#endif

        if (delay_ms > 0)
            os_delay(delay_ms);
        
        if((i%1000)==0){
            iot_printf("tx_test:%d/%d, interval:%d ms\n",i,count,delay_ms);
#ifdef RISCV
            extern void os_dump_taskinfo();
            os_dump_taskinfo();
#endif
        }

        // 1. allocate mbuf
        m = m_gethdr(0, MT_DATA);
        while (m==NULL) {
            os_delay(5);
            m = m_gethdr(0, MT_DATA);
        }
        /* reserve headroom */
        m->m_data = m->m_data + (vap->iv_ic->ic_headroom + sizeof(struct ieee80211_qosframe));

        //2. copy data to mbuf
#ifdef RISCV
        /* copy eth header by cpu, this is needed for 80211 header
         * encap; pkt content is copied by dma and can be completed
         * before assigning this pkt to hw
         */
        os_mem_cpy(m->m_data, test_eth_frame, ETH_HLEN);
        m->m_dma |= M_DMA_PENDING;
        ret = iot_dma_memcpy(m->m_data + ETH_HLEN, test_eth_frame + ETH_HLEN,
                             sizeof(test_eth_frame) - ETH_HLEN, hawk_dma_done,
                             (void *)m, 1);
        if (ret != RET_OK) {
            IOT_ASSERT(0);
        }
#else
        os_mem_cpy(m->m_data, test_eth_frame, sizeof(test_eth_frame));
#endif
        os_mem_cpy(m->m_data+6, IF_LLADDR(vap->iv_ifp), 6);
        m->m_len = sizeof(test_eth_frame);

        // 3. call wproto tx handler
        ieee80211_vap_xmitpkt(vap, m);
    }

    return;
}


#if defined(CERT_TEST) || !defined(RISCV)
uint8_t fragment_test_pkt1[]={
0x88,0x06 ,0x30,0x00 ,
0x00,0x11 ,0x22,0x33 ,0x4c,0x2b , //Receiver address
0x00,0x11 ,0x22,0x33 ,0x55,0x55,  //Transmitter address
0x00,0x11 ,0x22,0x33 ,0x55,0x55 , //Source address
0x90,0x13 ,0x80,0x00 ,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa, //1
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x27,0x7d ,0x40,0x00 ,0x40,0x06,
0x90,0xbe ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfc ,0xb2,0x86,
0x31,0x14 ,0xe2,0xae ,0xbe,0xd4 ,0x80,0x10 ,0x59,0xfc ,0x12,0xb4 ,0x00,0x00 ,0x01,0x01,
0x08,0x0a ,0xfa,0x50 ,0x64,0x09 ,0x5a,0x98 ,0x38,0x25 ,0x59,0x68 ,0x40,0x4d ,0x7f,0xa5,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00,
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x58,0x0f ,0x40,0x00 ,0x40,0x06 ,0x60,0x2c ,0xc0,0xa8,
0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfe ,0x72,0x85 ,0x46,0x2f ,0x27,0xa5,
0xd9,0xee ,0x80,0x10 ,0x4a,0x8c ,0xec,0xf7 ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50,
0x64,0x09 ,0x5a,0x98 ,0x38,0x25 ,0xfd,0x22 ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b, //10
0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00,
0x00,0x34 ,0x06,0x59 ,0x40,0x00 ,0x40,0x06 ,0xb1,0xe2 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8,
0x00,0xcd ,0x14,0x51 ,0xfc,0xfd ,0x08,0x71 ,0x13,0x6d ,0xda,0x4e ,0x8a,0x92 ,0x80,0x10,
0x5f,0xf8 ,0x11,0x70 ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x09 ,0x5a,0x98,
0x37,0xcb ,0xfb,0xd3 ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b,
0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x27,0x7e,
0x40,0x00 ,0x40,0x06 ,0x90,0xbd ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51,
0xfc,0xfc ,0xb2,0x86 ,0x31,0x14 ,0xe2,0xae ,0xc4,0x7c ,0x80,0x10 ,0x5a,0x12 ,0x0c,0xf6,
0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x09 ,0x5a,0x98 ,0x38,0x25 ,0x57,0xdc,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa, //20
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x58,0x10 ,0x40,0x00 ,0x40,0x06,
0x60,0x2b ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfe ,0x72,0x85,
0x46,0x2f ,0x27,0xa5 ,0xdf,0x96 ,0x80,0x10 ,0x4a,0xa2 ,0xe7,0x38 ,0x00,0x00 ,0x01,0x01,
0x08,0x0a ,0xfa,0x50 ,0x64,0x0a ,0x5a,0x98 ,0x38,0x25 ,0xf5,0xe9 ,0x40,0x4d ,0x7f,0xa5,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00,
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0xa8,0x64 ,0x40,0x00 ,0x40,0x06 ,0x0f,0xd7 ,0xc0,0xa8,
0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xff ,0xa0,0x7b ,0xb6,0xec ,0xd8,0xf9,
0xcc,0xab ,0x80,0x10 ,0x59,0x30 ,0x9b,0xa9 ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50,
0x64,0x0a ,0x5a,0x98 ,0x38,0x08 ,0xd5,0x3f ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b,
0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00, //30
0x00,0x34 ,0x9b,0xe1 ,0x40,0x00 ,0x40,0x06 ,0x1c,0x5a ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8,
0x00,0xcd ,0x14,0x51};

uint8_t fragment_test_pkt2[]={
0x88,0x06 ,0x30,0x00 ,
0x00,0x11 ,0x22,0x33 ,0x4c,0x2b , //Receiver address
0x00,0x11 ,0x22,0x33 ,0x55,0x55,  //Transmitter address
0x00,0x11 ,0x22,0x33 ,0x55,0x55 , //Source address
0x91,0x13 ,0x80,0x00 ,
0xfd,0x00 ,0x41,0x4c ,0x8a,0x5b ,0xa3,0xc7 ,0xb3,0xb2 ,0x80,0x10, 0x44,0x38 ,0x8a,0x6f , //1
0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x0a ,0x5a,0x98, 0x38,0x25 ,0x73,0x36 ,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b, 0x00,0x3e ,0xaa,0xaa ,
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x27,0x7f, 0x40,0x00 ,0x40,0x06 ,
0x90,0xbc ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51, 0xfc,0xfc ,0xb2,0x86 ,
0x31,0x14 ,0xe2,0xae ,0xca,0x24 ,0x80,0x10 ,0x5a,0x29 ,0x07,0x36, 0x00,0x00 ,0x01,0x01 ,
0x08,0x0a ,0xfa,0x50 ,0x64,0x0a ,0x5a,0x98 ,0x38,0x25 ,0x4e,0x08, 0x40,0x4d ,0x7f,0xa5 ,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa, 0x03,0x00 ,0x00,0x00 ,
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x58,0x11 ,0x40,0x00 ,0x40,0x06, 0x60,0x2a ,0xc0,0xa8 ,
0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfe ,0x72,0x85, 0x46,0x2f ,0x27,0xa5 , //10
0xe5,0x3e ,0x80,0x10 ,0x4a,0xb9 ,0xe1,0x79 ,0x00,0x00 ,0x01,0x01, 0x08,0x0a ,0xfa,0x50 ,
0x64,0x0a ,0x5a,0x98 ,0x38,0x25 ,0x28,0xb0 ,0x40,0x4d ,0x7f,0xa5, 0x4c,0x2b ,0x50,0x7b ,
0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00, 0x08,0x00 ,0x45,0x00 ,
0x00,0x34 ,0x06,0x5a ,0x40,0x00 ,0x40,0x06 ,0xb1,0xe1 ,0xc0,0xa8, 0x00,0x6b ,0xc0,0xa8 ,
0x00,0xcd ,0x14,0x51 ,0xfc,0xfd ,0x08,0x71 ,0x13,0x6d ,0xda,0x4e, 0x95,0xe2 ,0x80,0x10 ,
0x5f,0xf8 ,0x06,0x1f ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50, 0x64,0x0a ,0x5a,0x98 ,
0x37,0xcb ,0x77,0x6b, 0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b, 0x9d,0xca ,0x06,0x1b ,
0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00, 0x00,0x34 ,0x27,0x80 ,
0x40,0x00 ,0x40,0x06 ,0x90,0xbb ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8, 0x00,0xcd ,0x14,0x51 ,
0xfc,0xfc ,0xb2,0x86 ,0x31,0x14 ,0xe2,0xae ,0xcf,0xcc ,0x80,0x10, 0x5a,0x3f ,0x01,0x6e , //20
0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x0b ,0x5a,0x98, 0x38,0x2e ,0x98,0x15 ,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b, 0x00,0x3e ,0xaa,0xaa ,
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x58,0x12, 0x40,0x00 ,0x40,0x06 ,
0x60,0x29 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51, 0xfc,0xfe ,0x72,0x85 ,
0x46,0x2f ,0x27,0xa5 ,0xea,0xe6 ,0x80,0x10 ,0x4a,0xcf ,0xdb,0xba, 0x00,0x00 ,0x01,0x01 ,
0x08,0x0a ,0xfa,0x50 ,0x64,0x0b ,0x5a,0x98 ,0x38,0x25 ,0x16,0x87, 0x40,0x4d ,0x7f,0xa5 ,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa, 0x03,0x00 ,0x00,0x00 ,
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0xa8,0x65 ,0x40,0x00 ,0x40,0x06, 0x0f,0xd6 ,0xc0,0xa8 ,
0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xff ,0xa0,0x7b, 0xb6,0xec ,0xd8,0xf9 ,
0xd7,0xfb ,0x80,0x10 ,0x59,0x30 ,0x90,0x57 ,0x00,0x00 ,0x01,0x01, 0x08,0x0a ,0xfa,0x50 , //30
0x64,0x0b ,0x5a,0x98, 0x38,0x09 ,0xf4,0x3a ,0x40,0x4d ,0x7f,0xa5, 0x4c,0x2b ,0x50,0x7b ,
0x9d,0xca ,0x06,0x1b};

uint8_t fragment_test_pkt3[]={
0x88,0x06 ,0x30,0x00 ,
0x00,0x11 ,0x22,0x33 ,0x4c,0x2b , //Receiver address
0x00,0x11 ,0x22,0x33 ,0x55,0x55,  //Transmitter address
0x00,0x11 ,0x22,0x33 ,0x55,0x55 , //Source address
0x92,0x13 ,0x80,0x00 ,
0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00, 0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x9b,0xe2 , //1
0x40,0x00 ,0x40,0x06 ,0x1c,0x59 ,0xc0,0xa8, 0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,
0xfd,0x00 ,0x41,0x4c ,0x8a,0x5b ,0xa3,0xc7, 0xbf,0x02 ,0x80,0x10 ,0x44,0x38 ,0x7f,0x1e ,
0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50, 0x64,0x0b ,0x5a,0x98 ,0x38,0x25 ,0xe6,0x28 ,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b, 0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00, 0x00,0x34 ,0x27,0x81 ,0x40,0x00 ,0x40,0x06 ,
0x90,0xba ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8, 0x00,0xcd ,0x14,0x51 ,0xfc,0xfc ,0xb2,0x86 ,
0x31,0x14 ,0xe2,0xae ,0xd5,0x74 ,0x80,0x10, 0x5a,0x56 ,0xfb,0xae ,0x00,0x00 ,0x01,0x01 ,
0x08,0x0a ,0xfa,0x50 ,0x64,0x0b ,0x5a,0x98, 0x38,0x2e ,0xa8,0x34 ,0x40,0x4d ,0x7f,0xa5 ,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b, 0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 , //10
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x58,0x13, 0x40,0x00 ,0x40,0x06 ,0x60,0x28 ,0xc0,0xa8 ,
0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51, 0xfc,0xfe ,0x72,0x85 ,0x46,0x2f ,0x27,0xa5 ,
0xf0,0x8e ,0x80,0x10 ,0x4a,0xe6 ,0xd5,0xfb, 0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,
0x64,0x0b ,0x5a,0x98 ,0x38,0x25 ,0x55,0x38, 0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,
0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa, 0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,
0x00,0x34 ,0x06,0x5b ,0x40,0x00 ,0x40,0x06, 0xb1,0xe0 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,
0x00,0xcd ,0x14,0x51 ,0xfc,0xfd ,0x08,0x71, 0x13,0x6d ,0xda,0x4e ,0xa1,0x32 ,0x80,0x10 ,
0x5f,0xf8 ,0xfa,0xcc ,0x00,0x00 ,0x01,0x01, 0x08,0x0a ,0xfa,0x50 ,0x64,0x0c ,0x5a,0x98 ,
0x37,0xcb ,0xfa,0x7f ,0x40,0x4d ,0x7f,0xa5, 0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,
0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00, 0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x27,0x82 , //20
0x40,0x00 ,0x40,0x06 ,0x90,0xb9 ,0xc0,0xa8, 0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,
0xfc,0xfc ,0xb2,0x86 ,0x31,0x14 ,0xe2,0xae, 0xdb,0x1c ,0x80,0x10 ,0x5a,0x6d ,0xf5,0xed ,
0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50, 0x64,0x0c ,0x5a,0x98 ,0x38,0x2f ,0xce,0x37 ,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b, 0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00, 0x00,0x34 ,0x58,0x14 ,0x40,0x00 ,0x40,0x06 ,
0x60,0x27 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8, 0x00,0xcd ,0x14,0x51 ,0xfc,0xfe ,0x72,0x85 ,
0x46,0x2f ,0x27,0xa5 ,0xf6,0x36 ,0x80,0x10, 0x4a,0xfd ,0xd0,0x3a ,0x00,0x00 ,0x01,0x01 ,
0x08,0x0a ,0xfa,0x50 ,0x64,0x0c ,0x5a,0x98, 0x38,0x26 ,0x13,0x1f ,0x40,0x4d ,0x7f,0xa5 ,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b, 0x00,0x3c ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0xa8,0x66, 0x40,0x00 ,0x40,0x06 ,0x0f,0xd5 ,0xc0,0xa8 , //30
0x00,0x6b ,0xc0,0xa8, 0x00,0xcd ,0x14,0x51, 0xfc,0xff ,0xa0,0x7b ,0xb6,0xec ,0xd8,0xf9 , 
0xe3,0x4b ,0x80,0x10}; 

uint8_t fragment_test_pkt4[]={
0x88,0x02 ,0x30,0x00 ,
0x00,0x11 ,0x22,0x33 ,0x4c,0x2b , //Receiver address
0x00,0x11 ,0x22,0x33 ,0x55,0x55,  //Transmitter address
0x00,0x11 ,0x22,0x33 ,0x55,0x55 , //Source address
0x93,0x13 ,0x80,0x00 ,
0x59,0x30 ,0x85,0x06, 0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 , 0x64,0x0c ,0x5a,0x98 ,
0x38,0x09};


void tx_test_fragment_cmd(int argc, char **argv)
{
    struct mbuf *m;
    struct ieee80211_node *ni;
    int value;
    uint8_t node_idx;

    if(argc!=3){
        iot_printf("Usage: tx_test_fragment mac4mac5 node_idx\n");
        return;
    }    

    value=iot_strtol(argv[1],NULL,16);
    fragment_test_pkt2[8]= fragment_test_pkt1[8] = value>>8;
    fragment_test_pkt2[9]= fragment_test_pkt1[9] = value&0xFF;
    fragment_test_pkt4[8]= fragment_test_pkt3[8] = value>>8;
    fragment_test_pkt4[9]= fragment_test_pkt3[9] = value&0xFF;
    node_idx=iot_atoi(argv[2]);
    ni=(struct ieee80211_node *)get_ni_from_idx((struct ieee80211com *)hawk_vap0->iv_ic,node_idx);
    if(ni==NULL)
    {
        iot_printf("ERROR: node_idx %d is NULL\n",node_idx);    
        return;
    }

    m = m_gethdr(0, MT_DATA);
    if (m == NULL) {
        return;
    }
    m->m_data = m->m_data + (hawk_vap0->iv_ic->ic_headroom);

    os_mem_cpy(m->m_data, fragment_test_pkt1, sizeof(fragment_test_pkt1));
    m->m_len = sizeof(fragment_test_pkt1);

    ieee80211_node_incref(ni);
    ieee80211_raw_output(hawk_vap0, ni, m);


    m = m_gethdr(0, MT_DATA);
    if (m == NULL) {
        return;
    }
    m->m_data = m->m_data + (hawk_vap0->iv_ic->ic_headroom);

    os_mem_cpy(m->m_data, fragment_test_pkt2, sizeof(fragment_test_pkt2));
    m->m_len = sizeof(fragment_test_pkt2);

    ieee80211_node_incref(ni);
    ieee80211_raw_output(hawk_vap0, ni, m);


    m = m_gethdr(0, MT_DATA);
    if (m == NULL) {
        return;
    }
    m->m_data = m->m_data + (hawk_vap0->iv_ic->ic_headroom);

    os_mem_cpy(m->m_data, fragment_test_pkt3, sizeof(fragment_test_pkt3));
    m->m_len = sizeof(fragment_test_pkt3);

    ieee80211_node_incref(ni);
    ieee80211_raw_output(hawk_vap0, ni, m);

    m = m_gethdr(0, MT_DATA);
    if (m == NULL) {
        return;
    }
    m->m_data = m->m_data + (hawk_vap0->iv_ic->ic_headroom);

    os_mem_cpy(m->m_data, fragment_test_pkt4, sizeof(fragment_test_pkt4));
    m->m_len = sizeof(fragment_test_pkt4);

    ieee80211_node_incref(ni);
    ieee80211_raw_output(hawk_vap0, ni, m);
    return;
}


uint8_t amsdu_test_pkt[]={
0x88,0x02 ,0x30,0x00 ,
0x00,0x11 ,0x22,0x33 ,0x4c,0x2b , //Receiver address
0x00,0x11 ,0x22,0x33 ,0x55,0x55,  //Transmitter address
0x00,0x11 ,0x22,0x33 ,0x55,0x55 , //Source address
0x90,0x13 ,0x80,0x00 ,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa,
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x27,0x7d ,0x40,0x00 ,0x40,0x06,
0x90,0xbe ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfc ,0xb2,0x86,
0x31,0x14 ,0xe2,0xae ,0xbe,0xd4 ,0x80,0x10 ,0x59,0xfc ,0x12,0xb4 ,0x00,0x00 ,0x01,0x01,
0x08,0x0a ,0xfa,0x50 ,0x64,0x09 ,0x5a,0x98 ,0x38,0x25 ,0x59,0x68 ,0x40,0x4d ,0x7f,0xa5,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00,
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x58,0x0f ,0x40,0x00 ,0x40,0x06 ,0x60,0x2c ,0xc0,0xa8,
0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfe ,0x72,0x85 ,0x46,0x2f ,0x27,0xa5,
0xd9,0xee ,0x80,0x10 ,0x4a,0x8c ,0xec,0xf7 ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50,
0x64,0x09 ,0x5a,0x98 ,0x38,0x25 ,0xfd,0x22 ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b,
0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00,
0x00,0x34 ,0x06,0x59 ,0x40,0x00 ,0x40,0x06 ,0xb1,0xe2 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8,
0x00,0xcd ,0x14,0x51 ,0xfc,0xfd ,0x08,0x71 ,0x13,0x6d ,0xda,0x4e ,0x8a,0x92 ,0x80,0x10,
0x5f,0xf8 ,0x11,0x70 ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x09 ,0x5a,0x98,
0x37,0xcb ,0xfb,0xd3 ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b,
0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x27,0x7e,
0x40,0x00 ,0x40,0x06 ,0x90,0xbd ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51,
0xfc,0xfc ,0xb2,0x86 ,0x31,0x14 ,0xe2,0xae ,0xc4,0x7c ,0x80,0x10 ,0x5a,0x12 ,0x0c,0xf6,
0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x09 ,0x5a,0x98 ,0x38,0x25 ,0x57,0xdc,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa,
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x58,0x10 ,0x40,0x00 ,0x40,0x06,
0x60,0x2b ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfe ,0x72,0x85,
0x46,0x2f ,0x27,0xa5 ,0xdf,0x96 ,0x80,0x10 ,0x4a,0xa2 ,0xe7,0x38 ,0x00,0x00 ,0x01,0x01,
0x08,0x0a ,0xfa,0x50 ,0x64,0x0a ,0x5a,0x98 ,0x38,0x25 ,0xf5,0xe9 ,0x40,0x4d ,0x7f,0xa5,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00,
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0xa8,0x64 ,0x40,0x00 ,0x40,0x06 ,0x0f,0xd7 ,0xc0,0xa8,
0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xff ,0xa0,0x7b ,0xb6,0xec ,0xd8,0xf9,
0xcc,0xab ,0x80,0x10 ,0x59,0x30 ,0x9b,0xa9 ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50,
0x64,0x0a ,0x5a,0x98 ,0x38,0x08 ,0xd5,0x3f ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b,
0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00,
0x00,0x34 ,0x9b,0xe1 ,0x40,0x00 ,0x40,0x06 ,0x1c,0x5a ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8,
0x00,0xcd ,0x14,0x51 ,0xfd,0x00 ,0x41,0x4c ,0x8a,0x5b ,0xa3,0xc7 ,0xb3,0xb2 ,0x80,0x10,
0x44,0x38 ,0x8a,0x6f ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x0a ,0x5a,0x98,
0x38,0x25 ,0x73,0x36 ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b,
0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x27,0x7f,
0x40,0x00 ,0x40,0x06 ,0x90,0xbc ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51,
0xfc,0xfc ,0xb2,0x86 ,0x31,0x14 ,0xe2,0xae ,0xca,0x24 ,0x80,0x10 ,0x5a,0x29 ,0x07,0x36,
0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x0a ,0x5a,0x98 ,0x38,0x25 ,0x4e,0x08,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa,
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x58,0x11 ,0x40,0x00 ,0x40,0x06,
0x60,0x2a ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfe ,0x72,0x85,
0x46,0x2f ,0x27,0xa5 ,0xe5,0x3e ,0x80,0x10 ,0x4a,0xb9 ,0xe1,0x79 ,0x00,0x00 ,0x01,0x01,
0x08,0x0a ,0xfa,0x50 ,0x64,0x0a ,0x5a,0x98 ,0x38,0x25 ,0x28,0xb0 ,0x40,0x4d ,0x7f,0xa5,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00,
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x06,0x5a ,0x40,0x00 ,0x40,0x06 ,0xb1,0xe1 ,0xc0,0xa8,
0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfd ,0x08,0x71 ,0x13,0x6d ,0xda,0x4e,
0x95,0xe2 ,0x80,0x10 ,0x5f,0xf8 ,0x06,0x1f ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50,
0x64,0x0a ,0x5a,0x98 ,0x37,0xcb ,0x77,0x6b ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b,
0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00,
0x00,0x34 ,0x27,0x80 ,0x40,0x00 ,0x40,0x06 ,0x90,0xbb ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8,
0x00,0xcd ,0x14,0x51 ,0xfc,0xfc ,0xb2,0x86 ,0x31,0x14 ,0xe2,0xae ,0xcf,0xcc ,0x80,0x10,
0x5a,0x3f ,0x01,0x6e ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x0b ,0x5a,0x98,
0x38,0x2e ,0x98,0x15 };

#if 0
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b,
0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x58,0x12,
0x40,0x00 ,0x40,0x06 ,0x60,0x29 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51,
0xfc,0xfe ,0x72,0x85 ,0x46,0x2f ,0x27,0xa5 ,0xea,0xe6 ,0x80,0x10 ,0x4a,0xcf ,0xdb,0xba,
0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x0b ,0x5a,0x98 ,0x38,0x25 ,0x16,0x87,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa,
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0xa8,0x65 ,0x40,0x00 ,0x40,0x06,
0x0f,0xd6 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xff ,0xa0,0x7b,
0xb6,0xec ,0xd8,0xf9 ,0xd7,0xfb ,0x80,0x10 ,0x59,0x30 ,0x90,0x57 ,0x00,0x00 ,0x01,0x01,
0x08,0x0a ,0xfa,0x50 ,0x64,0x0b ,0x5a,0x98 ,0x38,0x09 ,0xf4,0x3a ,0x40,0x4d ,0x7f,0xa5,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00,
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x9b,0xe2 ,0x40,0x00 ,0x40,0x06 ,0x1c,0x59 ,0xc0,0xa8,
0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfd,0x00 ,0x41,0x4c ,0x8a,0x5b ,0xa3,0xc7,
0xbf,0x02 ,0x80,0x10 ,0x44,0x38 ,0x7f,0x1e ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50,
0x64,0x0b ,0x5a,0x98 ,0x38,0x25 ,0xe6,0x28 ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b,
0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00,
0x00,0x34 ,0x27,0x81 ,0x40,0x00 ,0x40,0x06 ,0x90,0xba ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8,
0x00,0xcd ,0x14,0x51 ,0xfc,0xfc ,0xb2,0x86 ,0x31,0x14 ,0xe2,0xae ,0xd5,0x74 ,0x80,0x10,
0x5a,0x56 ,0xfb,0xae ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x0b ,0x5a,0x98,
0x38,0x2e ,0xa8,0x34 ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b,
0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x58,0x13,
0x40,0x00 ,0x40,0x06 ,0x60,0x28 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51,
0xfc,0xfe ,0x72,0x85 ,0x46,0x2f ,0x27,0xa5 ,0xf0,0x8e ,0x80,0x10 ,0x4a,0xe6 ,0xd5,0xfb,
0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x0b ,0x5a,0x98 ,0x38,0x25 ,0x55,0x38,
0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa,
0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x06,0x5b ,0x40,0x00 ,0x40,0x06,
0xb1,0xe0 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfd ,0x08,0x71,
0x13,0x6d ,0xda,0x4e ,0xa1,0x32 ,0x80,0x10 ,0x5f,0xf8 ,0xfa,0xcc ,0x00,0x00 ,0x01,0x01,
0x08,0x0a ,0xfa,0x50 ,0x64,0x0c ,0x5a,0x98 ,0x37,0xcb ,0xfa,0x7f ,0x40,0x4d ,0x7f,0xa5,
0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00,
0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0x27,0x82 ,0x40,0x00 ,0x40,0x06 ,0x90,0xb9 ,0xc0,0xa8,
0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51 ,0xfc,0xfc ,0xb2,0x86 ,0x31,0x14 ,0xe2,0xae,
0xdb,0x1c ,0x80,0x10 ,0x5a,0x6d ,0xf5,0xed ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50,
0x64,0x0c ,0x5a,0x98 ,0x38,0x2f ,0xce,0x37 ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b,
0x9d,0xca ,0x06,0x1b ,0x00,0x3e ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00,
0x00,0x34 ,0x58,0x14 ,0x40,0x00 ,0x40,0x06 ,0x60,0x27 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8,
0x00,0xcd ,0x14,0x51 ,0xfc,0xfe ,0x72,0x85 ,0x46,0x2f ,0x27,0xa5 ,0xf6,0x36 ,0x80,0x10,
0x4a,0xfd ,0xd0,0x3a ,0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x0c ,0x5a,0x98,
0x38,0x26 ,0x13,0x1f ,0x40,0x4d ,0x7f,0xa5 ,0x4c,0x2b ,0x50,0x7b ,0x9d,0xca ,0x06,0x1b,
0x00,0x3c ,0xaa,0xaa ,0x03,0x00 ,0x00,0x00 ,0x08,0x00 ,0x45,0x00 ,0x00,0x34 ,0xa8,0x66,
0x40,0x00 ,0x40,0x06 ,0x0f,0xd5 ,0xc0,0xa8 ,0x00,0x6b ,0xc0,0xa8 ,0x00,0xcd ,0x14,0x51,
0xfc,0xff ,0xa0,0x7b ,0xb6,0xec ,0xd8,0xf9 ,0xe3,0x4b ,0x80,0x10 ,0x59,0x30 ,0x85,0x06,
0x00,0x00 ,0x01,0x01 ,0x08,0x0a ,0xfa,0x50 ,0x64,0x0c ,0x5a,0x98 ,0x38,0x09};
#endif

void tx_test_amsdu_cmd(int argc, char **argv)
{
    struct mbuf *m;
    struct ieee80211_node *ni;
    int value;
    uint8_t node_idx;

    if(argc!=3){
        iot_printf("Usage: tx_test_amsdu mac4mac5 node_idx\n");
        return;
    }    

    value=iot_strtol(argv[1],NULL,16);
    amsdu_test_pkt[8] = value>>8;
    amsdu_test_pkt[9] = value&0xFF;
    node_idx=iot_atoi(argv[2]);
    ni=(struct ieee80211_node *)get_ni_from_idx((struct ieee80211com *)hawk_vap0->iv_ic,node_idx);
    if(ni==NULL)
    {
        iot_printf("ERROR: node_idx %d is NULL\n",node_idx);    
        return;
    }

    // 1. allocate mbuf
    m = m_gethdr(0, MT_DATA);
    if (m == NULL) {
        return;
    }
    /* reserve headroom */
    m->m_data = m->m_data + (hawk_vap0->iv_ic->ic_headroom);

    //2. copy data to mbuf
    os_mem_cpy(m->m_data, amsdu_test_pkt, sizeof(amsdu_test_pkt));
    m->m_len = sizeof(amsdu_test_pkt);

    // 3. call wproto tx handler
    ieee80211_node_incref(ni);
    ieee80211_raw_output(hawk_vap0, ni, m);

    return;
}
#endif

void node_dump(void)
{
    struct ieee80211_node_table *nt = &hawk_vap0->iv_ic->ic_sta;
    ieee80211_dump_nodes(nt);
}

void power_save_cmd(int enable)
{
    if (enable == 1)
        ieee80211_syncflag(hawk_vap0, IEEE80211_F_PMGTON);
    else
        ieee80211_syncflag(hawk_vap0, -IEEE80211_F_PMGTON);
}

extern uint32_t wq_debug;
void set_dbg_cmd(int argc,char **argv)
{
    int iv_debug_value=0,wq_debug_value=0;

    if(argc!=3)
    {
        iot_printf("Usage: set_dbg IEEE80211_MSG_DBG_SETTING  WQ_DBG_SETTING\n");
        iot_printf("Current setting: hawk_vap0->iv_debug:0x%X, hawk_vap1->iv_debug:0x%X,wq_debug:0x%X",
                hawk_vap0?hawk_vap0->iv_debug:0,hawk_vap1?hawk_vap1->iv_debug:0,wq_debug);
        return;
    }


    iv_debug_value=iot_strtol(argv[1],NULL,16);
    wq_debug_value=iot_strtol(argv[2],NULL,16);

    if(hawk_vap0)
        hawk_vap0->iv_debug=iv_debug_value;
    if(hawk_vap1)
        hawk_vap1->iv_debug=iv_debug_value;
    wq_debug=wq_debug_value;

    iot_printf("Current setting: hawk_vap0->iv_debug:0x%X, hawk_vap1->iv_debug:0x%X,wq_debug:0x%X",
            hawk_vap0?hawk_vap0->iv_debug:0,hawk_vap1?hawk_vap1->iv_debug:0,wq_debug);
}
#endif

#if 0 //test code
void inject_beacon(void)
{
    struct mbuf *m = m_gethdr(0, MT_DATA);
    struct ieee80211_rx_stats rxs;
    uint8_t beacon_frame[] = {0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0x00, 0x14, 0x6c, 0x7e, 0x40, 0x80,
                              0x00, 0x14, 0x6c, 0x7e, 0x40, 0x80, 0xa0, 0x10,
                              0x81, 0x21, 0x82, 0xb3, 0x09, 0x00, 0x00, 0x00,
                              0x64, 0x00, 0x11, 0x04, 0x00, 0x05, 0x74, 0x65,
                              0x64, 0x64, 0x79, 0x01, 0x08, 0x82, 0x84, 0x8b,
                              0x96, 0x0c, 0x18, 0x30, 0x58, 0x03, 0x01, 0x30,
                              0x05, 0x04, 0x00, 0x01, 0x00, 0x00, 0x2a, 0x01,
                              0x00, 0x32, 0x04, 0x12, 0x24, 0x60, 0x6c, 0xdd,
                              0x16, 0x00, 0x50, 0xf2, 0x01, 0x01, 0x00, 0x00,
                              0x50, 0xf2, 0x02, 0x01, 0x00, 0x00, 0x50, 0xf2,
                              0x02, 0x01, 0x00, 0x00, 0x50, 0xf2, 0x02, 0xdd,
                              0x0c, 0x00, 0x03, 0x7f, 0x02, 0x01, 0x01, 0x00,
                              0x00, 0x02, 0xa3, 0x00, 0x00};

    os_mem_cpy(m->m_data, beacon_frame, sizeof(beacon_frame));
    m->m_len = sizeof(beacon_frame);

    //bzero(&rxs, sizeof(rxs));
    os_mem_set(&rxs, 0, sizeof(rxs));

	rxs.c_rssi = 30;
	rxs.c_nf = 20;
	
    ieee80211_add_rx_params(m, &rxs);
    ieee80211_input_mimo_all(net80211_ic, m);

    return;
}
#endif




struct ifnet *hawk_clone_create(struct if_clone *ifc,
                                        int unit,
                                        caddr_t params)
{
    struct ieee80211_clone_params cp;
	struct ieee80211com *ic;
    struct ieee80211vap **vap;

	os_mem_cpy(&cp, params, sizeof(cp));
    iot_printf("hawk: hawk_clone_create unit=%d, opmode=%d, flags=0x%x, mac=%s, bssid=%s\n", 
        unit, cp.icp_opmode, cp.icp_flags, ether_sprintf(cp.icp_macaddr), ether_sprintf(cp.icp_bssid));

	if ((ic = net80211_ic) == NULL) {
	    ic_printf(ic, "%s: net80211_ic NULL\n", __func__);
		return NULL;
	}
		
	if (cp.icp_opmode >= IEEE80211_OPMODE_MAX) {
		ic_printf(ic, "%s: invalid opmode %d\n", __func__,
		    cp.icp_opmode);
		return NULL;
	}
	if ((ic->ic_caps & ieee80211_opcap[cp.icp_opmode]) == 0) {
		ic_printf(ic, "%s mode not supported\n",
		    ieee80211_opmode_name[cp.icp_opmode]);
		return NULL;
	}

    if (hawk_vap0 == NULL)
    {
        vap = &hawk_vap0;
        ic_printf(ic, "%s: create VAP0\n", __func__);
    }
    else if (hawk_vap1 == NULL)
    {
        vap = &hawk_vap1;
        ic_printf(ic, "%s: create VAP1\n", __func__);
    }
    else
    {
        ic_printf(ic, "%s: both VAP are occupied\n", __func__);
		return NULL;
    }
        
        
    *vap = ic->ic_vap_create(ic, hawkname, unit,
                cp.icp_opmode,
    		    cp.icp_flags,
    		    cp.icp_bssid,
    		    cp.icp_flags & IEEE80211_CLONE_MACADDR ? cp.icp_macaddr : ic->ic_macaddr);

    return (*vap)->iv_ifp;
}

void hawk_clone_destroy(struct ifnet *ifp)
{
    extern void wq_stop_tbtt(struct ieee80211vap *vap);
    struct ieee80211vap *vap = ifp->if_softc;
    struct ieee80211com *ic = vap->iv_ic;
    iot_printf("hawk: clone destory %p\n", ifp);

    /* stop tbtt before vap destroy */
    wq_stop_tbtt(vap);

    /* stop keepalive */
 //   iot_printf("stop keepalive\n");
 //   callout_stop_v2(&ic->keepalive_timer);

    if(vap == hawk_vap0 || vap == hawk_vap1)
    {
        if(hawk_vap1 != NULL)
        {
            while(ic->ic_flags & IEEE80211_F_SCAN || hawk_vap1->iv_scandone_flags == 0 )
            {
                   IEEE80211_DPRINTF(vap, IEEE80211_MSG_SCAN, "%s: vap1 also in scan, wait\n", __func__);
                   os_delay(100);
            }
        }

        if (vap == hawk_vap0)
        {
            hawk_vap0 = NULL;
            ic_printf(ic, "%s: destroy VAP0\n", __func__);
        }
        else if (vap == hawk_vap1)
        {
            monitor_tx_flag = 0;
            if(vap->iv_ic->mo_payload)
            {
                kfree(vap->iv_ic->mo_payload);
            }
            hawk_vap1 = NULL;
            ic_printf(ic, "%s: destroy VAP1\n", __func__);
        }
    }
    else
    {
        ic_printf(ic, "%s: invalid VAP\n", __func__);
		return;
    }

    //while(ic->ic_flags & IEEE80211_F_SCAN)
    //{
        //IEEE80211_DPRINTF(vap, IEEE80211_MSG_SCAN, "%s: vap1 also in scan, wait\n", __func__);
        //os_delay(100);
    //}

    osal_semaphore_take(&ic->vap_delete_lock);
    ic->ic_vap_delete(vap);
    osal_semaphore_post(&ic->vap_delete_lock);

    return;
}

#define TIMER_RECORED_CHECK 1
#ifdef TIMER_RECORED_CHECK
#define TIMER_RECORED_NUM 300

typedef struct _timer_record
{
    void *addr;
	char func[100];
    int line_num;
}timer_record_s;
timer_record_s timer_record[TIMER_RECORED_NUM]={{0}};

void add_timer_record(const char *func, int line_num, void* addr)
{
    int idx;

    for(idx=0;idx<TIMER_RECORED_NUM;idx++)
    {
        if(!timer_record[idx].addr && timer_record[idx].line_num==0){
            timer_record[idx].line_num=line_num;
            strcpy(timer_record[idx].func,func);
		    timer_record[idx].addr=addr;
			//iot_printf("add timer: <%s;%d>, addr:0x%p\n",timer_record[idx].func,timer_record[idx].line_num,timer_record[idx].addr);
            break;
        }
    }

    if(idx==TIMER_RECORED_NUM)
        iot_printf("Out of timer_record\n");
}

void del_timer_record(void* addr)
{
    int idx;

    for(idx=0;idx<TIMER_RECORED_NUM;idx++)
    {
        if(timer_record[idx].addr==addr){
			//iot_printf("del timer: <%s;%d>, addr:0x%p\n",timer_record[idx].func,timer_record[idx].line_num,timer_record[idx].addr);
            timer_record[idx].addr=0;
            timer_record[idx].line_num=0;
            memset(timer_record[idx].func,0,sizeof(timer_record[idx].func));
            break;
        }
    }
}

void dump_timer_record()
{
    int idx;

    iot_printf("===== TIMER allocated records start ======\n");
    for(idx=0;idx<TIMER_RECORED_NUM;idx++)
    {
        if(timer_record[idx].addr!=0){
            iot_printf("<%s;%d>, addr:0x%p\n",timer_record[idx].func,timer_record[idx].line_num,timer_record[idx].addr);
        }
    }
    iot_printf("===== TIMER allocated records end ======\n");
}

#else
#define add_timer_record(func,line_num,addr)
#define del_timer_record(ptr)
#define dump_timer_record()
#endif

int callout_pending(struct osal_timer_t *c)
{
    return timer_pending(&c->timer);
}

void timer_work(struct work_struct *w)
{
    struct osal_timer_t* osal_timer = container_of(w, struct osal_timer_t, ws_timer_work);
    //iot_printf("timer_work %p\n", osal_timer);

    osal_timer->func(osal_timer->arg);
    del_timer_record(&osal_timer->timer);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)

void timer_callback(void *arg)
{
    struct osal_timer_t *osal_timer = arg;
    //iot_printf("timer_callback %p\n", osal_timer);

    if (!schedule_work(&osal_timer->ws_timer_work)) {
        iot_printf("timer_callback schedule work failed : %p\n", osal_timer->func);
    }
}

void callout_init_v2(struct osal_timer_t *osal_timer, timeout_t *func, void *arg)
{
    init_timer(&osal_timer->timer);
    osal_timer->timer.function=(void *)timer_callback;
    osal_timer->timer.data=(unsigned long)osal_timer;
    osal_timer->func=(void *)func;
    osal_timer->arg=(void *)arg;

    INIT_WORK(&osal_timer->ws_timer_work, timer_work);
}
#else

void timer_callback(struct timer_list *t)   
{
    struct osal_timer_t *osal_timer = from_timer(osal_timer, t, timer);
    //iot_printf("timer_callback %p\n", osal_timer);

    if (!schedule_work(&osal_timer->ws_timer_work)) {
        iot_printf("timer_callback schedule work failed : %p\n", osal_timer->func);
    }
    return;
}

void callout_init_v2(struct osal_timer_t *osal_timer, timeout_t *func, void *arg)
{
    osal_timer->func=(void *)func;
    osal_timer->arg=(void *)arg;
    timer_setup(&osal_timer->timer, (void *)timer_callback, 0);

    INIT_WORK(&osal_timer->ws_timer_work, timer_work);
}

#endif

void _callout_deinit_dbg(struct callout *timer, const char *function_name, int line_num)
{
    cancel_work_sync(&timer->ws_timer_work);
    del_timer(&timer->timer);
    del_timer_record(&timer->timer);
}

int _callout_reset_v2_dbg(struct callout *c, int tick, const char *function_name, int line_num)
{
    struct osal_timer_t *osal_timer=(struct osal_timer_t *)c;

    mod_timer(&osal_timer->timer, jiffies + tick);
    add_timer_record(function_name,line_num,&osal_timer->timer);
    return 0;
}

int _callout_stop_v2_dbg(struct callout *c, const char *function_name, int line_num)
{
    struct osal_timer_t *osal_timer=(struct osal_timer_t *)c;

    cancel_work_sync(&osal_timer->ws_timer_work);
    del_timer(&c->timer);
    del_timer_record(&c->timer);
    return 0;
}

int _callout_drain_v2_dbg(struct callout *c, const char *function_name, int line_num)
{
    struct osal_timer_t *osal_timer=(struct osal_timer_t *)c;

    cancel_work_sync(&osal_timer->ws_timer_work);
    del_timer(&c->timer);
    del_timer_record(&c->timer);
    return 0;
}

extern void wq_net_if_input(void *vap_ctx_param, struct sk_buff *skb, uint8_t rssi, uint8_t nf);

void net_if_input(struct ieee80211vap *vap, struct mbuf *m)
{
    struct sk_buff *skb;
    int pull_len;

    //take skb and invalid the ptr
    skb = m->skb;
    m->skb = NULL;

    //update skb data and len
    pull_len = m->m_data - skb->data;

    //iot_printf("net_if_input: pull_len:%d, m_len:%d, skblen:%d\n", pull_len, m->m_len,skb->len);

    skb_pull(skb, pull_len);
    skb->len = m->m_len;
#if (WQ_CONFIG_64 == 1)
    skb->tail = (uint64_t)(skb->data - skb->head) + skb->len;
#else
    skb->tail = skb->data + skb->len;
#endif

    if (vap->iv_caps & IEEE80211_C_EXTAP) {
        ieee80211_extap_input((struct ethhdr *)skb->data, vap->iv_myaddr);
    }

    //indicate skb to kernel (not in ISR, use netif_rx_ni())
    //netif_rx_ni(skb);

    // debug ping reply: Determine whether the ping reply is transmitted to the upper layer
    if((m->m_flags_ext & M_IS_PING_REPLY) && (wq_dbg_flag & WQ_DEBUG_PING)) {
        iot_printf("[%s] Ping reply(0x%04x) is transmitted to upper layer!\n", __func__, m->m_ping_seq);
    }

    wq_net_if_input((void*)vap->vif_ctx, skb, m->c_rssi, m->c_nf );

    //free mbuf without skb
    m_freem_wo_skb(m);


    return;
}

extern void wq_mac_put_to_tx_queue(struct mbuf *m);

int net_if_output(void *vap_ptr, struct sk_buff *skb)
{
	struct ieee80211vap *vap = vap_ptr;
	struct mbuf *m;

	if (vap->iv_caps & IEEE80211_C_EXTAP) {
		ieee80211_extap_output((struct ethhdr *)skb->data, vap->iv_myaddr);
	}

	m = _m_gethdr_with_skb(M_NOWAIT, MT_DATA, skb, skb->len);
	
	if (m != NULL)
	{
        // Avoid msg4 being wrongly encrypted.
        if(vap->iv_opmode == IEEE80211_M_STA)
        {
            struct ethhdr *eh = (struct ethhdr *)skb->data;
            if(htons(eh->h_proto) == ETH_P_PAE)
            {
                eapol_8021xhdr_t *eapol = (eapol_8021xhdr_t *)(eh + 1);
                // RSN-CCMP/RSN WPA-CCMP TKIP: 0x030a; WPA-TKIP: 0x0109; WPA-CCMP: 0x010a
                if((eapol->eapol_key_info[0] == 0x03 && eapol->eapol_key_info[1] == 0x0a) || 
                     (eapol->eapol_key_info[0] == 0x01 && (eapol->eapol_key_info[1] == 0x09 || eapol->eapol_key_info[1] == 0x0a)))
                {
                    m->m_flags_ext |= M_NO_ENCRYPT;
                }
            }
        }
	    m->vap = vap;
        wuqi_ping_debug_msg((struct ethhdr *)skb->data, m);

	    wq_mac_put_to_tx_queue(m);
	    //ieee80211_vap_transmit(vap->iv_ifp, m);
	    return 0;
    }
    else
    {
        iot_printf("%s: drop frame, len=%d\n", __func__, skb->len);
        //return -1 to let caller free skb
        return -1;
    }
}

uint8_t frame_becon_monitor[] = { 0x80, 0x00, 0x00, 0x00,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //A1
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //A2
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //A3
0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, //timestamp
0x64, 0x00, //beacon interval
0x21, 0x04, //capability
0x00, 0x03, 'W', 'Q', 'f',//SSID
0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
0x03, 0x01, 0x06, //channel
0x05, 0x04, 0x00, 0x01, 0x00, 0x00, //TIM
0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, //extended support rate
};

// set tx payload(802.11 frame) under monitor mode
void wq_set_monitor_tx_payload(uint8_t *payload, int len)
{
    struct ieee80211vap *vap = NULL;
    struct ieee80211com *ic = NULL;

    if (hawk_vap1)
    {
        vap = hawk_vap1;
        ic = vap->iv_ic;

        if (len <= HAL_RX_DATA_BUF_SIZE)
        {
            if (ic->mo_payload == NULL)
            {
                ic->mo_payload = (uint8_t *)wq_mem_alloc(HAL_RX_DATA_BUF_SIZE, 0);
            }
            if (ic->mo_payload == NULL)
            {
                iot_printf("%s: kmalloc fail\n", __func__);
                return;
            }
            memcpy(ic->mo_payload, payload, len);
            ic->mo_payload_len = len;
        } else {
            iot_printf("%s: maximum payload len: %d\n", __func__, HAL_RX_DATA_BUF_SIZE);
        }
    }

    return;
}

void wq_output_monitor(struct work_struct * work)
{
    struct mbuf *m = NULL;
    struct ieee80211vap *vap = NULL;
    struct ieee80211com *ic = NULL;
    int tx_nonstop = 0;
    int tx_done_count = 0;

    uint8_t pkt_for_phy_test[980]= {0x80, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //A1
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //A2
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //A3
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, //timestamp
    0x64, 0x00, //beacon interval
    0x21, 0x04, //capability
    0x00, 0x03, 'W', 'Q', 'f',//SSID
    0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
    0x03, 0x01, 0x06, //channel
    0x05, 0x04, 0x00, 0x01, 0x00, 0x00, //TIM
    0x32, 0x04, 0x30, 0x48, 0x60, 0x6c,};

    if (hawk_vap1)
    {
        vap = hawk_vap1;
        ic = vap->iv_ic;
        if (ic->tx_count == 0)
        {
            tx_nonstop = 1;
        }
        if (ic->phy_perf_flag == 3){
            printk("wq_output_monitor: ic->mp_tx_ms:%d, ic->mp_tx_us:%d, ic->tx_count:%d, ic->mp_pkt_len:%d\n",ic->mp_tx_ms,ic->mp_tx_us,(int)ic->tx_count, ic->mp_pkt_len);
        }
        while ((monitor_tx_flag && ic->tx_count > 0) || (monitor_tx_flag && tx_nonstop))
        {
            tx_done_count ++;
            m = m_gethdr(M_NOWAIT, MT_DATA);

            if (m != NULL)
            {
                if (tx_nonstop == 0)
                {
                    ic->tx_count -= 1;
                    if (ic->tx_count == 0) {
                        printk("%s:last tx pkt\n", __func__);
                    }
                }
                if (ic->mo_payload_len)
                {
                    memcpy(m->m_data, ic->mo_payload, ic->mo_payload_len);
                    m->m_len = ic->mo_payload_len;
                } else if (ic->phy_perf_flag){
                    //set source address
                    memcpy(pkt_for_phy_test+10, vap->iv_ifp->if_addr.ifa_addr, 6);
                    //set BSSID
                    memcpy(pkt_for_phy_test+16, vap->iv_ifp->if_addr.ifa_addr, 6);
                    if (ic->phy_perf_flag == 1) {
                        memcpy(m->m_data, pkt_for_phy_test, 975);
                        m->m_len = 975;
                    } else if (ic->phy_perf_flag == 3){
                        memcpy(m->m_data, pkt_for_phy_test, ic->mp_pkt_len);
                        m->m_len = ic->mp_pkt_len;
                    }else{
                        memcpy(m->m_data, pkt_for_phy_test, sizeof(pkt_for_phy_test));
                        m->m_len = sizeof(pkt_for_phy_test);
                    }
                } else {
                    //set source address
                    memcpy(frame_becon_monitor+10, vap->iv_ifp->if_addr.ifa_addr, 6);
                    //set BSSID
                    memcpy(frame_becon_monitor+16, vap->iv_ifp->if_addr.ifa_addr, 6);
                    memcpy(m->m_data, frame_becon_monitor, sizeof(frame_becon_monitor));
                    m->m_len = sizeof(frame_becon_monitor);
                }
                m->m_flags |= M_FW_NO_ACK;
                m->vap = vap;
                ieee80211_ref_node(vap->iv_bss);
                m->m_pkthdr.rcvif = (void *) vap->iv_bss;
                //dump_bytes("wq_output_monitor", m->m_data, m->m_len);

                IEEE80211_TX_LOCK(ic);
                ic->ic_transmit(ic, m);
                IEEE80211_TX_UNLOCK(ic);
            }
            else
            {
                iot_printf("%s: m_gethdr fail\n", __func__);
            }
            if (ic->phy_perf_flag == 1) {
                usleep_range(50, 100);
            } else if (ic->phy_perf_flag == 2) {
                if (tx_done_count % 20 == 0) {
                    msleep(1);
                }
            } else if (ic->phy_perf_flag == 3) {
                if (ic->mp_tx_ms != 0) {
                    if (tx_done_count % ic->mp_tx_ms == 0) {
                        msleep(1);
                    }
                }
                if (ic->mp_tx_us != 0) {
                    usleep_range(ic->mp_tx_us, ic->mp_tx_us+50);
                }

            } else {
                msleep(1);
            }
        }

        ic->phy_perf_flag = 0;
        ic->mp_tx_ms = 0;
        ic->mp_tx_us = 0;
    }

    return;
}

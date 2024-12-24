/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2007-2009 Sam Leffler, Errno Consulting
 * Copyright (c) 2007-2008 Marvell Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 */



#include "if_hawk_port.h"

#include <net80211/ieee80211_var.h>
#include <net80211/ieee80211_input.h>
#include <net80211/ieee80211_regdomain.h>

#ifdef INET
#include <netinet/in.h>
#include <netinet/if_ether.h>
#endif /* INET */

#include <if_hawkvar.h>
#include <wq_hw_config.h>
#include "wq_hal_util.h"
#include "wq_htc_desc.h"
#include "wq_ipc.h"

/* idiomatic shorthands: MS = mask+shift, SM = shift+mask */
#define MS(v,x) (((v) & x) >> x##_S)
#define SM(v,x) (((v) << x##_S) & x)

int wq_tx_sche(struct wq_softc *sc);
struct ieee80211vap *wq_vap_create(struct ieee80211com *,
            const char [IFNAMSIZ], int, enum ieee80211_opmode, int,
            const uint8_t [IEEE80211_ADDR_LEN],
            const uint8_t [IEEE80211_ADDR_LEN]);
static void wq_vap_delete(struct ieee80211vap *);
static int  wq_hal_reset(struct wq_softc *sc);
static int  wq_init(struct wq_softc *);
static void wq_parent(struct ieee80211com *);
static int  wq_reset(struct ieee80211vap *, u_long);
static void wq_stop(struct wq_softc *);
void wq_start(struct wq_softc *, struct mbuf *);
int wq_transmit(struct ieee80211com *, struct mbuf *);
static int  wq_raw_xmit(struct ieee80211_node *, struct mbuf *);
#if 0 //not used
static int  wq_media_change(struct ifnet *);
static void wq_chanswitch_proc(struct work_struct * work);
static void wq_bawatchdog_proc(struct work_struct * work);
static void wq_update_mcast(struct ieee80211com *);
static void wq_update_promisc(struct ieee80211com *);
static void wq_dma_cleanup(struct wq_softc *);
static void wq_node_getmimoinfo(const struct ieee80211_node *,
            struct ieee80211_mimo_info *);
static void wq_rx_proc(struct work_struct *);
static uint16_t wq_calcformat(uint8_t rate, const struct ieee80211_node *);
static void wq_draintxq(struct wq_softc *);
static int  allocstaid(struct wq_softc *sc, int aid);
static void delstaid(struct wq_softc *sc, int staid);
#endif
static int  wq_ioctl(struct ieee80211com *, u_long, void *);
static int  wq_key_alloc(struct ieee80211vap *,
            struct ieee80211_key *,
            ieee80211_keyix *, ieee80211_keyix *);
static int  wq_key_delete(struct ieee80211vap *,
            const struct ieee80211_key *);
static int  wq_key_set(struct ieee80211vap *,
            const struct ieee80211_key *);
static int  _wq_key_set(struct ieee80211vap *,
            const struct ieee80211_key *,
            const uint8_t mac[IEEE80211_ADDR_LEN]);
static int  wq_mode_init(struct wq_softc *);
static void wq_updateslot(struct ieee80211com *);
static int  wq_beacon_setup(struct ieee80211vap *);
static void wq_beacon_update(struct ieee80211vap *, int);
#ifdef WQ_HOST_PS_SUPPORT
static void wq_update_ps(struct ieee80211vap *, int);
static int  wq_set_tim(struct ieee80211_node *, int);
#endif
static struct ieee80211_node *wq_node_alloc(struct ieee80211vap *,
            const uint8_t [IEEE80211_ADDR_LEN]);
static void wq_node_cleanup(struct ieee80211_node *);
static void wq_node_free(struct ieee80211_node *ni);
static void wq_node_drain(struct ieee80211_node *);
static void wq_node_getsignal(const struct ieee80211_node *,
            int8_t *, int8_t *);
static void wq_txq_init(struct wq_softc *sc, struct wq_txq *, int);
static int  wq_tx_setup(struct wq_softc *, int, int);
static int  wq_wme_update(struct ieee80211com *);
static void wq_tx_cleanupq(struct wq_softc *, struct wq_txq *);
static void wq_tx_cleanup(struct wq_softc *);
//static int  wq_tx_start(struct wq_softc *, struct ieee80211_node *,
//                 struct wq_txbuf *, struct mbuf *);
static int  wq_chan_set(struct wq_softc *, struct ieee80211_channel *);
static void wq_update_chw(struct ieee80211com *ic);
static int  wq_recv_action(struct ieee80211_node *,
            const struct ieee80211_frame *,
            const uint8_t *, const uint8_t *);
static int  wq_addba_request(struct ieee80211_node *,
            struct ieee80211_tx_ampdu *, int dialogtoken,
            int baparamset, int batimeout);
static int  wq_addba_response(struct ieee80211_node *,
            struct ieee80211_tx_ampdu *, int status,
            int baparamset, int batimeout);
static void wq_addba_response_timeout(struct ieee80211_node *ni,
            struct ieee80211_tx_ampdu *tap);
static void wq_addba_stop(struct ieee80211_node *,
            struct ieee80211_tx_ampdu *);
static WQ_HAL_APMODE wq_getapmode(const struct ieee80211vap *,
            struct ieee80211_channel *);
static int  wq_setapmode(struct ieee80211vap *, struct ieee80211_channel*);
static void wq_scan_start(struct ieee80211com *);
static void wq_scan_end(struct ieee80211com *);
static void wq_set_channel(struct ieee80211com *);
static int  wq_peerstadb(struct ieee80211_node *,
            int aid, int staid, WQ_HAL_PEERINFO *pi);
static int  wq_localstadb(struct ieee80211vap *);
static int  wq_newstate(struct ieee80211vap *, enum ieee80211_state, int);
static void wq_newassoc(struct ieee80211_node *, int);
static void wq_agestations(void *);
static int  wq_setregdomain(struct ieee80211com *,
            struct ieee80211_regdomain *, int,
            struct ieee80211_channel []);
static void wq_getradiocaps(struct ieee80211com *, int, u_int *,
            struct ieee80211_channel []);
static int  wq_getchannels(struct wq_softc *);

static void wq_announce(struct wq_softc *);
//static void wq_beacon_proc(void *arg);
static int wq_ampdu_rx_start(struct ieee80211_node *ni, struct ieee80211_rx_ampdu *rap,
	int baparamset, int batimeout, int baseqctl);
static void wq_ampdu_rx_stop(struct ieee80211_node *ni, struct ieee80211_rx_ampdu *rap);
static void wq_set_power_state(struct wq_softc *sc, struct wq_vap *mvp, int new_power_state, const char *func, int line);

static uint32_t wq_mbuf_get_qidx(struct mbuf *m);
void wq_newstate_setup(struct ieee80211vap *vap);
//FAST_ATTR static int wq_tx_legacy_one(struct wq_softc *sc, struct ieee80211_node *ni, uint8_t qidx, struct wq_txbuf *one_bf);

//extern funcions
extern void rc_init(void);
extern void dev_wifi_sim_init(void *ic);
extern void hwsim_txq_dequeue(void);
extern void wq_intr_handler(struct wq_softc *sc);

extern void wq_hal_rx_start(struct wq_hal *mh);
extern void wq_txrx_proc(struct work_struct * work);
extern void wq_rxq_proc_comp();
extern void wq_event_rxq_proc(struct work_struct * work);
extern void wq_rxq_replenish(struct work_struct * work);
extern void hwsim_tx_isr(struct work_struct * work);
extern uint8_t htc_node_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t mnid, uint8_t erp);
extern void htc_init_node_notify(struct wq_softc *sc, struct ieee80211_node *ni);
extern void htc_tx_ampdu_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t mnid, uint8_t ac, uint8_t code);
extern void htc_key_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t mnid, uint8_t *keyval, uint8_t *mac);
extern int htc_status_notify(struct wq_softc *sc, uint8_t msg_type);
extern void htc_ps_notify(struct wq_softc *sc, uint8_t macid, int nsta);
extern void htc_tim_notify(struct wq_softc *sc, uint8_t macid, uint8_t pm, uint16_t aid);
extern void htc_beacon_update_notify(struct wq_softc *sc, uint8_t macid, uint8_t mnid, struct mbuf *m);
extern int htc_ch_bw_notify(struct wq_softc *sc, uint8_t ch_num, uint16_t freq, int8_t power, uint8_t sw_ch_cali);
extern void htc_dev_info_notify(struct wq_softc *sc, uint8_t type, uint8_t *mac_addr, uint8_t fixed_rate);
extern void htc_mvap_notify(struct wq_softc *sc, uint8_t msg_type, uint8_t macid, uint8_t opmode);
extern void ieee80211_self_cts_transmitted(struct ieee80211_node *ni, void *arg, int status);
extern void wq_tx_postpond(struct mbuf *m);
extern void wq_output_monitor(struct work_struct * work);
extern struct ieee80211com *net80211_ic;
extern void wq_dbg_print (void *);

extern spinlock_t rx_skb_lock;
extern struct ieee80211vap *hawk_vap0;
extern struct ieee80211vap *hawk_vap1;

extern uint32_t rx_count;
extern uint32_t rx_mngt_count;
extern uint32_t rx_data_count;
extern int send_keepalive_flag;

#ifdef WQ_DEBUG
uint32_t wq_debug = WDRV_DEBUG_INFO|WDRV_DEBUG_STATE;
#define IS_BEACON(wh) \
    ((wh->i_fc[0] & (IEEE80211_FC0_TYPE_MASK|IEEE80211_FC0_SUBTYPE_MASK)) == \
     (IEEE80211_FC0_TYPE_MGT|IEEE80211_FC0_SUBTYPE_BEACON))
#define IFF_DUMPPKTS_RECV(sc, wh) \
    ((wq_debug & WDRV_DEBUG_RECV) && \
      ((wq_debug & WDRV_DEBUG_RECV_ALL) || !IS_BEACON(wh)))
#define IFF_DUMPPKTS_XMIT(sc) \
    (wq_debug & WDRV_DEBUG_XMIT)

#define KEYPRINTF(sc, hk, mac) do {             \
    if (wq_debug & WDRV_DEBUG_KEYCACHE)          \
        wq_keyprint(sc, __func__, hk, mac);        \
} while (0)

static  void wq_printtxbuf(const struct wq_txbuf *bf, u_int qnum, u_int ix);
#else
#define IFF_DUMPPKTS_RECV(sc, wh)   0
#define IFF_DUMPPKTS_XMIT(sc)       0
#define KEYPRINTF(sc, k, mac)       do { (void )sc; } while (0)
#endif

//static MALLOC_DEFINE(M_MWLDEV, "wqdev", "wq driver dma buffers");

/*
 * Each packet has fixed front matter: a 2-byte length
 * of the payload, followed by a 4-address 802.11 header
 * (regardless of the actual header and always w/o any
 * QoS header).  The payload then follows.
 */
struct wqtxrec {
    uint16_t fwlen;
    struct ieee80211_frame_addr4 wh;
} __packed;

int sizeof_wq_vap()
{
    return sizeof(struct wq_vap);
}


void
wq_printtxbuf(const struct wq_txbuf *bf, u_int qnum, u_int ix)
{
#ifdef WQ_DEBUG
    struct ieee80211_frame *wh;
    uint16_t seq;

    wh= (struct ieee80211_frame *) bf->bf_m->m_data;
    seq=le16toh(*(uint16_t *)wh->i_seq) >> IEEE80211_SEQ_SEQ_SHIFT;
    DPRINTF(WDRV_DEBUG_XMIT, "bf:0x%p, m:0x%p ,qnum:%d idx=%d, desc=0x%p, seq:%d\n",bf,bf->bf_m, qnum,ix,bf->bf_desc,seq);
#endif
}

static void wq_txbuflist_dump(wq_txbufhead *head)
{
    struct wq_txbuf *bf;
    int i = 0;

    DPRINTF(WDRV_DEBUG_XMIT, ANSI_COLOR_GREEN "head:0x%p\n" ANSI_COLOR_RESET,head);
    STAILQ_FOREACH(bf, head, bf_list) {
        DPRINTF(WDRV_DEBUG_XMIT, "%s---->bf:0x%p, m:0x%p , idx=%d, desc=0x%p\n",__func__,bf,bf->bf_m,i,bf->bf_desc);
        i++;
    }
}


static int wq_txbuf_list_setup(struct wq_softc *sc, wq_txbufhead *head, int nbuf, int is_mgmt)
{
    struct wq_txbuf *bf;
    int i, bsize;
    bsize = sizeof(struct wq_txbuf) * nbuf;
    bf = mmal_malloc(bsize);
    if (bf == NULL) {
        device_printf(sc->sc_dev, "malloc of wq_txbuf buffers failed, size %u\n", bsize);
        goto fail;
    }

    if(is_mgmt){
        sc->wq_txbuf_mgmt_ptr=bf;
    }else
        sc->wq_txbuf_data_ptr=bf;

    STAILQ_INIT(head);
    for (i = 0; i < nbuf; i++, bf++) {
        bf->is_mgmt=is_mgmt;
        bf->retry_cnt=0;
        STAILQ_INSERT_TAIL(head, bf, bf_list);
    }

    return 0;

fail:
    return -1;
}


static int wq_txbuf_pool_deinit(struct wq_softc *sc)
{
    mmal_free(sc->wq_txbuf_mgmt_ptr);
    mmal_free(sc->wq_txbuf_data_ptr);

	return 0;
}

static int wq_txbuf_pool_init(struct wq_softc *sc)
{
    int error;

    //setup txbuf for DATA
    error = wq_txbuf_list_setup(sc, &sc->sc_txbuf_data_list,WQ_TXBUF_DATA, 0);
    if (error != 0) {
        return error;
    }

    sc->sc_txbuf_data_cnt = WQ_TXBUF_DATA;


    //setup txbuf for MGMT
    error = wq_txbuf_list_setup(sc, &sc->sc_txbuf_mgmt_list,WQ_TXBUF_MGMT,1);
    if (error != 0) {
        return error;
    }

    //check points
    wq_txbuflist_dump(&sc->sc_txbuf_data_list);
    wq_txbuflist_dump(&sc->sc_txbuf_mgmt_list);
    return 0;
}

static void wq_init_node_tx_sche(struct wq_softc *sc)
{
    int idx;
    for(idx=0;idx<WQ_NUM_TX_QUEUES;idx++)
    {
        os_mem_set(&sc->sc_node_tx_sche[idx],0,sizeof(struct wq_node_tx_sche));
        mtx_init(&sc->sc_node_tx_sche[idx].lock, NULL, NULL, MTX_DEF);
    }
}

void wq_keepalive(void *arg) {
    struct ieee80211com *ic = arg;
    struct wq_softc *sc = (struct wq_softc *) ic->ic_softc;
    int ret = 0;

    if (send_keepalive_flag) {
 //     iot_printf("alive\n");
        ret = htc_status_notify(sc, WQ_KEEPALIVE);		
    }

    if (ret) {
        /* stop alive timer to avoid panic in case the timer is removed
        while it is still in the workqueue but has not yet been executed during rmmod */
        iot_printf("alive fail, stop timer\n");
    } else {
        callout_reset_v2(&ic->keepalive_timer, KEEPALIVE_LEN*hz);
    }

}

#if 0
static void wq_tx_proc(struct work_struct * work)
{
    struct wq_softc *sc = (struct wq_softc *)WORK_ARGV(work);
    struct mbuf *m;

    while ((m = mbufq_dequeue(&sc->sc_snd, &sc->sc_queue_lock)) != NULL) {
        if (!sc->sc_running) {
            m_freem(m);
            continue;
        }
        wq_start(sc, m);
    }

    return;
}
#endif
int
wq_attach(uint16_t devid, struct wq_softc *sc)
{
    struct ieee80211com *ic = &sc->sc_ic;
    struct wq_hal *mh;
    int error = 0, i;
    ic->ic_softc = sc; //set it before wq_hal_attach() for usb communication
    net80211_ic = ic;

    DPRINTF(WDRV_DEBUG_ANY, "%s: devid 0x%x\n", __func__, devid);

    mh = wq_hal_attach(sc->sc_dev, devid);
    if (mh == NULL) {
        device_printf(sc->sc_dev, "unable to attach HAL\n");
        error = EIO;
        goto bad;
    }
    sc->sc_mh = mh;

    error = wq_getchannels(sc);
    if (error != 0) {
        device_printf(sc->sc_dev, "failed to get channels: %d", error);
        goto bad1;
    }

    ieee80211_announce_channels(ic);

    mtx_init(&sc->sc_rx_lock, NULL, NULL, NULL);
    mtx_init(&sc->sc_node_lock, NULL, NULL, NULL);
    mtx_init(&sc->sc_tx_postpond_lock, NULL, NULL, NULL);
    mtx_init(&sc->tx_sche_lock, NULL, NULL, NULL);

    spin_lock_init(&sc->rx_skb_lock);
    spin_lock_init(&sc->txdone_mbuf_lock);
    spin_lock_init(&sc->tx_mbuf_lock);
    spin_lock_init(&sc->mac_cmd_lock);
    spin_lock_init(&sc->event_rx_skb_lock);

    sc->sc_ageinterval = WQ_AGEINTERVAL;

    os_mem_set(sc->node_uid_table,0,sizeof(sc->node_uid_table));
    /*
     * Allocate wq_txbufs and populate the lists.
     */
    error=wq_txbuf_pool_init(sc);
    if (error != 0) {
        device_printf(sc->sc_dev,
            "failed to allocate wq_txbufs: %d\n", error);
        goto bad;
    }

    /*
     * Initial aggregation settings.
     */
    //sc->sc_hwq_limit_aggr = WQ_AGGR_MIN_QDEPTH;
    //sc->sc_hwq_limit_nonaggr = WQ_NONAGGR_MIN_QDEPTH;

    callout_init_v2(&sc->sc_timer, wq_agestations, sc);
	iot_printf("init timer %s %p\n", "wq_agestations", wq_agestations);
    for (i = 0; i < WQ_NUM_EDCA_QUEUES; i++) {
        mbufq_init(&sc->sc_acq[i], IFQMAXLEN);
    }

    //debug timer
    callout_init_v2(&ic->ic_print, wq_dbg_print, sc);
    iot_printf("init timer %s %p\n", "wq_dbg_print", wq_dbg_print);
    callout_reset_v2(&ic->ic_print, hz);

    //keepalive timer
    callout_init_v2(&ic->keepalive_timer, wq_keepalive, ic);
    iot_printf("init timer %s %p\n", "wq_keepalive", wq_keepalive);
    callout_reset_v2(&ic->keepalive_timer, KEEPALIVE_LEN*hz);

    //sc->sc_tq = wk_task_queue_create("wq_taskq");

    //wk_init(&sc->sc_chanswitchtask, wq_chanswitch_proc, sc);
    //wk_init(&sc->sc_tx_task, wq_tx_proc, NULL);
    wk_init(&sc->sc_event_rxq_process_task, wq_event_rxq_proc, NULL);
    wk_init(&sc->sc_monitor_tx_task, wq_output_monitor, NULL);

    //wk_init(&sc->sc_rxq_replenish_task, wq_rxq_replenish, NULL);

    //wk_init_comp(&sc->sc_txq_process_task, wq_txq_proc, NULL, wq_txq_proc_comp);
    wk_init_comp(&sc->sc_rxq_process_task, wq_txrx_proc, NULL, wq_rxq_proc_comp);
    //wk_init(&sc->sc_posttbtt_task, wq_beaconq_proc, NULL);


    wq_tx_setup(sc, WME_AC_BK, WQ_WME_AC_BK);
    wq_tx_setup(sc, WME_AC_BE, WQ_WME_AC_BE);
    wq_tx_setup(sc, WME_AC_VI, WQ_WME_AC_VI);
    wq_tx_setup(sc, WME_AC_VO, WQ_WME_AC_VO);

    //set for GROUP/MGMT
    //wq_tx_setup(sc, WME_AC_MGMT, WQ_WME_AC_MGMT);
    //wq_tx_setup(sc, WME_AC_GROUP, WQ_WME_AC_GROUP);

    /* setup mcast swq & txq for sleep stas */
    //WQ_TXQ_LOCK_INIT(sc, &sc->sc_mcast_swq);
    STAILQ_INIT(&sc->sc_mcast_swq.active);
    //WQ_TXQ_LOCK_INIT(sc, &sc->sc_mcast_txq);
    STAILQ_INIT(&sc->sc_mcast_txq.active);

    mtx_init(&sc->wq_txq_lock, NULL, NULL, MTX_DEF);

    //init node_uid_table
    wq_init_node_tx_sche(sc);

    //rc_init();

    ic->ic_name = device_get_nameunit(sc->sc_dev);
    /* XXX not right but it's not used anywhere important */
    ic->ic_opmode = IEEE80211_M_STA;
    ic->ic_caps =
          IEEE80211_C_STA       /* station mode supported */
        | IEEE80211_C_HOSTAP        /* hostap mode */
        | IEEE80211_C_MONITOR       /* monitor mode */
#if 0
        | IEEE80211_C_IBSS      /* ibss, nee adhoc, mode */
        | IEEE80211_C_AHDEMO        /* adhoc demo mode */
        | IEEE80211_C_MBSS      /* mesh point link mode */
        | IEEE80211_C_WDS       /* WDS supported */
        | IEEE80211_C_TXFRAG        /* handle tx frags */
        | IEEE80211_C_DFS       /* DFS supported */
#endif
        | IEEE80211_C_SHPREAMBLE    /* short preamble supported */
        | IEEE80211_C_SHSLOT        /* short slot time supported */
        | IEEE80211_C_WME       /* WME/WMM supported */
        | IEEE80211_C_BURST     /* xmit bursting supported */
        | IEEE80211_C_WPA       /* capable of WPA1+WPA2 */
//        | IEEE80211_C_BGSCAN        /* capable of bg scanning */
        | IEEE80211_C_TXPMGT        /* capable of txpow mgt */
        | IEEE80211_C_SWSLEEP
        | IEEE80211_C_WPA3
        ;

    ic->ic_htcaps =
        //IEEE80211_HTCAP_CHWIDTH40 /* 40MHz channel width */
        //IEEE80211_HTCAP_SHORTGI20 /* short GI in 20MHz */
        //| IEEE80211_HTCAP_SHORTGI40 /* short GI in 40MHz */
        IEEE80211_HTCAP_RXSTBC_1STREAM/* 1-2 spatial streams */
#if WQ_AGGR_SIZE == 7935
        | IEEE80211_HTCAP_MAXAMSDU_7935 /* max A-MSDU length */
#else
        | IEEE80211_HTCAP_MAXAMSDU_3839 /* max A-MSDU length */
#endif
#if 0
        | IEEE80211_HTCAP_SMPS_ENA  /* SM PS mode enabled */
        | IEEE80211_HTCAP_PSMP      /* PSMP supported */
        | IEEE80211_HTCAP_40INTOLERANT  /* 40MHz intolerant */
        | IEEE80211_HTC_AMSDU       /* tx A-MSDU */
        | IEEE80211_HTC_SMPS        /* SMPS available */
#endif
        /* s/w capabilities */
        | IEEE80211_HTC_HT      /* HT operation */
        | IEEE80211_HTC_AMPDU       /* tx A-MPDU */
        ;
    if (sgi_rx_enable) {
        ic->ic_htcaps |= IEEE80211_HTCAP_SHORTGI20; /* short GI in 20MHz */
    }

    /*
     * Mark h/w crypto support.
     * XXX no way to query h/w support.
     */
#if 1 //USB project
    ic->ic_cryptocaps = IEEE80211_CRYPTO_AES_CCM
              |  IEEE80211_CRYPTO_WEP
              |  IEEE80211_CRYPTO_TKIP
              |  IEEE80211_CRYPTO_TKIPMIC
              ;
#else
    /* s/w crypt */
    ic->ic_cryptocaps = 0;
#endif

    /* driver crypto capability */
    ic->ic_drvcryptocaps = IEEE80211_CRYPTO_WEP
              |  IEEE80211_CRYPTO_AES_CCM
              |  IEEE80211_CRYPTO_TKIP
              |  IEEE80211_CRYPTO_TKIPMIC
              ;

    /*
     * Transmit requires space in the packet for a special
     * format transmit record and optional padding between
     * this record and the payload.  Ask the net80211 layer
     * to arrange this when encapsulating packets so we can
     * add it efficiently.
     */
    ic->ic_headroom = sizeof(struct wqtxrec) -
        sizeof(struct ieee80211_frame);

    IEEE80211_ADDR_COPY(ic->ic_macaddr, tmp_macaddr);

    ic->ic_rxstream = 1;
    ic->ic_txstream = 1;

    /* call MI attach routine. */
    ieee80211_ifattach(ic);
    ic->ic_setregdomain = wq_setregdomain;
    ic->ic_getradiocaps = wq_getradiocaps;
    /* override default methods */
    ic->ic_raw_xmit = wq_raw_xmit;
    ic->ic_newassoc = wq_newassoc;
    ic->ic_updateslot = wq_updateslot;
    //ic->ic_update_mcast = wq_update_mcast;
    //ic->ic_update_promisc = wq_update_promisc;
    ic->ic_wme.wme_update = wq_wme_update;
    ic->ic_transmit = wq_transmit;
    ic->ic_ioctl = wq_ioctl;
    ic->ic_parent = wq_parent;

    ic->ic_node_alloc = wq_node_alloc;
    sc->sc_node_free = ic->ic_node_free;
    ic->ic_node_free = wq_node_free;
    sc->sc_node_cleanup = ic->ic_node_cleanup;
    ic->ic_node_cleanup = wq_node_cleanup;
    sc->sc_node_drain = ic->ic_node_drain;
    ic->ic_node_drain = wq_node_drain;
    ic->ic_node_getsignal = wq_node_getsignal;
    //ic->ic_node_getmimoinfo = wq_node_getmimoinfo;

    ic->ic_scan_start = wq_scan_start;
    ic->ic_scan_end = wq_scan_end;
    ic->ic_set_channel = wq_set_channel;
    ic->ic_update_chw = wq_update_chw;

    sc->sc_recv_action = ic->ic_recv_action;
    ic->ic_recv_action = wq_recv_action;
    sc->sc_addba_request = ic->ic_addba_request;
    ic->ic_addba_request = wq_addba_request;
    sc->sc_addba_response = ic->ic_addba_response;
    ic->ic_addba_response = wq_addba_response;
    sc->sc_addba_response_timeout = ic->ic_addba_response_timeout;
    ic->ic_addba_response_timeout = wq_addba_response_timeout;
    sc->sc_addba_stop = ic->ic_addba_stop;
    ic->ic_addba_stop = wq_addba_stop;

    sc->sc_ampdu_rx_start = ic->ic_ampdu_rx_start;
	ic->ic_ampdu_rx_start = wq_ampdu_rx_start;
	sc->sc_ampdu_rx_stop = ic->ic_ampdu_rx_stop;
	ic->ic_ampdu_rx_stop = wq_ampdu_rx_stop;

    ic->ic_vap_create = wq_vap_create;
    ic->ic_vap_delete = wq_vap_delete;



    if (bootverbose)
        ieee80211_announce(ic);
    wq_announce(sc);

    /* reserve node table slot 0/1 for sta/ap group key */
    setbit(sc->sc_keymap, 0);
    setbit(sc->sc_keymap, 1);
    /* reserve node table slot (WQ_NODE_TABLE_SZ - 1) for sta wep key */
    setbit(sc->sc_keymap, (WQ_NODE_TABLE_SZ - 1));

    atomic_set(&sc->sc_tx_fw_pending, 0);

#ifdef WIFI_UART_SIM
    extern struct ieee80211com *g_ic;
    g_ic=ic;
#endif
#ifdef WIFI_SIM
    dev_wifi_sim_init(sc);
#endif
    wq_dbg_flag = WQ_DEBUG_NONE;

    return 0;
bad1:
    wq_hal_detach(mh);
bad:
    IOT_ASSERT(0);
    return error;
}

extern void flush_all_queue(void);

void
wq_notify_hawk_stop(struct wq_softc *sc)
{
    if (sc == NULL) {
        printk("%s error!\n", __func__);
        return;
    }
    wq_stop(sc);
}

int
wq_detach(struct wq_softc *sc)
{
    struct ieee80211com *ic = &sc->sc_ic;
    int i;

    WQ_LOCK(sc);
    wq_stop(sc);
    WQ_UNLOCK(sc);

    /*
     * NB: the order of these is important:
     * o call the 802.11 layer before detaching the hal to
     *   insure callbacks into the driver to delete global
     *   key cache entries can be handled
     * o reclaim the tx queue data structures after calling
     *   the 802.11 layer as we'll get called back to reclaim
     *   node state and potentially want to use them
     * o to cleanup the tx queues the hal is called, so detach
     *   it last
     * Other than that, it's straightforward...
     */
    ieee80211_ifdetach(ic);

    //htc_status_notify(sc, WQ_USB_RESET);
    flush_all_queue();

    wq_tx_cleanup(sc);
    wq_hal_detach(sc->sc_mh);
    for (i = 0; i < WQ_NUM_TX_QUEUES; i++) {
        mbufq_drain(&sc->sc_acq[i]);
    }

    mtx_destroy(&sc->sc_rx_lock);
    mtx_destroy(&sc->sc_node_lock);
    mtx_destroy(&sc->sc_tx_postpond_lock);
    mtx_destroy(&sc->tx_sche_lock);

    wq_txbuf_pool_deinit(sc);
    mtx_destroy(&sc->wq_txq_lock);
    callout_drain_v2(&ic->ic_print);
    iot_printf("drain keepalive\n");
    callout_drain_v2(&ic->keepalive_timer);
    IOT_ASSERT(ic == net80211_ic);
    net80211_ic->ic_softc = NULL;

    return 0;
}

/*
 * MAC address handling for multiple BSS on the same radio.
 * The first vap uses the MAC address from the EEPROM.  For
 * subsequent vap's we set the U/L bit (bit 1) in the MAC
 * address and use the next six bits as an index.
 */
static void
assign_address(struct wq_softc *sc, uint8_t mac[IEEE80211_ADDR_LEN], int clone)
{
    int i;

    if (clone) {
        /* NB: we only do this if h/w supports multiple bssid */
        for (i = 0; i < 32; i++)
            if ((sc->sc_bssidmask & (1<<i)) == 0)
                break;
        if (i != 0)
            mac[0] |= (i << 2)|0x2;
    } else
        i = 0;
    sc->sc_bssidmask |= 1<<i;
    if (i == 0)
        sc->sc_nbssid0++;
}

static void
reclaim_address(struct wq_softc *sc, const uint8_t mac[IEEE80211_ADDR_LEN])
{
    int i = mac[0] >> 2;
    if (i != 0 || --sc->sc_nbssid0 == 0)
        sc->sc_bssidmask &= ~(1<<i);
}


struct ieee80211vap *
wq_vap_create(struct ieee80211com *ic, const char name[IFNAMSIZ], int unit,
    enum ieee80211_opmode opmode, int flags,
    const uint8_t bssid[IEEE80211_ADDR_LEN],
    const uint8_t mac0[IEEE80211_ADDR_LEN])
{
    struct wq_softc *sc = ic->ic_softc;
    struct wq_hal *mh = sc->sc_mh;
    struct ieee80211vap *vap;
    struct wq_hal_vap *hvap;
    struct wq_vap *mvp;
    uint8_t mac[IEEE80211_ADDR_LEN];

    IEEE80211_ADDR_COPY(mac, mac0);
    switch (opmode) {
    case IEEE80211_M_HOSTAP:
        if ((flags & IEEE80211_CLONE_MACADDR) == 0)
            assign_address(sc, mac, flags & IEEE80211_CLONE_BSSID);
        hvap = wq_hal_newvap(mh, WQ_HAL_AP, mac);
        if (hvap == NULL) {
            if ((flags & IEEE80211_CLONE_MACADDR) == 0)
                reclaim_address(sc, mac);
            return NULL;
        }
        break;
    case IEEE80211_M_STA:
    case IEEE80211_M_MONITOR:
        if ((flags & IEEE80211_CLONE_MACADDR) == 0)
            assign_address(sc, mac, flags & IEEE80211_CLONE_BSSID);
        hvap = wq_hal_newvap(mh, WQ_HAL_STA, mac);
        if (hvap == NULL) {
            if ((flags & IEEE80211_CLONE_MACADDR) == 0)
                reclaim_address(sc, mac);
            return NULL;
        }
        /* no h/w beacon miss support; always use s/w */
        flags |= IEEE80211_CLONE_NOBEACONS;
        break;
    default:
        return NULL;
    }

    mvp = malloc(sizeof(struct wq_vap), M_80211_VAP, M_WAITOK | M_ZERO);
    memset(mvp,0,sizeof(struct wq_vap));

    mvp->mv_hvap = hvap;

    mvp->wq_vap_power_state = WQ_SLEEP_STATE;

    vap = &mvp->mv_vap;
    ieee80211_vap_setup(ic, vap, name, unit, opmode, flags, bssid);
    /* override with driver methods */
    mvp->mv_newstate = vap->iv_newstate;
    vap->iv_newstate = wq_newstate;
    //vap->iv_max_keyix = 0;  /* XXX */
    vap->iv_key_alloc = wq_key_alloc;
    vap->iv_key_delete = wq_key_delete;
    vap->iv_key_set = wq_key_set;
#ifdef WQ_HOST_PS_SUPPORT
    if (opmode == IEEE80211_M_HOSTAP || opmode == IEEE80211_M_MBSS) {
        vap->iv_update_ps = wq_update_ps;
        mvp->mv_set_tim = vap->iv_set_tim;
        vap->iv_set_tim = wq_set_tim;
    }
#endif
    vap->iv_reset = wq_reset;
    vap->iv_update_beacon = wq_beacon_update;

    /* override max aid so sta's cannot assoc when we're out of sta id's */
    vap->iv_max_aid = WQ_MAXSTAID;
    /* override default A-MPDU rx parameters */
    //vap->iv_ampdu_rxmax = IEEE80211_HTCAP_MAXRXAMPDU_64K;
    //vap->iv_ampdu_density = IEEE80211_HTCAP_MPDUDENSITY_4;

    memcpy(vap->iv_ifp->if_addr.ifa_addr, mac, 6);
    if(vap->iv_opmode==IEEE80211_M_HOSTAP){
        vap->iv_ifp->if_index=0;
    }else
        vap->iv_ifp->if_index=1;

    /* complete setup */
    ieee80211_vap_attach(vap, NULL, NULL,
        mac);

    switch (vap->iv_opmode) {
    case IEEE80211_M_HOSTAP:
    //case IEEE80211_M_MBSS:
    case IEEE80211_M_STA:
        /*
         * Setup sta db entry for local address.
         */
        wq_localstadb(vap);
        if (vap->iv_opmode == IEEE80211_M_HOSTAP ||
            vap->iv_opmode == IEEE80211_M_MBSS)
            sc->sc_napvaps++;
        else
            sc->sc_nstavaps++;
        break;

    default:
        break;
    }

    if (vap->iv_opmode == IEEE80211_M_HOSTAP) {
        /* support only 1 ap for now */
        IOT_ASSERT(sc->sc_bvap == NULL);
        sc->sc_bvap = vap;
        mvp->beacon_info.buf = NULL;
    }

    /*
     * Setup overall operating mode.
     */
    if (sc->sc_napvaps)
        ic->ic_opmode = IEEE80211_M_HOSTAP;
    else if (sc->sc_nstavaps)
        ic->ic_opmode = IEEE80211_M_STA;
    else
        ic->ic_opmode = opmode;

    vap->iv_macid = wq_hal_get_macid(hvap);
    init_completion(&vap->iv_wait);
    vap->iv_wait.done = 1;

    init_completion(&vap->iv_self_cts_wait);
    init_completion(&vap->iv_nulldata_wait);

    htc_mvap_notify(sc, MVAP_ALLOC, vap->iv_macid, vap->iv_opmode); //notify fw to create mvap
    return vap;
}

void wq_newstate_setup(struct ieee80211vap *vap)
{
    struct wq_vap *mvp = WQ_VAP(vap);
    mvp->mv_newstate = vap->iv_newstate;
    vap->iv_newstate = wq_newstate;
}

void wq_stop_tbtt(struct ieee80211vap *vap)
{
    struct wq_vap *mvp = WQ_VAP(vap);
    struct wq_hal_vap *hvap = mvp->mv_hvap;
    wq_hal_clr_tbtt_timer(hvap);
}

static void
wq_vap_delete(struct ieee80211vap *vap)
{
    struct wq_softc *sc = vap->iv_ic->ic_softc;
    struct wq_vap *mvp = WQ_VAP(vap);
    //struct wq_hal *mh = sc->sc_mh;
    struct wq_hal_vap *hvap = mvp->mv_hvap;
    enum ieee80211_opmode opmode = vap->iv_opmode;

    htc_mvap_notify(sc, MVAP_FREE, vap->iv_macid, 0); //notify fw to FREE fw mvap
    ieee80211_vap_detach(vap);
    switch (opmode) {
    case IEEE80211_M_HOSTAP:
    case IEEE80211_M_MBSS:
    case IEEE80211_M_STA:
    case IEEE80211_M_MONITOR:
        KASSERT(hvap != NULL, ("no hal vap handle"));
        wq_hal_delvap(hvap);
        if (opmode == IEEE80211_M_HOSTAP || opmode == IEEE80211_M_MBSS)
        {
            sc->sc_napvaps--;
            sc->sc_bvap = NULL;
        }
        else
            sc->sc_nstavaps--;
        /* XXX don't do it for IEEE80211_CLONE_MACADDR */
        reclaim_address(sc, vap->iv_myaddr);
        break;

    default:
        break;
    }
    //wq_cleartxq(sc, vap);

    if (mvp->beacon_info.buf) {
        m_free(mvp->beacon_info.buf);
    }

    free(mvp, M_80211_VAP);
}

#if 0
void
wq_suspend(struct wq_softc *sc)
{

    WQ_LOCK(sc);
    wq_stop(sc);
    WQ_UNLOCK(sc);
}

void
wq_resume(struct wq_softc *sc)
{
    int error = EDOOFUS;

    WQ_LOCK(sc);
    if (sc->sc_ic.ic_nrunning > 0)
        error = wq_init(sc);
    WQ_UNLOCK(sc);


    IEEE80211_LOCK(&sc->sc_ic);
    if (error == 0)
        ieee80211_start_all(&sc->sc_ic);    /* start all vap's */
	IEEE80211_UNLOCK(&sc->sc_ic);

}
#endif
void
wq_shutdown(void *arg)
{
    struct wq_softc *sc = arg;

    WQ_LOCK(sc);
    wq_stop(sc);
    WQ_UNLOCK(sc);
}

static int
wq_hal_reset(struct wq_softc *sc)
{
    struct ieee80211com *ic = &sc->sc_ic;
    struct wq_hal *mh = sc->sc_mh;

    wq_chan_set(sc, ic->ic_curchan);

    wq_hal_setcfend(mh, 1);            /* XXX */

    return 1;
}

static int
wq_init(struct wq_softc *sc)
{
    struct wq_hal *mh = sc->sc_mh;

    WQ_LOCK_ASSERT(sc);

    /*
     * Stop anything previously setup.  This is safe
     * whether this is the first time through or not.
     */
    wq_stop(sc);

    //mtx_init(&sc->sc_rx_lock, NULL, NULL, NULL);
    //(&sc->sc_queue_lock, NULL, NULL, NULL);

    /*
     * Push vap-independent state to the firmware.
     */
    if (!wq_hal_reset(sc)) {
        device_printf(sc->sc_dev, "unable to reset hardware\n");
        return EIO;
    }

    /*
     * Setup tx/rx
     */
    htc_status_notify(sc, WQ_INIT);

    /*
     * Enable interrupts.
     */
    sc->sc_imask = 0;

    sc->sc_running = 1;

    wq_hal_intrset(mh, sc->sc_imask);

    return 0;
}

static void
wq_stop(struct wq_softc *sc)
{
    WQ_LOCK_ASSERT(sc);
    if (sc->sc_running) {
        /*
         * Shutdown the hardware and driver.
         */
        sc->sc_running = 0;
        htc_status_notify(sc, WQ_STOP);
    }
}

static int
wq_reset_vap(struct ieee80211vap *vap, int state)
{
    struct wq_hal_vap *hvap = WQ_VAP(vap)->mv_hvap;
    struct ieee80211com *ic = vap->iv_ic;

    //if (state == IEEE80211_S_RUN)
    //    wq_setrates(vap);
    /* XXX off by 1? */
    //wq_hal_setrtsthreshold(hvap, vap->iv_rtsthreshold);
    /* XXX auto? 20/40 split? */
    wq_hal_sethtgi(hvap, (vap->iv_flags_ht &
        (IEEE80211_FHT_SHORTGI20|IEEE80211_FHT_SHORTGI40)) ? 1 : 0);
    wq_hal_setnprot(hvap, ic->ic_htprotmode == IEEE80211_PROT_NONE ?
        HTPROTECT_NONE : HTPROTECT_AUTO);
    /* XXX txpower cap */

    /* re-setup beacons */
    if (state == IEEE80211_S_RUN &&
        (vap->iv_opmode == IEEE80211_M_HOSTAP ||
         vap->iv_opmode == IEEE80211_M_MBSS ||
         vap->iv_opmode == IEEE80211_M_IBSS)) {
        wq_setapmode(vap, vap->iv_bss->ni_chan);
        wq_hal_setnprotmode(hvap,
            MS(ic->ic_curhtprotmode, IEEE80211_HTINFO_OPMODE));
        return wq_beacon_setup(vap);
    }
    return 0;
}

/*
 * Reset the hardware w/o losing operational state.
 * Used to reset or reload hardware state for a vap.
 */
static int
wq_reset(struct ieee80211vap *vap, u_long cmd)
{
    struct wq_hal_vap *hvap = WQ_VAP(vap)->mv_hvap;
    int error = 0;

    if (hvap != NULL) {         /* WDS, MONITOR, etc. */
        struct ieee80211com *ic = vap->iv_ic;
        struct wq_softc *sc = ic->ic_softc;
        struct wq_hal *mh = sc->sc_mh;

        /* XXX handle DWDS sta vap change */
        /* XXX do we need to disable interrupts? */
        wq_hal_intrset(mh, 0);     /* disable interrupts */
        error = wq_reset_vap(vap, vap->iv_state);
        wq_hal_intrset(mh, sc->sc_imask);
    }
    return error;
}


#ifdef WIFI_UART_SIM
extern void IRAM_ATTR uart_dma_write(uint8_t *bufptr, uint32_t size,
    void (*callback) (void*, uint32_t), void* dummy);
extern void dump_bytes(char *note, uint8_t *data, int len);

uint8_t uart_start_header[]={0xAB,0xCD,0x00,0x00};


void uart_dma_write_header(int len)
{
    int lower_8=len%256;
    int upper_4=len>>8;

    DPRINTF(WDRV_DEBUG_SIMTX, "len %d->0x%2x %2x\n",len,upper_4,lower_8);
    uart_start_header[2]=upper_4;
    uart_start_header[3]=lower_8;
    uart_dma_write((uint8_t *)uart_start_header, sizeof(uart_start_header), 0,0);
}
#endif

int
FAST_ATTR wq_transmit(struct ieee80211com *ic, struct mbuf *m)
{
    struct wq_softc *sc = ic->ic_softc;
    struct ieee80211_node *ni = (struct ieee80211_node *)m->m_pkthdr.rcvif;
    int qidx = wq_mbuf_get_qidx(m);

    if (sc->sc_tx_postpond) {
        wq_tx_postpond(m);
        return 0;
    }

    /* htc encap for fw */
    m->m_flags |= M_NEED_ACK;
    if (!ieee80211_host_desc_gen(m, ni)) {
        ieee80211_free_node(ni);
        m_freem(m);
        return 0;
    }

    /* reserve header room for ipc header */
    if (!m_prepend(m, wq_ipc_headroom_size(), 0)) {
        IOT_ASSERT(0);
    }

    if (mbufq_enqueue(&sc->sc_acq[qidx], &sc->wq_txq_lock, m, QUEUE_TAIL)) {
        /* queue full, drop this frame */
        ieee80211_free_node(ni);
        atomic_add(1, &drop_pkt_cnt);
        m_freem(m);
        return 0;
       }
    wq_tx_sche(sc);

    return (0);
}

static __inline uint32_t wq_mbuf_get_qidx(struct mbuf *m)
{
    struct ieee80211_frame *wh=(struct ieee80211_frame *)m->m_data;
    uint8_t qidx=WQ_WME_AC_BK;
    uint8_t subtype = wh->i_fc[0] & IEEE80211_FC0_SUBTYPE_MASK;

    if(IEEE80211_IS_MGMT(wh)){
        qidx=WQ_WME_AC_VO;
    }
    else{
        if( (subtype == IEEE80211_FC0_SUBTYPE_QOS_NULL)||
            (subtype == IEEE80211_FC0_SUBTYPE_NODATA)){
            //NOTE: Avoid QOS_NULL/NODATA be aggregated
            qidx=WQ_WME_AC_VO;
        }
        else if(IEEE80211_IS_MULTICAST(wh->i_addr1)){
            qidx=WQ_WME_AC_BE;
        }
        else
            qidx=M_WME_GETAC(m);
    }

    DPRINTF(WDRV_DEBUG_XMIT, "%s: qidx=%d\n",__func__,qidx);
    return qidx;
}


void
wq_txq_dump(struct wq_txq *txq)
{
    struct wq_txbuf *bf;
    int i = 0;

    STAILQ_FOREACH(bf, &txq->active, bf_list) {
        wq_printtxbuf(bf, txq->qnum, i);
        i++;
    }
}

uint8_t tx_frame_ctrl;
uint8_t tx_auth_frame_seq;

void inject_packets(void *vap_ptr, int types)
{
	struct ieee80211vap *vap = vap_ptr;
	struct ieee80211com *ic;
	struct wq_softc *sc;
	struct wuqi_work_struct *wk;
	tx_frame_ctrl = types;
	iot_printf("inject_probe_req vap=%p\n", vap_ptr);
	if (vap_ptr != NULL)
	{
	    ic = vap->iv_ic;
	    sc = ic->ic_softc;

		iot_printf("inject_probe_req ic=%p sc=%p\n", ic ,sc);

		if ((ic != NULL) && (sc != NULL))
		{
			//simulate rx porbe req
			wk = &sc->sc_rxq_process_task;

			//Check if workqueue is idle then insert another work
			wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)(1));
		}
	}
}

static int
wq_raw_xmit(struct ieee80211_node *ni, struct mbuf *m)
{
    struct ieee80211com *ic = ni->ni_ic;
    struct wq_softc *sc = ic->ic_softc;
    uint8_t qidx;
    //struct wuqi_work_struct *wk = &sc->sc_tx_task;
#ifdef WQ_SIM_HOST_ONLY
	struct wuqi_work_struct *wk;
#endif

    //DPRINTF(WDRV_DEBUG_XMIT, ANSI_COLOR_YELLOW "wq_raw_xmit\n" ANSI_COLOR_RESET);
#if 1
	//mbuf_dump_data("wq_raw_xmit", m);

	tx_frame_ctrl = m->m_data[0];
	tx_auth_frame_seq = m->m_data[26];

    qidx = wq_mbuf_get_qidx(m);

    /* htc encap for fw */
    m->m_flags |= M_NEED_ACK;
    if (!ieee80211_host_desc_gen(m, ni)) {
        ieee80211_free_node(ni);
        m_freem(m);
        return 0;
    }

    IOT_ASSERT(ni == (struct ieee80211_node *)m->m_pkthdr.rcvif);

    /* reserve header room for ipc header */
    if (!m_prepend(m, wq_ipc_headroom_size(), 0)) {
        IOT_ASSERT(0);
    }

    if (mbufq_enqueue(&sc->sc_acq[qidx], &sc->wq_txq_lock, m, QUEUE_TAIL)) {
        /* queue full, drop this frame */
        ieee80211_free_node(ni);
        atomic_add(1, &drop_pkt_cnt);
        m_freem(m);
        return 0;
    }
    wq_tx_sche(sc);

#ifdef WQ_SIM_HOST_ONLY
	//simulate rx porbe rsp
    wk = &sc->sc_rxq_process_task;

    //Check if workqueue is idle then insert another work
    wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)(1));
#endif

#else
    if (!sc->sc_running) {
        m_freem(m);
        return ENETDOWN;
    }
    mbufq_enqueue(&sc->sc_snd, &sc->sc_queue_lock, m, QUEUE_HP);
    wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)sc);
#endif
    return (0);
}
#if 0
static int
wq_media_change(struct ifnet *ifp)
{
    //struct ieee80211vap *vap = ifp->if_softc;
    int error;

    error = ieee80211_media_change(ifp);
    /* NB: only the fixed rate can change and that doesn't need a reset */
    if (error == ENETRESET) {
    //    wq_setrates(vap);
        error = 0;
    }
    return error;
}
#endif
#ifdef WQ_DEBUG
static void
wq_keyprint(struct wq_softc *sc, const char *tag,
    const WQ_HAL_KEYVAL *hk, const uint8_t mac[IEEE80211_ADDR_LEN])
{
    static const char *ciphers[] = {
        "WEP",
        "TKIP",
        "AES-CCM",
    };
    uint32_t i, n;

    DPRINTF(WDRV_DEBUG_KEYCACHE, "%s: [%u] %-7s", tag, hk->keyIndex, ciphers[hk->keyTypeId]);
    for (i = 0, n = hk->keyLen; i < n; i++)
        DPRINTF(WDRV_DEBUG_KEYCACHE, " %02x", hk->key.aes[i]);
    DPRINTF(WDRV_DEBUG_KEYCACHE, " mac %s", ether_sprintf(mac));
    if (hk->keyTypeId == KEY_TYPE_ID_TKIP) {
        DPRINTF(WDRV_DEBUG_KEYCACHE, " %s", "rxmic");
        for (i = 0; i < sizeof(hk->key.tkip.rxMic); i++)
            DPRINTF(WDRV_DEBUG_KEYCACHE, " %02x", hk->key.tkip.rxMic[i]);
        DPRINTF(WDRV_DEBUG_KEYCACHE, " txmic");
        for (i = 0; i < sizeof(hk->key.tkip.txMic); i++)
            DPRINTF(WDRV_DEBUG_KEYCACHE, " %02x", hk->key.tkip.txMic[i]);
    }
    DPRINTF(WDRV_DEBUG_KEYCACHE, " flags 0x%x\n", hk->keyFlags);
}
#endif

ieee80211_keyix wq_keyix_found(struct ieee80211vap *vap)
{
    struct wq_softc *sc = vap->iv_ic->ic_softc;
    ieee80211_keyix keyix;
    int i;

    /* entry 0 is reserved for group key */
    for (i = 0; i < WQ_NODE_TABLE_SZ/NBBY; i++) {
        uint8_t b = sc->sc_keymap[i];
        if (b != 0xf) {
            keyix = (i * NBBY);
            while (b & 1) {
                keyix++;
                b >>= 1;
            }
            setbit(sc->sc_keymap, keyix);
            return keyix;
        }
    }

    return IEEE80211_KEYIX_NONE;
}

/*
 * Allocate a key cache slot for a unicast key.  The
 * firmware handles key allocation and every station is
 * guaranteed key space so we are always successful.
 */
static int
wq_key_alloc(struct ieee80211vap *vap, struct ieee80211_key *k,
    ieee80211_keyix *keyix, ieee80211_keyix *rxkeyix)
{
    const struct ieee80211_cipher *cip = k->wk_cipher;

    if (cip->ic_cipher == IEEE80211_CIPHER_WEP) {
        /* for wep, keyix represents keyid but hw slot id */
        *keyix = *rxkeyix = ieee80211_crypto_get_key_wepidx(vap, k);
    } else {
        /* allocate hw node table idx for this key */
        if (k->wk_flags & IEEE80211_KEY_GROUP) {
            /* for group key arrangement, idx 0 is used for STA,
             * and idx 1 is for AP
             */
            if (vap->iv_opmode == IEEE80211_M_STA) {
                *keyix = *rxkeyix = 0;
            } else {
                *keyix = *rxkeyix = 1;
            }
        } else {
            *keyix = *rxkeyix = wq_keyix_found(vap);
        }
    }

    DPRINTF(WDRV_DEBUG_KEYCACHE, "%s: alloc key: %u %u\n", __func__, *keyix, *rxkeyix);
    return 1;
}

/*
 * Delete a key entry allocated by wq_key_alloc.
 */
static int
wq_key_delete(struct ieee80211vap *vap, const struct ieee80211_key *k)
{
    struct wq_softc *sc = vap->iv_ic->ic_softc;
    struct wq_hal_vap *hvap = WQ_VAP(vap)->mv_hvap;
    WQ_HAL_KEYVAL hk;
    const uint8_t bcastaddr[IEEE80211_ADDR_LEN] =
        { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    if (hvap == NULL) {
            /* XXX monitor mode? */
            DPRINTF(WDRV_DEBUG_KEYCACHE,
                "%s: no hvap for opmode %d\n", __func__,
                vap->iv_opmode);
            return 0;
    }

    DPRINTF(WDRV_DEBUG_KEYCACHE, "%s: delete key %u\n",
        __func__, k->wk_keyix);

    memset(&hk, 0, sizeof(hk));
    hk.keyIndex = k->wk_keyix;
    switch (k->wk_cipher->ic_cipher) {
    case IEEE80211_CIPHER_WEP:
        hk.keyTypeId = KEY_TYPE_ID_WEP;
        break;
    case IEEE80211_CIPHER_TKIP:
        hk.keyTypeId = KEY_TYPE_ID_TKIP;
        break;
    case IEEE80211_CIPHER_AES_CCM:
        hk.keyTypeId = KEY_TYPE_ID_AES;
        break;
    default:
        /* XXX should not happen */
        DPRINTF(WDRV_DEBUG_KEYCACHE, "%s: unknown cipher %d\n",
            __func__, k->wk_cipher->ic_cipher);
        return 0;
    }

    if (hk.keyTypeId == KEY_TYPE_ID_WEP) {
        if (vap->iv_opmode == IEEE80211_M_STA) {
            /* for wep sta, clear group key */
            hk.keyIndex = 0;
            htc_key_notify(sc, KEY_DELETE, WQ_NODE_INVALID, (uint8_t *)&hk, (uint8_t *)bcastaddr);
        }
        hk.keyIndex = WEP_KEY_IDX;
        htc_key_notify(sc, KEY_DELETE, WQ_NODE_INVALID, (uint8_t *)&hk, (uint8_t *)bcastaddr);

        return 1;
    }

    if(hk.keyIndex>=2) //First two slot is reserved. No clear.
        clrbit(sc->sc_keymap, hk.keyIndex);

    htc_key_notify(sc, KEY_DELETE, WQ_NODE_INVALID, (uint8_t *)&hk, (uint8_t *)bcastaddr);
    return 1;
}

static __inline int
addgroupflags(WQ_HAL_KEYVAL *hk, const struct ieee80211_key *k)
{
    if (k->wk_flags & IEEE80211_KEY_GROUP) {
        if (k->wk_flags & IEEE80211_KEY_XMIT)
            hk->keyFlags |= KEY_FLAG_TXGROUPKEY;
        if (k->wk_flags & IEEE80211_KEY_RECV)
            hk->keyFlags |= KEY_FLAG_RXGROUPKEY;
        return 1;
    } else
        return 0;
}

bool is_wq_tx_postpond(struct wq_softc *sc,int ind)
{
    if (sc->sc_tx_postpond & ind) {
        return true;
    } else {
        return false;
    }
}

void wq_tx_postpond_set(struct wq_softc *sc, int ind)
{
    sc->sc_tx_postpond |= ind;
}

void wq_tx_postpond_clr(struct wq_softc *sc, int ind)
{
    struct wuqi_work_struct *wk;
    struct ieee80211com *ic = &sc->sc_ic;

    sc->sc_tx_postpond &= ~ind;

    /* reschedule the postpond work */
    wk = &sc->sc_rxq_process_task;
    wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)(1));
}

/*
 * Set the key cache contents for the specified key.  Key cache
 * slot(s) must already have been allocated by wq_key_alloc.
 */
static int
wq_key_set(struct ieee80211vap *vap, const struct ieee80211_key *k)
{
    return (_wq_key_set(vap, k, k->wk_macaddr));
}

static int
_wq_key_set(struct ieee80211vap *vap, const struct ieee80211_key *k,
    const uint8_t mac[IEEE80211_ADDR_LEN])
{
#define GRPXMIT (IEEE80211_KEY_XMIT | IEEE80211_KEY_GROUP)
/* NB: static wep keys are marked GROUP+tx/rx; GTK will be tx or rx */
#define IEEE80211_IS_STATICKEY(k) \
    (((k)->wk_flags & (GRPXMIT|IEEE80211_KEY_RECV)) == \
     (GRPXMIT|IEEE80211_KEY_RECV))
    struct wq_softc *sc = vap->iv_ic->ic_softc;
    struct wq_hal_vap *hvap = WQ_VAP(vap)->mv_hvap;
    const struct ieee80211_cipher *cip = k->wk_cipher;
    const uint8_t *macaddr;
    WQ_HAL_KEYVAL hk;
    struct ieee80211_node *ni;
    int hash;
    struct ieee80211_node_table *nt = &vap->iv_ic->ic_sta;

    ni = ieee80211_find_txnode(vap, mac);
    if (ni == NULL) {
        iot_printf("ap selbs node macaddr:%pM\n",mac);
        hash = IEEE80211_NODE_HASH(nt->nt_ic, mac);
        LIST_FOREACH(ni, &nt->nt_hash[hash], ni_hash)
        {
            iot_printf("node macaddr in nt:%pM\n",ni->ni_macaddr);
        }
        IOT_ASSERT(0);
        return 0;
    }

#if 0
    KASSERT((k->wk_flags & IEEE80211_KEY_SWCRYPT) == 0,
        ("s/w crypto set?"));
#endif

    if (hvap == NULL) {
            /* XXX monitor mode? */
            DPRINTF(WDRV_DEBUG_KEYCACHE,
                "%s: no hvap for opmode %d\n", __func__,
                vap->iv_opmode);
            ieee80211_free_node(ni);
            return 0;
    }
    memset(&hk, 0, sizeof(hk));
    hk.keyIndex = k->wk_keyix;
    switch (cip->ic_cipher) {
    case IEEE80211_CIPHER_WEP:
#if 0 //USB project, from upper layer will pass broadcast addr
        /* skip key set for broadcast addr */
        if (ETHER_IS_BROADCAST(mac)) {
            ieee80211_free_node(ni);
            return 1;
        }
#endif
        hk.keyTypeId = KEY_TYPE_ID_WEP;
        hk.keyLen = k->wk_keylen;
        if (k->wk_keyix == vap->iv_def_txkey)
            hk.keyFlags = KEY_FLAG_WEP_TXKEY;

        if ((vap->iv_opmode == IEEE80211_M_HOSTAP) &&
            (!memcmp(mac, vap->iv_myaddr, IEEE80211_ADDR_LEN))) {
            hk.keyFlags |= KEY_FLAG_TXGROUPKEY;
        }
        break;
    case IEEE80211_CIPHER_TKIP:
        hk.keyTypeId = KEY_TYPE_ID_TKIP;
        hk.key.tkip.tsc.high = (uint32_t)(k->wk_keytsc >> 16);
        hk.key.tkip.tsc.low = (uint16_t)k->wk_keytsc;
        hk.keyFlags = KEY_FLAG_TSC_VALID | KEY_FLAG_MICKEY_VALID;
        hk.keyLen = k->wk_keylen + IEEE80211_MICBUF_SIZE;
        if (!addgroupflags(&hk, k)) {
            hk.keyFlags |= KEY_FLAG_PAIRWISE;
        }
        hk.keyId = ieee80211_crypto_get_keyid(vap, (struct ieee80211_key *)k);
        break;
    case IEEE80211_CIPHER_AES_CCM:
        hk.keyTypeId = KEY_TYPE_ID_AES;
        hk.keyLen = k->wk_keylen;
        if (!addgroupflags(&hk, k)) {
            hk.keyFlags |= KEY_FLAG_PAIRWISE;
        }
        hk.keyId = ieee80211_crypto_get_keyid(vap, (struct ieee80211_key *)k);
        break;
    default:
        /* XXX should not happen */
        DPRINTF(WDRV_DEBUG_KEYCACHE, "%s: unknown cipher %d\n",
            __func__, k->wk_cipher->ic_cipher);
        ieee80211_free_node(ni);
        return 0;
    }
    /*
     * NB: tkip mic keys get copied here too; the layout
     *     just happens to match that in ieee80211_key.
     */
    memcpy(hk.key.aes, k->wk_key, hk.keyLen);

    /*
     * Locate address of sta db entry for writing key;
     * the convention unfortunately is somewhat different
     * than how net80211, hostapd, and wpa_supplicant think.
     */
    if (vap->iv_opmode == IEEE80211_M_STA) {
        /*
         * NB: keys plumbed before the sta reaches AUTH state
         * will be discarded or written to the wrong sta db
         * entry because iv_bss is meaningless.  This is ok
         * (right now) because we handle deferred plumbing of
         * WEP keys when the sta reaches AUTH state.
         */
        macaddr = vap->iv_bss->ni_bssid;
        if ((k->wk_flags & IEEE80211_KEY_GROUP) == 0) {
            if (cip->ic_cipher == IEEE80211_CIPHER_WEP) {
                /* add as group key */
                WQ_HAL_KEYVAL ghk = hk;
                ghk.keyFlags |= (KEY_FLAG_RXGROUPKEY | KEY_FLAG_WEP_KEY_STA);
                htc_key_notify(sc, KEY_ADD, WQ_NODE_INVALID, (uint8_t *)&ghk, (uint8_t *)macaddr);
            }
        }
    } else if (vap->iv_opmode == IEEE80211_M_WDS &&
        vap->iv_state != IEEE80211_S_RUN) {
        /*
         * Prior to RUN state a WDS vap will not it's BSS node
         * setup so we will plumb the key to the wrong mac
         * address (it'll be our local address).  Workaround
         * this for the moment by grabbing the correct address.
         */
        macaddr = vap->iv_des_bssid;
    } else if ((k->wk_flags & GRPXMIT) == GRPXMIT)
        macaddr = vap->iv_myaddr;
    else
        macaddr = mac;
    KEYPRINTF(sc, &hk, macaddr);

    if ((k->wk_flags & IEEE80211_KEY_GROUP) == 0) {
        /* unicast key id */
        htc_key_notify(sc, KEY_ADD, ni->ni_mnid, (uint8_t *)&hk, (uint8_t *)macaddr);
    } else {
        /* multicast key id, used for AP only */
        if (vap->iv_opmode == IEEE80211_M_HOSTAP) {
            htc_key_notify(sc, KEY_ADD, vap->iv_bss->ni_mnid, (uint8_t *)&hk, (uint8_t *)macaddr);
        }
        else {
            htc_key_notify(sc, KEY_ADD, WQ_NODE_INVALID, (uint8_t *)&hk, (uint8_t *)macaddr);
        }
    }

    ieee80211_free_node(ni);
    return 1;
#undef IEEE80211_IS_STATICKEY
#undef GRPXMIT
}

static int
wq_mode_init(struct wq_softc *sc)
{
    //struct ieee80211com *ic = &sc->sc_ic;
    //struct wq_hal *mh = sc->sc_mh;

    //wq_hal_setpromisc(mh, ic->ic_promisc > 0);
    // wq_setmcastfilter(sc);

    return 0;
}

/*
 * Callback from the 802.11 layer to update the slot time
 * based on the current setting.  We use it to notify the
 * firmware of ERP changes and the f/w takes care of things
 * like slot time and preamble.
 */
static void
wq_updateslot(struct ieee80211com *ic)
{
    struct wq_softc *sc = ic->ic_softc;
    struct wq_hal *mh = sc->sc_mh;
    int prot;

    /* NB: can be called early; suppress needless cmds */
    if (!sc->sc_running)
        return;

    /*
     * Calculate the ERP flags.  The firwmare will use
     * this to carry out the appropriate measures.
     */
    prot = 0;
    if (IEEE80211_IS_CHAN_ANYG(ic->ic_curchan)) {
        if ((ic->ic_flags & IEEE80211_F_SHSLOT) == 0)
            prot |= IEEE80211_ERP_NON_ERP_PRESENT;
        if (ic->ic_flags & IEEE80211_F_USEPROT)
            prot |= IEEE80211_ERP_USE_PROTECTION;
        if (ic->ic_flags & IEEE80211_F_USEBARKER)
            prot |= IEEE80211_ERP_LONG_PREAMBLE;
    }

    DPRINTF(WDRV_DEBUG_RESET,
        "%s: chan %u MHz/flags 0x%x %s slot, (prot 0x%x ic_flags 0x%x)\n",
        __func__, ic->ic_curchan->ic_freq, ic->ic_curchan->ic_flags,
        ic->ic_flags & IEEE80211_F_SHSLOT ? "short" : "long", prot,
        ic->ic_flags);

    wq_hal_setslottime(mh, ic->ic_flags & IEEE80211_F_SHSLOT?9:20);

    wq_hal_setgprot(mh, prot);
}

/*
 * Setup the beacon frame.
 */
static int
wq_beacon_setup(struct ieee80211vap *vap)
{
    struct ieee80211_node *ni = vap->iv_bss;
    struct ieee80211com *ic = ni->ni_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct mbuf *m;

    m = ieee80211_beacon_alloc(ni);
    IOT_ASSERT(m);
    /* send beacon template to fw */
    htc_beacon_update_notify(sc, vap->iv_macid, ni->ni_mnid, m);
    m_freem(m);

    return 0;
}

/*
 * Update the beacon frame in response to a change.
 */
static void
wq_beacon_update(struct ieee80211vap *vap, int item)
{
    struct wq_hal_vap *hvap = WQ_VAP(vap)->mv_hvap;
    struct ieee80211com *ic = vap->iv_ic;

    KASSERT(hvap != NULL, ("no beacon"));
    switch (item) {
    case IEEE80211_BEACON_ERP:
        wq_updateslot(ic);
        break;
    case IEEE80211_BEACON_HTINFO:
        wq_hal_setnprotmode(hvap,
            MS(ic->ic_curhtprotmode, IEEE80211_HTINFO_OPMODE));
        break;
    case IEEE80211_BEACON_CAPS:
    case IEEE80211_BEACON_WME:
    case IEEE80211_BEACON_APPIE:
    case IEEE80211_BEACON_CSA:
    case IEEE80211_BEACON_CH:
        break;
    case IEEE80211_BEACON_TIM:
        /* NB: firmware always forms TIM */
        return;
    }
    /* XXX retain beacon frame and update */
    wq_beacon_setup(vap);
}



#ifdef WQ_HOST_PS_SUPPORT
/*
 * Handle power save station occupancy changes.
 */
static void
wq_update_ps(struct ieee80211vap *vap, int nsta)
{
    struct ieee80211com *ic = vap->iv_ic;
    struct wq_softc *sc = ic->ic_softc;

    htc_ps_notify(sc, vap->iv_macid, nsta);
}

/*
 * Handle associated station power save state changes.
 */
static int
wq_set_tim(struct ieee80211_node *ni, int set)
{
    struct ieee80211vap *vap = ni->ni_vap;
    struct ieee80211com *ic = vap->iv_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct wq_vap *mvp = WQ_VAP(vap);

    if (mvp->mv_set_tim(ni, set)) {     /* NB: state change */
        htc_tim_notify(sc, vap->iv_macid, set, IEEE80211_AID(ni->ni_associd));
        return 1;
    } else
        return 0;
}
#endif /* WQ_HOST_PS_SUPPORT */

//Setup Rx AMPDU HW registers, HAWK HW supports up to 8 Rx aggregation
static int wq_set_rx_ampdu(struct ieee80211_node *ni, uint8_t id, uint8_t tid, uint32_t start_seq, uint32_t window)
{
    struct ieee80211vap *vap = ni->ni_vap;
    struct wq_hal_vap *hvap = WQ_VAP(vap)->mv_hvap;

    DPRINTF(WDRV_DEBUG_AMPDU, "wq_set_rx_ampdu id=%d, tid=%d, mac=%X-%X-%X-%X-%X-%X, seq=%d, win=%d\n", id, tid,
        ni->ni_macaddr[0], ni->ni_macaddr[1], ni->ni_macaddr[2], ni->ni_macaddr[3], ni->ni_macaddr[4], ni->ni_macaddr[5],
        start_seq, window);

    wq_hal_set_rx_ampdu(hvap, id, tid, ni->ni_macaddr, start_seq, window);

    return 0;
}

//Reset Rx AMPDU HW registers
static int wq_reset_rx_ampdu(struct ieee80211_node *ni, uint8_t id)
{
    struct ieee80211vap *vap = ni->ni_vap;
    struct wq_hal_vap *hvap = WQ_VAP(vap)->mv_hvap;
    uint8_t null_addr[6] = {0, 0, 0, 0, 0, 0};

    DPRINTF(WDRV_DEBUG_AMPDU, "wq_reset_rx_ampdu id=%d, mac=%X-%X-%X-%X-%X-%X\n", id,
        ni->ni_macaddr[0], ni->ni_macaddr[1], ni->ni_macaddr[2], ni->ni_macaddr[3], ni->ni_macaddr[4], ni->ni_macaddr[5]);

    wq_hal_set_rx_ampdu(hvap, id, 0, null_addr, 0, 64);

    return 0;
}

static int
wq_ampdu_rx_start(struct ieee80211_node *ni, struct ieee80211_rx_ampdu *rap,
	int baparamset, int batimeout, int baseqctl)
{
    struct ieee80211com *ic = ni->ni_ic;
    struct wq_softc *sc = ic->ic_softc;
    int bufsiz = MS(baparamset, IEEE80211_BAPS_BUFSIZ);
    uint32_t start_seq;

    bufsiz = 64;
	start_seq = MS(baseqctl, IEEE80211_BASEQ_START);

    DPRINTF(WDRV_DEBUG_AMPDU, "wq_ampdu_rx_start id=%d, tid=%d, mac=%X-%X-%X-%X-%X-%X, seq=%d, win=%d\n", rap->rxa_id, MS(baparamset, IEEE80211_BAPS_TID),
        ni->ni_macaddr[0], ni->ni_macaddr[1], ni->ni_macaddr[2], ni->ni_macaddr[3], ni->ni_macaddr[4], ni->ni_macaddr[5],
        start_seq, bufsiz);

    wq_set_rx_ampdu(ni, rap->rxa_id, MS(baparamset, IEEE80211_BAPS_TID), start_seq, bufsiz);

    return sc->sc_ampdu_rx_start(ni, rap, baparamset, batimeout, baseqctl);
}

static void
wq_ampdu_rx_stop(struct ieee80211_node *ni, struct ieee80211_rx_ampdu *rap)
{
    struct ieee80211com *ic = ni->ni_ic;
    struct wq_softc *sc = ic->ic_softc;

    DPRINTF(WDRV_DEBUG_AMPDU, "wq_ampdu_rx_stop id=%d, mac=%X-%X-%X-%X-%X-%X\n", rap->rxa_id,
        ni->ni_macaddr[0], ni->ni_macaddr[1], ni->ni_macaddr[2], ni->ni_macaddr[3], ni->ni_macaddr[4], ni->ni_macaddr[5]);

    wq_reset_rx_ampdu(ni, rap->rxa_id);

    sc->sc_ampdu_rx_stop(ni, rap);
}

static struct ieee80211_node *
wq_node_alloc(struct ieee80211vap *vap, const uint8_t mac[IEEE80211_ADDR_LEN])
{
    struct wq_node *mn;
    struct wq_softc *sc = vap->iv_ic->ic_softc;
    uint8_t mnid;

    mnid = htc_node_notify(sc, NODE_ALLOC, 0, 0);
    if (mnid == WQ_NODE_INVALID) {
        /* fw node table full */
        iot_printf("wq_node_alloc WQ_NODE_INVALID\n");
    }

    mn = malloc(sizeof(struct wq_node), M_80211_NODE, M_NOWAIT|M_ZERO);
    if (mn == NULL) {
        htc_node_notify(sc, NODE_FREE, mnid, 0);
        return NULL;
    }
    os_mem_set(mn, 0, sizeof(struct wq_node));
    mn->mn_node.ni_mnid = mnid;
    if (mnid != WQ_NODE_INVALID) {
#if (WQ_CONFIG_64 == 1)
        sc->node_uid_table[mn->mn_node.ni_mnid] = (uint64_t)&mn->mn_node;
#else
        sc->node_uid_table[mn->mn_node.ni_mnid] = (uint32_t)&mn->mn_node;
#endif
    }

    return &mn->mn_node;
}

static void
wq_node_cleanup(struct ieee80211_node *ni)
{
}

static void
wq_node_free(struct ieee80211_node *ni)
{
    struct ieee80211com *ic = ni->ni_ic;
    struct wq_softc *sc = ic->ic_softc;

    if (ni->ni_mnid != WQ_NODE_INVALID) {
        htc_node_notify(sc, NODE_FREE, ni->ni_mnid, 0);
        sc->node_uid_table[ni->ni_mnid] = 0;
    }

    sc->sc_node_cleanup(ni);
    sc->sc_node_free(ni);
}

/*
 * Callback to reclaim resources.  We first let the
 * net80211 layer do it's thing, then if we are still
 * blocked by a lack of rx dma buffers we walk the ampdu
 * reorder q's to reclaim buffers by copying to a system
 * cluster.
 */
static void
wq_node_drain(struct ieee80211_node *ni)
{
    struct ieee80211com *ic = ni->ni_ic;
        struct wq_softc *sc = ic->ic_softc;
#ifdef WQ_DEBUG
    DPRINTF(WDRV_DEBUG_NODE, "%s: ni %p vap %p\n",
        __func__, ni, ni->ni_vap);
#endif
    /* NB: call up first to age out ampdu q's */
    sc->sc_node_drain(ni);
#if 0
    /* XXX better to not check low water mark? */
    if (sc->sc_rxblocked && mn->mn_staid != 0 &&
        (ni->ni_flags & IEEE80211_NODE_HT)) {
        uint8_t tid;
        /*
         * Walk the reorder q and reclaim rx dma buffers by copying
         * the packet contents into clusters.
         */
        for (tid = 0; tid < WME_NUM_TID; tid++) {
            struct ieee80211_rx_ampdu *rap;

            if (ni->ni_rx_ampdu[tid] != NULL)
            {
                rap = ni->ni_rx_ampdu[tid];
                if ((rap->rxa_flags & IEEE80211_AGGR_XCHGPEND) == 0)
                    continue;
                if (rap->rxa_qframes)
                    wq_ampdu_rxdma_reclaim(rap);
            }
        }
    }
#endif
}

static void
wq_node_getsignal(const struct ieee80211_node *ni, int8_t *rssi, int8_t *noise)
{
    *rssi = ni->ni_ic->ic_node_getrssi(ni);
#ifdef WQ_ANT_INFO_SUPPORT
#if 0
    /* XXX need to smooth data */
    *noise = -WQ_NODE_CONST(ni)->mn_ai.nf;
#else
    *noise = -95;       /* XXX */
#endif
#else
    *noise = -95;       /* XXX */
#endif
}

struct wq_frame_bar {
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_ra[IEEE80211_ADDR_LEN];
    u_int8_t    i_ta[IEEE80211_ADDR_LEN];
    /* ctl, seq, FCS */
} __packed;

void
wq_handlemicerror(struct ieee80211com *ic, const uint8_t *data)
{
    const struct ieee80211_frame *wh;
    struct ieee80211_node *ni;

    wh = (const struct ieee80211_frame *)data;
    ni = ieee80211_find_rxnode(ic, (const struct ieee80211_frame_min *) wh);
    if (ni != NULL) {
        struct ieee80211vap *vap = ni->ni_vap;
        struct ieee80211_key *k ;
        const struct ieee80211_cipher *cip ;

        if (IEEE80211_IS_MULTICAST(wh->i_addr1)) {
            k = &vap->iv_nw_keys[vap->iv_def_txkey];
        } else {
            k = &ni->ni_ucastkey;
        }
        cip = k->wk_cipher;

        if (cip && (cip->ic_cipher == IEEE80211_CIPHER_TKIP)) {
            ieee80211_notify_michael_failure(vap, wh, 0);
        }

        ieee80211_free_node(ni);
    }
}


static void
wq_txq_init(struct wq_softc *sc, struct wq_txq *txq, int qnum)
{
    //struct wq_txbuf *bf, *bn;
    //struct wq_txdesc *ds;

    //WQ_TXQ_LOCK_INIT(sc, txq);
    txq->qnum = qnum;

    STAILQ_INIT(&txq->active);
}

/*
 * Setup a hardware data transmit queue for the specified
 * access control.  We record the mapping from ac's
 * to h/w queues for use by wq_tx_start.
 */
static int
wq_tx_setup(struct wq_softc *sc, int ac, int mvtype)
{
    struct wq_txq *txq;

    txq = &sc->sc_txq[mvtype];
    wq_txq_init(sc, txq, mvtype);
    sc->sc_ac2q[ac] = txq;
    return 1;
}

/*
 * Update WME parameters for a transmit queue.
 */
static int
wq_txq_update(struct wq_softc *sc, int ac)
{
#define WQ_EXPONENT_TO_VALUE(v)    ((1<<v)-1)
    struct ieee80211com *ic = &sc->sc_ic;
    struct chanAccParams chp;
    struct wq_txq *txq = sc->sc_ac2q[ac];
    struct wmeParams *wmep;
    struct wq_hal *mh = sc->sc_mh;
    int aifs, cwmin, cwmax, txoplim;

    ieee80211_wme_ic_getparams(ic, &chp);
    wmep = &chp.cap_wmeParams[ac];

    aifs = wmep->wmep_aifsn;
    /* XXX in sta mode need to pass log values for cwmin/max */
    cwmin = WQ_EXPONENT_TO_VALUE(wmep->wmep_logcwmin);
    cwmax = WQ_EXPONENT_TO_VALUE(wmep->wmep_logcwmax);
    txoplim = wmep->wmep_txopLimit;     /* NB: units of 32us */
    if (wq_hal_setedcaparams(mh, txq->qnum, cwmin, cwmax, aifs, txoplim, ic->ic_flags & IEEE80211_F_SHSLOT?9:20)) {
        device_printf(sc->sc_dev, "unable to update hardware queue "
            "parameters for %s traffic!\n",
            ieee80211_wme_acnames[ac]);
        return 0;
    }
    return 1;
#undef WQ_EXPONENT_TO_VALUE
}

/*
 * Callback from the 802.11 layer to update WME parameters.
 */
static int
wq_wme_update(struct ieee80211com *ic)
{
    struct wq_softc *sc = ic->ic_softc;
    return !wq_txq_update(sc, WME_AC_BE) ||
        !wq_txq_update(sc, WME_AC_BK) ||
        !wq_txq_update(sc, WME_AC_VI) ||
        !wq_txq_update(sc, WME_AC_VO) ? EIO : 0;
}

/*
 * Reclaim resources for a setup queue.
 */
static void
wq_tx_cleanupq(struct wq_softc *sc, struct wq_txq *txq)
{
    /* XXX hal work? */
    //WQ_TXQ_LOCK_DESTROY(txq);
}

/*
 * Reclaim all tx queue resources.
 */
static void
wq_tx_cleanup(struct wq_softc *sc)
{
    int i;

    for (i = 0; i < WQ_NUM_TX_QUEUES; i++)
        wq_tx_cleanupq(sc, &sc->sc_txq[i]);
}

int wq_tx_sche(struct wq_softc *sc)
{
    struct mbuf *m;
    int i, qidx;
    int ac_prio[] = {WQ_WME_AC_VO, WQ_WME_AC_VI, WQ_WME_AC_BE, WQ_WME_AC_BK};

    mtx_lock(&sc->tx_sche_lock);
    /* send mbuf from high prio. queue to low */
    for (i = 0; i < WQ_NUM_EDCA_QUEUES; i++) {
        qidx = ac_prio[i];
        while ((m = mbufq_dequeue(&sc->sc_acq[qidx], &sc->wq_txq_lock)) != NULL) {
            /* stop pushing frame to fw if fw pending frame exceeds the threshold */
            if (atomic_read(&sc->sc_tx_fw_pending) >= WQ_MAX_FW_PENDING) {
                if (mbufq_enqueue(&sc->sc_acq[qidx], &sc->wq_txq_lock, m, QUEUE_HEAD)) {
                    struct ieee80211_node *ni = (struct ieee80211_node *)m->m_pkthdr.rcvif;
                    ieee80211_free_node(ni);
                    if(wq_dbg_flag & WQ_DEBUG_PKT_XMIT) {
                        iot_printf("%s: drop frame 1, len=%d\n", __func__, m->m_len);
                    }
                    m_freem(m);
                }
                mtx_unlock(&sc->tx_sche_lock);
                return 0;
            }
            if (wq_ipc_tx_pkt(sc->sc_wq_ipc, WQ_IPC_RCV_DEV, qidx,
                               0, (void *)m, m->m_data, m->m_len)) {
                /* send to ipc failed, drop */
                struct ieee80211_node *ni = (struct ieee80211_node *)m->m_pkthdr.rcvif;
                ieee80211_free_node(ni);
                if(wq_dbg_flag & WQ_DEBUG_PKT_XMIT) {
                    iot_printf("%s: drop frame 2, len=%d\n", __func__, m->m_len);
                }
                m_freem(m);
                mtx_unlock(&sc->tx_sche_lock);
                return 0;
            } else {
                atomic_inc(&sc->sc_tx_fw_pending);
                if((m->m_flags_ext & M_IS_ICMP) && (wq_dbg_flag & WQ_DEBUG_PING)) {
                    iot_printf("%s: The ping pkt(0x%04x) is trasmmitted to fw!\n", __func__, m->m_ping_seq);
                }
                else if(m->m_flags_ext & M_IS_AUTH)
                    iot_printf("%s: The AUTH is trasmmitted to fw!\n", __func__);
                else if(m->m_flags_ext & M_IS_ASSOC_RESP)
                    iot_printf("%s: The ASSOCRESP is trasmmitted to fw!\n", __func__);
                else if(m->m_flags_ext & M_IS_REASSOC_RESP)
                    iot_printf("%s: The REASSOCRESP is trasmmitted to fw!\n", __func__);

                //statistic pkt number of drv to fw
                atomic_add(1, &drv_fw_pkt_cnt);
            }
        }
    }

    mtx_unlock(&sc->tx_sche_lock);
    return 0;
}

void wq_compute_hw_padding(struct ieee80211com *ic,
                            const struct ieee80211_cipher *cip,
                            struct mbuf *m)
{
    IOT_ASSERT(m->m_padding == 0);

    if ((ic->ic_cryptocaps & (1 << cip->ic_cipher)) == 0) {
        /* use sw crypto, no hw padding */
        return;
    }

    switch (cip->ic_cipher) {
        case IEEE80211_CIPHER_WEP:
            /* reserve ICV */
            m->m_padding += cip->ic_trailer;
            break;
        case IEEE80211_CIPHER_TKIP:
            /* reserve MIC + ICV */
            m->m_padding += cip->ic_miclen + cip->ic_trailer;
            break;
        case IEEE80211_CIPHER_AES_CCM:
            /* reserve MIC */
            m->m_padding += cip->ic_trailer;
            break;
        default:
            IOT_ASSERT(0);
            break;
    }

#if defined(WIFI_SIM) || defined(WIFI_UART_SIM)
    m->m_len += m->m_padding;
#endif
}

#if 0
static int
FAST_ATTR wq_tx_start(struct wq_softc *sc, struct ieee80211_node *ni, struct wq_txbuf *bf,
    struct mbuf *m0)
{
    struct ieee80211vap *vap = ni->ni_vap;
    int iswep, ismcast;
    struct ieee80211_frame *wh;

#if WQ_TXDESC > 1
    int i;
#endif

    IOT_ASSERT(vap && ni);

    wh = mtod(m0, struct ieee80211_frame *);
    iswep = wh->i_fc[1] & IEEE80211_FC1_PROTECTED;
    ismcast = IEEE80211_IS_MULTICAST(wh->i_addr1);
#ifdef RISCV
    /* reserved 4 bytes for HW CRC */
    bf->bf_m->m_len += 4;
#endif
    bf->tid = ieee80211_gettid(wh);
    bf->seqno = M_SEQNO_GET(m0) << IEEE80211_SEQ_SEQ_SHIFT;

    if (iswep) {
        const struct ieee80211_cipher *cip;
        struct ieee80211_key *k;
        struct ieee80211com *ic = &sc->sc_ic;

        /*
         * Construct the 802.11 header+trailer for an encrypted
         * frame. The only reason this can fail is because of an
         * unknown or unsupported cipher/key type.
         *
         * NB: we do this even though the firmware will ignore
         *     what we've done for WEP and TKIP as we need the
         *     ExtIV filled in for CCMP and this also adjusts
         *     the headers which simplifies our work below.
         */
        k = ieee80211_crypto_encap(ni, m0);
        if (k == NULL) {
            /*
             * This can happen when the key is yanked after the
             * frame was queued.  Just discard the frame; the
             * 802.11 layer counts failures and provides
             * debugging/diagnostics.
             */
            m_freem(m0);
            return EIO;
        }
        /*
         * Adjust the packet length for the crypto additions
         * done during encap and any other bits that the f/w
         * will add later on.
         */
        cip = k->wk_cipher;
        wq_compute_hw_padding(ic, cip, m0);
        m0->m_keyid = ieee80211_crypto_get_keyid(vap, k);
    }

    /* handle for mcast packets queuing */
    if (ismcast && (vap->iv_ps_sta || sc->sc_mcast_swq.txq_depth)) {
        wq_disable_irq();
        wq_insert_mcast_txbuf(sc, bf, m0, 0);
        wq_enable_irq();
        return 0;
    }

    //insert txbuf into node txqueue
    wq_node_insert_txbuf(sc, ni, bf, m0, 0);

#ifdef TX_SCHE_DEBUG
    wq_node_txq_dump(sc, ni);
#endif

    wq_tx_sche(sc);

    return 0;
}
#endif


/*
 * Clear the transmit queues of any frames submitted for the
 * specified vap.  This is done when the vap is deleted so we
 * don't potentially reference the vap after it is gone.
 * Note we cannot remove the frames; we only reclaim the node
 * reference.
 */

static int
wq_recv_action(struct ieee80211_node *ni, const struct ieee80211_frame *wh,
    const uint8_t *frm, const uint8_t *efrm)
{
    struct wq_softc *sc = ni->ni_ic->ic_softc;
    const struct ieee80211_action *ia;

    ia = (const struct ieee80211_action *) frm;
    if (ia->ia_category == IEEE80211_ACTION_CAT_HT &&
        ia->ia_action == IEEE80211_ACTION_HT_MIMOPWRSAVE) {
        const struct ieee80211_action_ht_mimopowersave *mps =
            (const struct ieee80211_action_ht_mimopowersave *) ia;

        wq_hal_setmimops(sc->sc_mh, ni->ni_macaddr,
            mps->am_control & IEEE80211_A_HT_MIMOPWRSAVE_ENA,
            MS(mps->am_control, IEEE80211_A_HT_MIMOPWRSAVE_MODE));
        return 0;
    } else
        return sc->sc_recv_action(ni, wh, frm, efrm);
}

static int
wq_addba_request(struct ieee80211_node *ni, struct ieee80211_tx_ampdu *tap,
    int dialogtoken, int baparamset, int batimeout)
{
    struct wq_softc *sc = ni->ni_ic->ic_softc;
    int ac = TID_TO_WME_AC(tap->txa_tid);

    htc_tx_ampdu_notify(sc, AMPDU_TX_START, ni->ni_mnid, ac, ni->ni_htparam);
    return sc->sc_addba_request(ni, tap, dialogtoken, baparamset, batimeout);
}

static int
wq_addba_response(struct ieee80211_node *ni, struct ieee80211_tx_ampdu *tap,
    int code, int baparamset, int batimeout)
{
    struct wq_softc *sc = ni->ni_ic->ic_softc;
    int ac = TID_TO_WME_AC(tap->txa_tid);

    htc_tx_ampdu_notify(sc, AMPDU_TX_DONE, ni->ni_mnid, ac, code);
    return sc->sc_addba_response(ni, tap, code, baparamset, batimeout);
}

static void
wq_addba_response_timeout(struct ieee80211_node *ni,
    struct ieee80211_tx_ampdu *tap)
{
    struct wq_softc *sc = ni->ni_ic->ic_softc;
    int ac = TID_TO_WME_AC(tap->txa_tid);

    htc_tx_ampdu_notify(sc, AMPDU_TX_TIMEOUT, ni->ni_mnid, ac, 0);
    return sc->sc_addba_response_timeout(ni, tap);
}

static void
wq_addba_stop(struct ieee80211_node *ni, struct ieee80211_tx_ampdu *tap)
{
    struct wq_softc *sc = ni->ni_ic->ic_softc;

    htc_tx_ampdu_notify(sc, AMPDU_TX_STOP, ni->ni_mnid, TID_TO_WME_AC(tap->txa_tid), 0);
    sc->sc_addba_stop(ni, tap);
}


static WQ_HAL_APMODE
wq_getapmode(const struct ieee80211vap *vap, struct ieee80211_channel *chan)
{
    WQ_HAL_APMODE mode;

    if (IEEE80211_IS_CHAN_HT(chan)) {
        if (vap->iv_flags_ht & IEEE80211_FHT_PUREN)
            mode = AP_MODE_N_ONLY;
        else if (IEEE80211_IS_CHAN_5GHZ(chan))
            mode = AP_MODE_AandN;
        else if (vap->iv_flags & IEEE80211_F_PUREG)
            mode = AP_MODE_GandN;
        else
            mode = AP_MODE_BandGandN;
    } else if (IEEE80211_IS_CHAN_ANYG(chan)) {
        if (vap->iv_flags & IEEE80211_F_PUREG)
            mode = AP_MODE_G_ONLY;
        else
            mode = AP_MODE_MIXED;
    } else if (IEEE80211_IS_CHAN_B(chan))
        mode = AP_MODE_B_ONLY;
    else if (IEEE80211_IS_CHAN_A(chan))
        mode = AP_MODE_A_ONLY;
    else
        mode = AP_MODE_MIXED;       /* XXX should not happen? */
    return mode;
}

static int
wq_setapmode(struct ieee80211vap *vap, struct ieee80211_channel *chan)
{
    struct wq_hal_vap *hvap = WQ_VAP(vap)->mv_hvap;
    return wq_hal_setapmode(hvap, wq_getapmode(vap, chan));
}

void wq_channel_assess(int clear)
{
    uint32_t tx_ing, busy_rx_ing, idle_rx_ing, busy_rx_listen, idle_rx_listen, total;

    tx_ing = mac_cca_tx_ing_stat_sw_ro_mac_cca_tx_ing_stat_get();
    busy_rx_ing = mac_cca_busy_rx_ing_stat_sw_ro_mac_cca_busy_rx_ing_stat_get();
    idle_rx_ing = mac_cca_idle_rx_ing_stat_sw_ro_mac_cca_idle_rx_ing_stat_get();
    busy_rx_listen = mac_cca_busy_rx_listen_stat_sw_ro_mac_cca_busy_rx_listen_stat_get();
    idle_rx_listen = mac_cca_idle_rx_listen_stat_sw_ro_mac_cca_idle_rx_listen_stat_get();
    total = tx_ing + busy_rx_ing + idle_rx_ing + busy_rx_listen + idle_rx_listen;
	if (total != 0)
	{	
        iot_printf("Tx=%d%% Rx=%d%% Listen_Busy=%d%% Listen_Idle=%d%% \n", (tx_ing*100+50)/total,
            ((busy_rx_ing+idle_rx_ing)*100+50)/total, (busy_rx_listen*100+50)/total,
            (idle_rx_listen*100+50)/total);
	}

    if (clear) {
        mac_cca_stat_ctrl_sw_mac_cca_stat_clr_set(1);
    }
}

/*
 * Set/change channels.
 */
static int
wq_chan_set(struct wq_softc *sc, struct ieee80211_channel *chan)
{
    int ret;
    struct ieee80211com *ic = &sc->sc_ic;

    if (!sc->sc_running)
        return 0;
    else if(chan->ic_ieee == ic->ic_rf_chan && ic->sw_ch_cali)
    {
        iot_printf("[%s] No need to config the same channel again!\n", __func__);
        return 0;
    }
    else
    {
        DPRINTF(WDRV_DEBUG_RESET, "%s: chan %u MHz/flags 0x%x\n",
            __func__, chan->ic_freq, chan->ic_flags);
    	
    	//for cca debugging	: print cca idle/busy percentage
    	if(wq_dbg_flag & WQ_DEBUG_CCA)
            wq_channel_assess(0);

        if(chan->ic_maxregpower == 0)
            chan->ic_maxregpower = 20;

        iot_printf("wq_chan_set %d, rcnt=%d, rmngt_cnt=%d, rdata=%d, mac0 = %d, mac1 = %d, ieee_ch=%d, max_reg_pwr=%d, cali=%d\n",
            chan->ic_freq, rx_count, rx_mngt_count, rx_data_count, mac_rxr0_sts_8_sw_ro_rxr0_drop_mpdu_cnt_get(),
            mac_rxr1_sts_8_sw_ro_rxr1_drop_mpdu_cnt_get(), chan->ic_ieee, chan->ic_maxregpower, ic->sw_ch_cali);
        ret = htc_ch_bw_notify(sc, chan->ic_ieee, chan->ic_freq, chan->ic_maxregpower, ic->sw_ch_cali);
        ic->sw_ch_cali = 0;
        rx_count=0;
        rx_mngt_count=0;
        rx_data_count=0;

    	//for cca debugging	: reset cca counters
    	if(wq_dbg_flag & WQ_DEBUG_CCA)
    	    mac_cca_stat_ctrl_sw_mac_cca_stat_clr_set(1);

        IOT_ASSERT(ret == 0);
        return 0;
    }
}

static void
wq_scan_start(struct ieee80211com *ic)
{
    struct wq_softc *sc = (struct wq_softc *) ic->ic_softc;

    if ((hawk_vap0 && (hawk_vap0->iv_state == IEEE80211_S_RUN)) || (hawk_vap1 && (hawk_vap1->iv_state == IEEE80211_S_SCAN))) {

        if (hawk_vap0)
            DPRINTF(WDRV_DEBUG_STATE, "%s, stop sending beacon\n", __func__);
        htc_status_notify(sc, WQ_SCAN_START);

        /* for ap + sta, queue data during sta scanning */
        wq_tx_postpond_set(sc, WQ_POSTPOND_SCAN);
        return;
    }

    DPRINTF(WDRV_DEBUG_STATE, "%s\n", __func__);
}

static void
wq_scan_end(struct ieee80211com *ic)
{
    struct wq_softc *sc = (struct wq_softc *) ic->ic_softc;

    if (is_wq_tx_postpond(sc, WQ_POSTPOND_SCAN)) {
        DPRINTF(WDRV_DEBUG_STATE, "%s, start to send beacon\n", __func__);
        htc_status_notify(sc, WQ_SCAN_END);
        wq_tx_postpond_clr(sc, WQ_POSTPOND_SCAN);
        return;
    }

    DPRINTF(WDRV_DEBUG_STATE, "%s\n", __func__);
}

static void
wq_set_channel(struct ieee80211com *ic)
{
    struct wq_softc *sc = ic->ic_softc;

    wq_chan_set(sc, ic->ic_curchan);
}

static void
wq_update_chw(struct ieee80211com *ic)
{
#ifdef WQ_DEBUG
    struct ieee80211_channel *c = ic->ic_curchan;

    DPRINTF(WDRV_DEBUG_STATE, "wq_update_chw: flag: %x\n", c->ic_flags);
#endif
    wq_set_channel(ic);
}

/*
 * Handle a channel switch request.  We inform the firmware
 * and mark the global state to suppress various actions.
 * NB: we issue only one request to the fw; we may be called
 * multiple times if there are multiple vap's.
 */
static void
wq_startcsa(struct ieee80211vap *vap)
{
    struct ieee80211com *ic = vap->iv_ic;
    struct wq_softc *sc = ic->ic_softc;
    //WQ_HAL_CHANNEL hchan;

    if (sc->sc_csapending)
        return;

    //wq_mapchan(&hchan, ic->ic_csa_newchan);
    /* 1 =>'s quiet channel */
    wq_hal_setchannelswitchie(sc->sc_mh, ic->ic_csa_newchan, 1, ic->ic_csa_count);
    sc->sc_csapending = 1;
}

/*
 * Plumb any static WEP key for the station.  This is
 * necessary as we must propagate the key from the
 * global key table of the vap to each sta db entry.
 */
static void
wq_setanywepkey(struct ieee80211vap *vap, const uint8_t mac[IEEE80211_ADDR_LEN])
{
    if ((vap->iv_flags & (IEEE80211_F_PRIVACY|IEEE80211_F_WPA)) ==
        IEEE80211_F_PRIVACY &&
        vap->iv_def_txkey != IEEE80211_KEYIX_NONE &&
        vap->iv_nw_keys[vap->iv_def_txkey].wk_keyix != IEEE80211_KEYIX_NONE)
        (void) _wq_key_set(vap, &vap->iv_nw_keys[vap->iv_def_txkey],
                    mac);
}

static int
wq_peerstadb(struct ieee80211_node *ni, int aid, int staid, WQ_HAL_PEERINFO *pi)
{
#define WME(ie) ((const struct ieee80211_wme_info *) ie)
    struct ieee80211vap *vap = ni->ni_vap;


    if (1) {
        /*
         * Setup security for this station.  For sta mode this is
         * needed even though do the same thing on transition to
         * AUTH state because the call to wq_hal_newstation
         * clobbers the crypto state we setup.
         */
        wq_setanywepkey(vap, ni->ni_macaddr);
    }
    return 0;
#undef WME
}

static void
wq_setglobalkeys(struct ieee80211vap *vap)
{
    struct ieee80211_key *wk;

    wk = &vap->iv_nw_keys[0];
    for (; wk < &vap->iv_nw_keys[IEEE80211_WEP_NKID]; wk++)
        if (wk->wk_keyix != IEEE80211_KEYIX_NONE)
            (void) _wq_key_set(vap, wk, vap->iv_myaddr);
}

/*
 * Convert a legacy rate set to a firmware bitmask.
 */
static uint32_t
get_rate_bitmap(const struct ieee80211_rateset *rs)
{
    uint32_t rates;
    int i;

    rates = 0;
    for (i = 0; i < rs->rs_nrates; i++)
        switch (rs->rs_rates[i] & IEEE80211_RATE_VAL) {
        case 2:   rates |= 0x001; break;
        case 4:   rates |= 0x002; break;
        case 11:  rates |= 0x004; break;
        case 22:  rates |= 0x008; break;
        case 44:  rates |= 0x010; break;
        case 12:  rates |= 0x020; break;
        case 18:  rates |= 0x040; break;
        case 24:  rates |= 0x080; break;
        case 36:  rates |= 0x100; break;
        case 48:  rates |= 0x200; break;
        case 72:  rates |= 0x400; break;
        case 96:  rates |= 0x800; break;
        case 108: rates |= 0x1000; break;
        }
    return rates;
}

/*
 * Construct an HT firmware bitmask from an HT rate set.
 */
static uint32_t
get_htrate_bitmap(const struct ieee80211_htrateset *rs)
{
    uint32_t rates;
    int i;

    rates = 0;
    for (i = 0; i < rs->rs_nrates; i++) {
        if (rs->rs_rates[i] < 16)
            rates |= 1<<rs->rs_rates[i];
    }
    return rates;
}

/*
 * Craft station database entry for station.
 * NB: use host byte order here, the hal handles byte swapping.
 */
static WQ_HAL_PEERINFO *
mkpeerinfo(WQ_HAL_PEERINFO *pi, const struct ieee80211_node *ni)
{
    const struct ieee80211vap *vap = ni->ni_vap;

    memset(pi, 0, sizeof(*pi));
    pi->LegacyRateBitMap = get_rate_bitmap(&ni->ni_rates);
    pi->CapInfo = ni->ni_capinfo;
    if (ni->ni_flags & IEEE80211_NODE_HT) {
        /* HT capabilities, etc */
        pi->HTCapabilitiesInfo = ni->ni_htcap;
        /* XXX pi.HTCapabilitiesInfo */
            pi->MacHTParamInfo = ni->ni_htparam;
        pi->HTRateBitMap = get_htrate_bitmap(&ni->ni_htrates);
        pi->AddHtInfo.ControlChan = ni->ni_htctlchan;
        pi->AddHtInfo.AddChan = ni->ni_ht2ndchan;
        //pi->AddHtInfo.OpMode = ni->ni_htopmode;
        //pi->AddHtInfo.stbc = ni->ni_htstbc;

        /* constrain according to local configuration */
        if ((vap->iv_flags_ht & IEEE80211_FHT_SHORTGI40) == 0)
            pi->HTCapabilitiesInfo &= ~IEEE80211_HTCAP_SHORTGI40;
        if ((vap->iv_flags_ht & IEEE80211_FHT_SHORTGI20) == 0)
            pi->HTCapabilitiesInfo &= ~IEEE80211_HTCAP_SHORTGI20;
        if (ni->ni_chw != 40)
            pi->HTCapabilitiesInfo &= ~IEEE80211_HTCAP_CHWIDTH40;
    }
    return pi;
}

/*
 * Re-create the local sta db entry for a vap to ensure
 * up to date WME state is pushed to the firmware.  Because
 * this resets crypto state this must be followed by a
 * reload of any keys in the global key table.
 */
static int
wq_localstadb(struct ieee80211vap *vap)
{



    switch (vap->iv_opmode) {
    case IEEE80211_M_STA:

        if (1)
            wq_setglobalkeys(vap);
        break;
    case IEEE80211_M_HOSTAP:
    case IEEE80211_M_MBSS:

        if (1)
            wq_setglobalkeys(vap);
        break;
    default:
        break;
    }
    return 0;
}


static int
wq_newstate(struct ieee80211vap *vap, enum ieee80211_state nstate, int arg)
{
    struct wq_vap *mvp = WQ_VAP(vap);
    struct wq_hal_vap *hvap = mvp->mv_hvap;
    struct ieee80211com *ic = vap->iv_ic;
    struct ieee80211_node *ni = NULL;
    struct wq_softc *sc = ic->ic_softc;
    struct wq_hal *mh = sc->sc_mh;
    enum ieee80211_state ostate = vap->iv_state;
    int error;
    uint8_t mac0[6]={0};

    DPRINTF(WDRV_DEBUG_STATE, "%s: %s: %s -> %s\n",
        vap->iv_ifp->if_xname, __func__,
        ieee80211_state_name[ostate], ieee80211_state_name[nstate]);

    //wake up anyway, code below shall decide whether to go to sleep
    wq_set_power_state(sc, mvp, WQ_AWAKE_STATE, __func__, __LINE__);

    callout_stop_v2(&sc->sc_timer);
    /*
     * Clear current radar detection state.
     */
    if (ostate == IEEE80211_S_CAC) {
        /* stop quiet mode radar detection */
        wq_hal_setradardetection(mh, DR_CHK_CHANNEL_AVAILABLE_STOP);
    } else if (sc->sc_radarena) {
        /* stop in-service radar detection */
        wq_hal_setradardetection(mh, DR_DFS_DISABLE);
        sc->sc_radarena = 0;
    }
    /*
     * Carry out per-state actions before doing net80211 work.
     */
    if (nstate == IEEE80211_S_INIT ||(ostate == IEEE80211_S_RUN && nstate == IEEE80211_S_ASSOC)) {
        /* NB: only ap+sta vap's have a fw entity */
        if (hvap != NULL)
            wq_hal_stop(hvap);
        if (vap->iv_opmode == IEEE80211_M_HOSTAP) {
            wq_hal_clr_tbtt_timer(hvap);
        }
        if (vap->iv_opmode == IEEE80211_M_STA && ostate == IEEE80211_S_RUN) {
            iot_printf("htc_dev_info_notify AP_MAC_INFO 0\n");
            htc_dev_info_notify(sc, AP_MAC_INFO, mac0, 0);
        }
    } else if (nstate == IEEE80211_S_SCAN) {
        wq_hal_start(hvap);
        /* NB: this disables beacon frames */
        wq_hal_setinframode(hvap);
    } else if (nstate == IEEE80211_S_AUTH) {
        /*
         * Must create a sta db entry in case a WEP key needs to
         * be plumbed.  This entry will be overwritten if we
         * associate; otherwise it will be reclaimed on node free.
         */
        ni = vap->iv_bss;
        WQ_NODE(ni)->mn_hvap = hvap;
        (void) wq_peerstadb(ni, 0, 0, NULL);
    } else if (nstate == IEEE80211_S_CSA) {
        /* XXX move to below? */
        if (vap->iv_opmode == IEEE80211_M_HOSTAP ||
            vap->iv_opmode == IEEE80211_M_MBSS)
            wq_startcsa(vap);
    } else if (nstate == IEEE80211_S_CAC) {
        /* XXX move to below? */
        /* stop ap xmit and enable quiet mode radar detection */
        wq_hal_setradardetection(mh, DR_CHK_CHANNEL_AVAILABLE_START);
    }

    /*
     * Invoke the parent method to do net80211 work.
     */
    error = mvp->mv_newstate(vap, nstate, arg);

    /*
     * Carry out work that must be done after net80211 runs;
     * this work requires up to date state (e.g. iv_bss).
     */
    if (error == 0 && nstate == IEEE80211_S_RUN) {
        /* NB: collect bss node again, it may have changed */
        ni = vap->iv_bss;

        DPRINTF(WDRV_DEBUG_STATE,
            "%s: %s(RUN): iv_flags 0x%08x bintvl %d bssid %s "
            "capinfo 0x%04x chan %d\n",
            vap->iv_ifp->if_xname, __func__, vap->iv_flags,
            ni->ni_intval, ether_sprintf(ni->ni_bssid), ni->ni_capinfo,
            ieee80211_chan2ieee(ic, ic->ic_curchan));

        /*
         * Recreate local sta db entry to update WME/HT state.
         */
        wq_localstadb(vap);
        switch (vap->iv_opmode) {
        case IEEE80211_M_HOSTAP:
        case IEEE80211_M_MBSS:
            if (ostate == IEEE80211_S_CAC) {
                /* enable in-service radar detection */
                wq_hal_setradardetection(mh,
                    DR_IN_SERVICE_MONITOR_START);
                sc->sc_radarena = 1;
            }
            /*
             * Allocate and setup the beacon frame
             * (and related state).
             */
            error = wq_reset_vap(vap, IEEE80211_S_RUN);
            if (error != 0) {
                DPRINTF(WDRV_DEBUG_STATE,
                    "%s: beacon setup failed, error %d\n",
                    __func__, error);
                goto bad;
            }
            /* NB: must be after setting up beacon */
            wq_hal_start(hvap);

            /* init beacon interval and preTBTT related registers */
            wq_hal_set_beacon_interval(hvap, ni->ni_intval);
            wq_hal_set_tbtt_timer(hvap, ni->ni_intval, WQ_PRE_TBTT_OFFSET, WQ_POST_TBTT_OFFSET);
            wq_hal_set_bcn_ts_delta(hvap, WQ_BCN_TS_DELTA);
#if defined(WIFI_SIM) || defined(WIFI_UART_SIM)
            callout_init_v2(&sc->mh_sim_swba_timer, wq_beacon_proc, sc);
			iot_printf("init timer %s %p\n", "wq_beacon_proc", wq_beacon_proc);
            callout_reset_v2(&sc->mh_sim_swba_timer, 5000);
#endif
            if (hawk_vap0 && (hawk_vap0->iv_state == IEEE80211_S_RUN)) {
                wq_beacon_update(hawk_vap0, IEEE80211_BEACON_CH);
            }
            break;
        case IEEE80211_M_STA:
            DPRINTF(WDRV_DEBUG_STATE, "%s: %s: aid 0x%x\n",
                vap->iv_ifp->if_xname, __func__, ni->ni_associd);
            /*
             * Set state now that we're associated.
             *
             * XXX Dont need to do this (and others) if we've transitioned
             * from SLEEP->RUN.
             */
            if (ostate != IEEE80211_S_SLEEP)
            {
                wq_hal_setassocid(hvap, ni->ni_bssid, ni->ni_associd);
                //wq_setrates(vap);
                //wq_hal_setrtsthreshold(hvap, vap->iv_rtsthreshold);
            }
            /* init beacon interval and preTBTT related registers */
            wq_hal_set_beacon_interval(hvap, ni->ni_intval);
            wq_hal_set_tbtt_timer(hvap, ni->ni_intval, WQ_PRE_TBTT_OFFSET, WQ_POST_TBTT_OFFSET);
            htc_dev_info_notify(sc, AP_MAC_INFO, ni->ni_macaddr, ni->ni_mnid);

            /* Since ap channel follows sta interface, update vap0 channel info/beacon content
             * when sta interface connedted.
             */
            if (hawk_vap0 && (hawk_vap0->iv_state == IEEE80211_S_RUN)) {
                hawk_vap0->iv_bss->ni_chan = hawk_vap0->iv_des_chan = vap->iv_bss->ni_chan;
                wq_beacon_update(hawk_vap0, IEEE80211_BEACON_CH);
            }
            break;
        //case IEEE80211_M_WDS:
        //    DPRINTF(WDRV_DEBUG_STATE, "%s: %s: bssid %s\n",
        //        vap->iv_ifp->if_xname, __func__,
        //        ether_sprintf(ni->ni_bssid));
        //    wq_seteapolformat(vap);
        //    break;
        default:
            break;
        }
        /*
         * Set CS mode according to operating channel;
         * this mostly an optimization for 5GHz.
         *
         * NB: must follow wq_hal_start which resets csmode
         */
        //if (IEEE80211_IS_CHAN_5GHZ(ic->ic_bsschan))
        //  wq_hal_setcsmode(mh, CSMODE_AGGRESSIVE);
        //else
        //  wq_hal_setcsmode(mh, CSMODE_AUTO_ENA);

        /*
         * Start timer to prod firmware.
         */
        if (sc->sc_ageinterval != 0)
            callout_reset_v2(&sc->sc_timer, sc->sc_ageinterval*hz);
    } else if (nstate == IEEE80211_S_SLEEP) {
        /* XXX set chip in power save */
        wq_set_power_state(sc, mvp, WQ_SLEEP_STATE, __func__, __LINE__);
    }
bad:
    return error;
}

/*
 * Setup driver-specific state for a newly associated node.
 * Note that we're called also on a re-associate, the isnew
 * param tells us if this is the first time or not.
 */
static void
wq_newassoc(struct ieee80211_node *ni, int isnew)
{
    struct ieee80211vap *vap = ni->ni_vap;
    struct wq_softc *sc = vap->iv_ic->ic_softc;
    struct wq_node *mn = WQ_NODE(ni);
    WQ_HAL_PEERINFO pi;
    uint16_t aid;
    int error;

    aid = IEEE80211_AID(ni->ni_associd);
    if (isnew) {
        mn->mn_hvap = WQ_VAP(vap)->mv_hvap;
    } else {
        mn = WQ_NODE(ni);
        /* XXX reset BA stream? */
    }
    DPRINTF(WDRV_DEBUG_NODE, "%s: mac %s isnew %d aid %d \n",
        __func__, ether_sprintf(ni->ni_macaddr), isnew, aid);
    error = wq_peerstadb(ni, aid, 0, mkpeerinfo(&pi, ni));
    if (error != 0) {
        DPRINTF(WDRV_DEBUG_NODE,
            "%s: error %d creating sta db entry\n",
            __func__, error);
        /* XXX how to deal with error? */
    }
    htc_init_node_notify(sc, ni);
}

/*
 * Periodically poke the firmware to age out station state
 * (power save queues, pending tx aggregates).
 */
static void
wq_agestations(void *arg)
{
    struct wq_softc *sc = arg;

    if (sc->sc_ageinterval != 0)        /* NB: catch dynamic changes */
        callout_reset_v2(&sc->sc_timer, sc->sc_ageinterval*hz);
}

static int
wq_setregdomain(struct ieee80211com *ic, struct ieee80211_regdomain *rd,
    int nchan, struct ieee80211_channel chans[])
{
    struct wq_softc *sc = ic->ic_softc;
    struct wq_hal *mh = sc->sc_mh;
    const WQ_HAL_CHANNELINFO *ci;
    int i;

    for (i = 0; i < nchan; i++) {
        struct ieee80211_channel *c = &chans[i];

        if (IEEE80211_IS_CHAN_2GHZ(c)) {
            wq_hal_getchannelinfo(mh, WQ_FREQ_BAND_2DOT4GHZ,
                IEEE80211_IS_CHAN_HT40(c) ?
                WQ_CH_40_MHz_WIDTH : WQ_CH_20_MHz_WIDTH, &ci);
        } else if (IEEE80211_IS_CHAN_5GHZ(c)) {
            wq_hal_getchannelinfo(mh, WQ_FREQ_BAND_5GHZ,
                IEEE80211_IS_CHAN_HT40(c) ?
                WQ_CH_40_MHz_WIDTH : WQ_CH_20_MHz_WIDTH, &ci);
        } else {
            device_printf(sc->sc_dev,
                "%s: channel %u freq %u/0x%x not 2.4/5GHz\n",
                __func__, c->ic_ieee, c->ic_freq, c->ic_flags);
            return EINVAL;
        }

    }
    return 0;
}

#define IEEE80211_CHAN_HTG  (IEEE80211_CHAN_HT20|IEEE80211_CHAN_G)
#define IEEE80211_CHAN_HTA  (IEEE80211_CHAN_HT|IEEE80211_CHAN_A)

static void
addht40channels(struct ieee80211_channel chans[], int maxchans, int *nchans,
    const WQ_HAL_CHANNELINFO *ci, int flags)
{
    int i, error;

    for (i = 0; i < ci->nchannels; i++) {
        const struct wq_hal_channel *hc = &ci->channels[i];

        error = ieee80211_add_channel_ht40(chans, maxchans, nchans,
            hc->ieee, hc->maxTxPow, flags);
        if (error != 0 && error != ENOENT)
            break;
    }
}

static void
addchannels(struct ieee80211_channel chans[], int maxchans, int *nchans,
    const WQ_HAL_CHANNELINFO *ci, const uint8_t bands[])
{
    int i, error;

    error = 0;
    for (i = 0; i < ci->nchannels && error == 0; i++) {
        const struct wq_hal_channel *hc = &ci->channels[i];

        error = ieee80211_add_channel(chans, maxchans, nchans,
            hc->ieee, hc->maxTxPow, 0, bands);
    }
}

static void
getchannels(struct wq_softc *sc, int maxchans, int *nchans,
    struct ieee80211_channel chans[])
{
    const WQ_HAL_CHANNELINFO *ci;
    uint8_t bands[IEEE80211_MODE_BYTES];

    /*
     * Use the channel info from the hal to craft the
     * channel list.  Note that we pass back an unsorted
     * list; the caller is required to sort it for us
     * (if desired).
     */
    *nchans = 0;
    if (wq_hal_getchannelinfo(sc->sc_mh,
        WQ_FREQ_BAND_2DOT4GHZ, WQ_CH_20_MHz_WIDTH, &ci) == 0) {
        memset(bands, 0, sizeof(bands));
        setbit(bands, IEEE80211_MODE_11B);
        setbit(bands, IEEE80211_MODE_11G);
        setbit(bands, IEEE80211_MODE_11NG);
        addchannels(chans, maxchans, nchans, ci, bands);
    }
    if (wq_hal_getchannelinfo(sc->sc_mh,
        WQ_FREQ_BAND_5GHZ, WQ_CH_20_MHz_WIDTH, &ci) == 0) {
        memset(bands, 0, sizeof(bands));
        setbit(bands, IEEE80211_MODE_11A);
        setbit(bands, IEEE80211_MODE_11NA);
        addchannels(chans, maxchans, nchans, ci, bands);
    }
    if (wq_hal_getchannelinfo(sc->sc_mh,
        WQ_FREQ_BAND_2DOT4GHZ, WQ_CH_40_MHz_WIDTH, &ci) == 0)
        addht40channels(chans, maxchans, nchans, ci, IEEE80211_CHAN_HTG);
    if (wq_hal_getchannelinfo(sc->sc_mh,
        WQ_FREQ_BAND_5GHZ, WQ_CH_40_MHz_WIDTH, &ci) == 0)
        addht40channels(chans, maxchans, nchans, ci, IEEE80211_CHAN_HTA);
}

static void
wq_getradiocaps(struct ieee80211com *ic,
    int maxchans, u_int *nchans, struct ieee80211_channel chans[])
{
    struct wq_softc *sc = ic->ic_softc;

    getchannels(sc, maxchans, (int *)nchans, chans);
}

static int
wq_getchannels(struct wq_softc *sc)
{
    struct ieee80211com *ic = &sc->sc_ic;

    /*
     * Use the channel info from the hal to craft the
     * channel list for net80211.  Note that we pass up
     * an unsorted list; net80211 will sort it for us.
     */
    memset(ic->ic_channels, 0, sizeof(ic->ic_channels));
    ic->ic_nchans = 0;
    getchannels(sc, IEEE80211_CHAN_MAX, &ic->ic_nchans, ic->ic_channels);

    ic->ic_regdomain.regdomain = SKU_DEBUG;
    ic->ic_regdomain.country = CTRY_DEFAULT;
    ic->ic_regdomain.location = 'I';
    ic->ic_regdomain.isocc[0] = ' ';    /* XXX? */
    ic->ic_regdomain.isocc[1] = ' ';
    return (ic->ic_nchans == 0 ? EIO : 0);
}
#undef IEEE80211_CHAN_HTA
#undef IEEE80211_CHAN_HTG


ISR_ATTR static void wq_set_power_state(struct wq_softc *sc, struct wq_vap *mvp, int new_power_state, const char *func, int line)
{
    int hw_power_state;
    struct wq_hal_vap *hvap = mvp->mv_hvap;

    device_printf(sc->sc_dev, "%s:%d wq_set_power_state %d -> %d\n", func, line, mvp->wq_vap_power_state, new_power_state);

    if (mvp->wq_vap_power_state != new_power_state)
    {
        if ((mvp->wq_vap_power_state == WQ_AWAKE_STATE) && (new_power_state == WQ_SLEEP_STATE))
        {
            hw_power_state = HAL_HW_SLEEP;
        }
        else if ((mvp->wq_vap_power_state == WQ_SLEEP_STATE) && (new_power_state == WQ_AWAKE_STATE))
        {
            hw_power_state = HAL_HW_AWAKE;
        }
        else if ((mvp->wq_vap_power_state == WQ_DTIM_WAKEUP_STATE) && (new_power_state == WQ_AWAKE_STATE))
        {
            hw_power_state = HAL_HW_AWAKE;
        }
        else if ((mvp->wq_vap_power_state == WQ_DTIM_WAKEUP_STATE) && (new_power_state == WQ_SLEEP_STATE))
        {
            hw_power_state = HAL_HW_SLEEP;
        }
        else if ((mvp->wq_vap_power_state == WQ_SLEEP_STATE) && (new_power_state == WQ_DTIM_WAKEUP_STATE))
        {
            hw_power_state = HAL_HW_AWAKE;
        }
        else
        {
            //invalid state transition : from WQ_AWAKE_STATE tp WQ_DTIM_WAKEUP_STATE
            IOT_ASSERT(0);
            hw_power_state = HAL_HW_AWAKE;
        }

        mvp->wq_vap_power_state = new_power_state;

        wq_hal_set_power_state(hvap, hw_power_state);
    }

}

void wq_received_group_data_with_more_bit_zeo(struct ieee80211vap *vap)
{
    struct wq_vap *mvp = WQ_VAP(vap);
    struct wq_softc *sc = vap->iv_ic->ic_softc;

    //if VAP in STA mode and power state == DTIM_WAKEUP
    //now entering SLEEP state
    if (mvp->wq_vap_power_state == WQ_DTIM_WAKEUP_STATE)
    {
        wq_set_power_state(sc, mvp, WQ_SLEEP_STATE, __func__, __LINE__);
    }
}

void wq_received_beacon_tim_group_bit(struct ieee80211vap *vap, uint32_t group_bit)
{
    //if thre is no activity for a while in STA mode, send keep-alive(null data) to AP,
    //since some AP may deauth iactivity STA without notification

    if (((uint32_t)ticks - (uint32_t)vap->iv_ic->ic_lastdata) > WQ_HAL_KEEP_ALIVE_THRESHOLD)
    {
        struct ieee80211_node	*ni = vap->iv_bss;
        DPRINTF(WDRV_DEBUG_INFO, "STA mode keep-alive %x-%x-%x-%x-%x-%x\n",
                ni->ni_macaddr[0], ni->ni_macaddr[1], ni->ni_macaddr[2], ni->ni_macaddr[3], ni->ni_macaddr[4], ni->ni_macaddr[5]);
        ieee80211_send_nulldata(ieee80211_ref_node(ni));
        vap->iv_ic->ic_lastdata = ticks;
    }
}

void wq_erp_update(struct ieee80211_node *ni)
{
    struct ieee80211com	*ic = ni->ni_ic;
    struct wq_softc *sc = (struct wq_softc *)ic->ic_softc;

    htc_node_notify(sc, NODE_UPDATE, ni->ni_mnid, ni->ni_erp);
}

ISR_ATTR int is_wmm_running(struct ieee80211vap *vap)
{
    struct wq_softc *sc;

    if (vap->iv_opmode != IEEE80211_M_STA) {
        return 0;
    }

    sc = vap->iv_ic->ic_softc;

    return sc->sc_wmm_running;
}

int is_wmm_hi_ac(struct ieee80211vap *vap, int ac)
{
    struct wq_softc *sc = vap->iv_ic->ic_softc;
    return (ac == sc->sc_wmm_hi_ac);
}

static void
wq_parent(struct ieee80211com *ic)
{
    struct wq_softc *sc = ic->ic_softc;
    int startall = 0;

    WQ_LOCK(sc);
    if (ic->ic_nrunning > 0) {
        if (sc->sc_running) {
            /*
             * To avoid rescanning another access point,
             * do not call wq_init() here.  Instead,
             * only reflect promisc mode settings.
             */
            wq_mode_init(sc);
        } else {
            /*
             * Beware of being called during attach/detach
             * to reset promiscuous mode.  In that case we
             * will still be marked UP but not RUNNING.
             * However trying to re-init the interface
             * is the wrong thing to do as we've already
             * torn down much of our state.  There's
             * probably a better way to deal with this.
             */
             wq_init(sc);   /* XXX lose error */
             startall = 1;
        }
    } else{
        wq_stop(sc);
    }
    WQ_UNLOCK(sc);
    if (startall)
        ieee80211_start_all(ic);
}

static int
wq_ioctl(struct ieee80211com *ic, u_long cmd, void *data)
{
    int error = 0;

    switch (cmd) {
    default:
        error = ENOTTY;
        break;
    }
    return (error);
}

/*
 * Announce various information on device/driver attach.
 */
static void
wq_announce(struct wq_softc *sc)
{

    //device_printf(sc->sc_dev, "Rev A%d hardware(regioncode %d)\n",
    //    sc->sc_hwspecs.hwVersion,
    //    sc->sc_hwspecs.regionCode);

}




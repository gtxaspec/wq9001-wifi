/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2007-2009 Sam Leffler, Errno Consulting
 * Copyright (c) 2007-2009 Marvell Semiconductor, Inc.
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
 *
 * $FreeBSD$
 */


#ifndef _DEV_WQ_MVVAR_H
#define _DEV_WQ_MVVAR_H


#include "if_hawk_port.h"
#include <net80211/ieee80211_radiotap.h>
#include <hawkhal.h>


#include "wq_hw_config.h"

/*
 * How 'busy' to try and keep the hardware txq
 */
#define WQ_AGGR_MIN_QDEPTH     2
#define WQ_NONAGGR_MIN_QDEPTH      8

#ifndef WQ_TXBUF_MGMT
#define WQ_TXBUF_MGMT      8
#endif

#ifndef WQ_TXBUF_DATA
#define WQ_TXBUF_DATA  38      /* number of TX descriptors/buffers */ /* shall be equal to mbuf number */
#endif

#ifndef WQ_RXDESC
#define WQ_RXDESC  8       /* number of RX descriptors */
#endif
#ifndef WQ_RXBUF
#define WQ_RXBUF   ((5*WQ_RXDESC)/2)/* number of RX dma buffers */
#endif
#ifndef WQ_MAXBA
#define WQ_MAXBA   2       /* max BA streams/sta */
#endif

#ifdef WQ_SGDMA_SUPPORT
#define WQ_TXDESC  6       /* max tx descriptors/segments */
#else
#define WQ_TXDESC  1       /* max tx descriptors/segments */
#endif
#ifndef WQ_AGGR_SIZE
#define WQ_AGGR_SIZE   3839        /* max tx aggregation size */
#endif
#define WQ_AGEINTERVAL 1       /* poke f/w every sec to age q's */
#define WQ_MAXSTAID    8      /* max of 64 stations */

#ifndef WQ_NUM_MAXNODE
#define WQ_NUM_MAXNODE  8
#endif

#define WQ_RX_AGGR_MASK    0x7
#define WQ_RX_AGGR_FIRST   0x1
#define WQ_RX_AGGR_MID     0x2
#define WQ_RX_AGGR_LAST    0x3
#define WQ_RX_AGGR_FST_LST 0x4

#define WQ_NODE_INVALID    0xff

#define WQ_NODE_TABLE_SZ   8  /* 8 for HAWK-USB(32 for HAWK) : entries for cipher table */



#define WQ_AWAKE_STATE          0
#define WQ_SLEEP_STATE          1
#define WQ_DTIM_WAKEUP_STATE    2

#define WQ_HAL_KEEP_ALIVE_THRESHOLD     20000 //keep alive threshold : 20 seconds

#define WQ_MAX_FW_PENDING       36

enum wq_postpond_type {
    WQ_POSTPOND_KEY    = 0x1,
    WQ_POSTPOND_SCAN   = 0x2,
};

/*
 * Software backed version of tx/rx descriptors.  We keep
 * the software state out of the h/w descriptor structure
 * so that may be allocated in uncached memory w/o paying
 * performance hit.
 */
struct wq_txbuf {
    STAILQ_ENTRY(wq_txbuf) bf_list;
    void        *bf_desc;   /* h/w descriptor */
    struct mbuf *bf_m;
    //  struct ieee80211_node *bf_node;                 //Save memory, use bf_m->m_pkthdr.rcvif
    struct wq_txq  *bf_txq;        /* backpointer to tx q/ring */
    uint8_t  is_mgmt:1,   /* mgmt or data */
             retry_cnt:3;    /* retry counter for this txbuf, number will be 0~7 */
    uint8_t tid;
    uint16_t seqno;
};
typedef STAILQ_HEAD(, wq_txbuf) wq_txbufhead;


/*
 * TX/RX ring definitions.  There are 4 tx rings, one
 * per AC, and 1 rx ring.  Note carefully that transmit
 * descriptors are treated as a contiguous chunk and the
 * firmware pre-fetches descriptors.  This means that we
 * must preserve order when moving descriptors between
 * the active+free lists; otherwise we may stall transmit.
 */
struct wq_txq {
    //struct wq_descdma dma;       /* bus dma resources */
    //struct mtx      lock;       /* tx q lock */
    //char          name[12];   /* e.g. "wq0_txq4" */
    uint8_t             qnum;       /* hw q number */
    //int       txpri;      /* f/w tx priority */
    //int       nfree;      /* # buffers on free list */
    //wq_txbufhead free;       /* queue of free buffers */
    wq_txbufhead   active;     /* queue of active buffers */
    uint8_t           active_depth;   /* active buffer depth, node txq only */
    uint8_t           txq_depth;  /* queue depth (stat only) */
    uint8_t           txq_aggr_depth; /* how many aggregates are queued */
};






#define WQ_BA_INDEX(_st, _seq) (((_seq) - (_st)) & (IEEE80211_SEQ_RANGE - 1))
#define WQ_TID_MAX_BUFS        (2 * IEEE80211_AGGR_BAWMAX)
#define SEQNO(_a)               ((_a) >> IEEE80211_SEQ_SEQ_SHIFT)
#define INCR(_l, _sz)           (_l) ++; (_l) &= ((_sz) - 1)

struct wq_tid {
    /* active tx buffers, beginning at current BAW */
    struct wq_txbuf *tx_buf[WQ_TID_MAX_BUFS];
    /* where the baw head is in the array */
    int baw_head;
    /* where the BAW tail is in the array */
    int baw_tail;
};

/* driver-specific node state */
struct wq_node {
    struct ieee80211_node mn_node;  /* base class */
    //struct wq_ant_info mn_ai;  /* antenna info */
    //uint32_t    mn_avgrssi; /* average rssi over all rx frames */
    //uint16_t    mn_staid;   /* firmware station id */
    //struct wq_bastate mn_ba[WQ_MAXBA];
    struct wq_hal_vap *mn_hvap;    /* hal vap handle */
    struct wq_txq      node_txq[WQ_NUM_TX_QUEUES];
    uint16_t    start_seq[WQ_NUM_TX_QUEUES]; /* Next sequence number need to be acked */
    uint8_t mn_is_powersave;    /* node is sleeping */
    uint8_t mn_is_addba_paused; /* bitmap for TxQ for each AC, 1:paused due to addba request handshaking */
    uint8_t uid;    /* unique node_id */
    uint8_t node_id; /* node table id */
    uint8_t encrypt;
    //struct wq_node_rc_table rc_table;
    struct wq_tid *mn_ac[WME_NUM_AC]; /* per-TID state */
};
#define WQ_NODE(ni)        ((struct wq_node *)(ni))
#define WQ_NODE_CONST(ni)  ((const struct wq_node *)(ni))

/* Tx Schdule information for each AC QUEUE */
struct wq_node_tx_sche{
    struct mtx  lock;
    uint8_t  last_scheduled_node_idx;
    uint32_t nid_bitmap;
};

struct wq_bcn_info_t {
    struct mbuf *buf;
};

/*
 * Driver-specific vap state.
 */
struct wq_vap {
    struct ieee80211vap mv_vap;     /* base class */
    struct wq_hal_vap *mv_hvap;        /* hal vap handle */

    uint8_t wq_vap_power_state;

    struct wq_bcn_info_t beacon_info;
    uint16_t    mv_last_ps_sta;     /* last count of ps sta's */
    //uint16_t    mv_eapolformat;     /* fixed tx rate for EAPOL */
    int     (*mv_newstate)(struct ieee80211vap *,
                    enum ieee80211_state, int);
    int     (*mv_set_tim)(struct ieee80211_node *, int);
};
#define WQ_VAP(vap)    ((struct wq_vap *)(vap))
#define WQ_VAP_CONST(vap)  ((const struct wq_vap *)(vap))

struct wq_aggr_stat {
    uint32_t tx_ampdu[HW_AMPDU_MAX_SUBFRAME_NUM + 1];
    uint32_t rx_ampdu[HW_AMPDU_MAX_SUBFRAME_NUM + 1];
};

struct wq_softc {
    struct ieee80211com sc_ic;
    struct mbufq        sc_acq[WQ_NUM_EDCA_QUEUES];
    //struct wq_stats    sc_stats;   /* interface statistics */
    //int                 sc_debug;
    //int                 sc_powerstate;

    device_t        sc_dev;
    void            *sc_wq_ipc;

    struct mtx      sc_mtx;     /* master lock (recursive) */
    //struct workqueue_struct   *sc_tq;     /* private task queue */
    //int         sc_tx_timer;
    unsigned int        sc_running : 1,
                //sc_invalid : 1, /* disable hardware accesses */
                //sc_recvsetup:1, /* recv setup */
                sc_csapending:1,/* 11h channel switch pending */
                sc_radarena : 1;/* radar detection enabled */
                //sc_rxblocked: 1;/* rx waiting for dma buffers */
    struct mtx  sc_rx_lock; /* rx mutex */
    struct mtx  sc_node_lock; /* mn node mutex */
    spinlock_t  event_rx_skb_lock;
    struct mtx  sc_tx_postpond_lock;
    spinlock_t rx_skb_lock;
    spinlock_t txdone_mbuf_lock;
    spinlock_t tx_mbuf_lock;
    spinlock_t mac_cmd_lock;
    struct mtx tx_sche_lock;

    struct wq_hal      *sc_mh;     /* h/w access layer */
    //struct wq_hal_vap  *sc_hvap;   /* hal vap handle */
    //struct wq_hal_hwspec   sc_hwspecs; /* h/w capabilities */
    //struct wq_hal_txrxdma    sc_hwdma;   /* h/w dma setup */
    uint32_t        sc_imask;   /* interrupt mask copy */
    //enum ieee80211_phymode  sc_curmode;
    //u_int16_t       sc_curaid;  /* current association id */
    //u_int8_t        sc_curbssid[IEEE80211_ADDR_LEN];
    //WQ_HAL_CHANNEL     sc_curchan;

    uint8_t         sc_tx_postpond; /* tx postpond indication */
    uint8_t         sc_napvaps; /* # ap mode vaps */
    uint8_t         sc_nstavaps;    /* # sta mode vaps */
    uint8_t         sc_nbssid0; /* # vap's using base mac */
    uint32_t        sc_bssidmask;   /* bssid mask */
    atomic_t        sc_tx_fw_pending; /* frame pending for fw tx */
    struct ieee80211vap *sc_bvap; /* vap needs to send beacon */

    void            (*sc_recv_mgmt)(struct ieee80211com *,
                    struct mbuf *,
                    struct ieee80211_node *,
                    int, int, int, uint32_t);
    int         (*sc_newstate)(struct ieee80211com *,
                    enum ieee80211_state, int);
    void            (*sc_node_cleanup)(struct ieee80211_node *);
    void            (*sc_node_free)(struct ieee80211_node *);
    void            (*sc_node_drain)(struct ieee80211_node *);
    int         (*sc_recv_action)(struct ieee80211_node *,
                    const struct ieee80211_frame *,
                    const uint8_t *, const uint8_t *);
    int         (*sc_addba_request)(struct ieee80211_node *,
                    struct ieee80211_tx_ampdu *,
                    int dialogtoken, int baparamset,
                    int batimeout);
    int         (*sc_addba_response)(struct ieee80211_node *,
                    struct ieee80211_tx_ampdu *,
                    int status, int baparamset,
                    int batimeout);
    void            (*sc_addba_response_timeout)
                    (struct ieee80211_node *,
                    struct ieee80211_tx_ampdu *);
    void            (*sc_addba_stop)(struct ieee80211_node *,
                    struct ieee80211_tx_ampdu *);
    int             (*sc_ampdu_rx_start)(struct ieee80211_node *ni, struct ieee80211_rx_ampdu *rap,
	                int baparamset, int batimeout, int baseqctl);
    void            (*sc_ampdu_rx_stop)(struct ieee80211_node *ni, struct ieee80211_rx_ampdu *rap);

    //Tx
    void * wq_txbuf_data_ptr;  /* transmit data buffer ptr  */
    wq_txbufhead       sc_txbuf_data_list; /* transmit buffer */
    int                 sc_txbuf_data_cnt;
    void * wq_txbuf_mgmt_ptr;  /* transmit mgmt buffer ptr  */
    wq_txbufhead       sc_txbuf_mgmt_list;  /* mgmt transmit buffer */
    struct wq_txq      *sc_ac2q[WQ_NUM_TX_QUEUES];    /* WME AC -> h/w q map */
    struct wq_txq      sc_txq[WQ_NUM_TX_QUEUES];
    struct wq_txq      sc_mcast_swq; /* mcast swq to buffer pkt for stas in ps */
    struct wq_txq      sc_mcast_txq; /* mcast txq to keep track of bufs */

    struct wq_node_tx_sche sc_node_tx_sche[WQ_NUM_TX_QUEUES];
    //int    sc_hwq_limit_nonaggr;
    //int    sc_hwq_limit_aggr;

    //others
    //TODO : is this unused?
    struct task     sc_chanswitchtask;/* chan switch processing */

    //struct wuqi_work_struct  sc_txq_process_task;
    struct wuqi_work_struct  sc_rxq_process_task;
    struct wuqi_work_struct  sc_event_rxq_process_task;
    struct wuqi_work_struct  sc_monitor_tx_task;
    //struct wuqi_work_struct  sc_tx_task;
    struct wuqi_work_struct  sc_node_free_task[WQ_NUM_MAXNODE];
    struct wuqi_work_struct  sc_vap_delete_task;
    struct wuqi_work_struct  sc_parent_task;
    //struct wuqi_work_struct  sc_rxq_replenish_task;
    //struct wuqi_work_struct  sc_posttbtt_task;
#if defined(WIFI_SIM) || defined(WIFI_UART_SIM)
    struct wuqi_work_struct  sc_pretbtt_task;
    struct wuqi_work_struct  sc_sim_tx_isr_task;
#endif

    uint8_t         sc_staid[WQ_MAXSTAID/NBBY];
    int             sc_ageinterval;
    struct callout  sc_timer;   /* periodic work */
#if (WQ_CONFIG_64 == 1)
    uint64_t        node_uid_table[WQ_NUM_MAXNODE];
#else
    uint32_t        node_uid_table[WQ_NUM_MAXNODE];
#endif
    uint8_t         sc_keymap[WQ_NODE_TABLE_SZ/NBBY];

    struct wq_aggr_stat sc_aggr_stat;
    uint8_t         sc_aggr_force_en:2,
                    sc_aggr_force_cnt:6;
    uint32_t        sc_last_txdone;
#ifdef AMPDU_TX_DEBUG
    uint32_t        sc_aggr_seq;
    uint32_t        sc_hw_aggr_cnt;
#endif

    bool            sc_ac_traffic[WQ_NUM_TX_QUEUES+1];
    bool            sc_wmm_running;
    int             sc_wmm_hi_ac;

    struct mtx      wq_txq_lock;       /* global tx q lock for wq txq operation*/

#if defined(WIFI_SIM) || defined(WIFI_UART_SIM)
    struct callout      mh_sim_swba_timer;  /* callout handle for sim_swba */
#endif
};

#define WQ_LOCK_INIT(_sc) \
    mtx_init(&(_sc)->sc_mtx, device_get_nameunit((_sc)->sc_dev), \
         NULL, MTX_DEF | MTX_RECURSE)
#define WQ_LOCK_DESTROY(_sc)   mtx_destroy(&(_sc)->sc_mtx)
#define WQ_LOCK(_sc)       mtx_lock(&(_sc)->sc_mtx)
#define WQ_UNLOCK(_sc)     mtx_unlock(&(_sc)->sc_mtx)
#define WQ_LOCK_ASSERT(_sc)    mtx_assert(&(_sc)->sc_mtx, MA_OWNED)


int wq_attach(u_int16_t, struct wq_softc *);
int wq_detach(struct wq_softc *);
void    wq_resume(struct wq_softc *);
void    wq_suspend(struct wq_softc *);
void    wq_shutdown(void *);
void    wq_intr(void *);

#endif /* _DEV_WQ_MVVAR_H */

/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2003-2008 Sam Leffler, Errno Consulting
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
 *
 * $FreeBSD: stable/12/sys/net80211/ieee80211_freebsd.h 343892 2019-02-08 04:32:41Z avos $
 */
#ifndef _NET80211_IEEE80211_SIM_H_
#define _NET80211_IEEE80211_SIM_H_

#include "sdk.h"
#include "oss.h"
#include "taskq.h"
#include "../wdrv/mwl/wq_wifi_dbg.h"

#ifdef RISCV
#ifdef OPENCORE 
#include "os_timer.h"
#include "os_utils.h"
#else
#include "os_timer_api.h"
#include "os_utils_api.h"
#endif
#endif

#if 0 //backup of FREEBSD implementation
#define	LIST_ENTRY(type)						\
struct {								\
	struct type *le_next;	/* next element */			\
	struct type **le_prev;	/* address of previous next element */	\
}


#define	LIST_HEAD(name, type)						\
struct name {								\
	struct type *lh_first;	/* first element */			\
}

#define	LIST_FIRST(head)	((head)->lh_first)

#define	LIST_NEXT(elm, field)	((elm)->field.le_next)

#define	LIST_REMOVE(elm, field) do {					\
	if (LIST_NEXT((elm), field) != NULL)				\
		LIST_NEXT((elm), field)->field.le_prev = 		\
		    (elm)->field.le_prev;				\
	*(elm)->field.le_prev = LIST_NEXT((elm), field);		\
} while (0)

#define	LIST_HEAD_INITIALIZER(head)					\
	{ NULL }

#define	LIST_INSERT_HEAD(head, elm, field) do {				\
	if ((LIST_NEXT((elm), field) = LIST_FIRST((head))) != NULL)	\
		LIST_FIRST((head))->field.le_prev = &LIST_NEXT((elm), field);\
	LIST_FIRST((head)) = (elm);					\
	(elm)->field.le_prev = &LIST_FIRST((head));			\
} while (0)

#define	LIST_FOREACH(var, head, field)					\
	for ((var) = LIST_FIRST((head));				\
	    (var);							\
	    (var) = LIST_NEXT((var), field))


#define	TAILQ_ENTRY(type)						\
struct {								\
	struct type *tqe_next;	/* next element */			\
	struct type **tqe_prev;	/* address of previous next element */	\
}

#define	TAILQ_HEAD(name, type)						\
struct name {								\
	struct type *tqh_first;	/* first element */			\
	struct type **tqh_last;	/* addr of last next element */		\
}

#define	TAILQ_FIRST(head)	((head)->tqh_first)

#define	TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)

#define	TAILQ_REMOVE(head, elm, field) do {				\
	if ((TAILQ_NEXT((elm), field)) != NULL)				\
		TAILQ_NEXT((elm), field)->field.tqe_prev = 		\
		    (elm)->field.tqe_prev;				\
	else {								\
		(head)->tqh_last = (elm)->field.tqe_prev;		\
	}								\
	*(elm)->field.tqe_prev = TAILQ_NEXT((elm), field);		\
} while (0)

#define	TAILQ_INIT(head) do {						\
	TAILQ_FIRST((head)) = NULL;					\
	(head)->tqh_last = &TAILQ_FIRST((head));			\
} while (0)

#define	TAILQ_INSERT_TAIL(head, elm, field) do {			\
	TAILQ_NEXT((elm), field) = NULL;				\
	(elm)->field.tqe_prev = (head)->tqh_last;			\
	*(head)->tqh_last = (elm);					\
	(head)->tqh_last = &TAILQ_NEXT((elm), field);			\
} while (0)

#define	TAILQ_FOREACH(var, head, field)					\
	for ((var) = TAILQ_FIRST((head));				\
	    (var);							\
	    (var) = TAILQ_NEXT((var), field))

#define	TAILQ_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = TAILQ_FIRST((head));				\
	    (var) && ((tvar) = TAILQ_NEXT((var), field), 1);		\
	    (var) = (tvar))

#endif

//disable aggressive mode for WIFI certification
//#define CONFIG_AGGR_MODE

//Platform specific definition
#if 0
#define ntohl(x)  (inet_ntohl(x))
#define ntohs(x)  (inet_ntohs(x))
#define htonl(x)  (inet_htonl(x))
#define htons(x)  (inet_htons(x))
#define htole16(x) (x)
#define le16toh(x) (x)
#endif
#define htole16(x) (x)
#define le16toh(x) (x)
#define htole32(x) (x)
#define le32toh(x) (x)

//used only for ic_list_mtx
#define mtx_lock     osal_mutex_acquire
#define	mtx_unlock   osal_mutex_release
	

#define NET80211_MID_START						0x00001000
#define IEEE80211_LOCK_MID						(NET80211_MID_START)
#define IEEE80211_TX_LOCK_MID					(NET80211_MID_START+1)
#define IEEE80211_FF_LOCK_MID  				    (NET80211_MID_START+2)
#define IEEE80211_NODE_LOCK_MID				    (NET80211_MID_START+3)
#define IEEE80211_PSQ_LOCK_MID				    (NET80211_MID_START+4)
#define IEEE80211_AGEQ_LOCK_MID				    (NET80211_MID_START+5)
#define IEEE80211_ACL_LOCK_MID				    (NET80211_MID_START+6)
#define IEEE80211_SCANTABLE_LOCK_MID	        (NET80211_MID_START+7)
#define IEEE80211_SCANITER_LOCK_MID		        (NET80211_MID_START+8)
#define IEEE80211_MESHRTE_LOCK_MID		        (NET80211_MID_START+9)
#define IEEE80211_MESHRT_LOCK_MID 		        (NET80211_MID_START+10)
#define IEEE80211_IC_LIST_LOCK_MID				(NET80211_MID_START+11)
#define IEEE80211_NULL_DATA_TXC_LOCK_MID        (NET80211_MID_START+12)
#define IEEE80211_RF_CHAN_LOCK_MID              (NET80211_MID_START+13)
#define IEEE80211_VAP_DELETE_LOCK_MID           (NET80211_MID_START+14)
#define IEEE80211_DISCON_LOCK_MID               (NET80211_MID_START+15)

#define AMPDU_NORMAL  0
#define AMPDU_FORCE   1
#define AMPDU_LEGACY  2

//#define AMPDU_TX_DEBUG 1
#ifdef AMPDU_TX_DEBUG
extern int wifi_tx_queue_len();
#endif

/*
 * Common state locking definitions.
 */
typedef struct {
	char		name[16];		/* e.g. "ath0_com_lock" */
	struct osal_mutex_t	mtx;
} ieee80211_com_lock_t;
#define	IEEE80211_LOCK_INIT(_ic, _name) do {				\
	ieee80211_com_lock_t *cl = &(_ic)->ic_comlock;			\
	iot_snprintf(cl->name, sizeof(cl->name), "%s_com_lock", _name);	\
	osal_recurse_mutex_create(&cl->mtx, IEEE80211_LOCK_MID);	\
} while (0)
#define	IEEE80211_LOCK_OBJ(_ic)	(&(_ic)->ic_comlock.mtx)
#define	IEEE80211_LOCK_DESTROY(_ic) osal_mutex_destory(IEEE80211_LOCK_OBJ(_ic))
#define	IEEE80211_LOCK(_ic)	   osal_recurse_mutex_acquire(IEEE80211_LOCK_OBJ(_ic))
#define	IEEE80211_UNLOCK(_ic)	   osal_recurse_mutex_release(IEEE80211_LOCK_OBJ(_ic))
#define	IEEE80211_LOCK_ASSERT(_ic) //TODO do nothing for now
#define	IEEE80211_UNLOCK_ASSERT(_ic) //TODO do nothing for now

/*
 * Transmit lock.
 *
 * This is a (mostly) temporary lock designed to serialise all of the
 * transmission operations throughout the stack.
 */
typedef struct {
	char		name[16];		/* e.g. "ath0_tx_lock" */
	struct osal_mutex_t	mtx;
} ieee80211_tx_lock_t;
#define	IEEE80211_TX_LOCK_INIT(_ic, _name) do {				\
	ieee80211_tx_lock_t *cl = &(_ic)->ic_txlock;			\
	iot_snprintf(cl->name, sizeof(cl->name), "%s_tx_lock", _name);	\
	osal_mutex_create(&cl->mtx, IEEE80211_TX_LOCK_MID);	\
} while (0)
#define	IEEE80211_TX_LOCK_OBJ(_ic)	(&(_ic)->ic_txlock.mtx)
#define	IEEE80211_TX_LOCK_DESTROY(_ic) osal_mutex_destory(IEEE80211_TX_LOCK_OBJ(_ic))
#define	IEEE80211_TX_LOCK(_ic)	   osal_mutex_acquire(IEEE80211_TX_LOCK_OBJ(_ic))
#define	IEEE80211_TX_UNLOCK(_ic)	   osal_mutex_release(IEEE80211_TX_LOCK_OBJ(_ic))
#define	IEEE80211_TX_LOCK_ASSERT(_ic) //TODO do nothing for now
#define	IEEE80211_TX_UNLOCK_ASSERT(_ic) //TODO do nothing for now

/*
 * Stageq / ni_tx_superg lock
 */
typedef struct {
	char		name[16];		/* e.g. "ath0_ff_lock" */
	struct osal_mutex_t	mtx;
} ieee80211_ff_lock_t;
#define IEEE80211_FF_LOCK_INIT(_ic, _name) do {				\
	ieee80211_ff_lock_t *fl = &(_ic)->ic_fflock;			\
	iot_snprintf(fl->name, sizeof(fl->name), "%s_ff_lock", _name);	\
	osal_mutex_create(&fl->mtx, IEEE80211_FF_LOCK_MID);			\
} while (0)
#define IEEE80211_FF_LOCK_OBJ(_ic)	(&(_ic)->ic_fflock.mtx)
#define IEEE80211_FF_LOCK_DESTROY(_ic)	osal_mutex_destory(IEEE80211_FF_LOCK_OBJ(_ic))
#define IEEE80211_FF_LOCK(_ic)		osal_mutex_acquire(IEEE80211_FF_LOCK_OBJ(_ic))
#define IEEE80211_FF_UNLOCK(_ic)	osal_mutex_release(IEEE80211_FF_LOCK_OBJ(_ic))
#define IEEE80211_FF_LOCK_ASSERT(_ic) //TODO do nothing for now

/*
 * Node locking definitions.
 */
typedef struct {
	char		name[16];		/* e.g. "ath0_node_lock" */
	struct osal_mutex_t	mtx;
} ieee80211_node_lock_t;
#define	IEEE80211_NODE_LOCK_INIT(_nt, _name) do {			\
	ieee80211_node_lock_t *nl = &(_nt)->nt_nodelock;		\
	iot_snprintf(nl->name, sizeof(nl->name), "%s_node_lock", _name);	\
	osal_recurse_mutex_create(&nl->mtx, IEEE80211_NODE_LOCK_MID);	\
} while (0)
#define	IEEE80211_NODE_LOCK_OBJ(_nt)	(&(_nt)->nt_nodelock.mtx)
#define	IEEE80211_NODE_LOCK_DESTROY(_nt) \
	osal_mutex_destory(IEEE80211_NODE_LOCK_OBJ(_nt))
#define	IEEE80211_NODE_LOCK(_nt) \
	osal_recurse_mutex_acquire(IEEE80211_NODE_LOCK_OBJ(_nt))
#define	IEEE80211_NODE_IS_LOCKED(_nt) \
	(0)//TODO use trylock to implement?
#define	IEEE80211_NODE_UNLOCK(_nt) \
	osal_recurse_mutex_release(IEEE80211_NODE_LOCK_OBJ(_nt))
#define	IEEE80211_NODE_LOCK_ASSERT(_nt)	//TODO do nothing for now

/*
 * discon locking definitions under AP mode
 */
typedef struct osal_mutex_t ieee80211_discon_lock_t;
#define	IEEE80211_DISCON_LOCK_INIT(_vap) \
	osal_mutex_create(&(_vap)->mode_ap_discon_lock, IEEE80211_DISCON_LOCK_MID)
#define	IEEE80211_DISCON_LOCK_DESTROY(_vap)	osal_mutex_destory(&(_vap)->mode_ap_discon_lock)
#define	IEEE80211_DISCON_LOCK(_vap)		osal_mutex_acquire(&(_vap)->mode_ap_discon_lock)
#define	IEEE80211_DISCON_UNLOCK(_vap)	osal_mutex_release(&(_vap)->mode_ap_discon_lock)

/*
 * Power-save queue definitions. 
 */
typedef struct osal_mutex_t ieee80211_psq_lock_t;
#define	IEEE80211_PSQ_INIT(_psq, _name) \
	osal_mutex_create(&(_psq)->psq_lock, IEEE80211_PSQ_LOCK_MID)
#define	IEEE80211_PSQ_DESTROY(_psq)	osal_mutex_destory(&(_psq)->psq_lock)
#define	IEEE80211_PSQ_LOCK(_psq)	osal_mutex_acquire(&(_psq)->psq_lock)
#define	IEEE80211_PSQ_UNLOCK(_psq)	osal_mutex_release(&(_psq)->psq_lock)

#ifndef IF_PREPEND_LIST
#define _IF_PREPEND_LIST(ifq, mhead, mtail, mcount) do {	\
	(mtail)->m_nextpkt = (ifq)->ifq_head;			\
	if ((ifq)->ifq_tail == NULL)				\
		(ifq)->ifq_tail = (mtail);			\
	(ifq)->ifq_head = (mhead);				\
	(ifq)->ifq_len += (mcount);				\
} while (0)
#define IF_PREPEND_LIST(ifq, mhead, mtail, mcount) do {		\
	IF_LOCK(ifq);						\
	_IF_PREPEND_LIST(ifq, mhead, mtail, mcount);		\
	IF_UNLOCK(ifq);						\
} while (0)
#endif /* IF_PREPEND_LIST */
 
/*
 * Age queue definitions.
 */
typedef struct osal_mutex_t ieee80211_ageq_lock_t;
#define	IEEE80211_AGEQ_INIT(_aq, _name) \
	osal_mutex_create(&(_aq)->aq_lock, IEEE80211_AGEQ_LOCK_MID)
#define	IEEE80211_AGEQ_DESTROY(_aq)	osal_mutex_destory(&(_aq)->aq_lock)
#define	IEEE80211_AGEQ_LOCK(_aq)	osal_mutex_acquire(&(_aq)->aq_lock)
#define	IEEE80211_AGEQ_UNLOCK(_aq)	osal_mutex_release(&(_aq)->aq_lock)

/*
 * 802.1x MAC ACL database locking definitions.
 */
typedef struct osal_mutex_t acl_lock_t;
#define	ACL_LOCK_INIT(_as, _name) \
	osal_mutex_create(&(_as)->as_lock, IEEE80211_ACL_LOCK_MID)
#define	ACL_LOCK_DESTROY(_as)		osal_mutex_destory(&(_as)->as_lock)
#define	ACL_LOCK(_as)			osal_mutex_acquire(&(_as)->as_lock)
#define	ACL_UNLOCK(_as)			osal_mutex_release(&(_as)->as_lock)
#define	ACL_LOCK_ASSERT(_as) //TODO do nothing for now

/*
 * Scan table definitions.
 */
typedef struct osal_mutex_t ieee80211_scan_table_lock_t;
#define	IEEE80211_SCAN_TABLE_LOCK_INIT(_st, _name) \
	osal_mutex_create(&(_st)->st_lock, IEEE80211_SCANTABLE_LOCK_MID)
#define	IEEE80211_SCAN_TABLE_LOCK_DESTROY(_st)	osal_mutex_destory(&(_st)->st_lock)
#define	IEEE80211_SCAN_TABLE_LOCK(_st)		osal_mutex_acquire(&(_st)->st_lock)
#define	IEEE80211_SCAN_TABLE_UNLOCK(_st)	osal_mutex_release(&(_st)->st_lock)

typedef struct osal_mutex_t ieee80211_scan_iter_lock_t;
#define	IEEE80211_SCAN_ITER_LOCK_INIT(_st, _name) \
	osal_mutex_create(&(_st)->st_scanlock, IEEE80211_SCANITER_LOCK_MID)
#define	IEEE80211_SCAN_ITER_LOCK_DESTROY(_st)	osal_mutex_destory(&(_st)->st_scanlock)
#define	IEEE80211_SCAN_ITER_LOCK(_st)		osal_mutex_acquire(&(_st)->st_scanlock)
#define	IEEE80211_SCAN_ITER_UNLOCK(_st)	osal_mutex_release(&(_st)->st_scanlock)

/*
 * Mesh node/routing definitions.
 */
typedef struct osal_mutex_t ieee80211_rte_lock_t;
#define	MESH_RT_ENTRY_LOCK_INIT(_rt, _name) \
	osal_mutex_create(&(rt)->rt_lock, IEEE80211_MESHRTE_LOCK_MID)
#define	MESH_RT_ENTRY_LOCK_DESTROY(_rt) \
	osal_mutex_destory(&(_rt)->rt_lock)
#define	MESH_RT_ENTRY_LOCK(rt)	osal_mutex_acquire(&(rt)->rt_lock)
#define	MESH_RT_ENTRY_LOCK_ASSERT(rt) //TODO do nothing for now
#define	MESH_RT_ENTRY_UNLOCK(rt)	osal_mutex_release(&(rt)->rt_lock)

typedef struct osal_mutex_t ieee80211_rt_lock_t;
#define	MESH_RT_LOCK(ms)	osal_mutex_acquire(&(ms)->ms_rt_lock)
#define	MESH_RT_LOCK_ASSERT(ms)	//TODO do nothing for now
#define	MESH_RT_UNLOCK(ms)	osal_mutex_release(&(ms)->ms_rt_lock)
#define	MESH_RT_LOCK_INIT(ms, name) \
	osal_mutex_create(&(ms)->ms_rt_lock, IEEE80211_MESHRT_LOCK_MID)
#define	MESH_RT_LOCK_DESTROY(ms) \
	osal_mutex_destory(&(ms)->ms_rt_lock)

/*
 * Node reference counting definitions.
 *
 * ieee80211_node_initref	initialize the reference count to 1
 * ieee80211_node_incref	add a reference
 * ieee80211_node_decref	remove a reference
 * ieee80211_node_dectestref	remove a reference and return 1 if this
 *				is the last reference, otherwise 0
 * ieee80211_node_refcnt	reference count for printing (only)
 */

struct ieee80211vap;
int	ieee80211_com_vincref(struct ieee80211vap *);
void	ieee80211_com_vdecref(struct ieee80211vap *);
void	ieee80211_com_vdetach(struct ieee80211vap *);

//atomic functions
#define ieee80211_node_initref(_ni) \
	do { ((_ni)->ni_refcnt = 1); } while (0)
#define ieee80211_node_incref(_ni) osal_atomic_inc((struct osal_atomic_t *)&((_ni)->ni_refcnt))
#define	ieee80211_node_decref(_ni) osal_atomic_dec((struct osal_atomic_t *)&((_ni)->ni_refcnt))
struct ieee80211_node;
int	ieee80211_node_dectestref(struct ieee80211_node *ni);
#define	ieee80211_node_refcnt(_ni)	((_ni)->ni_refcnt)

struct ifqueue;
void	ieee80211_drain_ifq(struct ifqueue *);
void	ieee80211_flush_ifq(struct ifqueue *, struct ieee80211vap *);

void	ieee80211_vap_destroy(struct ieee80211vap *);

#if 1
#define	IFNET_IS_UP_RUNNING(_ifp) \
	((_ifp)->if_drv_flags & IFF_DRV_RUNNING)
#else
#define	IFNET_IS_UP_RUNNING(_ifp) \
	(((_ifp)->if_flags & IFF_UP) && \
	 ((_ifp)->if_drv_flags & IFF_DRV_RUNNING))
#endif

//#define	msecs_to_ticks(ms)	MSEC_2_TICKS(ms)
//#define	ticks_to_msecs(t)	TICKS_2_MSEC(t)
//#define	ticks_to_secs(t)	((t) / hz)

#define ieee80211_time_after(a,b) 	((long)(b) - (long)(a) < 0)
#define ieee80211_time_before(a,b)	ieee80211_time_after(b,a)
#define ieee80211_time_after_eq(a,b)	((long)(a) - (long)(b) >= 0)
#define ieee80211_time_before_eq(a,b)	ieee80211_time_after_eq(b,a)

struct mbuf *ieee80211_getmgtframe(uint8_t **frm, int headroom, int pktlen);

/* tx path usage */
#define	M_ENCAP		M_PROTO1		/* 802.11 encap done */
#define	M_NEED_ACK	M_PROTO2		/* host need know whether ack is recieved */
#define	M_EAPOL		M_PROTO3		/* PAE/EAPOL frame */
#define	M_PWR_SAV	M_PROTO4		/* bypass PS handling */
#define	M_MORE_DATA	M_PROTO5		/* more data frames to follow */
#define	M_FF		M_PROTO6		/* fast frame / A-MSDU */
#define	M_TXCB		M_PROTO7		/* do tx complete callback */
#define	M_AMPDU_MPDU	M_PROTO8		/* ok for A-MPDU aggregation */
#define	M_FRAG		M_PROTO9		/* frame fragmentation */
#define	M_FW_NO_ACK	M_PROTO10		/* if retry needed */
#define	M_FW_SWDUR	M_PROTO11		/* if using sw duration */
#define	M_FW_WAIT_TXC	M_PROTO12		/* fw to wait txc before other cmd */

#define	M_DMA_PENDING	0x1			/* frame dma pending */
#define	M_DMA_CKSUM 	0x2			/* swdma cksum */

#define	M_80211_TX \
	(M_ENCAP|M_EAPOL|M_PWR_SAV|M_MORE_DATA|M_FF|M_TXCB| \
	 M_AMPDU_MPDU|M_FRAG|M_FIRSTFRAG|M_LASTFRAG)

/* rx path usage */
#define	M_AMPDU         M_PROTO1        /* A-MPDU subframe */
#define	M_WEP           M_PROTO2        /* WEP done by hardware */
#define	M_RXCB          M_PROTO7        /* do rx complete callback */
#define	M_SW_DECRYPT    M_PROTO12       /* S/W decrypt */
#define	M_80211_RX     (M_AMPDU|M_WEP|M_AMPDU_MPDU)

#define	IEEE80211_MBUF_TX_FLAG_BITS \
	M_FLAG_BITS \
	"\15M_ENCAP\17M_EAPOL\20M_PWR_SAV\21M_MORE_DATA\22M_FF\23M_TXCB" \
	"\24M_AMPDU_MPDU\25M_FRAG\26M_FIRSTFRAG\27M_LASTFRAG"

#define	IEEE80211_MBUF_RX_FLAG_BITS \
	M_FLAG_BITS \
	"\15M_AMPDU\16M_WEP\24M_AMPDU_MPDU"

/*
 * Store WME access control bits in the vlan tag.
 * This is safe since it's done after the packet is classified
 * (where we use any previous tag) and because it's passed
 * directly in to the driver and there's no chance someone
 * else will clobber them on us.
 */
#define	M_WME_SETAC(m, ac) \
	((m)->m_pkthdr.ether_vtag = (ac))
#define	M_WME_GETAC(m)	((m)->m_pkthdr.ether_vtag)

/*
 * Mbufs on the power save queue are tagged with an age and
 * timed out.  We reuse the hardware checksum field in the
 * mbuf packet header to store this data.
 */
#define	M_AGE_SET(m,v)		(m->m_pkthdr.csum_data = v)
#define	M_AGE_GET(m)		(m->m_pkthdr.csum_data)
#define	M_AGE_SUB(m,adj)	(m->m_pkthdr.csum_data -= adj)

/*
 * Store the sequence number.
 */
#define	M_SEQNO_SET(m, seqno) \
	((m)->m_pkthdr.tso_segsz = (seqno))
#define	M_SEQNO_GET(m)	((m)->m_pkthdr.tso_segsz)

#define	MTAG_ABI_NET80211	1132948340	/* net80211 ABI */

struct ieee80211_cb {
	void	(*func)(struct ieee80211_node *, void *, int status);
	void	*arg;
};
#define	NET80211_TAG_CALLBACK	0	/* xmit complete callback */
int	ieee80211_add_callback(struct mbuf *m, struct ieee80211_node *,
		void (*func)(struct ieee80211_node *, void *, int), void *arg);
int	ieee80211_add_rx_callback(struct mbuf *m,
		void (*func)(struct ieee80211_node *, void *, int), void *arg);
void	ieee80211_process_callback(struct ieee80211_node *, struct mbuf *, int);

#define	NET80211_TAG_XMIT_PARAMS	1
/* See below; this is after the bpf_params definition */

#define	NET80211_TAG_RECV_PARAMS	2

#define	NET80211_TAG_TOA_PARAMS		3

struct ieee80211com;
int	ieee80211_parent_xmitpkt(struct ieee80211com *, struct mbuf *);
int	ieee80211_vap_xmitpkt(struct ieee80211vap *, struct mbuf *);



int if_printf(struct ifnet *ifp, const char * fmt, ...);

void	ieee80211_sysctl_attach(struct ieee80211com *);
void	ieee80211_sysctl_detach(struct ieee80211com *);
void	ieee80211_sysctl_vattach(struct ieee80211vap *);
void	ieee80211_sysctl_vdetach(struct ieee80211vap *);

//int	ieee80211_sysctl_msecs_ticks(SYSCTL_HANDLER_ARGS);

void	ieee80211_load_module(const char *);

/*
 * A "policy module" is an adjunct module to net80211 that provides
 * functionality that typically includes policy decisions.  This
 * modularity enables extensibility and vendor-supplied functionality.
 */
#define	_IEEE80211_POLICY_MODULE(policy, name, version)			\
typedef void (*policy##_setup)(int);					\
SET_DECLARE(policy##_set, policy##_setup);				\
static int								\
wlan_##name##_modevent(module_t mod, int type, void *unused)		\
{									\
	policy##_setup * const *iter, f;				\
	switch (type) {							\
	case MOD_LOAD:							\
		SET_FOREACH(iter, policy##_set) {			\
			f = (void*) *iter;				\
			f(type);					\
		}							\
		return 0;						\
	case MOD_UNLOAD:						\
	case MOD_QUIESCE:						\
		if (nrefs) {						\
			iot_printf("wlan_" #name ": still in use "		\
				"(%u dynamic refs)\n", nrefs);		\
			return EBUSY;					\
		}							\
		if (type == MOD_UNLOAD) {				\
			SET_FOREACH(iter, policy##_set) {		\
				f = (void*) *iter;			\
				f(type);				\
			}						\
		}							\
		return 0;						\
	}								\
	return EINVAL;							\
}									\
static moduledata_t name##_mod = {					\
	"wlan_" #name,							\
	wlan_##name##_modevent,						\
	0								\
};									\
DECLARE_MODULE(wlan_##name, name##_mod, SI_SUB_DRIVERS, SI_ORDER_FIRST);\
MODULE_VERSION(wlan_##name, version);					\
MODULE_DEPEND(wlan_##name, wlan, 1, 1, 1)

/*
 * Crypto modules implement cipher support.
 */
#define	IEEE80211_CRYPTO_MODULE(name, version)				\
_IEEE80211_POLICY_MODULE(crypto, name, version);			\
static void								\
name##_modevent(int type)						\
{									\
	if (type == MOD_LOAD)						\
		ieee80211_crypto_register(&name);			\
	else								\
		ieee80211_crypto_unregister(&name);			\
}									\
TEXT_SET(crypto##_set, name##_modevent)

/*
 * Scanner modules provide scanning policy.
 */
#define	IEEE80211_SCANNER_MODULE(name, version)				\
	_IEEE80211_POLICY_MODULE(scanner, name, version)

#define	IEEE80211_SCANNER_ALG(name, alg, v)				\
static void								\
name##_modevent(int type)						\
{									\
	if (type == MOD_LOAD)						\
		ieee80211_scanner_register(alg, &v);			\
	else								\
		ieee80211_scanner_unregister(alg, &v);			\
}									\
TEXT_SET(scanner_set, name##_modevent);					\

/*
 * ACL modules implement acl policy.
 */
#define	IEEE80211_ACL_MODULE(name, alg, version)			\
_IEEE80211_POLICY_MODULE(acl, name, version);				\
static void								\
alg##_modevent(int type)						\
{									\
	if (type == MOD_LOAD)						\
		ieee80211_aclator_register(&alg);			\
	else								\
		ieee80211_aclator_unregister(&alg);			\
}									\
TEXT_SET(acl_set, alg##_modevent);					\

/*
 * Authenticator modules handle 802.1x/WPA authentication.
 */
#define	IEEE80211_AUTH_MODULE(name, version)				\
	_IEEE80211_POLICY_MODULE(auth, name, version)

#define	IEEE80211_AUTH_ALG(name, alg, v)				\
static void								\
name##_modevent(int type)						\
{									\
	if (type == MOD_LOAD)						\
		ieee80211_authenticator_register(alg, &v);		\
	else								\
		ieee80211_authenticator_unregister(alg);		\
}									\
TEXT_SET(auth_set, name##_modevent)

/*
 * Rate control modules provide tx rate control support.
 */
#define	IEEE80211_RATECTL_MODULE(alg, version)				\
	_IEEE80211_POLICY_MODULE(ratectl, alg, version);		\

#define	IEEE80211_RATECTL_ALG(name, alg, v)				\
static void								\
alg##_modevent(int type)						\
{									\
	if (type == MOD_LOAD)						\
		ieee80211_ratectl_register(alg, &v);			\
	else								\
		ieee80211_ratectl_unregister(alg);			\
}									\
TEXT_SET(ratectl##_set, alg##_modevent)

struct ieee80211req;
typedef int ieee80211_ioctl_getfunc(struct ieee80211vap *,
    struct ieee80211req *);
//SET_DECLARE(ieee80211_ioctl_getset, ieee80211_ioctl_getfunc);
//#define	IEEE80211_IOCTL_GET(_name, _get) TEXT_SET(ieee80211_ioctl_getset, _get)

typedef int ieee80211_ioctl_setfunc(struct ieee80211vap *,
    struct ieee80211req *);
//SET_DECLARE(ieee80211_ioctl_setset, ieee80211_ioctl_setfunc);
//#define	IEEE80211_IOCTL_SET(_name, _set) TEXT_SET(ieee80211_ioctl_setset, _set)

/* XXX this stuff belongs elsewhere */
/*
 * Message formats for messages from the net80211 layer to user
 * applications via the routing socket.  These messages are appended
 * to an if_announcemsghdr structure.
 */
struct ieee80211_join_event {
	uint8_t		iev_addr[6];
};

struct ieee80211_leave_event {
	uint8_t		iev_addr[6];
};

struct ieee80211_replay_event {
	uint8_t		iev_src[6];	/* src MAC */
	uint8_t		iev_dst[6];	/* dst MAC */
	uint8_t		iev_cipher;	/* cipher type */
	uint8_t		iev_keyix;	/* key id/index */
	uint64_t	iev_keyrsc;	/* RSC from key */
	uint64_t	iev_rsc;	/* RSC from frame */
};

struct ieee80211_michael_event {
	uint8_t		iev_src[6];	/* src MAC */
	uint8_t		iev_dst[6];	/* dst MAC */
	uint8_t		iev_cipher;	/* cipher type */
	uint8_t		iev_keyix;	/* key id/index */
};

struct ieee80211_wds_event {
	uint8_t		iev_addr[6];
};

struct ieee80211_csa_event {
	uint32_t	iev_flags;	/* channel flags */
	uint16_t	iev_freq;	/* setting in Mhz */
	uint8_t		iev_ieee;	/* IEEE channel number */
	uint8_t		iev_mode;	/* CSA mode */
	uint8_t		iev_count;	/* CSA count */
};

struct ieee80211_cac_event {
	uint32_t	iev_flags;	/* channel flags */
	uint16_t	iev_freq;	/* setting in Mhz */
	uint8_t		iev_ieee;	/* IEEE channel number */
	/* XXX timestamp? */
	uint8_t		iev_type;	/* IEEE80211_NOTIFY_CAC_* */
};

struct ieee80211_radar_event {
	uint32_t	iev_flags;	/* channel flags */
	uint16_t	iev_freq;	/* setting in Mhz */
	uint8_t		iev_ieee;	/* IEEE channel number */
	/* XXX timestamp? */
};

struct ieee80211_auth_event {
	uint8_t		iev_addr[6];
};

struct ieee80211_deauth_event {
	uint8_t		iev_addr[6];
};

struct ieee80211_country_event {
	uint8_t		iev_addr[6];
	uint8_t		iev_cc[2];	/* ISO country code */
};

struct ieee80211_radio_event {
	uint8_t		iev_state;	/* 1 on, 0 off */
};

#define	RTM_IEEE80211_ASSOC	100	/* station associate (bss mode) */
#define	RTM_IEEE80211_REASSOC	101	/* station re-associate (bss mode) */
#define	RTM_IEEE80211_DISASSOC	102	/* station disassociate (bss mode) */
#define	RTM_IEEE80211_JOIN	103	/* station join (ap mode) */
#define	RTM_IEEE80211_LEAVE	104	/* station leave (ap mode) */
#define	RTM_IEEE80211_SCAN	105	/* scan complete, results available */
#define	RTM_IEEE80211_REPLAY	106	/* sequence counter replay detected */
#define	RTM_IEEE80211_MICHAEL	107	/* Michael MIC failure detected */
#define	RTM_IEEE80211_REJOIN	108	/* station re-associate (ap mode) */
#define	RTM_IEEE80211_WDS	109	/* WDS discovery (ap mode) */
#define	RTM_IEEE80211_CSA	110	/* Channel Switch Announcement event */
#define	RTM_IEEE80211_RADAR	111	/* radar event */
#define	RTM_IEEE80211_CAC	112	/* Channel Availability Check event */
#define	RTM_IEEE80211_DEAUTH	113	/* station deauthenticate */
#define	RTM_IEEE80211_AUTH	114	/* station authenticate (ap mode) */
#define	RTM_IEEE80211_COUNTRY	115	/* discovered country code (sta mode) */
#define	RTM_IEEE80211_RADIO	116	/* RF kill switch state change */
#define RTM_IEEE80211_RXMGMT    117     /* Rx MGMT frame */
#define RTM_IEEE80211_RX_PROMISCUOUS    118     /* Promiscuous mode Rx frame */
#define RTM_IEEE80211_VENDOR_IE 119     /* Rx Vendor IE */
#define RTM_IEEE80211_PROBEREQ  120     /* Rx PROBE REQ */
#define RTM_IEEE80211_RXEAPOL   121     /* Rx EAPOL */

#define	IEEE80211_BPF_CRYPTO	0x04	/* tx with h/w encryption */



struct ieee80211_rx_stats;

int	ieee80211_add_rx_params(struct mbuf *m,
	    const struct ieee80211_rx_stats *rxs);
int	ieee80211_get_rx_params(struct mbuf *m,
	    struct ieee80211_rx_stats *rxs);
uint32_t ieee80211_get_rx_params_ptr(struct mbuf *m);

struct ieee80211_toa_params {
	int request_id;
};
int	ieee80211_add_toa_params(struct mbuf *m,
	    const struct ieee80211_toa_params *p);
int	ieee80211_get_toa_params(struct mbuf *m,
	    struct ieee80211_toa_params *p);

#define	IEEE80211_F_SURVEY_TIME		0x00000001
#define	IEEE80211_F_SURVEY_TIME_BUSY	0x00000002
#define	IEEE80211_F_SURVEY_NOISE_DBM	0x00000004
#define	IEEE80211_F_SURVEY_TSC		0x00000008
struct ieee80211_channel_survey {
	uint32_t s_flags;
	uint32_t s_time;
	uint32_t s_time_busy;
	int32_t s_noise;
	uint64_t s_tsc;
};



/* XXX TODO: get rid of WAITOK, fix all the users of it? */
#define	IEEE80211_M_NOWAIT	M_NOWAIT
#define	IEEE80211_M_WAITOK	M_WAITOK
#define	IEEE80211_M_ZERO	0x1000 //M_ZERO

#define M_80211_NODE       0
#define M_80211_NODE_IE    1
#define M_80211_SCAN       2
#define M_80211_CRYPTO     3
#define M_80211_POWER      4
#define M_80211_RATECTL    5
#define M_TEMP             6
/*
 * Malloc API.  Other BSD operating systems have slightly
 * different malloc/free namings (eg DragonflyBSD.)
 */
//#define	IEEE80211_MALLOC(a, b, c)	mmal_malloc(a)
static __inline void* IEEE80211_MALLOC(size_t size, int type, int flags)
{
	void *ret;

	if (((ret = mmal_malloc(size)) != 0) && (flags & IEEE80211_M_ZERO))
	{
	    os_mem_set(ret, 0, size);
	}

	return ret;
}

#define	IEEE80211_FREE(a, b)	\
{ \
    mmal_free(a); \
}


/* XXX TODO: the type fields */


//TODO ticks to be implemented
#ifdef RISCV
#define ticks                   os_boot_time32() //in milli-second
#define time_init()
#else
#define ticks                   (jiffies)
#endif

#define hz                      (HZ)
#define time_uptime             ((ticks/1000)+1) //in second
#define msecs_to_ticks(ms)      msecs_to_jiffies(ms)

#define	ticks_to_msecs(t)	((t * 1000)/ hz)
#define	ticks_to_secs(t)	((t) / hz)

#define	ETHER_ADDR_LEN		6	/* length of an Ethernet address */


struct ether_header {
	u_char	ether_dhost[ETHER_ADDR_LEN];
	u_char	ether_shost[ETHER_ADDR_LEN];
	u_short	ether_type;
} __attribute__((__packed__));

#define	ETHER_IS_MULTICAST(addr) (*(addr) & 0x01) /* is address mcast/bcast? */
#define	ETHER_IS_BROADCAST(addr) \
	(((addr)[0] & (addr)[1] & (addr)[2] & \
	  (addr)[3] & (addr)[4] & (addr)[5]) == 0xff)
	  

#define KASSERT(a, b) IOT_ASSERT(a)


//TODO task to be implemented
#define task     wuqi_work_struct
/*struct task {
    uint8_t task_id;
};*/




#define	howmany(x, y)	(((x)+((y)-1))/(y))

#define	NBBY	8		/* 8 bits per byte */


#if 0
#define roundup(x, y)   ((((x)+((y)-1))/(y))*(y))  /* to any y */
#define roundup2(x, y)  (((x)+((y)-1))&(~((y)-1))) /* if y is powers of two */
#endif
#define nitems(x)   (sizeof((x)) / sizeof((x)[0]))
//#define MAX(a,b) (((a)>(b))?(a):(b))
//#define MIN(a,b) (((a)<(b))?(a):(b))
//#define min(a,b) (((a)<(b))?(a):(b))

#define setbit(a,i) (((unsigned char *)(a))[(i)/NBBY] |= 1<<((i)%NBBY))
#define clrbit(a,i) (((unsigned char *)(a))[(i)/NBBY] &= ~(1<<((i)%NBBY)))
#define isset(a,i)                          \
    (((const unsigned char *)(a))[(i)/NBBY] & (1<<((i)%NBBY)))
#define isclr(a,i)                          \
    ((((const unsigned char *)(a))[(i)/NBBY] & (1<<((i)%NBBY))) == 0)

/*
static __inline int abs(int j)
{
    return(j < 0 ? -j : j);
}
*/

//not used
#define MALLOC_DECLARE(a) 
#define MALLOC_DEFINE(a, b, c);

//#define ifm_change_cb_t uint8_t
//#define ifm_stat_cb_t uint8_t
    
int ic_printf(struct ieee80211com *ic, const char * fmt, ...);

//random number
//#define arc4random() os_rand()

/*
static __inline void get_random_bytes(void *p, size_t n)
{
    uint8_t *dp = p;

    while (n > 0) {
        uint32_t v = arc4random();
        size_t nb = n > sizeof(uint32_t) ? sizeof(uint32_t) : n;
        //bcopy(&v, dp, n > sizeof(uint32_t) ? sizeof(uint32_t) : n);
        os_mem_cpy(dp, &v, n > sizeof(uint32_t) ? sizeof(uint32_t) : n);
        dp += sizeof(uint32_t), n -= nb;
    }
}
*/

#define IFM_AUTO    0       /* Autoselect best media */

/* operating mode for multi-mode devices */
#define IFM_IEEE80211_11A   0x00010000  /* 5Ghz, OFDM mode */
#define IFM_IEEE80211_11B   0x00020000  /* Direct Sequence mode */
#define IFM_IEEE80211_11G   0x00030000  /* 2Ghz, CCK mode */
#define IFM_IEEE80211_FH    0x00040000  /* 2Ghz, GFSK mode */
#define IFM_IEEE80211_11NA  0x00050000  /* 5Ghz, HT mode */
#define IFM_IEEE80211_11NG  0x00060000  /* 2Ghz, HT mode */
#define IFM_IEEE80211_VHT5G 0x00070000  /* 5Ghz, VHT mode */
#define IFM_IEEE80211_VHT2G 0x00080000  /* 2Ghz, VHT mode */

/*
 * IEEE 802.11 Wireless
 */
#define IFM_IEEE80211   0x00000080
/* NB: 0,1,2 are auto, manual, none defined below */
#define IFM_IEEE80211_FH1   3   /* Frequency Hopping 1Mbps */
#define IFM_IEEE80211_FH2   4   /* Frequency Hopping 2Mbps */
#define IFM_IEEE80211_DS1   5   /* Direct Sequence 1Mbps */
#define IFM_IEEE80211_DS2   6   /* Direct Sequence 2Mbps */
#define IFM_IEEE80211_DS5   7   /* Direct Sequence 5.5Mbps */
#define IFM_IEEE80211_DS11  8   /* Direct Sequence 11Mbps */
#define IFM_IEEE80211_DS22  9   /* Direct Sequence 22Mbps */
#define IFM_IEEE80211_OFDM6 10  /* OFDM 6Mbps */
#define IFM_IEEE80211_OFDM9 11  /* OFDM 9Mbps */
#define IFM_IEEE80211_OFDM12    12  /* OFDM 12Mbps */
#define IFM_IEEE80211_OFDM18    13  /* OFDM 18Mbps */
#define IFM_IEEE80211_OFDM24    14  /* OFDM 24Mbps */
#define IFM_IEEE80211_OFDM36    15  /* OFDM 36Mbps */
#define IFM_IEEE80211_OFDM48    16  /* OFDM 48Mbps */
#define IFM_IEEE80211_OFDM54    17  /* OFDM 54Mbps */
#define IFM_IEEE80211_OFDM72    18  /* OFDM 72Mbps */
#define IFM_IEEE80211_DS354k    19  /* Direct Sequence 354Kbps */
#define IFM_IEEE80211_DS512k    20  /* Direct Sequence 512Kbps */
#define IFM_IEEE80211_OFDM3 21  /* OFDM 3Mbps */
#define IFM_IEEE80211_OFDM4 22  /* OFDM 4.5Mbps */
#define IFM_IEEE80211_OFDM27    23  /* OFDM 27Mbps */
/* NB: not enough bits to express MCS fully */
#define IFM_IEEE80211_MCS   24  /* HT MCS rate */
#define IFM_IEEE80211_VHT   25  /* VHT MCS rate */

#define IFM_ETHER   0x00000020
#define IFM_TMASK   0x0000001f  /* Media sub-type */
#define IFM_ETH_XTYPE   0x00007800  /* extended media variants */
#define IFM_NMASK   0x000000e0  /* Network type */
#define IFM_ETHER_SUBTYPE_GET(x) ((x) & (IFM_TMASK|IFM_ETH_XTYPE))
#define IFM_TYPE(x)     ((x) & IFM_NMASK)


static __inline uint32_t
le32dec(const void *pp)
{
    uint8_t const *p = (uint8_t const *)pp;

    return (((unsigned)p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0]);
}


//MBUF---------------------------------------------------------------
//MBUF related definitions


#define	M_ENCAP		M_PROTO1		/* 802.11 encap done */
#define	M_EAPOL		M_PROTO3		/* PAE/EAPOL frame */
#define	M_PWR_SAV	M_PROTO4		/* bypass PS handling */
#define	M_MORE_DATA	M_PROTO5		/* more data frames to follow */
#define	M_FF		M_PROTO6		/* fast frame / A-MSDU */
#define	M_TXCB		M_PROTO7		/* do tx complete callback */
#define	M_AMPDU_MPDU	M_PROTO8		/* ok for A-MPDU aggregation */
#define	M_FRAG		M_PROTO9		/* frame fragmentation */
#define	M_FIRSTFRAG	M_PROTO10		/* first frame fragment */
#define	M_LASTFRAG	M_PROTO11		/* last frame fragment */

#define MT_DATA     1   /* dynamic (data) allocation */

#define M_NOWAIT 0
#define M_BCAST     0x0000001 /* send/received as link-level broadcast */
#define M_MCAST     0x0000002 /* send/received as link-level multicast */
#define M_VLANTAG   0x0000008 /* ether_vtag is valid */

//#define M_PKTHDR    0x00000002 /* start of record */
//MBUF---------------------------------------------------------------


#define ETHERTYPE_PAE       0x888e  /* EAPOL PAE/802.1x */
#define ETHERTYPE_AARP      0x80F3  /* AppleTalk AARP */
#define ETHERTYPE_IPX       0x8137  /* Novell (old) NetWare IPX (ECONFIG E option) */



static __inline uint16_t
le16dec(const void *pp)
{
    uint8_t const *p = (uint8_t const *)pp;

    return ((p[1] << 8) | p[0]);
}


#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef RISCV
#define __offsetof offsetof
#endif

//#define ovbcopy(f, t, l) bcopy((f), (t), (l))


static __inline char *
ether_sprintf(const u_char *ap)
{
    static char etherbuf[20];
    iot_snprintf(etherbuf, sizeof (etherbuf), "%2X:%2X:%2X:%2X:%2X:%2X", ap[0], ap[1], ap[2], ap[3], ap[4], ap[5]);
    return (etherbuf);
}

static __inline char *
ssid_sprintf(const u_char *ssid, int ssid_len)
{
    static char ssidbuf[33];
	int i;
	
	if (ssid_len > 32)
		ssid_len = 32;
    //iot_snprintf(etherbuf, sizeof (etherbuf), "%2X:%2X:%2X:%2X:%2X:%2X", ap[0], ap[1], ap[2], ap[3], ap[4], ap[5]);
	for(i=0; i<ssid_len; i++)
	{
		if ((ssid[i] >= ' ') && (ssid[i] <= '~'))
			ssidbuf[i] = ssid[i];
	    else
			ssidbuf[i] = '?';
	}
	ssidbuf[i] = 0;
	
    return (ssidbuf);
}

//#define if_inc_counter(ifp, cnt, inc) net_if_inc_counter(ifp, cnt, inc)



struct llc {
    u_int8_t llc_dsap;
    u_int8_t llc_ssap;
    union {
        struct {
        u_int8_t control;
        u_int8_t format_id;
        u_int8_t class;
        u_int8_t window_x2;
        } __attribute__((__packed__)) type_u;
        struct {
        u_int8_t num_snd_x2;
        u_int8_t num_rcv_x2;
        } __attribute__((__packed__)) type_i;
        struct {
        u_int8_t control;
        u_int8_t num_rcv_x2;
        } __attribute__((__packed__)) type_s;
        struct {
            u_int8_t control;
        /*
         * We cannot put the following fields in a structure because
         * the structure rounding might cause padding.
         */
        u_int8_t frmr_rej_pdu0;
        u_int8_t frmr_rej_pdu1;
        u_int8_t frmr_control;
        u_int8_t frmr_control_ext;
        u_int8_t frmr_cause;
        } __attribute__((__packed__)) type_frmr;
        struct {
        u_int8_t  control;
        u_int8_t  org_code[3];
        u_int16_t ether_type;
        } __attribute__((__packed__)) type_snap;
        struct {
        u_int8_t control;
        u_int8_t control_ext;
        } __attribute__((__packed__)) type_raw;
    } __attribute__((__packed__)) llc_un;
} __attribute__((__packed__));

#define llc_control     llc_un.type_u.control
#define llc_snap        llc_un.type_snap

#define LLC_SNAP_LSAP   0xaa
#define LLC_UI      0x3

#define EVL_VLID_MASK       0x0FFF
#define EVL_VLANOFTAG(tag)  ((tag) & EVL_VLID_MASK)


//TODO : do we need this?
#define __NO_STRICT_ALIGNMENT

//there is no kernel or user address space
static __inline int copyin(const void *uaddr, void *kaddr, size_t len)
{
	os_mem_cpy(kaddr, uaddr, len);
	return 0;
}

static __inline int copyout(const void	*kaddr,	void *uaddr, size_t len)
{
	os_mem_cpy(uaddr, kaddr, len);
	return 0;
}

static __inline void
le16enc(void *pp, uint16_t u)
{
    uint8_t *p = (uint8_t *)pp;

    p[0] = u & 0xff;
    p[1] = (u >> 8) & 0xff;
}

#define EVL_PRIOFTAG(tag)   (((tag) >> 13) & 7)

#define callout osal_timer_t
typedef void timeout_t(void *); /* timeout function type */
void dump_timer_record();
int callout_pending(struct osal_timer_t *c);
void callout_init_v2(struct osal_timer_t *osal_timer, timeout_t *func, void *arg);

int _callout_reset_v2_dbg(struct callout *c, int tick, const char *function_name, int line_num);
#define callout_reset_v2(x, y) _callout_reset_v2_dbg(x, y,  __func__, __LINE__)

int _callout_stop_v2_dbg(struct callout *c, const char *function_name, int line_num);
#define callout_stop_v2(x) _callout_stop_v2_dbg(x,  __func__, __LINE__)

void _callout_deinit_dbg(struct callout *timer, const char *function_name, int line_num);
#define callout_deinit(x) _callout_deinit_dbg(x,  __func__, __LINE__)

int _callout_drain_v2_dbg(struct callout *c, const char *function_name, int line_num);
#define callout_drain_v2(x) _callout_drain_v2_dbg(x,  __func__, __LINE__)


//TODO FREEBSD taskqueue on RTOS embedded system
typedef void task_fn_t(void *context, int unused);
void wakeup_net80211_thread(uint32_t cause, int in_isr);


struct net80211_subtask {
	task_fn_t*	func;		/* size of ie_data */
	void*			  context;	/* user-specified IE's */
	uint32_t    count;
};
#define NET80211_MAX_TASK_NUM 10
	 
//#define taskqueue 
//#define taskqueue_create(name, mflags, enqueue, context) 
//TODO
#define taskqueue_enqueue(a, b) 
#define taskqueue_drain(a, b)
#define taskqueue_free(a)
#define taskqueue_cancel_timeout(a, b, c) 
#define taskqueue_drain_timeout(a, b) 

//Defined in OSS now
//#define	IFNAMSIZ	16

void wifi_tx_attach(void);
void wifi_tx_detach(void);

int is_wmm_running(struct ieee80211vap *vap);
int is_wmm_hi_ac(struct ieee80211vap *vap, int ac);

void wifi_tx_work_start(struct ieee80211com *ic, struct ieee80211vap *vap);

void net_if_input(struct ieee80211vap *vap, struct mbuf *m);

#endif /* _NET80211_IEEE80211_SIM_H_ */

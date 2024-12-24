#ifndef _MBUF_H
#define _MBUF_H


//#include "oss.h"

#include "wq_hw_config.h"
//#define HAL_RX_DESC_SIZE 28
//#define HAL_RX_DATA_BUF_SIZE 1700

//#define CONFIG_MBUF_PRINTF


#ifdef CONFIG_MBUF_PRINTF
#define mbuf_printf                 iot_printf
#else
#define mbuf_printf(_f, args...)
#endif

/*
 * mbuf inherit from SDK's iot_pkt and assume "iot_pkt header + data buffer" is
 * a linear memory. The begining of memory is iot_pkt header then data buffer.
 *
 * After m_gethdr() and all buffer operation base on m_data pointer instead of
 * mbuf.__pkt.data pointer. ONLY mbuf.__pkt.head/mbuf.__pkt.end are valid for
 * head/tail room checking.
 *
 * +--------+ <---- mbuf header
 * |iot_pkt | (DO NOT MOVE AND TOUCH iot_pkt)
 * |...     |
 * |.m_len  |
 * |.m_data |
 * |...     |
 * ---------+ <---- mbuf data
 * |        |
 * | Data   |
 * | Buffer |
 * |        |
 * +--------+
 */

#define SIZEOF_MBUF         (sizeof(struct mbuf))
#define MBUF_MT_DATA_SZ     (HAL_RX_DATA_BUF_SIZE + 4) //add 4 bytes margin

/* At least 802.11/QoS/HTC header (3-addr:30, do not support 4-addr:36) +
 *          IV (8 for AES, 18 for WAPI) +
 *          802.2 SNAP header (8) +
 *          802.3 payload (1500) +
 *          MIC/ICV (8 for AES, 16 for WAPI) +
 *          FCS (4) = 1558 or 1576(wapi) bytes
 */
#define MBUF_ALLOC_SIZE     (SIZEOF_MBUF + 8 + HAL_RX_DESC_SIZE + MBUF_MT_DATA_SZ + 4) //iot packet alloc size = struct mbuf + 8 bytes magic pattern and mbuf ptr + Rx DESC size + 802.11 frame size + 4 bytes tail for buffer corruption detection

#define MT_DATA     1       /* only type support */

#define M_NOWAIT    0

#define M_BCAST     0x0000001 /* send/received as link-level broadcast */
#define M_MCAST     0x0000002 /* send/received as link-level multicast */
#define M_VLANTAG   0x0000008 /* ether_vtag is valid */

//#define M_PKTHDR    0x00000002 /* start of record */

#define M_PROTO1    0x000010 /* protocol-specific */
#define M_PROTO2    0x000020 /* protocol-specific */
#define M_PROTO3    0x000040 /* protocol-specific */
#define M_PROTO4    0x000080 /* protocol-specific */
#define M_PROTO5    0x000100 /* protocol-specific */
#define M_PROTO6    0x000200 /* protocol-specific */
#define M_PROTO7    0x000400 /* protocol-specific */
#define M_PROTO8    0x000800 /* protocol-specific */
#define M_PROTO9    0x001000 /* protocol-specific */
#define M_PROTO10   0x002000 /* protocol-specific */
#define M_PROTO11   0x004000 /* protocol-specific */
#define M_PROTO12   0x008000 /* protocol-specific */
#define M_PROTOFLAGS \
    (M_PROTO1|M_PROTO2|M_PROTO3|M_PROTO4|M_PROTO5|M_PROTO6|M_PROTO7|M_PROTO8|\
     M_PROTO9|M_PROTO10|M_PROTO11|M_PROTO12)


struct pkthdr {
    struct ifnet *rcvif;
    uint32_t len;

    /* information across all modules access mbuf */
    uint32_t csum_data; //age
    uint16_t ether_vtag; //WMM AC
    uint16_t tso_segsz; //seq num
};

struct mbuf {
    /* WARN: keep first */
    //iot_pkt_t       __pkt;
    struct sk_buff *skb;
    
    struct ieee80211vap *vap;

    /*
     * AMSDU will use m_next to chain if buffer cross multiple RxDesc.
     */
    struct mbuf *m_next;

	struct mbuf *txq_next;

    union {
        struct mbuf *m_nextpkt;
        STAILQ_ENTRY(mbuf) m_stailqpkt;
    };

    uint8_t *m_data;

    /*
     * Not support one packet across multiple mbuf and so only keep one length
     * value.
     */
#define m_len      m_pkthdr.len
    /* int32_t m_len */

    uint32_t m_type:1,
             m_dma:2,
             m_padding:5,
             m_flags:16,
             m_keyid:8;


    struct pkthdr m_pkthdr;

    void (*cbfunc)(void *, void *, int);
    void *cbarg;
    void *cbni;

    uint16_t m_flags_ext;

    uint16_t m_header_cksum;
    uint16_t m_payload_cksum;
    uint32_t m_cksum_addr;

    //void *rxs;
    //uint32_t c_pktflags;		/* IEEE80211_RX_F_* flags */
	uint8_t c_nf;			/* global NF */
	uint8_t c_rssi;			/* global RSSI */
	//uint16_t c_freq;		/* Frequency, MHz */
	//uint8_t c_ifidx;		/* interface index */

    /* for wifi debug pkt dump */
    int dbg_pkt_idx;        /* pkt dump index */
    uint32_t proto_ts;      /* proto timestamp */
    uint32_t driver_ts;     /* driver timestamp */
    uint32_t hal_ts;        /* hal timestamp */
    uint32_t hal_tsf_ts;    /* hal tsf timestamp */
    uint32_t mac_ts;        /* mac timestamp */
    uint16_t m_ping_seq;    /* ping sequence*/

    /* DEBUG */
#define MBUF_DEBUG_POSION       0xdeadbeef
    uint32_t poison;
};

#if (WQ_CONFIG_64 == 1)
static inline uint64_t M_LEADINGSPACE(struct mbuf *m)
{
    struct sk_buff *skb = m->skb;

    BUG_ON((uint64_t)m->m_data < (uint64_t)skb->head);

    return (uint64_t)((uint8_t*)m->m_data - (uint8_t*)(skb->head));
}
#else
static inline uint32_t M_LEADINGSPACE(struct mbuf *m)
{
    struct sk_buff *skb = m->skb;

    BUG_ON((uint32_t)m->m_data < (uint32_t)skb->head);

    return (uint32_t)((uint8_t*)m->m_data - (uint8_t*)(skb->head));
}
#endif

static inline uint32_t M_TRAILINGSPACE(struct mbuf *m)
{
    struct sk_buff *skb = m->skb;
#ifdef NET_SKBUFF_DATA_USES_OFFSET
    return skb->end - (uint32_t)(m->m_data - skb->head) - m->m_len;
#else
    return (uint32_t)(((uint8_t*)skb->end) - (uint8_t*)(m->m_data + m->m_len));
#endif
}

#define mtod(m, t)          ((t)((m)->m_data))
#define mtodo(m, o)         ((void *)(((m)->m_data) + (o)))

static inline void m_copydata(const struct mbuf *m,
                              int offset,
                              int len,
                              uint8_t *buf)
{
    IOT_ASSERT(offset >= 0 && len >= 0);
#ifdef NET_SKBUFF_DATA_USES_OFFSET
    BUG_ON((uint32_t)(m->m_data - m->skb->head) + offset + len > m->skb->end);
#else
    BUG_ON(m->m_data + offset + len > m->skb->end);
#endif


    os_mem_cpy((void *)buf, (const void *)(m->m_data + offset), (uint32_t)len);

    return;
}

static inline void m_clrprotoflags(struct mbuf *m)
{
    BUG_ON(m->m_next);

    while (m) {
        m->m_flags &= ~M_PROTOFLAGS;
        m = m->m_nextpkt;
    }

    return;
}

static inline void m_adj(struct mbuf *m, int req_len)
{
    if (req_len >= 0) {             /* trim the head bytes */
#if (WQ_CONFIG_64 == 1)
        BUG_ON((uint64_t)(m->m_data - m->skb->head) + req_len > m->skb->end);
#else
        BUG_ON(m->m_data + req_len > m->skb->end);
#endif
        m->m_data += req_len;
        m->m_len -= req_len;
    } else {                        /* trim the tail bytes */
        BUG_ON((int)m->m_len + req_len < 0);

        m->m_len += req_len;
    }

    return;
}


struct mbuf_op_ctx_t {
    int pre_leading;
};

#define M_PREPEND           m_prepend

//#define DEBUG_MBUF_TRACKING

#ifdef DEBUG_MBUF_TRACKING
//for mbuf debug
struct mbuf *m_gethdr_debug(int how, short type, void* ra);
#define m_gethdr(how, type) m_gethdr_debug(how, type, __builtin_return_address(0))
struct mbuf *m_gethdr_amsdu_debug(int how, short type, void* ra);
#define m_gethdr_amsdu(how, type) _m_gethdr_amsdu_debug(how, type,__builtin_return_address(0))
#else
struct mbuf* _m_gethdr(int how, short type);
#define m_gethdr(how, type) _m_gethdr(how, type)
struct mbuf* _m_gethdr_amsdu(int how, short type);
#define m_gethdr_amsdu(how, type) _m_gethdr_amsdu(how, type)
#endif

struct mbuf *_m_gethdr_with_skb(int how, short type, struct sk_buff *skb, int len);
void m_freem_wo_skb(struct mbuf *m);


void m_freem(struct mbuf *m);
struct mbuf *m_dup(const struct mbuf *m, int how);
struct mbuf *m_prepend(struct mbuf *m, uint32_t len, int how);
int m_append(struct mbuf *m, uint32_t len, const uint8_t *cp);
void m_catpkt(struct mbuf *m, struct mbuf *n);

void mbuf_pre_leading(int pre_leading);
void mbuf_init(int tag0_num, int tag1_num, int tag2_num, int tag3_num);
void mbuf_reset_stat(void);
void mbuf_stat(void);
void mbuf_dump_data(char *note, struct mbuf *m);

#endif

#if 0
static inline struct mbuf *m_pullup(struct mbuf *m0, int len)
{
    BUG_ON(m0->m_next);
    IOT_ASSERT((m0->skb->end - m0->m_data) >= len);

    /* if len is already contig in m0, then don't do any work */
    if ((uint32_t)len <= m0->m_len)
        return (m0);

    m_freem(m0);
    return (NULL);
}
#endif
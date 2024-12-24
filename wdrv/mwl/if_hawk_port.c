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

#include <net80211/ieee80211_var.h>
#include <net80211/ieee80211_crypto.h>

#include "if_hawk_port.h"
#include "if_hawkvar.h"
#include "wq_hw_config.h"
#include "wq_hal_util.h"
#include "wq_htc_desc.h"
#include "wq_ipc.h"
#include "../../wifi_drv.h"
#include "usb.h"

extern int is_wtest_mode(void);
extern void wtest_rx_proc(struct mbuf* m);
extern void hal_rx_data_ring_proc(struct wq_softc *sc);
extern void hal_rx_mgmt_ring_proc(struct wq_softc *sc);
extern void hal_update_node_start_seq(struct wq_softc *sc, int qidx,struct ieee80211_node *ni);
extern int wq_tx_sche(struct wq_softc *sc);
extern void wq_start(struct wq_softc *sc);
extern void wq_handlemicerror(struct ieee80211com *ic, const uint8_t *data);
extern void wq_free_skb(struct sk_buff *skb);
extern int wq_transmit(struct ieee80211com *ic, struct mbuf *m);
extern struct ieee80211_key *ieee80211_crypto_getucastkey(struct ieee80211vap *vap,
	struct ieee80211_node *ni);

extern struct ieee80211vap *hawk_vap0, *hawk_vap1;
extern struct ieee80211com *net80211_ic;
extern void *g_beacon_tx_desc;
extern void htc_cali_info(struct wq_softc *sc, uint8_t *cali_data, uint16_t cali_len);
extern int wuqi_change_channel_dpd();

#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
extern void wq_sche_pktin_work_ex(void * wq_ipc);
extern int submit_rxurb_limit_l;
#endif

struct sk_buff *rx_skb_head = NULL;
struct sk_buff *rx_skb_tail = NULL;
struct sk_buff *event_rx_skb_head = NULL;
struct sk_buff *event_rx_skb_tail = NULL;
struct sk_buff *wq_alloc_skb_amsdu(int len_skb);

#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
int rx_done_count_wk_limit = 5;
int rx_skb_count_limit = 50;
#endif

#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
extern bool reset_down_flag;
#endif

int rx_skb_count = 0;
uint32_t rx_count = 0;
uint32_t rx_mngt_count = 0;
uint32_t rx_data_count = 0;
int event_rx_skb_count = 0;

struct mbuf *txdone_mbuf_head = NULL;
struct mbuf *txdone_mbuf_tail = NULL;

struct mbuf *tx_mbuf_head = NULL;
struct mbuf *tx_mbuf_tail = NULL;

struct mbuf *txpostpond_mbuf_head = NULL;
struct mbuf *txpostpond_mbuf_tail = NULL;
struct htc_msg_desc *cmd_desc_head;
struct htc_msg_desc *cmd_desc_tail;

void flush_rx_skb_queue(void);
void flush_event_rx_skb_queue(void);
void wq_mac_tx_done_work(void);
void wq_mac_tx_work(uint8_t action);
void wq_mac_cmd_work(uint8_t);
void wq_tx_postpond_work(uint8_t);
void flush_all_queue(void);

#if (WQ_CONFIG_64 == 1)
#define diff_pointer_sizes 4 //the difference of pointer sizes in different systems (32bit 4, 64bit 8)
#endif

void dump_hw_reg_base_addr(void)
{
    DPRINTF(WDRV_DEBUG_HAL, "===============================\n");
    DPRINTF(WDRV_DEBUG_HAL, "MAC_TOP_BASEADDR : 0X%X\n",MAC_TOP_BASEADDR);
    DPRINTF(WDRV_DEBUG_HAL, "MAC_TMR_BASEADDR : 0X%X\n",MAC_TMR_BASEADDR);
    DPRINTF(WDRV_DEBUG_HAL, "MAC_TXQ_BASEADDR : 0X%X\n",MAC_TXQ_BASEADDR);
    DPRINTF(WDRV_DEBUG_HAL, "MAC_MPI_BASEADDR : 0X%X\n",MAC_MPI_BASEADDR);
    DPRINTF(WDRV_DEBUG_HAL, "MAC_TX_BASEADDR  : 0X%X\n",MAC_TX_BASEADDR );
    DPRINTF(WDRV_DEBUG_HAL, "MAC_RX_BASEADDR  : 0X%X\n",MAC_RX_BASEADDR );
    DPRINTF(WDRV_DEBUG_HAL, "MAC_RXR_BASEADDR : 0X%X\n",MAC_RXR_BASEADDR);
    DPRINTF(WDRV_DEBUG_HAL, "===============================\n");
}

char *device_get_nameunit(device_t dev)
{
    return "hawkdev";
}

#if (WQ_CONFIG_64 == 1)
uint64_t get_ni_from_idx(struct ieee80211com *ic, int node_idx)
#else
uint32_t get_ni_from_idx(struct ieee80211com *ic, int node_idx)
#endif
{
    struct wq_softc *sc=(struct wq_softc *)ic->ic_softc;
    if(node_idx<WQ_NUM_MAXNODE)
        return sc->node_uid_table[node_idx];

    return 0;
}

#ifdef WIFI_SIM
void *wifi_sim_ctx;
struct ieee80211com *wifi_sim_ic;
extern void hwsim_rx_dma(struct mbuf *m);

void dev_sim_rxdone(void)
{
    int len;
    struct ieee80211_rx_stats rxs;
    struct ieee80211_node *ni;
    struct wq_softc *sc = wifi_sim_ic->ic_softc;
//#define OLDWAY 1  //Reserved if need to revert to old way.
#ifdef OLDWAY
    struct mbuf *m = NULL;
#else
    static struct mbuf *m = NULL;
    if(m==NULL)
#endif
    m=m_gethdr(0, MT_DATA);

    len = simraw_rx(wifi_sim_ctx, m->m_data);
    m->m_len = len;

    if (is_wtest_mode()) {
        wtest_rx_proc(m);
        return;
    }

#ifdef OLDWAY
    bzero(&rxs, sizeof(rxs));

    rxs.c_rssi = 30;
    rxs.c_nf = 20;

    ieee80211_add_rx_params(m, &rxs);
#endif

    if ((*(m->m_data + 4) & 0x1) == 0)
    {
        DPRINTF(WDRV_DEBUG_RECV, "unicast frame\n");
    }
    else
    {
        DPRINTF(WDRV_DEBUG_RECV, "broadcast frame\n");
    }

#ifdef OLDWAY
     //Original just pass to wproto
    ni = ieee80211_find_rxnode_withkey(wifi_sim_ic,
                mtod(m, const struct ieee80211_frame_min *),
                IEEE80211_KEYIX_NONE);
    if (ni != NULL) {
        ieee80211_input_mimo(ni, m);
        ieee80211_free_node(ni);
    }
    else {
        ieee80211_input_mimo_all(wifi_sim_ic, m);
    }
#else
    if (hwsim_rx_status_check() < 0) {
        return;
    }

    //Here to Simulate HW DMA move to RX descriptors
    hwsim_rx_dma(m);

    //Here to Simulate SW receive buffer from RX Descriptors
    hal_rx_data_ring_proc(sc);
    hal_rx_mgmt_ring_proc(sc);
#endif

    return;
}

void dev_wifi_sim_init(void *ic)
{
    wifi_sim_ic = (struct ieee80211com *)ic;
    wifi_sim_ctx = sim_create_device_raw(1,
                                         os_rand()%0xffff,
                                         dev_sim_rxdone);
}
#endif


/* mbufq */
int mbufq_full(const struct mbufq *mq)
{
    return (mq->mq_len >= mq->mq_maxlen);
}

void mbufq_init(struct mbufq *mq, int maxlen)
{
    STAILQ_INIT(&mq->mq_head);
    mq->mq_maxlen = maxlen;
    mq->mq_len = 0;
}

void mbufq_drain(struct mbufq *mq)
{
    struct mbuf *m, *n;

    n = mbufq_flush(mq);
    while ((m = n) != NULL) {
        n = STAILQ_NEXT(m, m_stailqpkt);
        m->m_nextpkt = NULL;
        m_freem(m);
    }
}

struct mbuf *mbufq_dequeue(struct mbufq *mq, struct mtx *lock)
{
    struct mbuf *m;

    mtx_lock(lock);
    m = STAILQ_FIRST(&mq->mq_head);
    if (m) {
        STAILQ_REMOVE_HEAD(&mq->mq_head, m_stailqpkt);
        m->m_nextpkt = NULL;
        mq->mq_len--;
    }
    mtx_unlock(lock);
    return m;
}

int mbufq_enqueue(struct mbufq *mq, struct mtx *lock, struct mbuf *m, int type)
{
    mtx_lock(lock);
    if (mbufq_full(mq)) {
        mtx_unlock(lock);
        return (ENOBUFS);
    }

    switch (type) {
        case QUEUE_HEAD:
            STAILQ_INSERT_HEAD(&mq->mq_head, m, m_stailqpkt);
            break;
        case QUEUE_TAIL:
            STAILQ_INSERT_TAIL(&mq->mq_head, m, m_stailqpkt);
            break;
        default:
            IOT_ASSERT(0);
            break;
    }
    mq->mq_len++;
    mtx_unlock(lock);
    return 0;
}

struct mbuf *mbufq_flush(struct mbufq *mq)
{
    struct mbuf *m;

    m = STAILQ_FIRST(&mq->mq_head);
    STAILQ_INIT(&mq->mq_head);
    mq->mq_len = 0;
    return (m);
}

uint8_t beacon_frame_open[] = { 0x80, 0x00, 0x00, 0x00,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //A1
0x00, 'H',  'A',  'W',  'K',  'F',  //A2
0x00, 'H',  'A',  'W',  'K',  'F',  //A3
0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, //timestamp
0x64, 0x00, //beacon interval
0x21, 0x04, //capability
0x00, 0x03, 'W', 'Q', 'f',//SSID
0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
0x03, 0x01, 0x06, //channel
0x05, 0x04, 0x00, 0x01, 0x00, 0x00, //TIM
0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, //extended support rate
//0x00, 0x00, 0x00, 0x00 //crc
};

uint8_t beacon_frame_wep[] = { 0x80, 0x00, 0x00, 0x00,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //A1
0x00, 'H',  'A',  'W',  'K',  '1',  //A2
0x00, 'H',  'A',  'W',  'K',  '1',  //A3
0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, //timestamp
0x66, 0x00, //beacon interval
0x31, 0x04, //capability
0x00, 0x03, 'w', 'e', 'p',//SSID
0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
0x03, 0x01, 0x01, //channel
0x05, 0x04, 0x00, 0x01, 0x00, 0x00, //TIM
0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, //extended support rate
//0x00, 0x00, 0x00, 0x00 //crc
};


uint8_t beacon_frame_wpa2[] = { 0x80, 0x00, 0x00, 0x00,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //A1
0x00, 'H',  'A',  'W',  'K',  'B',  //A2
0x00, 'H',  'A',  'W',  'K',  'B',  //A3
0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, //timestamp
0x64, 0x00, //beacon interval
0x31, 0x04, //capability
0x00, 0x03, 'w', 'p', 'a',//SSID
0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
0x03, 0x01, 0x0b, //channel
0x05, 0x04, 0x00, 0x01, 0x00, 0x00, //TIM
0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, //extended support rate
0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, //RSN
0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00,
0x00, 0x0f, 0xac, 0x02, 0x0c, 0x00,
//0x00, 0x00, 0x00, 0x00 //crc
};

uint8_t auth_frame[] = { 0xb0, 0x00, 0x00, 0x00,
0x00, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  'F',  //A2
0x00, 'H',  'A',  'W',  'K',  'F',  //A3
0x01, 0x00,
0x00, 0x00,
0x02, 0x00,
0x00, 0x00,
//0x00, 0x00, 0x00, 0x00 //crc
};

uint8_t auth_frame_wep[] = { 0xb0, 0x00, 0x00, 0x00,
0x00, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  '1',  //A2
0x00, 'H',  'A',  'W',  'K',  '1',  //A3
0x01, 0x00,
0x00, 0x00,
0x02, 0x00,
0x00, 0x00,
//0x00, 0x00, 0x00, 0x00 //crc
};

uint8_t auth_frame_wpa2[] = { 0xb0, 0x00, 0x00, 0x00,
0x00, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  'B',  //A2
0x00, 'H',  'A',  'W',  'K',  'B',  //A3
0x01, 0x00,
0x00, 0x00,
0x02, 0x00,
0x00, 0x00,
//0x00, 0x00, 0x00, 0x00 //crc
};


uint8_t asoc_rsp_frame[] = { 0x10, 0x00, 0x00, 0x00,
0x00, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  'F',  //A2
0x00, 'H',  'A',  'W',  'K',  'F',  //A3
0x02, 0x00,
0x21, 0x04, //capability
0x00, 0x00,
0x01, 0xC0,

//0x00, 0x03, 'W', 'Q', 'f',//SSID
0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, //extended support rate
0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01,
0x00, 0x00, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4,
0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, 0x62, 0x32,
0x2f, 0x00,
//0x00, 0x00, 0x00, 0x00 //crc
};

uint8_t asoc_rsp_frame_wep[] = { 0x10, 0x00, 0x00, 0x00,
0x00, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  '1',  //A2
0x00, 'H',  'A',  'W',  'K',  '1',  //A3
0x02, 0x00,
0x31, 0x04, //capability
0x00, 0x00,
0x01, 0xC0,

0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, //extended support rate
0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01,
0x00, 0x00, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4,
0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, 0x62, 0x32,
0x2f, 0x00,
//0x00, 0x00, 0x00, 0x00 //crc
};

uint8_t asoc_rsp_frame_wpa[] = { 0x10, 0x00, 0x00, 0x00,
0x00, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  'B',  //A2
0x00, 'H',  'A',  'W',  'K',  'B',  //A3
0x02, 0x00,
0x31, 0x04, //capability
0x00, 0x00,
0x01, 0xC0,

0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, //extended support rate

0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, //RSN
0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00,
0x00, 0x0f, 0xac, 0x02, 0x0c, 0x00,

0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02, 0x00, 0x01, //WMM
0x06, 0x40, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4,
0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, 0x62, 0x32,
0x2f, 0x00,
//0x00, 0x00, 0x00, 0x00 //crc
};

uint8_t probe_req_frame[] = { 0x40, 0x00, 0x00, 0x00,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //A1
0x5C, 0xEA, 0x1D, 0x10, 0xB7, 0x33, //A2
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //A3
0x60, 0x5D, 0x00, 0x00, 0x01, 0x04, 0x02, 0x04,
0x0B, 0x16, 0x32, 0x08, 0x0C, 0x12, 0x18, 0x24,
0x30, 0x48, 0x60, 0x6c};

uint8_t sta_disasoc_frame[] = { 0xa0, 0x00, 0x00, 0x00,
0x02, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  's',  //A2
0x02, 'h',  'a',  'w',  'k',  'x',  //A3
0x02, 0x00,

0x00, 0x02,
};

uint8_t sta_auth_frame[] = { 0xb0, 0x00, 0x00, 0x00,
0x02, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  's',  //A2
0x02, 'h',  'a',  'w',  'k',  'x',  //A3
0x01, 0x00,
0x00, 0x00,
0x01, 0x00,
0x00, 0x00,
//0x00, 0x00, 0x00, 0x00 //crc
};

uint8_t sta_asoc_req_frame[] = { 0x00, 0x00, 0x00, 0x00,
0x02, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  's',  //A2
0x02, 'h',  'a',  'w',  'k',  'x',  //A3
0x02, 0x00,
0x21, 0x04, //capability
0x01, 0x00, //listen interval

0x00, 0x03, 'W', 'Q', 'f',//SSID
0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, //extended support rate
//0x00, 0x00, 0x00, 0x00 //crc
};


uint8_t sta_asoc_req_frame_wep[] = { 0x00, 0x00, 0x00, 0x00,
0x02, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  's',  //A2
0x02, 'h',  'a',  'w',  'k',  'x',  //A3
0x02, 0x00,
0x31, 0x04, //capability
0x01, 0x00, //listen interval

0x00, 0x03, 'w', 'e', 'p',//SSID
0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, //extended support rate
//0x00, 0x00, 0x00, 0x00 //crc
};

uint8_t sta_asoc_req_frame_wpa2[] = { 0x00, 0x00, 0x00, 0x00,
0x02, 'h',  'a',  'w',  'k',  'x',  //A1
0x00, 'H',  'A',  'W',  'K',  's',  //A2
0x02, 'h',  'a',  'w',  'k',  'x',  //A3
0x02, 0x00,
0x21, 0x04, //capability
0x01, 0x00, //listen interval

0x00, 0x03, 'w', 'p', 'a',//SSID
0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, //support rate
0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, //extended support rate
0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, //RSN
0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00,
0x00, 0x0f, 0xac, 0x02, 0x0c, 0x00,

0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02, 0x00, 0x01, //WMM
0x06, 0x40, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4,
0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, 0x62, 0x32,
0x2f, 0x00,
//0x00, 0x00, 0x00, 0x00 //crc
};


void wq_mac_rx_done_work(void)
{
    struct ieee80211com *ic = net80211_ic;
	struct wq_softc *sc = ic->ic_softc;
    struct mbuf *m = NULL;
    struct ieee80211_qosframe *wh;
    struct ieee80211_node *ni;
    struct htc_data_desc *rx_data_desc;
    int buf_len;
    struct sk_buff * skb;
    unsigned long flags;
#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
    int rx_done_count_wk = 0;
#endif

    while (1)
    {
        spin_lock_irqsave(&sc->rx_skb_lock, flags);

        skb = rx_skb_head;
        if (skb != NULL)
        {
            rx_skb_count--;
            rx_skb_head = *((struct sk_buff **)skb->cb);
            if (rx_skb_head == NULL)
                rx_skb_tail = NULL;
        }
        spin_unlock_irqrestore(&sc->rx_skb_lock, flags);

        if (skb == NULL){
            break;
        }

        rx_data_desc = (struct htc_data_desc *)skb->data;
#if (WQ_CONFIG_64 == 1)
        buf_len = skb->len-HTC_DATA_DESC_LEN + 2*diff_pointer_sizes;
        skb_pull(skb, HTC_DATA_DESC_LEN - 2*diff_pointer_sizes);//remove HTC_DATA_DESC
#else
        buf_len = skb->len-HTC_DATA_DESC_LEN;
        skb_pull(skb, HTC_DATA_DESC_LEN);
#endif
        if (rx_data_desc->info.rx.chain_seq)
        {
            //printk("%s:%d amsdu subframe, rx_data_desc->info.rx.chain_seq:%d, skb->len:%d, ic->next_chain_seq:%d\n",
            //        __func__,__LINE__,rx_data_desc->info.rx.chain_seq,skb->len,ic->next_chain_seq);
            if (rx_data_desc->info.rx.chain_seq == 1)
            {
                //allocate big skb (max size is 3893)
                ic->skb_amsdu = wq_alloc_skb_amsdu(SKB_MAX_LEN);
                if(ic->skb_amsdu == NULL)
                {
                    wq_free_skb(skb);
                    //printk("%s:%d allocte skb_amsdu fail, free first amsdu subframe\n",__func__,__LINE__);
                    continue;
                }

                //copy data to big skb
                os_mem_cpy(ic->skb_amsdu->data, skb->data, skb->len);
                skb_put(ic->skb_amsdu, skb->len);
                ic->next_chain_seq = 2;
                wq_free_skb(skb);
                continue;
            } else {
                if (rx_data_desc->info.rx.chain_seq == ic->next_chain_seq)
                {
                    //copy data to the big skb
                    os_mem_cpy(ic->skb_amsdu->data + ic->skb_amsdu->len, skb->data, skb->len);
                    skb_put(ic->skb_amsdu, skb->len);
                    ic->next_chain_seq++;
                    wq_free_skb(skb);

                    if (rx_data_desc->info.rx.is_next==0)
                    {
                        skb = ic->skb_amsdu;
                        ic->next_chain_seq = 0;
                        ic->skb_amsdu = NULL;
                        buf_len = skb->len;
                    } else {
                         continue;
                    }
                }else{
                    //printk("%s:%d allocte skb_amsdu fail, free middle/last amsdu subframe\n",__func__,__LINE__);

                    ic->next_chain_seq = 0;
                    wq_free_skb(skb);
                    continue;
                }

            }
        }

        m = _m_gethdr_with_skb(M_NOWAIT, MT_DATA, skb, buf_len);
        if (m == NULL)
        {
            iot_printf("m_gethdr failed");
            wq_free_skb(skb);
            break;
        }

        //AMSDU
        if (rx_data_desc->info.rx.chain_seq)
            m->m_flags_ext |= M_AMSDU;

        //record rx rssi and snr
        m->c_nf = rx_data_desc->info.rx.nf;
        m->c_rssi = rx_data_desc->info.rx.rssi;

        wh = mtod(m, struct ieee80211_qosframe *);


        ni = ieee80211_find_rxnode(ic, (const struct ieee80211_frame_min *)wh);
        if (ni != NULL) {
            /* tag AMPDU aggregates for reorder processing */
            if (ni->ni_flags & IEEE80211_NODE_HT) {
                m->m_flags |= M_AMPDU;
            }
            ieee80211_input(ni, m, m->c_rssi, m->c_nf);
            ieee80211_free_node(ni);
        } else {
            ieee80211_input_all(ic, m, m->c_rssi, m->c_nf);
        }

#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
#if LINUX_VERSION_CODE == KERNEL_VERSION(4,4,192)
        if ((rx_skb_count < submit_rxurb_limit_l) && (!reset_down_flag)) {
        //sumbit rx urb
            wq_sche_pktin_work_ex(sc->sc_wq_ipc);
        }
#else
        if (rx_skb_count < submit_rxurb_limit_l) {
            //sumbit rx urb
            wq_sche_pktin_work_ex(sc->sc_wq_ipc);
        }
#endif
        rx_done_count_wk++;
        if (rx_done_count_wk >= rx_done_count_wk_limit) {
            wq_mac_tx_done_work();
            wq_mac_tx_work(WQ_MAC_TXQ_SEND);
            rx_done_count_wk = 0;
        }
#endif

    }
}

static void wq_eapol_comp(struct ieee80211_node *ni, void *arg, int status)
{
    struct ieee80211vap *vap = (struct ieee80211vap *)arg;
    complete(&vap->iv_wait);
}

void wq_mac_tx_work(uint8_t action)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct mbuf *m;
    unsigned long flags;
    struct ieee80211_node *ni = NULL;
    struct ieee80211vap *vap = NULL;
    const struct ether_header *eh = NULL;

    while (1)
    {
        //get from tx_mbuf queue
        spin_lock_irqsave(&sc->tx_mbuf_lock, flags);

        m = tx_mbuf_head;
        if (m != NULL)
        {
            tx_mbuf_head = m->txq_next;
            if (tx_mbuf_head == NULL)
                tx_mbuf_tail = NULL;

            m->txq_next = NULL;
        }

        spin_unlock_irqrestore(&sc->tx_mbuf_lock, flags);

        //iot_printf("wq_mac_tx_work %p\n", m);

        if (m != NULL)
        {

            eh = mtod(m, struct ether_header *);
            /* For the race condition of m4 eapol and key install issue.
             * We check here if key is valid before push eapol to tx queue.
             */
            if (eh->ether_type == htons(ETHERTYPE_PAE)) {
                vap = m->vap;
                ni = ieee80211_find_txnode(vap, eh->ether_dhost);
                if (ni) {
                    iot_printf("%s: ETHERTYPE_PAE, node mac %pM\n", __func__, ni->ni_macaddr);
                    ni->ni_recv_eapol = 1;
                    ieee80211_free_node(ni);
                }
            }

            if (action == WQ_MAC_TXQ_SEND)
                ieee80211_vap_transmit(m->vap->iv_ifp, m);
            else
                m_freem(m);
        }
        else
        {
            break;
        }
    }

    return;
}

void wq_mac_put_to_tx_queue(struct mbuf *m)
{
	struct ieee80211com *ic = net80211_ic;
	struct wq_softc *sc = ic->ic_softc;
	struct wuqi_work_struct *wk;
    const struct ether_header *eh = NULL;

    m->txq_next = NULL;

    eh = mtod(m, struct ether_header *);
    /* For the race condition of m4 eapol and key install issue.
     * We check here if key is valid before push eapol to tx queue.
     */
    if (eh->ether_type == htons(ETHERTYPE_PAE)) {
        struct ieee80211vap *vap = m->vap;

        if (vap->iv_bss && (vap->iv_opmode == IEEE80211_M_STA)) {
            /* keep the order of EAPOL tx and key install. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
            reinit_completion(&vap->iv_wait);
#else
            (&vap->iv_wait)->done = 0;
#endif
            ieee80211_add_callback(m, NULL, wq_eapol_comp, vap);
        }

    }

    //put to txdone_mbuf queue
	//spin_lock_irqsave(&rx_skb_lock, flags);
    spin_lock(&sc->tx_mbuf_lock);

	if (tx_mbuf_head == NULL)
	{
		tx_mbuf_head = m;
	    tx_mbuf_tail = m;
	}
	else
	{
		tx_mbuf_tail->txq_next = m;
        tx_mbuf_tail = m;
	}

	//spin_unlock_irqrestore(&rx_skb_lock, flags);
	spin_unlock(&sc->tx_mbuf_lock);

	//schedule rx work
    wk = &sc->sc_rxq_process_task;
    wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)(1));

    return;
}

void wq_txrx_proc(struct work_struct * work)
{
    //struct wuqi_work_struct *wuqi_wk = container_of(work, struct wuqi_work_struct, work);
    //uint32_t rxq_intr_status = (uint32_t)wuqi_wk->argv;

#ifdef WQ_SIM_HOST_ONLY
    extern uint8_t tx_frame_ctrl;
    extern uint8_t tx_auth_frame_seq;

    //simulate received probe rsp frame
    struct mbuf *m = NULL;
    struct ieee80211_qosframe *wh;
    struct ieee80211_node *ni;
    struct ieee80211com *ic = net80211_ic;

    if (tx_frame_ctrl == 0x80)
    {
        iot_printf("0x%x, trigger disassoc\n",tx_frame_ctrl);
        m = m_gethdr(M_NOWAIT, MT_DATA);

        if (m != NULL)
        {
            memcpy(m->m_data, sta_disasoc_frame, sizeof(sta_disasoc_frame));
            m->m_len = sizeof(sta_disasoc_frame);
        }
        else
        {
            iot_printf(" m=null, trigger disassoc req\n");
        }
    }
    else if (tx_frame_ctrl == 0xff)
    {
        iot_printf("0xff, trigger probe req\n");
        m = m_gethdr(M_NOWAIT, MT_DATA);

        if (m != NULL)
        {
            memcpy(m->m_data, probe_req_frame, sizeof(probe_req_frame));
            m->m_len = sizeof(probe_req_frame);
        }
        else
        {
            iot_printf(" m=null, trigger probe req\n");
        }
    }
    else if (!((ic->ic_rf_chan == 1) || (ic->ic_rf_chan == 6) || (ic->ic_rf_chan == 11)))
    {
        return;
    }

    if ((tx_frame_ctrl == 0x40) || (tx_frame_ctrl == 0x48)) //probe req and null
    {
        iot_printf("tx probe req, trigger rx beacon\n");
        m = m_gethdr(M_NOWAIT, MT_DATA);

        if (ic->ic_rf_chan == 6)
        {
            memcpy(m->m_data, beacon_frame_open, sizeof(beacon_frame_open));
            m->m_len = sizeof(beacon_frame_open);
        }
        else if (ic->ic_rf_chan == 1)
        {
            memcpy(m->m_data, beacon_frame_wep, sizeof(beacon_frame_wep));
            m->m_len = sizeof(beacon_frame_wep);
        }
        else if (ic->ic_rf_chan == 11)
        {
            memcpy(m->m_data, beacon_frame_wpa2, sizeof(beacon_frame_wpa2));
            m->m_len = sizeof(beacon_frame_wpa2);
        }
    }
    else if (tx_frame_ctrl == 0xb0) //auth
    {
        iot_printf("tx auth-%d\n", tx_auth_frame_seq);

        if (tx_auth_frame_seq == 1)
        {
            iot_printf("tx auth-1(sta mode), trigger auth-2\n");

            m = m_gethdr(M_NOWAIT, MT_DATA);

            if (ic->ic_rf_chan == 6)
            {
                memcpy(m->m_data, auth_frame, sizeof(auth_frame));
                m->m_len = sizeof(auth_frame);
            }
            else if (ic->ic_rf_chan == 1)
            {
                memcpy(m->m_data, auth_frame_wep, sizeof(auth_frame_wep));
                m->m_len = sizeof(auth_frame_wep);
            }
            else if (ic->ic_rf_chan == 11)
            {
                memcpy(m->m_data, auth_frame_wpa2, sizeof(auth_frame_wpa2));
                m->m_len = sizeof(auth_frame_wpa2);
            }
        }
        else if (tx_auth_frame_seq == 2)
        {
            iot_printf("tx auth-2(ap mode), trigger rx assoc req\n");
            m = m_gethdr(M_NOWAIT, MT_DATA);

            if (ic->ic_rf_chan == 6)
            {
                memcpy(m->m_data, sta_asoc_req_frame, sizeof(sta_asoc_req_frame));
                m->m_len = sizeof(sta_asoc_req_frame);
            }
            else if (ic->ic_rf_chan == 1)
            {
                memcpy(m->m_data, sta_asoc_req_frame_wep, sizeof(sta_asoc_req_frame_wep));
                m->m_len = sizeof(sta_asoc_req_frame_wep);
            }
            else if (ic->ic_rf_chan == 11)
            {
                memcpy(m->m_data, sta_asoc_req_frame_wpa2, sizeof(sta_asoc_req_frame_wpa2));
                m->m_len = sizeof(sta_asoc_req_frame_wpa2);
            }
        }
    }
    else if (tx_frame_ctrl == 0x0) //asoc req
    {
        iot_printf("tx asoc req, trigger asoc rsp\n");

        m = m_gethdr(M_NOWAIT, MT_DATA);

        if (ic->ic_rf_chan == 6)
        {
            memcpy(m->m_data, asoc_rsp_frame, sizeof(asoc_rsp_frame));
            m->m_len = sizeof(asoc_rsp_frame);
        }
        else if (ic->ic_rf_chan == 1)
        {
            memcpy(m->m_data, asoc_rsp_frame_wep, sizeof(asoc_rsp_frame_wep));
            m->m_len = sizeof(asoc_rsp_frame_wep);
        }
        else if (ic->ic_rf_chan == 11)
        {
            memcpy(m->m_data, asoc_rsp_frame_wpa, sizeof(asoc_rsp_frame_wpa));
            m->m_len = sizeof(asoc_rsp_frame_wpa);
        }
    }
    else if (tx_frame_ctrl == 0x50) //probe rsp
    {
        iot_printf("tx probe rsp, trigger auth req\n");
        m = m_gethdr(M_NOWAIT, MT_DATA);

        memcpy(m->m_data, sta_auth_frame, sizeof(sta_auth_frame));
        m->m_len = sizeof(sta_auth_frame);
    }



    if (m != NULL)
    {
        wh = mtod(m, struct ieee80211_qosframe *); //4B mbuf pointer + 4B magic pattern + RxDESC size

        ni = ieee80211_find_rxnode(ic, (const struct ieee80211_frame_min *)wh);
        if (ni != NULL) {
            /* tag AMPDU aggregates for reorder processing */
            if (ni->ni_flags & IEEE80211_NODE_HT) {
                m->m_flags |= M_AMPDU;
            }
            ieee80211_input(ni, m, 0, 0);
            ieee80211_free_node(ni);
        } else {
            ieee80211_input_all(ic, m, 0, 0);
        }
    }

#else
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    //process rx done packet
    wq_mac_rx_done_work();

    //procsss tx done packet
    wq_mac_tx_done_work();

    //procsss tx packet
    wq_mac_tx_work(WQ_MAC_TXQ_SEND);

    /* process pending cmd queue */
    wq_mac_cmd_work(WQ_MAC_CMDQ_SEND);

    /* process postpond tx frames */
    if (txpostpond_mbuf_head && (sc->sc_tx_postpond == 0)) {
        wq_tx_postpond_work(WQ_MAC_POSTPONE_TXQ_SEND);
    }

    return;

#endif
}

void flush_event_rx_skb_queue(void)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct sk_buff * skb;
    unsigned long flags;

    while (1)
    {
        spin_lock_irqsave(&sc->event_rx_skb_lock, flags);

        skb = event_rx_skb_head;
        if (skb != NULL) {
            event_rx_skb_head = *((struct sk_buff **)skb->cb);
            if (event_rx_skb_head == NULL)
                event_rx_skb_tail = NULL;
        }
        spin_unlock_irqrestore(&sc->event_rx_skb_lock, flags);

        if (skb != NULL)
            wq_free_skb(skb);
        else
            break;
    }
}

void wq_node_inact_reload(struct ieee80211com *ic, const uint8_t node_idx)
{
    struct ieee80211_node *ni;

    ni = (struct ieee80211_node *)get_ni_from_idx(ic, node_idx);
    if (ni != NULL) {
        ni->ni_inact = ni->ni_inact_reload;
    }
}

void wq_ack_proc(struct ieee80211com *ic, uint8_t *data)
{
    struct ack_payload *payload = (struct ack_payload *)data;

    switch (payload->type)
    {
        case WQ_ACK_INACT_RELOAD:
            if(payload->succ) {
                 wq_node_inact_reload(ic, payload->mnid);
            }
            break;
        default:
            break;
    }
}

// writing calibration data to tp flash
void wq_write_cali(uint8_t *data)
{
    //int ret = 0;
    struct cali_info *cali = (struct cali_info *) data;

    dump_bytes("wq_write_cali", cali->cali_data, cali->cali_len);
    //ret = tp_write_cali_flash(cali->cali_data, cali->cali_len);
    //printk("%s:%d ret = %d\n",__func__,__LINE__, ret);

    return;
}

// sending calibration data from tp flash to fw
void wq_read_cali(struct wq_softc *sc)
{
    struct cali_info cali;
    //struct cali_info *cali_tp;
    uint8_t cali_data_fake[256] = { 0x01,};

    /*
    cali_tp = tp_read_cali_flash();
    if(cali_tp->cali_len > 0 && cali_tp->cali_len < MAX_CALI_LEN)
    {
        cali.cali_len = cali_tp->cali_len;
        memcpy(cali.cali_data, cali_tp->cali_data, cali.cali_len);
    } else {
        printk("%s:%d: read fail\n",__func__,__LINE__);
        return;
    }*/

    memset(cali_data_fake, 0x01, 256);
    cali.cali_len = 256;
    memcpy(cali.cali_data, cali_data_fake, cali.cali_len);
    dump_bytes("cali data from tp",cali.cali_data, 256);
    
    htc_cali_info(sc, cali.cali_data, cali.cali_len);

    return;
}

void wq_mp_tone_notify(uint8_t enable, uint16_t freq, uint8_t num, uint16_t amp)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct mp_tone_gen_t param;

    param.enable = enable;
    param.freq = freq;
    param.amp = amp;
    param.num = num;

    htc_mp_set_notify(sc, MP_TONE_NOTIFY, (uint8_t *)&param, sizeof(param));

    return;
}

int wq_mp_freq_dev_notify(uint8_t set, uint16_t *freq_dev)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    int ret = 0;

    if (set == 1) {
        struct mp_freq_dev_t param;
        param.freq_dev = *freq_dev;
        htc_mp_set_notify(sc, MP_FREQ_DEV_NOTIFY, (uint8_t *)&param, sizeof(param));
    } else {
        uint16_t buf;
        ret = htc_mp_get_notify(sc, MP_FREQ_DEV_NOTIFY, NULL, 0, (uint8_t *)&buf, sizeof(buf));
        *freq_dev = buf;
    }

    return 0;
}

int wq_mp_power_notify(uint8_t set, uint8_t rate_idx, int8_t *power)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct mp_power_t param;

    param.rate_idx = rate_idx;
    param.power = *power;
    if (set == 1) {
        htc_mp_set_notify(sc, MP_POWER_NOTIFY, (uint8_t *)&param, sizeof(param));
    } else {
        int ret;
        int8_t buf;
        ret = htc_mp_get_notify(sc, MP_POWER_NOTIFY, (uint8_t *)&param, sizeof(struct mp_power_t), (uint8_t *)&buf, sizeof(buf));
        if (ret) {
            return -1;
        }
        *power = buf;
    }

    return 0;
}

int wq_mp_mac_notify(uint8_t set, uint8_t *mac_addr)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    if (set == 1) {
        htc_mp_set_notify(sc, MP_MAC_ADDR_NOTIFY, mac_addr, ETH_ALEN);
    } else {
        int ret;
        ret = htc_mp_get_notify(sc, MP_MAC_ADDR_NOTIFY, NULL, 0, mac_addr, ETH_ALEN);
        if (ret) {
            return -1;
        }
    }

    return 0;
}

int wq_rate_pwr_notify(int8_t *pwr_list)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    htc_mp_set_notify(sc, MP_RATE_PWR_NOTIFY, (uint8_t *)pwr_list, 12);

    return 0;
}

int wq_rate_pwr_srrc_notify(int8_t *pwr_list)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    htc_mp_set_notify(sc, MP_RATE_PWR_SRRC_NOTIFY, (uint8_t *)pwr_list, 24);

    return 0;
}

int wq_mp_efuse_notify(uint8_t set, struct wuqi_mptest_efuse_ctx *efuse_ctx)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    if (set == 1) {
        htc_mp_set_notify(sc, MP_EFUSE_NOTIFY, efuse_ctx->efuse_info, (uint16_t)efuse_ctx->info_len);
    } else {
        int ret;
        ret = htc_mp_get_notify(sc, MP_EFUSE_NOTIFY, efuse_ctx->efuse_info, 1, efuse_ctx->rsp_efuse_info, (uint16_t)efuse_ctx->info_len);
        if (ret) {
            return -1;
        }
    }

    return 0;
}

int wq_mp_cali_efuse_notify(int set, uint8_t *cali_efuse, uint8_t cali_efuse_len)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    if (set == 1) {
        htc_mp_set_notify(sc, MP_EFUSE_CALIB_NOTIFY, NULL, 0);
    } else {
        int ret;
        ret = htc_mp_get_notify(sc, MP_EFUSE_CALIB_NOTIFY, NULL, 0, cali_efuse, cali_efuse_len);
        if (ret) {
            return -1;
        }
    }

    return 0;
}

int wq_mp_socinfo_notify(uint8_t *soc_info)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    int ret;
    ret = htc_mp_get_notify(sc, MP_SOC_INFO_NOTIFY, NULL, 0, soc_info, MP_SOC_INFO_LEN);
    if (ret) {
        return -1;
    }

    return 0;
}

int wq_mp_cali_remain_times_notify(uint8_t *remain_times)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    int ret;

    ret = htc_mp_get_notify(sc, MP_CALI_REMAIN_TIMES_NOTIFY, NULL, 0, remain_times, 1);
    if (ret) {
        return -1;
    }

    return 0;
}

int wq_mp_crc_err_notify(uint32_t *crc_err)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    int ret;
    uint32_t buf;
    ret = htc_mp_get_notify(sc, MP_CRC_ERR_NOTIFY, NULL, 0, (uint8_t *)&buf, sizeof(buf));
    if (ret) {
        return -1;
    } else {
        *crc_err = buf;
        printk("crc err from fw=%u", *crc_err);
    }

    return 0;
}

int wq_mp_power_ctrl_notify(uint8_t *power_ctrl)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct mp_power_ctrl_t param;

    param.power_ctrl = *power_ctrl;
    htc_mp_set_notify(sc, MP_POWER_CTRL_NOTIFY, (uint8_t *)&param, sizeof(param));

    return 0;
}

int wq_mp_dpd_ctrl_notify(uint8_t *dpd_ctrl)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct mp_dpd_ctrl_t param;

    param.dpd_ctrl = *dpd_ctrl;
    htc_mp_set_notify(sc, MP_DPD_CTRL_NOTIFY, (uint8_t *)&param, sizeof(param));

    return 0;
}

int wq_mp_digit_gain_notify(uint8_t mode, uint16_t freq)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct mp_digit_gain_t param;

    param.mode = mode;
    param.freq = freq;
    htc_mp_set_notify(sc, MP_DIGIT_GAIN_NOTIFY, (uint8_t *)&param, sizeof(param));

    return 0;
}

int wq_mp_status_notify(uint8_t status)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    htc_mp_set_notify(sc, MP_STATUS_NOTIFY, &status, sizeof(status));

    return 0;
}

int wq_mp_gain_notify(uint8_t gain_info)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    htc_mp_set_notify(sc, MP_GAIN_INFO_NOTIFY, &gain_info, sizeof(gain_info));
    return 0;
}

int wq_mp_get_dpdgain_notify(uint8_t *dpd_gain_info)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    int ret;
    ret = htc_mp_get_notify(sc, MP_GAIN_INFO_NOTIFY, NULL, 0, dpd_gain_info, 12);
    if (ret) {
        return -1;
    }
    return 0;
}

int wq_mp_gain_ctrl_notify(uint8_t gain_ctrl_info)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    uint8_t gain_info[12];
    int ret;

    htc_mp_set_notify(sc, MP_GAIN_CTRL_INFO_NOTIFY, &gain_ctrl_info, sizeof(gain_ctrl_info));
    ret = wq_mp_get_dpdgain_notify(gain_info);
    if (ret) {
        printk("read dpd gain info fail\n");
        return -1;
    }
    wuqi_change_channel_dpd();
    return 0;
}

int wq_mp_kfree_cali_notify(char *ver, char *cont)
{	
	char *src_ver = ver;
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
	struct mp_kfree_cali_t param;
	int i, ret;
	
	while (*ver != '.' && *ver != '\0') {
		ver++;
	}
	*ver = 0;
	ret = kstrtou8(src_ver, 10, &param.version); 
	
	for (i = 0; i < strlen(cont); i += 3) {
		*(cont + i + 2) = 0;
		ret = kstrtou8(cont + i, 16, &param.cali_efuse[i/3]);
	}
	
	iot_printf("version:%d,cali efuse from WQ9001_WiFi_Mapfile.dat 0x%02x%02x%02x%02x%02x\n", param.version, param.cali_efuse[4], param.cali_efuse[3],
		param.cali_efuse[2], param.cali_efuse[1], param.cali_efuse[0]);

    htc_mp_set_notify(sc, MP_KFREE_CALI_NOTIFY, (uint8_t *)&param, sizeof(param));

    return 0;
}

int wq_mp_set_gain_notify(uint32_t *ref_gain)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct mp_set_ref_gain_t param;
    uint8_t gain_info[12];
    int ret;

    memcpy(param.ref_gain, ref_gain, 12);

    htc_mp_set_notify(sc, MP_SET_REF_GAIN_NOTIFY, (uint8_t *)&param, sizeof(param));
    ret = wq_mp_get_dpdgain_notify(gain_info);
    if (ret) {
        printk("read dpd gain info fail\n");
        return -1;
    }
    wuqi_change_channel_dpd();
    return 0;
}

const char mgmt_subtype_to_str[16][10] =
{
	"ASOCREQ", //00 0000
	"ASOCRSP", //00 0001
	"REASOCREQ", //00 0010
	"REASOCRSP", //00 0011
	"PROBEREQ", //00 0100
	"PROBERSP", //00 0101
	"TA", //00 0110
	"RSVD", //00 0111
	"BEACON", //00 1000
	"ATIM", //00 1001
	"DISASSOC", //00 1010
	"AUTH", //00 1011
	"DEAUTH", //00 1100
	"ACTION", //00 1101
	"ACTIONNA", //00 1110
	"RSVD", //00 1111

};

const char* frame_type_to_str(uint8_t* frame_ctrl, int is_ampdu)
{
	int amsdu_bit;
	int qos_ctrl_offset;
	
	
	if ((frame_ctrl[0] & 0xc) == 0)
	{
		return &mgmt_subtype_to_str[(frame_ctrl[0]>>4)][0];
	}
    else if ((frame_ctrl[0] & 0xc) == 0x4)
	{
		return "CTRL";
	}
	else if ((frame_ctrl[0] & 0xc) == 0x8)
	{		
		if ((frame_ctrl[0] & 0xf0) == 0x80) //QoS frame
		{
			qos_ctrl_offset = 24;
		    if (frame_ctrl[1] & 0x3){ //toDS and frDS both 1 : 4-address frame
		        qos_ctrl_offset = 30;
            }
				
			amsdu_bit = frame_ctrl[qos_ctrl_offset] >> 7; //qos-ctrl bit7 : A-MSDU bit
		
		    if (amsdu_bit)
				return "AMSDU";
		    else if (is_ampdu)
		        return "AMPDU";		
			else
				return "QDATA";
		}
		else if ((frame_ctrl[0] & 0xf0) == 0x00) //data frame
	        return "DATA";
		else
			return "D-TBD";
	}
	//else if ((frame_ctrl & 0xc) == 0x8)
	return "RESERVED";
}

extern uint32_t ipc_tx_seq;
extern uint32_t ipc_tx_done_seq;
extern uint32_t ipc_pre_check_seq;
int time_record = 0;

void usb_tx_pkt_check(void) {

    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;

    int time_diff;
    uint32_t tx_seq = ipc_tx_seq;
    uint32_t tx_done_seq = ipc_tx_done_seq;
    uint32_t pre_check_seq = ipc_pre_check_seq;

    if(pre_check_seq == tx_done_seq && tx_seq != tx_done_seq) {
        iot_printf("[%s] tx_pkt_check! tx_seq = %d / tx_done_seq = %d / pre_check_seq =%d \n", __func__, tx_seq, tx_done_seq, pre_check_seq);

        if(time_record == 0) {
            time_record = ticks_to_secs(ticks);
            iot_printf("[%s] first time_record :%d\n", __func__, time_record);
        } else if (time_record > 0) {
            time_diff = ticks_to_secs(ticks) - time_record;
            if(time_diff > 10) {
                iot_printf("[%s] time_diff > 10 sec, do trigger pattern\n", __func__);
                wq_ipc_send_trigger_pattern(sc->sc_wq_ipc, WQ_USB_TRI_TXPKT_CHK, (ipc_tx_seq & 0xFFFF));
                time_record = 0;
            }
        } else {
            iot_printf("[%s] abnormal\n", __func__);
        }
    } else {
        time_record = 0; //normal process, reset time_record
    }

    //update last check tx seq
    ipc_pre_check_seq = ipc_tx_done_seq;
}

void packet_dump_evt_handler(struct ieee80211com *ic, uint8_t *data)
{
    WIFI_DBG_PKT *pkt;
    const struct ieee80211_frame *wh;
    unsigned int ether_offset = sizeof(struct ieee80211_frame);
    unsigned char eth_str[12] = {0};
	uint32_t ts0, ts1, ts2;

    pkt = (WIFI_DBG_PKT *)data;
    wh = (struct ieee80211_frame *) pkt->pkt_data;

    if (((wq_dbg_flag & WQ_DEBUG_RX_MGMT_DUMP) && (pkt->pkt_data[0] & 0xc) == 0) || (wq_dbg_flag & WQ_DEBUG_RX_DATA_DUMP))
    {
        if (IEEE80211_IS_DATA(wh) && (pkt->len > ether_offset)) {
            if (IEEE80211_IS_QOSDATA(wh))
                ether_offset += IEEE80211_QOS_CTL_LEN;
            if (wh->i_fc[1] & IEEE80211_FC1_PROTECTED) {
                ether_offset += IEEE80211_WEP_IVLEN + IEEE80211_WEP_KIDLEN;
                if (pkt->pkt_data[ether_offset-1] & IEEE80211_WEP_EXTIV)
                    ether_offset += IEEE80211_WEP_EXTIVLEN;
            }
            ether_offset += 6; /* Ignore SNAP/LLC header */
            if (pkt->len >= ether_offset)
                snprintf(eth_str, 12, "eth: %02x%02x", pkt->pkt_data[ether_offset], pkt->pkt_data[ether_offset+1]);
        }

        if (pkt->dir == WIFI_DBG_PKT_TX) {
            //printk("tx time => fw:%d  hw:%d  hw done:%d\n", pkt->driver_ts, pkt->hal_ts, pkt->mac_ts);
			ts0 = pkt->driver_ts;
			ts1 = pkt->hal_ts - pkt->driver_ts;
			ts2 = pkt->mac_ts - pkt->hal_ts;
        } else {
            //printk("rx time => fw:%d\n", pkt->driver_ts);
			ts0 = pkt->driver_ts;
			ts1 = ts2 = 0;
        }
		
        printk("%03d %c%d stat=%x:%d %02x %02x%02x(%s%c) %02x%02x %02x%02x%02x%02x%02x%02x %02x%02x%02x%02x%02x%02x ... %02x%02x %s TS:%d %d %d\n",
            pkt->sn, pkt->dir==WIFI_DBG_PKT_TX?'T':'R', pkt->len, pkt->cs, pkt->frame_cnt,
            pkt->rate,
            pkt->pkt_data[0], pkt->pkt_data[1],
            frame_type_to_str(pkt->pkt_data, pkt->is_ampdu),
            pkt->pkt_data[1]&0x8?'+':' ',
            pkt->pkt_data[2], pkt->pkt_data[3],
            pkt->pkt_data[4], pkt->pkt_data[5], pkt->pkt_data[6], pkt->pkt_data[7], pkt->pkt_data[8], pkt->pkt_data[9],
            pkt->pkt_data[10], pkt->pkt_data[11], pkt->pkt_data[12], pkt->pkt_data[13], pkt->pkt_data[14], pkt->pkt_data[15],
            pkt->pkt_data[22], pkt->pkt_data[23], eth_str, ts0, ts1, ts2);

    }

        usb_tx_pkt_check();
}

extern u8 g_trigger_flag;

void wq_event_rxq_proc(struct work_struct * work)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct mbuf *m = NULL;
    struct sk_buff *skb;
    u32 buf_len = 0, ipc_buf_len = 0, ipc_hdr_len = 0, id = 0;
    unsigned long flags;

    while (1)
    {
        spin_lock_irqsave(&sc->event_rx_skb_lock, flags);

        skb = event_rx_skb_head;
        if (skb != NULL) {
            event_rx_skb_count--;
            event_rx_skb_head = *((struct sk_buff **)skb->cb);
            if (event_rx_skb_head == NULL)
                event_rx_skb_tail = NULL;
        }
        spin_unlock_irqrestore(&sc->event_rx_skb_lock, flags);

        if (skb == NULL)
            break;

        wq_ipc_event_hdr_parse(skb, &id, &ipc_buf_len, &ipc_hdr_len);
        IOT_ASSERT(skb->len >= ipc_hdr_len);
        skb_pull(skb, ipc_hdr_len);
        buf_len = skb->len;

        m = _m_gethdr_with_skb(M_NOWAIT, MT_DATA, skb, buf_len);
        if (m == NULL) {
            iot_printf("m_gethdr failed");
            wq_free_skb(skb);
            return;
        }

        switch (id)
        {
            case WQ_IPC_EVT_BMISS:
                break;
            case WQ_IPC_EVT_DISCONNECT:
                break;
            case WQ_IPC_EVT_MIC_ERROR:
                wq_handlemicerror(ic, m->m_data);
                break;
            case WQ_IPC_EVT_ACK_RETURN:
                atomic_dec(&sc->sc_tx_fw_pending);
                atomic_add(1, &fw_ack_cnt);  //stat number of fw ack
                wq_tx_sche(sc);
                wq_ack_proc(ic, m->m_data);
                break;
            case WQ_IPC_EVT_FW_LOG:
                m->m_data[ipc_buf_len] = 0;
                //printk("fwlog: ipc_buf_len=%d, buf_len=%d\n", ipc_buf_len, buf_len);
                printk("fwlog: %s\n", m->m_data);
                break;
            case WQ_IPC_EVT_PACKET_DUMP:
                if(wq_dbg_flag & WQ_DEBUG_RX_DATA_DUMP || wq_dbg_flag & WQ_DEBUG_RX_MGMT_DUMP)
                    packet_dump_evt_handler(ic, m->m_data);
                break;
            case WQ_IPC_EVT_TRIGGER_PATTERN:
                //send trigger frame
                g_trigger_flag++;
                printk("%s: usb trigger - cnt=%u,  data=0x%x\n", __func__, g_trigger_flag, *((u16 *)(m->m_data)));
                wq_ipc_send_trigger_pattern(sc->sc_wq_ipc, WQ_USB_TRI_EVENT, *((u16 *)(m->m_data)));
                break;
            case WQ_IPC_EVT_WRITE_CALI:
                wq_write_cali(m->m_data);
                break;
            case WQ_IPC_EVT_READ_CALI:
                wq_read_cali(sc);
                break;
            default:
                break;
        };

        /* skb will be freed too, because m->skb == skb */
        m_freem(m);
    }
}

void wq_mac_event_handle(void *wq_mac, u32 buf_len, struct sk_buff *skb)
{
    struct sk_buff **next_skb;
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct wuqi_work_struct *wk;
    u32 ipc_buf_len = 0, ipc_hdr_len = 0, id = 0;
    static int count = 0;
    static unsigned long lastprint_time = 0;

    wq_ipc_event_hdr_parse(skb, &id, &ipc_buf_len, &ipc_hdr_len);
    if(event_rx_skb_count >= 20 && id == WQ_IPC_EVT_PACKET_DUMP)
    {
        count++;
        if ((ticks - lastprint_time > hz ) || (lastprint_time == 0))
        {
            iot_printf("[%s] pkt_dump_event drop!!! event_rx_skb_count: %d, the number of pkt_dump_event_drop: %d\n", __func__, event_rx_skb_count, count);
            lastprint_time = ticks;
            count = 0;
        }
        wq_free_skb(skb);
        return;
    }

	if (sc == NULL) {
        iot_printf("%s, sc is null. Free buffer and return\n", __func__);
		wq_free_skb(skb);
        return;
    }
	
    WARN_ON(buf_len > skb->len);
    *((struct sk_buff **)skb->cb) = NULL;

    spin_lock(&sc->event_rx_skb_lock);

    event_rx_skb_count++;

    if (event_rx_skb_head == NULL) {
        event_rx_skb_head = skb;
        event_rx_skb_tail = skb;
    } else {
        next_skb = (struct sk_buff **)event_rx_skb_tail->cb;
        *next_skb = skb;
        event_rx_skb_tail = skb;
    }

    spin_unlock(&sc->event_rx_skb_lock);

    wk = &sc->sc_event_rxq_process_task;
    wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)(1));
    return;
}

void flush_rx_skb_queue(void)
{
	struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
	struct sk_buff * skb;
    unsigned long flags;

	while (1)
	{
		spin_lock_irqsave(&sc->rx_skb_lock, flags);

		skb = rx_skb_head;
		if (skb != NULL)
		{
            rx_skb_head = *((struct sk_buff **)skb->cb);
            if (rx_skb_head == NULL)
				rx_skb_tail = NULL;
		}
		spin_unlock_irqrestore(&sc->rx_skb_lock, flags);

		if (skb != NULL)
			wq_free_skb(skb);
		else
			break;
	}
}

void wq_mac_rx_pkt(void *wq_mac, struct sk_buff *skb, u32 buf_len)
{
	struct sk_buff **next_skb;
    struct htc_data_desc *temp_rx_data_desc;
	struct ieee80211com *ic = net80211_ic;
	struct wq_softc *sc = ic->ic_softc;
	struct wuqi_work_struct *wk;
    //unsigned long flags;

    static int count = 0;
    static unsigned long lastprint_time = 0;

    skb->len = buf_len;
    temp_rx_data_desc = (struct htc_data_desc *)skb->data;

#if (WQ_CONFIG_64 == 1)
    IOT_ASSERT((skb->len > (HTC_DATA_DESC_LEN - 2*diff_pointer_sizes))||((skb->len == (HTC_DATA_DESC_LEN - 2*diff_pointer_sizes))&&(temp_rx_data_desc->info.rx.chain_seq != 0)));
#else
    IOT_ASSERT((skb->len > HTC_DATA_DESC_LEN)||((skb->len == HTC_DATA_DESC_LEN)&&(temp_rx_data_desc->info.rx.chain_seq != 0)));
#endif




#if WQ_USB_RX_TYPE == WQ_USB_RX_CPU
if ((rx_skb_count >= rx_skb_count_limit) && (temp_rx_data_desc->info.rx.chain_seq == 0))
    {
        count++;
        if ((ticks - lastprint_time > hz ) || (lastprint_time == 0))
        {
           //iot_printf("wq_mac_rx_pkt drop!!!!! rx_skb_count:%d,The number of rx_drop:%d\n", rx_skb_count, count);
           lastprint_time = ticks;
           count = 0;
        }
        wq_free_skb(skb);
        return;
    }
#else
    if ((wq_dbg_flag & WQ_DEBUG_RX_LIMIT) && (rx_skb_count >= 20) && (temp_rx_data_desc->info.rx.chain_seq == 0))
    {
        count++;
        if ((ticks - lastprint_time > hz ) || (lastprint_time == 0))
        {
           iot_printf("wq_mac_rx_pkt drop!!!!! rx_skb_count:%d,The number of rx_drop:%d\n", rx_skb_count, count);
           lastprint_time = ticks;
           count = 0;
        }
        wq_free_skb(skb);
        return;
    }
#endif

	if (sc == NULL) {
        iot_printf("%s, sc is null. Free buffer and return\n", __func__);
		wq_free_skb(skb);
        return;
    }
	

	*((struct sk_buff **)skb->cb) = NULL;

	//put to rx_skb queue
	//spin_lock_irqsave(&rx_skb_lock, flags);

    rx_count++;

	spin_lock(&sc->rx_skb_lock);
    rx_skb_count++;
	if (rx_skb_head == NULL)
	{
		rx_skb_head = skb;
	    rx_skb_tail = skb;
	}
	else
	{
		next_skb = (struct sk_buff **)rx_skb_tail->cb;
		*next_skb = skb;
        rx_skb_tail = skb;
	}
	spin_unlock(&sc->rx_skb_lock);

	//spin_unlock_irqrestore(&rx_skb_lock, flags);

	//schedule rx work
    wk = &sc->sc_rxq_process_task;
    wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)(1));

}

void wq_mac_tx_done(void *wq_mac, void *buf, int status)
{
    struct mbuf *m = (struct mbuf *)buf;
	struct ieee80211com *ic = net80211_ic;
	struct wq_softc *sc = ic->ic_softc;
	struct wuqi_work_struct *wk;
    //unsigned long flags;

    if (sc == NULL) {
        iot_printf("%s, sc is null. Free buffer and return\n", __func__);
        ieee80211_free_node((struct ieee80211_node *)m->m_pkthdr.rcvif);
        m_freem(m);
        return;
    }

    //iot_printf("wq_mac_tx_done %p\n", m);
    m->txq_next = NULL;

    //put to txdone_mbuf queue
	spin_lock(&sc->txdone_mbuf_lock);
	if (txdone_mbuf_head == NULL)
	{
		txdone_mbuf_head = m;
	    txdone_mbuf_tail = m;
	}
	else
	{
		txdone_mbuf_tail->txq_next = m;
        txdone_mbuf_tail = m;
	}
	spin_unlock(&sc->txdone_mbuf_lock);

	//schedule rx work
    wk = &sc->sc_rxq_process_task;
    wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)(1));

    return;
}


void wq_mac_tx_done_work(void)
{
	struct ieee80211com *ic = net80211_ic;
	struct wq_softc *sc = ic->ic_softc;
    struct mbuf *m;
    unsigned long flags;

    while (1)
	{
		//get from txdone_mbuf queue
		spin_lock_irqsave(&sc->txdone_mbuf_lock, flags);

		m = txdone_mbuf_head;
		if (m != NULL)
		{
            txdone_mbuf_head = m->txq_next;
            if (txdone_mbuf_head == NULL)
				txdone_mbuf_tail = NULL;

			m->txq_next = NULL;
		}

		spin_unlock_irqrestore(&sc->txdone_mbuf_lock, flags);

		//iot_printf("wq_mac_tx_done_work %p\n", m);

        if (m != NULL)
		{

		    ieee80211_free_node((struct ieee80211_node *)m->m_pkthdr.rcvif);
		    m_freem(m);

		    /* schedule next tx */
		    //wq_tx_sche((struct wq_softc *)wq_mac);
		}
		else
		{
			break;
		}
    }

    return;
}

void wq_tx_postpond(struct mbuf *m)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct wuqi_work_struct *wk;

    m->m_next = NULL;

    mtx_lock(&sc->sc_tx_postpond_lock);
    if (txpostpond_mbuf_head == NULL)
    {
        txpostpond_mbuf_head = m;
        txpostpond_mbuf_tail = m;
    } else {
        txpostpond_mbuf_tail->m_next = m;
        txpostpond_mbuf_tail = m;
    }
    mtx_unlock(&sc->sc_tx_postpond_lock);

    wk = &sc->sc_rxq_process_task;
    wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)(1));

    return;
}

void wq_tx_postpond_work(uint8_t action)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct mbuf *m;

    while (1) {
        mtx_lock(&sc->sc_tx_postpond_lock);
        m = txpostpond_mbuf_head;
        if (m != NULL) {
            txpostpond_mbuf_head = m->m_next;
            if (txpostpond_mbuf_head == NULL) {
                txpostpond_mbuf_tail = NULL;
            }
            m->m_next = NULL;
            mtx_unlock(&sc->sc_tx_postpond_lock);

            /* send this frame */
            if (action == WQ_MAC_POSTPONE_TXQ_SEND)
                wq_transmit(net80211_ic, m);
            else
                m_freem(m);
        } else {
            mtx_unlock(&sc->sc_tx_postpond_lock);
            break;
        }
    }

    return;
}

void wq_mac_cmd_in_atomic(struct wq_softc *sc, struct htc_msg_desc *desc)
{
    struct wuqi_work_struct *wk;
    struct ieee80211com *ic = &sc->sc_ic;

    desc->next = NULL;

    spin_lock(&sc->mac_cmd_lock);
    /* put to cmd_desc queue */
    if (cmd_desc_head == NULL) {
        cmd_desc_head = desc;
        cmd_desc_tail = desc;
    } else {
        cmd_desc_tail->next = desc;
        cmd_desc_tail = desc;
    }
    spin_unlock(&sc->mac_cmd_lock);

    /* schedule rx work */
    wk = &sc->sc_rxq_process_task;
    wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)(1));

    return;
}

void wq_mac_cmd_work(uint8_t action)
{
    struct ieee80211com *ic = net80211_ic;
    struct wq_softc *sc = ic->ic_softc;
    struct htc_msg_desc *desc;
    unsigned long flags;
    int ret;

    while (1) {
        spin_lock_irqsave(&sc->mac_cmd_lock, flags);

        /* get cmd from cmd desc queue */
        desc = cmd_desc_head;
        if (desc != NULL) {
            cmd_desc_head = desc->next;
            if (cmd_desc_head == NULL) {
                cmd_desc_tail = NULL;
            }
            desc->next = NULL;
        }

        spin_unlock_irqrestore(&sc->mac_cmd_lock, flags);

        if (desc != NULL) {
            if (action == WQ_MAC_CMDQ_SEND) {
                /* do command */
                switch (desc->msg_idx) {
                    case MSG_TYPE_TX_AMPDU_NOTIFY:
                        ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV,
                                WQ_IPC_CMD, (uint8_t *)desc,
                                desc->len + HTC_MSG_DESC_LEN, NULL, 0);
                        break;
                    case MSG_TYPE_TIM_NOTIFY:
                        ret = wq_ipc_cmd_out(sc->sc_wq_ipc, WQ_IPC_RCV_DEV,
                                WQ_IPC_CMD, (uint8_t *)desc,
                                desc->len + HTC_MSG_DESC_LEN, NULL, 0);
                        break;
                    default:
                        iot_printf("unknow msg_idx %d\n", desc->msg_idx);
                        IOT_ASSERT(0);
                        break;
                }
            }

            mmal_free(desc);
        } else {
            break;
        }
    }

    return;
}

void wq_rxq_proc_comp()
{
#if 0
    uint32_t mac_int_en;

    /* enable rx_done interrupt */
    wq_disable_irq();
    mac_int_en = mac_int_p0g1_en_sw_mac_int_p0g1_en_get();
    mac_int_p0g1_en_sw_mac_int_p0g1_en_set(mac_int_en | MAC_INT_RXRN_STATUS_MASK);
    wq_enable_irq();
#endif
}

void wq_set_aggr_cnt(int en, int cnt)
{
    struct ieee80211com *ic = hawk_vap0->iv_ic;
    struct wq_softc *sc = ic->ic_softc;
    sc->sc_aggr_force_en = en;
    sc->sc_aggr_force_cnt = cnt;
}

void flush_all_queue(void)
{
    flush_rx_skb_queue();
    flush_event_rx_skb_queue();
    wq_mac_tx_done_work();
    wq_mac_tx_work(WQ_MAC_TXQ_FREE);
    wq_mac_cmd_work(WQ_MAC_CMDQ_FREE);

    /* free postpond tx frames */
    if (txpostpond_mbuf_head) {
        wq_tx_postpond_work(WQ_MAC_POSTPONE_TXQ_FREE);
    }
}

void wq_proc_tx_monitor(long tx_count, int phy_perf_flag, int pkt_len, int tx_ms, int tx_us)
{
    struct ieee80211vap *vap = NULL;
    struct ieee80211com *ic = NULL;
    struct wq_softc *sc = NULL;
    struct wuqi_work_struct *wk = NULL;

    if (hawk_vap1)
    {
        vap = hawk_vap1;
        ic = vap->iv_ic;
        ic->tx_count = tx_count;
        ic->phy_perf_flag = phy_perf_flag;
        if (ic->phy_perf_flag == 3) {
            ic->mp_pkt_len = pkt_len;
            ic->mp_tx_ms = tx_ms;
            ic->mp_tx_us = tx_us;
        }
        sc = ic->ic_softc;
        //printk("wq_proc_tx_monitor:ic->phy_perf_flag =%d\n",ic->phy_perf_flag,ic->mp_pkt_len,ic->mp_tx_us);

        wk = &sc->sc_monitor_tx_task;
        wk_queue_insert_ex(ic->ic_wdrvq, wk, (void *)(1));
    }
    else {
        iot_printf("%s: hawk_vap1 not exist\n", __func__);
    }

    return;
}

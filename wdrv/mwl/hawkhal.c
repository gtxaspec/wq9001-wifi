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
#ifdef DTEST_MODE
#include "oss.h"
#else
#include "oss.h"
#include <net80211/ieee80211_var.h>

#include <hawkhal.h>

#endif
#include "wq_hw_config.h"
#include "if_hawkvar.h"
#include "wq_hal_util.h"
#include "wq_wifi_dbg.h"

#define MWLHAL_DEBUG            /* debug msgs */

typedef enum {
    WL_ANTENNAMODE_RX = 0xffff,
    WL_ANTENNAMODE_TX = 2,
} wlantennamode_e;

typedef enum {
    WL_TX_POWERLEVEL_LOW = 5,
    WL_TX_POWERLEVEL_MEDIUM = 10,
    WL_TX_POWERLEVEL_HIGH = 15,
} wltxpowerlevel_e;

#define WQ_CMDBUF_SIZE 0x4000      /* size of f/w command buffer */
#define WQ_BASTREAMS_MAX   7   /* max BA streams (NB: fw >3.3.5.9) */
#define WQ_BAQID_MAX       8   /* max BA Q id's (NB: fw >3.3.5.9) */
#define WQ_MBSS_AP_MAX     1   /* max ap vap's */
#define WQ_MBSS_STA_MAX    1   /* max station/client vap's */
#define WQ_MBSS_MAX    (WQ_MBSS_AP_MAX+WQ_MBSS_STA_MAX)
#if 0
/*
 * BA stream -> queue ID mapping
 *
 * The first 2 streams map to h/w; the remaining streams are
 * implemented in firmware.
 */
static const int ba2qid[WQ_BASTREAMS_MAX] = {
    5, 6                /* h/w supported */
#if WQ_BASTREAMS_MAX == 7
    , 7, 0, 1, 2, 3         /* f/w supported */
#endif
};
static int qid2ba[WQ_BAQID_MAX];
#endif

#define _CMD_SETUP(pCmd, type, cmd) do {                \
    pCmd = (type *)&mh->mh_cmdbuf[0];               \
    memset(pCmd, 0, sizeof(type));                  \
    pCmd->CmdHdr.Cmd = htole16(cmd);                \
    pCmd->CmdHdr.Length = htole16(sizeof(type));            \
} while (0)

#define _VCMD_SETUP(vap, pCmd, type, cmd) do {              \
    _CMD_SETUP(pCmd, type, cmd);                    \
    pCmd->CmdHdr.MacId = vap->macid;                \
} while (0)

#define PWTAGETRATETABLE20M 14*4
#define PWTAGETRATETABLE40M 9*4
#define PWTAGETRATETABLE20M_5G  35*4
#define PWTAGETRATETABLE40M_5G  16*4

struct wq_hal_bastream {
    //WQ_HAL_BASTREAM public;    /* public state */
    uint8_t stream;         /* stream # */
    uint8_t setup;          /* f/w cmd sent */
    uint8_t ba_policy;      /* direct/delayed BA policy */
    uint8_t tid;
    uint8_t paraminfo;
    uint8_t macaddr[IEEE80211_ADDR_LEN];
};

struct wq_hal_priv;

struct wq_hal_vap {
    struct wq_hal_priv *mh;    /* back pointer */
    uint8_t vap_type;       /* WQ_HAL_BSSTYPE */
    uint8_t macid;          /* for passing to f/w */
    uint8_t flags;
    uint8_t power_state;
    uint32_t last_tick;
    uint32_t accumulated_awake;
    uint32_t accumulated_sleep;
#define MVF_RUNNING 0x01        /* BSS_START issued */
//#define MVF_STATION 0x02        /* sta db entry created */
    uint8_t mac[IEEE80211_ADDR_LEN];/* mac address */
};
#define MWLVAP(_vap)    ((_vap)->mh)

/*
 * Per-device state.  We allocate a single cmd buffer for
 * submitting operations to the firmware.  Access to this
 * buffer (and the f/w) are single-threaded.  At present
 * we spin waiting for cmds to complete which is bad.  Not
 * sure if it's possible to submit multiple requests or
 * control when we get cmd done interrupts.  There's no
 * documentation and no example code to indicate what can
 * or cannot be done so all we can do right now is follow the
 * linux driver logic.  This falls apart when the f/w fails;
 * the system comes to a crawl as we spin waiting for operations
 * to finish.
 */
struct wq_hal_priv {
    struct wq_hal  public;     /* public area */
    device_t    mh_dev;
    //char        mh_mtxname[12];
    struct mtx  mh_mtx;

    //int     mh_flags;
//#define MHF_CALDATA 0x0001      /* cal data retrieved */
//#define MHF_FWHANG  0x0002      /* fw appears hung */
//#define MHF_MBSS    0x0004      /* mbss enabled */
    struct wq_hal_vap mh_vaps[WQ_MBSS_MAX];
    //int     mh_bastreams;   /* bit mask of available BA streams */
    //int     mh_regioncode;  /* XXX last region code sent to fw */
    struct wq_hal_bastream mh_streams[WQ_BASTREAMS_MAX];
    //int     mh_debug;
    WQ_HAL_CHANNELINFO mh_20M;
    WQ_HAL_CHANNELINFO mh_40M;
    //WQ_HAL_CHANNELINFO mh_20M_5G;
    //WQ_HAL_CHANNELINFO mh_40M_5G;
    //uint32_t    mh_RTSSuccesses;/* cumulative stats for read-on-clear */
    //uint32_t    mh_RTSFailures;
    //uint32_t    mh_RxDuplicateFrames;
    //uint32_t    mh_FCSErrorCount;
    //WQ_DIAG_REVS   mh_revs;
};
#define MWLPRIV(_mh)    ((struct wq_hal_priv *)(_mh))

static int wq_hal_setmac_locked(struct wq_hal_vap *,
    const uint8_t addr[IEEE80211_ADDR_LEN]);
//static int wqExecuteCmd(struct wq_hal_priv *, unsigned short cmd);
//static int wqGetPwrCalTable(struct wq_hal_priv *);
#ifdef MWLHAL_DEBUG
//static const char *wqcmdname(int cmd);
//static void dumpresult(struct wq_hal_priv *, int showresult);
#endif /* MWLHAL_DEBUG */

struct hal_txq;
extern void hal_deinit_dummy_desc_pool(void);
extern void hal_deinit_tx_desc_pool(void);
extern void dump_hw_reg_base_addr();
extern void hwsim_txq_dequeue_by_qid(int qidx);
#if defined(WIFI_SIM) || defined(WIFI_UART_SIM)
extern void wq_beacon_intr_handler(struct wq_softc *sc);
#endif

extern struct ieee80211vap *hawk_vap0,*hawk_vap1;

void *g_beacon_tx_desc=NULL, *g_beacon_tx_dummy_desc=NULL;



static __inline void
WQ_HAL_LOCK(struct wq_hal_priv *mh)
{
    mtx_lock(&mh->mh_mtx);
}

static __inline void
WQ_HAL_LOCK_ASSERT(struct wq_hal_priv *mh)
{
    mtx_assert(&mh->mh_mtx, MA_OWNED);
}

static __inline void
WQ_HAL_UNLOCK(struct wq_hal_priv *mh)
{
    mtx_unlock(&mh->mh_mtx);
}

extern struct ieee80211com *net80211_ic;
void wq_hal_lock(void)
{
    struct wq_softc *sc = net80211_ic->ic_softc;
    struct wq_hal *mh0 = sc->sc_mh;
    struct wq_hal_priv *mh = MWLPRIV(mh0);

    mtx_lock(&mh->mh_mtx);
}

void wq_hal_unlock(void)
{
    struct wq_softc *sc = net80211_ic->ic_softc;
    struct wq_hal *mh0 = sc->sc_mh;
    struct wq_hal_priv *mh = MWLPRIV(mh0);

    mtx_unlock(&mh->mh_mtx);
}



extern struct hal_txq gv_txq[HAL_TXQ_NUM];
#if defined(WIFI_SIM) || defined(WIFI_UART_SIM)
extern void *wifi_sim_ctx;
extern void wq_intr_handler(struct wq_softc *sc);
#endif //#ifdef WIFI_SIM

void hal_beacon_xmit(void *data, int len);

static int wqResetHalState(struct wq_hal_priv *mh);
extern int wifilib_is_freq_supported(uint16_t freq, char* ccode, uint8_t bw, uint8_t * tx_power);
extern void wifilib_get_ccode(char* ccode);

/*
 * Setup for communication with the device.  We allocate
 * a command buffer and map it for bus dma use.  The pci
 * device id is used to identify whether the device has
 * SRAM on it (in which case f/w download must include a
 * memory controller reset).  All bus i/o operations happen
 * in BAR 1; the driver passes in the tag and handle we need.
 */
struct wq_hal *
wq_hal_attach(device_t dev, uint16_t devid)
{
    struct wq_hal_priv *mh;
    struct wq_hal_vap *hvap;
    int i;
    uint16_t gband_channel_list[14] = { 2412, 2417, 2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462, 2467, 2472, 2484 };
    char ccode[2];
    uint16_t freq;
    int nchan20 = 0;
    int nchan40 = 0;
    uint8_t tx_power;
    WQ_HAL_CHANNELINFO chinfo_2g_20m;
    WQ_HAL_CHANNELINFO chinfo_2g_40m;

    dump_hw_reg_base_addr();
    mh = malloc(sizeof(struct wq_hal_priv), M_DEVBUF, M_NOWAIT | M_ZERO);
    if (mh == NULL)
        return NULL;
    mh->mh_dev = dev;
#if 0
    for (i = 0; i < WQ_BASTREAMS_MAX; i++) {
        mh->mh_streams[i].public.txq = ba2qid[i];
        mh->mh_streams[i].stream = i;
        /* construct back-mapping while we're at it */
        if (mh->mh_streams[i].public.txq < WQ_BAQID_MAX)
            qid2ba[mh->mh_streams[i].public.txq] = i;
        else
            device_printf(dev, "unexpected BA tx qid %d for "
                "stream %d\n", mh->mh_streams[i].public.txq, i);
    }
#endif    
#if 1

    wifilib_get_ccode(ccode);
    //TODO : if application does not specify ccode, then load ccode from calibration data
    if ((ccode[0] == 0) && (ccode[1] == 0))
    {
        //TODO : shall load ccode from calibration data
        ccode[0] = '0';
        ccode[1] = '0';
    }
    //iot_printf("ccode = %c%c\n", ccode[0], ccode[1]);

    os_mem_set(&chinfo_2g_20m, 0, sizeof(WQ_HAL_CHANNELINFO));
    os_mem_set(&chinfo_2g_40m, 0, sizeof(WQ_HAL_CHANNELINFO));

    for (i = 0; i < 14; i++)
    {
        freq = gband_channel_list[i];

        if (wifilib_is_freq_supported(freq, ccode, 20, &tx_power))
        {
            //chinfo_2g_20m.channels[nchan20].freq = freq;
            chinfo_2g_20m.channels[nchan20].ieee = ieee80211_mhz2ieee(freq, 0);
            chinfo_2g_20m.channels[nchan20].maxTxPow = tx_power;
            nchan20++;
        }
        if (wifilib_is_freq_supported(freq, ccode, 40, &tx_power))
        {
            //chinfo_2g_40m.channels[nchan40].freq = freq;
            chinfo_2g_40m.channels[nchan40].ieee = ieee80211_mhz2ieee(freq, 0);
            chinfo_2g_40m.channels[nchan40].maxTxPow = tx_power;
            nchan40++;
        }

    }
    if (nchan20 > 0)
    {
        chinfo_2g_20m.nchannels = nchan20;
    }
    if (nchan40 > 0)
    {
        chinfo_2g_40m.nchannels = nchan40;
    }
    iot_printf("20M : nchan=%d\n", nchan20);
    iot_printf("40M : nchan=%d\n", nchan40);

    os_mem_cpy(&mh->mh_20M, &chinfo_2g_20m, sizeof(WQ_HAL_CHANNELINFO));
    os_mem_cpy(&mh->mh_40M, &chinfo_2g_40m, sizeof(WQ_HAL_CHANNELINFO));
#endif

    #if 0
    /* TODO : shall use calibration data to generate channel info table */
    chinfo_2g_20m.freqLow = 2412;
    chinfo_2g_20m.freqHigh = 2484;
    chinfo_2g_20m.nchannels = 14;
    for (i=0; i<13; i++) {
        chinfo_2g_20m.channels[i].freq = 2412 + (i*5);
        chinfo_2g_20m.channels[i].ieee = 1+i;
        chinfo_2g_20m.channels[i].maxTxPow = 10;
        for (j=0; j<4; j++)
          chinfo_2g_20m.channels[i].targetPowers[j] = 10;
    }

    chinfo_2g_20m.channels[13].freq = 2484;
    chinfo_2g_20m.channels[13].ieee = 14;
    chinfo_2g_20m.channels[13].maxTxPow = 14;
    for (j=0; j<4; j++)
      chinfo_2g_20m.channels[13].targetPowers[j] = 10;

    os_mem_cpy(&mh->mh_20M, &chinfo_2g_20m, sizeof(WQ_HAL_CHANNELINFO));
    os_mem_cpy(&mh->mh_40M, &chinfo_2g_20m, sizeof(WQ_HAL_CHANNELINFO));
    
    #endif
    
    /* setup constant portion of vap state */
    //Dedicate VAP0 for softAP, VAP1 for STA
    hvap = &mh->mh_vaps[0];
    hvap->vap_type = WQ_HAL_AP;
    hvap->macid = 0;
    hvap->power_state = HAL_HW_SLEEP;
    hvap->accumulated_awake = 0;
    hvap->accumulated_sleep = 0;
    hvap->last_tick = ticks;
    
    hvap = &mh->mh_vaps[1];
    hvap->vap_type = WQ_HAL_STA;
    hvap->macid = 1;
    hvap->power_state = HAL_HW_SLEEP;
    hvap->accumulated_awake = 0;
    hvap->accumulated_sleep = 0;
    hvap->last_tick = ticks;
    
    //mh->mh_revs.mh_devid = devid;
    //snprintf(mh->mh_mtxname, sizeof(mh->mh_mtxname),
    //    "%s_hal", device_get_nameunit(dev));
    mtx_init(&mh->mh_mtx, NULL, NULL, MTX_DEF);

    //done in fw
    //wq_hal_mac_hw_init(0); 
    

    //Tx Init
    //hal_init_tx_desc_pool();
    //hal_init_dummy_desc_pool();

    wqResetHalState(mh); //reset BA stream

    //done in fw
    //hal_node_table_init();

    //HW ready, now enable interrupt
    //hal_intr_enable();

    //enable wifi dbg in debug build, disable in release build
    //wifi_dbg_init();
    //wifi_dbg_enable(0);

    return &mh->public;
}

void
wq_hal_detach(struct wq_hal *mh0)
{
    struct wq_hal_priv *mh = MWLPRIV(mh0);

    mtx_destroy(&mh->mh_mtx);
    free(mh, M_DEVBUF);

    //TX
    //hal_deinit_dummy_desc_pool();
    //hal_deinit_tx_desc_pool();

    //RX
    //hal_rx_data_ring_deinit();
    //hal_rx_mgmt_ring_deinit();

    //Deinit beacon's Descriptor
    //hal_beacon_txq_deinit();
}

uint8_t
ISR_ATTR wq_hal_get_macid(struct wq_hal_vap *hvap)
{
    return hvap->macid;
}

/*
 * Reset internal state after a firmware download.
 */
static int
wqResetHalState(struct wq_hal_priv *mh)
{
    int i;

    /* XXX get from f/w */
    //mh->mh_bastreams = (1<<WQ_BASTREAMS_MAX)-1;
    for (i = 0; i < WQ_MBSS_MAX; i++)
        mh->mh_vaps[i].mh = NULL;
    /*
     * Clear cumulative stats.
     */
    //mh->mh_RTSSuccesses = 0;
    //mh->mh_RTSFailures = 0;
    //mh->mh_RxDuplicateFrames = 0;
    //mh->mh_FCSErrorCount = 0;
    /*
     * Fetch cal data for later use.
     * XXX may want to fetch other stuff too.
     */
    /* XXX check return */
    //if ((mh->mh_flags & MHF_CALDATA) == 0)
    //  wqGetPwrCalTable(mh);
    
    //TODO : reset BA stream HW
    
    return 0;
}

struct wq_hal_vap *
wq_hal_newvap(struct wq_hal *mh0, WQ_HAL_BSSTYPE type,
    const uint8_t mac[IEEE80211_ADDR_LEN])
{
    struct wq_hal_priv *mh = MWLPRIV(mh0);
    struct wq_hal_vap *vap;
    int i;

    WQ_HAL_LOCK(mh);
    /* NB: could optimize but not worth it w/ max 32 bss */
    for (i = 0; i < WQ_MBSS_MAX; i++) {
        vap = &mh->mh_vaps[i];
        if (vap->vap_type == type && vap->mh == NULL) {
            vap->mh = mh;
            wq_hal_setmac_locked(vap, mac);
            break;
        }
    }
    WQ_HAL_UNLOCK(mh);
    return (i < WQ_MBSS_MAX) ? vap : NULL;
}

void
wq_hal_delvap(struct wq_hal_vap *vap)
{
    /* NB: locking not needed for single write */
    vap->mh = NULL;
}

/*
 * Manipulate the debug mask.  Note debug
 * msgs are only provided when this code is
 * compiled with MWLHAL_DEBUG defined.
 */





/*
 * Set the interrupt mask.
 */
void
wq_hal_intrset(struct wq_hal *mh0, uint32_t mask)
{

}

void
wq_hal_rx_start(struct wq_hal *mh)
{
    DPRINTF(WDRV_DEBUG_RECV, "start hal rx ring \n");
    mac_rxr0_ctrl_sw_rxr0_en_set(1);
    mac_rxr1_ctrl_sw_rxr1_en_set(1);
}


/*
 * Set HT guard interval handling.
 * Takes effect immediately.
 */
int
wq_hal_sethtgi(struct wq_hal_vap *vap, int GIType)
{
    struct wq_hal_priv *mh = MWLVAP(vap);

    WQ_HAL_LOCK(mh);
    //TODO : set guard interval, though LGI and SGI is controlled by Tx DESC 
    WQ_HAL_UNLOCK(mh);
    return 0;
}

/*
 * Set packet size threshold for implicit use of RTS.
 * Takes effect immediately.
 * XXX packet length > threshold =>'s RTS
 */
int
wq_hal_setrtsthreshold(struct wq_hal_vap *vap, int threshold)
{
    struct wq_hal_priv *mh = MWLVAP(vap);

    WQ_HAL_LOCK(mh);
    //TODO : set RTS threshold, RTS is controlled by Tx DESC
    WQ_HAL_UNLOCK(mh);
    return 0;
}

/*
 * Enable sta-mode operation (disables beacon frame xmit).
 */
int
wq_hal_setinframode(struct wq_hal_vap *vap)
{
    struct wq_hal_priv *mh = MWLVAP(vap);

    WQ_HAL_LOCK(mh);
    //TODO : to configure beacon related registers to disable beacon transmission
    
    DPRINTF(WDRV_DEBUG_STATE, "wq_hal_setinframode mac-%d\n", vap->macid);

    IOT_ASSERT(vap->macid < 2);
    
    if (vap->macid == 0)
        mac_role0_ctrl_sw_in_bss_self_role0_set(0); //0 for STA, 1 for softAP
    else{
        mac_role1_ctrl_sw_in_bss_self_role1_set(0); //0 for STA, 1 for softAP
    }
    
    WQ_HAL_UNLOCK(mh);
    return 0;
}

#define IEEE80211_TU_TO_MICROSECOND       1024 //one TU = 1024 us
int wq_hal_set_beacon_interval(struct wq_hal_vap *vap, uint32_t beacon_interval)
{
    struct wq_hal_priv *mh = MWLVAP(vap);

    WQ_HAL_LOCK(mh);
    
    DPRINTF(WDRV_DEBUG_BEACON, "wq_hal_set_beacon_interval mac-%d interval=%d\n", vap->macid, beacon_interval);

    IOT_ASSERT(vap->macid < 2);
    
    //beacon interval = N (TU)
    //HW reg = N * 1024 micro-seconds
    beacon_interval = beacon_interval * IEEE80211_TU_TO_MICROSECOND;
    
    
    //separate HW register to program beacon interval for each VAP
    if (vap->macid == 0)
    {
        mac_tsf_0_tbtt_period_set(beacon_interval); 
    }
    else
    {
        mac_tsf_1_tbtt_period_set(beacon_interval);
    }
    
    WQ_HAL_UNLOCK(mh);
    return 0;
}



int wq_hal_set_tbtt_timer(struct wq_hal_vap *vap, uint32_t beacon_interval, uint32_t pre_tbtt_offset, uint32_t post_tbtt_offset)
{
    struct wq_hal_priv *mh = MWLVAP(vap);
    uint32_t int_enable = 0;
    
    WQ_HAL_LOCK(mh);
    
    DPRINTF(WDRV_DEBUG_BEACON, "wq_hal_set_tbtt_timer mac-%d pre_tbtt=%d, post_tbtt=%d\n", vap->macid, pre_tbtt_offset, post_tbtt_offset);

    IOT_ASSERT(vap->macid < 2);
    
    //separate HW tbtt timers for each VAP
    if (vap->macid == 0)
    {
        mac_tsf_0_tbtt_delta_pack(pre_tbtt_offset, post_tbtt_offset);
        /* enable mac0 tbtt interrupt */
        int_enable = mac_int_p0g0_en_sw_mac_int_p0g0_en_get();
        int_enable |= (MAC_INT_TSF_0_TBTT_PRE | MAC_INT_TSF_0_TBTT_POST);
        mac_int_p0g0_clr_set(MAC_INT_TSF_0_TBTT_PRE | MAC_INT_TSF_0_TBTT_POST);
        mac_int_p0g0_en_sw_mac_int_p0g0_en_set(int_enable);
    }
    else
    {
        mac_tsf_1_tbtt_delta_pack(pre_tbtt_offset, post_tbtt_offset);
        /* enable mac1 tbtt interrupt */
        int_enable = mac_int_p0g0_en_sw_mac_int_p0g0_en_get();
        int_enable |= (MAC_INT_TSF_1_TBTT_PRE | MAC_INT_TSF_1_TBTT_POST);
        mac_int_p0g0_clr_set(MAC_INT_TSF_1_TBTT_PRE | MAC_INT_TSF_1_TBTT_POST);
        mac_int_p0g0_en_sw_mac_int_p0g0_en_set(int_enable);
    }
    
    WQ_HAL_UNLOCK(mh);
    return 0;
}

int wq_hal_clr_tbtt_timer(struct wq_hal_vap *vap)
{
    struct wq_hal_priv *mh = MWLVAP(vap);
    uint32_t int_enable = 0;

    WQ_HAL_LOCK(mh);

    DPRINTF(WDRV_DEBUG_BEACON, "wq_hal_clr_tbtt_timer mac-%d\n", vap->macid);

    IOT_ASSERT(vap->macid < 2);

    int_enable = mac_int_p0g0_en_sw_mac_int_p0g0_en_get();
    if (vap->macid == 0)
    {
        /* disable mac0 tbtt interrupt */
        int_enable = mac_int_p0g0_en_sw_mac_int_p0g0_en_get();
        int_enable &= ~(MAC_INT_TSF_0_TBTT_PRE | MAC_INT_TSF_0_TBTT_POST);
        mac_int_p0g0_en_sw_mac_int_p0g0_en_set(int_enable);
        mac_int_p0g0_clr_set(MAC_INT_TSF_0_TBTT_PRE | MAC_INT_TSF_0_TBTT_POST);
    }
    else
    {
        /* disable mac1 tbtt interrupt */
        int_enable = mac_int_p0g0_en_sw_mac_int_p0g0_en_get();
        int_enable &= ~(MAC_INT_TSF_1_TBTT_PRE | MAC_INT_TSF_1_TBTT_POST);
        mac_int_p0g0_en_sw_mac_int_p0g0_en_set(int_enable);
        mac_int_p0g0_clr_set(MAC_INT_TSF_1_TBTT_PRE | MAC_INT_TSF_1_TBTT_POST);
    }

    WQ_HAL_UNLOCK(mh);
    return 0;
}

int wq_hal_set_bcn_ts_delta(struct wq_hal_vap *vap, uint32_t delta)
{
    struct wq_hal_priv *mh = MWLVAP(vap);

    WQ_HAL_LOCK(mh);

    DPRINTF(WDRV_DEBUG_BEACON, "wq_hal_set_bcn_ts_delta mac-%d delta=%d\n", vap->macid, delta);

    if (vap->macid == 0) {
        mac_tsf_0_bcn_ts_delta_sw_tsf_0_bcn_tx_ts_delta_set(delta);
    } else {
        mac_tsf_1_bcn_ts_delta_sw_tsf_1_bcn_tx_ts_delta_set(delta);
    }

    WQ_HAL_UNLOCK(mh);
    return 0;
}

/*
 * Configure radar detection in support of 802.11h.
 */
int
wq_hal_setradardetection(struct wq_hal *mh0, WQ_HAL_RADAR action)
{
    struct wq_hal_priv *mh = MWLPRIV(mh0);

    DPRINTF(WDRV_DEBUG_STATE, "warning : HW does not support radar detection\n");
    
    WQ_HAL_LOCK(mh);
    //do nothing, HAWK does not support Radar detection
    WQ_HAL_UNLOCK(mh);
    return 0;
} 

/*
 * Start a channel switch announcement countdown.  The IE
 * in the beacon frame is allowed to go out and the firmware
 * counts down and notifies the host when it's time to switch
 * channels.
 */
int
wq_hal_setchannelswitchie(struct wq_hal *mh0,
    const struct ieee80211_channel *nextchan, uint32_t mode, uint32_t count)
{
    struct wq_hal_priv *mh = MWLPRIV(mh0);

    WQ_HAL_LOCK(mh);
    //TODO : CSA countdown shall be done in driver
    WQ_HAL_UNLOCK(mh);
    return 0;
}

#define RATEVAL(r)  ((r) &~ RATE_MCS)
#define RATETYPE(r) (((r) & RATE_MCS) ? HT_RATE_TYPE : LEGACY_RATE_TYPE)


#undef RATEVAL
#undef RATETYPE

int
wq_hal_setslottime(struct wq_hal *mh0, int usecs)
{
    struct wq_hal_priv *mh = MWLPRIV(mh0);

    //sanity check
    IOT_ASSERT(usecs == 9 || usecs == 20);

    WQ_HAL_LOCK(mh);

    DPRINTF(WDRV_DEBUG_STATE, "wq_hal_setslottime %d\n", usecs);

    mac_ifs_ctrl_sw_slot_set(usecs);

    WQ_HAL_UNLOCK(mh);
    return 0;
}

int
wq_hal_getchannelinfo(struct wq_hal *mh0, int band, int chw,
    const WQ_HAL_CHANNELINFO **ci)
{
    struct wq_hal_priv *mh = MWLPRIV(mh0);
    
    DPRINTF(WDRV_DEBUG_STATE, "wq_hal_getchannelinfo band=%d chw=%d\n", band, chw);

    
    switch (band) {
    case WQ_FREQ_BAND_2DOT4GHZ:
        *ci = (chw == WQ_CH_20_MHz_WIDTH) ? &mh->mh_20M : &mh->mh_40M;
        break;
    //case WQ_FREQ_BAND_5GHZ:
    //  *ci = (chw == WQ_CH_20_MHz_WIDTH) ?
    //       &mh->mh_20M_5G : &mh->mh_40M_5G;
    //  break;
    default:
        return EINVAL;
    }
    return ((*ci)->nchannels == 0) ? EINVAL : 0;
}


static int wq_hal_setmac_locked(struct wq_hal_vap *vap, const uint8_t addr[IEEE80211_ADDR_LEN])
{
    uint32_t low_addr, high_addr;

    DPRINTF(WDRV_DEBUG_STATE, "wq_hal_setmac_locked mac-%d %02X-%02X-%02X-%02X-%02X-%02X\n", vap->macid, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    low_addr = ((uint32_t)addr[0]) + (((uint32_t)addr[1]) << 8) + (((uint32_t)addr[2]) << 16) + (((uint32_t)addr[3]) << 24);
    high_addr =  ((uint32_t)addr[4]) + (((uint32_t)addr[5]) << 8);

    //set MAC address to register
    //macid 0 for softAP
    //macid 1 for STA
    //Do not support P2P for now

    //NOTE:mac_rx_filter_rule_0_sw_addr_filter_en_set
    //     0: role0/1_RA and all multicast receive.
    //     1: role0_RA and all multicast receive.
    //     2: role1_RA and all multicast receive.
    //     others: all frame receive, don't care RA
    IOT_ASSERT(vap->macid < 2);
    if (vap->macid == 0) //macid 0 for softAP
    {
        mac_role0_self_addr_low_pack(low_addr);
        mac_role0_self_addr_high_pack(high_addr);
        if (hawk_vap1)  //STA existing?
            mac_rx_filter_rule_0_sw_addr_filter_en_set(0);
        else
            mac_rx_filter_rule_0_sw_addr_filter_en_set(1);
    }
    else if (vap->macid == 1) //macid 1 for STA
    {
        mac_role1_self_addr_low_pack(low_addr);
        mac_role1_self_addr_high_pack(high_addr);

        if (hawk_vap0)
            mac_rx_filter_rule_0_sw_addr_filter_en_set(0);
        else
            mac_rx_filter_rule_0_sw_addr_filter_en_set(2);
    }

    return 0;
}

int
wq_hal_setmac(struct wq_hal_vap *vap, const uint8_t addr[IEEE80211_ADDR_LEN])
{
    struct wq_hal_priv *mh = MWLVAP(vap);
    int retval;

    WQ_HAL_LOCK(mh);
    retval = wq_hal_setmac_locked(vap, addr);
    WQ_HAL_UNLOCK(mh);
    return retval;
}


#ifdef HW_PLATFORM_FPGA
extern void st_force_cntr(uint8_t en);
extern void phy_param_config(uint8_t bw, uint8_t pri_ch_upp_en, uint32_t ch_id, uint16_t pwr);
#else
#define st_force_cntr(x)
extern void phy_param_config(uint8_t bw, uint8_t pri_ch_upp_en, uint32_t ch_id, uint16_t pwr);
#endif

//Set HW chip to power state
int
wq_hal_set_power_state(struct wq_hal_vap *hvap, HAL_HW_POWER_STATE power_state)
{
    struct wq_hal_priv *mh = MWLVAP(hvap);
    uint32_t cur_tick, diff_time;

    WQ_HAL_LOCK(mh);
    
    //for debug purpose, tracking total awake and sleep time
    DPRINTF(WDRV_DEBUG_STATE, "mac-%d old power state=%d, new power state=%d\n",
        hvap->macid, hvap->power_state, power_state);
    if (power_state != hvap->power_state)
    {
        cur_tick = wq_ticks;
        if (cur_tick > hvap->last_tick) {
            diff_time = cur_tick - hvap->last_tick;
        } else {
            diff_time = 0xffffffff - hvap->last_tick + cur_tick + 1;
        }
        if (hvap->power_state == HAL_HW_AWAKE)
            hvap->accumulated_awake += diff_time;
        else
            hvap->accumulated_sleep += diff_time;
        hvap->last_tick = cur_tick;

        DPRINTF(WDRV_DEBUG_STATE, "mac-%d accumulated_awake=%d, accumulated_sleep=%d\n",
            hvap->macid, hvap->accumulated_awake, hvap->accumulated_sleep);
                    
        hvap->power_state = power_state;
    
        if (power_state == HAL_HW_AWAKE)
        {
            DPRINTF(WDRV_DEBUG_STATE, "wq_hal_set_power_state => AWAKE\n");
            //TODO : turn on RF, PHY and MAC
            st_force_cntr(0); //Turned on phy
        }
        else if (power_state == HAL_HW_SLEEP)
        {
            DPRINTF(WDRV_DEBUG_STATE, "wq_hal_set_power_state => SLEEP\n");
            //TODO : turn off RF, PHY and MAC 
            st_force_cntr(1); //Turned off phy
        }
        else
            IOT_ASSERT(0);//invalid power state
    }

    WQ_HAL_UNLOCK(mh);
    return 0;
}

int
wq_hal_setassocid(struct wq_hal_vap *vap,
    const uint8_t bssId[IEEE80211_ADDR_LEN], uint16_t assocId)
{
    struct wq_hal_priv *mh = MWLVAP(vap);
    uint32_t low_bssid, high_bssid;


    WQ_HAL_LOCK(mh);
    //set AID register        
    DPRINTF(WDRV_DEBUG_STATE, "wq_hal_setassocid mac-%d aid=0x%X bssid=%02X-%02X-%02X-%02X-%02X-%02X\n", vap->macid, assocId, bssId[0], bssId[1], bssId[2], bssId[3], bssId[4], bssId[5]);
    low_bssid = ((uint32_t)bssId[0]) + (((uint32_t)bssId[1]) << 8) + (((uint32_t)bssId[2]) << 16) + (((uint32_t)bssId[3]) << 24);
    high_bssid =  ((uint32_t)bssId[4]) + (((uint32_t)bssId[5]) << 8);

    IOT_ASSERT(vap->macid < 2);
    
    if (vap->macid == 0)
    {
        IOT_ASSERT(0); //softAP shall use macid 0
        mac_role0_ctrl_sw_self_alloc_aid0_set(assocId);
        mac_role0_ap_addr_low_pack(low_bssid);
        mac_role0_ap_addr_high_pack(high_bssid);
    }
    else
    {
        mac_role1_ctrl_sw_self_alloc_aid1_set(assocId);
        mac_role1_ap_addr_low_pack(low_bssid);
        mac_role1_ap_addr_high_pack(high_bssid);
    }
    //pCmd->AssocID = htole16(assocId);
    //IEEE80211_ADDR_COPY(&pCmd->MacAddr[0], bssId);

    WQ_HAL_UNLOCK(mh);
    return 0;
}




    
int wq_hal_set_rx_ampdu(struct wq_hal_vap *vap, uint8_t id, uint8_t tid, uint8_t *addr, uint32_t seqno, uint32_t window)
{
    //struct wq_hal_priv *mh = MWLVAP(vap);
    uint32_t addr_l, addr_h;
    uint8_t vap_id = vap->macid;
    
    addr_l = (uint32_t)addr[0] + (((uint32_t)addr[1]) << 8) + (((uint32_t)addr[2]) << 16) + (((uint32_t)addr[3]) << 24);
    addr_h = (uint32_t)addr[4] + (((uint32_t)addr[5]) << 8);

    DPRINTF(WDRV_DEBUG_AMPDU, "wq_hal_set_rx_ampdu id=%d,, vap_id=%d, tid=%d, addr=%X-%X-%X-%X-%X-%X, seq=%d, windows=%d\n", vap_id, id, tid,
        addr[0], addr[1], addr[2], addr[3], addr[4], addr[5],
        seqno, window);

    //WQ_HAL_LOCK(mh);

    //to program Rx Aggregation related registers to setup rx aggregation
    switch(id)
    {
        case 0:
            mac_ba_0_bitmap_ctrl_2_sw_ba_0_bitmap_force_zero_set(1);
            mac_ba_0_bitmap_ctrl_2_sw_ba_0_bitmap_force_zero_set(0);
            mac_ba_0_bitmap_ctrl_1_sw_ba_0_addr1_chos_set(vap_id);
            mac_ba_0_bitmap_ctrl_1_sw_ba_0_tid_set(tid);
            mac_ba_0_bitmap_ctrl_0_sw_ba_0_addr2_low_set(addr_l);
            mac_ba_0_bitmap_ctrl_1_sw_ba_0_addr2_high_set(addr_h);
            mac_ba_0_bitmap_ctrl_2_sw_ba_0_winstart_set(seqno);
            mac_ba_0_bitmap_ctrl_2_sw_ba_0_winoffset_set(window - 1); //windows size - 1
            mac_ba_0_bitmap_ctrl_2_sw_ba_0_win_set_set(1);
            break;
        case 1:
            mac_ba_1_bitmap_ctrl_2_sw_ba_1_bitmap_force_zero_set(1);
            mac_ba_1_bitmap_ctrl_2_sw_ba_1_bitmap_force_zero_set(0);
            mac_ba_1_bitmap_ctrl_1_sw_ba_1_addr1_chos_set(vap_id);
            mac_ba_1_bitmap_ctrl_1_sw_ba_1_tid_set(tid);
            mac_ba_1_bitmap_ctrl_0_sw_ba_1_addr2_low_set(addr_l);
            mac_ba_1_bitmap_ctrl_1_sw_ba_1_addr2_high_set(addr_h);
            mac_ba_1_bitmap_ctrl_2_sw_ba_1_winstart_set(seqno);
            mac_ba_1_bitmap_ctrl_2_sw_ba_1_winoffset_set(window - 1); //windows size - 1
            mac_ba_1_bitmap_ctrl_2_sw_ba_1_win_set_set(1);
            break;
        case 2:
            mac_ba_2_bitmap_ctrl_2_sw_ba_2_bitmap_force_zero_set(1);
            mac_ba_2_bitmap_ctrl_2_sw_ba_2_bitmap_force_zero_set(0);
            mac_ba_2_bitmap_ctrl_1_sw_ba_2_addr1_chos_set(vap_id);
            mac_ba_2_bitmap_ctrl_1_sw_ba_2_tid_set(tid);
            mac_ba_2_bitmap_ctrl_0_sw_ba_2_addr2_low_set(addr_l);
            mac_ba_2_bitmap_ctrl_1_sw_ba_2_addr2_high_set(addr_h);
            mac_ba_2_bitmap_ctrl_2_sw_ba_2_winstart_set(seqno);
            mac_ba_2_bitmap_ctrl_2_sw_ba_2_winoffset_set(window - 1); //windows size - 1
            mac_ba_2_bitmap_ctrl_2_sw_ba_2_win_set_set(1);
            break;
        case 3:
            mac_ba_3_bitmap_ctrl_2_sw_ba_3_bitmap_force_zero_set(1);
            mac_ba_3_bitmap_ctrl_2_sw_ba_3_bitmap_force_zero_set(0);
            mac_ba_3_bitmap_ctrl_1_sw_ba_3_addr1_chos_set(vap_id);
            mac_ba_3_bitmap_ctrl_1_sw_ba_3_tid_set(tid);
            mac_ba_3_bitmap_ctrl_0_sw_ba_3_addr2_low_set(addr_l);
            mac_ba_3_bitmap_ctrl_1_sw_ba_3_addr2_high_set(addr_h);
            mac_ba_3_bitmap_ctrl_2_sw_ba_3_winstart_set(seqno);
            mac_ba_3_bitmap_ctrl_2_sw_ba_3_winoffset_set(window - 1); //windows size - 1
            mac_ba_3_bitmap_ctrl_2_sw_ba_3_win_set_set(1);
            break;
        case 4:
            mac_ba_4_bitmap_ctrl_2_sw_ba_4_bitmap_force_zero_set(1);
            mac_ba_4_bitmap_ctrl_2_sw_ba_4_bitmap_force_zero_set(0);
            mac_ba_4_bitmap_ctrl_1_sw_ba_4_addr1_chos_set(vap_id);
            mac_ba_4_bitmap_ctrl_1_sw_ba_4_tid_set(tid);
            mac_ba_4_bitmap_ctrl_0_sw_ba_4_addr2_low_set(addr_l);
            mac_ba_4_bitmap_ctrl_1_sw_ba_4_addr2_high_set(addr_h);
            mac_ba_4_bitmap_ctrl_2_sw_ba_4_winstart_set(seqno);
            mac_ba_4_bitmap_ctrl_2_sw_ba_4_winoffset_set(window - 1); //windows size - 1
            mac_ba_4_bitmap_ctrl_2_sw_ba_4_win_set_set(1);
            break;
        default:
		    IOT_ASSERT(0);
			break;
    }
    
    //pCmd->ActionType = htole32(BaCreateStream);
    //pCmd->BaInfo.CreateParams.BarThrs = htole32(BarThrs);
    //pCmd->BaInfo.CreateParams.WindowSize = htole32(WindowSize);
    //pCmd->BaInfo.CreateParams.IdleThrs = htole32(0x22000);
    //IEEE80211_ADDR_COPY(&pCmd->BaInfo.CreateParams.PeerMacAddr[0],
    //    sp->macaddr);
    /* XXX proxy STA */
    //pCmd->BaInfo.CreateParams.Tid = sp->tid;
    //pCmd->BaInfo.CreateParams.QueueId = sp->stream;
    //pCmd->BaInfo.CreateParams.ParamInfo = sp->paraminfo;
    /* NB: ResetSeqNo known to be zero */
    //pCmd->BaInfo.CreateParams.StartSeqNo = htole16(seqno);
    //pCmd->BaInfo.CreateParams.Flags =
    //        htole32(BASTREAM_FLAG_IMMEDIATE_TYPE)
    //      | htole32(BASTREAM_FLAG_DIRECTION_UPSTREAM)
    //      ;


    //WQ_HAL_UNLOCK(mh);
    return 0;
}


/*
 * Set CFEND status Enable/Disable
 */
int
wq_hal_setcfend(struct wq_hal *mh0, int ena)
{
    struct wq_hal_priv *mh = MWLPRIV(mh0);
    WQ_HAL_LOCK(mh);

    //set CFEND configuration register
    mac_txq0_ctrl_sw_txq0_cf_end_en_set(ena);
    mac_txq1_ctrl_sw_txq1_cf_end_en_set(ena);
    mac_txq2_ctrl_sw_txq2_cf_end_en_set(ena);
    mac_txq3_ctrl_sw_txq3_cf_end_en_set(ena);

    WQ_HAL_UNLOCK(mh); 
    return 0;
}

int
wq_hal_setapmode(struct wq_hal_vap *vap, WQ_HAL_APMODE ApMode)
{
    struct wq_hal_priv *mh = MWLVAP(vap);

    WQ_HAL_LOCK(mh);
    //set AP mode
    DPRINTF(WDRV_DEBUG_STATE, "wq_hal_setapmode mac-%d\n", vap->macid);

    IOT_ASSERT(vap->macid < 2);
    
    if (vap->macid == 0)
        mac_role0_ctrl_sw_in_bss_self_role0_set(1); //0 for STA, 1 for softAP
    else{
        mac_role1_ctrl_sw_in_bss_self_role1_set(1); //0 for STA, 1 for softAP
    }
        
    WQ_HAL_UNLOCK(mh);

    return 0;
}

int
wq_hal_stop(struct wq_hal_vap *vap)
{
    struct wq_hal_priv *mh = MWLVAP(vap);
    //HostCmd_DS_BSS_START *pCmd;
    //int retval;

    WQ_HAL_LOCK(mh);
    //TODO : stop HAL
    
    /* NB: mark !running regardless */
    vap->flags &= ~MVF_RUNNING;
    WQ_HAL_UNLOCK(mh);
    return 0;
}

int
wq_hal_start(struct wq_hal_vap *vap)
{
    struct wq_hal_priv *mh = MWLVAP(vap);


    WQ_HAL_LOCK(mh);
    //TODO : start HAL

    vap->flags |= MVF_RUNNING;
    
    WQ_HAL_UNLOCK(mh);
    return 0;
}

int
wq_hal_setgprot(struct wq_hal *mh0, int prot)
{
    struct wq_hal_priv *mh = MWLPRIV(mh0);

    WQ_HAL_LOCK(mh);
    //TODO : set protection mode, protectio mode is controleld in TxDESC
    
    WQ_HAL_UNLOCK(mh);
    return 0;
}


int
wq_hal_setedcaparams(struct wq_hal *mh0, uint8_t qnum,
    uint32_t CWmin, uint32_t CWmax, uint8_t AIFSN,  uint16_t TXOPLimit, uint8_t slottime)
{
    struct wq_hal_priv *mh = MWLPRIV(mh0);
    uint8_t aifs;
    uint16_t txop;

    aifs = 10+(AIFSN*slottime);
    txop = TXOPLimit*32;
    
    WQ_HAL_LOCK(mh);

/*  Part 11: Wireless LAN Medium Access Control(MAC) and Physical Layer (PHY) Specifications
 *  EIFS calculating equation:
 *  DIFS = aSIFSTime + 2 ? aSlotTime    (9-3)
 *  EIFS = aSIFSTime + DIFS + ACKTxTime (9-4)
 *  aSIFSTime=10 us
 *  aSlotTime=20 us
 *  ACK:14 bytes: 14*8+192=304
 *  So: EIFS= 10+(10+2*20)+304=364
 */
    switch(qnum)
    {
        case HAL_TXQ_BE:
            mac_txq0_cw_sw_txq0_cw_max_set(CWmax);
            mac_txq0_cw_sw_txq0_cw_min_set(CWmin);
            mac_txq0_ifs_ctrl_sw_txq0_difs_set(aifs);
            mac_txq0_ifs_ctrl_sw_txq0_eifs_set(364);
            mac_txq0_period_sw_txq0_txop_set(txop); 
            mac_txq0_tx_timeout_sw_txq0_timeout_set(1000000);
            mac_txq0_ctrl_sw_txq0_cw_min_set_pls_set(1);
            break;
        case HAL_TXQ_BK:
            mac_txq1_cw_sw_txq1_cw_max_set(CWmax);
            mac_txq1_cw_sw_txq1_cw_min_set(CWmin);
            mac_txq1_ifs_ctrl_sw_txq1_difs_set(aifs);
            mac_txq1_ifs_ctrl_sw_txq1_eifs_set(364);
            mac_txq1_period_sw_txq1_txop_set(txop); 
            mac_txq1_tx_timeout_sw_txq1_timeout_set(1000000);
            mac_txq1_ctrl_sw_txq1_cw_min_set_pls_set(1);
            break;
        case HAL_TXQ_VI:
            mac_txq2_cw_sw_txq2_cw_max_set((1<<3)-1);
            mac_txq2_cw_sw_txq2_cw_min_set((1<<2)-1);
            mac_txq2_ifs_ctrl_sw_txq2_difs_set(20);
            mac_txq2_ifs_ctrl_sw_txq2_eifs_set(314);
            mac_txq2_period_sw_txq2_txop_set(txop); 
            mac_txq2_tx_timeout_sw_txq2_timeout_set(1000000);
            mac_txq2_ctrl_sw_txq2_cw_min_set_pls_set(1);
            break;
        case HAL_TXQ_VO:
            mac_txq3_cw_sw_txq3_cw_max_set(CWmax);
            mac_txq3_cw_sw_txq3_cw_min_set(CWmin);
            mac_txq3_ifs_ctrl_sw_txq3_difs_set(aifs);
            mac_txq3_ifs_ctrl_sw_txq3_eifs_set(364);
            mac_txq3_period_sw_txq3_txop_set(txop); 
            mac_txq3_tx_timeout_sw_txq3_timeout_set(1000000);
            mac_txq3_ctrl_sw_txq3_cw_min_set_pls_set(1);
            break;
        default:
            IOT_ASSERT(0); 
            break;
    }  

    WQ_HAL_UNLOCK(mh);
    return 0;
}


int
wq_hal_setnprot(struct wq_hal_vap *vap, WQ_HAL_HTPROTECT mode)
{
    struct wq_hal_priv *mh = MWLVAP(vap);
    //HostCmd_FW_SET_N_PROTECT_FLAG *pCmd;
    //int retval;

    /* XXX validate mode */
    WQ_HAL_LOCK(mh);
    //TODO : protection mode
    WQ_HAL_UNLOCK(mh);
    return 0;
}

int
wq_hal_setnprotmode(struct wq_hal_vap *vap, uint8_t mode)
{
    struct wq_hal_priv *mh = MWLVAP(vap);
    //HostCmd_FW_SET_N_PROTECT_OPMODE *pCmd;
    //int retval;
    DPRINTF(WDRV_DEBUG_STATE, "wq_hal_setnprotmode\n");

    WQ_HAL_LOCK(mh);
    //TODO : protection mode
    WQ_HAL_UNLOCK(mh);
    return 0;
}


int
wq_hal_setmimops(struct wq_hal *mh0, const uint8_t addr[IEEE80211_ADDR_LEN],
    uint8_t enable, uint8_t mode)
{
    //struct wq_hal_priv *mh = MWLPRIV(mh0);

    //WQ_HAL_LOCK(mh);
    //HAWK is 1x1, MIMO power save not required
    //WQ_HAL_UNLOCK(mh);
    return 0;
}

int wq_hal_setfilter(uint32_t filter0, uint32_t filter1)
{
    //set type and subtype
    mac_rx_filter_rule_0_sw_pv_filter_en_set(filter0); //disable protocol version 1,2,3 frame receiving
    mac_rx_filter_rule_2_sw_type_filter_en_1_set(filter1); 

    return 0;
}

int wq_hal_setpromisc(struct wq_hal *mh0, int ena)
{
    struct wq_hal_priv *mh = MWLPRIV(mh0);

    WQ_HAL_LOCK(mh);
    
    if (ena)
    {
        //set sniffer mode
        //no decrypt
        mac_rx_ctrl_sw_force_no_decrypt_en_set(1);
        //all protocol version
        mac_rx_filter_rule_0_sw_pv_filter_en_set(0x0); 
        //dont care mac address
        mac_rx_filter_rule_0_sw_addr_filter_en_set(3);
        //all type and subtype
        mac_rx_filter_rule_1_sw_type_filter_en_0_set(0); 
        mac_rx_filter_rule_2_sw_type_filter_en_1_set(0); 
        /* disable HW response frame : cts, ack, ba */
        mac_rx_ctrl_sw_not_response_ack_en_set(1);
        mac_rx_ctrl_sw_not_response_ba_en_set(1);
        mac_rx_ctrl_sw_not_response_cts_en_set(1);
    }
    else
    {
        //decrypt
        mac_rx_ctrl_sw_force_no_decrypt_en_set(0);
        //protocol version
        mac_rx_filter_rule_0_sw_pv_filter_en_set(0x3); 
        //mac address
        if ((hawk_vap0 != NULL) && (hawk_vap1 != NULL))
            mac_rx_filter_rule_0_sw_addr_filter_en_set(0); //role0/1_RA and all multicast receive.
        else
            mac_rx_filter_rule_0_sw_addr_filter_en_set(2); //role1_RA(STA) and all multicast receive.

        //set type and subtype
        mac_rx_filter_rule_0_sw_pv_filter_en_set(0xe); //disable protocol version 1,2,3 frame receiving
        mac_rx_filter_rule_1_sw_type_filter_en_0_set(0x3a7f8080); //disable rts/cts/ba/ack(default value under sta mode)
        mac_rx_filter_rule_2_sw_type_filter_en_1_set(0);
        /* enable HW response frame : cts, ack, ba */
        mac_rx_ctrl_sw_not_response_ack_en_set(0);
        mac_rx_ctrl_sw_not_response_ba_en_set(0);
        mac_rx_ctrl_sw_not_response_cts_en_set(0);
    }
    
    WQ_HAL_UNLOCK(mh);
    return 0;
}

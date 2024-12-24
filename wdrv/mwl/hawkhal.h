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

#ifndef _WQ_HAL_H_
#define _WQ_HAL_H_
#include "if_hawk_port.h"
/*
 * Hardware Access Layer for Marvell Wireless Devices.
 */

#define WQ_MBSS_SUPPORT        /* enable multi-bss support */

/*
 * Define total number of TX queues in the shared memory.
 * This count includes the EDCA queues, Block Ack queues, and HCCA queues
 * In addition to this, there could be a management packet queue some 
 * time in the future
 */
#define WQ_NUM_EDCA_QUEUES 4
#define WQ_NUM_HCCA_QUEUES 0
#define WQ_NUM_BA_QUEUES   0
#define WQ_NUM_MGMT_QUEUES 0
#define WQ_NUM_ACK_QUEUES  0
#if 0
#define WQ_NUM_TX_QUEUES \
    (WQ_NUM_EDCA_QUEUES + WQ_NUM_HCCA_QUEUES + WQ_NUM_BA_QUEUES + \
     WQ_NUM_MGMT_QUEUES + WQ_NUM_ACK_QUEUES)
#else
#define WQ_NUM_TX_QUEUES 4  //data_VO/data_VI/data_BK/data_BE
#endif

#define WQ_MAX_RXWCB_QUEUES    1

#define WQ_MAX_SUPPORTED_RATES 12
#define WQ_MAX_SUPPORTED_MCS   32

/* beacon related parameters */
#define WQ_PRE_TBTT_OFFSET   5000  //preTBTT 5ms
#define WQ_POST_TBTT_OFFSET  15000 //postTBTT 15ms
/* setting of the delta time between ppdu head to timestamp for hw
 * to correct the timestamp field wlan header for beacon/probe resp.
 * delta time = long preamble(192) + 24bytes payload in 1mbps
 */
#define WQ_BCN_TS_DELTA      (192 + 24 * 8)

typedef enum {
    WQ_HAL_OK
} WQ_HAL_STATUS;

/*
 * Transmit queue assignment.
 */
enum {
    WQ_WME_AC_BE     = 0,    /* best effort access category*/
    WQ_WME_AC_BK     = 1,    /* background access category */
    WQ_WME_AC_VI     = 2,    /* video access category */
    WQ_WME_AC_VO     = 3,    /* voice access category */
    WQ_WME_AC_MGMT   = 4,    /* MGMT */
    WQ_WME_AC_GROUP  = 5,    /* GROUP */
    WQ_WME_AC_BEACON = 6     /* BEACON */
};

struct wq_hal {
    uint32_t    mh_imask;   /* interrupt mask */
    /* remainder is opaque to driver */
};
struct wq_hal *wq_hal_attach(device_t dev, uint16_t devid);
void    wq_hal_detach(struct wq_hal *);


typedef enum {
    WQ_HAL_AP,
    WQ_HAL_STA,            /* infrastructure mode */
    WQ_HAL_IBSS            /* ibss/adhoc mode */
} WQ_HAL_BSSTYPE;
struct wq_hal_vap;

struct wq_hal_vap *wq_hal_newvap(struct wq_hal *, WQ_HAL_BSSTYPE,
    const uint8_t mac[6]);
void    wq_hal_delvap(struct wq_hal_vap *);

enum {
    WQ_HAL_DEBUG_SENDCMD   = 0x00000001,
    WQ_HAL_DEBUG_CMDDONE   = 0x00000002,
    WQ_HAL_DEBUG_IGNHANG   = 0x00000004,
};
//void    wq_hal_setdebug(struct wq_hal *, int);
//int wq_hal_getdebug(struct wq_hal *);

typedef struct {
    uint8_t nchannels;
    struct wq_hal_channel {
        //uint16_t freq;      /* channel center */
        uint8_t ieee;       /* channel number */
        int8_t maxTxPow;    /* max tx power (dBm) */
#define WQ_HAL_MAXCHAN 40
    } channels[WQ_HAL_MAXCHAN];
} WQ_HAL_CHANNELINFO;
int wq_hal_getchannelinfo(struct wq_hal *, int band, int chw,
        const WQ_HAL_CHANNELINFO **);


void    wq_hal_intrset(struct wq_hal *mh, uint32_t mask);



typedef struct {
    uint32_t    FreqBand : 6,
#define WQ_FREQ_BAND_2DOT4GHZ  0x1 
#define WQ_FREQ_BAND_5GHZ      0x4
        ChnlWidth: 5,
#define WQ_CH_10_MHz_WIDTH     0x1
#define WQ_CH_20_MHz_WIDTH     0x2
#define WQ_CH_40_MHz_WIDTH     0x4
        ExtChnlOffset: 2,
#define WQ_EXT_CH_NONE     0x0
#define WQ_EXT_CH_ABOVE_CTRL_CH 0x1
#define WQ_EXT_CH_BELOW_CTRL_CH 0x3
             : 19;      /* reserved */
} WQ_HAL_CHANNEL_FLAGS;

typedef struct {
    uint32_t    channel;
    WQ_HAL_CHANNEL_FLAGS channelFlags;
} WQ_HAL_CHANNEL;

/*
 * Get Hardware/Firmware capabilities.
 */
struct wq_hal_hwspec {
    uint8_t    hwVersion;       /* version of the HW */
    uint8_t    macAddr[6];      /* MAC address programmed in HW */
    uint16_t   regionCode;      /* EEPROM region code */
};
int wq_hal_gethwspecs(struct wq_hal *mh, struct wq_hal_hwspec *);



/*
 * Set HT Guard Interval.
 *
 * GIType = 0:  enable long and short GI
 * GIType = 1:  enable short GI
 * GIType = 2:  enable long GI
 */
int wq_hal_sethtgi(struct wq_hal_vap *, int GIType);



/*
 * Set Antenna Configuration (legacy operation).
 *
 * The RX antenna can be selected using the bitmask
 * ant (bit 0 = antenna 1, bit 1 = antenna 2, etc.)
 * (diversity?XXX)
 */
typedef enum {
    WL_ANTENNATYPE_RX = 1,
    WL_ANTENNATYPE_TX = 2,
} WQ_HAL_ANTENNA;

/*
 * Set the threshold for using RTS on TX.
 */
int wq_hal_setrtsthreshold(struct wq_hal_vap *, int threshold);

/*
 * Set the adapter to operate in infrastructure mode.
 */
int wq_hal_setinframode(struct wq_hal_vap *);

int wq_hal_set_beacon_interval(struct wq_hal_vap *vap, uint32_t beacon_interval);

int wq_hal_set_tbtt_timer(struct wq_hal_vap *vap, uint32_t beacon_interval, uint32_t pre_tbtt_offset, uint32_t post_tbtt_offset);

int wq_hal_clr_tbtt_timer(struct wq_hal_vap *vap);

int wq_hal_set_bcn_ts_delta(struct wq_hal_vap *vap, uint32_t delta);

/*
 * Set Radar Detection Configuration.
 */
typedef enum {
    DR_DFS_DISABLE          = 0,
    DR_CHK_CHANNEL_AVAILABLE_START  = 1,
    DR_CHK_CHANNEL_AVAILABLE_STOP   = 2,
    DR_IN_SERVICE_MONITOR_START = 3
} WQ_HAL_RADAR;
int wq_hal_setradardetection(struct wq_hal *mh, WQ_HAL_RADAR action);

/*
 * Initiate an 802.11h-based channel switch.  The CSA ie
 * is included in the next beacon(s) using the specified
 * information and the firmware counts down until switch
 * time after which it notifies the driver by delivering
 * an interrupt with MACREG_A2HRIC_BIT_CHAN_SWITCH set in
 * the cause register.
 */
int wq_hal_setchannelswitchie(struct wq_hal *,
       const struct ieee80211_channel *nextchan, uint32_t mode, uint32_t count);

/*
 * Set regdomain code (IEEE SKU).
 */
enum {
    DOMAIN_CODE_FCC     = 0x10, /* USA */
    DOMAIN_CODE_IC      = 0x20, /* Canda */
    DOMAIN_CODE_ETSI    = 0x30, /* Europe */
    DOMAIN_CODE_SPAIN   = 0x31, /* Spain */
    DOMAIN_CODE_FRANCE  = 0x32, /* France */
    DOMAIN_CODE_ETSI_131    = 0x130,/* ETSI w/ 1.3.1 radar type */
    DOMAIN_CODE_MKK     = 0x40, /* Japan */
    DOMAIN_CODE_MKK2    = 0x41, /* Japan w/ 10MHz chan spacing */
    DOMAIN_CODE_DGT     = 0x80, /* Taiwan */
    DOMAIN_CODE_AUS     = 0x81, /* Australia */
};

/*
 * Transmit rate control.  Rate codes with bit 0x80 set are
 * interpreted as MCS codes (this limits us to 0-127).  The
 * transmit rate can be set to a single fixed rate or can
 * be configured to start at an initial rate and drop based
 * on retry counts.
 */
typedef enum {
    RATE_AUTO   = 0,    /* rate selected by firmware */
    RATE_FIXED  = 2,    /* rate fixed */
    RATE_FIXED_DROP = 1,    /* rate starts fixed but may drop */
} WQ_HAL_TXRATE_HANDLING;

typedef struct {
    uint8_t McastRate;  /* rate for multicast frames */
#define RATE_MCS    0x80    /* rate is an MCS index */
    uint8_t MgtRate;    /* rate for management frames */
    struct {
        uint8_t TryCount;   /* try this many times */
        uint8_t Rate;   /* use this tx rate */
    } RateSeries[4];    /* rate series */
} WQ_HAL_TXRATE;


/*
 * Set the Slot Time Configuration.
 * NB: usecs must either be 9 or 20 for now.
 */
int wq_hal_setslottime(struct wq_hal *mh, int usecs);


/*
 * Crypto Configuration.
 */
typedef struct {
    uint16_t  pad;
    uint16_t  keyTypeId;
#define KEY_TYPE_ID_WEP     0
#define KEY_TYPE_ID_TKIP    1
#define KEY_TYPE_ID_AES     2   /* AES-CCMP */
    uint32_t  keyFlags;
#define KEY_FLAG_INUSE      0x00000001  /* indicate key is in use */
#define KEY_FLAG_RXGROUPKEY 0x00000002  /* Group key for RX only */
#define KEY_FLAG_TXGROUPKEY 0x00000004  /* Group key for TX */
#define KEY_FLAG_PAIRWISE   0x00000008  /* pairwise */
#define KEY_FLAG_RXONLY     0x00000010  /* only used for RX */
#define KEY_FLAG_AUTHENTICATOR  0x00000020  /* Key is for Authenticator */
#define KEY_FLAG_TSC_VALID  0x00000040  /* Sequence counters valid */
#define KEY_FLAG_WEP_KEY_STA    0x00000100  /* STA WEP key */
#define KEY_FLAG_WEP_TXKEY  0x01000000  /* Tx key for WEP */
#define KEY_FLAG_MICKEY_VALID   0x02000000  /* Tx/Rx MIC keys are valid */
    uint32_t  keyIndex;     /* for WEP only; actual key index */
    uint16_t  keyLen;       /* key size in bytes */
    uint8_t   keyId;        /* key id */
    union {         /* key material, keyLen gives size */
    uint8_t wep[16];    /* enough for 128 bits */
    uint8_t aes[16];
    struct {
        /* NB: group or pairwise key is determined by keyFlags */
        uint8_t keyMaterial[16];
        uint8_t txMic[8];
        uint8_t rxMic[8];
        struct {
            uint16_t low;
        uint32_t high;
        } rsc;
        struct {
            uint16_t low;
        uint32_t high;
        } tsc;
    } __packed tkip;
    }__packed key;
} __packed WQ_HAL_KEYVAL;


/*
 * Set the MAC address.
 */
int wq_hal_setmac(struct wq_hal_vap *, const uint8_t addr[6]);

/*
 * Set Association Configuration for station operation.
 */
int wq_hal_setassocid(struct wq_hal_vap *, const uint8_t bssId[6],
        uint16_t assocId);

/*
 * A-MPDU Block Ack (BA) stream support.  There are several
 * streams that the driver must multiplex.  Once assigned
 * to a station the driver queues frames to a corresponding
 * transmit queue and the firmware handles all the work.
 *
 * XXX no way to find out how many streams are supported
 */
typedef struct {
    void    *data[2];   /* opaque data */
    int txq;
} WQ_HAL_BASTREAM;

const WQ_HAL_BASTREAM *wq_hal_bastream_alloc(struct wq_hal_vap *,
        int ba_type, const uint8_t Macaddr[6], uint8_t Tid,
        uint8_t ParamInfo, void *, void *);
const WQ_HAL_BASTREAM *wq_hal_bastream_lookup(struct wq_hal *mh, int s);
int wq_hal_bastream_create(struct wq_hal_vap *, const WQ_HAL_BASTREAM *,
        int BarThrs, int WindowSize, uint16_t seqno);
int wq_hal_bastream_destroy(struct wq_hal *mh, const WQ_HAL_BASTREAM *);
int wq_hal_getwatchdogbitmap(struct wq_hal *mh, uint8_t bitmap[1]);
int wq_hal_bastream_get_seqno(struct wq_hal *mh, const WQ_HAL_BASTREAM *,
        const uint8_t Macaddr[6], uint16_t *pseqno);


int wq_hal_set_rx_ampdu(struct wq_hal_vap *vap, uint8_t id, uint8_t tid, uint8_t *addr, uint32_t seqno, uint32_t window);


/*
 * Inform the firmware of a new association station.
 * The address is the MAC address of the peer station.
 * The AID is supplied sans the 0xc000 bits.  The station
 * ID is defined by the caller.  The peer information must
 * be supplied.
 *
 * NB: All values are in host byte order; any byte swapping
 *     is handled by the hal.
 */
typedef struct {
    uint32_t LegacyRateBitMap;
    uint32_t HTRateBitMap;
    uint16_t CapInfo;
    uint16_t HTCapabilitiesInfo;
    uint8_t MacHTParamInfo;
    uint8_t Rev;
    struct {
        uint8_t ControlChan;
        uint8_t AddChan;
        uint8_t OpMode;
        uint8_t stbc;
    } __packed AddHtInfo;
} __packed WQ_HAL_PEERINFO;
int wq_hal_newstation(struct wq_hal_vap *, const uint8_t addr[6],
       uint16_t aid, uint16_t sid, const WQ_HAL_PEERINFO *,
       int isQosSta, int wmeInfo);
int wq_hal_delstation(struct wq_hal_vap *, const uint8_t addr[6]);


typedef enum {
    AP_MODE_B_ONLY = 1,
    AP_MODE_G_ONLY = 2,
    AP_MODE_MIXED = 3,
    AP_MODE_N_ONLY = 4,
    AP_MODE_BandN = 5,
    AP_MODE_GandN = 6,
    AP_MODE_BandGandN = 7,
    AP_MODE_A_ONLY = 8,
    AP_MODE_AandG = 10,
    AP_MODE_AandN = 12,
} WQ_HAL_APMODE;
int wq_hal_setapmode(struct wq_hal_vap *, WQ_HAL_APMODE);

/*
 * Enable/disable firmware operation.  wq_hal_start is
 * also used to sync state updates, e.g. beacon frame
 * reconstruction after content changes.
 */
int wq_hal_stop(struct wq_hal_vap *);
int wq_hal_start(struct wq_hal_vap *);

/*
 * Add/Remove station from Power Save TIM handling.
 *
 * If set is non-zero the AID is enabled, if zero it is removed.
 */
int wq_hal_updatetim(struct wq_hal_vap *, uint16_t aid, int set);

/*
 * Enable/disable 11g protection use.  This call specifies
 * the ERP information element flags to use.
 */
int wq_hal_setgprot(struct wq_hal *, int);


/*
 * Configure WMM EDCA parameters for the specified h/w ring.
 */
int wq_hal_setedcaparams(struct wq_hal *mh, uint8_t qnum,
       uint32_t CWmin, uint32_t CWmax, uint8_t AIFSN,  uint16_t TXOPLimit, uint8_t slottime);



/*
 * Configure 11n protection on/off.
 */
typedef enum {
    HTPROTECT_NONE   = 0,       /* disable */
    HTPROTECT_OPT    = 1,       /* optional */
    HTPROTECT_HT20   = 2,       /* protect only HT20 */
    HTPROTECT_HT2040 = 3,       /* protect HT20/40 */
    HTPROTECT_AUTO   = 4,       /* automatic */
}  WQ_HAL_HTPROTECT;
int wq_hal_setnprot(struct wq_hal_vap *, WQ_HAL_HTPROTECT mode);
/*
 * Configure 11n protection mechanism for when protection is enabled.
 */
int wq_hal_setnprotmode(struct wq_hal_vap *, uint8_t mode);



/*
 * Set MIMO Power Save handling for a station; the enable and mode
 * values come directly from the Action frame.
 */
int wq_hal_setmimops(struct wq_hal *mh, const uint8_t addr[6],
        uint8_t enable, uint8_t mode);


/*
 * Set/get promiscuous mode.
 */
int wq_hal_setpromisc(struct wq_hal *, int ena);
int wq_hal_getpromisc(struct wq_hal *);

/*
 * Enable/disable CF-End use.
 */
int wq_hal_setcfend(struct wq_hal *, int ena);


//Set HW chip power state
typedef enum {
	HAL_HW_SLEEP	    = 0,
	HAL_HW_AWAKE		= 1,
} HAL_HW_POWER_STATE;
int wq_hal_set_power_state(struct wq_hal_vap *vap, HAL_HW_POWER_STATE power_state);

uint8_t wq_hal_get_macid(struct wq_hal_vap *hvap);

#endif /* _WQ_HAL_H_ */

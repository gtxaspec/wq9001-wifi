#ifndef _NET_IF_MEDIA_H
#define _NET_IF_MEDIA_H

typedef int (*ifm_change_cb_t)(struct ifnet *ifp);
typedef void (*ifm_stat_cb_t)(struct ifnet *ifp, struct ifmediareq *req);

struct ifmedia_entry {
    WQ_LIST_ENTRY(ifmedia_entry) ifm_list;
    int ifm_media;                          /* description of this media attachment */
};

struct ifmedia {
    //int ifm_mask;                           /* mask of changes we don't care about */
    //int ifm_media;                          /* current user-set media word */
    //struct ifmedia_entry *ifm_cur;          /* currently selected media */
    //LIST_HEAD(, ifmedia_entry) ifm_list;    /* list of all supported media */
    //ifm_change_cb_t ifm_change;             /* media change driver callback */
    //ifm_stat_cb_t ifm_status;               /* media status driver callback */
};

/*
 * if_media Options word:
 *      Bits    Use
 *      ----    -------
 *      0-4     Media variant
 *      5-7     Media type
 *      8-15    Type specific options (includes added variant bits on Ethernet)
 *      16-18   Mode (for multi-mode devices)
 *      19      RFU
 *      20-27   Shared (global) options
 *      28-31   Instance
 */

/* IEEE 802.11 Wireless */
#define IFM_IEEE80211   0x00000080
/* NB: 0,1,2 are auto, manual, none defined below */
#define IFM_IEEE80211_FH1       3       /* Frequency Hopping 1Mbps */
#define IFM_IEEE80211_FH2       4       /* Frequency Hopping 2Mbps */
#define IFM_IEEE80211_DS1       5       /* Direct Sequence 1Mbps */
#define IFM_IEEE80211_DS2       6       /* Direct Sequence 2Mbps */
#define IFM_IEEE80211_DS5       7       /* Direct Sequence 5.5Mbps */
#define IFM_IEEE80211_DS11      8       /* Direct Sequence 11Mbps */
#define IFM_IEEE80211_DS22      9       /* Direct Sequence 22Mbps */
#define IFM_IEEE80211_OFDM6     10      /* OFDM 6Mbps */
#define IFM_IEEE80211_OFDM9     11      /* OFDM 9Mbps */
#define IFM_IEEE80211_OFDM12    12      /* OFDM 12Mbps */
#define IFM_IEEE80211_OFDM18    13      /* OFDM 18Mbps */
#define IFM_IEEE80211_OFDM24    14      /* OFDM 24Mbps */
#define IFM_IEEE80211_OFDM36    15      /* OFDM 36Mbps */
#define IFM_IEEE80211_OFDM48    16      /* OFDM 48Mbps */
#define IFM_IEEE80211_OFDM54    17      /* OFDM 54Mbps */
#define IFM_IEEE80211_OFDM72    18      /* OFDM 72Mbps */
#define IFM_IEEE80211_DS354k    19      /* Direct Sequence 354Kbps */
#define IFM_IEEE80211_DS512k    20      /* Direct Sequence 512Kbps */
#define IFM_IEEE80211_OFDM3     21      /* OFDM 3Mbps */
#define IFM_IEEE80211_OFDM4     22      /* OFDM 4.5Mbps */
#define IFM_IEEE80211_OFDM27    23      /* OFDM 27Mbps */
/* NB: not enough bits to express MCS fully */
#define IFM_IEEE80211_MCS       24      /* HT MCS rate */
#define IFM_IEEE80211_VHT       25      /* VHT MCS rate */

#define IFM_IEEE80211_ADHOC     0x00000100      /* Operate in Adhoc mode */
#define IFM_IEEE80211_HOSTAP    0x00000200      /* Operate in Host AP mode */
#define IFM_IEEE80211_IBSS      0x00000400      /* Operate in IBSS mode */
#define IFM_IEEE80211_WDS       0x00000800      /* Operate in WDS mode */
#define IFM_IEEE80211_TURBO     0x00001000      /* Operate in turbo mode */
#define IFM_IEEE80211_MONITOR   0x00002000      /* Operate in monitor mode */
#define IFM_IEEE80211_MBSS      0x00004000      /* Operate in MBSS mode */

/* operating mode for multi-mode devices */
#define IFM_IEEE80211_11A       0x00010000      /* 5Ghz, OFDM mode */
#define IFM_IEEE80211_11B       0x00020000      /* Direct Sequence mode */
#define IFM_IEEE80211_11G       0x00030000      /* 2Ghz, CCK mode */
#define IFM_IEEE80211_FH        0x00040000      /* 2Ghz, GFSK mode */
#define IFM_IEEE80211_11NA      0x00050000      /* 5Ghz, HT mode */
#define IFM_IEEE80211_11NG      0x00060000      /* 2Ghz, HT mode */
#define IFM_IEEE80211_VHT5G     0x00070000      /* 5Ghz, VHT mode */
#define IFM_IEEE80211_VHT2G     0x00080000      /* 2Ghz, VHT mode */

#define IFM_AUTO        0               /* Autoselect best media */
#define IFM_MANUAL      1               /* Jumper/dipswitch selects media */
#define IFM_NONE        2               /* Deselect all media */

#define IFM_FDX         0x00100000      /* Force full duplex */
#define IFM_HDX         0x00200000      /* Force half duplex */
#define IFM_FLOW        0x00400000      /* enable hardware flow control */
#define IFM_FLAG0       0x01000000      /* Driver defined flag */
#define IFM_FLAG1       0x02000000      /* Driver defined flag */
#define IFM_FLAG2       0x04000000      /* Driver defined flag */
#define IFM_LOOP        0x08000000      /* Put hardware in loopback */

#define IFM_NMASK       0x000000e0      /* Network type */
#define IFM_TMASK       0x0000001f      /* Media sub-type */
#define IFM_IMASK       0xf0000000      /* Instance */
#define IFM_ISHIFT      28              /* Instance shift */
#define IFM_OMASK       0x0000ff00      /* Type specific options */
#define IFM_MMASK       0x00070000      /* Mode */
#define IFM_MSHIFT      16              /* Mode shift */
#define IFM_GMASK       0x0ff00000      /* Global options */

#define IFM_AVALID      0x00000001      /* Active bit valid */
#define IFM_ACTIVE      0x00000002      /* Interface attached to working net */

/* Macros to extract various bits of information from the media word. */
#define IFM_TYPE(x)             ((x) & IFM_NMASK)
#define IFM_SUBTYPE(x)          ((x) & IFM_TMASK)
#define IFM_TYPE_MATCH(x,y) \
  (IFM_TYPE(x) == IFM_TYPE(y) && IFM_SUBTYPE(x) == IFM_SUBTYPE(y))
#define IFM_TYPE_OPTIONS(x)     ((x) & IFM_OMASK)
#define IFM_INST(x)             (((x) & IFM_IMASK) >> IFM_ISHIFT)
#define IFM_OPTIONS(x)          ((x) & (IFM_OMASK | IFM_GMASK))
#define IFM_MODE(x)             ((x) & IFM_MMASK)

#define IFM_INST_MAX            IFM_INST(IFM_IMASK)

/* Macro to create a media word. */
#define IFM_MAKEWORD(type, subtype, options, instance)                  \
        ((type) | (subtype) | (options) | ((instance) << IFM_ISHIFT))
#define IFM_MAKEMODE(mode) \
        (((mode) << IFM_MSHIFT) & IFM_MMASK)

void net_ifmedia_init(struct ifmedia *ifm,
                      int dontcare_mask,
                      ifm_change_cb_t change_callback,
                      ifm_stat_cb_t status_callback);
void net_ifmedia_removeall(struct ifmedia *ifm);
void net_ifmedia_add(struct ifmedia *ifm, int mword, int data, void *aux);
void net_ifmedia_set(struct ifmedia *ifm, int mword);
int net_ifmedia_ioctl(struct ifnet *ifp,
                      struct ifreq *ifr,
                      struct ifmedia *ifm,
                      unsigned int cmd);
void net_ifmedia_dump(struct ifmedia *ifm);
#endif

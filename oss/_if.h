#ifndef __IF_H
#define __IF_H
#if LINUX_VERSION_CODE == KERNEL_VERSION(3,0,8)
#include <linux/if.h>
#endif
#define IFNAMSIZ            16

/*
 * The following symbols identify read and write requirements for fields:
 *
 * (i) if_flags field set by device driver before attach, read-only there
 *     after.
 * (n) if_flags field written only by the network stack, read by either the
 *     stack or driver.
 * (d) if_drv_flags field written only by the device driver, read by either
 *     the stack or driver.
 */
#define IFF_UP          0x1             /* (n) interface is up */
#define IFF_BROADCAST   0x2             /* (i) broadcast address valid */
#define IFF_DEBUG       0x4             /* (n) turn on debugging */
#define IFF_LOOPBACK    0x8             /* (i) is a loopback net */
#define IFF_POINTOPOINT 0x10            /* (i) is a point-to-point link */
#define IFF_DRV_RUNNING 0x40            /* (d) resources allocated */
#define IFF_NOARP       0x80            /* (n) no address resolution protocol */
#define IFF_PROMISC     0x100           /* (n) receive all packets */
#define IFF_ALLMULTI    0x200           /* (n) receive all multicast packets */
#define IFF_DRV_OACTIVE 0x400           /* (d) tx hardware queue is full */
#define IFF_SIMPLEX     0x800           /* (i) can't hear own transmissions */
#define IFF_LINK0       0x1000          /* per link layer defined bit */
#define IFF_LINK1       0x2000          /* per link layer defined bit */
#define IFF_LINK2       0x4000          /* per link layer defined bit */
#define IFF_ALTPHYS     IFF_LINK2       /* use alternate physical connection */
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)
#define IFF_MULTICAST   0x8000          /* (i) supports multicast */
#endif
#define IFF_CANTCONFIG  0x10000         /* (i) unconfigurable using ioctl(2) */
#define IFF_PPROMISC    0x20000         /* (n) user-requested promisc mode */
#define IFF_MONITOR     0x40000         /* (n) user-requested monitor mode */
#define IFF_STATICARP   0x80000         /* (n) static ARP */
#define IFF_DYING       0x200000        /* (n) interface is winding down */
#define IFF_RENAMING    0x400000        /* (n) interface is being renamed */

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)
#define IFF_RUNNING         IFF_DRV_RUNNING
#endif
#define IFF_OACTIVE         IFF_DRV_OACTIVE

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)
#define IFF_CANTCHANGE                                              \
    (IFF_BROADCAST|IFF_POINTOPOINT|IFF_DRV_RUNNING|IFF_DRV_OACTIVE| \
     IFF_SIMPLEX|IFF_MULTICAST|IFF_ALLMULTI|IFF_PROMISC|            \
     IFF_DYING|IFF_CANTCONFIG)
#else
#define IFF_CANTCHANGE                                              \
    (IFF_BROADCAST|IFF_POINTOPOINT|IFF_DRV_RUNNING|IFF_DRV_OACTIVE| \
     IFF_SIMPLEX|IFF_ALLMULTI|IFF_PROMISC|            \
     IFF_DYING|IFF_CANTCONFIG)
#endif

#define	IFAN_ARRIVAL        0   /* interface arrival */
#define	IFAN_DEPARTURE      1   /* interface departure */

struct ifreq_buffer {
    size_t  length;
    void    *buffer;
};
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)
struct ifreq {
    char ifr_name[IFNAMSIZ];
    union {
        struct sockaddr ifru_addr;
        struct sockaddr ifru_dstaddr;
        struct sockaddr ifru_broadaddr;
        struct ifreq_buffer ifru_buffer;
        short ifru_flags[2];
        short ifru_index;
        int ifru_jid;
        int ifru_metric;
        int ifru_mtu;
        int ifru_phys;
        int ifru_media;
        caddr_t ifru_data;
        int ifru_cap[2];
        u_int   ifru_fib;
        u_char  ifru_vlan_pcp;
    } ifr_ifru;
#define ifr_addr        ifr_ifru.ifru_addr      /* address */
#define ifr_dstaddr     ifr_ifru.ifru_dstaddr   /* other end of p-to-p link */
#define ifr_broadaddr   ifr_ifru.ifru_broadaddr /* broadcast address */
#define ifr_buffer      ifr_ifru.ifru_buffer    /* user supplied buffer with its length */
#define ifr_flags       ifr_ifru.ifru_flags[0]  /* flags (low 16 bits) */
#define ifr_flagshigh   ifr_ifru.ifru_flags[1]  /* flags (high 16 bits) */
#define ifr_jid         ifr_ifru.ifru_jid       /* jail/vnet */
#define ifr_metric      ifr_ifru.ifru_metric    /* metric */
#define ifr_mtu         ifr_ifru.ifru_mtu       /* mtu */
#define ifr_phys        ifr_ifru.ifru_phys      /* physical wire */
#define ifr_media       ifr_ifru.ifru_media     /* physical media */
#define ifr_data        ifr_ifru.ifru_data      /* for use by interface */
#define ifr_reqcap      ifr_ifru.ifru_cap[0]    /* requested capabilities */
#define ifr_curcap      ifr_ifru.ifru_cap[1]    /* current capabilities */
#define ifr_index       ifr_ifru.ifru_index     /* interface index */
#define ifr_fib         ifr_ifru.ifru_fib       /* interface fib */
#define ifr_vlan_pcp    ifr_ifru.ifru_vlan_pcp  /* VLAN priority */
#define ifr_lan_pcp     ifr_ifru.ifru_vlan_pcp  /* VLAN priority */
};
#endif

union ifreq_union {
    struct ifreq ifr;
};

static inline void *ifr_data_get_ptr(void *ifrp)
{
    union ifreq_union *ifrup = (union ifreq_union *)ifrp;

    /* use another memory to carry ioctl parameter */

    IOT_ASSERT(ifrup->ifr.ifr_ifru.ifru_data);

    return (ifrup->ifr.ifr_ifru.ifru_data);
}

struct if_data {
    /* generic interface information */
    uint8_t ifi_addrlen;            /* media address length */
    uint8_t ifi_hdrlen;             /* media header length */
    uint8_t ifi_link_state;         /* current link state */
    uint16_t        ifi_datalen;    /* length of this data struct */
    uint32_t        ifi_mtu;        /* maximum transmission unit */

    /* volatile statistics */
    uint64_t        ifi_ipackets;   /* packets received on interface */
    uint64_t        ifi_ierrors;    /* input errors on interface */
    uint64_t        ifi_opackets;   /* packets sent on interface */
    uint64_t        ifi_oerrors;    /* output errors on interface */
    uint64_t        ifi_collisions; /* collisions on csma interfaces */
    uint64_t        ifi_ibytes;     /* total number of octets received */
    uint64_t        ifi_obytes;     /* total number of octets sent */
    uint64_t        ifi_imcasts;    /* packets received via multicast */
    uint64_t        ifi_omcasts;    /* packets sent via multicast */
    uint64_t        ifi_iqdrops;    /* dropped on input */
    uint64_t        ifi_oqdrops;    /* dropped on output */
    uint64_t        ifi_noproto;    /* destined for unsupported protocol */
};

/* WARN: casting to struct rt_msghdr */
struct if_msghdr {
    u_short ifm_msglen;     /* to skip over non-understood messages */
    u_char  ifm_version;    /* future binary compatibility */
    u_char  ifm_type;       /* message type */
    int     ifm_addrs;      /* like rtm_addrs */
    int     ifm_flags;      /* value of if_flags */
    u_short ifm_index;      /* index for associated ifp */
    u_short _ifm_spare1;
    struct  if_data ifm_data;/* statistics and other data about if */
};

/* WARN: casting to struct rt_msghdr */
struct if_announcemsghdr {
    u_short ifan_msglen;    /* to skip over non-understood messages */
    u_char  ifan_version;   /* future binary compatibility */
    u_char  ifan_type;      /* message type */
    u_short ifan_index;     /* index for associated ifp */
    char    ifan_name[IFNAMSIZ]; /* if name, e.g. "en0" */
    u_short ifan_what;      /* what type of announcement */
};


struct ifmediareq {
    char    ifm_name[IFNAMSIZ];     /* if name, e.g. "en0" */
    int     ifm_current;            /* current media options */
    int     ifm_mask;               /* don't care mask */
    int     ifm_status;             /* media status */
    int     ifm_active;             /* active options */
    int     ifm_count;              /* # entries in ifm_ulist array */
    int     *ifm_ulist;             /* media words */
};
#endif

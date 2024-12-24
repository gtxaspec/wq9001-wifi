#ifndef _NET_IF_H
#define _NET_IF_H
#include "_if.h"
//#include "_if_ioc.h"

#define net_printf iot_printf

/* the only support type */
#define IFT_ETHER       0x6             /* Ethernet CSMA/CD */

typedef enum {
    IFCOUNTER_IPACKETS = 0,
    IFCOUNTER_IERRORS,
    IFCOUNTER_OPACKETS,
    IFCOUNTER_OERRORS,
    IFCOUNTER_COLLISIONS,
    IFCOUNTER_IBYTES,
    IFCOUNTER_OBYTES,
    IFCOUNTER_IMCASTS,
    IFCOUNTER_OMCASTS,
    IFCOUNTER_IQDROPS,
    IFCOUNTER_OQDROPS,
    IFCOUNTER_NOPROTO,
    IFCOUNTERS /* Array size. */
} ift_counter;

struct ifnet;

#define IFC_NAMSIZ              8

struct if_clone {
    char ifc_name[IFC_NAMSIZ];
    struct ifnet *(*ifc_create)(struct if_clone *ifc, int unit, caddr_t params);
    void (*ifc_destroy)(struct ifnet *ifp);

    struct osal_atomic_t ref;
};

struct route {
    void *dummy;
};

typedef int (*if_ioctl_fn_t)(struct ifnet *ifp, unsigned int cmd, caddr_t data);
typedef void (*if_init_fn_t)(void *arg);
typedef void (*if_qflush_fn_t)(struct ifnet *ifp);
typedef int (*if_transmit_fn_t)(struct ifnet *ifp, struct mbuf *m);
typedef uint64_t (*if_get_counter_t)(struct ifnet *ifp, ift_counter type);

struct ifaddr {
    /* simply fixed address structure instead of AF_LINK sockaddr */
    uint8_t ifa_addr[8];
    int ifa_addrlen;
};

struct ifnet {
    /* OSS oriented */
    struct list_head list;
    struct list_head rxflt_list;

    /* FreeBSD oriented */
    struct if_clone *if_clone;
    void *if_softc;
    char if_xname[IFNAMSIZ];
    int if_index;
#define IF_UNIT_ID_NULL     (-1)
    int if_unit_id;
    int if_flags;
    int if_drv_flags;
    struct osal_atomic_t if_refcount;

#define	LINK_STATE_UNKNOWN	0	/* link invalid/unknown */
#define	LINK_STATE_DOWN		1	/* link is down */
#define	LINK_STATE_UP		2	/* link is up */
    int if_link_state;

    uint32_t if_mtu;
    struct work_struct if_linktask;
    struct ifaddr if_addr;
    const u_int8_t *if_broadcastaddr;
    u_int8_t *if_directedaddr;
    uint8_t if_hdrlen;

    /* simply treat all in/out mbuf as raw ethernet packet */
    int (*if_output)(struct ifnet *ifp,
                     struct mbuf *m,
                     const struct sockaddr *sk,
                     struct route *r);
    void (*if_input)(struct ifnet *ifp, struct mbuf *m);

    if_ioctl_fn_t if_ioctl;
    if_init_fn_t if_init;
    if_qflush_fn_t if_qflush;
    if_transmit_fn_t if_transmit;
    if_get_counter_t if_get_counter;

    counter_u64_t if_counters[IFCOUNTERS];
};

#define IF_LLADDR(ifp)       ((ifp)->if_addr.ifa_addr)
#define IF_LLADDR_LEN(ifp)   ((ifp)->if_addr.ifa_addrlen)

/* TOOD: not here */
#if 1
#define	IFNET_IS_UP_RUNNING(_ifp) \
	((_ifp)->if_drv_flags & IFF_DRV_RUNNING)
#else
#define IFNET_IS_UP_RUNNING(_ifp) \
    (((_ifp)->if_flags & IFF_UP) && \
     ((_ifp)->if_drv_flags & IFF_DRV_RUNNING))
#endif
void net_if_link_state_change(struct ifnet *ifp, int link_state);
struct ifnet *net_if_alloc(uint8_t type);
void net_if_free(struct ifnet *ifp);
void net_if_attach(struct ifnet *ifp);
void net_if_detach(struct ifnet *ifp);
void net_if_down(struct ifnet *ifp);
void net_if_up(struct ifnet *ifp);
void net_if_initname(struct ifnet *ifp, const char *name, int unit);
struct ifnet *net_if_byname(char *name);
struct ifnet *net_if_byindex(int idx);
uint64_t net_if_get_counter_default(struct ifnet *ifp, ift_counter cnt);
void net_if_inc_counter(struct ifnet *ifp, ift_counter cnt, int64_t inc);
void net_if_data_copy(struct ifnet *ifp, struct if_data *ifd);
int net_if_ioctl(struct ifnet *ifp, unsigned int cmd, caddr_t data);
#endif

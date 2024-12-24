#include "oss.h"

void net_if_link_state_change(struct ifnet *ifp, int link_state)
{
    /* Return if state hasn't changed. */
    if (ifp->if_link_state == link_state)
        return;

    ifp->if_link_state = link_state;

    //wk_insert(&ifp->if_linktask);

    return;
}

#if 0
static void __do_link_state_change(struct work_struct *work)
{
    struct ifnet *ifp = (struct ifnet *)WORK_ARGV(work);

    net_printf("net: link_state_change, idx %d\n", ifp->if_index);

    /* Notify that the link state has changed. */
    //route_ifmsg(ifp);

    net_printf("net: link state changed to %s\n",
                         (ifp->if_link_state == LINK_STATE_UP) ? "UP" : "DOWN");
    return;
}


struct ifnet *net_if_alloc(uint8_t type)
{
    struct ifnet *ifp = kmalloc(sizeof(struct ifnet), GFP_KERNEL);

    IOT_ASSERT(ifp);


    //wk_init(&ifp->if_linktask, __do_link_state_change, (void *)ifp);

    return ifp;
}

void net_if_free(struct ifnet *ifp)
{
    //osal_atomic_dec(&ifp->if_refcount);    

    //osal_mutex_destory(&ifp->event_mutex);

    //wk_deinit(&ifp->if_linktask);

    kfree(ifp);

    return;
}


static void __net_if_input_dummy(struct ifnet *ifp, struct mbuf *m)
{
	m_freem(m);

    return;
}


void net_if_attach(struct ifnet *ifp)
{
    BUG_ON((ifp->if_index == 0) || net_ifnet_registerred_by_idx(ifp->if_index));
	IOT_ASSERT(ifp->if_transmit && ifp->if_qflush);

    /* TODO: replace if_clone implementation */

    if (ifp->if_input == NULL)
        ifp->if_input = __net_if_input_dummy;

    net_ifnet_register(ifp);

    /* Announce the interface create */
    //supplicant is not ready at this moment, defer ifan arrival message
    //route_ifannouncemsg(ifp, IFAN_ARRIVAL);

    return;
}

void net_if_detach(struct ifnet *ifp)
{
    BUG_ON(net_ifnet_registerred_by_idx(ifp->if_index) == NULL);

	net_if_down(ifp);

    /* Announce the interface destory */
    route_ifannouncemsg(ifp->if_index, ifp->if_xname, IFAN_DEPARTURE);

    /* TODO: replace if_clone implementation */
    net_ifnet_deregister(ifp);

    return;
}


void net_if_down(struct ifnet *ifp)
{
    net_printf("net: ifp %p idx %d flags 0x%x DOWN\n", ifp, 
                                                       ifp->if_index,
                                                       ifp->if_flags);

    ifp->if_flags &= ~IFF_UP;
	ifp->if_qflush(ifp);

    route_ifmsg(ifp);

    return;
}

void net_if_up(struct ifnet *ifp)
{
    net_printf("net: ifp %p idx %d flags 0x%x UP\n", ifp, 
                                                     ifp->if_index,
                                                     ifp->if_flags);

    ifp->if_flags |= IFF_UP;
    route_ifmsg(ifp);

    return;
}
#endif
void net_if_initname(struct ifnet *ifp, const char *name, int unit)
{
    char xname[IFNAMSIZ] = { 0 }; 

    iot_snprintf(xname, IFNAMSIZ, "%s%d", name, unit);

    //BUG_ON(net_ifnet_registerred_by_name(xname));

    ifp->if_unit_id = unit;
    strncpy(ifp->if_xname, xname, IFNAMSIZ);

    net_printf("net: init ifp %p name %s unit %d\n", ifp, ifp->if_xname, unit);

    return;
}

#if 0
struct ifnet *net_if_byname(char *name)
{
    return net_ifnet_registerred_by_name(name);
}

struct ifnet *net_if_byindex(int idx)
{
    return net_ifnet_registerred_by_idx(idx);
}
#endif

void net_if_inc_counter(struct ifnet *ifp, ift_counter cnt, int64_t inc)
{

    //IOT_ASSERT(cnt < IFCOUNTERS);

    /* just for debug and don't care read-when-update in 32bit CPU */
    //ifp->if_counters[cnt]++;

    return;
}

#if 0
void net_if_data_copy(struct ifnet *ifp, struct if_data *ifd)
{
    //ifd->ifi_addrlen = IF_LLADDR_LEN(ifp);
    //ifd->ifi_hdrlen = ifp->if_hdrlen;
    ifd->ifi_link_state = ifp->if_link_state;
    ifd->ifi_datalen = sizeof(struct if_data);
    //ifd->ifi_mtu = ifp->if_mtu;
#if 0
    ifd->ifi_ipackets = ifp->if_get_counter(ifp, IFCOUNTER_IPACKETS);
    ifd->ifi_ierrors = ifp->if_get_counter(ifp, IFCOUNTER_IERRORS);
    ifd->ifi_opackets = ifp->if_get_counter(ifp, IFCOUNTER_OPACKETS);
    ifd->ifi_oerrors = ifp->if_get_counter(ifp, IFCOUNTER_OERRORS);
    ifd->ifi_collisions = ifp->if_get_counter(ifp, IFCOUNTER_COLLISIONS);
    ifd->ifi_ibytes = ifp->if_get_counter(ifp, IFCOUNTER_IBYTES);
    ifd->ifi_obytes = ifp->if_get_counter(ifp, IFCOUNTER_OBYTES);
    ifd->ifi_imcasts = ifp->if_get_counter(ifp, IFCOUNTER_IMCASTS);
    ifd->ifi_omcasts = ifp->if_get_counter(ifp, IFCOUNTER_OMCASTS);
    ifd->ifi_iqdrops = ifp->if_get_counter(ifp, IFCOUNTER_IQDROPS);
    ifd->ifi_oqdrops = ifp->if_get_counter(ifp, IFCOUNTER_OQDROPS);
    ifd->ifi_noproto = ifp->if_get_counter(ifp, IFCOUNTER_NOPROTO);
#endif
    return;
}

int net_if_ioctl(struct ifnet *ifp, unsigned int cmd, caddr_t data)
{
    struct ifreq *ifr = (struct ifreq *)data;
    int ret = 0, do_ifup = 0;
    int new_flags, temp_flags;

    net_printf("net: net_if_ioctl cmd 0x%x\n", cmd);

    switch (cmd) {
        case SIOCGIFINDEX:
            ifr->ifr_index = ifp->if_index;
            break;

        case SIOCGIFFLAGS:
            temp_flags = ifp->if_flags | ifp->if_drv_flags;
            ifr->ifr_flags = temp_flags & 0xffff;
            ifr->ifr_flagshigh = temp_flags >> 16;
            break;

        case SIOCGIFMTU:
            ifr->ifr_mtu = ETH_MAX_MTU;
            break;

        case SIOCSIFFLAGS:
            new_flags = (ifr->ifr_flags & 0xffff) |
                        (ifr->ifr_flagshigh << 16);
            if (ifp->if_flags & IFF_UP && (new_flags & IFF_UP) == 0)
                net_if_down(ifp);
            else if (new_flags & IFF_UP && (ifp->if_flags & IFF_UP) == 0)
                do_ifup = 1;

            if ((ifp->if_flags ^ new_flags) & IFF_PPROMISC) {
                if (new_flags & IFF_PPROMISC)
                    ifp->if_flags |= IFF_PROMISC;
            }

            ifp->if_flags = (ifp->if_flags & IFF_CANTCHANGE) |
                            (new_flags &~ IFF_CANTCHANGE);

            if (ifp->if_ioctl)
                ret = ifp->if_ioctl(ifp, cmd, (caddr_t)ifr);

            if (do_ifup)
                net_if_up(ifp);
            break;

        case SIOCADDMULTI:
        case SIOCDELMULTI:
            if ((ifp->if_flags & IFF_MULTICAST) == 0)
                return -EOPNOTSUPP;

            /*
             * Using SW to do MC rx-filter is too expensive and just offload
             * to driver. Currently, ifnet don't have MC table and driver should
             * maintain MC address pool by itself.
             */
            if (ifp->if_ioctl)
                ret = ifp->if_ioctl(ifp, cmd, (caddr_t)ifr);
            else
                return -EOPNOTSUPP;

            break;

#if 0   /* TODO: SIOCSIFNAME may need for P2P in the future. */
        case SIOCSIFNAME:
            break;
#endif

        default:
            ret = -ENOIOCTLCMD;
            break;
    }

    return ret;
}
#endif
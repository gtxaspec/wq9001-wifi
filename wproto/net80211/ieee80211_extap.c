#include "ieee80211_extap.h"

static extap_entry_t extap_tbl[NUM_EXTAPTBL_ENTRIES];

extap_entry_t *_extap_tbl_lookup(uint8_t *ip)
{
    int i;

    for (i = 0; i < NUM_EXTAPTBL_ENTRIES; i++) {
        if ((extap_tbl[i].use == 1) && (os_mem_cmp(ip, extap_tbl[i].ip, IPV4_IP_LEN) == 0)) {
            return &extap_tbl[i];
        }
    }
    return NULL;
}

uint8_t *extap_tbl_lookup(uint8_t *ip)
{
    extap_entry_t *entry = _extap_tbl_lookup(ip);
    if (entry) {
        return entry->mac;
    }

    return NULL;
}

extap_entry_t *extap_tbl_add(u_int8_t *ip, uint8_t *mac)
{
    int i;
    extap_entry_t *entry;

    if ((entry = _extap_tbl_lookup(ip)) != NULL) {
        return entry;
    }

    for (i = 0; i < NUM_EXTAPTBL_ENTRIES; i++) {
        if (extap_tbl[i].use == 0) {
            extap_tbl[i].use = 1;
            os_mem_cpy(extap_tbl[i].mac, mac, ETH_ALEN);
            os_mem_cpy(extap_tbl[i].ip, ip, IPV4_IP_LEN);

            return &extap_tbl[i];
        }
    }

    return NULL;
}

void extap_tbl_del(uint8_t *ip)
{
    extap_entry_t *entry = _extap_tbl_lookup(ip);
    if (entry) {
        entry->use = 0;
        return;
    }

    return;
}

void extap_tbl_dump()
{
    int i, j;

    printk("extap table dump:\n");
    for (i = 0; i < NUM_EXTAPTBL_ENTRIES; i++) {
        if (extap_tbl[i].use) {
            printk("idx:%d ip:", i);
            for (j = 0; j < IPV4_IP_LEN; j++) {
                printk("%d:", extap_tbl[i].ip[j]);
            }
            printk(" mac:");
            for (j = 0; j < ETH_ALEN; j++) {
                printk("%x:", extap_tbl[i].mac[j]);
            }
            printk("\n");
        }
    }

    return;
}

void extap_tbl_clear()
{
    int i;

    for (i = 0; i < NUM_EXTAPTBL_ENTRIES; i++) {
        extap_tbl[i].use = 0;
    }

    return;
}


int ieee80211_extap_input(struct ethhdr *eh, uint8_t *myaddr)
{
    uint8_t *mac;
    uint8_t *sip, *dip;
    struct iphdr *iphdr;
    eth_arphdr_t *arp = NULL;

    switch (ntohs(eh->h_proto)) {
        case ETH_P_IP:
            iphdr = (struct iphdr *)(eh + 1);
            sip = (uint8_t *)&iphdr->saddr;
            dip = (uint8_t *)&iphdr->daddr;
            break;

        case ETH_P_ARP:
            arp = (eth_arphdr_t *)(eh + 1);
            if (ntohs(arp->arp_op) == ARP_REQ ||
                ntohs(arp->arp_op) == ARP_RREQ) {
                return 0;
            }
            mac = extap_tbl_lookup(arp->arp_sip);
            if (mac) {
                IEEE80211_ADDR_COPY(arp->arp_sha, mac);
            }

            sip = arp->arp_sip;
            dip = arp->arp_tip;
            break;

        case ETH_P_PAE:
            IEEE80211_ADDR_COPY(eh->h_dest, myaddr);
            return 0;

        default:
            //printk("%s(%d): Uknown packet type - 0x%x\n", __func__, __LINE__, eh->h_proto);
            return 0;
    }

    mac = extap_tbl_lookup(dip);
    if (mac) {
        IEEE80211_ADDR_COPY(eh->h_dest, mac);
    } else {
        /* do nothing */
    }

    return 0;
}

static int ieee80211_extap_out_arp(struct ethhdr *eh, uint8_t *myaddr)
{
    eth_arphdr_t *arp = (eth_arphdr_t *)(eh + 1);

    //In order to avoid mistakenly modifying the pkt after receiving an ARP reply sent by the same IP device as hawk
    if(!os_mem_cmp(eh->h_source, myaddr, ETH_ALEN)) {
        return 0;
    }

    /* For ARP requests, note down the sender's details */
    extap_tbl_add(arp->arp_sip, arp->arp_sha);

    /* Modify eth frame as if we sent */
    IEEE80211_ADDR_COPY(eh->h_source, myaddr);

    /* Modify ARP content as if we initiated the req */
    IEEE80211_ADDR_COPY(arp->arp_sha, myaddr);
    /* For ARP replys, note down the target's details */
    if (ntohs(arp->arp_op) == ARP_RSP || ntohs(arp->arp_op) == ARP_RRSP) {
        extap_tbl_add(arp->arp_tip, arp->arp_tha);
    }

    return 0;
}

void _recompute_udp_checksum(struct iphdr *p_iph, unsigned short  *ip_payload) {
        register unsigned long sum = 0;
        struct udphdr *udphdrp = (struct udphdr *)ip_payload;
        unsigned short udpLen = htons(udphdrp->len);

        sum += (p_iph->saddr>>16)&0xFFFF;
        sum += (p_iph->saddr)&0xFFFF;
        sum += (p_iph->daddr>>16)&0xFFFF;
        sum += (p_iph->daddr)&0xFFFF;
        sum += htons(IPPROTO_UDP);
        sum += udphdrp->len;

        udphdrp->check = 0;
        while (udpLen > 1) {
          sum += * ip_payload++;
          udpLen -= 2;
        }

        if(udpLen > 0) {
          sum += ((*ip_payload)&htons(0xFF00));
        }

        while (sum>>16) {
          sum = (sum & 0xffff) + (sum >> 16);
        }
        sum = ~sum;

        udphdrp->check = ((unsigned short)sum == 0x0000)?0xFFFF:(unsigned short)sum;
}

static void ieee80211_extap_out_ipv4(struct ethhdr *eh, uint8_t *myaddr)
{
    struct iphdr *iph = (struct iphdr *)(eh + 1);

    if(iph->protocol == PROTO_UDP) {
        struct udphdr *udph = (struct udphdr *)(iph + 1);

        if (udph->dest == htons(DHCP_PORT)) {
            net_dhcphdr_t *dhcph = (net_dhcphdr_t *) (udph + 1);

            if(dhcph->dhcp_cookie[0] == 0x63 && dhcph->dhcp_cookie[1] == 0x82 &&
                dhcph->dhcp_cookie[2] == 0x53 && dhcph->dhcp_cookie[3] == 0x63) {
                if (dhcph->dhcp_msg_type == 1) { /* dhcp REQ or DISCOVER*/
                    /* Set broadcast bit in dhcp->flags to identify this is
                     * dhcp relay frame.
                     */
                    dhcph->dhcp_flags = htons(ntohs(dhcph->dhcp_flags) | 0x8000);
                    _recompute_udp_checksum(iph, (uint16_t *)udph);
                }
            }
        }
    }

    IEEE80211_ADDR_COPY(eh->h_source, myaddr);
}

int ieee80211_extap_output(struct ethhdr *eh, uint8_t *myaddr)
{
    switch(htons(eh->h_proto)) {
        case ETH_P_ARP:
            return ieee80211_extap_out_arp(eh, myaddr);

        case ETH_P_IP:
            ieee80211_extap_out_ipv4(eh, myaddr);
            break;

        case ETH_P_PAE:
            IEEE80211_ADDR_COPY(eh->h_source, myaddr);
            break;

        default:
            //printk("%s(%d): Uknown packet type - 0x%x\n", __func__, __LINE__, eh->h_proto);
            break;
    }

    return 0;
}


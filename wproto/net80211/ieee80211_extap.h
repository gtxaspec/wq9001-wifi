#ifndef __IEEE80211_EXTAP_H__
#define __IEEE80211_EXTAP_H__

#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
#include <uapi/linux/if_ether.h>
#include <uapi/linux/ip.h>
#include <uapi/linux/udp.h>
#include <uapi/linux/in.h>
#include <uapi/linux/icmp.h>
#else
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/in.h>
#include <linux/icmp.h>
#endif

#include "sdk.h"
#include "ieee80211_var.h"

#define ARP_REQ     1 /* ARP request */
#define ARP_RSP     2 /* ARP response */
#define ARP_RREQ    3 /* RARP request */
#define ARP_RRSP    4 /* RARP response */

#define NUM_EXTAPTBL_ENTRIES    16
#define IPV4_IP_LEN             4

#define PROTO_UDP   0x11
#define DHCP_PORT   67
#define PROTO_ICMP  0x1

typedef struct extap_entry {
    uint8_t use;
    uint8_t mac[ETH_ALEN];
    uint8_t ip[IPV4_IP_LEN];
} extap_entry_t;

typedef struct eth_arphdr {
    uint16_t arp_hrd;   /* format of hardware address */
    uint16_t arp_pro;   /* format of protocol address */
    uint8_t arp_hln;    /* length of hardware address */
    uint8_t arp_pln;    /* length of protocol address */
    uint16_t arp_op;    /* ARP opcode (command) */
    uint8_t arp_sha[ETH_ALEN];    /* sender hardware address */
    uint8_t arp_sip[IPV4_IP_LEN]; /* sender IP address */
    uint8_t arp_tha[ETH_ALEN];    /* target hardware address */
    uint8_t arp_tip[IPV4_IP_LEN]; /* target IP address */
} eth_arphdr_t;

typedef struct net_dhcphdr {
    uint8_t dhcp_msg_type;
    uint8_t dhcp_hw_type;
    uint8_t dhcp_hw_addr_len;
    uint8_t dhcp_num_hops;
    uint32_t dhcp_transc_id;
    uint16_t dhcp_secs_elapsed;
    uint16_t dhcp_flags;
    uint32_t dhcp_ciaddr; /* client IP (if client is in BOUND, RENEW or REBINDING state) */
    uint32_t dhcp_yiaddr; /* 'your' (client) IP address */
    /* IP address of next server to use in bootstrap, returned in DHCPOFFER, DHCPACK by server */
    uint32_t dhcp_siaddr_nip;
    uint32_t dhcp_gateway_nip; /* relay agent IP address */
    uint8_t dhcp_chaddr[16];  /* link-layer client hardware address (MAC) */
    uint8_t dhcp_sname[64];   /* server host name (ASCIZ) */
    uint8_t dhcp_file[128];   /* boot file name (ASCIZ) */
    uint8_t dhcp_cookie[4];   /* fixed first four option bytes (99,130,83,99 dec) */
 } net_dhcphdr_t;

typedef struct eapol_8021xhdr {
    uint8_t eapol_version;
    uint8_t eapol_type;
    uint16_t eapol_length;
    uint8_t eapol_key_desc_type;
    uint8_t eapol_key_info[2];
    uint16_t eapol_key_length;
    uint8_t eapol_replay_counter[8];
    uint8_t eapol_key_nonce[32];
    uint8_t eapol_key_iv[16];
    uint8_t eapol_key_rsc[8];
    uint8_t eapol_key_id[8];
    uint8_t eapol_key_mic[16];
    uint16_t eapol_key_data;
 } eapol_8021xhdr_t;

extap_entry_t *extap_tbl_add(uint8_t *, uint8_t *);
uint8_t *extap_tbl_lookup(uint8_t *);
void extap_tbl_del(uint8_t *);
void extap_tbl_dump();
void extap_tbl_clear();

int ieee80211_extap_input(struct ethhdr *eh, uint8_t *myaddr);
int ieee80211_extap_output(struct ethhdr *eh, uint8_t *myaddr);

#endif

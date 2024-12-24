#ifndef _WQ_WIFI_DBG_H
#define _WQ_WIFI_DBG_H

typedef enum {
    WIFI_DBG_PKT_TX    = 0,
    WIFI_DBG_PKT_RX    = 1
}WIFI_DBG_PKT_DIR;
extern int (*wifi_dbg_insert_func_ptr)(WIFI_DBG_PKT_DIR , void *, struct mbuf *, uint8_t cindex, uint8_t is_ampdu);

#ifndef HAWK_RELEASE_BUILD

//  Usage Note:
//  1. GDB
//  > dump binary memory dump.bin  start_addr end_addr
//  EX (128K) : dump binary memory dump.bin 0x100E0000 0x10100000
//  EX (32K) : dump binary memory  dump_32K_2.bin 0x10078000 0x10080000
//
//  2. OPENOCD
//  >halt
//  >dump_image  dump.bin 0x100E0000 0x20000
//  >exit
//
//  3. Gen pcapng
//  >python3 wifi/core/utils/gen_pcapng.py inputfile output.pcapng
//  EX:python3 wifi_core/utils/gen_pcapng.py  dump.bin dump.pcapng
//
//############################ format ###############################
//#  0                   1                   2                   3
//#  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//#  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//#  |                     PKT_START_PATTERN(4B)                     |
//#  +-------------------------------+-------------------------------+
//#  |                        MAC Time Stamp (4B)                    |
//#  +-------------------------------+-------------------------------+
//#  |     pkt_len(2B)               |   rate8b      |a|d|frame_cnt6b|
//#  +-------------------------------+-------------------------------+
//#  | cs4b  |  cindex6b |    rxrssi 8b   |   reserved 14 bits       |
//#  +-------------------------------+-------------------------------+
//#  |                      PROTO Time Stamp (4B)                    |
//#  +-------------------------------+-------------------------------+
//#  |                     DRIVER Time Stamp (4B)                    |
//#  +-------------------------------+-------------------------------+
//#  |                        HAL Time Stamp (4B)                    |
//#  +-------------------------------+-------------------------------+
//#  |                           pkt_data(32B)                       |
//#  |                           ............                        |
//#  |                           ............                        |
//#  +-------------------------------+-------------------------------+
//#
//#   rate8b: used rate
//#   frame_cnt6b: data send total cnt (TX only) (6bits)
//#   d     : direction (1bit), 0(tx),1(rx)
//#   a     : is ampdu (1bit)
//#   cs4b: complete status (4bits)
//#   cindex : 6bits (0~63)
//#
//############################ format ###############################


#define PKT_START_PATTERN 0x12344321
#define PKT_COPY_LEN   64

typedef struct _wifi_dbg_pkt{
    uint32_t start;
    uint32_t mac_ts;
    uint16_t len;
	
    uint16_t rate:8;
    uint16_t is_ampdu:1;
    uint16_t dir:1;
    uint16_t frame_cnt:6;
	
    uint32_t cs:4;
    uint32_t cindex:6;
    uint32_t rxrssi:8;
    uint32_t reserved2:6;
    uint32_t sn:8;
	
    uint32_t proto_ts;
    uint32_t driver_ts;
    uint32_t hal_ts;
    uint8_t pkt_data[PKT_COPY_LEN];
}__packed WIFI_DBG_PKT;

void wifi_dbg_enable(int enable);

#else ////HAWK_RELEASE_BUILD

#define wifi_dbg_enable(enable)
#endif
#endif //_WQ_WIFI_DBG_H

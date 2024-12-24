#ifndef _WQ_HW_CONFIG_H
#define _WQ_HW_CONFIG_H

//TODO: Base address need to sync with final design.
#define HAWK_MAC_REG_BASE 0x41000000
#define MAC_TOP_BASEADDR  0x41000000
#define MAC_TMR_BASEADDR  0x41100000
#define MAC_TXQ_BASEADDR  0x41200000
#define MAC_MPI_BASEADDR  0x41300000
#define MAC_TX_BASEADDR   0x41400000
#define MAC_RX_BASEADDR   0x41500000
#define MAC_RXR_BASEADDR  0x41600000
#define MAC_RAM_BASEADDR  0x41900000

#define assert(a)

#define MAC_READ_REG(addr) ((volatile uint32_t)*((volatile uint32_t*)(addr)))
#define MAC_WRITE_REG(addr, value) \
        (*((volatile uint32_t*)(addr)) = (value)); \
        DPRINTF(WDRV_DEBUG_HAL, "MAC_WRITE_REG addr:0x%X, val:0x%X\n",addr,value);

#define NO_MAC_WRITE_LOG

#ifdef NO_MAC_WRITE_LOG
#define MAC_WRITE_LOG(fn, addr,value) 
#else
#ifndef DTEST_MODE
#define MAC_WRITE_LOG(fn, addr, value) \
        DPRINTF(WDRV_DEBUG_HAL, "MAC_WRITE_REG addr:0x%08X, val:0x%08X [%s]\n",addr-MAC_TOP_BASEADDR+HAWK_MAC_REG_BASE,value,fn);
#else
#define MAC_WRITE_LOG(fn, addr, value) \
        DPRINTF(WDRV_DEBUG_HAL, "MAC_WRITE_REG addr:0x%08X, val:0x%08X [%s]\n",addr,value,fn);
#endif
#endif

#if 0
//for direct register access
#include "mac_mpi_reg.h"
#include "mac_rx_reg.h"
#include "mac_rxr_reg.h"
#include "mac_tmr_reg.h"
#include "mac_top_reg.h"
#include "mac_tx_reg.h"
#include "mac_txq_reg.h"
#else 
//for usb interface
//#include "mac_mpi_reg_usb.h"
#include "mac_rx_reg_usb.h"
#include "mac_rxr_reg_usb.h"
#include "mac_tmr_reg_usb.h"
#include "mac_top_reg_usb.h"
//#include "mac_tx_reg_usb.h"
#include "mac_txq_reg_usb.h"
#endif
//#include "mac_desc_define.h"


//Maximum number of subframes in an aggregation
#define HW_AMPDU_MAX_SUBFRAME_NUM   16

//Maximum retry counter for MPDU
#define MPDU_MAX_RETRY_CNT       7

//Legacy mode  TX Descripor number chain setting
#define LEGACY_TX_CHAIN_NUM 5

/* RX MBUF design

   mbuf->+------------------+ 
         |        .         |
         |        .         |
         |        .         |
         |        .         |    
 m_data->+------------------+
         |   mbuf address   | 4B
         +------------------+
         |  magic pattern   | 4B
Rx DESC_>+------------------+
         |        .         |
         |        .         |
         |        .         | HAL_RX_DESC_SIZE
         |        .         |
         |        .         |    
         |        .         |    
 Rx Buf->+------------------+ 
         |        .         |
         |        .         |
         |        .         |
         |        .         |
         |        .         |    
         |        .         |    
         |        .         |
         |        .         |    
         |        .         |    
         +------------------+ 
*/
#define HAL_MBUF_RX_MAGIC_PATTERN   0x0000
#define HAL_MBUF_RX_MAGIC_PATTERN_SIZE 4
#define HAL_MBUF_RX_DESC_OFFSET (HAL_MBUF_RX_MAGIC_PATTERN_SIZE+4)


#define HAL_RX_DATA_RING_SIZE   16 //must be 2^N
#if (((HAL_RX_DATA_RING_SIZE) & (HAL_RX_DATA_RING_SIZE-1)) != 0)
    HAL_RX_DATA_RING_SIZE must be 2^N;
#endif 

#define HAL_RX_MGMT_RING_SIZE   4 //must be 2^N
#if (((HAL_RX_MGMT_RING_SIZE) & (HAL_RX_MGMT_RING_SIZE-1)) != 0)
    HAL_RX_MGMT_RING_SIZE must be 2^N;
#endif 

    /* At least 802.11/QoS/HTC header (3-addr:30, do not support 4-addr:36) +
    *          IV (8 for AES, 18 for WAPI) +
    *          802.2 SNAP header (8) +
    *          802.3 payload (1500) +
    *          MIC/ICV (8 for AES, 16 for WAPI) +
    *          FCS (4) = 1558 or 1576(wapi) bytes
    */

#define HAL_RX_DATA_BUF_SIZE    1600 //should be multiple of 32, HW limitation
#define HAL_RX_MGMT_BUF_SIZE    1600 //should be multiple of 32  HW limitation
#define HAL_RX_DESC_SIZE        (16+12) //shall update this once Rx DESC size changed

#define MBUF_TO_RXBUF(m) (m->m_data+HAL_MBUF_RX_DESC_OFFSET+HAL_RX_DESC_SIZE)

//Rx DESC OWN bit
#define HAL_DESC_OWN_BY_HW      1
#define HAL_DESC_OWN_BY_SW      0



#define RX_HW_BUF_NOT_RDY             (99)              //HW RX Queue don't have finished RX buffer

/*****   Here is Tx Chain design  from spec  
         Use DUMMY_DESC, let HW know it's end of this TX DESC chain.

                +-------------+
       +------- |   TX_CTL    | 
       |        +-------------+      
       |        |  TX_STATUS  |     
       |        +-------------+    
       |        | TX_BUF_DESC | -----------> +--------+
       |        +-------------+              | Buffer |
       |        | TX_BUF_DESC | --+          +--------+
next_desc_ptr   +-------------+    \
       |        |      .      |     +------> +--------+
       |        +      .      |              | Buffer |
       |        |      .      |              +--------+
       |        +-------------+
       |        | TX_BUF_DESC | -----------> +--------+
       |        +-------------+              | Buffer |
       |                                     +--------+
       |    
       |
       +------> +-------------+
       +------- |   TX_CTL    | 
       |        +-------------+      
       |        |  TX_STATUS  |     
       |        +-------------+    
       |        | TX_BUF_DESC | -----------> +--------+
       |        +-------------+              | Buffer |
       |        | TX_BUF_DESC | --+          +--------+
next_desc_ptr   +-------------+    \
       |        |      .      |     +------> +--------+
       |        +      .      |              | Buffer |
       |        |      .      |              +--------+
       |        +-------------+
       |        | TX_BUF_DESC | -----------> +--------+
       |        +-------------+              | Buffer |
       |                                     +--------+
       |    
       |
       +------> +-------------+
                |  DUMMY_DESC | 
                +-------------+

******/

#define HAL_TXQ_BE      0
#define HAL_TXQ_BK      1
#define HAL_TXQ_VI      2
#define HAL_TXQ_VO      3
#define HAL_TXQ_MGMT    4
#define HAL_TXQ_GROUP   5
#define HAL_TXQ_BEACON  6
#define HAL_TXQ_NUM     7
 
#define HAL_MBUF_TX_MAGIC_PATTERN   0x00000000

#define HAL_HW_TX_BUF_DESC_SIZE     12  //TX_BUF_DESC size : 12 bytes
#define HAL_HW_TX_BUF_DESC_NUM      16 //Max number of TX_BUF_DESC in one Tx DESC : up to 16
#define HAL_HW_TX_DESC_SIZE         (CFG_HAWK_TX_DESC_TX_BUF_DESC_DWORD0_OFFSET + (HAL_HW_TX_BUF_DESC_SIZE*HAL_HW_TX_BUF_DESC_NUM)) //shall update this once Tx DESC size changed

#define HAL_MAX_TX_DESC_NUMBER      32
#define HAL_MAX_TX_DUMMY_DESC_NUMBER      (HAL_MAX_TX_DESC_NUMBER+HAL_TXQ_NUM)  //TXQ init should put a DUMMY descriptor first.
#ifdef AMPDU_TX_DEBUG
#define HAL_TX_DESC_SIZE            (HAL_HW_TX_DESC_SIZE+16) //4B magic pattern, 4B priv info, 4B seqno, 4B timestamp
#else
#define HAL_TX_DESC_SIZE            (HAL_HW_TX_DESC_SIZE+8) //4B magic pattern, 4B priv info
#endif
#define HAL_HW_DUMMY_DESC_SIZE      8 
#define HAL_DUMMY_DESC_SIZE         (HAL_HW_DUMMY_DESC_SIZE+8) //8B dummy desc, 4B magic pattern, 4B priv info



#define HAL_SET_TX_DESC_MAGIC(desc) (*(uint32_t*)((uint32_t)desc+HAL_HW_TX_DESC_SIZE) = HAL_MBUF_TX_MAGIC_PATTERN)
#define HAL_GET_TX_DESC_MAGIC(desc) (*(uint32_t*)((uint32_t)desc+HAL_HW_TX_DESC_SIZE))
#define HAL_SET_TX_DESC_REF_COUNT(desc, val) (*(uint32_t*)((uint32_t)desc+HAL_HW_TX_DESC_SIZE+4) = val)
#define HAL_GET_TX_DESC_REF_COUNT(desc) (*(uint32_t*)((uint32_t)desc+HAL_HW_TX_DESC_SIZE+4))
#ifdef AMPDU_TX_DEBUG
#define HAL_SET_TX_DESC_SEQ(desc, val) (*(uint32_t*)((uint32_t)desc+HAL_HW_TX_DESC_SIZE+8) = val)
#define HAL_GET_TX_DESC_SEQ(desc) (*(uint32_t*)((uint32_t)desc+HAL_HW_TX_DESC_SIZE+8))
#define HAL_SET_TX_DESC_TS(desc, val) (*(uint32_t*)((uint32_t)desc+HAL_HW_TX_DESC_SIZE+12) = val)
#define HAL_GET_TX_DESC_TS(desc) (*(uint32_t*)((uint32_t)desc+HAL_HW_TX_DESC_SIZE+12))
#endif

#define HAL_SET_DUMMY_DESC_MAGIC(desc) (*(uint32_t*)((uint32_t)desc+HAL_HW_DUMMY_DESC_SIZE) = HAL_MBUF_TX_MAGIC_PATTERN)
#define HAL_GET_DUMMY_DESC_MAGIC(desc) (*(uint32_t*)((uint32_t)desc+HAL_HW_DUMMY_DESC_SIZE))
#define HAL_SET_DUMMY_DESC_REF_COUNT(desc, val) (*(uint32_t*)((uint32_t)desc+HAL_HW_DUMMY_DESC_SIZE+4) = val)
#define HAL_GET_DUMMY_DESC_REF_COUNT(desc) (*(uint32_t*)((uint32_t)desc+HAL_HW_DUMMY_DESC_SIZE+4))

#ifndef container_of
#define container_of(ptr, type, member) ({  \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);  \
           (type *)( (char *)__mptr - offsetof(type,member) );})
#endif //#ifndef container_of

//Tx DESC OWN bit
#define HAL_TX_DESC_OWN_BY_HW       1
#define HAL_TX_DESC_OWN_BY_SW       0

//DUMMY DESC type
#define TYPE_TX_DESC                0
#define TYPE_DUMMY_DESC             1

//Tx DESC status
#define HAL_TXC_OK                  0


typedef struct _tx_statistics{
    uint32_t hw_no_write_back;
    uint32_t success;
    uint32_t rts_max_try;
    uint32_t timeout;
    uint32_t channel_timeout;
    uint32_t node_table_disable;
    uint32_t cts_max_try;
    uint32_t other_rsvd;
}tx_statistics;

struct hal_txq {
    void *head;
    void *tail;
    int16_t count;
    tx_statistics tx_stats;
};

/* data structure for desc setup  API */
struct hal_rate_info {
    uint8_t tx_rate;
    uint8_t tx_prot_rate;
    uint8_t tx_power;
    uint8_t tx_retry;
    uint8_t prot;
};

#define HAL_RATE_SET_SIZE           4

struct hal_rate_set_info {
    struct hal_rate_info rate[HAL_RATE_SET_SIZE];
};


typedef enum {
    TX_STATUS_HW_NO_WRITE_BACK    = 0,  //rsvd,HW never write back it
    TX_STATUS_SUCCESS             = 1,  //TX_OK
    TX_STATUS_RTS_MAX_TRY         = 2,  //RTS reach max ry num, tx fail
    TX_STATUS_TIMEOUT             = 3,  //txq reach max try num, tx fail
    TX_STATUS_CHANNEL_TIMEOUT     = 4,  //TXQ channel timeout
    TX_STATUS_NODE_TABLE_DISABLE  = 5,  //Node table tx disable
    TX_STATUS_CTS_MAX_TRY         = 6,  //CTS reach max try num, tx fail
    TX_STATUS_RESERVED            = 7,  //others:rsvd
}TX_STATUS;

//Interrupt BIT setting
#define MAC_INT_RXRN_STATUS_MASK (0xF0000)
#define MAC_INT_RXR1_NO_AVAIL_RX_DESC    (19) //  (1<<19)
#define MAC_INT_RXR1_MPDU_RX_DESC_DONE   (18) //  (1<<18)
#define MAC_INT_RXR0_NO_AVAIL_RX_DESC    (17) //  (1<<17)
#define MAC_INT_RXR0_MPDU_RX_DESC_DONE   (16) //  (1<<16)

#define MAC_INT_TXQN_TX_START_MASK (0x3F80)
#define MAC_INT_TXQ6_TX_START   (13)  //(1<<13)
#define MAC_INT_TXQ5_TX_START   (12)  //(1<<12)
#define MAC_INT_TXQ4_TX_START   (11)  //(1<<11)
#define MAC_INT_TXQ3_TX_START   (10)  //(1<<10)
#define MAC_INT_TXQ2_TX_START   (9)   //(1<<9)
#define MAC_INT_TXQ1_TX_START   (8)   //(1<<8)
#define MAC_INT_TXQ0_TX_START   (7)   //(1<<7)

#define MAC_INT_TXQN_DESC_DONE_MASK (0x7f)
#define MAC_INT_TXQ6_DESC_DONE   (6)   //1<<6)
#define MAC_INT_TXQ5_DESC_DONE   (5)   //1<<5)
#define MAC_INT_TXQ4_DESC_DONE   (4)   //1<<4)
#define MAC_INT_TXQ3_DESC_DONE   (3)   //1<<3)
#define MAC_INT_TXQ2_DESC_DONE   (2)   //1<<2)
#define MAC_INT_TXQ1_DESC_DONE   (1)   //1<<1)
#define MAC_INT_TXQ0_DESC_DONE   (0)   //1<<0)

#define MAC_INT_TSF_1_TBTT_POST  (1 << 5)
#define MAC_INT_TSF_1_TBTT_PRE   (1 << 4)
#define MAC_INT_TSF_1_TBTT       (1 << 3)
#define MAC_INT_TSF_0_TBTT_POST  (1 << 2)   // this role TSF tbtt timer reach to pre-set time relative to the standard TBTT boundary interrupt.
#define MAC_INT_TSF_0_TBTT_PRE   (1 << 1)   // this role TSF tbtt timer reach to pre-set time relative to the standard TBTT boundary interrupt.
#define MAC_INT_TSF_0_TBTT       (1 << 0)   // this role TSF tbtt timer reach to TBTT period boundary

//#define  FIX_CHANNEL 1  //Test only
#endif //#ifndef _WQ_HW_CONFIG_H

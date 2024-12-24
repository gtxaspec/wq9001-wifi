
//-----------------------------------
#define CFG_MAC_CTRL_ADDR 0x0000
#define SW_RO_MAC_VERSION_OFFSET 16
#define SW_RO_MAC_VERSION_MASK 0xFFFF0000
#define SW_NONHT_NBW_CTRL_OFFSET 4
#define SW_NONHT_NBW_CTRL_MASK 0x00000030
#define SW_MAC_ENABLE_OFFSET 0
#define SW_MAC_ENABLE_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_MST_CTRL_ADDR 0x0004
#define SW_MAC_MST_NEW_OFFSET 1
#define SW_MAC_MST_NEW_MASK 0x00000002
#define SW_AHB_TRANS_DONE_SEL_OFFSET 0
#define SW_AHB_TRANS_DONE_SEL_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_SIGTAP_CTRL_ADDR 0x0008
#define SW_TO_TXQS_SIGTAP_MUX_OFFSET 4
#define SW_TO_TXQS_SIGTAP_MUX_MASK 0x000000F0
#define SW_MAC_SIGTAP_MUX_OFFSET 0
#define SW_MAC_SIGTAP_MUX_MASK 0x0000000F

//-----------------------------------
#define CFG_MAC_DBG_AMPDU_RETRY_CTRL_ADDR 0x000c
#define SW_DBG_AMPDU_MONITOR_CNT_OFFSET 0
#define SW_DBG_AMPDU_MONITOR_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_INT_P0G0_CLR_ADDR 0x0010
#define SW_MAC_INT_P0G0_CLR_OFFSET 0
#define SW_MAC_INT_P0G0_CLR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P0G0_EN_ADDR 0x0014
#define SW_MAC_INT_P0G0_EN_OFFSET 0
#define SW_MAC_INT_P0G0_EN_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P0G0_MASK_ADDR 0x0018
#define SW_MAC_INT_P0G0_MASK_OFFSET 0
#define SW_MAC_INT_P0G0_MASK_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P0G0_RAW_ADDR 0x001c
#define SW_MAC_INT_P0G0_RAW_OFFSET 0
#define SW_MAC_INT_P0G0_RAW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P0G1_CLR_ADDR 0x0020
#define SW_MAC_INT_P0G1_CLR_OFFSET 0
#define SW_MAC_INT_P0G1_CLR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P0G1_EN_ADDR 0x0024
#define SW_MAC_INT_P0G1_EN_OFFSET 0
#define SW_MAC_INT_P0G1_EN_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P0G1_MASK_ADDR 0x0028
#define SW_MAC_INT_P0G1_MASK_OFFSET 0
#define SW_MAC_INT_P0G1_MASK_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P0G1_RAW_ADDR 0x002c
#define SW_MAC_INT_P0G1_RAW_OFFSET 0
#define SW_MAC_INT_P0G1_RAW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P1G0_CLR_ADDR 0x0030
#define SW_MAC_INT_P1G0_CLR_OFFSET 0
#define SW_MAC_INT_P1G0_CLR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P1G0_EN_ADDR 0x0034
#define SW_MAC_INT_P1G0_EN_OFFSET 0
#define SW_MAC_INT_P1G0_EN_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P1G0_MASK_ADDR 0x0038
#define SW_MAC_INT_P1G0_MASK_OFFSET 0
#define SW_MAC_INT_P1G0_MASK_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P1G0_RAW_ADDR 0x003c
#define SW_MAC_INT_P1G0_RAW_OFFSET 0
#define SW_MAC_INT_P1G0_RAW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P1G1_CLR_ADDR 0x0040
#define SW_MAC_INT_P1G1_CLR_OFFSET 0
#define SW_MAC_INT_P1G1_CLR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P1G1_EN_ADDR 0x0044
#define SW_MAC_INT_P1G1_EN_OFFSET 0
#define SW_MAC_INT_P1G1_EN_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P1G1_MASK_ADDR 0x0048
#define SW_MAC_INT_P1G1_MASK_OFFSET 0
#define SW_MAC_INT_P1G1_MASK_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_INT_P1G1_RAW_ADDR 0x004c
#define SW_MAC_INT_P1G1_RAW_OFFSET 0
#define SW_MAC_INT_P1G1_RAW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_CS_CTRL_ADDR 0x0100
#define SW_SET_RX_WAIT_TX_RESP_IDLE_OFFSET 13
#define SW_SET_RX_WAIT_TX_RESP_IDLE_MASK 0x00002000
#define SW_SET_RX_WAIT_TX_RESP_BUSY_OFFSET 12
#define SW_SET_RX_WAIT_TX_RESP_BUSY_MASK 0x00001000
#define SW_SET_MAC_TX_IDLE_OFFSET 11
#define SW_SET_MAC_TX_IDLE_MASK 0x00000800
#define SW_SET_MAC_TX_BUSY_OFFSET 10
#define SW_SET_MAC_TX_BUSY_MASK 0x00000400
#define SW_SET_MAC_RX_IDLE_OFFSET 9
#define SW_SET_MAC_RX_IDLE_MASK 0x00000200
#define SW_SET_MAC_RX_BUSY_OFFSET 8
#define SW_SET_MAC_RX_BUSY_MASK 0x00000100
#define SW_SET_VCS_IDLE_OFFSET 7
#define SW_SET_VCS_IDLE_MASK 0x00000080
#define SW_SET_VCS_BUSY_OFFSET 6
#define SW_SET_VCS_BUSY_MASK 0x00000040
#define SW_SET_PCS_ALIGN_IDLE_OFFSET 5
#define SW_SET_PCS_ALIGN_IDLE_MASK 0x00000020
#define SW_SET_PCS_ALIGN_BUSY_OFFSET 4
#define SW_SET_PCS_ALIGN_BUSY_MASK 0x00000010
#define SW_SET_PCS_SEC_IDLE_OFFSET 3
#define SW_SET_PCS_SEC_IDLE_MASK 0x00000008
#define SW_SET_PCS_SEC_BUSY_OFFSET 2
#define SW_SET_PCS_SEC_BUSY_MASK 0x00000004
#define SW_SET_PCS_PRI_IDLE_OFFSET 1
#define SW_SET_PCS_PRI_IDLE_MASK 0x00000002
#define SW_SET_PCS_PRI_BUSY_OFFSET 0
#define SW_SET_PCS_PRI_BUSY_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR 0x0120
#define SW_PPDU_SYMB_NUM_CEIL_PARA_OFFSET 4
#define SW_PPDU_SYMB_NUM_CEIL_PARA_MASK 0x000000F0
#define SW_PPDU_TIME_CAL_CEIL_PARA_OFFSET 0
#define SW_PPDU_TIME_CAL_CEIL_PARA_MASK 0x0000000F

//-----------------------------------
#define CFG_MAC_CRYPT_CTRL_ADDR 0x0140
#define SW_CRYPT_START_CLR_EN_OFFSET 2
#define SW_CRYPT_START_CLR_EN_MASK 0x00000004
#define SW_CRYPT_AAD1_USING_QC_ALL_OFFSET 1
#define SW_CRYPT_AAD1_USING_QC_ALL_MASK 0x00000002
#define SW_CRYPT_AAD1_QC_AMSDU_BIT_OFFSET 0
#define SW_CRYPT_AAD1_QC_AMSDU_BIT_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_RAM_FORCE_ON_ADDR 0x0180
#define SW_RAM_MPI_TX_FIFO_FORCE_ON_OFFSET 3
#define SW_RAM_MPI_TX_FIFO_FORCE_ON_MASK 0x00000008
#define SW_RAM_RC4_SBOX_FORCE_ON_OFFSET 2
#define SW_RAM_RC4_SBOX_FORCE_ON_MASK 0x00000004
#define SW_RAM_NODE_TABLE_FORCE_ON_OFFSET 1
#define SW_RAM_NODE_TABLE_FORCE_ON_MASK 0x00000002
#define SW_RAM_MPI_RX_FIFO_FORCE_ON_OFFSET 0
#define SW_RAM_MPI_RX_FIFO_FORCE_ON_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_TOP_DBG_CTRL_ADDR 0x0300
#define SW_MAC_TOP_DBG_GENERAL_CLR_OFFSET 0
#define SW_MAC_TOP_DBG_GENERAL_CLR_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_TOP_DBG_STS_0_ADDR 0x0310
#define SW_RO_TXR_OVERLAP_CNT_OFFSET 8
#define SW_RO_TXR_OVERLAP_CNT_MASK 0x0000FF00
#define SW_RO_TRX_START_SAME_CNT_OFFSET 0
#define SW_RO_TRX_START_SAME_CNT_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_TOP_DBG_STS_1_ADDR 0x0314
#define SW_RO_AHB_MST_SYNC_DBG_0_OFFSET 0
#define SW_RO_AHB_MST_SYNC_DBG_0_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TOP_DBG_STS_2_ADDR 0x0318
#define SW_RO_AHB_MST_SYNC_DBG_1_OFFSET 0
#define SW_RO_AHB_MST_SYNC_DBG_1_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_CRYPT_DBG_FSM_STS_ADDR 0x0320
#define SW_RO_CS_CCMP_OFFSET 16
#define SW_RO_CS_CCMP_MASK 0x000F0000
#define SW_RO_WEP_TKIP_FSM_OFFSET 12
#define SW_RO_WEP_TKIP_FSM_MASK 0x0000F000
#define SW_RO_CS_RC4_OFFSET 8
#define SW_RO_CS_RC4_MASK 0x00000300
#define SW_RO_CS_RC4_CORE_INIT_AND_SWAP_OFFSET 4
#define SW_RO_CS_RC4_CORE_INIT_AND_SWAP_MASK 0x000000F0
#define SW_RO_CS_RC4_PROCESS_OFFSET 0
#define SW_RO_CS_RC4_PROCESS_MASK 0x0000000F

//-----------------------------------
#define CFG_MAC_CCA_STAT_CTRL_ADDR 0x0380
#define SW_MAC_CCA_STAT_CLR_OFFSET 0
#define SW_MAC_CCA_STAT_CLR_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_CCA_TX_ING_STAT_ADDR 0x0390
#define SW_RO_MAC_CCA_TX_ING_STAT_OFFSET 0
#define SW_RO_MAC_CCA_TX_ING_STAT_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_CCA_BUSY_RX_ING_STAT_ADDR 0x0394
#define SW_RO_MAC_CCA_BUSY_RX_ING_STAT_OFFSET 0
#define SW_RO_MAC_CCA_BUSY_RX_ING_STAT_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_CCA_IDLE_RX_ING_STAT_ADDR 0x0398
#define SW_RO_MAC_CCA_IDLE_RX_ING_STAT_OFFSET 0
#define SW_RO_MAC_CCA_IDLE_RX_ING_STAT_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_CCA_BUSY_RX_LISTEN_STAT_ADDR 0x039c
#define SW_RO_MAC_CCA_BUSY_RX_LISTEN_STAT_OFFSET 0
#define SW_RO_MAC_CCA_BUSY_RX_LISTEN_STAT_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_CCA_IDLE_RX_LISTEN_STAT_ADDR 0x03a0
#define SW_RO_MAC_CCA_IDLE_RX_LISTEN_STAT_OFFSET 0
#define SW_RO_MAC_CCA_IDLE_RX_LISTEN_STAT_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR 0x0400
#define SW_DSSS_LONG_1M_OFFSET 24
#define SW_DSSS_LONG_1M_MASK 0xFF000000
#define SW_DSSS_LONG_2M_OFFSET 16
#define SW_DSSS_LONG_2M_MASK 0x00FF0000
#define SW_DSSS_LONG_5P5M_OFFSET 8
#define SW_DSSS_LONG_5P5M_MASK 0x0000FF00
#define SW_DSSS_LONG_11M_OFFSET 0
#define SW_DSSS_LONG_11M_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR 0x0404
#define SW_DSSS_SHORT_2M_OFFSET 24
#define SW_DSSS_SHORT_2M_MASK 0xFF000000
#define SW_DSSS_SHORT_5P5M_OFFSET 16
#define SW_DSSS_SHORT_5P5M_MASK 0x00FF0000
#define SW_DSSS_SHORT_11M_OFFSET 8
#define SW_DSSS_SHORT_11M_MASK 0x0000FF00
#define SW_OFDM_NON_RATE_0_OFFSET 0
#define SW_OFDM_NON_RATE_0_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR 0x0408
#define SW_OFDM_NON_RATE_1_OFFSET 24
#define SW_OFDM_NON_RATE_1_MASK 0xFF000000
#define SW_OFDM_NON_RATE_2_OFFSET 16
#define SW_OFDM_NON_RATE_2_MASK 0x00FF0000
#define SW_OFDM_NON_RATE_3_OFFSET 8
#define SW_OFDM_NON_RATE_3_MASK 0x0000FF00
#define SW_OFDM_NON_RATE_4_OFFSET 0
#define SW_OFDM_NON_RATE_4_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR 0x040c
#define SW_OFDM_NON_RATE_5_OFFSET 24
#define SW_OFDM_NON_RATE_5_MASK 0xFF000000
#define SW_OFDM_NON_RATE_6_OFFSET 16
#define SW_OFDM_NON_RATE_6_MASK 0x00FF0000
#define SW_OFDM_NON_RATE_7_OFFSET 8
#define SW_OFDM_NON_RATE_7_MASK 0x0000FF00
#define SW_OFDM_HT_20M_MCS_0_OFFSET 0
#define SW_OFDM_HT_20M_MCS_0_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR 0x0410
#define SW_OFDM_HT_20M_MCS_1_OFFSET 24
#define SW_OFDM_HT_20M_MCS_1_MASK 0xFF000000
#define SW_OFDM_HT_20M_MCS_2_OFFSET 16
#define SW_OFDM_HT_20M_MCS_2_MASK 0x00FF0000
#define SW_OFDM_HT_20M_MCS_3_OFFSET 8
#define SW_OFDM_HT_20M_MCS_3_MASK 0x0000FF00
#define SW_OFDM_HT_20M_MCS_4_OFFSET 0
#define SW_OFDM_HT_20M_MCS_4_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR 0x0414
#define SW_OFDM_HT_20M_MCS_5_OFFSET 24
#define SW_OFDM_HT_20M_MCS_5_MASK 0xFF000000
#define SW_OFDM_HT_20M_MCS_6_OFFSET 16
#define SW_OFDM_HT_20M_MCS_6_MASK 0x00FF0000
#define SW_OFDM_HT_20M_MCS_7_OFFSET 8
#define SW_OFDM_HT_20M_MCS_7_MASK 0x0000FF00
#define SW_OFDM_HT_40M_MCS_0_OFFSET 0
#define SW_OFDM_HT_40M_MCS_0_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR 0x0418
#define SW_OFDM_HT_40M_MCS_1_OFFSET 24
#define SW_OFDM_HT_40M_MCS_1_MASK 0xFF000000
#define SW_OFDM_HT_40M_MCS_2_OFFSET 16
#define SW_OFDM_HT_40M_MCS_2_MASK 0x00FF0000
#define SW_OFDM_HT_40M_MCS_3_OFFSET 8
#define SW_OFDM_HT_40M_MCS_3_MASK 0x0000FF00
#define SW_OFDM_HT_40M_MCS_4_OFFSET 0
#define SW_OFDM_HT_40M_MCS_4_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR 0x041c
#define SW_OFDM_HT_40M_MCS_5_OFFSET 24
#define SW_OFDM_HT_40M_MCS_5_MASK 0xFF000000
#define SW_OFDM_HT_40M_MCS_6_OFFSET 16
#define SW_OFDM_HT_40M_MCS_6_MASK 0x00FF0000
#define SW_OFDM_HT_40M_MCS_7_OFFSET 8
#define SW_OFDM_HT_40M_MCS_7_MASK 0x0000FF00
#define SW_OFDM_HT_40M_MCS_32_OFFSET 0
#define SW_OFDM_HT_40M_MCS_32_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR 0x0420
#define SW_OFDM_HT_SGI_RESP_SGI_EN_OFFSET 16
#define SW_OFDM_HT_SGI_RESP_SGI_EN_MASK 0x00010000
#define SW_HT_CBW20_RESP_FORCE_EN_OFFSET 7
#define SW_HT_CBW20_RESP_FORCE_EN_MASK 0x00000080
#define SW_HT_CBW20_RESP_FORCE_VAL_OFFSET 6
#define SW_HT_CBW20_RESP_FORCE_VAL_MASK 0x00000040
#define SW_HT_CBW40_RESP_FORCE_EN_OFFSET 5
#define SW_HT_CBW40_RESP_FORCE_EN_MASK 0x00000020
#define SW_HT_CBW40_RESP_FORCE_VAL_OFFSET 4
#define SW_HT_CBW40_RESP_FORCE_VAL_MASK 0x00000010
#define SW_NON_HT_CBW20_RESP_FORCE_EN_OFFSET 3
#define SW_NON_HT_CBW20_RESP_FORCE_EN_MASK 0x00000008
#define SW_NON_HT_CBW20_RESP_FORCE_VAL_OFFSET 2
#define SW_NON_HT_CBW20_RESP_FORCE_VAL_MASK 0x00000004
#define SW_NON_HT_CBW40_RESP_FORCE_EN_OFFSET 1
#define SW_NON_HT_CBW40_RESP_FORCE_EN_MASK 0x00000002
#define SW_NON_HT_CBW40_RESP_FORCE_VAL_OFFSET 0
#define SW_NON_HT_CBW40_RESP_FORCE_VAL_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR 0x0424
#define SW_OFDM_HT_RTS_RSP_RATE_OFFSET 8
#define SW_OFDM_HT_RTS_RSP_RATE_MASK 0x0000FF00
#define SW_FORCE_HT_RTS_RATE_SAME_CTS_OFFSET 1
#define SW_FORCE_HT_RTS_RATE_SAME_CTS_MASK 0x00000002
#define SW_FORCE_HT_RTS_RSP_RATE_EN_OFFSET 0
#define SW_FORCE_HT_RTS_RSP_RATE_EN_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_ROLE0_SELF_ADDR_LOW_ADDR 0x0600
#define SW_ROLE0_SELF_ADDR_LOW_OFFSET 0
#define SW_ROLE0_SELF_ADDR_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_ROLE0_SELF_ADDR_HIGH_ADDR 0x0604
#define SW_ROLE0_SELF_ADDR_HIGH_OFFSET 0
#define SW_ROLE0_SELF_ADDR_HIGH_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_ROLE0_AP_ADDR_LOW_ADDR 0x0608
#define SW_ROLE0_AP_ADDR_LOW_OFFSET 0
#define SW_ROLE0_AP_ADDR_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_ROLE0_AP_ADDR_HIGH_ADDR 0x060c
#define SW_ROLE0_AP_ADDR_HIGH_OFFSET 0
#define SW_ROLE0_AP_ADDR_HIGH_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_ROLE0_CTRL_ADDR 0x0610
#define SW_SELF_ALLOC_AID0_OFFSET 16
#define SW_SELF_ALLOC_AID0_MASK 0xFFFF0000
#define SW_IN_BSS_SELF_ROLE0_OFFSET 0
#define SW_IN_BSS_SELF_ROLE0_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_ROLE1_SELF_ADDR_LOW_ADDR 0x0700
#define SW_ROLE1_SELF_ADDR_LOW_OFFSET 0
#define SW_ROLE1_SELF_ADDR_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_ROLE1_SELF_ADDR_HIGH_ADDR 0x0704
#define SW_ROLE1_SELF_ADDR_HIGH_OFFSET 0
#define SW_ROLE1_SELF_ADDR_HIGH_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_ROLE1_AP_ADDR_LOW_ADDR 0x0708
#define SW_ROLE1_AP_ADDR_LOW_OFFSET 0
#define SW_ROLE1_AP_ADDR_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_ROLE1_AP_ADDR_HIGH_ADDR 0x070c
#define SW_ROLE1_AP_ADDR_HIGH_OFFSET 0
#define SW_ROLE1_AP_ADDR_HIGH_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_ROLE1_CTRL_ADDR 0x0710
#define SW_SELF_ALLOC_AID1_OFFSET 16
#define SW_SELF_ALLOC_AID1_MASK 0xFFFF0000
#define SW_IN_BSS_SELF_ROLE1_OFFSET 0
#define SW_IN_BSS_SELF_ROLE1_MASK 0x00000001

//HW module read/write macro




static __inline uint32_t mac_ctrl_sw_ro_mac_version_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR)) & SW_RO_MAC_VERSION_MASK) >> SW_RO_MAC_VERSION_OFFSET;
}
static __inline void mac_ctrl_sw_nonht_nbw_ctrl_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR);
	tmp &= ~SW_NONHT_NBW_CTRL_MASK;
	tmp |= (value << SW_NONHT_NBW_CTRL_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_ctrl_sw_nonht_nbw_ctrl_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR)) & SW_NONHT_NBW_CTRL_MASK) >> SW_NONHT_NBW_CTRL_OFFSET;
}
static __inline void mac_ctrl_sw_mac_enable_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR);
	tmp &= ~SW_MAC_ENABLE_MASK;
	tmp |= (value << SW_MAC_ENABLE_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_ctrl_sw_mac_enable_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR)) & SW_MAC_ENABLE_MASK) >> SW_MAC_ENABLE_OFFSET;
}
static __inline void mac_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR), value);
}
static __inline uint32_t mac_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR));
}
static __inline void mac_ctrl_pack(uint32_t sw_nonht_nbw_ctrl, uint32_t sw_mac_enable)
{
	uint32_t tmp;

	assert((((uint32_t)sw_nonht_nbw_ctrl << SW_NONHT_NBW_CTRL_OFFSET) & ~((uint32_t)SW_NONHT_NBW_CTRL_MASK)) == 0);
	assert((((uint32_t)sw_mac_enable << SW_MAC_ENABLE_OFFSET) & ~((uint32_t)SW_MAC_ENABLE_MASK)) == 0);
	tmp =
		((uint32_t)sw_nonht_nbw_ctrl << SW_NONHT_NBW_CTRL_OFFSET) |
		((uint32_t)sw_mac_enable << SW_MAC_ENABLE_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CTRL_ADDR), tmp);
}
static __inline void mac_ctrl_unpack(volatile uint32_t* sw_ro_mac_version, volatile uint32_t* sw_nonht_nbw_ctrl, volatile uint32_t* sw_mac_enable)
{
	uint32_t tmp;

	tmp = mac_ctrl_get();
	*sw_ro_mac_version = (tmp & ((uint32_t)SW_RO_MAC_VERSION_MASK)) >> SW_RO_MAC_VERSION_OFFSET;
	*sw_nonht_nbw_ctrl = (tmp & ((uint32_t)SW_NONHT_NBW_CTRL_MASK)) >> SW_NONHT_NBW_CTRL_OFFSET;
	*sw_mac_enable = (tmp & ((uint32_t)SW_MAC_ENABLE_MASK)) >> SW_MAC_ENABLE_OFFSET;
}
static __inline void mac_mst_ctrl_sw_mac_mst_new_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR);
	tmp &= ~SW_MAC_MST_NEW_MASK;
	tmp |= (value << SW_MAC_MST_NEW_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_mst_ctrl_sw_mac_mst_new_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR)) & SW_MAC_MST_NEW_MASK) >> SW_MAC_MST_NEW_OFFSET;
}
static __inline void mac_mst_ctrl_sw_ahb_trans_done_sel_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR);
	tmp &= ~SW_AHB_TRANS_DONE_SEL_MASK;
	tmp |= (value << SW_AHB_TRANS_DONE_SEL_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_mst_ctrl_sw_ahb_trans_done_sel_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR)) & SW_AHB_TRANS_DONE_SEL_MASK) >> SW_AHB_TRANS_DONE_SEL_OFFSET;
}
static __inline void mac_mst_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR), value);
}
static __inline uint32_t mac_mst_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR));
}
static __inline void mac_mst_ctrl_pack(uint32_t sw_mac_mst_new, uint32_t sw_ahb_trans_done_sel)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_mst_new << SW_MAC_MST_NEW_OFFSET) & ~((uint32_t)SW_MAC_MST_NEW_MASK)) == 0);
	assert((((uint32_t)sw_ahb_trans_done_sel << SW_AHB_TRANS_DONE_SEL_OFFSET) & ~((uint32_t)SW_AHB_TRANS_DONE_SEL_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_mst_new << SW_MAC_MST_NEW_OFFSET) |
		((uint32_t)sw_ahb_trans_done_sel << SW_AHB_TRANS_DONE_SEL_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_MST_CTRL_ADDR), tmp);
}
static __inline void mac_mst_ctrl_unpack(volatile uint32_t* sw_mac_mst_new, volatile uint32_t* sw_ahb_trans_done_sel)
{
	uint32_t tmp;

	tmp = mac_mst_ctrl_get();
	*sw_mac_mst_new = (tmp & ((uint32_t)SW_MAC_MST_NEW_MASK)) >> SW_MAC_MST_NEW_OFFSET;
	*sw_ahb_trans_done_sel = (tmp & ((uint32_t)SW_AHB_TRANS_DONE_SEL_MASK)) >> SW_AHB_TRANS_DONE_SEL_OFFSET;
}
static __inline void mac_sigtap_ctrl_sw_to_txqs_sigtap_mux_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR);
	tmp &= ~SW_TO_TXQS_SIGTAP_MUX_MASK;
	tmp |= (value << SW_TO_TXQS_SIGTAP_MUX_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_sigtap_ctrl_sw_to_txqs_sigtap_mux_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR)) & SW_TO_TXQS_SIGTAP_MUX_MASK) >> SW_TO_TXQS_SIGTAP_MUX_OFFSET;
}
static __inline void mac_sigtap_ctrl_sw_mac_sigtap_mux_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR);
	tmp &= ~SW_MAC_SIGTAP_MUX_MASK;
	tmp |= (value << SW_MAC_SIGTAP_MUX_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_sigtap_ctrl_sw_mac_sigtap_mux_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR)) & SW_MAC_SIGTAP_MUX_MASK) >> SW_MAC_SIGTAP_MUX_OFFSET;
}
static __inline void mac_sigtap_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR), value);
}
static __inline uint32_t mac_sigtap_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR));
}
static __inline void mac_sigtap_ctrl_pack(uint32_t sw_to_txqs_sigtap_mux, uint32_t sw_mac_sigtap_mux)
{
	uint32_t tmp;

	assert((((uint32_t)sw_to_txqs_sigtap_mux << SW_TO_TXQS_SIGTAP_MUX_OFFSET) & ~((uint32_t)SW_TO_TXQS_SIGTAP_MUX_MASK)) == 0);
	assert((((uint32_t)sw_mac_sigtap_mux << SW_MAC_SIGTAP_MUX_OFFSET) & ~((uint32_t)SW_MAC_SIGTAP_MUX_MASK)) == 0);
	tmp =
		((uint32_t)sw_to_txqs_sigtap_mux << SW_TO_TXQS_SIGTAP_MUX_OFFSET) |
		((uint32_t)sw_mac_sigtap_mux << SW_MAC_SIGTAP_MUX_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_SIGTAP_CTRL_ADDR), tmp);
}
static __inline void mac_sigtap_ctrl_unpack(volatile uint32_t* sw_to_txqs_sigtap_mux, volatile uint32_t* sw_mac_sigtap_mux)
{
	uint32_t tmp;

	tmp = mac_sigtap_ctrl_get();
	*sw_to_txqs_sigtap_mux = (tmp & ((uint32_t)SW_TO_TXQS_SIGTAP_MUX_MASK)) >> SW_TO_TXQS_SIGTAP_MUX_OFFSET;
	*sw_mac_sigtap_mux = (tmp & ((uint32_t)SW_MAC_SIGTAP_MUX_MASK)) >> SW_MAC_SIGTAP_MUX_OFFSET;
}
static __inline void mac_dbg_ampdu_retry_ctrl_sw_dbg_ampdu_monitor_cnt_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_DBG_AMPDU_RETRY_CTRL_ADDR);
	tmp &= ~SW_DBG_AMPDU_MONITOR_CNT_MASK;
	tmp |= (value << SW_DBG_AMPDU_MONITOR_CNT_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_DBG_AMPDU_RETRY_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_DBG_AMPDU_RETRY_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_dbg_ampdu_retry_ctrl_sw_dbg_ampdu_monitor_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_DBG_AMPDU_RETRY_CTRL_ADDR)) & SW_DBG_AMPDU_MONITOR_CNT_MASK) >> SW_DBG_AMPDU_MONITOR_CNT_OFFSET;
}
static __inline void mac_dbg_ampdu_retry_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_DBG_AMPDU_RETRY_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_DBG_AMPDU_RETRY_CTRL_ADDR), value);
}
static __inline uint32_t mac_dbg_ampdu_retry_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_DBG_AMPDU_RETRY_CTRL_ADDR));
}
static __inline void mac_dbg_ampdu_retry_ctrl_pack(uint32_t sw_dbg_ampdu_monitor_cnt)
{
	uint32_t tmp;

	assert((((uint32_t)sw_dbg_ampdu_monitor_cnt << SW_DBG_AMPDU_MONITOR_CNT_OFFSET) & ~((uint32_t)SW_DBG_AMPDU_MONITOR_CNT_MASK)) == 0);
	tmp =
		((uint32_t)sw_dbg_ampdu_monitor_cnt << SW_DBG_AMPDU_MONITOR_CNT_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_DBG_AMPDU_RETRY_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_DBG_AMPDU_RETRY_CTRL_ADDR), tmp);
}
static __inline void mac_dbg_ampdu_retry_ctrl_unpack(volatile uint32_t* sw_dbg_ampdu_monitor_cnt)
{
	uint32_t tmp;

	tmp = mac_dbg_ampdu_retry_ctrl_get();
	*sw_dbg_ampdu_monitor_cnt = (tmp & ((uint32_t)SW_DBG_AMPDU_MONITOR_CNT_MASK)) >> SW_DBG_AMPDU_MONITOR_CNT_OFFSET;
}
static __inline void mac_int_p0g0_clr_sw_mac_int_p0g0_clr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_CLR_ADDR);
	tmp &= ~SW_MAC_INT_P0G0_CLR_MASK;
	tmp |= (value << SW_MAC_INT_P0G0_CLR_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_CLR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_CLR_ADDR), tmp);
}
static __inline void mac_int_p0g0_clr_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_CLR_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_CLR_ADDR), value);
}
static __inline void mac_int_p0g0_clr_pack(uint32_t sw_mac_int_p0g0_clr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_int_p0g0_clr << SW_MAC_INT_P0G0_CLR_OFFSET) & ~((uint32_t)SW_MAC_INT_P0G0_CLR_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_int_p0g0_clr << SW_MAC_INT_P0G0_CLR_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_CLR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_CLR_ADDR), tmp);
}
static __inline void mac_int_p0g0_en_sw_mac_int_p0g0_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_EN_ADDR);
	tmp &= ~SW_MAC_INT_P0G0_EN_MASK;
	tmp |= (value << SW_MAC_INT_P0G0_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_EN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_EN_ADDR), tmp);
}
static __inline uint32_t mac_int_p0g0_en_sw_mac_int_p0g0_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_EN_ADDR)) & SW_MAC_INT_P0G0_EN_MASK) >> SW_MAC_INT_P0G0_EN_OFFSET;
}
static __inline void mac_int_p0g0_en_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_EN_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_EN_ADDR), value);
}
static __inline uint32_t mac_int_p0g0_en_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_EN_ADDR));
}
static __inline void mac_int_p0g0_en_pack(uint32_t sw_mac_int_p0g0_en)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_int_p0g0_en << SW_MAC_INT_P0G0_EN_OFFSET) & ~((uint32_t)SW_MAC_INT_P0G0_EN_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_int_p0g0_en << SW_MAC_INT_P0G0_EN_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_EN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_EN_ADDR), tmp);
}
static __inline void mac_int_p0g0_en_unpack(volatile uint32_t* sw_mac_int_p0g0_en)
{
	uint32_t tmp;

	tmp = mac_int_p0g0_en_get();
	*sw_mac_int_p0g0_en = (tmp & ((uint32_t)SW_MAC_INT_P0G0_EN_MASK)) >> SW_MAC_INT_P0G0_EN_OFFSET;
}
static __inline uint32_t mac_int_p0g0_mask_sw_mac_int_p0g0_mask_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_MASK_ADDR)) & SW_MAC_INT_P0G0_MASK_MASK) >> SW_MAC_INT_P0G0_MASK_OFFSET;
}
static __inline uint32_t mac_int_p0g0_mask_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_MASK_ADDR));
}
static __inline void mac_int_p0g0_mask_unpack(volatile uint32_t* sw_mac_int_p0g0_mask)
{
	uint32_t tmp;

	tmp = mac_int_p0g0_mask_get();
	*sw_mac_int_p0g0_mask = (tmp & ((uint32_t)SW_MAC_INT_P0G0_MASK_MASK)) >> SW_MAC_INT_P0G0_MASK_OFFSET;
}
static __inline uint32_t mac_int_p0g0_raw_sw_mac_int_p0g0_raw_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_RAW_ADDR)) & SW_MAC_INT_P0G0_RAW_MASK) >> SW_MAC_INT_P0G0_RAW_OFFSET;
}
static __inline uint32_t mac_int_p0g0_raw_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G0_RAW_ADDR));
}
static __inline void mac_int_p0g0_raw_unpack(volatile uint32_t* sw_mac_int_p0g0_raw)
{
	uint32_t tmp;

	tmp = mac_int_p0g0_raw_get();
	*sw_mac_int_p0g0_raw = (tmp & ((uint32_t)SW_MAC_INT_P0G0_RAW_MASK)) >> SW_MAC_INT_P0G0_RAW_OFFSET;
}
static __inline void mac_int_p0g1_clr_sw_mac_int_p0g1_clr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_CLR_ADDR);
	tmp &= ~SW_MAC_INT_P0G1_CLR_MASK;
	tmp |= (value << SW_MAC_INT_P0G1_CLR_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_CLR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_CLR_ADDR), tmp);
}
static __inline void mac_int_p0g1_clr_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_CLR_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_CLR_ADDR), value);
}
static __inline void mac_int_p0g1_clr_pack(uint32_t sw_mac_int_p0g1_clr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_int_p0g1_clr << SW_MAC_INT_P0G1_CLR_OFFSET) & ~((uint32_t)SW_MAC_INT_P0G1_CLR_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_int_p0g1_clr << SW_MAC_INT_P0G1_CLR_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_CLR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_CLR_ADDR), tmp);
}
static __inline void mac_int_p0g1_en_sw_mac_int_p0g1_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_EN_ADDR);
	tmp &= ~SW_MAC_INT_P0G1_EN_MASK;
	tmp |= (value << SW_MAC_INT_P0G1_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_EN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_EN_ADDR), tmp);
}
static __inline uint32_t mac_int_p0g1_en_sw_mac_int_p0g1_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_EN_ADDR)) & SW_MAC_INT_P0G1_EN_MASK) >> SW_MAC_INT_P0G1_EN_OFFSET;
}
static __inline void mac_int_p0g1_en_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_EN_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_EN_ADDR), value);
}
static __inline uint32_t mac_int_p0g1_en_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_EN_ADDR));
}
static __inline void mac_int_p0g1_en_pack(uint32_t sw_mac_int_p0g1_en)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_int_p0g1_en << SW_MAC_INT_P0G1_EN_OFFSET) & ~((uint32_t)SW_MAC_INT_P0G1_EN_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_int_p0g1_en << SW_MAC_INT_P0G1_EN_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_EN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_EN_ADDR), tmp);
}
static __inline void mac_int_p0g1_en_unpack(volatile uint32_t* sw_mac_int_p0g1_en)
{
	uint32_t tmp;

	tmp = mac_int_p0g1_en_get();
	*sw_mac_int_p0g1_en = (tmp & ((uint32_t)SW_MAC_INT_P0G1_EN_MASK)) >> SW_MAC_INT_P0G1_EN_OFFSET;
}
static __inline uint32_t mac_int_p0g1_mask_sw_mac_int_p0g1_mask_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_MASK_ADDR)) & SW_MAC_INT_P0G1_MASK_MASK) >> SW_MAC_INT_P0G1_MASK_OFFSET;
}
static __inline uint32_t mac_int_p0g1_mask_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_MASK_ADDR));
}
static __inline void mac_int_p0g1_mask_unpack(volatile uint32_t* sw_mac_int_p0g1_mask)
{
	uint32_t tmp;

	tmp = mac_int_p0g1_mask_get();
	*sw_mac_int_p0g1_mask = (tmp & ((uint32_t)SW_MAC_INT_P0G1_MASK_MASK)) >> SW_MAC_INT_P0G1_MASK_OFFSET;
}
static __inline uint32_t mac_int_p0g1_raw_sw_mac_int_p0g1_raw_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_RAW_ADDR)) & SW_MAC_INT_P0G1_RAW_MASK) >> SW_MAC_INT_P0G1_RAW_OFFSET;
}
static __inline uint32_t mac_int_p0g1_raw_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P0G1_RAW_ADDR));
}
static __inline void mac_int_p0g1_raw_unpack(volatile uint32_t* sw_mac_int_p0g1_raw)
{
	uint32_t tmp;

	tmp = mac_int_p0g1_raw_get();
	*sw_mac_int_p0g1_raw = (tmp & ((uint32_t)SW_MAC_INT_P0G1_RAW_MASK)) >> SW_MAC_INT_P0G1_RAW_OFFSET;
}
static __inline void mac_int_p1g0_clr_sw_mac_int_p1g0_clr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_CLR_ADDR);
	tmp &= ~SW_MAC_INT_P1G0_CLR_MASK;
	tmp |= (value << SW_MAC_INT_P1G0_CLR_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_CLR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_CLR_ADDR), tmp);
}
static __inline void mac_int_p1g0_clr_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_CLR_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_CLR_ADDR), value);
}
static __inline void mac_int_p1g0_clr_pack(uint32_t sw_mac_int_p1g0_clr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_int_p1g0_clr << SW_MAC_INT_P1G0_CLR_OFFSET) & ~((uint32_t)SW_MAC_INT_P1G0_CLR_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_int_p1g0_clr << SW_MAC_INT_P1G0_CLR_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_CLR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_CLR_ADDR), tmp);
}
static __inline void mac_int_p1g0_en_sw_mac_int_p1g0_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_EN_ADDR);
	tmp &= ~SW_MAC_INT_P1G0_EN_MASK;
	tmp |= (value << SW_MAC_INT_P1G0_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_EN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_EN_ADDR), tmp);
}
static __inline uint32_t mac_int_p1g0_en_sw_mac_int_p1g0_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_EN_ADDR)) & SW_MAC_INT_P1G0_EN_MASK) >> SW_MAC_INT_P1G0_EN_OFFSET;
}
static __inline void mac_int_p1g0_en_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_EN_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_EN_ADDR), value);
}
static __inline uint32_t mac_int_p1g0_en_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_EN_ADDR));
}
static __inline void mac_int_p1g0_en_pack(uint32_t sw_mac_int_p1g0_en)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_int_p1g0_en << SW_MAC_INT_P1G0_EN_OFFSET) & ~((uint32_t)SW_MAC_INT_P1G0_EN_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_int_p1g0_en << SW_MAC_INT_P1G0_EN_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_EN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_EN_ADDR), tmp);
}
static __inline void mac_int_p1g0_en_unpack(volatile uint32_t* sw_mac_int_p1g0_en)
{
	uint32_t tmp;

	tmp = mac_int_p1g0_en_get();
	*sw_mac_int_p1g0_en = (tmp & ((uint32_t)SW_MAC_INT_P1G0_EN_MASK)) >> SW_MAC_INT_P1G0_EN_OFFSET;
}
static __inline uint32_t mac_int_p1g0_mask_sw_mac_int_p1g0_mask_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_MASK_ADDR)) & SW_MAC_INT_P1G0_MASK_MASK) >> SW_MAC_INT_P1G0_MASK_OFFSET;
}
static __inline uint32_t mac_int_p1g0_mask_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_MASK_ADDR));
}
static __inline void mac_int_p1g0_mask_unpack(volatile uint32_t* sw_mac_int_p1g0_mask)
{
	uint32_t tmp;

	tmp = mac_int_p1g0_mask_get();
	*sw_mac_int_p1g0_mask = (tmp & ((uint32_t)SW_MAC_INT_P1G0_MASK_MASK)) >> SW_MAC_INT_P1G0_MASK_OFFSET;
}
static __inline uint32_t mac_int_p1g0_raw_sw_mac_int_p1g0_raw_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_RAW_ADDR)) & SW_MAC_INT_P1G0_RAW_MASK) >> SW_MAC_INT_P1G0_RAW_OFFSET;
}
static __inline uint32_t mac_int_p1g0_raw_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G0_RAW_ADDR));
}
static __inline void mac_int_p1g0_raw_unpack(volatile uint32_t* sw_mac_int_p1g0_raw)
{
	uint32_t tmp;

	tmp = mac_int_p1g0_raw_get();
	*sw_mac_int_p1g0_raw = (tmp & ((uint32_t)SW_MAC_INT_P1G0_RAW_MASK)) >> SW_MAC_INT_P1G0_RAW_OFFSET;
}
static __inline void mac_int_p1g1_clr_sw_mac_int_p1g1_clr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_CLR_ADDR);
	tmp &= ~SW_MAC_INT_P1G1_CLR_MASK;
	tmp |= (value << SW_MAC_INT_P1G1_CLR_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_CLR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_CLR_ADDR), tmp);
}
static __inline void mac_int_p1g1_clr_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_CLR_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_CLR_ADDR), value);
}
static __inline void mac_int_p1g1_clr_pack(uint32_t sw_mac_int_p1g1_clr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_int_p1g1_clr << SW_MAC_INT_P1G1_CLR_OFFSET) & ~((uint32_t)SW_MAC_INT_P1G1_CLR_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_int_p1g1_clr << SW_MAC_INT_P1G1_CLR_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_CLR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_CLR_ADDR), tmp);
}
static __inline void mac_int_p1g1_en_sw_mac_int_p1g1_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_EN_ADDR);
	tmp &= ~SW_MAC_INT_P1G1_EN_MASK;
	tmp |= (value << SW_MAC_INT_P1G1_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_EN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_EN_ADDR), tmp);
}
static __inline uint32_t mac_int_p1g1_en_sw_mac_int_p1g1_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_EN_ADDR)) & SW_MAC_INT_P1G1_EN_MASK) >> SW_MAC_INT_P1G1_EN_OFFSET;
}
static __inline void mac_int_p1g1_en_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_EN_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_EN_ADDR), value);
}
static __inline uint32_t mac_int_p1g1_en_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_EN_ADDR));
}
static __inline void mac_int_p1g1_en_pack(uint32_t sw_mac_int_p1g1_en)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_int_p1g1_en << SW_MAC_INT_P1G1_EN_OFFSET) & ~((uint32_t)SW_MAC_INT_P1G1_EN_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_int_p1g1_en << SW_MAC_INT_P1G1_EN_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_EN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_EN_ADDR), tmp);
}
static __inline void mac_int_p1g1_en_unpack(volatile uint32_t* sw_mac_int_p1g1_en)
{
	uint32_t tmp;

	tmp = mac_int_p1g1_en_get();
	*sw_mac_int_p1g1_en = (tmp & ((uint32_t)SW_MAC_INT_P1G1_EN_MASK)) >> SW_MAC_INT_P1G1_EN_OFFSET;
}
static __inline uint32_t mac_int_p1g1_mask_sw_mac_int_p1g1_mask_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_MASK_ADDR)) & SW_MAC_INT_P1G1_MASK_MASK) >> SW_MAC_INT_P1G1_MASK_OFFSET;
}
static __inline uint32_t mac_int_p1g1_mask_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_MASK_ADDR));
}
static __inline void mac_int_p1g1_mask_unpack(volatile uint32_t* sw_mac_int_p1g1_mask)
{
	uint32_t tmp;

	tmp = mac_int_p1g1_mask_get();
	*sw_mac_int_p1g1_mask = (tmp & ((uint32_t)SW_MAC_INT_P1G1_MASK_MASK)) >> SW_MAC_INT_P1G1_MASK_OFFSET;
}
static __inline uint32_t mac_int_p1g1_raw_sw_mac_int_p1g1_raw_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_RAW_ADDR)) & SW_MAC_INT_P1G1_RAW_MASK) >> SW_MAC_INT_P1G1_RAW_OFFSET;
}
static __inline uint32_t mac_int_p1g1_raw_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_INT_P1G1_RAW_ADDR));
}
static __inline void mac_int_p1g1_raw_unpack(volatile uint32_t* sw_mac_int_p1g1_raw)
{
	uint32_t tmp;

	tmp = mac_int_p1g1_raw_get();
	*sw_mac_int_p1g1_raw = (tmp & ((uint32_t)SW_MAC_INT_P1G1_RAW_MASK)) >> SW_MAC_INT_P1G1_RAW_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_rx_wait_tx_resp_idle_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_RX_WAIT_TX_RESP_IDLE_MASK;
	tmp |= (value << SW_SET_RX_WAIT_TX_RESP_IDLE_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_rx_wait_tx_resp_idle_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_RX_WAIT_TX_RESP_IDLE_MASK) >> SW_SET_RX_WAIT_TX_RESP_IDLE_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_rx_wait_tx_resp_busy_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_RX_WAIT_TX_RESP_BUSY_MASK;
	tmp |= (value << SW_SET_RX_WAIT_TX_RESP_BUSY_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_rx_wait_tx_resp_busy_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_RX_WAIT_TX_RESP_BUSY_MASK) >> SW_SET_RX_WAIT_TX_RESP_BUSY_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_mac_tx_idle_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_MAC_TX_IDLE_MASK;
	tmp |= (value << SW_SET_MAC_TX_IDLE_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_mac_tx_idle_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_MAC_TX_IDLE_MASK) >> SW_SET_MAC_TX_IDLE_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_mac_tx_busy_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_MAC_TX_BUSY_MASK;
	tmp |= (value << SW_SET_MAC_TX_BUSY_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_mac_tx_busy_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_MAC_TX_BUSY_MASK) >> SW_SET_MAC_TX_BUSY_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_mac_rx_idle_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_MAC_RX_IDLE_MASK;
	tmp |= (value << SW_SET_MAC_RX_IDLE_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_mac_rx_idle_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_MAC_RX_IDLE_MASK) >> SW_SET_MAC_RX_IDLE_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_mac_rx_busy_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_MAC_RX_BUSY_MASK;
	tmp |= (value << SW_SET_MAC_RX_BUSY_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_mac_rx_busy_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_MAC_RX_BUSY_MASK) >> SW_SET_MAC_RX_BUSY_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_vcs_idle_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_VCS_IDLE_MASK;
	tmp |= (value << SW_SET_VCS_IDLE_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_vcs_idle_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_VCS_IDLE_MASK) >> SW_SET_VCS_IDLE_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_vcs_busy_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_VCS_BUSY_MASK;
	tmp |= (value << SW_SET_VCS_BUSY_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_vcs_busy_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_VCS_BUSY_MASK) >> SW_SET_VCS_BUSY_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_pcs_align_idle_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_PCS_ALIGN_IDLE_MASK;
	tmp |= (value << SW_SET_PCS_ALIGN_IDLE_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_pcs_align_idle_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_PCS_ALIGN_IDLE_MASK) >> SW_SET_PCS_ALIGN_IDLE_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_pcs_align_busy_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_PCS_ALIGN_BUSY_MASK;
	tmp |= (value << SW_SET_PCS_ALIGN_BUSY_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_pcs_align_busy_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_PCS_ALIGN_BUSY_MASK) >> SW_SET_PCS_ALIGN_BUSY_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_pcs_sec_idle_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_PCS_SEC_IDLE_MASK;
	tmp |= (value << SW_SET_PCS_SEC_IDLE_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_pcs_sec_idle_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_PCS_SEC_IDLE_MASK) >> SW_SET_PCS_SEC_IDLE_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_pcs_sec_busy_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_PCS_SEC_BUSY_MASK;
	tmp |= (value << SW_SET_PCS_SEC_BUSY_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_pcs_sec_busy_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_PCS_SEC_BUSY_MASK) >> SW_SET_PCS_SEC_BUSY_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_pcs_pri_idle_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_PCS_PRI_IDLE_MASK;
	tmp |= (value << SW_SET_PCS_PRI_IDLE_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_pcs_pri_idle_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_PCS_PRI_IDLE_MASK) >> SW_SET_PCS_PRI_IDLE_OFFSET;
}
static __inline void mac_cs_ctrl_sw_set_pcs_pri_busy_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR);
	tmp &= ~SW_SET_PCS_PRI_BUSY_MASK;
	tmp |= (value << SW_SET_PCS_PRI_BUSY_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cs_ctrl_sw_set_pcs_pri_busy_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR)) & SW_SET_PCS_PRI_BUSY_MASK) >> SW_SET_PCS_PRI_BUSY_OFFSET;
}
static __inline void mac_cs_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), value);
}
static __inline uint32_t mac_cs_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR));
}
static __inline void mac_cs_ctrl_pack(uint32_t sw_set_rx_wait_tx_resp_idle, uint32_t sw_set_rx_wait_tx_resp_busy, uint32_t sw_set_mac_tx_idle, uint32_t sw_set_mac_tx_busy, uint32_t sw_set_mac_rx_idle, uint32_t sw_set_mac_rx_busy, uint32_t sw_set_vcs_idle, uint32_t sw_set_vcs_busy, uint32_t sw_set_pcs_align_idle, uint32_t sw_set_pcs_align_busy, uint32_t sw_set_pcs_sec_idle, uint32_t sw_set_pcs_sec_busy, uint32_t sw_set_pcs_pri_idle, uint32_t sw_set_pcs_pri_busy)
{
	uint32_t tmp;

	assert((((uint32_t)sw_set_rx_wait_tx_resp_idle << SW_SET_RX_WAIT_TX_RESP_IDLE_OFFSET) & ~((uint32_t)SW_SET_RX_WAIT_TX_RESP_IDLE_MASK)) == 0);
	assert((((uint32_t)sw_set_rx_wait_tx_resp_busy << SW_SET_RX_WAIT_TX_RESP_BUSY_OFFSET) & ~((uint32_t)SW_SET_RX_WAIT_TX_RESP_BUSY_MASK)) == 0);
	assert((((uint32_t)sw_set_mac_tx_idle << SW_SET_MAC_TX_IDLE_OFFSET) & ~((uint32_t)SW_SET_MAC_TX_IDLE_MASK)) == 0);
	assert((((uint32_t)sw_set_mac_tx_busy << SW_SET_MAC_TX_BUSY_OFFSET) & ~((uint32_t)SW_SET_MAC_TX_BUSY_MASK)) == 0);
	assert((((uint32_t)sw_set_mac_rx_idle << SW_SET_MAC_RX_IDLE_OFFSET) & ~((uint32_t)SW_SET_MAC_RX_IDLE_MASK)) == 0);
	assert((((uint32_t)sw_set_mac_rx_busy << SW_SET_MAC_RX_BUSY_OFFSET) & ~((uint32_t)SW_SET_MAC_RX_BUSY_MASK)) == 0);
	assert((((uint32_t)sw_set_vcs_idle << SW_SET_VCS_IDLE_OFFSET) & ~((uint32_t)SW_SET_VCS_IDLE_MASK)) == 0);
	assert((((uint32_t)sw_set_vcs_busy << SW_SET_VCS_BUSY_OFFSET) & ~((uint32_t)SW_SET_VCS_BUSY_MASK)) == 0);
	assert((((uint32_t)sw_set_pcs_align_idle << SW_SET_PCS_ALIGN_IDLE_OFFSET) & ~((uint32_t)SW_SET_PCS_ALIGN_IDLE_MASK)) == 0);
	assert((((uint32_t)sw_set_pcs_align_busy << SW_SET_PCS_ALIGN_BUSY_OFFSET) & ~((uint32_t)SW_SET_PCS_ALIGN_BUSY_MASK)) == 0);
	assert((((uint32_t)sw_set_pcs_sec_idle << SW_SET_PCS_SEC_IDLE_OFFSET) & ~((uint32_t)SW_SET_PCS_SEC_IDLE_MASK)) == 0);
	assert((((uint32_t)sw_set_pcs_sec_busy << SW_SET_PCS_SEC_BUSY_OFFSET) & ~((uint32_t)SW_SET_PCS_SEC_BUSY_MASK)) == 0);
	assert((((uint32_t)sw_set_pcs_pri_idle << SW_SET_PCS_PRI_IDLE_OFFSET) & ~((uint32_t)SW_SET_PCS_PRI_IDLE_MASK)) == 0);
	assert((((uint32_t)sw_set_pcs_pri_busy << SW_SET_PCS_PRI_BUSY_OFFSET) & ~((uint32_t)SW_SET_PCS_PRI_BUSY_MASK)) == 0);
	tmp =
		((uint32_t)sw_set_rx_wait_tx_resp_idle << SW_SET_RX_WAIT_TX_RESP_IDLE_OFFSET) |
		((uint32_t)sw_set_rx_wait_tx_resp_busy << SW_SET_RX_WAIT_TX_RESP_BUSY_OFFSET) |
		((uint32_t)sw_set_mac_tx_idle << SW_SET_MAC_TX_IDLE_OFFSET) |
		((uint32_t)sw_set_mac_tx_busy << SW_SET_MAC_TX_BUSY_OFFSET) |
		((uint32_t)sw_set_mac_rx_idle << SW_SET_MAC_RX_IDLE_OFFSET) |
		((uint32_t)sw_set_mac_rx_busy << SW_SET_MAC_RX_BUSY_OFFSET) |
		((uint32_t)sw_set_vcs_idle << SW_SET_VCS_IDLE_OFFSET) |
		((uint32_t)sw_set_vcs_busy << SW_SET_VCS_BUSY_OFFSET) |
		((uint32_t)sw_set_pcs_align_idle << SW_SET_PCS_ALIGN_IDLE_OFFSET) |
		((uint32_t)sw_set_pcs_align_busy << SW_SET_PCS_ALIGN_BUSY_OFFSET) |
		((uint32_t)sw_set_pcs_sec_idle << SW_SET_PCS_SEC_IDLE_OFFSET) |
		((uint32_t)sw_set_pcs_sec_busy << SW_SET_PCS_SEC_BUSY_OFFSET) |
		((uint32_t)sw_set_pcs_pri_idle << SW_SET_PCS_PRI_IDLE_OFFSET) |
		((uint32_t)sw_set_pcs_pri_busy << SW_SET_PCS_PRI_BUSY_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CS_CTRL_ADDR), tmp);
}
static __inline void mac_cs_ctrl_unpack(volatile uint32_t* sw_set_rx_wait_tx_resp_idle, volatile uint32_t* sw_set_rx_wait_tx_resp_busy, volatile uint32_t* sw_set_mac_tx_idle, volatile uint32_t* sw_set_mac_tx_busy, volatile uint32_t* sw_set_mac_rx_idle, volatile uint32_t* sw_set_mac_rx_busy, volatile uint32_t* sw_set_vcs_idle, volatile uint32_t* sw_set_vcs_busy, volatile uint32_t* sw_set_pcs_align_idle, volatile uint32_t* sw_set_pcs_align_busy, volatile uint32_t* sw_set_pcs_sec_idle, volatile uint32_t* sw_set_pcs_sec_busy, volatile uint32_t* sw_set_pcs_pri_idle, volatile uint32_t* sw_set_pcs_pri_busy)
{
	uint32_t tmp;

	tmp = mac_cs_ctrl_get();
	*sw_set_rx_wait_tx_resp_idle = (tmp & ((uint32_t)SW_SET_RX_WAIT_TX_RESP_IDLE_MASK)) >> SW_SET_RX_WAIT_TX_RESP_IDLE_OFFSET;
	*sw_set_rx_wait_tx_resp_busy = (tmp & ((uint32_t)SW_SET_RX_WAIT_TX_RESP_BUSY_MASK)) >> SW_SET_RX_WAIT_TX_RESP_BUSY_OFFSET;
	*sw_set_mac_tx_idle = (tmp & ((uint32_t)SW_SET_MAC_TX_IDLE_MASK)) >> SW_SET_MAC_TX_IDLE_OFFSET;
	*sw_set_mac_tx_busy = (tmp & ((uint32_t)SW_SET_MAC_TX_BUSY_MASK)) >> SW_SET_MAC_TX_BUSY_OFFSET;
	*sw_set_mac_rx_idle = (tmp & ((uint32_t)SW_SET_MAC_RX_IDLE_MASK)) >> SW_SET_MAC_RX_IDLE_OFFSET;
	*sw_set_mac_rx_busy = (tmp & ((uint32_t)SW_SET_MAC_RX_BUSY_MASK)) >> SW_SET_MAC_RX_BUSY_OFFSET;
	*sw_set_vcs_idle = (tmp & ((uint32_t)SW_SET_VCS_IDLE_MASK)) >> SW_SET_VCS_IDLE_OFFSET;
	*sw_set_vcs_busy = (tmp & ((uint32_t)SW_SET_VCS_BUSY_MASK)) >> SW_SET_VCS_BUSY_OFFSET;
	*sw_set_pcs_align_idle = (tmp & ((uint32_t)SW_SET_PCS_ALIGN_IDLE_MASK)) >> SW_SET_PCS_ALIGN_IDLE_OFFSET;
	*sw_set_pcs_align_busy = (tmp & ((uint32_t)SW_SET_PCS_ALIGN_BUSY_MASK)) >> SW_SET_PCS_ALIGN_BUSY_OFFSET;
	*sw_set_pcs_sec_idle = (tmp & ((uint32_t)SW_SET_PCS_SEC_IDLE_MASK)) >> SW_SET_PCS_SEC_IDLE_OFFSET;
	*sw_set_pcs_sec_busy = (tmp & ((uint32_t)SW_SET_PCS_SEC_BUSY_MASK)) >> SW_SET_PCS_SEC_BUSY_OFFSET;
	*sw_set_pcs_pri_idle = (tmp & ((uint32_t)SW_SET_PCS_PRI_IDLE_MASK)) >> SW_SET_PCS_PRI_IDLE_OFFSET;
	*sw_set_pcs_pri_busy = (tmp & ((uint32_t)SW_SET_PCS_PRI_BUSY_MASK)) >> SW_SET_PCS_PRI_BUSY_OFFSET;
}
static __inline void mac_ppdu_time_cal_ctrl_sw_ppdu_symb_num_ceil_para_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR);
	tmp &= ~SW_PPDU_SYMB_NUM_CEIL_PARA_MASK;
	tmp |= (value << SW_PPDU_SYMB_NUM_CEIL_PARA_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_ppdu_time_cal_ctrl_sw_ppdu_symb_num_ceil_para_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR)) & SW_PPDU_SYMB_NUM_CEIL_PARA_MASK) >> SW_PPDU_SYMB_NUM_CEIL_PARA_OFFSET;
}
static __inline void mac_ppdu_time_cal_ctrl_sw_ppdu_time_cal_ceil_para_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR);
	tmp &= ~SW_PPDU_TIME_CAL_CEIL_PARA_MASK;
	tmp |= (value << SW_PPDU_TIME_CAL_CEIL_PARA_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_ppdu_time_cal_ctrl_sw_ppdu_time_cal_ceil_para_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR)) & SW_PPDU_TIME_CAL_CEIL_PARA_MASK) >> SW_PPDU_TIME_CAL_CEIL_PARA_OFFSET;
}
static __inline void mac_ppdu_time_cal_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR), value);
}
static __inline uint32_t mac_ppdu_time_cal_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR));
}
static __inline void mac_ppdu_time_cal_ctrl_pack(uint32_t sw_ppdu_symb_num_ceil_para, uint32_t sw_ppdu_time_cal_ceil_para)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ppdu_symb_num_ceil_para << SW_PPDU_SYMB_NUM_CEIL_PARA_OFFSET) & ~((uint32_t)SW_PPDU_SYMB_NUM_CEIL_PARA_MASK)) == 0);
	assert((((uint32_t)sw_ppdu_time_cal_ceil_para << SW_PPDU_TIME_CAL_CEIL_PARA_OFFSET) & ~((uint32_t)SW_PPDU_TIME_CAL_CEIL_PARA_MASK)) == 0);
	tmp =
		((uint32_t)sw_ppdu_symb_num_ceil_para << SW_PPDU_SYMB_NUM_CEIL_PARA_OFFSET) |
		((uint32_t)sw_ppdu_time_cal_ceil_para << SW_PPDU_TIME_CAL_CEIL_PARA_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_PPDU_TIME_CAL_CTRL_ADDR), tmp);
}
static __inline void mac_ppdu_time_cal_ctrl_unpack(volatile uint32_t* sw_ppdu_symb_num_ceil_para, volatile uint32_t* sw_ppdu_time_cal_ceil_para)
{
	uint32_t tmp;

	tmp = mac_ppdu_time_cal_ctrl_get();
	*sw_ppdu_symb_num_ceil_para = (tmp & ((uint32_t)SW_PPDU_SYMB_NUM_CEIL_PARA_MASK)) >> SW_PPDU_SYMB_NUM_CEIL_PARA_OFFSET;
	*sw_ppdu_time_cal_ceil_para = (tmp & ((uint32_t)SW_PPDU_TIME_CAL_CEIL_PARA_MASK)) >> SW_PPDU_TIME_CAL_CEIL_PARA_OFFSET;
}
static __inline void mac_crypt_ctrl_sw_crypt_start_clr_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR);
	tmp &= ~SW_CRYPT_START_CLR_EN_MASK;
	tmp |= (value << SW_CRYPT_START_CLR_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_crypt_ctrl_sw_crypt_start_clr_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR)) & SW_CRYPT_START_CLR_EN_MASK) >> SW_CRYPT_START_CLR_EN_OFFSET;
}
static __inline void mac_crypt_ctrl_sw_crypt_aad1_using_qc_all_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR);
	tmp &= ~SW_CRYPT_AAD1_USING_QC_ALL_MASK;
	tmp |= (value << SW_CRYPT_AAD1_USING_QC_ALL_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_crypt_ctrl_sw_crypt_aad1_using_qc_all_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR)) & SW_CRYPT_AAD1_USING_QC_ALL_MASK) >> SW_CRYPT_AAD1_USING_QC_ALL_OFFSET;
}
static __inline void mac_crypt_ctrl_sw_crypt_aad1_qc_amsdu_bit_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR);
	tmp &= ~SW_CRYPT_AAD1_QC_AMSDU_BIT_MASK;
	tmp |= (value << SW_CRYPT_AAD1_QC_AMSDU_BIT_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_crypt_ctrl_sw_crypt_aad1_qc_amsdu_bit_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR)) & SW_CRYPT_AAD1_QC_AMSDU_BIT_MASK) >> SW_CRYPT_AAD1_QC_AMSDU_BIT_OFFSET;
}
static __inline void mac_crypt_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR), value);
}
static __inline uint32_t mac_crypt_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR));
}
static __inline void mac_crypt_ctrl_pack(uint32_t sw_crypt_start_clr_en, uint32_t sw_crypt_aad1_using_qc_all, uint32_t sw_crypt_aad1_qc_amsdu_bit)
{
	uint32_t tmp;

	assert((((uint32_t)sw_crypt_start_clr_en << SW_CRYPT_START_CLR_EN_OFFSET) & ~((uint32_t)SW_CRYPT_START_CLR_EN_MASK)) == 0);
	assert((((uint32_t)sw_crypt_aad1_using_qc_all << SW_CRYPT_AAD1_USING_QC_ALL_OFFSET) & ~((uint32_t)SW_CRYPT_AAD1_USING_QC_ALL_MASK)) == 0);
	assert((((uint32_t)sw_crypt_aad1_qc_amsdu_bit << SW_CRYPT_AAD1_QC_AMSDU_BIT_OFFSET) & ~((uint32_t)SW_CRYPT_AAD1_QC_AMSDU_BIT_MASK)) == 0);
	tmp =
		((uint32_t)sw_crypt_start_clr_en << SW_CRYPT_START_CLR_EN_OFFSET) |
		((uint32_t)sw_crypt_aad1_using_qc_all << SW_CRYPT_AAD1_USING_QC_ALL_OFFSET) |
		((uint32_t)sw_crypt_aad1_qc_amsdu_bit << SW_CRYPT_AAD1_QC_AMSDU_BIT_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_CTRL_ADDR), tmp);
}
static __inline void mac_crypt_ctrl_unpack(volatile uint32_t* sw_crypt_start_clr_en, volatile uint32_t* sw_crypt_aad1_using_qc_all, volatile uint32_t* sw_crypt_aad1_qc_amsdu_bit)
{
	uint32_t tmp;

	tmp = mac_crypt_ctrl_get();
	*sw_crypt_start_clr_en = (tmp & ((uint32_t)SW_CRYPT_START_CLR_EN_MASK)) >> SW_CRYPT_START_CLR_EN_OFFSET;
	*sw_crypt_aad1_using_qc_all = (tmp & ((uint32_t)SW_CRYPT_AAD1_USING_QC_ALL_MASK)) >> SW_CRYPT_AAD1_USING_QC_ALL_OFFSET;
	*sw_crypt_aad1_qc_amsdu_bit = (tmp & ((uint32_t)SW_CRYPT_AAD1_QC_AMSDU_BIT_MASK)) >> SW_CRYPT_AAD1_QC_AMSDU_BIT_OFFSET;
}
static __inline void mac_ram_force_on_sw_ram_mpi_tx_fifo_force_on_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR);
	tmp &= ~SW_RAM_MPI_TX_FIFO_FORCE_ON_MASK;
	tmp |= (value << SW_RAM_MPI_TX_FIFO_FORCE_ON_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR), tmp);
}
static __inline uint32_t mac_ram_force_on_sw_ram_mpi_tx_fifo_force_on_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR)) & SW_RAM_MPI_TX_FIFO_FORCE_ON_MASK) >> SW_RAM_MPI_TX_FIFO_FORCE_ON_OFFSET;
}
static __inline void mac_ram_force_on_sw_ram_rc4_sbox_force_on_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR);
	tmp &= ~SW_RAM_RC4_SBOX_FORCE_ON_MASK;
	tmp |= (value << SW_RAM_RC4_SBOX_FORCE_ON_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR), tmp);
}
static __inline uint32_t mac_ram_force_on_sw_ram_rc4_sbox_force_on_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR)) & SW_RAM_RC4_SBOX_FORCE_ON_MASK) >> SW_RAM_RC4_SBOX_FORCE_ON_OFFSET;
}
static __inline void mac_ram_force_on_sw_ram_node_table_force_on_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR);
	tmp &= ~SW_RAM_NODE_TABLE_FORCE_ON_MASK;
	tmp |= (value << SW_RAM_NODE_TABLE_FORCE_ON_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR), tmp);
}
static __inline uint32_t mac_ram_force_on_sw_ram_node_table_force_on_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR)) & SW_RAM_NODE_TABLE_FORCE_ON_MASK) >> SW_RAM_NODE_TABLE_FORCE_ON_OFFSET;
}
static __inline void mac_ram_force_on_sw_ram_mpi_rx_fifo_force_on_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR);
	tmp &= ~SW_RAM_MPI_RX_FIFO_FORCE_ON_MASK;
	tmp |= (value << SW_RAM_MPI_RX_FIFO_FORCE_ON_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR), tmp);
}
static __inline uint32_t mac_ram_force_on_sw_ram_mpi_rx_fifo_force_on_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR)) & SW_RAM_MPI_RX_FIFO_FORCE_ON_MASK) >> SW_RAM_MPI_RX_FIFO_FORCE_ON_OFFSET;
}
static __inline void mac_ram_force_on_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR), value);
}
static __inline uint32_t mac_ram_force_on_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR));
}
static __inline void mac_ram_force_on_pack(uint32_t sw_ram_mpi_tx_fifo_force_on, uint32_t sw_ram_rc4_sbox_force_on, uint32_t sw_ram_node_table_force_on, uint32_t sw_ram_mpi_rx_fifo_force_on)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ram_mpi_tx_fifo_force_on << SW_RAM_MPI_TX_FIFO_FORCE_ON_OFFSET) & ~((uint32_t)SW_RAM_MPI_TX_FIFO_FORCE_ON_MASK)) == 0);
	assert((((uint32_t)sw_ram_rc4_sbox_force_on << SW_RAM_RC4_SBOX_FORCE_ON_OFFSET) & ~((uint32_t)SW_RAM_RC4_SBOX_FORCE_ON_MASK)) == 0);
	assert((((uint32_t)sw_ram_node_table_force_on << SW_RAM_NODE_TABLE_FORCE_ON_OFFSET) & ~((uint32_t)SW_RAM_NODE_TABLE_FORCE_ON_MASK)) == 0);
	assert((((uint32_t)sw_ram_mpi_rx_fifo_force_on << SW_RAM_MPI_RX_FIFO_FORCE_ON_OFFSET) & ~((uint32_t)SW_RAM_MPI_RX_FIFO_FORCE_ON_MASK)) == 0);
	tmp =
		((uint32_t)sw_ram_mpi_tx_fifo_force_on << SW_RAM_MPI_TX_FIFO_FORCE_ON_OFFSET) |
		((uint32_t)sw_ram_rc4_sbox_force_on << SW_RAM_RC4_SBOX_FORCE_ON_OFFSET) |
		((uint32_t)sw_ram_node_table_force_on << SW_RAM_NODE_TABLE_FORCE_ON_OFFSET) |
		((uint32_t)sw_ram_mpi_rx_fifo_force_on << SW_RAM_MPI_RX_FIFO_FORCE_ON_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RAM_FORCE_ON_ADDR), tmp);
}
static __inline void mac_ram_force_on_unpack(volatile uint32_t* sw_ram_mpi_tx_fifo_force_on, volatile uint32_t* sw_ram_rc4_sbox_force_on, volatile uint32_t* sw_ram_node_table_force_on, volatile uint32_t* sw_ram_mpi_rx_fifo_force_on)
{
	uint32_t tmp;

	tmp = mac_ram_force_on_get();
	*sw_ram_mpi_tx_fifo_force_on = (tmp & ((uint32_t)SW_RAM_MPI_TX_FIFO_FORCE_ON_MASK)) >> SW_RAM_MPI_TX_FIFO_FORCE_ON_OFFSET;
	*sw_ram_rc4_sbox_force_on = (tmp & ((uint32_t)SW_RAM_RC4_SBOX_FORCE_ON_MASK)) >> SW_RAM_RC4_SBOX_FORCE_ON_OFFSET;
	*sw_ram_node_table_force_on = (tmp & ((uint32_t)SW_RAM_NODE_TABLE_FORCE_ON_MASK)) >> SW_RAM_NODE_TABLE_FORCE_ON_OFFSET;
	*sw_ram_mpi_rx_fifo_force_on = (tmp & ((uint32_t)SW_RAM_MPI_RX_FIFO_FORCE_ON_MASK)) >> SW_RAM_MPI_RX_FIFO_FORCE_ON_OFFSET;
}
static __inline void mac_top_dbg_ctrl_sw_mac_top_dbg_general_clr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_CTRL_ADDR);
	tmp &= ~SW_MAC_TOP_DBG_GENERAL_CLR_MASK;
	tmp |= (value << SW_MAC_TOP_DBG_GENERAL_CLR_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_CTRL_ADDR), tmp);
}
static __inline void mac_top_dbg_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_CTRL_ADDR), value);
}
static __inline uint32_t mac_top_dbg_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_CTRL_ADDR));
}
static __inline void mac_top_dbg_ctrl_pack(uint32_t sw_mac_top_dbg_general_clr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_top_dbg_general_clr << SW_MAC_TOP_DBG_GENERAL_CLR_OFFSET) & ~((uint32_t)SW_MAC_TOP_DBG_GENERAL_CLR_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_top_dbg_general_clr << SW_MAC_TOP_DBG_GENERAL_CLR_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_top_dbg_sts_0_sw_ro_txr_overlap_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_STS_0_ADDR)) & SW_RO_TXR_OVERLAP_CNT_MASK) >> SW_RO_TXR_OVERLAP_CNT_OFFSET;
}
static __inline uint32_t mac_top_dbg_sts_0_sw_ro_trx_start_same_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_STS_0_ADDR)) & SW_RO_TRX_START_SAME_CNT_MASK) >> SW_RO_TRX_START_SAME_CNT_OFFSET;
}
static __inline uint32_t mac_top_dbg_sts_0_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_STS_0_ADDR));
}
static __inline void mac_top_dbg_sts_0_unpack(volatile uint32_t* sw_ro_txr_overlap_cnt, volatile uint32_t* sw_ro_trx_start_same_cnt)
{
	uint32_t tmp;

	tmp = mac_top_dbg_sts_0_get();
	*sw_ro_txr_overlap_cnt = (tmp & ((uint32_t)SW_RO_TXR_OVERLAP_CNT_MASK)) >> SW_RO_TXR_OVERLAP_CNT_OFFSET;
	*sw_ro_trx_start_same_cnt = (tmp & ((uint32_t)SW_RO_TRX_START_SAME_CNT_MASK)) >> SW_RO_TRX_START_SAME_CNT_OFFSET;
}
static __inline uint32_t mac_top_dbg_sts_1_sw_ro_ahb_mst_sync_dbg_0_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_STS_1_ADDR)) & SW_RO_AHB_MST_SYNC_DBG_0_MASK) >> SW_RO_AHB_MST_SYNC_DBG_0_OFFSET;
}
static __inline uint32_t mac_top_dbg_sts_1_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_STS_1_ADDR));
}
static __inline void mac_top_dbg_sts_1_unpack(volatile uint32_t* sw_ro_ahb_mst_sync_dbg_0)
{
	uint32_t tmp;

	tmp = mac_top_dbg_sts_1_get();
	*sw_ro_ahb_mst_sync_dbg_0 = (tmp & ((uint32_t)SW_RO_AHB_MST_SYNC_DBG_0_MASK)) >> SW_RO_AHB_MST_SYNC_DBG_0_OFFSET;
}
static __inline uint32_t mac_top_dbg_sts_2_sw_ro_ahb_mst_sync_dbg_1_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_STS_2_ADDR)) & SW_RO_AHB_MST_SYNC_DBG_1_MASK) >> SW_RO_AHB_MST_SYNC_DBG_1_OFFSET;
}
static __inline uint32_t mac_top_dbg_sts_2_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_TOP_DBG_STS_2_ADDR));
}
static __inline void mac_top_dbg_sts_2_unpack(volatile uint32_t* sw_ro_ahb_mst_sync_dbg_1)
{
	uint32_t tmp;

	tmp = mac_top_dbg_sts_2_get();
	*sw_ro_ahb_mst_sync_dbg_1 = (tmp & ((uint32_t)SW_RO_AHB_MST_SYNC_DBG_1_MASK)) >> SW_RO_AHB_MST_SYNC_DBG_1_OFFSET;
}
static __inline uint32_t mac_crypt_dbg_fsm_sts_sw_ro_cs_ccmp_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_DBG_FSM_STS_ADDR)) & SW_RO_CS_CCMP_MASK) >> SW_RO_CS_CCMP_OFFSET;
}
static __inline uint32_t mac_crypt_dbg_fsm_sts_sw_ro_wep_tkip_fsm_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_DBG_FSM_STS_ADDR)) & SW_RO_WEP_TKIP_FSM_MASK) >> SW_RO_WEP_TKIP_FSM_OFFSET;
}
static __inline uint32_t mac_crypt_dbg_fsm_sts_sw_ro_cs_rc4_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_DBG_FSM_STS_ADDR)) & SW_RO_CS_RC4_MASK) >> SW_RO_CS_RC4_OFFSET;
}
static __inline uint32_t mac_crypt_dbg_fsm_sts_sw_ro_cs_rc4_core_init_and_swap_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_DBG_FSM_STS_ADDR)) & SW_RO_CS_RC4_CORE_INIT_AND_SWAP_MASK) >> SW_RO_CS_RC4_CORE_INIT_AND_SWAP_OFFSET;
}
static __inline uint32_t mac_crypt_dbg_fsm_sts_sw_ro_cs_rc4_process_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_DBG_FSM_STS_ADDR)) & SW_RO_CS_RC4_PROCESS_MASK) >> SW_RO_CS_RC4_PROCESS_OFFSET;
}
static __inline uint32_t mac_crypt_dbg_fsm_sts_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CRYPT_DBG_FSM_STS_ADDR));
}
static __inline void mac_crypt_dbg_fsm_sts_unpack(volatile uint32_t* sw_ro_cs_ccmp, volatile uint32_t* sw_ro_wep_tkip_fsm, volatile uint32_t* sw_ro_cs_rc4, volatile uint32_t* sw_ro_cs_rc4_core_init_and_swap, volatile uint32_t* sw_ro_cs_rc4_process)
{
	uint32_t tmp;

	tmp = mac_crypt_dbg_fsm_sts_get();
	*sw_ro_cs_ccmp = (tmp & ((uint32_t)SW_RO_CS_CCMP_MASK)) >> SW_RO_CS_CCMP_OFFSET;
	*sw_ro_wep_tkip_fsm = (tmp & ((uint32_t)SW_RO_WEP_TKIP_FSM_MASK)) >> SW_RO_WEP_TKIP_FSM_OFFSET;
	*sw_ro_cs_rc4 = (tmp & ((uint32_t)SW_RO_CS_RC4_MASK)) >> SW_RO_CS_RC4_OFFSET;
	*sw_ro_cs_rc4_core_init_and_swap = (tmp & ((uint32_t)SW_RO_CS_RC4_CORE_INIT_AND_SWAP_MASK)) >> SW_RO_CS_RC4_CORE_INIT_AND_SWAP_OFFSET;
	*sw_ro_cs_rc4_process = (tmp & ((uint32_t)SW_RO_CS_RC4_PROCESS_MASK)) >> SW_RO_CS_RC4_PROCESS_OFFSET;
}
static __inline void mac_cca_stat_ctrl_sw_mac_cca_stat_clr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_STAT_CTRL_ADDR);
	tmp &= ~SW_MAC_CCA_STAT_CLR_MASK;
	tmp |= (value << SW_MAC_CCA_STAT_CLR_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CCA_STAT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CCA_STAT_CTRL_ADDR), tmp);
}
static __inline void mac_cca_stat_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CCA_STAT_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CCA_STAT_CTRL_ADDR), value);
}
static __inline uint32_t mac_cca_stat_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_STAT_CTRL_ADDR));
}
static __inline void mac_cca_stat_ctrl_pack(uint32_t sw_mac_cca_stat_clr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_cca_stat_clr << SW_MAC_CCA_STAT_CLR_OFFSET) & ~((uint32_t)SW_MAC_CCA_STAT_CLR_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_cca_stat_clr << SW_MAC_CCA_STAT_CLR_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_CCA_STAT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_CCA_STAT_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_cca_tx_ing_stat_sw_ro_mac_cca_tx_ing_stat_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_TX_ING_STAT_ADDR)) & SW_RO_MAC_CCA_TX_ING_STAT_MASK) >> SW_RO_MAC_CCA_TX_ING_STAT_OFFSET;
}
static __inline uint32_t mac_cca_tx_ing_stat_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_TX_ING_STAT_ADDR));
}
static __inline void mac_cca_tx_ing_stat_unpack(volatile uint32_t* sw_ro_mac_cca_tx_ing_stat)
{
	uint32_t tmp;

	tmp = mac_cca_tx_ing_stat_get();
	*sw_ro_mac_cca_tx_ing_stat = (tmp & ((uint32_t)SW_RO_MAC_CCA_TX_ING_STAT_MASK)) >> SW_RO_MAC_CCA_TX_ING_STAT_OFFSET;
}
static __inline uint32_t mac_cca_busy_rx_ing_stat_sw_ro_mac_cca_busy_rx_ing_stat_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_BUSY_RX_ING_STAT_ADDR)) & SW_RO_MAC_CCA_BUSY_RX_ING_STAT_MASK) >> SW_RO_MAC_CCA_BUSY_RX_ING_STAT_OFFSET;
}
static __inline uint32_t mac_cca_busy_rx_ing_stat_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_BUSY_RX_ING_STAT_ADDR));
}
static __inline void mac_cca_busy_rx_ing_stat_unpack(volatile uint32_t* sw_ro_mac_cca_busy_rx_ing_stat)
{
	uint32_t tmp;

	tmp = mac_cca_busy_rx_ing_stat_get();
	*sw_ro_mac_cca_busy_rx_ing_stat = (tmp & ((uint32_t)SW_RO_MAC_CCA_BUSY_RX_ING_STAT_MASK)) >> SW_RO_MAC_CCA_BUSY_RX_ING_STAT_OFFSET;
}
static __inline uint32_t mac_cca_idle_rx_ing_stat_sw_ro_mac_cca_idle_rx_ing_stat_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_IDLE_RX_ING_STAT_ADDR)) & SW_RO_MAC_CCA_IDLE_RX_ING_STAT_MASK) >> SW_RO_MAC_CCA_IDLE_RX_ING_STAT_OFFSET;
}
static __inline uint32_t mac_cca_idle_rx_ing_stat_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_IDLE_RX_ING_STAT_ADDR));
}
static __inline void mac_cca_idle_rx_ing_stat_unpack(volatile uint32_t* sw_ro_mac_cca_idle_rx_ing_stat)
{
	uint32_t tmp;

	tmp = mac_cca_idle_rx_ing_stat_get();
	*sw_ro_mac_cca_idle_rx_ing_stat = (tmp & ((uint32_t)SW_RO_MAC_CCA_IDLE_RX_ING_STAT_MASK)) >> SW_RO_MAC_CCA_IDLE_RX_ING_STAT_OFFSET;
}
static __inline uint32_t mac_cca_busy_rx_listen_stat_sw_ro_mac_cca_busy_rx_listen_stat_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_BUSY_RX_LISTEN_STAT_ADDR)) & SW_RO_MAC_CCA_BUSY_RX_LISTEN_STAT_MASK) >> SW_RO_MAC_CCA_BUSY_RX_LISTEN_STAT_OFFSET;
}
static __inline uint32_t mac_cca_busy_rx_listen_stat_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_BUSY_RX_LISTEN_STAT_ADDR));
}
static __inline void mac_cca_busy_rx_listen_stat_unpack(volatile uint32_t* sw_ro_mac_cca_busy_rx_listen_stat)
{
	uint32_t tmp;

	tmp = mac_cca_busy_rx_listen_stat_get();
	*sw_ro_mac_cca_busy_rx_listen_stat = (tmp & ((uint32_t)SW_RO_MAC_CCA_BUSY_RX_LISTEN_STAT_MASK)) >> SW_RO_MAC_CCA_BUSY_RX_LISTEN_STAT_OFFSET;
}
static __inline uint32_t mac_cca_idle_rx_listen_stat_sw_ro_mac_cca_idle_rx_listen_stat_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_IDLE_RX_LISTEN_STAT_ADDR)) & SW_RO_MAC_CCA_IDLE_RX_LISTEN_STAT_MASK) >> SW_RO_MAC_CCA_IDLE_RX_LISTEN_STAT_OFFSET;
}
static __inline uint32_t mac_cca_idle_rx_listen_stat_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_CCA_IDLE_RX_LISTEN_STAT_ADDR));
}
static __inline void mac_cca_idle_rx_listen_stat_unpack(volatile uint32_t* sw_ro_mac_cca_idle_rx_listen_stat)
{
	uint32_t tmp;

	tmp = mac_cca_idle_rx_listen_stat_get();
	*sw_ro_mac_cca_idle_rx_listen_stat = (tmp & ((uint32_t)SW_RO_MAC_CCA_IDLE_RX_LISTEN_STAT_MASK)) >> SW_RO_MAC_CCA_IDLE_RX_LISTEN_STAT_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_0_sw_dsss_long_1m_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR);
	tmp &= ~SW_DSSS_LONG_1M_MASK;
	tmp |= (value << SW_DSSS_LONG_1M_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_0_sw_dsss_long_1m_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR)) & SW_DSSS_LONG_1M_MASK) >> SW_DSSS_LONG_1M_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_0_sw_dsss_long_2m_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR);
	tmp &= ~SW_DSSS_LONG_2M_MASK;
	tmp |= (value << SW_DSSS_LONG_2M_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_0_sw_dsss_long_2m_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR)) & SW_DSSS_LONG_2M_MASK) >> SW_DSSS_LONG_2M_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_0_sw_dsss_long_5p5m_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR);
	tmp &= ~SW_DSSS_LONG_5P5M_MASK;
	tmp |= (value << SW_DSSS_LONG_5P5M_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_0_sw_dsss_long_5p5m_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR)) & SW_DSSS_LONG_5P5M_MASK) >> SW_DSSS_LONG_5P5M_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_0_sw_dsss_long_11m_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR);
	tmp &= ~SW_DSSS_LONG_11M_MASK;
	tmp |= (value << SW_DSSS_LONG_11M_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_0_sw_dsss_long_11m_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR)) & SW_DSSS_LONG_11M_MASK) >> SW_DSSS_LONG_11M_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_0_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR), value);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_0_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR));
}
static __inline void mac_rx_resp_rate_ctrl_0_pack(uint32_t sw_dsss_long_1m, uint32_t sw_dsss_long_2m, uint32_t sw_dsss_long_5p5m, uint32_t sw_dsss_long_11m)
{
	uint32_t tmp;

	assert((((uint32_t)sw_dsss_long_1m << SW_DSSS_LONG_1M_OFFSET) & ~((uint32_t)SW_DSSS_LONG_1M_MASK)) == 0);
	assert((((uint32_t)sw_dsss_long_2m << SW_DSSS_LONG_2M_OFFSET) & ~((uint32_t)SW_DSSS_LONG_2M_MASK)) == 0);
	assert((((uint32_t)sw_dsss_long_5p5m << SW_DSSS_LONG_5P5M_OFFSET) & ~((uint32_t)SW_DSSS_LONG_5P5M_MASK)) == 0);
	assert((((uint32_t)sw_dsss_long_11m << SW_DSSS_LONG_11M_OFFSET) & ~((uint32_t)SW_DSSS_LONG_11M_MASK)) == 0);
	tmp =
		((uint32_t)sw_dsss_long_1m << SW_DSSS_LONG_1M_OFFSET) |
		((uint32_t)sw_dsss_long_2m << SW_DSSS_LONG_2M_OFFSET) |
		((uint32_t)sw_dsss_long_5p5m << SW_DSSS_LONG_5P5M_OFFSET) |
		((uint32_t)sw_dsss_long_11m << SW_DSSS_LONG_11M_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_0_ADDR), tmp);
}
static __inline void mac_rx_resp_rate_ctrl_0_unpack(volatile uint32_t* sw_dsss_long_1m, volatile uint32_t* sw_dsss_long_2m, volatile uint32_t* sw_dsss_long_5p5m, volatile uint32_t* sw_dsss_long_11m)
{
	uint32_t tmp;

	tmp = mac_rx_resp_rate_ctrl_0_get();
	*sw_dsss_long_1m = (tmp & ((uint32_t)SW_DSSS_LONG_1M_MASK)) >> SW_DSSS_LONG_1M_OFFSET;
	*sw_dsss_long_2m = (tmp & ((uint32_t)SW_DSSS_LONG_2M_MASK)) >> SW_DSSS_LONG_2M_OFFSET;
	*sw_dsss_long_5p5m = (tmp & ((uint32_t)SW_DSSS_LONG_5P5M_MASK)) >> SW_DSSS_LONG_5P5M_OFFSET;
	*sw_dsss_long_11m = (tmp & ((uint32_t)SW_DSSS_LONG_11M_MASK)) >> SW_DSSS_LONG_11M_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_1_sw_dsss_short_2m_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR);
	tmp &= ~SW_DSSS_SHORT_2M_MASK;
	tmp |= (value << SW_DSSS_SHORT_2M_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_1_sw_dsss_short_2m_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR)) & SW_DSSS_SHORT_2M_MASK) >> SW_DSSS_SHORT_2M_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_1_sw_dsss_short_5p5m_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR);
	tmp &= ~SW_DSSS_SHORT_5P5M_MASK;
	tmp |= (value << SW_DSSS_SHORT_5P5M_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_1_sw_dsss_short_5p5m_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR)) & SW_DSSS_SHORT_5P5M_MASK) >> SW_DSSS_SHORT_5P5M_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_1_sw_dsss_short_11m_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR);
	tmp &= ~SW_DSSS_SHORT_11M_MASK;
	tmp |= (value << SW_DSSS_SHORT_11M_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_1_sw_dsss_short_11m_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR)) & SW_DSSS_SHORT_11M_MASK) >> SW_DSSS_SHORT_11M_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_1_sw_ofdm_non_rate_0_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR);
	tmp &= ~SW_OFDM_NON_RATE_0_MASK;
	tmp |= (value << SW_OFDM_NON_RATE_0_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_1_sw_ofdm_non_rate_0_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR)) & SW_OFDM_NON_RATE_0_MASK) >> SW_OFDM_NON_RATE_0_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_1_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR), value);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_1_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR));
}
static __inline void mac_rx_resp_rate_ctrl_1_pack(uint32_t sw_dsss_short_2m, uint32_t sw_dsss_short_5p5m, uint32_t sw_dsss_short_11m, uint32_t sw_ofdm_non_rate_0)
{
	uint32_t tmp;

	assert((((uint32_t)sw_dsss_short_2m << SW_DSSS_SHORT_2M_OFFSET) & ~((uint32_t)SW_DSSS_SHORT_2M_MASK)) == 0);
	assert((((uint32_t)sw_dsss_short_5p5m << SW_DSSS_SHORT_5P5M_OFFSET) & ~((uint32_t)SW_DSSS_SHORT_5P5M_MASK)) == 0);
	assert((((uint32_t)sw_dsss_short_11m << SW_DSSS_SHORT_11M_OFFSET) & ~((uint32_t)SW_DSSS_SHORT_11M_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_non_rate_0 << SW_OFDM_NON_RATE_0_OFFSET) & ~((uint32_t)SW_OFDM_NON_RATE_0_MASK)) == 0);
	tmp =
		((uint32_t)sw_dsss_short_2m << SW_DSSS_SHORT_2M_OFFSET) |
		((uint32_t)sw_dsss_short_5p5m << SW_DSSS_SHORT_5P5M_OFFSET) |
		((uint32_t)sw_dsss_short_11m << SW_DSSS_SHORT_11M_OFFSET) |
		((uint32_t)sw_ofdm_non_rate_0 << SW_OFDM_NON_RATE_0_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_1_ADDR), tmp);
}
static __inline void mac_rx_resp_rate_ctrl_1_unpack(volatile uint32_t* sw_dsss_short_2m, volatile uint32_t* sw_dsss_short_5p5m, volatile uint32_t* sw_dsss_short_11m, volatile uint32_t* sw_ofdm_non_rate_0)
{
	uint32_t tmp;

	tmp = mac_rx_resp_rate_ctrl_1_get();
	*sw_dsss_short_2m = (tmp & ((uint32_t)SW_DSSS_SHORT_2M_MASK)) >> SW_DSSS_SHORT_2M_OFFSET;
	*sw_dsss_short_5p5m = (tmp & ((uint32_t)SW_DSSS_SHORT_5P5M_MASK)) >> SW_DSSS_SHORT_5P5M_OFFSET;
	*sw_dsss_short_11m = (tmp & ((uint32_t)SW_DSSS_SHORT_11M_MASK)) >> SW_DSSS_SHORT_11M_OFFSET;
	*sw_ofdm_non_rate_0 = (tmp & ((uint32_t)SW_OFDM_NON_RATE_0_MASK)) >> SW_OFDM_NON_RATE_0_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_2_sw_ofdm_non_rate_1_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR);
	tmp &= ~SW_OFDM_NON_RATE_1_MASK;
	tmp |= (value << SW_OFDM_NON_RATE_1_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_2_sw_ofdm_non_rate_1_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR)) & SW_OFDM_NON_RATE_1_MASK) >> SW_OFDM_NON_RATE_1_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_2_sw_ofdm_non_rate_2_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR);
	tmp &= ~SW_OFDM_NON_RATE_2_MASK;
	tmp |= (value << SW_OFDM_NON_RATE_2_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_2_sw_ofdm_non_rate_2_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR)) & SW_OFDM_NON_RATE_2_MASK) >> SW_OFDM_NON_RATE_2_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_2_sw_ofdm_non_rate_3_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR);
	tmp &= ~SW_OFDM_NON_RATE_3_MASK;
	tmp |= (value << SW_OFDM_NON_RATE_3_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_2_sw_ofdm_non_rate_3_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR)) & SW_OFDM_NON_RATE_3_MASK) >> SW_OFDM_NON_RATE_3_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_2_sw_ofdm_non_rate_4_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR);
	tmp &= ~SW_OFDM_NON_RATE_4_MASK;
	tmp |= (value << SW_OFDM_NON_RATE_4_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_2_sw_ofdm_non_rate_4_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR)) & SW_OFDM_NON_RATE_4_MASK) >> SW_OFDM_NON_RATE_4_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_2_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR), value);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_2_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR));
}
static __inline void mac_rx_resp_rate_ctrl_2_pack(uint32_t sw_ofdm_non_rate_1, uint32_t sw_ofdm_non_rate_2, uint32_t sw_ofdm_non_rate_3, uint32_t sw_ofdm_non_rate_4)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ofdm_non_rate_1 << SW_OFDM_NON_RATE_1_OFFSET) & ~((uint32_t)SW_OFDM_NON_RATE_1_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_non_rate_2 << SW_OFDM_NON_RATE_2_OFFSET) & ~((uint32_t)SW_OFDM_NON_RATE_2_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_non_rate_3 << SW_OFDM_NON_RATE_3_OFFSET) & ~((uint32_t)SW_OFDM_NON_RATE_3_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_non_rate_4 << SW_OFDM_NON_RATE_4_OFFSET) & ~((uint32_t)SW_OFDM_NON_RATE_4_MASK)) == 0);
	tmp =
		((uint32_t)sw_ofdm_non_rate_1 << SW_OFDM_NON_RATE_1_OFFSET) |
		((uint32_t)sw_ofdm_non_rate_2 << SW_OFDM_NON_RATE_2_OFFSET) |
		((uint32_t)sw_ofdm_non_rate_3 << SW_OFDM_NON_RATE_3_OFFSET) |
		((uint32_t)sw_ofdm_non_rate_4 << SW_OFDM_NON_RATE_4_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_2_ADDR), tmp);
}
static __inline void mac_rx_resp_rate_ctrl_2_unpack(volatile uint32_t* sw_ofdm_non_rate_1, volatile uint32_t* sw_ofdm_non_rate_2, volatile uint32_t* sw_ofdm_non_rate_3, volatile uint32_t* sw_ofdm_non_rate_4)
{
	uint32_t tmp;

	tmp = mac_rx_resp_rate_ctrl_2_get();
	*sw_ofdm_non_rate_1 = (tmp & ((uint32_t)SW_OFDM_NON_RATE_1_MASK)) >> SW_OFDM_NON_RATE_1_OFFSET;
	*sw_ofdm_non_rate_2 = (tmp & ((uint32_t)SW_OFDM_NON_RATE_2_MASK)) >> SW_OFDM_NON_RATE_2_OFFSET;
	*sw_ofdm_non_rate_3 = (tmp & ((uint32_t)SW_OFDM_NON_RATE_3_MASK)) >> SW_OFDM_NON_RATE_3_OFFSET;
	*sw_ofdm_non_rate_4 = (tmp & ((uint32_t)SW_OFDM_NON_RATE_4_MASK)) >> SW_OFDM_NON_RATE_4_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_3_sw_ofdm_non_rate_5_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR);
	tmp &= ~SW_OFDM_NON_RATE_5_MASK;
	tmp |= (value << SW_OFDM_NON_RATE_5_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_3_sw_ofdm_non_rate_5_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR)) & SW_OFDM_NON_RATE_5_MASK) >> SW_OFDM_NON_RATE_5_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_3_sw_ofdm_non_rate_6_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR);
	tmp &= ~SW_OFDM_NON_RATE_6_MASK;
	tmp |= (value << SW_OFDM_NON_RATE_6_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_3_sw_ofdm_non_rate_6_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR)) & SW_OFDM_NON_RATE_6_MASK) >> SW_OFDM_NON_RATE_6_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_3_sw_ofdm_non_rate_7_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR);
	tmp &= ~SW_OFDM_NON_RATE_7_MASK;
	tmp |= (value << SW_OFDM_NON_RATE_7_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_3_sw_ofdm_non_rate_7_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR)) & SW_OFDM_NON_RATE_7_MASK) >> SW_OFDM_NON_RATE_7_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_3_sw_ofdm_ht_20m_mcs_0_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR);
	tmp &= ~SW_OFDM_HT_20M_MCS_0_MASK;
	tmp |= (value << SW_OFDM_HT_20M_MCS_0_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_3_sw_ofdm_ht_20m_mcs_0_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR)) & SW_OFDM_HT_20M_MCS_0_MASK) >> SW_OFDM_HT_20M_MCS_0_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_3_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR), value);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_3_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR));
}
static __inline void mac_rx_resp_rate_ctrl_3_pack(uint32_t sw_ofdm_non_rate_5, uint32_t sw_ofdm_non_rate_6, uint32_t sw_ofdm_non_rate_7, uint32_t sw_ofdm_ht_20m_mcs_0)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ofdm_non_rate_5 << SW_OFDM_NON_RATE_5_OFFSET) & ~((uint32_t)SW_OFDM_NON_RATE_5_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_non_rate_6 << SW_OFDM_NON_RATE_6_OFFSET) & ~((uint32_t)SW_OFDM_NON_RATE_6_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_non_rate_7 << SW_OFDM_NON_RATE_7_OFFSET) & ~((uint32_t)SW_OFDM_NON_RATE_7_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_20m_mcs_0 << SW_OFDM_HT_20M_MCS_0_OFFSET) & ~((uint32_t)SW_OFDM_HT_20M_MCS_0_MASK)) == 0);
	tmp =
		((uint32_t)sw_ofdm_non_rate_5 << SW_OFDM_NON_RATE_5_OFFSET) |
		((uint32_t)sw_ofdm_non_rate_6 << SW_OFDM_NON_RATE_6_OFFSET) |
		((uint32_t)sw_ofdm_non_rate_7 << SW_OFDM_NON_RATE_7_OFFSET) |
		((uint32_t)sw_ofdm_ht_20m_mcs_0 << SW_OFDM_HT_20M_MCS_0_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_3_ADDR), tmp);
}
static __inline void mac_rx_resp_rate_ctrl_3_unpack(volatile uint32_t* sw_ofdm_non_rate_5, volatile uint32_t* sw_ofdm_non_rate_6, volatile uint32_t* sw_ofdm_non_rate_7, volatile uint32_t* sw_ofdm_ht_20m_mcs_0)
{
	uint32_t tmp;

	tmp = mac_rx_resp_rate_ctrl_3_get();
	*sw_ofdm_non_rate_5 = (tmp & ((uint32_t)SW_OFDM_NON_RATE_5_MASK)) >> SW_OFDM_NON_RATE_5_OFFSET;
	*sw_ofdm_non_rate_6 = (tmp & ((uint32_t)SW_OFDM_NON_RATE_6_MASK)) >> SW_OFDM_NON_RATE_6_OFFSET;
	*sw_ofdm_non_rate_7 = (tmp & ((uint32_t)SW_OFDM_NON_RATE_7_MASK)) >> SW_OFDM_NON_RATE_7_OFFSET;
	*sw_ofdm_ht_20m_mcs_0 = (tmp & ((uint32_t)SW_OFDM_HT_20M_MCS_0_MASK)) >> SW_OFDM_HT_20M_MCS_0_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_4_sw_ofdm_ht_20m_mcs_1_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR);
	tmp &= ~SW_OFDM_HT_20M_MCS_1_MASK;
	tmp |= (value << SW_OFDM_HT_20M_MCS_1_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_4_sw_ofdm_ht_20m_mcs_1_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR)) & SW_OFDM_HT_20M_MCS_1_MASK) >> SW_OFDM_HT_20M_MCS_1_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_4_sw_ofdm_ht_20m_mcs_2_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR);
	tmp &= ~SW_OFDM_HT_20M_MCS_2_MASK;
	tmp |= (value << SW_OFDM_HT_20M_MCS_2_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_4_sw_ofdm_ht_20m_mcs_2_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR)) & SW_OFDM_HT_20M_MCS_2_MASK) >> SW_OFDM_HT_20M_MCS_2_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_4_sw_ofdm_ht_20m_mcs_3_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR);
	tmp &= ~SW_OFDM_HT_20M_MCS_3_MASK;
	tmp |= (value << SW_OFDM_HT_20M_MCS_3_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_4_sw_ofdm_ht_20m_mcs_3_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR)) & SW_OFDM_HT_20M_MCS_3_MASK) >> SW_OFDM_HT_20M_MCS_3_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_4_sw_ofdm_ht_20m_mcs_4_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR);
	tmp &= ~SW_OFDM_HT_20M_MCS_4_MASK;
	tmp |= (value << SW_OFDM_HT_20M_MCS_4_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_4_sw_ofdm_ht_20m_mcs_4_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR)) & SW_OFDM_HT_20M_MCS_4_MASK) >> SW_OFDM_HT_20M_MCS_4_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_4_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR), value);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_4_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR));
}
static __inline void mac_rx_resp_rate_ctrl_4_pack(uint32_t sw_ofdm_ht_20m_mcs_1, uint32_t sw_ofdm_ht_20m_mcs_2, uint32_t sw_ofdm_ht_20m_mcs_3, uint32_t sw_ofdm_ht_20m_mcs_4)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ofdm_ht_20m_mcs_1 << SW_OFDM_HT_20M_MCS_1_OFFSET) & ~((uint32_t)SW_OFDM_HT_20M_MCS_1_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_20m_mcs_2 << SW_OFDM_HT_20M_MCS_2_OFFSET) & ~((uint32_t)SW_OFDM_HT_20M_MCS_2_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_20m_mcs_3 << SW_OFDM_HT_20M_MCS_3_OFFSET) & ~((uint32_t)SW_OFDM_HT_20M_MCS_3_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_20m_mcs_4 << SW_OFDM_HT_20M_MCS_4_OFFSET) & ~((uint32_t)SW_OFDM_HT_20M_MCS_4_MASK)) == 0);
	tmp =
		((uint32_t)sw_ofdm_ht_20m_mcs_1 << SW_OFDM_HT_20M_MCS_1_OFFSET) |
		((uint32_t)sw_ofdm_ht_20m_mcs_2 << SW_OFDM_HT_20M_MCS_2_OFFSET) |
		((uint32_t)sw_ofdm_ht_20m_mcs_3 << SW_OFDM_HT_20M_MCS_3_OFFSET) |
		((uint32_t)sw_ofdm_ht_20m_mcs_4 << SW_OFDM_HT_20M_MCS_4_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_4_ADDR), tmp);
}
static __inline void mac_rx_resp_rate_ctrl_4_unpack(volatile uint32_t* sw_ofdm_ht_20m_mcs_1, volatile uint32_t* sw_ofdm_ht_20m_mcs_2, volatile uint32_t* sw_ofdm_ht_20m_mcs_3, volatile uint32_t* sw_ofdm_ht_20m_mcs_4)
{
	uint32_t tmp;

	tmp = mac_rx_resp_rate_ctrl_4_get();
	*sw_ofdm_ht_20m_mcs_1 = (tmp & ((uint32_t)SW_OFDM_HT_20M_MCS_1_MASK)) >> SW_OFDM_HT_20M_MCS_1_OFFSET;
	*sw_ofdm_ht_20m_mcs_2 = (tmp & ((uint32_t)SW_OFDM_HT_20M_MCS_2_MASK)) >> SW_OFDM_HT_20M_MCS_2_OFFSET;
	*sw_ofdm_ht_20m_mcs_3 = (tmp & ((uint32_t)SW_OFDM_HT_20M_MCS_3_MASK)) >> SW_OFDM_HT_20M_MCS_3_OFFSET;
	*sw_ofdm_ht_20m_mcs_4 = (tmp & ((uint32_t)SW_OFDM_HT_20M_MCS_4_MASK)) >> SW_OFDM_HT_20M_MCS_4_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_5_sw_ofdm_ht_20m_mcs_5_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR);
	tmp &= ~SW_OFDM_HT_20M_MCS_5_MASK;
	tmp |= (value << SW_OFDM_HT_20M_MCS_5_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_5_sw_ofdm_ht_20m_mcs_5_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR)) & SW_OFDM_HT_20M_MCS_5_MASK) >> SW_OFDM_HT_20M_MCS_5_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_5_sw_ofdm_ht_20m_mcs_6_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR);
	tmp &= ~SW_OFDM_HT_20M_MCS_6_MASK;
	tmp |= (value << SW_OFDM_HT_20M_MCS_6_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_5_sw_ofdm_ht_20m_mcs_6_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR)) & SW_OFDM_HT_20M_MCS_6_MASK) >> SW_OFDM_HT_20M_MCS_6_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_5_sw_ofdm_ht_20m_mcs_7_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR);
	tmp &= ~SW_OFDM_HT_20M_MCS_7_MASK;
	tmp |= (value << SW_OFDM_HT_20M_MCS_7_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_5_sw_ofdm_ht_20m_mcs_7_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR)) & SW_OFDM_HT_20M_MCS_7_MASK) >> SW_OFDM_HT_20M_MCS_7_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_5_sw_ofdm_ht_40m_mcs_0_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR);
	tmp &= ~SW_OFDM_HT_40M_MCS_0_MASK;
	tmp |= (value << SW_OFDM_HT_40M_MCS_0_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_5_sw_ofdm_ht_40m_mcs_0_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR)) & SW_OFDM_HT_40M_MCS_0_MASK) >> SW_OFDM_HT_40M_MCS_0_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_5_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR), value);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_5_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR));
}
static __inline void mac_rx_resp_rate_ctrl_5_pack(uint32_t sw_ofdm_ht_20m_mcs_5, uint32_t sw_ofdm_ht_20m_mcs_6, uint32_t sw_ofdm_ht_20m_mcs_7, uint32_t sw_ofdm_ht_40m_mcs_0)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ofdm_ht_20m_mcs_5 << SW_OFDM_HT_20M_MCS_5_OFFSET) & ~((uint32_t)SW_OFDM_HT_20M_MCS_5_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_20m_mcs_6 << SW_OFDM_HT_20M_MCS_6_OFFSET) & ~((uint32_t)SW_OFDM_HT_20M_MCS_6_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_20m_mcs_7 << SW_OFDM_HT_20M_MCS_7_OFFSET) & ~((uint32_t)SW_OFDM_HT_20M_MCS_7_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_40m_mcs_0 << SW_OFDM_HT_40M_MCS_0_OFFSET) & ~((uint32_t)SW_OFDM_HT_40M_MCS_0_MASK)) == 0);
	tmp =
		((uint32_t)sw_ofdm_ht_20m_mcs_5 << SW_OFDM_HT_20M_MCS_5_OFFSET) |
		((uint32_t)sw_ofdm_ht_20m_mcs_6 << SW_OFDM_HT_20M_MCS_6_OFFSET) |
		((uint32_t)sw_ofdm_ht_20m_mcs_7 << SW_OFDM_HT_20M_MCS_7_OFFSET) |
		((uint32_t)sw_ofdm_ht_40m_mcs_0 << SW_OFDM_HT_40M_MCS_0_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_5_ADDR), tmp);
}
static __inline void mac_rx_resp_rate_ctrl_5_unpack(volatile uint32_t* sw_ofdm_ht_20m_mcs_5, volatile uint32_t* sw_ofdm_ht_20m_mcs_6, volatile uint32_t* sw_ofdm_ht_20m_mcs_7, volatile uint32_t* sw_ofdm_ht_40m_mcs_0)
{
	uint32_t tmp;

	tmp = mac_rx_resp_rate_ctrl_5_get();
	*sw_ofdm_ht_20m_mcs_5 = (tmp & ((uint32_t)SW_OFDM_HT_20M_MCS_5_MASK)) >> SW_OFDM_HT_20M_MCS_5_OFFSET;
	*sw_ofdm_ht_20m_mcs_6 = (tmp & ((uint32_t)SW_OFDM_HT_20M_MCS_6_MASK)) >> SW_OFDM_HT_20M_MCS_6_OFFSET;
	*sw_ofdm_ht_20m_mcs_7 = (tmp & ((uint32_t)SW_OFDM_HT_20M_MCS_7_MASK)) >> SW_OFDM_HT_20M_MCS_7_OFFSET;
	*sw_ofdm_ht_40m_mcs_0 = (tmp & ((uint32_t)SW_OFDM_HT_40M_MCS_0_MASK)) >> SW_OFDM_HT_40M_MCS_0_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_6_sw_ofdm_ht_40m_mcs_1_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR);
	tmp &= ~SW_OFDM_HT_40M_MCS_1_MASK;
	tmp |= (value << SW_OFDM_HT_40M_MCS_1_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_6_sw_ofdm_ht_40m_mcs_1_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR)) & SW_OFDM_HT_40M_MCS_1_MASK) >> SW_OFDM_HT_40M_MCS_1_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_6_sw_ofdm_ht_40m_mcs_2_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR);
	tmp &= ~SW_OFDM_HT_40M_MCS_2_MASK;
	tmp |= (value << SW_OFDM_HT_40M_MCS_2_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_6_sw_ofdm_ht_40m_mcs_2_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR)) & SW_OFDM_HT_40M_MCS_2_MASK) >> SW_OFDM_HT_40M_MCS_2_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_6_sw_ofdm_ht_40m_mcs_3_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR);
	tmp &= ~SW_OFDM_HT_40M_MCS_3_MASK;
	tmp |= (value << SW_OFDM_HT_40M_MCS_3_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_6_sw_ofdm_ht_40m_mcs_3_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR)) & SW_OFDM_HT_40M_MCS_3_MASK) >> SW_OFDM_HT_40M_MCS_3_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_6_sw_ofdm_ht_40m_mcs_4_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR);
	tmp &= ~SW_OFDM_HT_40M_MCS_4_MASK;
	tmp |= (value << SW_OFDM_HT_40M_MCS_4_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_6_sw_ofdm_ht_40m_mcs_4_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR)) & SW_OFDM_HT_40M_MCS_4_MASK) >> SW_OFDM_HT_40M_MCS_4_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_6_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR), value);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_6_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR));
}
static __inline void mac_rx_resp_rate_ctrl_6_pack(uint32_t sw_ofdm_ht_40m_mcs_1, uint32_t sw_ofdm_ht_40m_mcs_2, uint32_t sw_ofdm_ht_40m_mcs_3, uint32_t sw_ofdm_ht_40m_mcs_4)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ofdm_ht_40m_mcs_1 << SW_OFDM_HT_40M_MCS_1_OFFSET) & ~((uint32_t)SW_OFDM_HT_40M_MCS_1_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_40m_mcs_2 << SW_OFDM_HT_40M_MCS_2_OFFSET) & ~((uint32_t)SW_OFDM_HT_40M_MCS_2_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_40m_mcs_3 << SW_OFDM_HT_40M_MCS_3_OFFSET) & ~((uint32_t)SW_OFDM_HT_40M_MCS_3_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_40m_mcs_4 << SW_OFDM_HT_40M_MCS_4_OFFSET) & ~((uint32_t)SW_OFDM_HT_40M_MCS_4_MASK)) == 0);
	tmp =
		((uint32_t)sw_ofdm_ht_40m_mcs_1 << SW_OFDM_HT_40M_MCS_1_OFFSET) |
		((uint32_t)sw_ofdm_ht_40m_mcs_2 << SW_OFDM_HT_40M_MCS_2_OFFSET) |
		((uint32_t)sw_ofdm_ht_40m_mcs_3 << SW_OFDM_HT_40M_MCS_3_OFFSET) |
		((uint32_t)sw_ofdm_ht_40m_mcs_4 << SW_OFDM_HT_40M_MCS_4_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_6_ADDR), tmp);
}
static __inline void mac_rx_resp_rate_ctrl_6_unpack(volatile uint32_t* sw_ofdm_ht_40m_mcs_1, volatile uint32_t* sw_ofdm_ht_40m_mcs_2, volatile uint32_t* sw_ofdm_ht_40m_mcs_3, volatile uint32_t* sw_ofdm_ht_40m_mcs_4)
{
	uint32_t tmp;

	tmp = mac_rx_resp_rate_ctrl_6_get();
	*sw_ofdm_ht_40m_mcs_1 = (tmp & ((uint32_t)SW_OFDM_HT_40M_MCS_1_MASK)) >> SW_OFDM_HT_40M_MCS_1_OFFSET;
	*sw_ofdm_ht_40m_mcs_2 = (tmp & ((uint32_t)SW_OFDM_HT_40M_MCS_2_MASK)) >> SW_OFDM_HT_40M_MCS_2_OFFSET;
	*sw_ofdm_ht_40m_mcs_3 = (tmp & ((uint32_t)SW_OFDM_HT_40M_MCS_3_MASK)) >> SW_OFDM_HT_40M_MCS_3_OFFSET;
	*sw_ofdm_ht_40m_mcs_4 = (tmp & ((uint32_t)SW_OFDM_HT_40M_MCS_4_MASK)) >> SW_OFDM_HT_40M_MCS_4_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_7_sw_ofdm_ht_40m_mcs_5_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR);
	tmp &= ~SW_OFDM_HT_40M_MCS_5_MASK;
	tmp |= (value << SW_OFDM_HT_40M_MCS_5_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_7_sw_ofdm_ht_40m_mcs_5_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR)) & SW_OFDM_HT_40M_MCS_5_MASK) >> SW_OFDM_HT_40M_MCS_5_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_7_sw_ofdm_ht_40m_mcs_6_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR);
	tmp &= ~SW_OFDM_HT_40M_MCS_6_MASK;
	tmp |= (value << SW_OFDM_HT_40M_MCS_6_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_7_sw_ofdm_ht_40m_mcs_6_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR)) & SW_OFDM_HT_40M_MCS_6_MASK) >> SW_OFDM_HT_40M_MCS_6_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_7_sw_ofdm_ht_40m_mcs_7_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR);
	tmp &= ~SW_OFDM_HT_40M_MCS_7_MASK;
	tmp |= (value << SW_OFDM_HT_40M_MCS_7_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_7_sw_ofdm_ht_40m_mcs_7_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR)) & SW_OFDM_HT_40M_MCS_7_MASK) >> SW_OFDM_HT_40M_MCS_7_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_7_sw_ofdm_ht_40m_mcs_32_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR);
	tmp &= ~SW_OFDM_HT_40M_MCS_32_MASK;
	tmp |= (value << SW_OFDM_HT_40M_MCS_32_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_7_sw_ofdm_ht_40m_mcs_32_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR)) & SW_OFDM_HT_40M_MCS_32_MASK) >> SW_OFDM_HT_40M_MCS_32_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_7_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR), value);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_7_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR));
}
static __inline void mac_rx_resp_rate_ctrl_7_pack(uint32_t sw_ofdm_ht_40m_mcs_5, uint32_t sw_ofdm_ht_40m_mcs_6, uint32_t sw_ofdm_ht_40m_mcs_7, uint32_t sw_ofdm_ht_40m_mcs_32)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ofdm_ht_40m_mcs_5 << SW_OFDM_HT_40M_MCS_5_OFFSET) & ~((uint32_t)SW_OFDM_HT_40M_MCS_5_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_40m_mcs_6 << SW_OFDM_HT_40M_MCS_6_OFFSET) & ~((uint32_t)SW_OFDM_HT_40M_MCS_6_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_40m_mcs_7 << SW_OFDM_HT_40M_MCS_7_OFFSET) & ~((uint32_t)SW_OFDM_HT_40M_MCS_7_MASK)) == 0);
	assert((((uint32_t)sw_ofdm_ht_40m_mcs_32 << SW_OFDM_HT_40M_MCS_32_OFFSET) & ~((uint32_t)SW_OFDM_HT_40M_MCS_32_MASK)) == 0);
	tmp =
		((uint32_t)sw_ofdm_ht_40m_mcs_5 << SW_OFDM_HT_40M_MCS_5_OFFSET) |
		((uint32_t)sw_ofdm_ht_40m_mcs_6 << SW_OFDM_HT_40M_MCS_6_OFFSET) |
		((uint32_t)sw_ofdm_ht_40m_mcs_7 << SW_OFDM_HT_40M_MCS_7_OFFSET) |
		((uint32_t)sw_ofdm_ht_40m_mcs_32 << SW_OFDM_HT_40M_MCS_32_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_7_ADDR), tmp);
}
static __inline void mac_rx_resp_rate_ctrl_7_unpack(volatile uint32_t* sw_ofdm_ht_40m_mcs_5, volatile uint32_t* sw_ofdm_ht_40m_mcs_6, volatile uint32_t* sw_ofdm_ht_40m_mcs_7, volatile uint32_t* sw_ofdm_ht_40m_mcs_32)
{
	uint32_t tmp;

	tmp = mac_rx_resp_rate_ctrl_7_get();
	*sw_ofdm_ht_40m_mcs_5 = (tmp & ((uint32_t)SW_OFDM_HT_40M_MCS_5_MASK)) >> SW_OFDM_HT_40M_MCS_5_OFFSET;
	*sw_ofdm_ht_40m_mcs_6 = (tmp & ((uint32_t)SW_OFDM_HT_40M_MCS_6_MASK)) >> SW_OFDM_HT_40M_MCS_6_OFFSET;
	*sw_ofdm_ht_40m_mcs_7 = (tmp & ((uint32_t)SW_OFDM_HT_40M_MCS_7_MASK)) >> SW_OFDM_HT_40M_MCS_7_OFFSET;
	*sw_ofdm_ht_40m_mcs_32 = (tmp & ((uint32_t)SW_OFDM_HT_40M_MCS_32_MASK)) >> SW_OFDM_HT_40M_MCS_32_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_8_sw_ofdm_ht_sgi_resp_sgi_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR);
	tmp &= ~SW_OFDM_HT_SGI_RESP_SGI_EN_MASK;
	tmp |= (value << SW_OFDM_HT_SGI_RESP_SGI_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_8_sw_ofdm_ht_sgi_resp_sgi_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR)) & SW_OFDM_HT_SGI_RESP_SGI_EN_MASK) >> SW_OFDM_HT_SGI_RESP_SGI_EN_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_8_sw_ht_cbw20_resp_force_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR);
	tmp &= ~SW_HT_CBW20_RESP_FORCE_EN_MASK;
	tmp |= (value << SW_HT_CBW20_RESP_FORCE_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_8_sw_ht_cbw20_resp_force_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR)) & SW_HT_CBW20_RESP_FORCE_EN_MASK) >> SW_HT_CBW20_RESP_FORCE_EN_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_8_sw_ht_cbw20_resp_force_val_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR);
	tmp &= ~SW_HT_CBW20_RESP_FORCE_VAL_MASK;
	tmp |= (value << SW_HT_CBW20_RESP_FORCE_VAL_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_8_sw_ht_cbw20_resp_force_val_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR)) & SW_HT_CBW20_RESP_FORCE_VAL_MASK) >> SW_HT_CBW20_RESP_FORCE_VAL_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_8_sw_ht_cbw40_resp_force_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR);
	tmp &= ~SW_HT_CBW40_RESP_FORCE_EN_MASK;
	tmp |= (value << SW_HT_CBW40_RESP_FORCE_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_8_sw_ht_cbw40_resp_force_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR)) & SW_HT_CBW40_RESP_FORCE_EN_MASK) >> SW_HT_CBW40_RESP_FORCE_EN_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_8_sw_ht_cbw40_resp_force_val_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR);
	tmp &= ~SW_HT_CBW40_RESP_FORCE_VAL_MASK;
	tmp |= (value << SW_HT_CBW40_RESP_FORCE_VAL_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_8_sw_ht_cbw40_resp_force_val_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR)) & SW_HT_CBW40_RESP_FORCE_VAL_MASK) >> SW_HT_CBW40_RESP_FORCE_VAL_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_8_sw_non_ht_cbw20_resp_force_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR);
	tmp &= ~SW_NON_HT_CBW20_RESP_FORCE_EN_MASK;
	tmp |= (value << SW_NON_HT_CBW20_RESP_FORCE_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_8_sw_non_ht_cbw20_resp_force_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR)) & SW_NON_HT_CBW20_RESP_FORCE_EN_MASK) >> SW_NON_HT_CBW20_RESP_FORCE_EN_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_8_sw_non_ht_cbw20_resp_force_val_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR);
	tmp &= ~SW_NON_HT_CBW20_RESP_FORCE_VAL_MASK;
	tmp |= (value << SW_NON_HT_CBW20_RESP_FORCE_VAL_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_8_sw_non_ht_cbw20_resp_force_val_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR)) & SW_NON_HT_CBW20_RESP_FORCE_VAL_MASK) >> SW_NON_HT_CBW20_RESP_FORCE_VAL_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_8_sw_non_ht_cbw40_resp_force_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR);
	tmp &= ~SW_NON_HT_CBW40_RESP_FORCE_EN_MASK;
	tmp |= (value << SW_NON_HT_CBW40_RESP_FORCE_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_8_sw_non_ht_cbw40_resp_force_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR)) & SW_NON_HT_CBW40_RESP_FORCE_EN_MASK) >> SW_NON_HT_CBW40_RESP_FORCE_EN_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_8_sw_non_ht_cbw40_resp_force_val_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR);
	tmp &= ~SW_NON_HT_CBW40_RESP_FORCE_VAL_MASK;
	tmp |= (value << SW_NON_HT_CBW40_RESP_FORCE_VAL_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_8_sw_non_ht_cbw40_resp_force_val_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR)) & SW_NON_HT_CBW40_RESP_FORCE_VAL_MASK) >> SW_NON_HT_CBW40_RESP_FORCE_VAL_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_8_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), value);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_8_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR));
}
static __inline void mac_rx_resp_rate_ctrl_8_pack(uint32_t sw_ofdm_ht_sgi_resp_sgi_en, uint32_t sw_ht_cbw20_resp_force_en, uint32_t sw_ht_cbw20_resp_force_val, uint32_t sw_ht_cbw40_resp_force_en, uint32_t sw_ht_cbw40_resp_force_val, uint32_t sw_non_ht_cbw20_resp_force_en, uint32_t sw_non_ht_cbw20_resp_force_val, uint32_t sw_non_ht_cbw40_resp_force_en, uint32_t sw_non_ht_cbw40_resp_force_val)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ofdm_ht_sgi_resp_sgi_en << SW_OFDM_HT_SGI_RESP_SGI_EN_OFFSET) & ~((uint32_t)SW_OFDM_HT_SGI_RESP_SGI_EN_MASK)) == 0);
	assert((((uint32_t)sw_ht_cbw20_resp_force_en << SW_HT_CBW20_RESP_FORCE_EN_OFFSET) & ~((uint32_t)SW_HT_CBW20_RESP_FORCE_EN_MASK)) == 0);
	assert((((uint32_t)sw_ht_cbw20_resp_force_val << SW_HT_CBW20_RESP_FORCE_VAL_OFFSET) & ~((uint32_t)SW_HT_CBW20_RESP_FORCE_VAL_MASK)) == 0);
	assert((((uint32_t)sw_ht_cbw40_resp_force_en << SW_HT_CBW40_RESP_FORCE_EN_OFFSET) & ~((uint32_t)SW_HT_CBW40_RESP_FORCE_EN_MASK)) == 0);
	assert((((uint32_t)sw_ht_cbw40_resp_force_val << SW_HT_CBW40_RESP_FORCE_VAL_OFFSET) & ~((uint32_t)SW_HT_CBW40_RESP_FORCE_VAL_MASK)) == 0);
	assert((((uint32_t)sw_non_ht_cbw20_resp_force_en << SW_NON_HT_CBW20_RESP_FORCE_EN_OFFSET) & ~((uint32_t)SW_NON_HT_CBW20_RESP_FORCE_EN_MASK)) == 0);
	assert((((uint32_t)sw_non_ht_cbw20_resp_force_val << SW_NON_HT_CBW20_RESP_FORCE_VAL_OFFSET) & ~((uint32_t)SW_NON_HT_CBW20_RESP_FORCE_VAL_MASK)) == 0);
	assert((((uint32_t)sw_non_ht_cbw40_resp_force_en << SW_NON_HT_CBW40_RESP_FORCE_EN_OFFSET) & ~((uint32_t)SW_NON_HT_CBW40_RESP_FORCE_EN_MASK)) == 0);
	assert((((uint32_t)sw_non_ht_cbw40_resp_force_val << SW_NON_HT_CBW40_RESP_FORCE_VAL_OFFSET) & ~((uint32_t)SW_NON_HT_CBW40_RESP_FORCE_VAL_MASK)) == 0);
	tmp =
		((uint32_t)sw_ofdm_ht_sgi_resp_sgi_en << SW_OFDM_HT_SGI_RESP_SGI_EN_OFFSET) |
		((uint32_t)sw_ht_cbw20_resp_force_en << SW_HT_CBW20_RESP_FORCE_EN_OFFSET) |
		((uint32_t)sw_ht_cbw20_resp_force_val << SW_HT_CBW20_RESP_FORCE_VAL_OFFSET) |
		((uint32_t)sw_ht_cbw40_resp_force_en << SW_HT_CBW40_RESP_FORCE_EN_OFFSET) |
		((uint32_t)sw_ht_cbw40_resp_force_val << SW_HT_CBW40_RESP_FORCE_VAL_OFFSET) |
		((uint32_t)sw_non_ht_cbw20_resp_force_en << SW_NON_HT_CBW20_RESP_FORCE_EN_OFFSET) |
		((uint32_t)sw_non_ht_cbw20_resp_force_val << SW_NON_HT_CBW20_RESP_FORCE_VAL_OFFSET) |
		((uint32_t)sw_non_ht_cbw40_resp_force_en << SW_NON_HT_CBW40_RESP_FORCE_EN_OFFSET) |
		((uint32_t)sw_non_ht_cbw40_resp_force_val << SW_NON_HT_CBW40_RESP_FORCE_VAL_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_8_ADDR), tmp);
}
static __inline void mac_rx_resp_rate_ctrl_8_unpack(volatile uint32_t* sw_ofdm_ht_sgi_resp_sgi_en, volatile uint32_t* sw_ht_cbw20_resp_force_en, volatile uint32_t* sw_ht_cbw20_resp_force_val, volatile uint32_t* sw_ht_cbw40_resp_force_en, volatile uint32_t* sw_ht_cbw40_resp_force_val, volatile uint32_t* sw_non_ht_cbw20_resp_force_en, volatile uint32_t* sw_non_ht_cbw20_resp_force_val, volatile uint32_t* sw_non_ht_cbw40_resp_force_en, volatile uint32_t* sw_non_ht_cbw40_resp_force_val)
{
	uint32_t tmp;

	tmp = mac_rx_resp_rate_ctrl_8_get();
	*sw_ofdm_ht_sgi_resp_sgi_en = (tmp & ((uint32_t)SW_OFDM_HT_SGI_RESP_SGI_EN_MASK)) >> SW_OFDM_HT_SGI_RESP_SGI_EN_OFFSET;
	*sw_ht_cbw20_resp_force_en = (tmp & ((uint32_t)SW_HT_CBW20_RESP_FORCE_EN_MASK)) >> SW_HT_CBW20_RESP_FORCE_EN_OFFSET;
	*sw_ht_cbw20_resp_force_val = (tmp & ((uint32_t)SW_HT_CBW20_RESP_FORCE_VAL_MASK)) >> SW_HT_CBW20_RESP_FORCE_VAL_OFFSET;
	*sw_ht_cbw40_resp_force_en = (tmp & ((uint32_t)SW_HT_CBW40_RESP_FORCE_EN_MASK)) >> SW_HT_CBW40_RESP_FORCE_EN_OFFSET;
	*sw_ht_cbw40_resp_force_val = (tmp & ((uint32_t)SW_HT_CBW40_RESP_FORCE_VAL_MASK)) >> SW_HT_CBW40_RESP_FORCE_VAL_OFFSET;
	*sw_non_ht_cbw20_resp_force_en = (tmp & ((uint32_t)SW_NON_HT_CBW20_RESP_FORCE_EN_MASK)) >> SW_NON_HT_CBW20_RESP_FORCE_EN_OFFSET;
	*sw_non_ht_cbw20_resp_force_val = (tmp & ((uint32_t)SW_NON_HT_CBW20_RESP_FORCE_VAL_MASK)) >> SW_NON_HT_CBW20_RESP_FORCE_VAL_OFFSET;
	*sw_non_ht_cbw40_resp_force_en = (tmp & ((uint32_t)SW_NON_HT_CBW40_RESP_FORCE_EN_MASK)) >> SW_NON_HT_CBW40_RESP_FORCE_EN_OFFSET;
	*sw_non_ht_cbw40_resp_force_val = (tmp & ((uint32_t)SW_NON_HT_CBW40_RESP_FORCE_VAL_MASK)) >> SW_NON_HT_CBW40_RESP_FORCE_VAL_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_9_sw_ofdm_ht_rts_rsp_rate_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR);
	tmp &= ~SW_OFDM_HT_RTS_RSP_RATE_MASK;
	tmp |= (value << SW_OFDM_HT_RTS_RSP_RATE_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_9_sw_ofdm_ht_rts_rsp_rate_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR)) & SW_OFDM_HT_RTS_RSP_RATE_MASK) >> SW_OFDM_HT_RTS_RSP_RATE_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_9_sw_force_ht_rts_rate_same_cts_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR);
	tmp &= ~SW_FORCE_HT_RTS_RATE_SAME_CTS_MASK;
	tmp |= (value << SW_FORCE_HT_RTS_RATE_SAME_CTS_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_9_sw_force_ht_rts_rate_same_cts_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR)) & SW_FORCE_HT_RTS_RATE_SAME_CTS_MASK) >> SW_FORCE_HT_RTS_RATE_SAME_CTS_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_9_sw_force_ht_rts_rsp_rate_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR);
	tmp &= ~SW_FORCE_HT_RTS_RSP_RATE_EN_MASK;
	tmp |= (value << SW_FORCE_HT_RTS_RSP_RATE_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR), tmp);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_9_sw_force_ht_rts_rsp_rate_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR)) & SW_FORCE_HT_RTS_RSP_RATE_EN_MASK) >> SW_FORCE_HT_RTS_RSP_RATE_EN_OFFSET;
}
static __inline void mac_rx_resp_rate_ctrl_9_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR), value);
}
static __inline uint32_t mac_rx_resp_rate_ctrl_9_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR));
}
static __inline void mac_rx_resp_rate_ctrl_9_pack(uint32_t sw_ofdm_ht_rts_rsp_rate, uint32_t sw_force_ht_rts_rate_same_cts, uint32_t sw_force_ht_rts_rsp_rate_en)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ofdm_ht_rts_rsp_rate << SW_OFDM_HT_RTS_RSP_RATE_OFFSET) & ~((uint32_t)SW_OFDM_HT_RTS_RSP_RATE_MASK)) == 0);
	assert((((uint32_t)sw_force_ht_rts_rate_same_cts << SW_FORCE_HT_RTS_RATE_SAME_CTS_OFFSET) & ~((uint32_t)SW_FORCE_HT_RTS_RATE_SAME_CTS_MASK)) == 0);
	assert((((uint32_t)sw_force_ht_rts_rsp_rate_en << SW_FORCE_HT_RTS_RSP_RATE_EN_OFFSET) & ~((uint32_t)SW_FORCE_HT_RTS_RSP_RATE_EN_MASK)) == 0);
	tmp =
		((uint32_t)sw_ofdm_ht_rts_rsp_rate << SW_OFDM_HT_RTS_RSP_RATE_OFFSET) |
		((uint32_t)sw_force_ht_rts_rate_same_cts << SW_FORCE_HT_RTS_RATE_SAME_CTS_OFFSET) |
		((uint32_t)sw_force_ht_rts_rsp_rate_en << SW_FORCE_HT_RTS_RSP_RATE_EN_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_RX_RESP_RATE_CTRL_9_ADDR), tmp);
}
static __inline void mac_rx_resp_rate_ctrl_9_unpack(volatile uint32_t* sw_ofdm_ht_rts_rsp_rate, volatile uint32_t* sw_force_ht_rts_rate_same_cts, volatile uint32_t* sw_force_ht_rts_rsp_rate_en)
{
	uint32_t tmp;

	tmp = mac_rx_resp_rate_ctrl_9_get();
	*sw_ofdm_ht_rts_rsp_rate = (tmp & ((uint32_t)SW_OFDM_HT_RTS_RSP_RATE_MASK)) >> SW_OFDM_HT_RTS_RSP_RATE_OFFSET;
	*sw_force_ht_rts_rate_same_cts = (tmp & ((uint32_t)SW_FORCE_HT_RTS_RATE_SAME_CTS_MASK)) >> SW_FORCE_HT_RTS_RATE_SAME_CTS_OFFSET;
	*sw_force_ht_rts_rsp_rate_en = (tmp & ((uint32_t)SW_FORCE_HT_RTS_RSP_RATE_EN_MASK)) >> SW_FORCE_HT_RTS_RSP_RATE_EN_OFFSET;
}
static __inline void mac_role0_self_addr_low_sw_role0_self_addr_low_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_LOW_ADDR);
	tmp &= ~SW_ROLE0_SELF_ADDR_LOW_MASK;
	tmp |= (value << SW_ROLE0_SELF_ADDR_LOW_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_LOW_ADDR), tmp);
}
static __inline uint32_t mac_role0_self_addr_low_sw_role0_self_addr_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_LOW_ADDR)) & SW_ROLE0_SELF_ADDR_LOW_MASK) >> SW_ROLE0_SELF_ADDR_LOW_OFFSET;
}
static __inline void mac_role0_self_addr_low_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_LOW_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_LOW_ADDR), value);
}
static __inline uint32_t mac_role0_self_addr_low_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_LOW_ADDR));
}
static __inline void mac_role0_self_addr_low_pack(uint32_t sw_role0_self_addr_low)
{
	uint32_t tmp;

	assert((((uint32_t)sw_role0_self_addr_low << SW_ROLE0_SELF_ADDR_LOW_OFFSET) & ~((uint32_t)SW_ROLE0_SELF_ADDR_LOW_MASK)) == 0);
	tmp =
		((uint32_t)sw_role0_self_addr_low << SW_ROLE0_SELF_ADDR_LOW_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_LOW_ADDR), tmp);
}
static __inline void mac_role0_self_addr_low_unpack(volatile uint32_t* sw_role0_self_addr_low)
{
	uint32_t tmp;

	tmp = mac_role0_self_addr_low_get();
	*sw_role0_self_addr_low = (tmp & ((uint32_t)SW_ROLE0_SELF_ADDR_LOW_MASK)) >> SW_ROLE0_SELF_ADDR_LOW_OFFSET;
}
static __inline void mac_role0_self_addr_high_sw_role0_self_addr_high_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_HIGH_ADDR);
	tmp &= ~SW_ROLE0_SELF_ADDR_HIGH_MASK;
	tmp |= (value << SW_ROLE0_SELF_ADDR_HIGH_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_HIGH_ADDR), tmp);
}
static __inline uint32_t mac_role0_self_addr_high_sw_role0_self_addr_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_HIGH_ADDR)) & SW_ROLE0_SELF_ADDR_HIGH_MASK) >> SW_ROLE0_SELF_ADDR_HIGH_OFFSET;
}
static __inline void mac_role0_self_addr_high_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_HIGH_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_HIGH_ADDR), value);
}
static __inline uint32_t mac_role0_self_addr_high_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_HIGH_ADDR));
}
static __inline void mac_role0_self_addr_high_pack(uint32_t sw_role0_self_addr_high)
{
	uint32_t tmp;

	assert((((uint32_t)sw_role0_self_addr_high << SW_ROLE0_SELF_ADDR_HIGH_OFFSET) & ~((uint32_t)SW_ROLE0_SELF_ADDR_HIGH_MASK)) == 0);
	tmp =
		((uint32_t)sw_role0_self_addr_high << SW_ROLE0_SELF_ADDR_HIGH_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_SELF_ADDR_HIGH_ADDR), tmp);
}
static __inline void mac_role0_self_addr_high_unpack(volatile uint32_t* sw_role0_self_addr_high)
{
	uint32_t tmp;

	tmp = mac_role0_self_addr_high_get();
	*sw_role0_self_addr_high = (tmp & ((uint32_t)SW_ROLE0_SELF_ADDR_HIGH_MASK)) >> SW_ROLE0_SELF_ADDR_HIGH_OFFSET;
}
static __inline void mac_role0_ap_addr_low_sw_role0_ap_addr_low_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_LOW_ADDR);
	tmp &= ~SW_ROLE0_AP_ADDR_LOW_MASK;
	tmp |= (value << SW_ROLE0_AP_ADDR_LOW_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_LOW_ADDR), tmp);
}
static __inline uint32_t mac_role0_ap_addr_low_sw_role0_ap_addr_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_LOW_ADDR)) & SW_ROLE0_AP_ADDR_LOW_MASK) >> SW_ROLE0_AP_ADDR_LOW_OFFSET;
}
static __inline void mac_role0_ap_addr_low_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_LOW_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_LOW_ADDR), value);
}
static __inline uint32_t mac_role0_ap_addr_low_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_LOW_ADDR));
}
static __inline void mac_role0_ap_addr_low_pack(uint32_t sw_role0_ap_addr_low)
{
	uint32_t tmp;

	assert((((uint32_t)sw_role0_ap_addr_low << SW_ROLE0_AP_ADDR_LOW_OFFSET) & ~((uint32_t)SW_ROLE0_AP_ADDR_LOW_MASK)) == 0);
	tmp =
		((uint32_t)sw_role0_ap_addr_low << SW_ROLE0_AP_ADDR_LOW_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_LOW_ADDR), tmp);
}
static __inline void mac_role0_ap_addr_low_unpack(volatile uint32_t* sw_role0_ap_addr_low)
{
	uint32_t tmp;

	tmp = mac_role0_ap_addr_low_get();
	*sw_role0_ap_addr_low = (tmp & ((uint32_t)SW_ROLE0_AP_ADDR_LOW_MASK)) >> SW_ROLE0_AP_ADDR_LOW_OFFSET;
}
static __inline void mac_role0_ap_addr_high_sw_role0_ap_addr_high_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_HIGH_ADDR);
	tmp &= ~SW_ROLE0_AP_ADDR_HIGH_MASK;
	tmp |= (value << SW_ROLE0_AP_ADDR_HIGH_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_HIGH_ADDR), tmp);
}
static __inline uint32_t mac_role0_ap_addr_high_sw_role0_ap_addr_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_HIGH_ADDR)) & SW_ROLE0_AP_ADDR_HIGH_MASK) >> SW_ROLE0_AP_ADDR_HIGH_OFFSET;
}
static __inline void mac_role0_ap_addr_high_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_HIGH_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_HIGH_ADDR), value);
}
static __inline uint32_t mac_role0_ap_addr_high_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_HIGH_ADDR));
}
static __inline void mac_role0_ap_addr_high_pack(uint32_t sw_role0_ap_addr_high)
{
	uint32_t tmp;

	assert((((uint32_t)sw_role0_ap_addr_high << SW_ROLE0_AP_ADDR_HIGH_OFFSET) & ~((uint32_t)SW_ROLE0_AP_ADDR_HIGH_MASK)) == 0);
	tmp =
		((uint32_t)sw_role0_ap_addr_high << SW_ROLE0_AP_ADDR_HIGH_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_AP_ADDR_HIGH_ADDR), tmp);
}
static __inline void mac_role0_ap_addr_high_unpack(volatile uint32_t* sw_role0_ap_addr_high)
{
	uint32_t tmp;

	tmp = mac_role0_ap_addr_high_get();
	*sw_role0_ap_addr_high = (tmp & ((uint32_t)SW_ROLE0_AP_ADDR_HIGH_MASK)) >> SW_ROLE0_AP_ADDR_HIGH_OFFSET;
}
static __inline void mac_role0_ctrl_sw_self_alloc_aid0_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR);
	tmp &= ~SW_SELF_ALLOC_AID0_MASK;
	tmp |= (value << SW_SELF_ALLOC_AID0_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_role0_ctrl_sw_self_alloc_aid0_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR)) & SW_SELF_ALLOC_AID0_MASK) >> SW_SELF_ALLOC_AID0_OFFSET;
}
static __inline void mac_role0_ctrl_sw_in_bss_self_role0_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR);
	tmp &= ~SW_IN_BSS_SELF_ROLE0_MASK;
	tmp |= (value << SW_IN_BSS_SELF_ROLE0_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_role0_ctrl_sw_in_bss_self_role0_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR)) & SW_IN_BSS_SELF_ROLE0_MASK) >> SW_IN_BSS_SELF_ROLE0_OFFSET;
}
static __inline void mac_role0_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR), value);
}
static __inline uint32_t mac_role0_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR));
}
static __inline void mac_role0_ctrl_pack(uint32_t sw_self_alloc_aid0, uint32_t sw_in_bss_self_role0)
{
	uint32_t tmp;

	assert((((uint32_t)sw_self_alloc_aid0 << SW_SELF_ALLOC_AID0_OFFSET) & ~((uint32_t)SW_SELF_ALLOC_AID0_MASK)) == 0);
	assert((((uint32_t)sw_in_bss_self_role0 << SW_IN_BSS_SELF_ROLE0_OFFSET) & ~((uint32_t)SW_IN_BSS_SELF_ROLE0_MASK)) == 0);
	tmp =
		((uint32_t)sw_self_alloc_aid0 << SW_SELF_ALLOC_AID0_OFFSET) |
		((uint32_t)sw_in_bss_self_role0 << SW_IN_BSS_SELF_ROLE0_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE0_CTRL_ADDR), tmp);
}
static __inline void mac_role0_ctrl_unpack(volatile uint32_t* sw_self_alloc_aid0, volatile uint32_t* sw_in_bss_self_role0)
{
	uint32_t tmp;

	tmp = mac_role0_ctrl_get();
	*sw_self_alloc_aid0 = (tmp & ((uint32_t)SW_SELF_ALLOC_AID0_MASK)) >> SW_SELF_ALLOC_AID0_OFFSET;
	*sw_in_bss_self_role0 = (tmp & ((uint32_t)SW_IN_BSS_SELF_ROLE0_MASK)) >> SW_IN_BSS_SELF_ROLE0_OFFSET;
}
static __inline void mac_role1_self_addr_low_sw_role1_self_addr_low_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_LOW_ADDR);
	tmp &= ~SW_ROLE1_SELF_ADDR_LOW_MASK;
	tmp |= (value << SW_ROLE1_SELF_ADDR_LOW_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_LOW_ADDR), tmp);
}
static __inline uint32_t mac_role1_self_addr_low_sw_role1_self_addr_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_LOW_ADDR)) & SW_ROLE1_SELF_ADDR_LOW_MASK) >> SW_ROLE1_SELF_ADDR_LOW_OFFSET;
}
static __inline void mac_role1_self_addr_low_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_LOW_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_LOW_ADDR), value);
}
static __inline uint32_t mac_role1_self_addr_low_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_LOW_ADDR));
}
static __inline void mac_role1_self_addr_low_pack(uint32_t sw_role1_self_addr_low)
{
	uint32_t tmp;

	assert((((uint32_t)sw_role1_self_addr_low << SW_ROLE1_SELF_ADDR_LOW_OFFSET) & ~((uint32_t)SW_ROLE1_SELF_ADDR_LOW_MASK)) == 0);
	tmp =
		((uint32_t)sw_role1_self_addr_low << SW_ROLE1_SELF_ADDR_LOW_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_LOW_ADDR), tmp);
}
static __inline void mac_role1_self_addr_low_unpack(volatile uint32_t* sw_role1_self_addr_low)
{
	uint32_t tmp;

	tmp = mac_role1_self_addr_low_get();
	*sw_role1_self_addr_low = (tmp & ((uint32_t)SW_ROLE1_SELF_ADDR_LOW_MASK)) >> SW_ROLE1_SELF_ADDR_LOW_OFFSET;
}
static __inline void mac_role1_self_addr_high_sw_role1_self_addr_high_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_HIGH_ADDR);
	tmp &= ~SW_ROLE1_SELF_ADDR_HIGH_MASK;
	tmp |= (value << SW_ROLE1_SELF_ADDR_HIGH_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_HIGH_ADDR), tmp);
}
static __inline uint32_t mac_role1_self_addr_high_sw_role1_self_addr_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_HIGH_ADDR)) & SW_ROLE1_SELF_ADDR_HIGH_MASK) >> SW_ROLE1_SELF_ADDR_HIGH_OFFSET;
}
static __inline void mac_role1_self_addr_high_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_HIGH_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_HIGH_ADDR), value);
}
static __inline uint32_t mac_role1_self_addr_high_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_HIGH_ADDR));
}
static __inline void mac_role1_self_addr_high_pack(uint32_t sw_role1_self_addr_high)
{
	uint32_t tmp;

	assert((((uint32_t)sw_role1_self_addr_high << SW_ROLE1_SELF_ADDR_HIGH_OFFSET) & ~((uint32_t)SW_ROLE1_SELF_ADDR_HIGH_MASK)) == 0);
	tmp =
		((uint32_t)sw_role1_self_addr_high << SW_ROLE1_SELF_ADDR_HIGH_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_SELF_ADDR_HIGH_ADDR), tmp);
}
static __inline void mac_role1_self_addr_high_unpack(volatile uint32_t* sw_role1_self_addr_high)
{
	uint32_t tmp;

	tmp = mac_role1_self_addr_high_get();
	*sw_role1_self_addr_high = (tmp & ((uint32_t)SW_ROLE1_SELF_ADDR_HIGH_MASK)) >> SW_ROLE1_SELF_ADDR_HIGH_OFFSET;
}
static __inline void mac_role1_ap_addr_low_sw_role1_ap_addr_low_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_LOW_ADDR);
	tmp &= ~SW_ROLE1_AP_ADDR_LOW_MASK;
	tmp |= (value << SW_ROLE1_AP_ADDR_LOW_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_LOW_ADDR), tmp);
}
static __inline uint32_t mac_role1_ap_addr_low_sw_role1_ap_addr_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_LOW_ADDR)) & SW_ROLE1_AP_ADDR_LOW_MASK) >> SW_ROLE1_AP_ADDR_LOW_OFFSET;
}
static __inline void mac_role1_ap_addr_low_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_LOW_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_LOW_ADDR), value);
}
static __inline uint32_t mac_role1_ap_addr_low_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_LOW_ADDR));
}
static __inline void mac_role1_ap_addr_low_pack(uint32_t sw_role1_ap_addr_low)
{
	uint32_t tmp;

	assert((((uint32_t)sw_role1_ap_addr_low << SW_ROLE1_AP_ADDR_LOW_OFFSET) & ~((uint32_t)SW_ROLE1_AP_ADDR_LOW_MASK)) == 0);
	tmp =
		((uint32_t)sw_role1_ap_addr_low << SW_ROLE1_AP_ADDR_LOW_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_LOW_ADDR), tmp);
}
static __inline void mac_role1_ap_addr_low_unpack(volatile uint32_t* sw_role1_ap_addr_low)
{
	uint32_t tmp;

	tmp = mac_role1_ap_addr_low_get();
	*sw_role1_ap_addr_low = (tmp & ((uint32_t)SW_ROLE1_AP_ADDR_LOW_MASK)) >> SW_ROLE1_AP_ADDR_LOW_OFFSET;
}
static __inline void mac_role1_ap_addr_high_sw_role1_ap_addr_high_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_HIGH_ADDR);
	tmp &= ~SW_ROLE1_AP_ADDR_HIGH_MASK;
	tmp |= (value << SW_ROLE1_AP_ADDR_HIGH_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_HIGH_ADDR), tmp);
}
static __inline uint32_t mac_role1_ap_addr_high_sw_role1_ap_addr_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_HIGH_ADDR)) & SW_ROLE1_AP_ADDR_HIGH_MASK) >> SW_ROLE1_AP_ADDR_HIGH_OFFSET;
}
static __inline void mac_role1_ap_addr_high_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_HIGH_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_HIGH_ADDR), value);
}
static __inline uint32_t mac_role1_ap_addr_high_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_HIGH_ADDR));
}
static __inline void mac_role1_ap_addr_high_pack(uint32_t sw_role1_ap_addr_high)
{
	uint32_t tmp;

	assert((((uint32_t)sw_role1_ap_addr_high << SW_ROLE1_AP_ADDR_HIGH_OFFSET) & ~((uint32_t)SW_ROLE1_AP_ADDR_HIGH_MASK)) == 0);
	tmp =
		((uint32_t)sw_role1_ap_addr_high << SW_ROLE1_AP_ADDR_HIGH_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_AP_ADDR_HIGH_ADDR), tmp);
}
static __inline void mac_role1_ap_addr_high_unpack(volatile uint32_t* sw_role1_ap_addr_high)
{
	uint32_t tmp;

	tmp = mac_role1_ap_addr_high_get();
	*sw_role1_ap_addr_high = (tmp & ((uint32_t)SW_ROLE1_AP_ADDR_HIGH_MASK)) >> SW_ROLE1_AP_ADDR_HIGH_OFFSET;
}
static __inline void mac_role1_ctrl_sw_self_alloc_aid1_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR);
	tmp &= ~SW_SELF_ALLOC_AID1_MASK;
	tmp |= (value << SW_SELF_ALLOC_AID1_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_role1_ctrl_sw_self_alloc_aid1_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR)) & SW_SELF_ALLOC_AID1_MASK) >> SW_SELF_ALLOC_AID1_OFFSET;
}
static __inline void mac_role1_ctrl_sw_in_bss_self_role1_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR);
	tmp &= ~SW_IN_BSS_SELF_ROLE1_MASK;
	tmp |= (value << SW_IN_BSS_SELF_ROLE1_OFFSET);
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_role1_ctrl_sw_in_bss_self_role1_get(void)
{
	return ((WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR)) & SW_IN_BSS_SELF_ROLE1_MASK) >> SW_IN_BSS_SELF_ROLE1_OFFSET;
}
static __inline void mac_role1_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR), value);
}
static __inline uint32_t mac_role1_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR));
}
static __inline void mac_role1_ctrl_pack(uint32_t sw_self_alloc_aid1, uint32_t sw_in_bss_self_role1)
{
	uint32_t tmp;

	assert((((uint32_t)sw_self_alloc_aid1 << SW_SELF_ALLOC_AID1_OFFSET) & ~((uint32_t)SW_SELF_ALLOC_AID1_MASK)) == 0);
	assert((((uint32_t)sw_in_bss_self_role1 << SW_IN_BSS_SELF_ROLE1_OFFSET) & ~((uint32_t)SW_IN_BSS_SELF_ROLE1_MASK)) == 0);
	tmp =
		((uint32_t)sw_self_alloc_aid1 << SW_SELF_ALLOC_AID1_OFFSET) |
		((uint32_t)sw_in_bss_self_role1 << SW_IN_BSS_SELF_ROLE1_OFFSET) ;
	WQ_MAC_WRITE(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TOP_BASEADDR + CFG_MAC_ROLE1_CTRL_ADDR), tmp);
}
static __inline void mac_role1_ctrl_unpack(volatile uint32_t* sw_self_alloc_aid1, volatile uint32_t* sw_in_bss_self_role1)
{
	uint32_t tmp;

	tmp = mac_role1_ctrl_get();
	*sw_self_alloc_aid1 = (tmp & ((uint32_t)SW_SELF_ALLOC_AID1_MASK)) >> SW_SELF_ALLOC_AID1_OFFSET;
	*sw_in_bss_self_role1 = (tmp & ((uint32_t)SW_IN_BSS_SELF_ROLE1_MASK)) >> SW_IN_BSS_SELF_ROLE1_OFFSET;
}





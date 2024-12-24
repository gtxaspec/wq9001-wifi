
//-----------------------------------
#define CFG_MAC_RXR_CTRL_ADDR 0x0000
#define SW_MAC_RXR_ABORT_PROT_EN_OFFSET 0
#define SW_MAC_RXR_ABORT_PROT_EN_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_RXR_DBG_TIMER_ADDR 0x0004
#define SW_MAC_RXR_DBG_TIMER_OFFSET 0
#define SW_MAC_RXR_DBG_TIMER_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR_SIGTAP_CTRL_ADDR 0x0010
#define SW_MAC_RXR1_SIGTAP_TRIG_OFFSET 9
#define SW_MAC_RXR1_SIGTAP_TRIG_MASK 0x00000200
#define SW_MAC_RXR0_SIGTAP_TRIG_OFFSET 8
#define SW_MAC_RXR0_SIGTAP_TRIG_MASK 0x00000100
#define SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_OFFSET 4
#define SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_MASK 0x000000F0
#define SW_MAC_RXR_SIGTAP_MUX_OFFSET 0
#define SW_MAC_RXR_SIGTAP_MUX_MASK 0x0000000F

//-----------------------------------
#define CFG_MAC_RXR_DBG_CTRL_ADDR 0x0030
#define SW_FORCE_RXR_SEL_EN_OFFSET 5
#define SW_FORCE_RXR_SEL_EN_MASK 0x00000020
#define SW_FORCE_RXR_SEL_VAL_OFFSET 4
#define SW_FORCE_RXR_SEL_VAL_MASK 0x00000010
#define SW_RXR_PHY_RX_START_RST_EN_OFFSET 1
#define SW_RXR_PHY_RX_START_RST_EN_MASK 0x00000002
#define SW_MAC_RXR_DBG_GENERAL_CLR_OFFSET 0
#define SW_MAC_RXR_DBG_GENERAL_CLR_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_RXR_STS_0_ADDR 0x0040
#define SW_RO_RXR_MPDU_DATA_DONE_CNT_OFFSET 16
#define SW_RO_RXR_MPDU_DATA_DONE_CNT_MASK 0xFFFF0000
#define SW_RO_RXR_MPDU_START_CNT_OFFSET 0
#define SW_RO_RXR_MPDU_START_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR_STS_1_ADDR 0x0044
#define SW_RO_RXR_MPDU_DESC_DONE_CNT_OFFSET 16
#define SW_RO_RXR_MPDU_DESC_DONE_CNT_MASK 0xFFFF0000
#define SW_RO_RXR_PSDU_ABORT_CNT_OFFSET 0
#define SW_RO_RXR_PSDU_ABORT_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR_STS_2_ADDR 0x0048
#define SW_RO_RXR0_MPDU_DATA_DONE_CNT_OFFSET 16
#define SW_RO_RXR0_MPDU_DATA_DONE_CNT_MASK 0xFFFF0000
#define SW_RO_RXR0_MPDU_START_CNT_OFFSET 0
#define SW_RO_RXR0_MPDU_START_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR_STS_3_ADDR 0x004c
#define SW_RO_RXR0_MPDU_DESC_DONE_CNT_OFFSET 16
#define SW_RO_RXR0_MPDU_DESC_DONE_CNT_MASK 0xFFFF0000
#define SW_RO_RXR0_PSDU_ABORT_CNT_OFFSET 0
#define SW_RO_RXR0_PSDU_ABORT_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR_STS_4_ADDR 0x0050
#define SW_RO_RXR1_MPDU_DATA_DONE_CNT_OFFSET 16
#define SW_RO_RXR1_MPDU_DATA_DONE_CNT_MASK 0xFFFF0000
#define SW_RO_RXR1_MPDU_START_CNT_OFFSET 0
#define SW_RO_RXR1_MPDU_START_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR_STS_5_ADDR 0x0054
#define SW_RO_RXR1_MPDU_DESC_DONE_CNT_OFFSET 16
#define SW_RO_RXR1_MPDU_DESC_DONE_CNT_MASK 0xFFFF0000
#define SW_RO_RXR1_PSDU_ABORT_CNT_OFFSET 0
#define SW_RO_RXR1_PSDU_ABORT_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR_STS_6_ADDR 0x0058
#define RXR_MPDU_STORE_FLAG_OFFSET 2
#define RXR_MPDU_STORE_FLAG_MASK 0x00000004
#define IS_RXR1_SEL_OFFSET 1
#define IS_RXR1_SEL_MASK 0x00000002
#define RXR_MPDU_RDY_OFFSET 0
#define RXR_MPDU_RDY_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_RXR_STS_7_ADDR 0x005c
#define RXR0_MPDU_RDY_OFFSET 16
#define RXR0_MPDU_RDY_MASK 0x00010000
#define RXR1_MPDU_RDY_OFFSET 0
#define RXR1_MPDU_RDY_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_RXR0_CTRL_ADDR 0x0100
#define SW_RXR0_SRST_OFFSET 4
#define SW_RXR0_SRST_MASK 0x00000010
#define SW_RXR0_EN_OFFSET 2
#define SW_RXR0_EN_MASK 0x00000004
#define SW_RXR0_PP_BUF_SRST_OFFSET 1
#define SW_RXR0_PP_BUF_SRST_MASK 0x00000002
#define SW_RXR0_PTR_SRST_OFFSET 0
#define SW_RXR0_PTR_SRST_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_RXR0_PTR_ADDR 0x0108
#define SW_RXR0_START_PTR_OFFSET 0
#define SW_RXR0_START_PTR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_NUM_ADDR 0x010c
#define SW_RXR0_PTR_NUM_OFFSET 0
#define SW_RXR0_PTR_NUM_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR0_BUF_LEN_ADDR 0x0120
#define SW_RXR0_RX_BUF_LEN_OFFSET 0
#define SW_RXR0_RX_BUF_LEN_MASK 0x00003FFF

//-----------------------------------
#define CFG_MAC_RXR0_INT_CTRL_ADDR 0x0180
#define SW_RXR0_MPDU_DONE_INT_MITIGATION_OFFSET 16
#define SW_RXR0_MPDU_DONE_INT_MITIGATION_MASK 0x000F0000
#define SW_RXR0_MPDU_DONE_INT_TIMEOUT_OFFSET 0
#define SW_RXR0_MPDU_DONE_INT_TIMEOUT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR0_DBG_CTRL_ADDR 0x0200
#define SW_RXR0_MPDU_RDY_FORCE_EN_OFFSET 5
#define SW_RXR0_MPDU_RDY_FORCE_EN_MASK 0x00000020
#define SW_RXR0_MPDU_RDY_FORCE_VAL_OFFSET 4
#define SW_RXR0_MPDU_RDY_FORCE_VAL_MASK 0x00000010
#define SW_RXR0_DBG_GENERAL_CLR_OFFSET 0
#define SW_RXR0_DBG_GENERAL_CLR_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_RXR0_STS_0_ADDR 0x0210
#define SW_RO_RXR0_RX_DESC_PTR_IS_ZERO_OFFSET 9
#define SW_RO_RXR0_RX_DESC_PTR_IS_ZERO_MASK 0x00000200
#define SW_RO_RXR0_WORD_RXR_MPDU_RDY_OFFSET 8
#define SW_RO_RXR0_WORD_RXR_MPDU_RDY_MASK 0x00000100
#define SW_RO_RXR0_RX_DESC_ACCESS_FSM_OFFSET 4
#define SW_RO_RXR0_RX_DESC_ACCESS_FSM_MASK 0x000000F0
#define SW_RO_RXR0_RXR_MPDU_PROCESS_FSM_OFFSET 0
#define SW_RO_RXR0_RXR_MPDU_PROCESS_FSM_MASK 0x0000000F

//-----------------------------------
#define CFG_MAC_RXR0_STS_1_ADDR 0x0214
#define SW_RO_RXR0_RD_RX_DESC_DONE_CNT_OFFSET 24
#define SW_RO_RXR0_RD_RX_DESC_DONE_CNT_MASK 0xFF000000
#define SW_RO_RXR0_RX_BUF_FILL_DONE_CNT_OFFSET 16
#define SW_RO_RXR0_RX_BUF_FILL_DONE_CNT_MASK 0x00FF0000
#define SW_RO_RXR0_WB_RX_CTRL_OTHER_DONE_CNT_OFFSET 8
#define SW_RO_RXR0_WB_RX_CTRL_OTHER_DONE_CNT_MASK 0x0000FF00
#define SW_RO_RXR0_WB_RX_CTRL0_DONE_CNT_OFFSET 0
#define SW_RO_RXR0_WB_RX_CTRL0_DONE_CNT_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RXR0_STS_2_ADDR 0x0218
#define SW_RO_RXR0_DMA_CORE_DBG_OFFSET 0
#define SW_RO_RXR0_DMA_CORE_DBG_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_3_ADDR 0x021c
#define SW_RO_RXR0_GEN_INT_MPDU_WB_CTRL_DONE_CNT_OFFSET 28
#define SW_RO_RXR0_GEN_INT_MPDU_WB_CTRL_DONE_CNT_MASK 0xF0000000
#define SW_RO_RXR0_MPDU_FILL_BURST_DATA_CNT_OFFSET 24
#define SW_RO_RXR0_MPDU_FILL_BURST_DATA_CNT_MASK 0x0F000000
#define SW_RO_RXR0_MPDU_WB_CTRL_DONE_CNT_OFFSET 16
#define SW_RO_RXR0_MPDU_WB_CTRL_DONE_CNT_MASK 0x00FF0000
#define SW_RO_RXR0_RX_DESC_PTR_CNT_OFFSET 0
#define SW_RO_RXR0_RX_DESC_PTR_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_4_ADDR 0x0220
#define SW_RO_RXR0_CUR_RX_DESC_PTR_PTR_OFFSET 0
#define SW_RO_RXR0_CUR_RX_DESC_PTR_PTR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_5_ADDR 0x0224
#define SW_RO_RXR0_CUR_RX_DESC_PTR_OFFSET 0
#define SW_RO_RXR0_CUR_RX_DESC_PTR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_6_ADDR 0x0228
#define SW_RO_RXR0_MPDU_FILL_BURST_PTR_OFFSET 0
#define SW_RO_RXR0_MPDU_FILL_BURST_PTR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_7_ADDR 0x022c
#define SW_RO_RXR0_RX_DESC_DATA_CNT_OFFSET 28
#define SW_RO_RXR0_RX_DESC_DATA_CNT_MASK 0xF0000000
#define SW_RO_RXR0_RX_BUF_FILL_WORD_CNT_OFFSET 16
#define SW_RO_RXR0_RX_BUF_FILL_WORD_CNT_MASK 0x0FFF0000
#define SW_RO_RXR0_MPDU_FILL_ADDR_OFFSET_OFFSET 0
#define SW_RO_RXR0_MPDU_FILL_ADDR_OFFSET_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_8_ADDR 0x0230
#define SW_RO_RXR0_DROP_MPDU_CNT_OFFSET 16
#define SW_RO_RXR0_DROP_MPDU_CNT_MASK 0xFFFF0000
#define SW_RO_RXR0_RD_RX_DESC_PTR_DONE_CNT_OFFSET 8
#define SW_RO_RXR0_RD_RX_DESC_PTR_DONE_CNT_MASK 0x0000FF00
#define SW_RO_RXR0_MPDU_RX_DESC_CNT_OFFSET 0
#define SW_RO_RXR0_MPDU_RX_DESC_CNT_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RXR0_STS_9_ADDR 0x0234
#define SW_RO_RXR0_RXR_START_VECTOR_0_OFFSET 0
#define SW_RO_RXR0_RXR_START_VECTOR_0_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_10_ADDR 0x0238
#define SW_RO_RXR0_RXR_START_VECTOR_1_OFFSET 0
#define SW_RO_RXR0_RXR_START_VECTOR_1_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_11_ADDR 0x023c
#define SW_RO_RXR0_RXR_START_VECTOR_2_OFFSET 0
#define SW_RO_RXR0_RXR_START_VECTOR_2_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_12_ADDR 0x0240
#define SW_RO_RXR0_RXR_START_VECTOR_3_OFFSET 0
#define SW_RO_RXR0_RXR_START_VECTOR_3_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_13_ADDR 0x0244
#define SW_RO_RXR0_RXR_DONE_VECTOR_0_OFFSET 0
#define SW_RO_RXR0_RXR_DONE_VECTOR_0_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_14_ADDR 0x0248
#define SW_RO_RXR0_RXR_DONE_VECTOR_1_OFFSET 0
#define SW_RO_RXR0_RXR_DONE_VECTOR_1_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_15_ADDR 0x024c
#define SW_RO_RXR0_RXR_DONE_VECTOR_2_OFFSET 0
#define SW_RO_RXR0_RXR_DONE_VECTOR_2_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_16_ADDR 0x0250
#define SW_RO_RXR0_RXR_DONE_VECTOR_3_OFFSET 0
#define SW_RO_RXR0_RXR_DONE_VECTOR_3_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_17_ADDR 0x0254
#define SW_RO_RXR0_B2W_DIN_EN_CNT_OFFSET 16
#define SW_RO_RXR0_B2W_DIN_EN_CNT_MASK 0xFFFF0000
#define SW_RO_RXR0_B2W_DOUT_EN_CNT_OFFSET 0
#define SW_RO_RXR0_B2W_DOUT_EN_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_18_ADDR 0x0258
#define SW_RO_RXR0_BURST_CMD_STS_OFFSET 16
#define SW_RO_RXR0_BURST_CMD_STS_MASK 0xFFFF0000
#define SW_RO_RXR0_FIFO_BURST_GEN_OFFSET 0
#define SW_RO_RXR0_FIFO_BURST_GEN_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR0_STS_19_ADDR 0x025c
#define SW_RO_RXR0_BURST_DONE_PLS_CNT_OFFSET 16
#define SW_RO_RXR0_BURST_DONE_PLS_CNT_MASK 0xFFFF0000
#define SW_RO_RXR0_BURST_START_PLS_CNT_OFFSET 0
#define SW_RO_RXR0_BURST_START_PLS_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR1_CTRL_ADDR 0x0400
#define SW_RXR1_SRST_OFFSET 4
#define SW_RXR1_SRST_MASK 0x00000010
#define SW_RXR1_EN_OFFSET 2
#define SW_RXR1_EN_MASK 0x00000004
#define SW_RXR1_PP_BUF_SRST_OFFSET 1
#define SW_RXR1_PP_BUF_SRST_MASK 0x00000002
#define SW_RXR1_PTR_SRST_OFFSET 0
#define SW_RXR1_PTR_SRST_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_RXR1_PTR_ADDR 0x0408
#define SW_RXR1_START_PTR_OFFSET 0
#define SW_RXR1_START_PTR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_NUM_ADDR 0x040c
#define SW_RXR1_PTR_NUM_OFFSET 0
#define SW_RXR1_PTR_NUM_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR1_BUF_LEN_ADDR 0x0420
#define SW_RXR1_RX_BUF_LEN_OFFSET 0
#define SW_RXR1_RX_BUF_LEN_MASK 0x00003FFF

//-----------------------------------
#define CFG_MAC_RXR1_INT_CTRL_ADDR 0x0480
#define SW_RXR1_MPDU_DONE_INT_MITIGATION_OFFSET 16
#define SW_RXR1_MPDU_DONE_INT_MITIGATION_MASK 0x000F0000
#define SW_RXR1_MPDU_DONE_INT_TIMEOUT_OFFSET 0
#define SW_RXR1_MPDU_DONE_INT_TIMEOUT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR1_DBG_CTRL_ADDR 0x0500
#define SW_RXR1_MPDU_RDY_FORCE_EN_OFFSET 5
#define SW_RXR1_MPDU_RDY_FORCE_EN_MASK 0x00000020
#define SW_RXR1_MPDU_RDY_FORCE_VAL_OFFSET 4
#define SW_RXR1_MPDU_RDY_FORCE_VAL_MASK 0x00000010
#define SW_RXR1_DBG_GENERAL_CLR_OFFSET 0
#define SW_RXR1_DBG_GENERAL_CLR_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_RXR1_STS_0_ADDR 0x0510
#define SW_RO_RXR1_RX_DESC_PTR_IS_ZERO_OFFSET 9
#define SW_RO_RXR1_RX_DESC_PTR_IS_ZERO_MASK 0x00000200
#define SW_RO_RXR1_WORD_RXR_MPDU_RDY_OFFSET 8
#define SW_RO_RXR1_WORD_RXR_MPDU_RDY_MASK 0x00000100
#define SW_RO_RXR1_RX_DESC_ACCESS_FSM_OFFSET 4
#define SW_RO_RXR1_RX_DESC_ACCESS_FSM_MASK 0x000000F0
#define SW_RO_RXR1_RXR_MPDU_PROCESS_FSM_OFFSET 0
#define SW_RO_RXR1_RXR_MPDU_PROCESS_FSM_MASK 0x0000000F

//-----------------------------------
#define CFG_MAC_RXR1_STS_1_ADDR 0x0514
#define SW_RO_RXR1_RD_RX_DESC_DONE_CNT_OFFSET 24
#define SW_RO_RXR1_RD_RX_DESC_DONE_CNT_MASK 0xFF000000
#define SW_RO_RXR1_RX_BUF_FILL_DONE_CNT_OFFSET 16
#define SW_RO_RXR1_RX_BUF_FILL_DONE_CNT_MASK 0x00FF0000
#define SW_RO_RXR1_WB_RX_CTRL_OTHER_DONE_CNT_OFFSET 8
#define SW_RO_RXR1_WB_RX_CTRL_OTHER_DONE_CNT_MASK 0x0000FF00
#define SW_RO_RXR1_WB_RX_CTRL0_DONE_CNT_OFFSET 0
#define SW_RO_RXR1_WB_RX_CTRL0_DONE_CNT_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RXR1_STS_2_ADDR 0x0518
#define SW_RO_RXR1_DMA_CORE_DBG_OFFSET 0
#define SW_RO_RXR1_DMA_CORE_DBG_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_3_ADDR 0x051c
#define SW_RO_RXR1_GEN_INT_MPDU_WB_CTRL_DONE_CNT_OFFSET 28
#define SW_RO_RXR1_GEN_INT_MPDU_WB_CTRL_DONE_CNT_MASK 0xF0000000
#define SW_RO_RXR1_MPDU_FILL_BURST_DATA_CNT_OFFSET 24
#define SW_RO_RXR1_MPDU_FILL_BURST_DATA_CNT_MASK 0x0F000000
#define SW_RO_RXR1_MPDU_WB_CTRL_DONE_CNT_OFFSET 16
#define SW_RO_RXR1_MPDU_WB_CTRL_DONE_CNT_MASK 0x00FF0000
#define SW_RO_RXR1_RX_DESC_PTR_CNT_OFFSET 0
#define SW_RO_RXR1_RX_DESC_PTR_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_4_ADDR 0x0520
#define SW_RO_RXR1_CUR_RX_DESC_PTR_PTR_OFFSET 0
#define SW_RO_RXR1_CUR_RX_DESC_PTR_PTR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_5_ADDR 0x0524
#define SW_RO_RXR1_CUR_RX_DESC_PTR_OFFSET 0
#define SW_RO_RXR1_CUR_RX_DESC_PTR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_6_ADDR 0x0528
#define SW_RO_RXR1_MPDU_FILL_BURST_PTR_OFFSET 0
#define SW_RO_RXR1_MPDU_FILL_BURST_PTR_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_7_ADDR 0x052c
#define SW_RO_RXR1_RX_DESC_DATA_CNT_OFFSET 28
#define SW_RO_RXR1_RX_DESC_DATA_CNT_MASK 0xF0000000
#define SW_RO_RXR1_RX_BUF_FILL_WORD_CNT_OFFSET 16
#define SW_RO_RXR1_RX_BUF_FILL_WORD_CNT_MASK 0x0FFF0000
#define SW_RO_RXR1_MPDU_FILL_ADDR_OFFSET_OFFSET 0
#define SW_RO_RXR1_MPDU_FILL_ADDR_OFFSET_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_8_ADDR 0x0530
#define SW_RO_RXR1_DROP_MPDU_CNT_OFFSET 16
#define SW_RO_RXR1_DROP_MPDU_CNT_MASK 0xFFFF0000
#define SW_RO_RXR1_RD_RX_DESC_PTR_DONE_CNT_OFFSET 8
#define SW_RO_RXR1_RD_RX_DESC_PTR_DONE_CNT_MASK 0x0000FF00
#define SW_RO_RXR1_MPDU_RX_DESC_CNT_OFFSET 0
#define SW_RO_RXR1_MPDU_RX_DESC_CNT_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_RXR1_STS_9_ADDR 0x0534
#define SW_RO_RXR1_RXR_START_VECTOR_0_OFFSET 0
#define SW_RO_RXR1_RXR_START_VECTOR_0_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_10_ADDR 0x0538
#define SW_RO_RXR1_RXR_START_VECTOR_1_OFFSET 0
#define SW_RO_RXR1_RXR_START_VECTOR_1_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_11_ADDR 0x053c
#define SW_RO_RXR1_RXR_START_VECTOR_2_OFFSET 0
#define SW_RO_RXR1_RXR_START_VECTOR_2_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_12_ADDR 0x0540
#define SW_RO_RXR1_RXR_START_VECTOR_3_OFFSET 0
#define SW_RO_RXR1_RXR_START_VECTOR_3_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_13_ADDR 0x0544
#define SW_RO_RXR1_RXR_DONE_VECTOR_0_OFFSET 0
#define SW_RO_RXR1_RXR_DONE_VECTOR_0_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_14_ADDR 0x0548
#define SW_RO_RXR1_RXR_DONE_VECTOR_1_OFFSET 0
#define SW_RO_RXR1_RXR_DONE_VECTOR_1_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_15_ADDR 0x054c
#define SW_RO_RXR1_RXR_DONE_VECTOR_2_OFFSET 0
#define SW_RO_RXR1_RXR_DONE_VECTOR_2_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_16_ADDR 0x0550
#define SW_RO_RXR1_RXR_DONE_VECTOR_3_OFFSET 0
#define SW_RO_RXR1_RXR_DONE_VECTOR_3_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_17_ADDR 0x0554
#define SW_RO_RXR1_B2W_DIN_EN_CNT_OFFSET 16
#define SW_RO_RXR1_B2W_DIN_EN_CNT_MASK 0xFFFF0000
#define SW_RO_RXR1_B2W_DOUT_EN_CNT_OFFSET 0
#define SW_RO_RXR1_B2W_DOUT_EN_CNT_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_18_ADDR 0x0558
#define SW_RO_RXR1_BURST_CMD_STS_OFFSET 16
#define SW_RO_RXR1_BURST_CMD_STS_MASK 0xFFFF0000
#define SW_RO_RXR1_FIFO_BURST_GEN_OFFSET 0
#define SW_RO_RXR1_FIFO_BURST_GEN_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_RXR1_STS_19_ADDR 0x055c
#define SW_RO_RXR1_BURST_DONE_PLS_CNT_OFFSET 16
#define SW_RO_RXR1_BURST_DONE_PLS_CNT_MASK 0xFFFF0000
#define SW_RO_RXR1_BURST_START_PLS_CNT_OFFSET 0
#define SW_RO_RXR1_BURST_START_PLS_CNT_MASK 0x0000FFFF

//HW module read/write macro




static __inline void mac_rxr_ctrl_sw_mac_rxr_abort_prot_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_CTRL_ADDR);
	tmp &= ~SW_MAC_RXR_ABORT_PROT_EN_MASK;
	tmp |= (value << SW_MAC_RXR_ABORT_PROT_EN_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr_ctrl_sw_mac_rxr_abort_prot_en_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_CTRL_ADDR)) & SW_MAC_RXR_ABORT_PROT_EN_MASK) >> SW_MAC_RXR_ABORT_PROT_EN_OFFSET;
}
static __inline void mac_rxr_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_CTRL_ADDR), value);
}
static __inline uint32_t mac_rxr_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_CTRL_ADDR));
}
static __inline void mac_rxr_ctrl_pack(uint32_t sw_mac_rxr_abort_prot_en)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_rxr_abort_prot_en << SW_MAC_RXR_ABORT_PROT_EN_OFFSET) & ~((uint32_t)SW_MAC_RXR_ABORT_PROT_EN_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_rxr_abort_prot_en << SW_MAC_RXR_ABORT_PROT_EN_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_CTRL_ADDR), tmp);
}
static __inline void mac_rxr_ctrl_unpack(volatile uint32_t* sw_mac_rxr_abort_prot_en)
{
	uint32_t tmp;

	tmp = mac_rxr_ctrl_get();
	*sw_mac_rxr_abort_prot_en = (tmp & ((uint32_t)SW_MAC_RXR_ABORT_PROT_EN_MASK)) >> SW_MAC_RXR_ABORT_PROT_EN_OFFSET;
}
static __inline void mac_rxr_dbg_timer_sw_mac_rxr_dbg_timer_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_TIMER_ADDR);
	tmp &= ~SW_MAC_RXR_DBG_TIMER_MASK;
	tmp |= (value << SW_MAC_RXR_DBG_TIMER_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_TIMER_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_TIMER_ADDR), tmp);
}
static __inline uint32_t mac_rxr_dbg_timer_sw_mac_rxr_dbg_timer_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_TIMER_ADDR)) & SW_MAC_RXR_DBG_TIMER_MASK) >> SW_MAC_RXR_DBG_TIMER_OFFSET;
}
static __inline void mac_rxr_dbg_timer_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_TIMER_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_TIMER_ADDR), value);
}
static __inline uint32_t mac_rxr_dbg_timer_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_TIMER_ADDR));
}
static __inline void mac_rxr_dbg_timer_pack(uint32_t sw_mac_rxr_dbg_timer)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_rxr_dbg_timer << SW_MAC_RXR_DBG_TIMER_OFFSET) & ~((uint32_t)SW_MAC_RXR_DBG_TIMER_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_rxr_dbg_timer << SW_MAC_RXR_DBG_TIMER_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_TIMER_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_TIMER_ADDR), tmp);
}
static __inline void mac_rxr_dbg_timer_unpack(volatile uint32_t* sw_mac_rxr_dbg_timer)
{
	uint32_t tmp;

	tmp = mac_rxr_dbg_timer_get();
	*sw_mac_rxr_dbg_timer = (tmp & ((uint32_t)SW_MAC_RXR_DBG_TIMER_MASK)) >> SW_MAC_RXR_DBG_TIMER_OFFSET;
}
static __inline void mac_rxr_sigtap_ctrl_sw_mac_rxr1_sigtap_trig_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR);
	tmp &= ~SW_MAC_RXR1_SIGTAP_TRIG_MASK;
	tmp |= (value << SW_MAC_RXR1_SIGTAP_TRIG_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR), tmp);
}
static __inline void mac_rxr_sigtap_ctrl_sw_mac_rxr0_sigtap_trig_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR);
	tmp &= ~SW_MAC_RXR0_SIGTAP_TRIG_MASK;
	tmp |= (value << SW_MAC_RXR0_SIGTAP_TRIG_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR), tmp);
}
static __inline void mac_rxr_sigtap_ctrl_sw_mac_rxr_desc_processs_sigtap_mux_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR);
	tmp &= ~SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_MASK;
	tmp |= (value << SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr_sigtap_ctrl_sw_mac_rxr_desc_processs_sigtap_mux_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR)) & SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_MASK) >> SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_OFFSET;
}
static __inline void mac_rxr_sigtap_ctrl_sw_mac_rxr_sigtap_mux_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR);
	tmp &= ~SW_MAC_RXR_SIGTAP_MUX_MASK;
	tmp |= (value << SW_MAC_RXR_SIGTAP_MUX_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr_sigtap_ctrl_sw_mac_rxr_sigtap_mux_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR)) & SW_MAC_RXR_SIGTAP_MUX_MASK) >> SW_MAC_RXR_SIGTAP_MUX_OFFSET;
}
static __inline void mac_rxr_sigtap_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR), value);
}
static __inline uint32_t mac_rxr_sigtap_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR));
}
static __inline void mac_rxr_sigtap_ctrl_pack(uint32_t sw_mac_rxr1_sigtap_trig, uint32_t sw_mac_rxr0_sigtap_trig, uint32_t sw_mac_rxr_desc_processs_sigtap_mux, uint32_t sw_mac_rxr_sigtap_mux)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mac_rxr1_sigtap_trig << SW_MAC_RXR1_SIGTAP_TRIG_OFFSET) & ~((uint32_t)SW_MAC_RXR1_SIGTAP_TRIG_MASK)) == 0);
	assert((((uint32_t)sw_mac_rxr0_sigtap_trig << SW_MAC_RXR0_SIGTAP_TRIG_OFFSET) & ~((uint32_t)SW_MAC_RXR0_SIGTAP_TRIG_MASK)) == 0);
	assert((((uint32_t)sw_mac_rxr_desc_processs_sigtap_mux << SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_OFFSET) & ~((uint32_t)SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_MASK)) == 0);
	assert((((uint32_t)sw_mac_rxr_sigtap_mux << SW_MAC_RXR_SIGTAP_MUX_OFFSET) & ~((uint32_t)SW_MAC_RXR_SIGTAP_MUX_MASK)) == 0);
	tmp =
		((uint32_t)sw_mac_rxr1_sigtap_trig << SW_MAC_RXR1_SIGTAP_TRIG_OFFSET) |
		((uint32_t)sw_mac_rxr0_sigtap_trig << SW_MAC_RXR0_SIGTAP_TRIG_OFFSET) |
		((uint32_t)sw_mac_rxr_desc_processs_sigtap_mux << SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_OFFSET) |
		((uint32_t)sw_mac_rxr_sigtap_mux << SW_MAC_RXR_SIGTAP_MUX_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_SIGTAP_CTRL_ADDR), tmp);
}
static __inline void mac_rxr_sigtap_ctrl_unpack(volatile uint32_t* sw_mac_rxr_desc_processs_sigtap_mux, volatile uint32_t* sw_mac_rxr_sigtap_mux)
{
	uint32_t tmp;

	tmp = mac_rxr_sigtap_ctrl_get();
	*sw_mac_rxr_desc_processs_sigtap_mux = (tmp & ((uint32_t)SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_MASK)) >> SW_MAC_RXR_DESC_PROCESSS_SIGTAP_MUX_OFFSET;
	*sw_mac_rxr_sigtap_mux = (tmp & ((uint32_t)SW_MAC_RXR_SIGTAP_MUX_MASK)) >> SW_MAC_RXR_SIGTAP_MUX_OFFSET;
}
static __inline void mac_rxr_dbg_ctrl_sw_force_rxr_sel_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR);
	tmp &= ~SW_FORCE_RXR_SEL_EN_MASK;
	tmp |= (value << SW_FORCE_RXR_SEL_EN_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr_dbg_ctrl_sw_force_rxr_sel_en_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR)) & SW_FORCE_RXR_SEL_EN_MASK) >> SW_FORCE_RXR_SEL_EN_OFFSET;
}
static __inline void mac_rxr_dbg_ctrl_sw_force_rxr_sel_val_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR);
	tmp &= ~SW_FORCE_RXR_SEL_VAL_MASK;
	tmp |= (value << SW_FORCE_RXR_SEL_VAL_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr_dbg_ctrl_sw_force_rxr_sel_val_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR)) & SW_FORCE_RXR_SEL_VAL_MASK) >> SW_FORCE_RXR_SEL_VAL_OFFSET;
}
static __inline void mac_rxr_dbg_ctrl_sw_rxr_phy_rx_start_rst_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR);
	tmp &= ~SW_RXR_PHY_RX_START_RST_EN_MASK;
	tmp |= (value << SW_RXR_PHY_RX_START_RST_EN_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr_dbg_ctrl_sw_rxr_phy_rx_start_rst_en_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR)) & SW_RXR_PHY_RX_START_RST_EN_MASK) >> SW_RXR_PHY_RX_START_RST_EN_OFFSET;
}
static __inline void mac_rxr_dbg_ctrl_sw_mac_rxr_dbg_general_clr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR);
	tmp &= ~SW_MAC_RXR_DBG_GENERAL_CLR_MASK;
	tmp |= (value << SW_MAC_RXR_DBG_GENERAL_CLR_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR), tmp);
}
static __inline void mac_rxr_dbg_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR), value);
}
static __inline uint32_t mac_rxr_dbg_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR));
}
static __inline void mac_rxr_dbg_ctrl_pack(uint32_t sw_force_rxr_sel_en, uint32_t sw_force_rxr_sel_val, uint32_t sw_rxr_phy_rx_start_rst_en, uint32_t sw_mac_rxr_dbg_general_clr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_force_rxr_sel_en << SW_FORCE_RXR_SEL_EN_OFFSET) & ~((uint32_t)SW_FORCE_RXR_SEL_EN_MASK)) == 0);
	assert((((uint32_t)sw_force_rxr_sel_val << SW_FORCE_RXR_SEL_VAL_OFFSET) & ~((uint32_t)SW_FORCE_RXR_SEL_VAL_MASK)) == 0);
	assert((((uint32_t)sw_rxr_phy_rx_start_rst_en << SW_RXR_PHY_RX_START_RST_EN_OFFSET) & ~((uint32_t)SW_RXR_PHY_RX_START_RST_EN_MASK)) == 0);
	assert((((uint32_t)sw_mac_rxr_dbg_general_clr << SW_MAC_RXR_DBG_GENERAL_CLR_OFFSET) & ~((uint32_t)SW_MAC_RXR_DBG_GENERAL_CLR_MASK)) == 0);
	tmp =
		((uint32_t)sw_force_rxr_sel_en << SW_FORCE_RXR_SEL_EN_OFFSET) |
		((uint32_t)sw_force_rxr_sel_val << SW_FORCE_RXR_SEL_VAL_OFFSET) |
		((uint32_t)sw_rxr_phy_rx_start_rst_en << SW_RXR_PHY_RX_START_RST_EN_OFFSET) |
		((uint32_t)sw_mac_rxr_dbg_general_clr << SW_MAC_RXR_DBG_GENERAL_CLR_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR_DBG_CTRL_ADDR), tmp);
}
static __inline void mac_rxr_dbg_ctrl_unpack(volatile uint32_t* sw_force_rxr_sel_en, volatile uint32_t* sw_force_rxr_sel_val, volatile uint32_t* sw_rxr_phy_rx_start_rst_en)
{
	uint32_t tmp;

	tmp = mac_rxr_dbg_ctrl_get();
	*sw_force_rxr_sel_en = (tmp & ((uint32_t)SW_FORCE_RXR_SEL_EN_MASK)) >> SW_FORCE_RXR_SEL_EN_OFFSET;
	*sw_force_rxr_sel_val = (tmp & ((uint32_t)SW_FORCE_RXR_SEL_VAL_MASK)) >> SW_FORCE_RXR_SEL_VAL_OFFSET;
	*sw_rxr_phy_rx_start_rst_en = (tmp & ((uint32_t)SW_RXR_PHY_RX_START_RST_EN_MASK)) >> SW_RXR_PHY_RX_START_RST_EN_OFFSET;
}
static __inline uint32_t mac_rxr_sts_0_sw_ro_rxr_mpdu_data_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_0_ADDR)) & SW_RO_RXR_MPDU_DATA_DONE_CNT_MASK) >> SW_RO_RXR_MPDU_DATA_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_0_sw_ro_rxr_mpdu_start_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_0_ADDR)) & SW_RO_RXR_MPDU_START_CNT_MASK) >> SW_RO_RXR_MPDU_START_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_0_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_0_ADDR));
}
static __inline void mac_rxr_sts_0_unpack(volatile uint32_t* sw_ro_rxr_mpdu_data_done_cnt, volatile uint32_t* sw_ro_rxr_mpdu_start_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr_sts_0_get();
	*sw_ro_rxr_mpdu_data_done_cnt = (tmp & ((uint32_t)SW_RO_RXR_MPDU_DATA_DONE_CNT_MASK)) >> SW_RO_RXR_MPDU_DATA_DONE_CNT_OFFSET;
	*sw_ro_rxr_mpdu_start_cnt = (tmp & ((uint32_t)SW_RO_RXR_MPDU_START_CNT_MASK)) >> SW_RO_RXR_MPDU_START_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_1_sw_ro_rxr_mpdu_desc_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_1_ADDR)) & SW_RO_RXR_MPDU_DESC_DONE_CNT_MASK) >> SW_RO_RXR_MPDU_DESC_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_1_sw_ro_rxr_psdu_abort_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_1_ADDR)) & SW_RO_RXR_PSDU_ABORT_CNT_MASK) >> SW_RO_RXR_PSDU_ABORT_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_1_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_1_ADDR));
}
static __inline void mac_rxr_sts_1_unpack(volatile uint32_t* sw_ro_rxr_mpdu_desc_done_cnt, volatile uint32_t* sw_ro_rxr_psdu_abort_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr_sts_1_get();
	*sw_ro_rxr_mpdu_desc_done_cnt = (tmp & ((uint32_t)SW_RO_RXR_MPDU_DESC_DONE_CNT_MASK)) >> SW_RO_RXR_MPDU_DESC_DONE_CNT_OFFSET;
	*sw_ro_rxr_psdu_abort_cnt = (tmp & ((uint32_t)SW_RO_RXR_PSDU_ABORT_CNT_MASK)) >> SW_RO_RXR_PSDU_ABORT_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_2_sw_ro_rxr0_mpdu_data_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_2_ADDR)) & SW_RO_RXR0_MPDU_DATA_DONE_CNT_MASK) >> SW_RO_RXR0_MPDU_DATA_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_2_sw_ro_rxr0_mpdu_start_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_2_ADDR)) & SW_RO_RXR0_MPDU_START_CNT_MASK) >> SW_RO_RXR0_MPDU_START_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_2_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_2_ADDR));
}
static __inline void mac_rxr_sts_2_unpack(volatile uint32_t* sw_ro_rxr0_mpdu_data_done_cnt, volatile uint32_t* sw_ro_rxr0_mpdu_start_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr_sts_2_get();
	*sw_ro_rxr0_mpdu_data_done_cnt = (tmp & ((uint32_t)SW_RO_RXR0_MPDU_DATA_DONE_CNT_MASK)) >> SW_RO_RXR0_MPDU_DATA_DONE_CNT_OFFSET;
	*sw_ro_rxr0_mpdu_start_cnt = (tmp & ((uint32_t)SW_RO_RXR0_MPDU_START_CNT_MASK)) >> SW_RO_RXR0_MPDU_START_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_3_sw_ro_rxr0_mpdu_desc_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_3_ADDR)) & SW_RO_RXR0_MPDU_DESC_DONE_CNT_MASK) >> SW_RO_RXR0_MPDU_DESC_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_3_sw_ro_rxr0_psdu_abort_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_3_ADDR)) & SW_RO_RXR0_PSDU_ABORT_CNT_MASK) >> SW_RO_RXR0_PSDU_ABORT_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_3_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_3_ADDR));
}
static __inline void mac_rxr_sts_3_unpack(volatile uint32_t* sw_ro_rxr0_mpdu_desc_done_cnt, volatile uint32_t* sw_ro_rxr0_psdu_abort_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr_sts_3_get();
	*sw_ro_rxr0_mpdu_desc_done_cnt = (tmp & ((uint32_t)SW_RO_RXR0_MPDU_DESC_DONE_CNT_MASK)) >> SW_RO_RXR0_MPDU_DESC_DONE_CNT_OFFSET;
	*sw_ro_rxr0_psdu_abort_cnt = (tmp & ((uint32_t)SW_RO_RXR0_PSDU_ABORT_CNT_MASK)) >> SW_RO_RXR0_PSDU_ABORT_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_4_sw_ro_rxr1_mpdu_data_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_4_ADDR)) & SW_RO_RXR1_MPDU_DATA_DONE_CNT_MASK) >> SW_RO_RXR1_MPDU_DATA_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_4_sw_ro_rxr1_mpdu_start_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_4_ADDR)) & SW_RO_RXR1_MPDU_START_CNT_MASK) >> SW_RO_RXR1_MPDU_START_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_4_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_4_ADDR));
}
static __inline void mac_rxr_sts_4_unpack(volatile uint32_t* sw_ro_rxr1_mpdu_data_done_cnt, volatile uint32_t* sw_ro_rxr1_mpdu_start_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr_sts_4_get();
	*sw_ro_rxr1_mpdu_data_done_cnt = (tmp & ((uint32_t)SW_RO_RXR1_MPDU_DATA_DONE_CNT_MASK)) >> SW_RO_RXR1_MPDU_DATA_DONE_CNT_OFFSET;
	*sw_ro_rxr1_mpdu_start_cnt = (tmp & ((uint32_t)SW_RO_RXR1_MPDU_START_CNT_MASK)) >> SW_RO_RXR1_MPDU_START_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_5_sw_ro_rxr1_mpdu_desc_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_5_ADDR)) & SW_RO_RXR1_MPDU_DESC_DONE_CNT_MASK) >> SW_RO_RXR1_MPDU_DESC_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_5_sw_ro_rxr1_psdu_abort_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_5_ADDR)) & SW_RO_RXR1_PSDU_ABORT_CNT_MASK) >> SW_RO_RXR1_PSDU_ABORT_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_5_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_5_ADDR));
}
static __inline void mac_rxr_sts_5_unpack(volatile uint32_t* sw_ro_rxr1_mpdu_desc_done_cnt, volatile uint32_t* sw_ro_rxr1_psdu_abort_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr_sts_5_get();
	*sw_ro_rxr1_mpdu_desc_done_cnt = (tmp & ((uint32_t)SW_RO_RXR1_MPDU_DESC_DONE_CNT_MASK)) >> SW_RO_RXR1_MPDU_DESC_DONE_CNT_OFFSET;
	*sw_ro_rxr1_psdu_abort_cnt = (tmp & ((uint32_t)SW_RO_RXR1_PSDU_ABORT_CNT_MASK)) >> SW_RO_RXR1_PSDU_ABORT_CNT_OFFSET;
}
static __inline uint32_t mac_rxr_sts_6_rxr_mpdu_store_flag_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_6_ADDR)) & RXR_MPDU_STORE_FLAG_MASK) >> RXR_MPDU_STORE_FLAG_OFFSET;
}
static __inline uint32_t mac_rxr_sts_6_is_rxr1_sel_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_6_ADDR)) & IS_RXR1_SEL_MASK) >> IS_RXR1_SEL_OFFSET;
}
static __inline uint32_t mac_rxr_sts_6_rxr_mpdu_rdy_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_6_ADDR)) & RXR_MPDU_RDY_MASK) >> RXR_MPDU_RDY_OFFSET;
}
static __inline uint32_t mac_rxr_sts_6_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_6_ADDR));
}
static __inline void mac_rxr_sts_6_unpack(volatile uint32_t* rxr_mpdu_store_flag, volatile uint32_t* is_rxr1_sel, volatile uint32_t* rxr_mpdu_rdy)
{
	uint32_t tmp;

	tmp = mac_rxr_sts_6_get();
	*rxr_mpdu_store_flag = (tmp & ((uint32_t)RXR_MPDU_STORE_FLAG_MASK)) >> RXR_MPDU_STORE_FLAG_OFFSET;
	*is_rxr1_sel = (tmp & ((uint32_t)IS_RXR1_SEL_MASK)) >> IS_RXR1_SEL_OFFSET;
	*rxr_mpdu_rdy = (tmp & ((uint32_t)RXR_MPDU_RDY_MASK)) >> RXR_MPDU_RDY_OFFSET;
}
static __inline uint32_t mac_rxr_sts_7_rxr0_mpdu_rdy_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_7_ADDR)) & RXR0_MPDU_RDY_MASK) >> RXR0_MPDU_RDY_OFFSET;
}
static __inline uint32_t mac_rxr_sts_7_rxr1_mpdu_rdy_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_7_ADDR)) & RXR1_MPDU_RDY_MASK) >> RXR1_MPDU_RDY_OFFSET;
}
static __inline uint32_t mac_rxr_sts_7_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR_STS_7_ADDR));
}
static __inline void mac_rxr_sts_7_unpack(volatile uint32_t* rxr0_mpdu_rdy, volatile uint32_t* rxr1_mpdu_rdy)
{
	uint32_t tmp;

	tmp = mac_rxr_sts_7_get();
	*rxr0_mpdu_rdy = (tmp & ((uint32_t)RXR0_MPDU_RDY_MASK)) >> RXR0_MPDU_RDY_OFFSET;
	*rxr1_mpdu_rdy = (tmp & ((uint32_t)RXR1_MPDU_RDY_MASK)) >> RXR1_MPDU_RDY_OFFSET;
}
static __inline void mac_rxr0_ctrl_sw_rxr0_srst_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR);
	tmp &= ~SW_RXR0_SRST_MASK;
	tmp |= (value << SW_RXR0_SRST_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR), tmp);
}
static __inline void mac_rxr0_ctrl_sw_rxr0_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR);
	tmp &= ~SW_RXR0_EN_MASK;
	tmp |= (value << SW_RXR0_EN_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr0_ctrl_sw_rxr0_en_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR)) & SW_RXR0_EN_MASK) >> SW_RXR0_EN_OFFSET;
}
static __inline void mac_rxr0_ctrl_sw_rxr0_pp_buf_srst_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR);
	tmp &= ~SW_RXR0_PP_BUF_SRST_MASK;
	tmp |= (value << SW_RXR0_PP_BUF_SRST_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR), tmp);
}
static __inline void mac_rxr0_ctrl_sw_rxr0_ptr_srst_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR);
	tmp &= ~SW_RXR0_PTR_SRST_MASK;
	tmp |= (value << SW_RXR0_PTR_SRST_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR), tmp);
}
static __inline void mac_rxr0_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR), value);
}
static __inline uint32_t mac_rxr0_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR));
}
static __inline void mac_rxr0_ctrl_pack(uint32_t sw_rxr0_srst, uint32_t sw_rxr0_en, uint32_t sw_rxr0_pp_buf_srst, uint32_t sw_rxr0_ptr_srst)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr0_srst << SW_RXR0_SRST_OFFSET) & ~((uint32_t)SW_RXR0_SRST_MASK)) == 0);
	assert((((uint32_t)sw_rxr0_en << SW_RXR0_EN_OFFSET) & ~((uint32_t)SW_RXR0_EN_MASK)) == 0);
	assert((((uint32_t)sw_rxr0_pp_buf_srst << SW_RXR0_PP_BUF_SRST_OFFSET) & ~((uint32_t)SW_RXR0_PP_BUF_SRST_MASK)) == 0);
	assert((((uint32_t)sw_rxr0_ptr_srst << SW_RXR0_PTR_SRST_OFFSET) & ~((uint32_t)SW_RXR0_PTR_SRST_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr0_srst << SW_RXR0_SRST_OFFSET) |
		((uint32_t)sw_rxr0_en << SW_RXR0_EN_OFFSET) |
		((uint32_t)sw_rxr0_pp_buf_srst << SW_RXR0_PP_BUF_SRST_OFFSET) |
		((uint32_t)sw_rxr0_ptr_srst << SW_RXR0_PTR_SRST_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_CTRL_ADDR), tmp);
}
static __inline void mac_rxr0_ctrl_unpack(volatile uint32_t* sw_rxr0_en)
{
	uint32_t tmp;

	tmp = mac_rxr0_ctrl_get();
	*sw_rxr0_en = (tmp & ((uint32_t)SW_RXR0_EN_MASK)) >> SW_RXR0_EN_OFFSET;
}
static __inline void mac_rxr0_ptr_sw_rxr0_start_ptr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_PTR_ADDR);
	tmp &= ~SW_RXR0_START_PTR_MASK;
	tmp |= (value << SW_RXR0_START_PTR_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_PTR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_PTR_ADDR), tmp);
}
static __inline uint32_t mac_rxr0_ptr_sw_rxr0_start_ptr_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_PTR_ADDR)) & SW_RXR0_START_PTR_MASK) >> SW_RXR0_START_PTR_OFFSET;
}
static __inline void mac_rxr0_ptr_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_PTR_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_PTR_ADDR), value);
}
static __inline uint32_t mac_rxr0_ptr_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_PTR_ADDR));
}
static __inline void mac_rxr0_ptr_pack(uint32_t sw_rxr0_start_ptr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr0_start_ptr << SW_RXR0_START_PTR_OFFSET) & ~((uint32_t)SW_RXR0_START_PTR_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr0_start_ptr << SW_RXR0_START_PTR_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_PTR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_PTR_ADDR), tmp);
}
static __inline void mac_rxr0_ptr_unpack(volatile uint32_t* sw_rxr0_start_ptr)
{
	uint32_t tmp;

	tmp = mac_rxr0_ptr_get();
	*sw_rxr0_start_ptr = (tmp & ((uint32_t)SW_RXR0_START_PTR_MASK)) >> SW_RXR0_START_PTR_OFFSET;
}
static __inline void mac_rxr0_num_sw_rxr0_ptr_num_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_NUM_ADDR);
	tmp &= ~SW_RXR0_PTR_NUM_MASK;
	tmp |= (value << SW_RXR0_PTR_NUM_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_NUM_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_NUM_ADDR), tmp);
}
static __inline uint32_t mac_rxr0_num_sw_rxr0_ptr_num_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_NUM_ADDR)) & SW_RXR0_PTR_NUM_MASK) >> SW_RXR0_PTR_NUM_OFFSET;
}
static __inline void mac_rxr0_num_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_NUM_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_NUM_ADDR), value);
}
static __inline uint32_t mac_rxr0_num_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_NUM_ADDR));
}
static __inline void mac_rxr0_num_pack(uint32_t sw_rxr0_ptr_num)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr0_ptr_num << SW_RXR0_PTR_NUM_OFFSET) & ~((uint32_t)SW_RXR0_PTR_NUM_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr0_ptr_num << SW_RXR0_PTR_NUM_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_NUM_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_NUM_ADDR), tmp);
}
static __inline void mac_rxr0_num_unpack(volatile uint32_t* sw_rxr0_ptr_num)
{
	uint32_t tmp;

	tmp = mac_rxr0_num_get();
	*sw_rxr0_ptr_num = (tmp & ((uint32_t)SW_RXR0_PTR_NUM_MASK)) >> SW_RXR0_PTR_NUM_OFFSET;
}
static __inline void mac_rxr0_buf_len_sw_rxr0_rx_buf_len_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_BUF_LEN_ADDR);
	tmp &= ~SW_RXR0_RX_BUF_LEN_MASK;
	tmp |= (value << SW_RXR0_RX_BUF_LEN_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_BUF_LEN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_BUF_LEN_ADDR), tmp);
}
static __inline uint32_t mac_rxr0_buf_len_sw_rxr0_rx_buf_len_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_BUF_LEN_ADDR)) & SW_RXR0_RX_BUF_LEN_MASK) >> SW_RXR0_RX_BUF_LEN_OFFSET;
}
static __inline void mac_rxr0_buf_len_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_BUF_LEN_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_BUF_LEN_ADDR), value);
}
static __inline uint32_t mac_rxr0_buf_len_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_BUF_LEN_ADDR));
}
static __inline void mac_rxr0_buf_len_pack(uint32_t sw_rxr0_rx_buf_len)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr0_rx_buf_len << SW_RXR0_RX_BUF_LEN_OFFSET) & ~((uint32_t)SW_RXR0_RX_BUF_LEN_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr0_rx_buf_len << SW_RXR0_RX_BUF_LEN_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_BUF_LEN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_BUF_LEN_ADDR), tmp);
}
static __inline void mac_rxr0_buf_len_unpack(volatile uint32_t* sw_rxr0_rx_buf_len)
{
	uint32_t tmp;

	tmp = mac_rxr0_buf_len_get();
	*sw_rxr0_rx_buf_len = (tmp & ((uint32_t)SW_RXR0_RX_BUF_LEN_MASK)) >> SW_RXR0_RX_BUF_LEN_OFFSET;
}
static __inline void mac_rxr0_int_ctrl_sw_rxr0_mpdu_done_int_mitigation_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR);
	tmp &= ~SW_RXR0_MPDU_DONE_INT_MITIGATION_MASK;
	tmp |= (value << SW_RXR0_MPDU_DONE_INT_MITIGATION_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr0_int_ctrl_sw_rxr0_mpdu_done_int_mitigation_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR)) & SW_RXR0_MPDU_DONE_INT_MITIGATION_MASK) >> SW_RXR0_MPDU_DONE_INT_MITIGATION_OFFSET;
}
static __inline void mac_rxr0_int_ctrl_sw_rxr0_mpdu_done_int_timeout_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR);
	tmp &= ~SW_RXR0_MPDU_DONE_INT_TIMEOUT_MASK;
	tmp |= (value << SW_RXR0_MPDU_DONE_INT_TIMEOUT_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr0_int_ctrl_sw_rxr0_mpdu_done_int_timeout_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR)) & SW_RXR0_MPDU_DONE_INT_TIMEOUT_MASK) >> SW_RXR0_MPDU_DONE_INT_TIMEOUT_OFFSET;
}
static __inline void mac_rxr0_int_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR), value);
}
static __inline uint32_t mac_rxr0_int_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR));
}
static __inline void mac_rxr0_int_ctrl_pack(uint32_t sw_rxr0_mpdu_done_int_mitigation, uint32_t sw_rxr0_mpdu_done_int_timeout)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr0_mpdu_done_int_mitigation << SW_RXR0_MPDU_DONE_INT_MITIGATION_OFFSET) & ~((uint32_t)SW_RXR0_MPDU_DONE_INT_MITIGATION_MASK)) == 0);
	assert((((uint32_t)sw_rxr0_mpdu_done_int_timeout << SW_RXR0_MPDU_DONE_INT_TIMEOUT_OFFSET) & ~((uint32_t)SW_RXR0_MPDU_DONE_INT_TIMEOUT_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr0_mpdu_done_int_mitigation << SW_RXR0_MPDU_DONE_INT_MITIGATION_OFFSET) |
		((uint32_t)sw_rxr0_mpdu_done_int_timeout << SW_RXR0_MPDU_DONE_INT_TIMEOUT_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_INT_CTRL_ADDR), tmp);
}
static __inline void mac_rxr0_int_ctrl_unpack(volatile uint32_t* sw_rxr0_mpdu_done_int_mitigation, volatile uint32_t* sw_rxr0_mpdu_done_int_timeout)
{
	uint32_t tmp;

	tmp = mac_rxr0_int_ctrl_get();
	*sw_rxr0_mpdu_done_int_mitigation = (tmp & ((uint32_t)SW_RXR0_MPDU_DONE_INT_MITIGATION_MASK)) >> SW_RXR0_MPDU_DONE_INT_MITIGATION_OFFSET;
	*sw_rxr0_mpdu_done_int_timeout = (tmp & ((uint32_t)SW_RXR0_MPDU_DONE_INT_TIMEOUT_MASK)) >> SW_RXR0_MPDU_DONE_INT_TIMEOUT_OFFSET;
}
static __inline void mac_rxr0_dbg_ctrl_sw_rxr0_mpdu_rdy_force_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR);
	tmp &= ~SW_RXR0_MPDU_RDY_FORCE_EN_MASK;
	tmp |= (value << SW_RXR0_MPDU_RDY_FORCE_EN_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr0_dbg_ctrl_sw_rxr0_mpdu_rdy_force_en_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR)) & SW_RXR0_MPDU_RDY_FORCE_EN_MASK) >> SW_RXR0_MPDU_RDY_FORCE_EN_OFFSET;
}
static __inline void mac_rxr0_dbg_ctrl_sw_rxr0_mpdu_rdy_force_val_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR);
	tmp &= ~SW_RXR0_MPDU_RDY_FORCE_VAL_MASK;
	tmp |= (value << SW_RXR0_MPDU_RDY_FORCE_VAL_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr0_dbg_ctrl_sw_rxr0_mpdu_rdy_force_val_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR)) & SW_RXR0_MPDU_RDY_FORCE_VAL_MASK) >> SW_RXR0_MPDU_RDY_FORCE_VAL_OFFSET;
}
static __inline void mac_rxr0_dbg_ctrl_sw_rxr0_dbg_general_clr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR);
	tmp &= ~SW_RXR0_DBG_GENERAL_CLR_MASK;
	tmp |= (value << SW_RXR0_DBG_GENERAL_CLR_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR), tmp);
}
static __inline void mac_rxr0_dbg_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR), value);
}
static __inline uint32_t mac_rxr0_dbg_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR));
}
static __inline void mac_rxr0_dbg_ctrl_pack(uint32_t sw_rxr0_mpdu_rdy_force_en, uint32_t sw_rxr0_mpdu_rdy_force_val, uint32_t sw_rxr0_dbg_general_clr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr0_mpdu_rdy_force_en << SW_RXR0_MPDU_RDY_FORCE_EN_OFFSET) & ~((uint32_t)SW_RXR0_MPDU_RDY_FORCE_EN_MASK)) == 0);
	assert((((uint32_t)sw_rxr0_mpdu_rdy_force_val << SW_RXR0_MPDU_RDY_FORCE_VAL_OFFSET) & ~((uint32_t)SW_RXR0_MPDU_RDY_FORCE_VAL_MASK)) == 0);
	assert((((uint32_t)sw_rxr0_dbg_general_clr << SW_RXR0_DBG_GENERAL_CLR_OFFSET) & ~((uint32_t)SW_RXR0_DBG_GENERAL_CLR_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr0_mpdu_rdy_force_en << SW_RXR0_MPDU_RDY_FORCE_EN_OFFSET) |
		((uint32_t)sw_rxr0_mpdu_rdy_force_val << SW_RXR0_MPDU_RDY_FORCE_VAL_OFFSET) |
		((uint32_t)sw_rxr0_dbg_general_clr << SW_RXR0_DBG_GENERAL_CLR_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR0_DBG_CTRL_ADDR), tmp);
}
static __inline void mac_rxr0_dbg_ctrl_unpack(volatile uint32_t* sw_rxr0_mpdu_rdy_force_en, volatile uint32_t* sw_rxr0_mpdu_rdy_force_val)
{
	uint32_t tmp;

	tmp = mac_rxr0_dbg_ctrl_get();
	*sw_rxr0_mpdu_rdy_force_en = (tmp & ((uint32_t)SW_RXR0_MPDU_RDY_FORCE_EN_MASK)) >> SW_RXR0_MPDU_RDY_FORCE_EN_OFFSET;
	*sw_rxr0_mpdu_rdy_force_val = (tmp & ((uint32_t)SW_RXR0_MPDU_RDY_FORCE_VAL_MASK)) >> SW_RXR0_MPDU_RDY_FORCE_VAL_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_0_sw_ro_rxr0_rx_desc_ptr_is_zero_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_0_ADDR)) & SW_RO_RXR0_RX_DESC_PTR_IS_ZERO_MASK) >> SW_RO_RXR0_RX_DESC_PTR_IS_ZERO_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_0_sw_ro_rxr0_word_rxr_mpdu_rdy_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_0_ADDR)) & SW_RO_RXR0_WORD_RXR_MPDU_RDY_MASK) >> SW_RO_RXR0_WORD_RXR_MPDU_RDY_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_0_sw_ro_rxr0_rx_desc_access_fsm_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_0_ADDR)) & SW_RO_RXR0_RX_DESC_ACCESS_FSM_MASK) >> SW_RO_RXR0_RX_DESC_ACCESS_FSM_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_0_sw_ro_rxr0_rxr_mpdu_process_fsm_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_0_ADDR)) & SW_RO_RXR0_RXR_MPDU_PROCESS_FSM_MASK) >> SW_RO_RXR0_RXR_MPDU_PROCESS_FSM_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_0_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_0_ADDR));
}
static __inline void mac_rxr0_sts_0_unpack(volatile uint32_t* sw_ro_rxr0_rx_desc_ptr_is_zero, volatile uint32_t* sw_ro_rxr0_word_rxr_mpdu_rdy, volatile uint32_t* sw_ro_rxr0_rx_desc_access_fsm, volatile uint32_t* sw_ro_rxr0_rxr_mpdu_process_fsm)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_0_get();
	*sw_ro_rxr0_rx_desc_ptr_is_zero = (tmp & ((uint32_t)SW_RO_RXR0_RX_DESC_PTR_IS_ZERO_MASK)) >> SW_RO_RXR0_RX_DESC_PTR_IS_ZERO_OFFSET;
	*sw_ro_rxr0_word_rxr_mpdu_rdy = (tmp & ((uint32_t)SW_RO_RXR0_WORD_RXR_MPDU_RDY_MASK)) >> SW_RO_RXR0_WORD_RXR_MPDU_RDY_OFFSET;
	*sw_ro_rxr0_rx_desc_access_fsm = (tmp & ((uint32_t)SW_RO_RXR0_RX_DESC_ACCESS_FSM_MASK)) >> SW_RO_RXR0_RX_DESC_ACCESS_FSM_OFFSET;
	*sw_ro_rxr0_rxr_mpdu_process_fsm = (tmp & ((uint32_t)SW_RO_RXR0_RXR_MPDU_PROCESS_FSM_MASK)) >> SW_RO_RXR0_RXR_MPDU_PROCESS_FSM_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_1_sw_ro_rxr0_rd_rx_desc_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_1_ADDR)) & SW_RO_RXR0_RD_RX_DESC_DONE_CNT_MASK) >> SW_RO_RXR0_RD_RX_DESC_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_1_sw_ro_rxr0_rx_buf_fill_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_1_ADDR)) & SW_RO_RXR0_RX_BUF_FILL_DONE_CNT_MASK) >> SW_RO_RXR0_RX_BUF_FILL_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_1_sw_ro_rxr0_wb_rx_ctrl_other_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_1_ADDR)) & SW_RO_RXR0_WB_RX_CTRL_OTHER_DONE_CNT_MASK) >> SW_RO_RXR0_WB_RX_CTRL_OTHER_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_1_sw_ro_rxr0_wb_rx_ctrl0_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_1_ADDR)) & SW_RO_RXR0_WB_RX_CTRL0_DONE_CNT_MASK) >> SW_RO_RXR0_WB_RX_CTRL0_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_1_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_1_ADDR));
}
static __inline void mac_rxr0_sts_1_unpack(volatile uint32_t* sw_ro_rxr0_rd_rx_desc_done_cnt, volatile uint32_t* sw_ro_rxr0_rx_buf_fill_done_cnt, volatile uint32_t* sw_ro_rxr0_wb_rx_ctrl_other_done_cnt, volatile uint32_t* sw_ro_rxr0_wb_rx_ctrl0_done_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_1_get();
	*sw_ro_rxr0_rd_rx_desc_done_cnt = (tmp & ((uint32_t)SW_RO_RXR0_RD_RX_DESC_DONE_CNT_MASK)) >> SW_RO_RXR0_RD_RX_DESC_DONE_CNT_OFFSET;
	*sw_ro_rxr0_rx_buf_fill_done_cnt = (tmp & ((uint32_t)SW_RO_RXR0_RX_BUF_FILL_DONE_CNT_MASK)) >> SW_RO_RXR0_RX_BUF_FILL_DONE_CNT_OFFSET;
	*sw_ro_rxr0_wb_rx_ctrl_other_done_cnt = (tmp & ((uint32_t)SW_RO_RXR0_WB_RX_CTRL_OTHER_DONE_CNT_MASK)) >> SW_RO_RXR0_WB_RX_CTRL_OTHER_DONE_CNT_OFFSET;
	*sw_ro_rxr0_wb_rx_ctrl0_done_cnt = (tmp & ((uint32_t)SW_RO_RXR0_WB_RX_CTRL0_DONE_CNT_MASK)) >> SW_RO_RXR0_WB_RX_CTRL0_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_2_sw_ro_rxr0_dma_core_dbg_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_2_ADDR)) & SW_RO_RXR0_DMA_CORE_DBG_MASK) >> SW_RO_RXR0_DMA_CORE_DBG_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_2_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_2_ADDR));
}
static __inline void mac_rxr0_sts_2_unpack(volatile uint32_t* sw_ro_rxr0_dma_core_dbg)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_2_get();
	*sw_ro_rxr0_dma_core_dbg = (tmp & ((uint32_t)SW_RO_RXR0_DMA_CORE_DBG_MASK)) >> SW_RO_RXR0_DMA_CORE_DBG_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_3_sw_ro_rxr0_gen_int_mpdu_wb_ctrl_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_3_ADDR)) & SW_RO_RXR0_GEN_INT_MPDU_WB_CTRL_DONE_CNT_MASK) >> SW_RO_RXR0_GEN_INT_MPDU_WB_CTRL_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_3_sw_ro_rxr0_mpdu_fill_burst_data_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_3_ADDR)) & SW_RO_RXR0_MPDU_FILL_BURST_DATA_CNT_MASK) >> SW_RO_RXR0_MPDU_FILL_BURST_DATA_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_3_sw_ro_rxr0_mpdu_wb_ctrl_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_3_ADDR)) & SW_RO_RXR0_MPDU_WB_CTRL_DONE_CNT_MASK) >> SW_RO_RXR0_MPDU_WB_CTRL_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_3_sw_ro_rxr0_rx_desc_ptr_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_3_ADDR)) & SW_RO_RXR0_RX_DESC_PTR_CNT_MASK) >> SW_RO_RXR0_RX_DESC_PTR_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_3_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_3_ADDR));
}
static __inline void mac_rxr0_sts_3_unpack(volatile uint32_t* sw_ro_rxr0_gen_int_mpdu_wb_ctrl_done_cnt, volatile uint32_t* sw_ro_rxr0_mpdu_fill_burst_data_cnt, volatile uint32_t* sw_ro_rxr0_mpdu_wb_ctrl_done_cnt, volatile uint32_t* sw_ro_rxr0_rx_desc_ptr_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_3_get();
	*sw_ro_rxr0_gen_int_mpdu_wb_ctrl_done_cnt = (tmp & ((uint32_t)SW_RO_RXR0_GEN_INT_MPDU_WB_CTRL_DONE_CNT_MASK)) >> SW_RO_RXR0_GEN_INT_MPDU_WB_CTRL_DONE_CNT_OFFSET;
	*sw_ro_rxr0_mpdu_fill_burst_data_cnt = (tmp & ((uint32_t)SW_RO_RXR0_MPDU_FILL_BURST_DATA_CNT_MASK)) >> SW_RO_RXR0_MPDU_FILL_BURST_DATA_CNT_OFFSET;
	*sw_ro_rxr0_mpdu_wb_ctrl_done_cnt = (tmp & ((uint32_t)SW_RO_RXR0_MPDU_WB_CTRL_DONE_CNT_MASK)) >> SW_RO_RXR0_MPDU_WB_CTRL_DONE_CNT_OFFSET;
	*sw_ro_rxr0_rx_desc_ptr_cnt = (tmp & ((uint32_t)SW_RO_RXR0_RX_DESC_PTR_CNT_MASK)) >> SW_RO_RXR0_RX_DESC_PTR_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_4_sw_ro_rxr0_cur_rx_desc_ptr_ptr_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_4_ADDR)) & SW_RO_RXR0_CUR_RX_DESC_PTR_PTR_MASK) >> SW_RO_RXR0_CUR_RX_DESC_PTR_PTR_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_4_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_4_ADDR));
}
static __inline void mac_rxr0_sts_4_unpack(volatile uint32_t* sw_ro_rxr0_cur_rx_desc_ptr_ptr)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_4_get();
	*sw_ro_rxr0_cur_rx_desc_ptr_ptr = (tmp & ((uint32_t)SW_RO_RXR0_CUR_RX_DESC_PTR_PTR_MASK)) >> SW_RO_RXR0_CUR_RX_DESC_PTR_PTR_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_5_sw_ro_rxr0_cur_rx_desc_ptr_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_5_ADDR)) & SW_RO_RXR0_CUR_RX_DESC_PTR_MASK) >> SW_RO_RXR0_CUR_RX_DESC_PTR_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_5_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_5_ADDR));
}
static __inline void mac_rxr0_sts_5_unpack(volatile uint32_t* sw_ro_rxr0_cur_rx_desc_ptr)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_5_get();
	*sw_ro_rxr0_cur_rx_desc_ptr = (tmp & ((uint32_t)SW_RO_RXR0_CUR_RX_DESC_PTR_MASK)) >> SW_RO_RXR0_CUR_RX_DESC_PTR_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_6_sw_ro_rxr0_mpdu_fill_burst_ptr_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_6_ADDR)) & SW_RO_RXR0_MPDU_FILL_BURST_PTR_MASK) >> SW_RO_RXR0_MPDU_FILL_BURST_PTR_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_6_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_6_ADDR));
}
static __inline void mac_rxr0_sts_6_unpack(volatile uint32_t* sw_ro_rxr0_mpdu_fill_burst_ptr)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_6_get();
	*sw_ro_rxr0_mpdu_fill_burst_ptr = (tmp & ((uint32_t)SW_RO_RXR0_MPDU_FILL_BURST_PTR_MASK)) >> SW_RO_RXR0_MPDU_FILL_BURST_PTR_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_7_sw_ro_rxr0_rx_desc_data_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_7_ADDR)) & SW_RO_RXR0_RX_DESC_DATA_CNT_MASK) >> SW_RO_RXR0_RX_DESC_DATA_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_7_sw_ro_rxr0_rx_buf_fill_word_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_7_ADDR)) & SW_RO_RXR0_RX_BUF_FILL_WORD_CNT_MASK) >> SW_RO_RXR0_RX_BUF_FILL_WORD_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_7_sw_ro_rxr0_mpdu_fill_addr_offset_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_7_ADDR)) & SW_RO_RXR0_MPDU_FILL_ADDR_OFFSET_MASK) >> SW_RO_RXR0_MPDU_FILL_ADDR_OFFSET_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_7_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_7_ADDR));
}
static __inline void mac_rxr0_sts_7_unpack(volatile uint32_t* sw_ro_rxr0_rx_desc_data_cnt, volatile uint32_t* sw_ro_rxr0_rx_buf_fill_word_cnt, volatile uint32_t* sw_ro_rxr0_mpdu_fill_addr_offset)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_7_get();
	*sw_ro_rxr0_rx_desc_data_cnt = (tmp & ((uint32_t)SW_RO_RXR0_RX_DESC_DATA_CNT_MASK)) >> SW_RO_RXR0_RX_DESC_DATA_CNT_OFFSET;
	*sw_ro_rxr0_rx_buf_fill_word_cnt = (tmp & ((uint32_t)SW_RO_RXR0_RX_BUF_FILL_WORD_CNT_MASK)) >> SW_RO_RXR0_RX_BUF_FILL_WORD_CNT_OFFSET;
	*sw_ro_rxr0_mpdu_fill_addr_offset = (tmp & ((uint32_t)SW_RO_RXR0_MPDU_FILL_ADDR_OFFSET_MASK)) >> SW_RO_RXR0_MPDU_FILL_ADDR_OFFSET_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_8_sw_ro_rxr0_drop_mpdu_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_8_ADDR)) & SW_RO_RXR0_DROP_MPDU_CNT_MASK) >> SW_RO_RXR0_DROP_MPDU_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_8_sw_ro_rxr0_rd_rx_desc_ptr_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_8_ADDR)) & SW_RO_RXR0_RD_RX_DESC_PTR_DONE_CNT_MASK) >> SW_RO_RXR0_RD_RX_DESC_PTR_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_8_sw_ro_rxr0_mpdu_rx_desc_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_8_ADDR)) & SW_RO_RXR0_MPDU_RX_DESC_CNT_MASK) >> SW_RO_RXR0_MPDU_RX_DESC_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_8_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_8_ADDR));
}
static __inline void mac_rxr0_sts_8_unpack(volatile uint32_t* sw_ro_rxr0_drop_mpdu_cnt, volatile uint32_t* sw_ro_rxr0_rd_rx_desc_ptr_done_cnt, volatile uint32_t* sw_ro_rxr0_mpdu_rx_desc_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_8_get();
	*sw_ro_rxr0_drop_mpdu_cnt = (tmp & ((uint32_t)SW_RO_RXR0_DROP_MPDU_CNT_MASK)) >> SW_RO_RXR0_DROP_MPDU_CNT_OFFSET;
	*sw_ro_rxr0_rd_rx_desc_ptr_done_cnt = (tmp & ((uint32_t)SW_RO_RXR0_RD_RX_DESC_PTR_DONE_CNT_MASK)) >> SW_RO_RXR0_RD_RX_DESC_PTR_DONE_CNT_OFFSET;
	*sw_ro_rxr0_mpdu_rx_desc_cnt = (tmp & ((uint32_t)SW_RO_RXR0_MPDU_RX_DESC_CNT_MASK)) >> SW_RO_RXR0_MPDU_RX_DESC_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_9_sw_ro_rxr0_rxr_start_vector_0_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_9_ADDR)) & SW_RO_RXR0_RXR_START_VECTOR_0_MASK) >> SW_RO_RXR0_RXR_START_VECTOR_0_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_9_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_9_ADDR));
}
static __inline void mac_rxr0_sts_9_unpack(volatile uint32_t* sw_ro_rxr0_rxr_start_vector_0)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_9_get();
	*sw_ro_rxr0_rxr_start_vector_0 = (tmp & ((uint32_t)SW_RO_RXR0_RXR_START_VECTOR_0_MASK)) >> SW_RO_RXR0_RXR_START_VECTOR_0_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_10_sw_ro_rxr0_rxr_start_vector_1_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_10_ADDR)) & SW_RO_RXR0_RXR_START_VECTOR_1_MASK) >> SW_RO_RXR0_RXR_START_VECTOR_1_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_10_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_10_ADDR));
}
static __inline void mac_rxr0_sts_10_unpack(volatile uint32_t* sw_ro_rxr0_rxr_start_vector_1)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_10_get();
	*sw_ro_rxr0_rxr_start_vector_1 = (tmp & ((uint32_t)SW_RO_RXR0_RXR_START_VECTOR_1_MASK)) >> SW_RO_RXR0_RXR_START_VECTOR_1_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_11_sw_ro_rxr0_rxr_start_vector_2_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_11_ADDR)) & SW_RO_RXR0_RXR_START_VECTOR_2_MASK) >> SW_RO_RXR0_RXR_START_VECTOR_2_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_11_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_11_ADDR));
}
static __inline void mac_rxr0_sts_11_unpack(volatile uint32_t* sw_ro_rxr0_rxr_start_vector_2)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_11_get();
	*sw_ro_rxr0_rxr_start_vector_2 = (tmp & ((uint32_t)SW_RO_RXR0_RXR_START_VECTOR_2_MASK)) >> SW_RO_RXR0_RXR_START_VECTOR_2_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_12_sw_ro_rxr0_rxr_start_vector_3_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_12_ADDR)) & SW_RO_RXR0_RXR_START_VECTOR_3_MASK) >> SW_RO_RXR0_RXR_START_VECTOR_3_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_12_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_12_ADDR));
}
static __inline void mac_rxr0_sts_12_unpack(volatile uint32_t* sw_ro_rxr0_rxr_start_vector_3)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_12_get();
	*sw_ro_rxr0_rxr_start_vector_3 = (tmp & ((uint32_t)SW_RO_RXR0_RXR_START_VECTOR_3_MASK)) >> SW_RO_RXR0_RXR_START_VECTOR_3_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_13_sw_ro_rxr0_rxr_done_vector_0_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_13_ADDR)) & SW_RO_RXR0_RXR_DONE_VECTOR_0_MASK) >> SW_RO_RXR0_RXR_DONE_VECTOR_0_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_13_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_13_ADDR));
}
static __inline void mac_rxr0_sts_13_unpack(volatile uint32_t* sw_ro_rxr0_rxr_done_vector_0)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_13_get();
	*sw_ro_rxr0_rxr_done_vector_0 = (tmp & ((uint32_t)SW_RO_RXR0_RXR_DONE_VECTOR_0_MASK)) >> SW_RO_RXR0_RXR_DONE_VECTOR_0_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_14_sw_ro_rxr0_rxr_done_vector_1_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_14_ADDR)) & SW_RO_RXR0_RXR_DONE_VECTOR_1_MASK) >> SW_RO_RXR0_RXR_DONE_VECTOR_1_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_14_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_14_ADDR));
}
static __inline void mac_rxr0_sts_14_unpack(volatile uint32_t* sw_ro_rxr0_rxr_done_vector_1)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_14_get();
	*sw_ro_rxr0_rxr_done_vector_1 = (tmp & ((uint32_t)SW_RO_RXR0_RXR_DONE_VECTOR_1_MASK)) >> SW_RO_RXR0_RXR_DONE_VECTOR_1_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_15_sw_ro_rxr0_rxr_done_vector_2_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_15_ADDR)) & SW_RO_RXR0_RXR_DONE_VECTOR_2_MASK) >> SW_RO_RXR0_RXR_DONE_VECTOR_2_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_15_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_15_ADDR));
}
static __inline void mac_rxr0_sts_15_unpack(volatile uint32_t* sw_ro_rxr0_rxr_done_vector_2)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_15_get();
	*sw_ro_rxr0_rxr_done_vector_2 = (tmp & ((uint32_t)SW_RO_RXR0_RXR_DONE_VECTOR_2_MASK)) >> SW_RO_RXR0_RXR_DONE_VECTOR_2_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_16_sw_ro_rxr0_rxr_done_vector_3_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_16_ADDR)) & SW_RO_RXR0_RXR_DONE_VECTOR_3_MASK) >> SW_RO_RXR0_RXR_DONE_VECTOR_3_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_16_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_16_ADDR));
}
static __inline void mac_rxr0_sts_16_unpack(volatile uint32_t* sw_ro_rxr0_rxr_done_vector_3)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_16_get();
	*sw_ro_rxr0_rxr_done_vector_3 = (tmp & ((uint32_t)SW_RO_RXR0_RXR_DONE_VECTOR_3_MASK)) >> SW_RO_RXR0_RXR_DONE_VECTOR_3_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_17_sw_ro_rxr0_b2w_din_en_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_17_ADDR)) & SW_RO_RXR0_B2W_DIN_EN_CNT_MASK) >> SW_RO_RXR0_B2W_DIN_EN_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_17_sw_ro_rxr0_b2w_dout_en_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_17_ADDR)) & SW_RO_RXR0_B2W_DOUT_EN_CNT_MASK) >> SW_RO_RXR0_B2W_DOUT_EN_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_17_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_17_ADDR));
}
static __inline void mac_rxr0_sts_17_unpack(volatile uint32_t* sw_ro_rxr0_b2w_din_en_cnt, volatile uint32_t* sw_ro_rxr0_b2w_dout_en_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_17_get();
	*sw_ro_rxr0_b2w_din_en_cnt = (tmp & ((uint32_t)SW_RO_RXR0_B2W_DIN_EN_CNT_MASK)) >> SW_RO_RXR0_B2W_DIN_EN_CNT_OFFSET;
	*sw_ro_rxr0_b2w_dout_en_cnt = (tmp & ((uint32_t)SW_RO_RXR0_B2W_DOUT_EN_CNT_MASK)) >> SW_RO_RXR0_B2W_DOUT_EN_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_18_sw_ro_rxr0_burst_cmd_sts_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_18_ADDR)) & SW_RO_RXR0_BURST_CMD_STS_MASK) >> SW_RO_RXR0_BURST_CMD_STS_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_18_sw_ro_rxr0_fifo_burst_gen_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_18_ADDR)) & SW_RO_RXR0_FIFO_BURST_GEN_MASK) >> SW_RO_RXR0_FIFO_BURST_GEN_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_18_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_18_ADDR));
}
static __inline void mac_rxr0_sts_18_unpack(volatile uint32_t* sw_ro_rxr0_burst_cmd_sts, volatile uint32_t* sw_ro_rxr0_fifo_burst_gen)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_18_get();
	*sw_ro_rxr0_burst_cmd_sts = (tmp & ((uint32_t)SW_RO_RXR0_BURST_CMD_STS_MASK)) >> SW_RO_RXR0_BURST_CMD_STS_OFFSET;
	*sw_ro_rxr0_fifo_burst_gen = (tmp & ((uint32_t)SW_RO_RXR0_FIFO_BURST_GEN_MASK)) >> SW_RO_RXR0_FIFO_BURST_GEN_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_19_sw_ro_rxr0_burst_done_pls_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_19_ADDR)) & SW_RO_RXR0_BURST_DONE_PLS_CNT_MASK) >> SW_RO_RXR0_BURST_DONE_PLS_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_19_sw_ro_rxr0_burst_start_pls_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_19_ADDR)) & SW_RO_RXR0_BURST_START_PLS_CNT_MASK) >> SW_RO_RXR0_BURST_START_PLS_CNT_OFFSET;
}
static __inline uint32_t mac_rxr0_sts_19_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR0_STS_19_ADDR));
}
static __inline void mac_rxr0_sts_19_unpack(volatile uint32_t* sw_ro_rxr0_burst_done_pls_cnt, volatile uint32_t* sw_ro_rxr0_burst_start_pls_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr0_sts_19_get();
	*sw_ro_rxr0_burst_done_pls_cnt = (tmp & ((uint32_t)SW_RO_RXR0_BURST_DONE_PLS_CNT_MASK)) >> SW_RO_RXR0_BURST_DONE_PLS_CNT_OFFSET;
	*sw_ro_rxr0_burst_start_pls_cnt = (tmp & ((uint32_t)SW_RO_RXR0_BURST_START_PLS_CNT_MASK)) >> SW_RO_RXR0_BURST_START_PLS_CNT_OFFSET;
}
static __inline void mac_rxr1_ctrl_sw_rxr1_srst_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR);
	tmp &= ~SW_RXR1_SRST_MASK;
	tmp |= (value << SW_RXR1_SRST_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR), tmp);
}
static __inline void mac_rxr1_ctrl_sw_rxr1_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR);
	tmp &= ~SW_RXR1_EN_MASK;
	tmp |= (value << SW_RXR1_EN_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr1_ctrl_sw_rxr1_en_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR)) & SW_RXR1_EN_MASK) >> SW_RXR1_EN_OFFSET;
}
static __inline void mac_rxr1_ctrl_sw_rxr1_pp_buf_srst_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR);
	tmp &= ~SW_RXR1_PP_BUF_SRST_MASK;
	tmp |= (value << SW_RXR1_PP_BUF_SRST_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR), tmp);
}
static __inline void mac_rxr1_ctrl_sw_rxr1_ptr_srst_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR);
	tmp &= ~SW_RXR1_PTR_SRST_MASK;
	tmp |= (value << SW_RXR1_PTR_SRST_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR), tmp);
}
static __inline void mac_rxr1_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR), value);
}
static __inline uint32_t mac_rxr1_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR));
}
static __inline void mac_rxr1_ctrl_pack(uint32_t sw_rxr1_srst, uint32_t sw_rxr1_en, uint32_t sw_rxr1_pp_buf_srst, uint32_t sw_rxr1_ptr_srst)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr1_srst << SW_RXR1_SRST_OFFSET) & ~((uint32_t)SW_RXR1_SRST_MASK)) == 0);
	assert((((uint32_t)sw_rxr1_en << SW_RXR1_EN_OFFSET) & ~((uint32_t)SW_RXR1_EN_MASK)) == 0);
	assert((((uint32_t)sw_rxr1_pp_buf_srst << SW_RXR1_PP_BUF_SRST_OFFSET) & ~((uint32_t)SW_RXR1_PP_BUF_SRST_MASK)) == 0);
	assert((((uint32_t)sw_rxr1_ptr_srst << SW_RXR1_PTR_SRST_OFFSET) & ~((uint32_t)SW_RXR1_PTR_SRST_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr1_srst << SW_RXR1_SRST_OFFSET) |
		((uint32_t)sw_rxr1_en << SW_RXR1_EN_OFFSET) |
		((uint32_t)sw_rxr1_pp_buf_srst << SW_RXR1_PP_BUF_SRST_OFFSET) |
		((uint32_t)sw_rxr1_ptr_srst << SW_RXR1_PTR_SRST_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_CTRL_ADDR), tmp);
}
static __inline void mac_rxr1_ctrl_unpack(volatile uint32_t* sw_rxr1_en)
{
	uint32_t tmp;

	tmp = mac_rxr1_ctrl_get();
	*sw_rxr1_en = (tmp & ((uint32_t)SW_RXR1_EN_MASK)) >> SW_RXR1_EN_OFFSET;
}
static __inline void mac_rxr1_ptr_sw_rxr1_start_ptr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_PTR_ADDR);
	tmp &= ~SW_RXR1_START_PTR_MASK;
	tmp |= (value << SW_RXR1_START_PTR_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_PTR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_PTR_ADDR), tmp);
}
static __inline uint32_t mac_rxr1_ptr_sw_rxr1_start_ptr_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_PTR_ADDR)) & SW_RXR1_START_PTR_MASK) >> SW_RXR1_START_PTR_OFFSET;
}
static __inline void mac_rxr1_ptr_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_PTR_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_PTR_ADDR), value);
}
static __inline uint32_t mac_rxr1_ptr_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_PTR_ADDR));
}
static __inline void mac_rxr1_ptr_pack(uint32_t sw_rxr1_start_ptr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr1_start_ptr << SW_RXR1_START_PTR_OFFSET) & ~((uint32_t)SW_RXR1_START_PTR_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr1_start_ptr << SW_RXR1_START_PTR_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_PTR_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_PTR_ADDR), tmp);
}
static __inline void mac_rxr1_ptr_unpack(volatile uint32_t* sw_rxr1_start_ptr)
{
	uint32_t tmp;

	tmp = mac_rxr1_ptr_get();
	*sw_rxr1_start_ptr = (tmp & ((uint32_t)SW_RXR1_START_PTR_MASK)) >> SW_RXR1_START_PTR_OFFSET;
}
static __inline void mac_rxr1_num_sw_rxr1_ptr_num_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_NUM_ADDR);
	tmp &= ~SW_RXR1_PTR_NUM_MASK;
	tmp |= (value << SW_RXR1_PTR_NUM_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_NUM_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_NUM_ADDR), tmp);
}
static __inline uint32_t mac_rxr1_num_sw_rxr1_ptr_num_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_NUM_ADDR)) & SW_RXR1_PTR_NUM_MASK) >> SW_RXR1_PTR_NUM_OFFSET;
}
static __inline void mac_rxr1_num_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_NUM_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_NUM_ADDR), value);
}
static __inline uint32_t mac_rxr1_num_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_NUM_ADDR));
}
static __inline void mac_rxr1_num_pack(uint32_t sw_rxr1_ptr_num)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr1_ptr_num << SW_RXR1_PTR_NUM_OFFSET) & ~((uint32_t)SW_RXR1_PTR_NUM_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr1_ptr_num << SW_RXR1_PTR_NUM_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_NUM_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_NUM_ADDR), tmp);
}
static __inline void mac_rxr1_num_unpack(volatile uint32_t* sw_rxr1_ptr_num)
{
	uint32_t tmp;

	tmp = mac_rxr1_num_get();
	*sw_rxr1_ptr_num = (tmp & ((uint32_t)SW_RXR1_PTR_NUM_MASK)) >> SW_RXR1_PTR_NUM_OFFSET;
}
static __inline void mac_rxr1_buf_len_sw_rxr1_rx_buf_len_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_BUF_LEN_ADDR);
	tmp &= ~SW_RXR1_RX_BUF_LEN_MASK;
	tmp |= (value << SW_RXR1_RX_BUF_LEN_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_BUF_LEN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_BUF_LEN_ADDR), tmp);
}
static __inline uint32_t mac_rxr1_buf_len_sw_rxr1_rx_buf_len_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_BUF_LEN_ADDR)) & SW_RXR1_RX_BUF_LEN_MASK) >> SW_RXR1_RX_BUF_LEN_OFFSET;
}
static __inline void mac_rxr1_buf_len_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_BUF_LEN_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_BUF_LEN_ADDR), value);
}
static __inline uint32_t mac_rxr1_buf_len_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_BUF_LEN_ADDR));
}
static __inline void mac_rxr1_buf_len_pack(uint32_t sw_rxr1_rx_buf_len)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr1_rx_buf_len << SW_RXR1_RX_BUF_LEN_OFFSET) & ~((uint32_t)SW_RXR1_RX_BUF_LEN_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr1_rx_buf_len << SW_RXR1_RX_BUF_LEN_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_BUF_LEN_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_BUF_LEN_ADDR), tmp);
}
static __inline void mac_rxr1_buf_len_unpack(volatile uint32_t* sw_rxr1_rx_buf_len)
{
	uint32_t tmp;

	tmp = mac_rxr1_buf_len_get();
	*sw_rxr1_rx_buf_len = (tmp & ((uint32_t)SW_RXR1_RX_BUF_LEN_MASK)) >> SW_RXR1_RX_BUF_LEN_OFFSET;
}
static __inline void mac_rxr1_int_ctrl_sw_rxr1_mpdu_done_int_mitigation_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR);
	tmp &= ~SW_RXR1_MPDU_DONE_INT_MITIGATION_MASK;
	tmp |= (value << SW_RXR1_MPDU_DONE_INT_MITIGATION_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr1_int_ctrl_sw_rxr1_mpdu_done_int_mitigation_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR)) & SW_RXR1_MPDU_DONE_INT_MITIGATION_MASK) >> SW_RXR1_MPDU_DONE_INT_MITIGATION_OFFSET;
}
static __inline void mac_rxr1_int_ctrl_sw_rxr1_mpdu_done_int_timeout_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR);
	tmp &= ~SW_RXR1_MPDU_DONE_INT_TIMEOUT_MASK;
	tmp |= (value << SW_RXR1_MPDU_DONE_INT_TIMEOUT_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr1_int_ctrl_sw_rxr1_mpdu_done_int_timeout_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR)) & SW_RXR1_MPDU_DONE_INT_TIMEOUT_MASK) >> SW_RXR1_MPDU_DONE_INT_TIMEOUT_OFFSET;
}
static __inline void mac_rxr1_int_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR), value);
}
static __inline uint32_t mac_rxr1_int_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR));
}
static __inline void mac_rxr1_int_ctrl_pack(uint32_t sw_rxr1_mpdu_done_int_mitigation, uint32_t sw_rxr1_mpdu_done_int_timeout)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr1_mpdu_done_int_mitigation << SW_RXR1_MPDU_DONE_INT_MITIGATION_OFFSET) & ~((uint32_t)SW_RXR1_MPDU_DONE_INT_MITIGATION_MASK)) == 0);
	assert((((uint32_t)sw_rxr1_mpdu_done_int_timeout << SW_RXR1_MPDU_DONE_INT_TIMEOUT_OFFSET) & ~((uint32_t)SW_RXR1_MPDU_DONE_INT_TIMEOUT_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr1_mpdu_done_int_mitigation << SW_RXR1_MPDU_DONE_INT_MITIGATION_OFFSET) |
		((uint32_t)sw_rxr1_mpdu_done_int_timeout << SW_RXR1_MPDU_DONE_INT_TIMEOUT_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_INT_CTRL_ADDR), tmp);
}
static __inline void mac_rxr1_int_ctrl_unpack(volatile uint32_t* sw_rxr1_mpdu_done_int_mitigation, volatile uint32_t* sw_rxr1_mpdu_done_int_timeout)
{
	uint32_t tmp;

	tmp = mac_rxr1_int_ctrl_get();
	*sw_rxr1_mpdu_done_int_mitigation = (tmp & ((uint32_t)SW_RXR1_MPDU_DONE_INT_MITIGATION_MASK)) >> SW_RXR1_MPDU_DONE_INT_MITIGATION_OFFSET;
	*sw_rxr1_mpdu_done_int_timeout = (tmp & ((uint32_t)SW_RXR1_MPDU_DONE_INT_TIMEOUT_MASK)) >> SW_RXR1_MPDU_DONE_INT_TIMEOUT_OFFSET;
}
static __inline void mac_rxr1_dbg_ctrl_sw_rxr1_mpdu_rdy_force_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR);
	tmp &= ~SW_RXR1_MPDU_RDY_FORCE_EN_MASK;
	tmp |= (value << SW_RXR1_MPDU_RDY_FORCE_EN_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr1_dbg_ctrl_sw_rxr1_mpdu_rdy_force_en_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR)) & SW_RXR1_MPDU_RDY_FORCE_EN_MASK) >> SW_RXR1_MPDU_RDY_FORCE_EN_OFFSET;
}
static __inline void mac_rxr1_dbg_ctrl_sw_rxr1_mpdu_rdy_force_val_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR);
	tmp &= ~SW_RXR1_MPDU_RDY_FORCE_VAL_MASK;
	tmp |= (value << SW_RXR1_MPDU_RDY_FORCE_VAL_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_rxr1_dbg_ctrl_sw_rxr1_mpdu_rdy_force_val_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR)) & SW_RXR1_MPDU_RDY_FORCE_VAL_MASK) >> SW_RXR1_MPDU_RDY_FORCE_VAL_OFFSET;
}
static __inline void mac_rxr1_dbg_ctrl_sw_rxr1_dbg_general_clr_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR);
	tmp &= ~SW_RXR1_DBG_GENERAL_CLR_MASK;
	tmp |= (value << SW_RXR1_DBG_GENERAL_CLR_OFFSET);
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR), tmp);
}
static __inline void mac_rxr1_dbg_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR), value);
}
static __inline uint32_t mac_rxr1_dbg_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR));
}
static __inline void mac_rxr1_dbg_ctrl_pack(uint32_t sw_rxr1_mpdu_rdy_force_en, uint32_t sw_rxr1_mpdu_rdy_force_val, uint32_t sw_rxr1_dbg_general_clr)
{
	uint32_t tmp;

	assert((((uint32_t)sw_rxr1_mpdu_rdy_force_en << SW_RXR1_MPDU_RDY_FORCE_EN_OFFSET) & ~((uint32_t)SW_RXR1_MPDU_RDY_FORCE_EN_MASK)) == 0);
	assert((((uint32_t)sw_rxr1_mpdu_rdy_force_val << SW_RXR1_MPDU_RDY_FORCE_VAL_OFFSET) & ~((uint32_t)SW_RXR1_MPDU_RDY_FORCE_VAL_MASK)) == 0);
	assert((((uint32_t)sw_rxr1_dbg_general_clr << SW_RXR1_DBG_GENERAL_CLR_OFFSET) & ~((uint32_t)SW_RXR1_DBG_GENERAL_CLR_MASK)) == 0);
	tmp =
		((uint32_t)sw_rxr1_mpdu_rdy_force_en << SW_RXR1_MPDU_RDY_FORCE_EN_OFFSET) |
		((uint32_t)sw_rxr1_mpdu_rdy_force_val << SW_RXR1_MPDU_RDY_FORCE_VAL_OFFSET) |
		((uint32_t)sw_rxr1_dbg_general_clr << SW_RXR1_DBG_GENERAL_CLR_OFFSET) ;
	WQ_MAC_WRITE(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_RXR_BASEADDR + CFG_MAC_RXR1_DBG_CTRL_ADDR), tmp);
}
static __inline void mac_rxr1_dbg_ctrl_unpack(volatile uint32_t* sw_rxr1_mpdu_rdy_force_en, volatile uint32_t* sw_rxr1_mpdu_rdy_force_val)
{
	uint32_t tmp;

	tmp = mac_rxr1_dbg_ctrl_get();
	*sw_rxr1_mpdu_rdy_force_en = (tmp & ((uint32_t)SW_RXR1_MPDU_RDY_FORCE_EN_MASK)) >> SW_RXR1_MPDU_RDY_FORCE_EN_OFFSET;
	*sw_rxr1_mpdu_rdy_force_val = (tmp & ((uint32_t)SW_RXR1_MPDU_RDY_FORCE_VAL_MASK)) >> SW_RXR1_MPDU_RDY_FORCE_VAL_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_0_sw_ro_rxr1_rx_desc_ptr_is_zero_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_0_ADDR)) & SW_RO_RXR1_RX_DESC_PTR_IS_ZERO_MASK) >> SW_RO_RXR1_RX_DESC_PTR_IS_ZERO_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_0_sw_ro_rxr1_word_rxr_mpdu_rdy_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_0_ADDR)) & SW_RO_RXR1_WORD_RXR_MPDU_RDY_MASK) >> SW_RO_RXR1_WORD_RXR_MPDU_RDY_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_0_sw_ro_rxr1_rx_desc_access_fsm_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_0_ADDR)) & SW_RO_RXR1_RX_DESC_ACCESS_FSM_MASK) >> SW_RO_RXR1_RX_DESC_ACCESS_FSM_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_0_sw_ro_rxr1_rxr_mpdu_process_fsm_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_0_ADDR)) & SW_RO_RXR1_RXR_MPDU_PROCESS_FSM_MASK) >> SW_RO_RXR1_RXR_MPDU_PROCESS_FSM_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_0_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_0_ADDR));
}
static __inline void mac_rxr1_sts_0_unpack(volatile uint32_t* sw_ro_rxr1_rx_desc_ptr_is_zero, volatile uint32_t* sw_ro_rxr1_word_rxr_mpdu_rdy, volatile uint32_t* sw_ro_rxr1_rx_desc_access_fsm, volatile uint32_t* sw_ro_rxr1_rxr_mpdu_process_fsm)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_0_get();
	*sw_ro_rxr1_rx_desc_ptr_is_zero = (tmp & ((uint32_t)SW_RO_RXR1_RX_DESC_PTR_IS_ZERO_MASK)) >> SW_RO_RXR1_RX_DESC_PTR_IS_ZERO_OFFSET;
	*sw_ro_rxr1_word_rxr_mpdu_rdy = (tmp & ((uint32_t)SW_RO_RXR1_WORD_RXR_MPDU_RDY_MASK)) >> SW_RO_RXR1_WORD_RXR_MPDU_RDY_OFFSET;
	*sw_ro_rxr1_rx_desc_access_fsm = (tmp & ((uint32_t)SW_RO_RXR1_RX_DESC_ACCESS_FSM_MASK)) >> SW_RO_RXR1_RX_DESC_ACCESS_FSM_OFFSET;
	*sw_ro_rxr1_rxr_mpdu_process_fsm = (tmp & ((uint32_t)SW_RO_RXR1_RXR_MPDU_PROCESS_FSM_MASK)) >> SW_RO_RXR1_RXR_MPDU_PROCESS_FSM_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_1_sw_ro_rxr1_rd_rx_desc_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_1_ADDR)) & SW_RO_RXR1_RD_RX_DESC_DONE_CNT_MASK) >> SW_RO_RXR1_RD_RX_DESC_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_1_sw_ro_rxr1_rx_buf_fill_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_1_ADDR)) & SW_RO_RXR1_RX_BUF_FILL_DONE_CNT_MASK) >> SW_RO_RXR1_RX_BUF_FILL_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_1_sw_ro_rxr1_wb_rx_ctrl_other_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_1_ADDR)) & SW_RO_RXR1_WB_RX_CTRL_OTHER_DONE_CNT_MASK) >> SW_RO_RXR1_WB_RX_CTRL_OTHER_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_1_sw_ro_rxr1_wb_rx_ctrl0_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_1_ADDR)) & SW_RO_RXR1_WB_RX_CTRL0_DONE_CNT_MASK) >> SW_RO_RXR1_WB_RX_CTRL0_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_1_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_1_ADDR));
}
static __inline void mac_rxr1_sts_1_unpack(volatile uint32_t* sw_ro_rxr1_rd_rx_desc_done_cnt, volatile uint32_t* sw_ro_rxr1_rx_buf_fill_done_cnt, volatile uint32_t* sw_ro_rxr1_wb_rx_ctrl_other_done_cnt, volatile uint32_t* sw_ro_rxr1_wb_rx_ctrl0_done_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_1_get();
	*sw_ro_rxr1_rd_rx_desc_done_cnt = (tmp & ((uint32_t)SW_RO_RXR1_RD_RX_DESC_DONE_CNT_MASK)) >> SW_RO_RXR1_RD_RX_DESC_DONE_CNT_OFFSET;
	*sw_ro_rxr1_rx_buf_fill_done_cnt = (tmp & ((uint32_t)SW_RO_RXR1_RX_BUF_FILL_DONE_CNT_MASK)) >> SW_RO_RXR1_RX_BUF_FILL_DONE_CNT_OFFSET;
	*sw_ro_rxr1_wb_rx_ctrl_other_done_cnt = (tmp & ((uint32_t)SW_RO_RXR1_WB_RX_CTRL_OTHER_DONE_CNT_MASK)) >> SW_RO_RXR1_WB_RX_CTRL_OTHER_DONE_CNT_OFFSET;
	*sw_ro_rxr1_wb_rx_ctrl0_done_cnt = (tmp & ((uint32_t)SW_RO_RXR1_WB_RX_CTRL0_DONE_CNT_MASK)) >> SW_RO_RXR1_WB_RX_CTRL0_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_2_sw_ro_rxr1_dma_core_dbg_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_2_ADDR)) & SW_RO_RXR1_DMA_CORE_DBG_MASK) >> SW_RO_RXR1_DMA_CORE_DBG_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_2_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_2_ADDR));
}
static __inline void mac_rxr1_sts_2_unpack(volatile uint32_t* sw_ro_rxr1_dma_core_dbg)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_2_get();
	*sw_ro_rxr1_dma_core_dbg = (tmp & ((uint32_t)SW_RO_RXR1_DMA_CORE_DBG_MASK)) >> SW_RO_RXR1_DMA_CORE_DBG_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_3_sw_ro_rxr1_gen_int_mpdu_wb_ctrl_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_3_ADDR)) & SW_RO_RXR1_GEN_INT_MPDU_WB_CTRL_DONE_CNT_MASK) >> SW_RO_RXR1_GEN_INT_MPDU_WB_CTRL_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_3_sw_ro_rxr1_mpdu_fill_burst_data_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_3_ADDR)) & SW_RO_RXR1_MPDU_FILL_BURST_DATA_CNT_MASK) >> SW_RO_RXR1_MPDU_FILL_BURST_DATA_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_3_sw_ro_rxr1_mpdu_wb_ctrl_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_3_ADDR)) & SW_RO_RXR1_MPDU_WB_CTRL_DONE_CNT_MASK) >> SW_RO_RXR1_MPDU_WB_CTRL_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_3_sw_ro_rxr1_rx_desc_ptr_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_3_ADDR)) & SW_RO_RXR1_RX_DESC_PTR_CNT_MASK) >> SW_RO_RXR1_RX_DESC_PTR_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_3_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_3_ADDR));
}
static __inline void mac_rxr1_sts_3_unpack(volatile uint32_t* sw_ro_rxr1_gen_int_mpdu_wb_ctrl_done_cnt, volatile uint32_t* sw_ro_rxr1_mpdu_fill_burst_data_cnt, volatile uint32_t* sw_ro_rxr1_mpdu_wb_ctrl_done_cnt, volatile uint32_t* sw_ro_rxr1_rx_desc_ptr_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_3_get();
	*sw_ro_rxr1_gen_int_mpdu_wb_ctrl_done_cnt = (tmp & ((uint32_t)SW_RO_RXR1_GEN_INT_MPDU_WB_CTRL_DONE_CNT_MASK)) >> SW_RO_RXR1_GEN_INT_MPDU_WB_CTRL_DONE_CNT_OFFSET;
	*sw_ro_rxr1_mpdu_fill_burst_data_cnt = (tmp & ((uint32_t)SW_RO_RXR1_MPDU_FILL_BURST_DATA_CNT_MASK)) >> SW_RO_RXR1_MPDU_FILL_BURST_DATA_CNT_OFFSET;
	*sw_ro_rxr1_mpdu_wb_ctrl_done_cnt = (tmp & ((uint32_t)SW_RO_RXR1_MPDU_WB_CTRL_DONE_CNT_MASK)) >> SW_RO_RXR1_MPDU_WB_CTRL_DONE_CNT_OFFSET;
	*sw_ro_rxr1_rx_desc_ptr_cnt = (tmp & ((uint32_t)SW_RO_RXR1_RX_DESC_PTR_CNT_MASK)) >> SW_RO_RXR1_RX_DESC_PTR_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_4_sw_ro_rxr1_cur_rx_desc_ptr_ptr_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_4_ADDR)) & SW_RO_RXR1_CUR_RX_DESC_PTR_PTR_MASK) >> SW_RO_RXR1_CUR_RX_DESC_PTR_PTR_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_4_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_4_ADDR));
}
static __inline void mac_rxr1_sts_4_unpack(volatile uint32_t* sw_ro_rxr1_cur_rx_desc_ptr_ptr)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_4_get();
	*sw_ro_rxr1_cur_rx_desc_ptr_ptr = (tmp & ((uint32_t)SW_RO_RXR1_CUR_RX_DESC_PTR_PTR_MASK)) >> SW_RO_RXR1_CUR_RX_DESC_PTR_PTR_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_5_sw_ro_rxr1_cur_rx_desc_ptr_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_5_ADDR)) & SW_RO_RXR1_CUR_RX_DESC_PTR_MASK) >> SW_RO_RXR1_CUR_RX_DESC_PTR_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_5_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_5_ADDR));
}
static __inline void mac_rxr1_sts_5_unpack(volatile uint32_t* sw_ro_rxr1_cur_rx_desc_ptr)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_5_get();
	*sw_ro_rxr1_cur_rx_desc_ptr = (tmp & ((uint32_t)SW_RO_RXR1_CUR_RX_DESC_PTR_MASK)) >> SW_RO_RXR1_CUR_RX_DESC_PTR_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_6_sw_ro_rxr1_mpdu_fill_burst_ptr_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_6_ADDR)) & SW_RO_RXR1_MPDU_FILL_BURST_PTR_MASK) >> SW_RO_RXR1_MPDU_FILL_BURST_PTR_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_6_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_6_ADDR));
}
static __inline void mac_rxr1_sts_6_unpack(volatile uint32_t* sw_ro_rxr1_mpdu_fill_burst_ptr)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_6_get();
	*sw_ro_rxr1_mpdu_fill_burst_ptr = (tmp & ((uint32_t)SW_RO_RXR1_MPDU_FILL_BURST_PTR_MASK)) >> SW_RO_RXR1_MPDU_FILL_BURST_PTR_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_7_sw_ro_rxr1_rx_desc_data_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_7_ADDR)) & SW_RO_RXR1_RX_DESC_DATA_CNT_MASK) >> SW_RO_RXR1_RX_DESC_DATA_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_7_sw_ro_rxr1_rx_buf_fill_word_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_7_ADDR)) & SW_RO_RXR1_RX_BUF_FILL_WORD_CNT_MASK) >> SW_RO_RXR1_RX_BUF_FILL_WORD_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_7_sw_ro_rxr1_mpdu_fill_addr_offset_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_7_ADDR)) & SW_RO_RXR1_MPDU_FILL_ADDR_OFFSET_MASK) >> SW_RO_RXR1_MPDU_FILL_ADDR_OFFSET_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_7_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_7_ADDR));
}
static __inline void mac_rxr1_sts_7_unpack(volatile uint32_t* sw_ro_rxr1_rx_desc_data_cnt, volatile uint32_t* sw_ro_rxr1_rx_buf_fill_word_cnt, volatile uint32_t* sw_ro_rxr1_mpdu_fill_addr_offset)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_7_get();
	*sw_ro_rxr1_rx_desc_data_cnt = (tmp & ((uint32_t)SW_RO_RXR1_RX_DESC_DATA_CNT_MASK)) >> SW_RO_RXR1_RX_DESC_DATA_CNT_OFFSET;
	*sw_ro_rxr1_rx_buf_fill_word_cnt = (tmp & ((uint32_t)SW_RO_RXR1_RX_BUF_FILL_WORD_CNT_MASK)) >> SW_RO_RXR1_RX_BUF_FILL_WORD_CNT_OFFSET;
	*sw_ro_rxr1_mpdu_fill_addr_offset = (tmp & ((uint32_t)SW_RO_RXR1_MPDU_FILL_ADDR_OFFSET_MASK)) >> SW_RO_RXR1_MPDU_FILL_ADDR_OFFSET_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_8_sw_ro_rxr1_drop_mpdu_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_8_ADDR)) & SW_RO_RXR1_DROP_MPDU_CNT_MASK) >> SW_RO_RXR1_DROP_MPDU_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_8_sw_ro_rxr1_rd_rx_desc_ptr_done_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_8_ADDR)) & SW_RO_RXR1_RD_RX_DESC_PTR_DONE_CNT_MASK) >> SW_RO_RXR1_RD_RX_DESC_PTR_DONE_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_8_sw_ro_rxr1_mpdu_rx_desc_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_8_ADDR)) & SW_RO_RXR1_MPDU_RX_DESC_CNT_MASK) >> SW_RO_RXR1_MPDU_RX_DESC_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_8_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_8_ADDR));
}
static __inline void mac_rxr1_sts_8_unpack(volatile uint32_t* sw_ro_rxr1_drop_mpdu_cnt, volatile uint32_t* sw_ro_rxr1_rd_rx_desc_ptr_done_cnt, volatile uint32_t* sw_ro_rxr1_mpdu_rx_desc_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_8_get();
	*sw_ro_rxr1_drop_mpdu_cnt = (tmp & ((uint32_t)SW_RO_RXR1_DROP_MPDU_CNT_MASK)) >> SW_RO_RXR1_DROP_MPDU_CNT_OFFSET;
	*sw_ro_rxr1_rd_rx_desc_ptr_done_cnt = (tmp & ((uint32_t)SW_RO_RXR1_RD_RX_DESC_PTR_DONE_CNT_MASK)) >> SW_RO_RXR1_RD_RX_DESC_PTR_DONE_CNT_OFFSET;
	*sw_ro_rxr1_mpdu_rx_desc_cnt = (tmp & ((uint32_t)SW_RO_RXR1_MPDU_RX_DESC_CNT_MASK)) >> SW_RO_RXR1_MPDU_RX_DESC_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_9_sw_ro_rxr1_rxr_start_vector_0_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_9_ADDR)) & SW_RO_RXR1_RXR_START_VECTOR_0_MASK) >> SW_RO_RXR1_RXR_START_VECTOR_0_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_9_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_9_ADDR));
}
static __inline void mac_rxr1_sts_9_unpack(volatile uint32_t* sw_ro_rxr1_rxr_start_vector_0)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_9_get();
	*sw_ro_rxr1_rxr_start_vector_0 = (tmp & ((uint32_t)SW_RO_RXR1_RXR_START_VECTOR_0_MASK)) >> SW_RO_RXR1_RXR_START_VECTOR_0_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_10_sw_ro_rxr1_rxr_start_vector_1_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_10_ADDR)) & SW_RO_RXR1_RXR_START_VECTOR_1_MASK) >> SW_RO_RXR1_RXR_START_VECTOR_1_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_10_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_10_ADDR));
}
static __inline void mac_rxr1_sts_10_unpack(volatile uint32_t* sw_ro_rxr1_rxr_start_vector_1)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_10_get();
	*sw_ro_rxr1_rxr_start_vector_1 = (tmp & ((uint32_t)SW_RO_RXR1_RXR_START_VECTOR_1_MASK)) >> SW_RO_RXR1_RXR_START_VECTOR_1_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_11_sw_ro_rxr1_rxr_start_vector_2_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_11_ADDR)) & SW_RO_RXR1_RXR_START_VECTOR_2_MASK) >> SW_RO_RXR1_RXR_START_VECTOR_2_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_11_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_11_ADDR));
}
static __inline void mac_rxr1_sts_11_unpack(volatile uint32_t* sw_ro_rxr1_rxr_start_vector_2)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_11_get();
	*sw_ro_rxr1_rxr_start_vector_2 = (tmp & ((uint32_t)SW_RO_RXR1_RXR_START_VECTOR_2_MASK)) >> SW_RO_RXR1_RXR_START_VECTOR_2_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_12_sw_ro_rxr1_rxr_start_vector_3_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_12_ADDR)) & SW_RO_RXR1_RXR_START_VECTOR_3_MASK) >> SW_RO_RXR1_RXR_START_VECTOR_3_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_12_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_12_ADDR));
}
static __inline void mac_rxr1_sts_12_unpack(volatile uint32_t* sw_ro_rxr1_rxr_start_vector_3)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_12_get();
	*sw_ro_rxr1_rxr_start_vector_3 = (tmp & ((uint32_t)SW_RO_RXR1_RXR_START_VECTOR_3_MASK)) >> SW_RO_RXR1_RXR_START_VECTOR_3_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_13_sw_ro_rxr1_rxr_done_vector_0_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_13_ADDR)) & SW_RO_RXR1_RXR_DONE_VECTOR_0_MASK) >> SW_RO_RXR1_RXR_DONE_VECTOR_0_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_13_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_13_ADDR));
}
static __inline void mac_rxr1_sts_13_unpack(volatile uint32_t* sw_ro_rxr1_rxr_done_vector_0)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_13_get();
	*sw_ro_rxr1_rxr_done_vector_0 = (tmp & ((uint32_t)SW_RO_RXR1_RXR_DONE_VECTOR_0_MASK)) >> SW_RO_RXR1_RXR_DONE_VECTOR_0_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_14_sw_ro_rxr1_rxr_done_vector_1_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_14_ADDR)) & SW_RO_RXR1_RXR_DONE_VECTOR_1_MASK) >> SW_RO_RXR1_RXR_DONE_VECTOR_1_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_14_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_14_ADDR));
}
static __inline void mac_rxr1_sts_14_unpack(volatile uint32_t* sw_ro_rxr1_rxr_done_vector_1)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_14_get();
	*sw_ro_rxr1_rxr_done_vector_1 = (tmp & ((uint32_t)SW_RO_RXR1_RXR_DONE_VECTOR_1_MASK)) >> SW_RO_RXR1_RXR_DONE_VECTOR_1_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_15_sw_ro_rxr1_rxr_done_vector_2_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_15_ADDR)) & SW_RO_RXR1_RXR_DONE_VECTOR_2_MASK) >> SW_RO_RXR1_RXR_DONE_VECTOR_2_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_15_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_15_ADDR));
}
static __inline void mac_rxr1_sts_15_unpack(volatile uint32_t* sw_ro_rxr1_rxr_done_vector_2)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_15_get();
	*sw_ro_rxr1_rxr_done_vector_2 = (tmp & ((uint32_t)SW_RO_RXR1_RXR_DONE_VECTOR_2_MASK)) >> SW_RO_RXR1_RXR_DONE_VECTOR_2_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_16_sw_ro_rxr1_rxr_done_vector_3_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_16_ADDR)) & SW_RO_RXR1_RXR_DONE_VECTOR_3_MASK) >> SW_RO_RXR1_RXR_DONE_VECTOR_3_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_16_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_16_ADDR));
}
static __inline void mac_rxr1_sts_16_unpack(volatile uint32_t* sw_ro_rxr1_rxr_done_vector_3)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_16_get();
	*sw_ro_rxr1_rxr_done_vector_3 = (tmp & ((uint32_t)SW_RO_RXR1_RXR_DONE_VECTOR_3_MASK)) >> SW_RO_RXR1_RXR_DONE_VECTOR_3_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_17_sw_ro_rxr1_b2w_din_en_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_17_ADDR)) & SW_RO_RXR1_B2W_DIN_EN_CNT_MASK) >> SW_RO_RXR1_B2W_DIN_EN_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_17_sw_ro_rxr1_b2w_dout_en_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_17_ADDR)) & SW_RO_RXR1_B2W_DOUT_EN_CNT_MASK) >> SW_RO_RXR1_B2W_DOUT_EN_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_17_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_17_ADDR));
}
static __inline void mac_rxr1_sts_17_unpack(volatile uint32_t* sw_ro_rxr1_b2w_din_en_cnt, volatile uint32_t* sw_ro_rxr1_b2w_dout_en_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_17_get();
	*sw_ro_rxr1_b2w_din_en_cnt = (tmp & ((uint32_t)SW_RO_RXR1_B2W_DIN_EN_CNT_MASK)) >> SW_RO_RXR1_B2W_DIN_EN_CNT_OFFSET;
	*sw_ro_rxr1_b2w_dout_en_cnt = (tmp & ((uint32_t)SW_RO_RXR1_B2W_DOUT_EN_CNT_MASK)) >> SW_RO_RXR1_B2W_DOUT_EN_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_18_sw_ro_rxr1_burst_cmd_sts_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_18_ADDR)) & SW_RO_RXR1_BURST_CMD_STS_MASK) >> SW_RO_RXR1_BURST_CMD_STS_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_18_sw_ro_rxr1_fifo_burst_gen_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_18_ADDR)) & SW_RO_RXR1_FIFO_BURST_GEN_MASK) >> SW_RO_RXR1_FIFO_BURST_GEN_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_18_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_18_ADDR));
}
static __inline void mac_rxr1_sts_18_unpack(volatile uint32_t* sw_ro_rxr1_burst_cmd_sts, volatile uint32_t* sw_ro_rxr1_fifo_burst_gen)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_18_get();
	*sw_ro_rxr1_burst_cmd_sts = (tmp & ((uint32_t)SW_RO_RXR1_BURST_CMD_STS_MASK)) >> SW_RO_RXR1_BURST_CMD_STS_OFFSET;
	*sw_ro_rxr1_fifo_burst_gen = (tmp & ((uint32_t)SW_RO_RXR1_FIFO_BURST_GEN_MASK)) >> SW_RO_RXR1_FIFO_BURST_GEN_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_19_sw_ro_rxr1_burst_done_pls_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_19_ADDR)) & SW_RO_RXR1_BURST_DONE_PLS_CNT_MASK) >> SW_RO_RXR1_BURST_DONE_PLS_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_19_sw_ro_rxr1_burst_start_pls_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_19_ADDR)) & SW_RO_RXR1_BURST_START_PLS_CNT_MASK) >> SW_RO_RXR1_BURST_START_PLS_CNT_OFFSET;
}
static __inline uint32_t mac_rxr1_sts_19_get(void)
{
	return (WQ_MAC_READ(MAC_RXR_BASEADDR + CFG_MAC_RXR1_STS_19_ADDR));
}
static __inline void mac_rxr1_sts_19_unpack(volatile uint32_t* sw_ro_rxr1_burst_done_pls_cnt, volatile uint32_t* sw_ro_rxr1_burst_start_pls_cnt)
{
	uint32_t tmp;

	tmp = mac_rxr1_sts_19_get();
	*sw_ro_rxr1_burst_done_pls_cnt = (tmp & ((uint32_t)SW_RO_RXR1_BURST_DONE_PLS_CNT_MASK)) >> SW_RO_RXR1_BURST_DONE_PLS_CNT_OFFSET;
	*sw_ro_rxr1_burst_start_pls_cnt = (tmp & ((uint32_t)SW_RO_RXR1_BURST_START_PLS_CNT_MASK)) >> SW_RO_RXR1_BURST_START_PLS_CNT_OFFSET;
}






//-----------------------------------
#define CFG_MAC_TMR_DUMMY_ADDR 0x0000
#define SW_DUMMY_CHK_PERIOD_OFFSET 0
#define SW_DUMMY_CHK_PERIOD_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_NAV_SET_ADDR 0x0008
#define SW_NAV_FORCE_EN_OFFSET 16
#define SW_NAV_FORCE_EN_MASK 0x00010000
#define SW_NAV_FORCE_VAL_OFFSET 0
#define SW_NAV_FORCE_VAL_MASK 0x00007FFF

//-----------------------------------
#define CFG_MAC_MPI_TX_DLY_ADDR 0x000c
#define SW_MPI_TX_START_DLY_OFFSET 0
#define SW_MPI_TX_START_DLY_MASK 0x00007FFF

//-----------------------------------
#define CFG_MAC_FREE_TIMER_CTRL_ADDR 0x0020
#define SW_FREE_TIMER_PERIOD_OFFSET 0
#define SW_FREE_TIMER_PERIOD_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_FREE_TIMER_INT_CTRL_ADDR 0x0024
#define SW_FREE_TIMER_INT_TS_OFFSET 0
#define SW_FREE_TIMER_INT_TS_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_FREE_TIMER_STS_ADDR 0x0028
#define SW_RO_FREE_TIMER_OFFSET 0
#define SW_RO_FREE_TIMER_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_IFS_CTRL_ADDR 0x0030
#define SW_SIGNAL_EXTENSION_OFFSET 16
#define SW_SIGNAL_EXTENSION_MASK 0x00FF0000
#define SW_SLOT_OFFSET 8
#define SW_SLOT_MASK 0x0000FF00
#define SW_SIFS_OFFSET 0
#define SW_SIFS_MASK 0x000000FF

//-----------------------------------
#define CFG_MAC_NAV_TIMER_ADDR 0x0034
#define SW_RO_NAV_CNT_OFFSET 0
#define SW_RO_NAV_CNT_MASK 0x00007FFF

//-----------------------------------
#define CFG_MAC_TSF_0_VAL_HIGH_ADDR 0x0100
#define SW_TSF_0_VAL_HIGH_OFFSET 0
#define SW_TSF_0_VAL_HIGH_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_VAL_LOW_ADDR 0x0104
#define SW_TSF_0_VAL_LOW_OFFSET 0
#define SW_TSF_0_VAL_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_SET_HIGH_ADDR 0x0108
#define SW_TSF_0_SET_HIGH_OFFSET 0
#define SW_TSF_0_SET_HIGH_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_SET_LOW_ADDR 0x010c
#define SW_TSF_0_SET_LOW_OFFSET 0
#define SW_TSF_0_SET_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_SW_SET_ADDR 0x0110
#define SW_TSF_0_BCN_UPDATE_EN_OFFSET 2
#define SW_TSF_0_BCN_UPDATE_EN_MASK 0x00000004
#define SW_TSF_0_IMME_PLS_OFFSET 1
#define SW_TSF_0_IMME_PLS_MASK 0x00000002
#define SW_TSF_0_PRE_PLS_OFFSET 0
#define SW_TSF_0_PRE_PLS_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_TSF_0_READ_0_ADDR 0x0114
#define SW_RO_TSF_0_HIGH_OFFSET 0
#define SW_RO_TSF_0_HIGH_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_READ_1_ADDR 0x0118
#define SW_RO_TSF_0_LOW_OFFSET 0
#define SW_RO_TSF_0_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_PPM_CTRL_ADDR 0x011c
#define SW_PPM_0_OFFSET 0
#define SW_PPM_0_MASK 0x000FFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_PPM_STS_ADDR 0x0120
#define SW_RO_ETA_OUT_0_OFFSET 0
#define SW_RO_ETA_OUT_0_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR 0x0128
#define SW_TSF_0_BCN_RX_TS_DELTA_OFFSET 16
#define SW_TSF_0_BCN_RX_TS_DELTA_MASK 0xFFFF0000
#define SW_TSF_0_BCN_TX_TS_DELTA_OFFSET 0
#define SW_TSF_0_BCN_TX_TS_DELTA_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_TSF_0_TBTT_PERIOD_ADDR 0x0130
#define SW_TSF_0_TBTT_PERIOD_OFFSET 0
#define SW_TSF_0_TBTT_PERIOD_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_TBTT_DELTA_ADDR 0x0134
#define SW_TSF_0_TBTT_PRE_DELTA_OFFSET 16
#define SW_TSF_0_TBTT_PRE_DELTA_MASK 0xFFFF0000
#define SW_TSF_0_TBTT_POST_DELTA_OFFSET 0
#define SW_TSF_0_TBTT_POST_DELTA_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_TSF_0_TBTT_DIVIDE_STS_0_ADDR 0x0138
#define SW_RO_TSF_0_QUOTIENT_HIGH_OFFSET 0
#define SW_RO_TSF_0_QUOTIENT_HIGH_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_TBTT_DIVIDE_STS_1_ADDR 0x013c
#define SW_RO_TSF_0_QUOTIENT_LOW_OFFSET 0
#define SW_RO_TSF_0_QUOTIENT_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_TBTT_DIVIDE_STS_2_ADDR 0x0140
#define SW_RO_TSF_0_REMAINDER_OFFSET 0
#define SW_RO_TSF_0_REMAINDER_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_TBTT_DIVIDE_STS_3_ADDR 0x0144
#define SW_RO_TSF_0_DIVIDENT_R_HIGH_OFFSET 0
#define SW_RO_TSF_0_DIVIDENT_R_HIGH_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_TBTT_DIVIDE_STS_4_ADDR 0x0148
#define SW_RO_TSF_0_DIVIDENT_R_LOW_OFFSET 0
#define SW_RO_TSF_0_DIVIDENT_R_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_TBTT_DIVIDE_STS_5_ADDR 0x014c
#define SW_RO_TSF_0_DIVISOR_R_OFFSET 0
#define SW_RO_TSF_0_DIVISOR_R_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_0_TBTT_DIVIDE_STS_6_ADDR 0x0150
#define SW_RO_TSF_0_DIVIDENT31_OFFSET 0
#define SW_RO_TSF_0_DIVIDENT31_MASK 0x7FFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_VAL_HIGH_ADDR 0x0400
#define SW_TSF_1_VAL_HIGH_OFFSET 0
#define SW_TSF_1_VAL_HIGH_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_VAL_LOW_ADDR 0x0404
#define SW_TSF_1_VAL_LOW_OFFSET 0
#define SW_TSF_1_VAL_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_SET_HIGH_ADDR 0x0408
#define SW_TSF_1_SET_HIGH_OFFSET 0
#define SW_TSF_1_SET_HIGH_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_SET_LOW_ADDR 0x040c
#define SW_TSF_1_SET_LOW_OFFSET 0
#define SW_TSF_1_SET_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_SW_SET_ADDR 0x0410
#define SW_TSF_1_BCN_UPDATE_EN_OFFSET 2
#define SW_TSF_1_BCN_UPDATE_EN_MASK 0x00000004
#define SW_TSF_1_IMME_PLS_OFFSET 1
#define SW_TSF_1_IMME_PLS_MASK 0x00000002
#define SW_TSF_1_PRE_PLS_OFFSET 0
#define SW_TSF_1_PRE_PLS_MASK 0x00000001

//-----------------------------------
#define CFG_MAC_TSF_1_READ_0_ADDR 0x0414
#define SW_RO_TSF_1_HIGH_OFFSET 0
#define SW_RO_TSF_1_HIGH_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_READ_1_ADDR 0x0418
#define SW_RO_TSF_1_LOW_OFFSET 0
#define SW_RO_TSF_1_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_PPM_CTRL_ADDR 0x041c
#define SW_PPM_1_OFFSET 0
#define SW_PPM_1_MASK 0x000FFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_PPM_STS_ADDR 0x0420
#define SW_RO_ETA_OUT_1_OFFSET 0
#define SW_RO_ETA_OUT_1_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR 0x0424
#define SW_TSF_1_BCN_RX_TS_DELTA_OFFSET 16
#define SW_TSF_1_BCN_RX_TS_DELTA_MASK 0xFFFF0000
#define SW_TSF_1_BCN_TX_TS_DELTA_OFFSET 0
#define SW_TSF_1_BCN_TX_TS_DELTA_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_TSF_1_TBTT_PERIOD_ADDR 0x0430
#define SW_TSF_1_TBTT_PERIOD_OFFSET 0
#define SW_TSF_1_TBTT_PERIOD_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_TBTT_DELTA_ADDR 0x0434
#define SW_TSF_1_TBTT_PRE_DELTA_OFFSET 16
#define SW_TSF_1_TBTT_PRE_DELTA_MASK 0xFFFF0000
#define SW_TSF_1_TBTT_POST_DELTA_OFFSET 0
#define SW_TSF_1_TBTT_POST_DELTA_MASK 0x0000FFFF

//-----------------------------------
#define CFG_MAC_TSF_1_TBTT_DIVIDE_STS_0_ADDR 0x0438
#define SW_RO_TSF_1_QUOTIENT_HIGH_OFFSET 0
#define SW_RO_TSF_1_QUOTIENT_HIGH_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_TBTT_DIVIDE_STS_1_ADDR 0x043c
#define SW_RO_TSF_1_QUOTIENT_LOW_OFFSET 0
#define SW_RO_TSF_1_QUOTIENT_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_TBTT_DIVIDE_STS_2_ADDR 0x0440
#define SW_RO_TSF_1_REMAINDER_OFFSET 0
#define SW_RO_TSF_1_REMAINDER_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_TBTT_DIVIDE_STS_3_ADDR 0x0444
#define SW_RO_TSF_1_DIVIDENT_R_HIGH_OFFSET 0
#define SW_RO_TSF_1_DIVIDENT_R_HIGH_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_TBTT_DIVIDE_STS_4_ADDR 0x0448
#define SW_RO_TSF_1_DIVIDENT_R_LOW_OFFSET 0
#define SW_RO_TSF_1_DIVIDENT_R_LOW_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_TBTT_DIVIDE_STS_5_ADDR 0x044c
#define SW_RO_TSF_1_DIVISOR_R_OFFSET 0
#define SW_RO_TSF_1_DIVISOR_R_MASK 0xFFFFFFFF

//-----------------------------------
#define CFG_MAC_TSF_1_TBTT_DIVIDE_STS_6_ADDR 0x0450
#define SW_RO_TSF_1_DIVIDENT31_OFFSET 0
#define SW_RO_TSF_1_DIVIDENT31_MASK 0x7FFFFFFF

//HW module read/write macro




static __inline void mac_tmr_dummy_sw_dummy_chk_period_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TMR_DUMMY_ADDR);
	tmp &= ~SW_DUMMY_CHK_PERIOD_MASK;
	tmp |= (value << SW_DUMMY_CHK_PERIOD_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TMR_DUMMY_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TMR_DUMMY_ADDR), tmp);
}
static __inline uint32_t mac_tmr_dummy_sw_dummy_chk_period_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TMR_DUMMY_ADDR)) & SW_DUMMY_CHK_PERIOD_MASK) >> SW_DUMMY_CHK_PERIOD_OFFSET;
}
static __inline void mac_tmr_dummy_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TMR_DUMMY_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TMR_DUMMY_ADDR), value);
}
static __inline uint32_t mac_tmr_dummy_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TMR_DUMMY_ADDR));
}
static __inline void mac_tmr_dummy_pack(uint32_t sw_dummy_chk_period)
{
	uint32_t tmp;

	assert((((uint32_t)sw_dummy_chk_period << SW_DUMMY_CHK_PERIOD_OFFSET) & ~((uint32_t)SW_DUMMY_CHK_PERIOD_MASK)) == 0);
	tmp =
		((uint32_t)sw_dummy_chk_period << SW_DUMMY_CHK_PERIOD_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TMR_DUMMY_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TMR_DUMMY_ADDR), tmp);
}
static __inline void mac_tmr_dummy_unpack(volatile uint32_t* sw_dummy_chk_period)
{
	uint32_t tmp;

	tmp = mac_tmr_dummy_get();
	*sw_dummy_chk_period = (tmp & ((uint32_t)SW_DUMMY_CHK_PERIOD_MASK)) >> SW_DUMMY_CHK_PERIOD_OFFSET;
}
static __inline void mac_nav_set_sw_nav_force_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR);
	tmp &= ~SW_NAV_FORCE_EN_MASK;
	tmp |= (value << SW_NAV_FORCE_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR), tmp);
}
static __inline uint32_t mac_nav_set_sw_nav_force_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR)) & SW_NAV_FORCE_EN_MASK) >> SW_NAV_FORCE_EN_OFFSET;
}
static __inline void mac_nav_set_sw_nav_force_val_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR);
	tmp &= ~SW_NAV_FORCE_VAL_MASK;
	tmp |= (value << SW_NAV_FORCE_VAL_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR), tmp);
}
static __inline uint32_t mac_nav_set_sw_nav_force_val_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR)) & SW_NAV_FORCE_VAL_MASK) >> SW_NAV_FORCE_VAL_OFFSET;
}
static __inline void mac_nav_set_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR), value);
}
static __inline uint32_t mac_nav_set_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR));
}
static __inline void mac_nav_set_pack(uint32_t sw_nav_force_en, uint32_t sw_nav_force_val)
{
	uint32_t tmp;

	assert((((uint32_t)sw_nav_force_en << SW_NAV_FORCE_EN_OFFSET) & ~((uint32_t)SW_NAV_FORCE_EN_MASK)) == 0);
	assert((((uint32_t)sw_nav_force_val << SW_NAV_FORCE_VAL_OFFSET) & ~((uint32_t)SW_NAV_FORCE_VAL_MASK)) == 0);
	tmp =
		((uint32_t)sw_nav_force_en << SW_NAV_FORCE_EN_OFFSET) |
		((uint32_t)sw_nav_force_val << SW_NAV_FORCE_VAL_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_NAV_SET_ADDR), tmp);
}
static __inline void mac_nav_set_unpack(volatile uint32_t* sw_nav_force_en, volatile uint32_t* sw_nav_force_val)
{
	uint32_t tmp;

	tmp = mac_nav_set_get();
	*sw_nav_force_en = (tmp & ((uint32_t)SW_NAV_FORCE_EN_MASK)) >> SW_NAV_FORCE_EN_OFFSET;
	*sw_nav_force_val = (tmp & ((uint32_t)SW_NAV_FORCE_VAL_MASK)) >> SW_NAV_FORCE_VAL_OFFSET;
}
static __inline void mac_mpi_tx_dly_sw_mpi_tx_start_dly_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_MPI_TX_DLY_ADDR);
	tmp &= ~SW_MPI_TX_START_DLY_MASK;
	tmp |= (value << SW_MPI_TX_START_DLY_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_MPI_TX_DLY_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_MPI_TX_DLY_ADDR), tmp);
}
static __inline uint32_t mac_mpi_tx_dly_sw_mpi_tx_start_dly_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_MPI_TX_DLY_ADDR)) & SW_MPI_TX_START_DLY_MASK) >> SW_MPI_TX_START_DLY_OFFSET;
}
static __inline void mac_mpi_tx_dly_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_MPI_TX_DLY_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_MPI_TX_DLY_ADDR), value);
}
static __inline uint32_t mac_mpi_tx_dly_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_MPI_TX_DLY_ADDR));
}
static __inline void mac_mpi_tx_dly_pack(uint32_t sw_mpi_tx_start_dly)
{
	uint32_t tmp;

	assert((((uint32_t)sw_mpi_tx_start_dly << SW_MPI_TX_START_DLY_OFFSET) & ~((uint32_t)SW_MPI_TX_START_DLY_MASK)) == 0);
	tmp =
		((uint32_t)sw_mpi_tx_start_dly << SW_MPI_TX_START_DLY_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_MPI_TX_DLY_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_MPI_TX_DLY_ADDR), tmp);
}
static __inline void mac_mpi_tx_dly_unpack(volatile uint32_t* sw_mpi_tx_start_dly)
{
	uint32_t tmp;

	tmp = mac_mpi_tx_dly_get();
	*sw_mpi_tx_start_dly = (tmp & ((uint32_t)SW_MPI_TX_START_DLY_MASK)) >> SW_MPI_TX_START_DLY_OFFSET;
}
static __inline void mac_free_timer_ctrl_sw_free_timer_period_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_CTRL_ADDR);
	tmp &= ~SW_FREE_TIMER_PERIOD_MASK;
	tmp |= (value << SW_FREE_TIMER_PERIOD_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_free_timer_ctrl_sw_free_timer_period_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_CTRL_ADDR)) & SW_FREE_TIMER_PERIOD_MASK) >> SW_FREE_TIMER_PERIOD_OFFSET;
}
static __inline void mac_free_timer_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_CTRL_ADDR), value);
}
static __inline uint32_t mac_free_timer_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_CTRL_ADDR));
}
static __inline void mac_free_timer_ctrl_pack(uint32_t sw_free_timer_period)
{
	uint32_t tmp;

	assert((((uint32_t)sw_free_timer_period << SW_FREE_TIMER_PERIOD_OFFSET) & ~((uint32_t)SW_FREE_TIMER_PERIOD_MASK)) == 0);
	tmp =
		((uint32_t)sw_free_timer_period << SW_FREE_TIMER_PERIOD_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_CTRL_ADDR), tmp);
}
static __inline void mac_free_timer_ctrl_unpack(volatile uint32_t* sw_free_timer_period)
{
	uint32_t tmp;

	tmp = mac_free_timer_ctrl_get();
	*sw_free_timer_period = (tmp & ((uint32_t)SW_FREE_TIMER_PERIOD_MASK)) >> SW_FREE_TIMER_PERIOD_OFFSET;
}
static __inline void mac_free_timer_int_ctrl_sw_free_timer_int_ts_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_INT_CTRL_ADDR);
	tmp &= ~SW_FREE_TIMER_INT_TS_MASK;
	tmp |= (value << SW_FREE_TIMER_INT_TS_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_INT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_INT_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_free_timer_int_ctrl_sw_free_timer_int_ts_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_INT_CTRL_ADDR)) & SW_FREE_TIMER_INT_TS_MASK) >> SW_FREE_TIMER_INT_TS_OFFSET;
}
static __inline void mac_free_timer_int_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_INT_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_INT_CTRL_ADDR), value);
}
static __inline uint32_t mac_free_timer_int_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_INT_CTRL_ADDR));
}
static __inline void mac_free_timer_int_ctrl_pack(uint32_t sw_free_timer_int_ts)
{
	uint32_t tmp;

	assert((((uint32_t)sw_free_timer_int_ts << SW_FREE_TIMER_INT_TS_OFFSET) & ~((uint32_t)SW_FREE_TIMER_INT_TS_MASK)) == 0);
	tmp =
		((uint32_t)sw_free_timer_int_ts << SW_FREE_TIMER_INT_TS_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_INT_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_INT_CTRL_ADDR), tmp);
}
static __inline void mac_free_timer_int_ctrl_unpack(volatile uint32_t* sw_free_timer_int_ts)
{
	uint32_t tmp;

	tmp = mac_free_timer_int_ctrl_get();
	*sw_free_timer_int_ts = (tmp & ((uint32_t)SW_FREE_TIMER_INT_TS_MASK)) >> SW_FREE_TIMER_INT_TS_OFFSET;
}
static __inline uint32_t mac_free_timer_sts_sw_ro_free_timer_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_STS_ADDR)) & SW_RO_FREE_TIMER_MASK) >> SW_RO_FREE_TIMER_OFFSET;
}
static __inline uint32_t mac_free_timer_sts_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_FREE_TIMER_STS_ADDR));
}
static __inline void mac_free_timer_sts_unpack(volatile uint32_t* sw_ro_free_timer)
{
	uint32_t tmp;

	tmp = mac_free_timer_sts_get();
	*sw_ro_free_timer = (tmp & ((uint32_t)SW_RO_FREE_TIMER_MASK)) >> SW_RO_FREE_TIMER_OFFSET;
}
static __inline void mac_ifs_ctrl_sw_signal_extension_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR);
	tmp &= ~SW_SIGNAL_EXTENSION_MASK;
	tmp |= (value << SW_SIGNAL_EXTENSION_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_ifs_ctrl_sw_signal_extension_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR)) & SW_SIGNAL_EXTENSION_MASK) >> SW_SIGNAL_EXTENSION_OFFSET;
}
static __inline void mac_ifs_ctrl_sw_slot_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR);
	tmp &= ~SW_SLOT_MASK;
	tmp |= (value << SW_SLOT_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_ifs_ctrl_sw_slot_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR)) & SW_SLOT_MASK) >> SW_SLOT_OFFSET;
}
static __inline void mac_ifs_ctrl_sw_sifs_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR);
	tmp &= ~SW_SIFS_MASK;
	tmp |= (value << SW_SIFS_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_ifs_ctrl_sw_sifs_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR)) & SW_SIFS_MASK) >> SW_SIFS_OFFSET;
}
static __inline void mac_ifs_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR), value);
}
static __inline uint32_t mac_ifs_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR));
}
static __inline void mac_ifs_ctrl_pack(uint32_t sw_signal_extension, uint32_t sw_slot, uint32_t sw_sifs)
{
	uint32_t tmp;

	assert((((uint32_t)sw_signal_extension << SW_SIGNAL_EXTENSION_OFFSET) & ~((uint32_t)SW_SIGNAL_EXTENSION_MASK)) == 0);
	assert((((uint32_t)sw_slot << SW_SLOT_OFFSET) & ~((uint32_t)SW_SLOT_MASK)) == 0);
	assert((((uint32_t)sw_sifs << SW_SIFS_OFFSET) & ~((uint32_t)SW_SIFS_MASK)) == 0);
	tmp =
		((uint32_t)sw_signal_extension << SW_SIGNAL_EXTENSION_OFFSET) |
		((uint32_t)sw_slot << SW_SLOT_OFFSET) |
		((uint32_t)sw_sifs << SW_SIFS_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_IFS_CTRL_ADDR), tmp);
}
static __inline void mac_ifs_ctrl_unpack(volatile uint32_t* sw_signal_extension, volatile uint32_t* sw_slot, volatile uint32_t* sw_sifs)
{
	uint32_t tmp;

	tmp = mac_ifs_ctrl_get();
	*sw_signal_extension = (tmp & ((uint32_t)SW_SIGNAL_EXTENSION_MASK)) >> SW_SIGNAL_EXTENSION_OFFSET;
	*sw_slot = (tmp & ((uint32_t)SW_SLOT_MASK)) >> SW_SLOT_OFFSET;
	*sw_sifs = (tmp & ((uint32_t)SW_SIFS_MASK)) >> SW_SIFS_OFFSET;
}
static __inline uint32_t mac_nav_timer_sw_ro_nav_cnt_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_NAV_TIMER_ADDR)) & SW_RO_NAV_CNT_MASK) >> SW_RO_NAV_CNT_OFFSET;
}
static __inline uint32_t mac_nav_timer_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_NAV_TIMER_ADDR));
}
static __inline void mac_nav_timer_unpack(volatile uint32_t* sw_ro_nav_cnt)
{
	uint32_t tmp;

	tmp = mac_nav_timer_get();
	*sw_ro_nav_cnt = (tmp & ((uint32_t)SW_RO_NAV_CNT_MASK)) >> SW_RO_NAV_CNT_OFFSET;
}
static __inline void mac_tsf_0_val_high_sw_tsf_0_val_high_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_HIGH_ADDR);
	tmp &= ~SW_TSF_0_VAL_HIGH_MASK;
	tmp |= (value << SW_TSF_0_VAL_HIGH_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_HIGH_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_val_high_sw_tsf_0_val_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_HIGH_ADDR)) & SW_TSF_0_VAL_HIGH_MASK) >> SW_TSF_0_VAL_HIGH_OFFSET;
}
static __inline void mac_tsf_0_val_high_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_HIGH_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_HIGH_ADDR), value);
}
static __inline uint32_t mac_tsf_0_val_high_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_HIGH_ADDR));
}
static __inline void mac_tsf_0_val_high_pack(uint32_t sw_tsf_0_val_high)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_0_val_high << SW_TSF_0_VAL_HIGH_OFFSET) & ~((uint32_t)SW_TSF_0_VAL_HIGH_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_0_val_high << SW_TSF_0_VAL_HIGH_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_HIGH_ADDR), tmp);
}
static __inline void mac_tsf_0_val_high_unpack(volatile uint32_t* sw_tsf_0_val_high)
{
	uint32_t tmp;

	tmp = mac_tsf_0_val_high_get();
	*sw_tsf_0_val_high = (tmp & ((uint32_t)SW_TSF_0_VAL_HIGH_MASK)) >> SW_TSF_0_VAL_HIGH_OFFSET;
}
static __inline void mac_tsf_0_val_low_sw_tsf_0_val_low_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_LOW_ADDR);
	tmp &= ~SW_TSF_0_VAL_LOW_MASK;
	tmp |= (value << SW_TSF_0_VAL_LOW_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_LOW_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_val_low_sw_tsf_0_val_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_LOW_ADDR)) & SW_TSF_0_VAL_LOW_MASK) >> SW_TSF_0_VAL_LOW_OFFSET;
}
static __inline void mac_tsf_0_val_low_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_LOW_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_LOW_ADDR), value);
}
static __inline uint32_t mac_tsf_0_val_low_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_LOW_ADDR));
}
static __inline void mac_tsf_0_val_low_pack(uint32_t sw_tsf_0_val_low)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_0_val_low << SW_TSF_0_VAL_LOW_OFFSET) & ~((uint32_t)SW_TSF_0_VAL_LOW_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_0_val_low << SW_TSF_0_VAL_LOW_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_VAL_LOW_ADDR), tmp);
}
static __inline void mac_tsf_0_val_low_unpack(volatile uint32_t* sw_tsf_0_val_low)
{
	uint32_t tmp;

	tmp = mac_tsf_0_val_low_get();
	*sw_tsf_0_val_low = (tmp & ((uint32_t)SW_TSF_0_VAL_LOW_MASK)) >> SW_TSF_0_VAL_LOW_OFFSET;
}
static __inline void mac_tsf_0_set_high_sw_tsf_0_set_high_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_HIGH_ADDR);
	tmp &= ~SW_TSF_0_SET_HIGH_MASK;
	tmp |= (value << SW_TSF_0_SET_HIGH_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_HIGH_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_set_high_sw_tsf_0_set_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_HIGH_ADDR)) & SW_TSF_0_SET_HIGH_MASK) >> SW_TSF_0_SET_HIGH_OFFSET;
}
static __inline void mac_tsf_0_set_high_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_HIGH_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_HIGH_ADDR), value);
}
static __inline uint32_t mac_tsf_0_set_high_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_HIGH_ADDR));
}
static __inline void mac_tsf_0_set_high_pack(uint32_t sw_tsf_0_set_high)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_0_set_high << SW_TSF_0_SET_HIGH_OFFSET) & ~((uint32_t)SW_TSF_0_SET_HIGH_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_0_set_high << SW_TSF_0_SET_HIGH_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_HIGH_ADDR), tmp);
}
static __inline void mac_tsf_0_set_high_unpack(volatile uint32_t* sw_tsf_0_set_high)
{
	uint32_t tmp;

	tmp = mac_tsf_0_set_high_get();
	*sw_tsf_0_set_high = (tmp & ((uint32_t)SW_TSF_0_SET_HIGH_MASK)) >> SW_TSF_0_SET_HIGH_OFFSET;
}
static __inline void mac_tsf_0_set_low_sw_tsf_0_set_low_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_LOW_ADDR);
	tmp &= ~SW_TSF_0_SET_LOW_MASK;
	tmp |= (value << SW_TSF_0_SET_LOW_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_LOW_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_set_low_sw_tsf_0_set_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_LOW_ADDR)) & SW_TSF_0_SET_LOW_MASK) >> SW_TSF_0_SET_LOW_OFFSET;
}
static __inline void mac_tsf_0_set_low_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_LOW_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_LOW_ADDR), value);
}
static __inline uint32_t mac_tsf_0_set_low_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_LOW_ADDR));
}
static __inline void mac_tsf_0_set_low_pack(uint32_t sw_tsf_0_set_low)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_0_set_low << SW_TSF_0_SET_LOW_OFFSET) & ~((uint32_t)SW_TSF_0_SET_LOW_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_0_set_low << SW_TSF_0_SET_LOW_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SET_LOW_ADDR), tmp);
}
static __inline void mac_tsf_0_set_low_unpack(volatile uint32_t* sw_tsf_0_set_low)
{
	uint32_t tmp;

	tmp = mac_tsf_0_set_low_get();
	*sw_tsf_0_set_low = (tmp & ((uint32_t)SW_TSF_0_SET_LOW_MASK)) >> SW_TSF_0_SET_LOW_OFFSET;
}
static __inline void mac_tsf_0_sw_set_sw_tsf_0_bcn_update_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR);
	tmp &= ~SW_TSF_0_BCN_UPDATE_EN_MASK;
	tmp |= (value << SW_TSF_0_BCN_UPDATE_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_sw_set_sw_tsf_0_bcn_update_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR)) & SW_TSF_0_BCN_UPDATE_EN_MASK) >> SW_TSF_0_BCN_UPDATE_EN_OFFSET;
}
static __inline void mac_tsf_0_sw_set_sw_tsf_0_imme_pls_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR);
	tmp &= ~SW_TSF_0_IMME_PLS_MASK;
	tmp |= (value << SW_TSF_0_IMME_PLS_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR), tmp);
}
static __inline void mac_tsf_0_sw_set_sw_tsf_0_pre_pls_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR);
	tmp &= ~SW_TSF_0_PRE_PLS_MASK;
	tmp |= (value << SW_TSF_0_PRE_PLS_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR), tmp);
}
static __inline void mac_tsf_0_sw_set_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR), value);
}
static __inline uint32_t mac_tsf_0_sw_set_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR));
}
static __inline void mac_tsf_0_sw_set_pack(uint32_t sw_tsf_0_bcn_update_en, uint32_t sw_tsf_0_imme_pls, uint32_t sw_tsf_0_pre_pls)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_0_bcn_update_en << SW_TSF_0_BCN_UPDATE_EN_OFFSET) & ~((uint32_t)SW_TSF_0_BCN_UPDATE_EN_MASK)) == 0);
	assert((((uint32_t)sw_tsf_0_imme_pls << SW_TSF_0_IMME_PLS_OFFSET) & ~((uint32_t)SW_TSF_0_IMME_PLS_MASK)) == 0);
	assert((((uint32_t)sw_tsf_0_pre_pls << SW_TSF_0_PRE_PLS_OFFSET) & ~((uint32_t)SW_TSF_0_PRE_PLS_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_0_bcn_update_en << SW_TSF_0_BCN_UPDATE_EN_OFFSET) |
		((uint32_t)sw_tsf_0_imme_pls << SW_TSF_0_IMME_PLS_OFFSET) |
		((uint32_t)sw_tsf_0_pre_pls << SW_TSF_0_PRE_PLS_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_SW_SET_ADDR), tmp);
}
static __inline void mac_tsf_0_sw_set_unpack(volatile uint32_t* sw_tsf_0_bcn_update_en)
{
	uint32_t tmp;

	tmp = mac_tsf_0_sw_set_get();
	*sw_tsf_0_bcn_update_en = (tmp & ((uint32_t)SW_TSF_0_BCN_UPDATE_EN_MASK)) >> SW_TSF_0_BCN_UPDATE_EN_OFFSET;
}
static __inline uint32_t mac_tsf_0_read_0_sw_ro_tsf_0_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_READ_0_ADDR)) & SW_RO_TSF_0_HIGH_MASK) >> SW_RO_TSF_0_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_0_read_0_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_READ_0_ADDR));
}
static __inline void mac_tsf_0_read_0_unpack(volatile uint32_t* sw_ro_tsf_0_high)
{
	uint32_t tmp;

	tmp = mac_tsf_0_read_0_get();
	*sw_ro_tsf_0_high = (tmp & ((uint32_t)SW_RO_TSF_0_HIGH_MASK)) >> SW_RO_TSF_0_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_0_read_1_sw_ro_tsf_0_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_READ_1_ADDR)) & SW_RO_TSF_0_LOW_MASK) >> SW_RO_TSF_0_LOW_OFFSET;
}
static __inline uint32_t mac_tsf_0_read_1_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_READ_1_ADDR));
}
static __inline void mac_tsf_0_read_1_unpack(volatile uint32_t* sw_ro_tsf_0_low)
{
	uint32_t tmp;

	tmp = mac_tsf_0_read_1_get();
	*sw_ro_tsf_0_low = (tmp & ((uint32_t)SW_RO_TSF_0_LOW_MASK)) >> SW_RO_TSF_0_LOW_OFFSET;
}
static __inline void mac_tsf_0_ppm_ctrl_sw_ppm_0_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_CTRL_ADDR);
	tmp &= ~SW_PPM_0_MASK;
	tmp |= (value << SW_PPM_0_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_ppm_ctrl_sw_ppm_0_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_CTRL_ADDR)) & SW_PPM_0_MASK) >> SW_PPM_0_OFFSET;
}
static __inline void mac_tsf_0_ppm_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_CTRL_ADDR), value);
}
static __inline uint32_t mac_tsf_0_ppm_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_CTRL_ADDR));
}
static __inline void mac_tsf_0_ppm_ctrl_pack(uint32_t sw_ppm_0)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ppm_0 << SW_PPM_0_OFFSET) & ~((uint32_t)SW_PPM_0_MASK)) == 0);
	tmp =
		((uint32_t)sw_ppm_0 << SW_PPM_0_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_CTRL_ADDR), tmp);
}
static __inline void mac_tsf_0_ppm_ctrl_unpack(volatile uint32_t* sw_ppm_0)
{
	uint32_t tmp;

	tmp = mac_tsf_0_ppm_ctrl_get();
	*sw_ppm_0 = (tmp & ((uint32_t)SW_PPM_0_MASK)) >> SW_PPM_0_OFFSET;
}
static __inline uint32_t mac_tsf_0_ppm_sts_sw_ro_eta_out_0_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_STS_ADDR)) & SW_RO_ETA_OUT_0_MASK) >> SW_RO_ETA_OUT_0_OFFSET;
}
static __inline uint32_t mac_tsf_0_ppm_sts_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_PPM_STS_ADDR));
}
static __inline void mac_tsf_0_ppm_sts_unpack(volatile uint32_t* sw_ro_eta_out_0)
{
	uint32_t tmp;

	tmp = mac_tsf_0_ppm_sts_get();
	*sw_ro_eta_out_0 = (tmp & ((uint32_t)SW_RO_ETA_OUT_0_MASK)) >> SW_RO_ETA_OUT_0_OFFSET;
}
static __inline void mac_tsf_0_bcn_ts_delta_sw_tsf_0_bcn_rx_ts_delta_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR);
	tmp &= ~SW_TSF_0_BCN_RX_TS_DELTA_MASK;
	tmp |= (value << SW_TSF_0_BCN_RX_TS_DELTA_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_bcn_ts_delta_sw_tsf_0_bcn_rx_ts_delta_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR)) & SW_TSF_0_BCN_RX_TS_DELTA_MASK) >> SW_TSF_0_BCN_RX_TS_DELTA_OFFSET;
}
static __inline void mac_tsf_0_bcn_ts_delta_sw_tsf_0_bcn_tx_ts_delta_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR);
	tmp &= ~SW_TSF_0_BCN_TX_TS_DELTA_MASK;
	tmp |= (value << SW_TSF_0_BCN_TX_TS_DELTA_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_bcn_ts_delta_sw_tsf_0_bcn_tx_ts_delta_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR)) & SW_TSF_0_BCN_TX_TS_DELTA_MASK) >> SW_TSF_0_BCN_TX_TS_DELTA_OFFSET;
}
static __inline void mac_tsf_0_bcn_ts_delta_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR), value);
}
static __inline uint32_t mac_tsf_0_bcn_ts_delta_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR));
}
static __inline void mac_tsf_0_bcn_ts_delta_pack(uint32_t sw_tsf_0_bcn_rx_ts_delta, uint32_t sw_tsf_0_bcn_tx_ts_delta)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_0_bcn_rx_ts_delta << SW_TSF_0_BCN_RX_TS_DELTA_OFFSET) & ~((uint32_t)SW_TSF_0_BCN_RX_TS_DELTA_MASK)) == 0);
	assert((((uint32_t)sw_tsf_0_bcn_tx_ts_delta << SW_TSF_0_BCN_TX_TS_DELTA_OFFSET) & ~((uint32_t)SW_TSF_0_BCN_TX_TS_DELTA_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_0_bcn_rx_ts_delta << SW_TSF_0_BCN_RX_TS_DELTA_OFFSET) |
		((uint32_t)sw_tsf_0_bcn_tx_ts_delta << SW_TSF_0_BCN_TX_TS_DELTA_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_BCN_TS_DELTA_ADDR), tmp);
}
static __inline void mac_tsf_0_bcn_ts_delta_unpack(volatile uint32_t* sw_tsf_0_bcn_rx_ts_delta, volatile uint32_t* sw_tsf_0_bcn_tx_ts_delta)
{
	uint32_t tmp;

	tmp = mac_tsf_0_bcn_ts_delta_get();
	*sw_tsf_0_bcn_rx_ts_delta = (tmp & ((uint32_t)SW_TSF_0_BCN_RX_TS_DELTA_MASK)) >> SW_TSF_0_BCN_RX_TS_DELTA_OFFSET;
	*sw_tsf_0_bcn_tx_ts_delta = (tmp & ((uint32_t)SW_TSF_0_BCN_TX_TS_DELTA_MASK)) >> SW_TSF_0_BCN_TX_TS_DELTA_OFFSET;
}
static __inline void mac_tsf_0_tbtt_period_sw_tsf_0_tbtt_period_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_PERIOD_ADDR);
	tmp &= ~SW_TSF_0_TBTT_PERIOD_MASK;
	tmp |= (value << SW_TSF_0_TBTT_PERIOD_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_PERIOD_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_PERIOD_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_tbtt_period_sw_tsf_0_tbtt_period_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_PERIOD_ADDR)) & SW_TSF_0_TBTT_PERIOD_MASK) >> SW_TSF_0_TBTT_PERIOD_OFFSET;
}
static __inline void mac_tsf_0_tbtt_period_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_PERIOD_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_PERIOD_ADDR), value);
}
static __inline uint32_t mac_tsf_0_tbtt_period_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_PERIOD_ADDR));
}
static __inline void mac_tsf_0_tbtt_period_pack(uint32_t sw_tsf_0_tbtt_period)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_0_tbtt_period << SW_TSF_0_TBTT_PERIOD_OFFSET) & ~((uint32_t)SW_TSF_0_TBTT_PERIOD_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_0_tbtt_period << SW_TSF_0_TBTT_PERIOD_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_PERIOD_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_PERIOD_ADDR), tmp);
}
static __inline void mac_tsf_0_tbtt_period_unpack(volatile uint32_t* sw_tsf_0_tbtt_period)
{
	uint32_t tmp;

	tmp = mac_tsf_0_tbtt_period_get();
	*sw_tsf_0_tbtt_period = (tmp & ((uint32_t)SW_TSF_0_TBTT_PERIOD_MASK)) >> SW_TSF_0_TBTT_PERIOD_OFFSET;
}
static __inline void mac_tsf_0_tbtt_delta_sw_tsf_0_tbtt_pre_delta_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR);
	tmp &= ~SW_TSF_0_TBTT_PRE_DELTA_MASK;
	tmp |= (value << SW_TSF_0_TBTT_PRE_DELTA_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_tbtt_delta_sw_tsf_0_tbtt_pre_delta_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR)) & SW_TSF_0_TBTT_PRE_DELTA_MASK) >> SW_TSF_0_TBTT_PRE_DELTA_OFFSET;
}
static __inline void mac_tsf_0_tbtt_delta_sw_tsf_0_tbtt_post_delta_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR);
	tmp &= ~SW_TSF_0_TBTT_POST_DELTA_MASK;
	tmp |= (value << SW_TSF_0_TBTT_POST_DELTA_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR), tmp);
}
static __inline uint32_t mac_tsf_0_tbtt_delta_sw_tsf_0_tbtt_post_delta_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR)) & SW_TSF_0_TBTT_POST_DELTA_MASK) >> SW_TSF_0_TBTT_POST_DELTA_OFFSET;
}
static __inline void mac_tsf_0_tbtt_delta_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR), value);
}
static __inline uint32_t mac_tsf_0_tbtt_delta_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR));
}
static __inline void mac_tsf_0_tbtt_delta_pack(uint32_t sw_tsf_0_tbtt_pre_delta, uint32_t sw_tsf_0_tbtt_post_delta)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_0_tbtt_pre_delta << SW_TSF_0_TBTT_PRE_DELTA_OFFSET) & ~((uint32_t)SW_TSF_0_TBTT_PRE_DELTA_MASK)) == 0);
	assert((((uint32_t)sw_tsf_0_tbtt_post_delta << SW_TSF_0_TBTT_POST_DELTA_OFFSET) & ~((uint32_t)SW_TSF_0_TBTT_POST_DELTA_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_0_tbtt_pre_delta << SW_TSF_0_TBTT_PRE_DELTA_OFFSET) |
		((uint32_t)sw_tsf_0_tbtt_post_delta << SW_TSF_0_TBTT_POST_DELTA_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DELTA_ADDR), tmp);
}
static __inline void mac_tsf_0_tbtt_delta_unpack(volatile uint32_t* sw_tsf_0_tbtt_pre_delta, volatile uint32_t* sw_tsf_0_tbtt_post_delta)
{
	uint32_t tmp;

	tmp = mac_tsf_0_tbtt_delta_get();
	*sw_tsf_0_tbtt_pre_delta = (tmp & ((uint32_t)SW_TSF_0_TBTT_PRE_DELTA_MASK)) >> SW_TSF_0_TBTT_PRE_DELTA_OFFSET;
	*sw_tsf_0_tbtt_post_delta = (tmp & ((uint32_t)SW_TSF_0_TBTT_POST_DELTA_MASK)) >> SW_TSF_0_TBTT_POST_DELTA_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_0_sw_ro_tsf_0_quotient_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_0_ADDR)) & SW_RO_TSF_0_QUOTIENT_HIGH_MASK) >> SW_RO_TSF_0_QUOTIENT_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_0_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_0_ADDR));
}
static __inline void mac_tsf_0_tbtt_divide_sts_0_unpack(volatile uint32_t* sw_ro_tsf_0_quotient_high)
{
	uint32_t tmp;

	tmp = mac_tsf_0_tbtt_divide_sts_0_get();
	*sw_ro_tsf_0_quotient_high = (tmp & ((uint32_t)SW_RO_TSF_0_QUOTIENT_HIGH_MASK)) >> SW_RO_TSF_0_QUOTIENT_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_1_sw_ro_tsf_0_quotient_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_1_ADDR)) & SW_RO_TSF_0_QUOTIENT_LOW_MASK) >> SW_RO_TSF_0_QUOTIENT_LOW_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_1_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_1_ADDR));
}
static __inline void mac_tsf_0_tbtt_divide_sts_1_unpack(volatile uint32_t* sw_ro_tsf_0_quotient_low)
{
	uint32_t tmp;

	tmp = mac_tsf_0_tbtt_divide_sts_1_get();
	*sw_ro_tsf_0_quotient_low = (tmp & ((uint32_t)SW_RO_TSF_0_QUOTIENT_LOW_MASK)) >> SW_RO_TSF_0_QUOTIENT_LOW_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_2_sw_ro_tsf_0_remainder_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_2_ADDR)) & SW_RO_TSF_0_REMAINDER_MASK) >> SW_RO_TSF_0_REMAINDER_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_2_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_2_ADDR));
}
static __inline void mac_tsf_0_tbtt_divide_sts_2_unpack(volatile uint32_t* sw_ro_tsf_0_remainder)
{
	uint32_t tmp;

	tmp = mac_tsf_0_tbtt_divide_sts_2_get();
	*sw_ro_tsf_0_remainder = (tmp & ((uint32_t)SW_RO_TSF_0_REMAINDER_MASK)) >> SW_RO_TSF_0_REMAINDER_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_3_sw_ro_tsf_0_divident_r_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_3_ADDR)) & SW_RO_TSF_0_DIVIDENT_R_HIGH_MASK) >> SW_RO_TSF_0_DIVIDENT_R_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_3_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_3_ADDR));
}
static __inline void mac_tsf_0_tbtt_divide_sts_3_unpack(volatile uint32_t* sw_ro_tsf_0_divident_r_high)
{
	uint32_t tmp;

	tmp = mac_tsf_0_tbtt_divide_sts_3_get();
	*sw_ro_tsf_0_divident_r_high = (tmp & ((uint32_t)SW_RO_TSF_0_DIVIDENT_R_HIGH_MASK)) >> SW_RO_TSF_0_DIVIDENT_R_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_4_sw_ro_tsf_0_divident_r_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_4_ADDR)) & SW_RO_TSF_0_DIVIDENT_R_LOW_MASK) >> SW_RO_TSF_0_DIVIDENT_R_LOW_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_4_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_4_ADDR));
}
static __inline void mac_tsf_0_tbtt_divide_sts_4_unpack(volatile uint32_t* sw_ro_tsf_0_divident_r_low)
{
	uint32_t tmp;

	tmp = mac_tsf_0_tbtt_divide_sts_4_get();
	*sw_ro_tsf_0_divident_r_low = (tmp & ((uint32_t)SW_RO_TSF_0_DIVIDENT_R_LOW_MASK)) >> SW_RO_TSF_0_DIVIDENT_R_LOW_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_5_sw_ro_tsf_0_divisor_r_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_5_ADDR)) & SW_RO_TSF_0_DIVISOR_R_MASK) >> SW_RO_TSF_0_DIVISOR_R_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_5_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_5_ADDR));
}
static __inline void mac_tsf_0_tbtt_divide_sts_5_unpack(volatile uint32_t* sw_ro_tsf_0_divisor_r)
{
	uint32_t tmp;

	tmp = mac_tsf_0_tbtt_divide_sts_5_get();
	*sw_ro_tsf_0_divisor_r = (tmp & ((uint32_t)SW_RO_TSF_0_DIVISOR_R_MASK)) >> SW_RO_TSF_0_DIVISOR_R_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_6_sw_ro_tsf_0_divident31_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_6_ADDR)) & SW_RO_TSF_0_DIVIDENT31_MASK) >> SW_RO_TSF_0_DIVIDENT31_OFFSET;
}
static __inline uint32_t mac_tsf_0_tbtt_divide_sts_6_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_0_TBTT_DIVIDE_STS_6_ADDR));
}
static __inline void mac_tsf_0_tbtt_divide_sts_6_unpack(volatile uint32_t* sw_ro_tsf_0_divident31)
{
	uint32_t tmp;

	tmp = mac_tsf_0_tbtt_divide_sts_6_get();
	*sw_ro_tsf_0_divident31 = (tmp & ((uint32_t)SW_RO_TSF_0_DIVIDENT31_MASK)) >> SW_RO_TSF_0_DIVIDENT31_OFFSET;
}
static __inline void mac_tsf_1_val_high_sw_tsf_1_val_high_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_HIGH_ADDR);
	tmp &= ~SW_TSF_1_VAL_HIGH_MASK;
	tmp |= (value << SW_TSF_1_VAL_HIGH_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_HIGH_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_val_high_sw_tsf_1_val_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_HIGH_ADDR)) & SW_TSF_1_VAL_HIGH_MASK) >> SW_TSF_1_VAL_HIGH_OFFSET;
}
static __inline void mac_tsf_1_val_high_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_HIGH_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_HIGH_ADDR), value);
}
static __inline uint32_t mac_tsf_1_val_high_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_HIGH_ADDR));
}
static __inline void mac_tsf_1_val_high_pack(uint32_t sw_tsf_1_val_high)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_1_val_high << SW_TSF_1_VAL_HIGH_OFFSET) & ~((uint32_t)SW_TSF_1_VAL_HIGH_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_1_val_high << SW_TSF_1_VAL_HIGH_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_HIGH_ADDR), tmp);
}
static __inline void mac_tsf_1_val_high_unpack(volatile uint32_t* sw_tsf_1_val_high)
{
	uint32_t tmp;

	tmp = mac_tsf_1_val_high_get();
	*sw_tsf_1_val_high = (tmp & ((uint32_t)SW_TSF_1_VAL_HIGH_MASK)) >> SW_TSF_1_VAL_HIGH_OFFSET;
}
static __inline void mac_tsf_1_val_low_sw_tsf_1_val_low_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_LOW_ADDR);
	tmp &= ~SW_TSF_1_VAL_LOW_MASK;
	tmp |= (value << SW_TSF_1_VAL_LOW_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_LOW_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_val_low_sw_tsf_1_val_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_LOW_ADDR)) & SW_TSF_1_VAL_LOW_MASK) >> SW_TSF_1_VAL_LOW_OFFSET;
}
static __inline void mac_tsf_1_val_low_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_LOW_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_LOW_ADDR), value);
}
static __inline uint32_t mac_tsf_1_val_low_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_LOW_ADDR));
}
static __inline void mac_tsf_1_val_low_pack(uint32_t sw_tsf_1_val_low)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_1_val_low << SW_TSF_1_VAL_LOW_OFFSET) & ~((uint32_t)SW_TSF_1_VAL_LOW_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_1_val_low << SW_TSF_1_VAL_LOW_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_VAL_LOW_ADDR), tmp);
}
static __inline void mac_tsf_1_val_low_unpack(volatile uint32_t* sw_tsf_1_val_low)
{
	uint32_t tmp;

	tmp = mac_tsf_1_val_low_get();
	*sw_tsf_1_val_low = (tmp & ((uint32_t)SW_TSF_1_VAL_LOW_MASK)) >> SW_TSF_1_VAL_LOW_OFFSET;
}
static __inline void mac_tsf_1_set_high_sw_tsf_1_set_high_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_HIGH_ADDR);
	tmp &= ~SW_TSF_1_SET_HIGH_MASK;
	tmp |= (value << SW_TSF_1_SET_HIGH_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_HIGH_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_set_high_sw_tsf_1_set_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_HIGH_ADDR)) & SW_TSF_1_SET_HIGH_MASK) >> SW_TSF_1_SET_HIGH_OFFSET;
}
static __inline void mac_tsf_1_set_high_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_HIGH_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_HIGH_ADDR), value);
}
static __inline uint32_t mac_tsf_1_set_high_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_HIGH_ADDR));
}
static __inline void mac_tsf_1_set_high_pack(uint32_t sw_tsf_1_set_high)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_1_set_high << SW_TSF_1_SET_HIGH_OFFSET) & ~((uint32_t)SW_TSF_1_SET_HIGH_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_1_set_high << SW_TSF_1_SET_HIGH_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_HIGH_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_HIGH_ADDR), tmp);
}
static __inline void mac_tsf_1_set_high_unpack(volatile uint32_t* sw_tsf_1_set_high)
{
	uint32_t tmp;

	tmp = mac_tsf_1_set_high_get();
	*sw_tsf_1_set_high = (tmp & ((uint32_t)SW_TSF_1_SET_HIGH_MASK)) >> SW_TSF_1_SET_HIGH_OFFSET;
}
static __inline void mac_tsf_1_set_low_sw_tsf_1_set_low_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_LOW_ADDR);
	tmp &= ~SW_TSF_1_SET_LOW_MASK;
	tmp |= (value << SW_TSF_1_SET_LOW_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_LOW_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_set_low_sw_tsf_1_set_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_LOW_ADDR)) & SW_TSF_1_SET_LOW_MASK) >> SW_TSF_1_SET_LOW_OFFSET;
}
static __inline void mac_tsf_1_set_low_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_LOW_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_LOW_ADDR), value);
}
static __inline uint32_t mac_tsf_1_set_low_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_LOW_ADDR));
}
static __inline void mac_tsf_1_set_low_pack(uint32_t sw_tsf_1_set_low)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_1_set_low << SW_TSF_1_SET_LOW_OFFSET) & ~((uint32_t)SW_TSF_1_SET_LOW_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_1_set_low << SW_TSF_1_SET_LOW_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_LOW_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SET_LOW_ADDR), tmp);
}
static __inline void mac_tsf_1_set_low_unpack(volatile uint32_t* sw_tsf_1_set_low)
{
	uint32_t tmp;

	tmp = mac_tsf_1_set_low_get();
	*sw_tsf_1_set_low = (tmp & ((uint32_t)SW_TSF_1_SET_LOW_MASK)) >> SW_TSF_1_SET_LOW_OFFSET;
}
static __inline void mac_tsf_1_sw_set_sw_tsf_1_bcn_update_en_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR);
	tmp &= ~SW_TSF_1_BCN_UPDATE_EN_MASK;
	tmp |= (value << SW_TSF_1_BCN_UPDATE_EN_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_sw_set_sw_tsf_1_bcn_update_en_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR)) & SW_TSF_1_BCN_UPDATE_EN_MASK) >> SW_TSF_1_BCN_UPDATE_EN_OFFSET;
}
static __inline void mac_tsf_1_sw_set_sw_tsf_1_imme_pls_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR);
	tmp &= ~SW_TSF_1_IMME_PLS_MASK;
	tmp |= (value << SW_TSF_1_IMME_PLS_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR), tmp);
}
static __inline void mac_tsf_1_sw_set_sw_tsf_1_pre_pls_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR);
	tmp &= ~SW_TSF_1_PRE_PLS_MASK;
	tmp |= (value << SW_TSF_1_PRE_PLS_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR), tmp);
}
static __inline void mac_tsf_1_sw_set_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR), value);
}
static __inline uint32_t mac_tsf_1_sw_set_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR));
}
static __inline void mac_tsf_1_sw_set_pack(uint32_t sw_tsf_1_bcn_update_en, uint32_t sw_tsf_1_imme_pls, uint32_t sw_tsf_1_pre_pls)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_1_bcn_update_en << SW_TSF_1_BCN_UPDATE_EN_OFFSET) & ~((uint32_t)SW_TSF_1_BCN_UPDATE_EN_MASK)) == 0);
	assert((((uint32_t)sw_tsf_1_imme_pls << SW_TSF_1_IMME_PLS_OFFSET) & ~((uint32_t)SW_TSF_1_IMME_PLS_MASK)) == 0);
	assert((((uint32_t)sw_tsf_1_pre_pls << SW_TSF_1_PRE_PLS_OFFSET) & ~((uint32_t)SW_TSF_1_PRE_PLS_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_1_bcn_update_en << SW_TSF_1_BCN_UPDATE_EN_OFFSET) |
		((uint32_t)sw_tsf_1_imme_pls << SW_TSF_1_IMME_PLS_OFFSET) |
		((uint32_t)sw_tsf_1_pre_pls << SW_TSF_1_PRE_PLS_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_SW_SET_ADDR), tmp);
}
static __inline void mac_tsf_1_sw_set_unpack(volatile uint32_t* sw_tsf_1_bcn_update_en)
{
	uint32_t tmp;

	tmp = mac_tsf_1_sw_set_get();
	*sw_tsf_1_bcn_update_en = (tmp & ((uint32_t)SW_TSF_1_BCN_UPDATE_EN_MASK)) >> SW_TSF_1_BCN_UPDATE_EN_OFFSET;
}
static __inline uint32_t mac_tsf_1_read_0_sw_ro_tsf_1_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_READ_0_ADDR)) & SW_RO_TSF_1_HIGH_MASK) >> SW_RO_TSF_1_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_1_read_0_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_READ_0_ADDR));
}
static __inline void mac_tsf_1_read_0_unpack(volatile uint32_t* sw_ro_tsf_1_high)
{
	uint32_t tmp;

	tmp = mac_tsf_1_read_0_get();
	*sw_ro_tsf_1_high = (tmp & ((uint32_t)SW_RO_TSF_1_HIGH_MASK)) >> SW_RO_TSF_1_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_1_read_1_sw_ro_tsf_1_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_READ_1_ADDR)) & SW_RO_TSF_1_LOW_MASK) >> SW_RO_TSF_1_LOW_OFFSET;
}
static __inline uint32_t mac_tsf_1_read_1_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_READ_1_ADDR));
}
static __inline void mac_tsf_1_read_1_unpack(volatile uint32_t* sw_ro_tsf_1_low)
{
	uint32_t tmp;

	tmp = mac_tsf_1_read_1_get();
	*sw_ro_tsf_1_low = (tmp & ((uint32_t)SW_RO_TSF_1_LOW_MASK)) >> SW_RO_TSF_1_LOW_OFFSET;
}
static __inline void mac_tsf_1_ppm_ctrl_sw_ppm_1_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_CTRL_ADDR);
	tmp &= ~SW_PPM_1_MASK;
	tmp |= (value << SW_PPM_1_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_CTRL_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_ppm_ctrl_sw_ppm_1_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_CTRL_ADDR)) & SW_PPM_1_MASK) >> SW_PPM_1_OFFSET;
}
static __inline void mac_tsf_1_ppm_ctrl_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_CTRL_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_CTRL_ADDR), value);
}
static __inline uint32_t mac_tsf_1_ppm_ctrl_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_CTRL_ADDR));
}
static __inline void mac_tsf_1_ppm_ctrl_pack(uint32_t sw_ppm_1)
{
	uint32_t tmp;

	assert((((uint32_t)sw_ppm_1 << SW_PPM_1_OFFSET) & ~((uint32_t)SW_PPM_1_MASK)) == 0);
	tmp =
		((uint32_t)sw_ppm_1 << SW_PPM_1_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_CTRL_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_CTRL_ADDR), tmp);
}
static __inline void mac_tsf_1_ppm_ctrl_unpack(volatile uint32_t* sw_ppm_1)
{
	uint32_t tmp;

	tmp = mac_tsf_1_ppm_ctrl_get();
	*sw_ppm_1 = (tmp & ((uint32_t)SW_PPM_1_MASK)) >> SW_PPM_1_OFFSET;
}
static __inline uint32_t mac_tsf_1_ppm_sts_sw_ro_eta_out_1_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_STS_ADDR)) & SW_RO_ETA_OUT_1_MASK) >> SW_RO_ETA_OUT_1_OFFSET;
}
static __inline uint32_t mac_tsf_1_ppm_sts_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_PPM_STS_ADDR));
}
static __inline void mac_tsf_1_ppm_sts_unpack(volatile uint32_t* sw_ro_eta_out_1)
{
	uint32_t tmp;

	tmp = mac_tsf_1_ppm_sts_get();
	*sw_ro_eta_out_1 = (tmp & ((uint32_t)SW_RO_ETA_OUT_1_MASK)) >> SW_RO_ETA_OUT_1_OFFSET;
}
static __inline void mac_tsf_1_bcn_ts_delta_sw_tsf_1_bcn_rx_ts_delta_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR);
	tmp &= ~SW_TSF_1_BCN_RX_TS_DELTA_MASK;
	tmp |= (value << SW_TSF_1_BCN_RX_TS_DELTA_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_bcn_ts_delta_sw_tsf_1_bcn_rx_ts_delta_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR)) & SW_TSF_1_BCN_RX_TS_DELTA_MASK) >> SW_TSF_1_BCN_RX_TS_DELTA_OFFSET;
}
static __inline void mac_tsf_1_bcn_ts_delta_sw_tsf_1_bcn_tx_ts_delta_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR);
	tmp &= ~SW_TSF_1_BCN_TX_TS_DELTA_MASK;
	tmp |= (value << SW_TSF_1_BCN_TX_TS_DELTA_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_bcn_ts_delta_sw_tsf_1_bcn_tx_ts_delta_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR)) & SW_TSF_1_BCN_TX_TS_DELTA_MASK) >> SW_TSF_1_BCN_TX_TS_DELTA_OFFSET;
}
static __inline void mac_tsf_1_bcn_ts_delta_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR), value);
}
static __inline uint32_t mac_tsf_1_bcn_ts_delta_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR));
}
static __inline void mac_tsf_1_bcn_ts_delta_pack(uint32_t sw_tsf_1_bcn_rx_ts_delta, uint32_t sw_tsf_1_bcn_tx_ts_delta)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_1_bcn_rx_ts_delta << SW_TSF_1_BCN_RX_TS_DELTA_OFFSET) & ~((uint32_t)SW_TSF_1_BCN_RX_TS_DELTA_MASK)) == 0);
	assert((((uint32_t)sw_tsf_1_bcn_tx_ts_delta << SW_TSF_1_BCN_TX_TS_DELTA_OFFSET) & ~((uint32_t)SW_TSF_1_BCN_TX_TS_DELTA_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_1_bcn_rx_ts_delta << SW_TSF_1_BCN_RX_TS_DELTA_OFFSET) |
		((uint32_t)sw_tsf_1_bcn_tx_ts_delta << SW_TSF_1_BCN_TX_TS_DELTA_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_BCN_TS_DELTA_ADDR), tmp);
}
static __inline void mac_tsf_1_bcn_ts_delta_unpack(volatile uint32_t* sw_tsf_1_bcn_rx_ts_delta, volatile uint32_t* sw_tsf_1_bcn_tx_ts_delta)
{
	uint32_t tmp;

	tmp = mac_tsf_1_bcn_ts_delta_get();
	*sw_tsf_1_bcn_rx_ts_delta = (tmp & ((uint32_t)SW_TSF_1_BCN_RX_TS_DELTA_MASK)) >> SW_TSF_1_BCN_RX_TS_DELTA_OFFSET;
	*sw_tsf_1_bcn_tx_ts_delta = (tmp & ((uint32_t)SW_TSF_1_BCN_TX_TS_DELTA_MASK)) >> SW_TSF_1_BCN_TX_TS_DELTA_OFFSET;
}
static __inline void mac_tsf_1_tbtt_period_sw_tsf_1_tbtt_period_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_PERIOD_ADDR);
	tmp &= ~SW_TSF_1_TBTT_PERIOD_MASK;
	tmp |= (value << SW_TSF_1_TBTT_PERIOD_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_PERIOD_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_PERIOD_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_tbtt_period_sw_tsf_1_tbtt_period_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_PERIOD_ADDR)) & SW_TSF_1_TBTT_PERIOD_MASK) >> SW_TSF_1_TBTT_PERIOD_OFFSET;
}
static __inline void mac_tsf_1_tbtt_period_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_PERIOD_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_PERIOD_ADDR), value);
}
static __inline uint32_t mac_tsf_1_tbtt_period_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_PERIOD_ADDR));
}
static __inline void mac_tsf_1_tbtt_period_pack(uint32_t sw_tsf_1_tbtt_period)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_1_tbtt_period << SW_TSF_1_TBTT_PERIOD_OFFSET) & ~((uint32_t)SW_TSF_1_TBTT_PERIOD_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_1_tbtt_period << SW_TSF_1_TBTT_PERIOD_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_PERIOD_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_PERIOD_ADDR), tmp);
}
static __inline void mac_tsf_1_tbtt_period_unpack(volatile uint32_t* sw_tsf_1_tbtt_period)
{
	uint32_t tmp;

	tmp = mac_tsf_1_tbtt_period_get();
	*sw_tsf_1_tbtt_period = (tmp & ((uint32_t)SW_TSF_1_TBTT_PERIOD_MASK)) >> SW_TSF_1_TBTT_PERIOD_OFFSET;
}
static __inline void mac_tsf_1_tbtt_delta_sw_tsf_1_tbtt_pre_delta_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR);
	tmp &= ~SW_TSF_1_TBTT_PRE_DELTA_MASK;
	tmp |= (value << SW_TSF_1_TBTT_PRE_DELTA_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_tbtt_delta_sw_tsf_1_tbtt_pre_delta_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR)) & SW_TSF_1_TBTT_PRE_DELTA_MASK) >> SW_TSF_1_TBTT_PRE_DELTA_OFFSET;
}
static __inline void mac_tsf_1_tbtt_delta_sw_tsf_1_tbtt_post_delta_set(uint32_t value)
{
	uint32_t tmp;

	tmp  = WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR);
	tmp &= ~SW_TSF_1_TBTT_POST_DELTA_MASK;
	tmp |= (value << SW_TSF_1_TBTT_POST_DELTA_OFFSET);
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR), tmp);
}
static __inline uint32_t mac_tsf_1_tbtt_delta_sw_tsf_1_tbtt_post_delta_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR)) & SW_TSF_1_TBTT_POST_DELTA_MASK) >> SW_TSF_1_TBTT_POST_DELTA_OFFSET;
}
static __inline void mac_tsf_1_tbtt_delta_set(uint32_t value)
{
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR, value);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR), value);
}
static __inline uint32_t mac_tsf_1_tbtt_delta_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR));
}
static __inline void mac_tsf_1_tbtt_delta_pack(uint32_t sw_tsf_1_tbtt_pre_delta, uint32_t sw_tsf_1_tbtt_post_delta)
{
	uint32_t tmp;

	assert((((uint32_t)sw_tsf_1_tbtt_pre_delta << SW_TSF_1_TBTT_PRE_DELTA_OFFSET) & ~((uint32_t)SW_TSF_1_TBTT_PRE_DELTA_MASK)) == 0);
	assert((((uint32_t)sw_tsf_1_tbtt_post_delta << SW_TSF_1_TBTT_POST_DELTA_OFFSET) & ~((uint32_t)SW_TSF_1_TBTT_POST_DELTA_MASK)) == 0);
	tmp =
		((uint32_t)sw_tsf_1_tbtt_pre_delta << SW_TSF_1_TBTT_PRE_DELTA_OFFSET) |
		((uint32_t)sw_tsf_1_tbtt_post_delta << SW_TSF_1_TBTT_POST_DELTA_OFFSET) ;
	WQ_MAC_WRITE(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR, tmp);
	MAC_WRITE_LOG(__func__,(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DELTA_ADDR), tmp);
}
static __inline void mac_tsf_1_tbtt_delta_unpack(volatile uint32_t* sw_tsf_1_tbtt_pre_delta, volatile uint32_t* sw_tsf_1_tbtt_post_delta)
{
	uint32_t tmp;

	tmp = mac_tsf_1_tbtt_delta_get();
	*sw_tsf_1_tbtt_pre_delta = (tmp & ((uint32_t)SW_TSF_1_TBTT_PRE_DELTA_MASK)) >> SW_TSF_1_TBTT_PRE_DELTA_OFFSET;
	*sw_tsf_1_tbtt_post_delta = (tmp & ((uint32_t)SW_TSF_1_TBTT_POST_DELTA_MASK)) >> SW_TSF_1_TBTT_POST_DELTA_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_0_sw_ro_tsf_1_quotient_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_0_ADDR)) & SW_RO_TSF_1_QUOTIENT_HIGH_MASK) >> SW_RO_TSF_1_QUOTIENT_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_0_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_0_ADDR));
}
static __inline void mac_tsf_1_tbtt_divide_sts_0_unpack(volatile uint32_t* sw_ro_tsf_1_quotient_high)
{
	uint32_t tmp;

	tmp = mac_tsf_1_tbtt_divide_sts_0_get();
	*sw_ro_tsf_1_quotient_high = (tmp & ((uint32_t)SW_RO_TSF_1_QUOTIENT_HIGH_MASK)) >> SW_RO_TSF_1_QUOTIENT_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_1_sw_ro_tsf_1_quotient_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_1_ADDR)) & SW_RO_TSF_1_QUOTIENT_LOW_MASK) >> SW_RO_TSF_1_QUOTIENT_LOW_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_1_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_1_ADDR));
}
static __inline void mac_tsf_1_tbtt_divide_sts_1_unpack(volatile uint32_t* sw_ro_tsf_1_quotient_low)
{
	uint32_t tmp;

	tmp = mac_tsf_1_tbtt_divide_sts_1_get();
	*sw_ro_tsf_1_quotient_low = (tmp & ((uint32_t)SW_RO_TSF_1_QUOTIENT_LOW_MASK)) >> SW_RO_TSF_1_QUOTIENT_LOW_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_2_sw_ro_tsf_1_remainder_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_2_ADDR)) & SW_RO_TSF_1_REMAINDER_MASK) >> SW_RO_TSF_1_REMAINDER_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_2_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_2_ADDR));
}
static __inline void mac_tsf_1_tbtt_divide_sts_2_unpack(volatile uint32_t* sw_ro_tsf_1_remainder)
{
	uint32_t tmp;

	tmp = mac_tsf_1_tbtt_divide_sts_2_get();
	*sw_ro_tsf_1_remainder = (tmp & ((uint32_t)SW_RO_TSF_1_REMAINDER_MASK)) >> SW_RO_TSF_1_REMAINDER_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_3_sw_ro_tsf_1_divident_r_high_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_3_ADDR)) & SW_RO_TSF_1_DIVIDENT_R_HIGH_MASK) >> SW_RO_TSF_1_DIVIDENT_R_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_3_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_3_ADDR));
}
static __inline void mac_tsf_1_tbtt_divide_sts_3_unpack(volatile uint32_t* sw_ro_tsf_1_divident_r_high)
{
	uint32_t tmp;

	tmp = mac_tsf_1_tbtt_divide_sts_3_get();
	*sw_ro_tsf_1_divident_r_high = (tmp & ((uint32_t)SW_RO_TSF_1_DIVIDENT_R_HIGH_MASK)) >> SW_RO_TSF_1_DIVIDENT_R_HIGH_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_4_sw_ro_tsf_1_divident_r_low_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_4_ADDR)) & SW_RO_TSF_1_DIVIDENT_R_LOW_MASK) >> SW_RO_TSF_1_DIVIDENT_R_LOW_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_4_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_4_ADDR));
}
static __inline void mac_tsf_1_tbtt_divide_sts_4_unpack(volatile uint32_t* sw_ro_tsf_1_divident_r_low)
{
	uint32_t tmp;

	tmp = mac_tsf_1_tbtt_divide_sts_4_get();
	*sw_ro_tsf_1_divident_r_low = (tmp & ((uint32_t)SW_RO_TSF_1_DIVIDENT_R_LOW_MASK)) >> SW_RO_TSF_1_DIVIDENT_R_LOW_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_5_sw_ro_tsf_1_divisor_r_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_5_ADDR)) & SW_RO_TSF_1_DIVISOR_R_MASK) >> SW_RO_TSF_1_DIVISOR_R_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_5_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_5_ADDR));
}
static __inline void mac_tsf_1_tbtt_divide_sts_5_unpack(volatile uint32_t* sw_ro_tsf_1_divisor_r)
{
	uint32_t tmp;

	tmp = mac_tsf_1_tbtt_divide_sts_5_get();
	*sw_ro_tsf_1_divisor_r = (tmp & ((uint32_t)SW_RO_TSF_1_DIVISOR_R_MASK)) >> SW_RO_TSF_1_DIVISOR_R_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_6_sw_ro_tsf_1_divident31_get(void)
{
	return ((WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_6_ADDR)) & SW_RO_TSF_1_DIVIDENT31_MASK) >> SW_RO_TSF_1_DIVIDENT31_OFFSET;
}
static __inline uint32_t mac_tsf_1_tbtt_divide_sts_6_get(void)
{
	return (WQ_MAC_READ(MAC_TMR_BASEADDR + CFG_MAC_TSF_1_TBTT_DIVIDE_STS_6_ADDR));
}
static __inline void mac_tsf_1_tbtt_divide_sts_6_unpack(volatile uint32_t* sw_ro_tsf_1_divident31)
{
	uint32_t tmp;

	tmp = mac_tsf_1_tbtt_divide_sts_6_get();
	*sw_ro_tsf_1_divident31 = (tmp & ((uint32_t)SW_RO_TSF_1_DIVIDENT31_MASK)) >> SW_RO_TSF_1_DIVIDENT31_OFFSET;
}





#ifndef IF_WQ_PORT
#define IF_WQ_PORT

#include "sdk.h"
#include "oss.h"
#include "taskq.h"



#ifndef HAWK_RELEASE_BUILD
#define WQ_DEBUG   1
#endif

enum {
    WDRV_DEBUG_XMIT      = 0x00000001,   /* basic xmit operation */
    WDRV_DEBUG_XMIT_DESC = 0x00000002,   /* xmit descriptors */
    WDRV_DEBUG_RECV      = 0x00000004,   /* basic recv operation */
    WDRV_DEBUG_RECV_DESC = 0x00000008,   /* recv descriptors */
    WDRV_DEBUG_RESET     = 0x00000010,   /* reset processing */
    WDRV_DEBUG_BEACON    = 0x00000020,   /* beacon handling */
    WDRV_DEBUG_INTR      = 0x00000040,   /* ISR */
    WDRV_DEBUG_TX_PROC   = 0x00000080,   /* tx ISR proc */
    WDRV_DEBUG_RX_PROC   = 0x00000100,   /* rx ISR proc */
    WDRV_DEBUG_KEYCACHE  = 0x00000200,   /* key cache management */
    WDRV_DEBUG_STATE     = 0x00000400,   /* 802.11 state transitions */
    WDRV_DEBUG_NODE      = 0x00000800,   /* node management */
    WDRV_DEBUG_RECV_ALL  = 0x00001000,   /* trace all frames (beacons) */
    WDRV_DEBUG_RATE      = 0x00002000,   /* rate */
    WDRV_DEBUG_AMPDU     = 0x00004000,   /* BA stream handling */
    WDRV_DEBUG_HALTX     = 0x00008000,   /* HAL tx */
    WDRV_DEBUG_HALRX     = 0x00010000,   /* HAL rx */
    WDRV_DEBUG_SIMTX     = 0x00020000,   /* sim tx */
    WDRV_DEBUG_SIMRX     = 0x00040000,   /* sim rx */
    WDRV_DEBUG_HAL       = 0x00080000,   /* HAL generic */
    WDRV_DEBUG_DEVICE    = 0x00100000,   /* device */
    WDRV_DEBUG_INFO      = 0x80000000,   /* important debug info */
    WDRV_DEBUG_ANY       = 0xffffffff
};

enum {
    WQ_MAC_TXQ_SEND = 0,
    WQ_MAC_TXQ_FREE,
    WQ_MAC_CMDQ_SEND,
    WQ_MAC_CMDQ_FREE,
    WQ_MAC_POSTPONE_TXQ_SEND,
    WQ_MAC_POSTPONE_TXQ_FREE
};

#ifdef WQ_DEBUG
extern uint32_t wq_debug;
extern void ieee80211_catch_log(char *fmt, ...);
#define DPRINTF(m, fmt, ...) do {       \
    if ((wq_dbg_flag & WQ_DEBUG_MWL) && (wq_debug & (m))){                 \
            if(wq_dbg_flag & WQ_DEBUG_LOG2FILE){            \
                ieee80211_catch_log(fmt, ##__VA_ARGS__);  \
            }          \
            else       \
                iot_printf(fmt, ##__VA_ARGS__);           \
        }              \
    } while (0)
#else
#define DPRINTF(m, fmt, ...)
#endif


#define IFQMAXLEN   500
#define uintptr_t unsigned int

#define WQ_HOST_PS_SUPPORT
#define WQ_MAX_MCAST_NUM       5

#define WDRV_MID_START          0x00002000
#define mtx_init(a, b, c, d)    osal_mutex_create(a, WDRV_MID_START)
#define mtx_lock                osal_mutex_acquire
#define mtx_unlock              osal_mutex_release
#define mtx_lock_spin           osal_mutex_acquire
#define mtx_unlock_spin         osal_mutex_release
#define mtx_destroy             osal_mutex_destory
#define mtx_assert(...)   //TODO
#define mtx osal_mutex_t


#define bootverbose     0  //??????

#define malloc(a, b, c)         mmal_malloc(a)
#define free(a, b)              mmal_free(a)

#define wq_ticks               ticks
#define KEEPALIVE_LEN      1/* send keep alive interval (secs) */


#define DELAY   iot_delay_us
#define QUEUE_HEAD 0
#define QUEUE_TAIL 1

extern struct wq_softc *g_psc;
extern uint8_t tmp_macaddr[6];

typedef struct dev *device_t;

typedef struct driver driver_t;
char *device_get_nameunit(device_t dev);



/* mbufq */
struct mbufq {
    STAILQ_HEAD(, mbuf)     mq_head;
    int                     mq_len;
    int                     mq_maxlen;
};

struct mp_tone_gen_t {
    uint8_t enable;
    uint8_t num;
    uint16_t freq;
    uint16_t amp;
};

struct mp_freq_dev_t {
    uint16_t freq_dev;
};

struct mp_power_t {
    uint8_t rate_idx;
    int8_t power;
};

struct mp_power_ctrl_t {
    uint8_t power_ctrl;
};

struct mp_dpd_ctrl_t {
    uint8_t dpd_ctrl;
};

struct mp_digit_gain_t {
    uint8_t mode;
    uint16_t freq;
};

struct mp_kfree_cali_t {
	uint8_t version;
    uint8_t cali_efuse[5];
};

struct mp_set_ref_gain_t{
    uint32_t ref_gain[3];
};

void mbufq_init(struct mbufq *mq, int maxlen);
void mbufq_drain(struct mbufq *q);
struct mbuf *mbufq_dequeue(struct mbufq *q, struct mtx *lock);
int mbufq_enqueue(struct mbufq *q, struct mtx *lock, struct mbuf *m, int type);
struct mbuf *mbufq_flush(struct mbufq *mq);

#define m_free  m_freem


#define device_printf(dev, fmt,...) \
    DPRINTF(WDRV_DEBUG_DEVICE, "%s: " fmt, device_get_nameunit(dev),## __VA_ARGS__)

#endif

#ifndef _OSAL_H
#define _OSAL_H

/*
 * NOTE BEFORE USE:
 *  1.Spinlock only in POSIX, ioc_core change to use mutex
 *  2.NOT support semaphore take timeout
 *  3.osal_semaphore_count() only for debug purose and only in POSIX
 *  4.No periodic timer support
 *  5.Use spinlock for atomic operation in POSIX but disable-IRQ in FreeRTOS
 */

//#define CONFIG_OSAL_PRINTF
#define CONFIG_OSAL_ETM

#ifdef CONFIG_OSAL_PRINTF
#define osal_printf                 printf
#else
#define osal_printf(_f, args...)
#endif

#define OSAL_TASK_PRIO_NORMAL       6
#define OSAL_TASK_PRIO_HI           8

extern const uint8_t eth_broadcast_addr[];

extern void route_ieee80211msg(void *ifp, int what, void *data, size_t data_len);
extern void route_ieee80211msg_ex(void *ifp, int what, void *prom_data, size_t prom_data_len, void *data, size_t data_len);

//#define WQ_MAC_READ(addr)         (0)
//#define WQ_MAC_WRITE(addr, val)    (0)

#ifndef WQ_BUS_TEST

extern uint32_t WQ_MAC_READ(uint32_t addr);
extern void WQ_MAC_WRITE(uint32_t addr, uint32_t val);

#else
static __inline uint32_t WQ_MAC_READ(uint32_t addr)
{
	return 0;
}

static __inline void WQ_MAC_WRITE(uint32_t addr, uint32_t val)
{
	return;
}
#endif

typedef uint32_t task_hint;

/* os atomic */
struct osal_atomic_t {
    atomic_t val;
};

void osal_set(struct osal_atomic_t *a, int v);
void osal_atomic_add(struct osal_atomic_t *a, int v);
int osal_atomic_add_return(struct osal_atomic_t *a, int v);
int osal_atomic_check_set(struct osal_atomic_t *a, int c, int v);
int osal_get(struct osal_atomic_t *a);

#define osal_atomic_inc(_a)         osal_atomic_add(_a, 1)
#define osal_atomic_dec(_a)         osal_atomic_add(_a, -1)
#define osal_atomic_inc_return(_a)  osal_atomic_add_return(_a, 1)
#define osal_atomic_dec_return(_a)  osal_atomic_add_return(_a, -1)


/* os locker */
struct osal_locker_t {
    spinlock_t lock;
};

void osal_locker_create(struct osal_locker_t *osal_locker);
void osal_locker_destory(struct osal_locker_t *osal_locker);
void osal_locker_lock(struct osal_locker_t *osal_locker);
void osal_locker_unlock(struct osal_locker_t *osal_locker);

/* os mutex */
struct osal_mutex_t {
    struct mutex mutex;

    uint32_t mid;

#define OSAL_MUTEX_UNLOCK       (0)
#define OSAL_MUTEX_LOCK         (1)
    int status;
    task_hint holder;
    spinlock_t lock;
    uint32_t lock_count;
    uint32_t pid;
};

void osal_mutex_create(struct osal_mutex_t *osal_mutex, uint32_t mid);
void osal_mutex_destory(struct osal_mutex_t *osal_mutex);
void osal_mutex_acquire(struct osal_mutex_t *osal_mutex);
void osal_mutex_release(struct osal_mutex_t *osal_mutex);
void osal_recurse_mutex_create(struct osal_mutex_t *osal_mutex, uint32_t mid);
void osal_recurse_mutex_acquire(struct osal_mutex_t *osal_mutex);
void osal_recurse_mutex_release(struct osal_mutex_t *osal_mutex);

/* os semaphore */
struct osal_semaphore_t {
    struct semaphore sem;

    uint32_t mid;
    //struct osal_atomic_t res_cnt;
};

void osal_semaphore_create(struct osal_semaphore_t *osal_semaphore,
                           int cnt,
                           uint32_t mid);
void osal_semaphore_destory(struct osal_semaphore_t *osal_semaphore);
void osal_semaphore_take(struct osal_semaphore_t *osal_semaphore);
void osal_semaphore_post(struct osal_semaphore_t *osal_semaphore);
int osal_semaphore_count(struct osal_semaphore_t *osal_semaphore);

/* os global locker */
struct osal_glocker_t {
    struct osal_locker_t locker;
    struct osal_atomic_t recur;
};

int osal_glocker_lock(void);
int osal_glocker_unlock(void);

/* os timer */
struct osal_timer_t {
    struct timer_list timer;
	void* arg;
	void (*func)(void *argv);	
	struct work_struct ws_timer_work;
};

void osal_timer_init(struct osal_timer_t *osal_timer);
void osal_timer_mod(struct osal_timer_t *osal_timer,
                    uint32_t ms,
                    void (*osal_timer_cb)(void *argv),
                    void *argv);
void osal_timer_add(struct osal_timer_t *osal_timer,
                    uint32_t ms,
                    void (*osal_timer_cb)(void *argv),
                    void *argv);
void osal_timer_del(struct osal_timer_t *osal_timer);
int osal_timer_is_pending(struct osal_timer_t *osal_timer);
int osal_timer_is_idle(struct osal_timer_t *osal_timer);

/* os gtimer */
struct osal_gtimer_t {
    struct list_head list;

    int cur_tick;
    int tick;
    void (*gtimer_cb)(void *argv);
    void *argv;
};

int osal_gtimer_register(int tick, void (*gtimer_cb)(void *argv), void *argv);
void osal_gtimer_deregister(void (*gtimer_cb)(void *argv));

/* os task */
struct osal_task_t {
    struct semaphore sem;
    //pthread_t task;
    uint32_t event;
};

void osal_task_create(struct osal_task_t *osal_task,
                      int priority,
                      uint16_t stack_size,
                      char *name,
                      void (*routine)(void *arg),
                      void *arg);
void osal_task_destory(struct osal_task_t *osal_task);
void osal_task_sleep(struct osal_task_t *osal_task);
void osal_task_wakeup(struct osal_task_t *osal_task,
                      uint32_t cause,
                      int in_isr);
task_hint osal_task_current_hint(void);

void osal_start(void);

#ifdef CONFIG_OSAL_ETM
struct osal_etm_cnt_t {
    uint32_t locker;
    uint32_t mutex;
    uint32_t semaphore;
    uint32_t timer;
    uint32_t task;
};

#define OSAL_ETM_INC(_t)    (osal_etm_cnt._t++)
#define OSAL_ETM_DEC(_t)    (osal_etm_cnt._t--)

void osal_etm_dump(void);
#else
#define osal_etm_dump()
#endif
#endif


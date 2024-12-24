#include "oss.h"

#ifdef CONFIG_OSAL_ETM
static struct osal_etm_cnt_t osal_etm_cnt;
#undef ETH_ALEN
#define ETH_ALEN    6       /* Octets in one ethernet addr	 */

void osal_etm_dump(void)
{
    iot_printf("spinlock : %d\n", osal_etm_cnt.locker);
    iot_printf("mutex    : %d\n", osal_etm_cnt.mutex);
    iot_printf("semaphore: %d\n", osal_etm_cnt.semaphore);
    iot_printf("timer    : %d\n", osal_etm_cnt.timer);
    iot_printf("task     : %d\n", osal_etm_cnt.task);

    return;
}
#endif

void route_ieee80211msg_ex(void *ifp, int what, void *prom_data, size_t prom_data_len, void *data, size_t data_len)
{
	//iot_printf("route_ieee80211msg TBD what=%d, len=%d + %d\n", what, prom_data_len, data_len);
}

void route_ieee80211msg(void *ifp, int what, void *data, size_t data_len)
{
    //iot_printf("route_ieee80211msg TBD what=%d, len=%d\n", what, data_len);
    return;
}

int wifilib_is_freq_supported(uint16_t freq, char *ccode, uint8_t bw, uint8_t* tx_power)
{
    *tx_power = 0;
    return 1;
}

void wifilib_get_ccode(char* ccode)
{
	//USB TODO
    *ccode = 'U';
    *(ccode+1) = 'S';
}

#if 0
void phy_param_config(uint8_t bw, uint8_t pri_ch_upp_en, uint32_t ch_id, uint16_t pwr)
{
}


int packet_tx_raw(int ifindex, uint8_t *dst_addr, uint16_t protocol, uint8_t *buf, uint32_t len)
{

    struct ifnet *ifp = net_ifnet_registerred_by_idx(ifindex);
    int ret = 0;

    iot_printf("packet_tx_shortcut: ifp %p, len %d\n", ifp, len);

    if (ifp) {
        struct mbuf *m;
        uint8_t *pkt;
        struct sockaddr sk = {
            .sa_family = AF_PACKET,
        };
        struct sockaddr_ll *ll=(struct sockaddr_ll *)&sk;

        while ((m = m_gethdr(M_NOWAIT, MT_DATA)) == NULL) {
            os_delay(5);
        }
        IOT_ASSERT(m && ifp->if_output);

        if (m) {
            /* reserve header bytes for driver */
            m->m_data += WLAN_FRAME_HEAD_ROOM;

            pkt = mtod(m, uint8_t *);

            if (protocol == inet_htons(ETH_P_EAPOL_NO_ENC)) {
                protocol = inet_htons(ETH_P_PAE);
                m->m_flags_ext |= M_NO_ENCRYPT;
            }
            m->m_flags_ext |= M_DIRECT_FRM;

            //if (info->halen&&info->protocol!=inet_htons(WLAN_P_RAW)) {
            if (protocol!=inet_htons(WLAN_P_RAW)) {
                struct ethhdr *hdr = (struct ethhdr *)pkt;

                /* always assume it's a 802.3/ethernet format */
                os_mem_cpy(hdr->h_dest, dst_addr, ETH_ALEN);
                os_mem_cpy(hdr->h_source, IF_LLADDR(ifp), ETH_ALEN);
                hdr->h_proto = protocol;

                pkt += ETH_HLEN;
                m->m_len += ETH_HLEN;
            }

            os_mem_cpy(pkt, buf, len);
            ll->sll_protocol = protocol;

            m->m_len += len;

            if (ifp->if_output(ifp, m, &sk, NULL))
                ret = -EIO;
            else
                ret = len + ETH_HLEN;
        } else {
            ret = -ENOMEM;
        }
    } else {
        ret = -ENODEV;
    }

    return ret;
}
#endif

const uint8_t eth_broadcast_addr[ETH_ALEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
uint8_t tmp_macaddr[6] = { 0xcd, 0xab, 0xcd, 0xab, 0xcc, 0xdd}; //mac address for wdrv init

static struct osal_glocker_t osal_glocker;
//static struct osal_timer_t osal_gtimer;
//static struct list_head osal_gtimer_list;
//static struct osal_glocker_t osal_atomic_locker;

void osal_set(struct osal_atomic_t *a, int v)
{
    atomic_set(&a->val, v);
    
    return;
}

void osal_atomic_add(struct osal_atomic_t *a, int v)
{
    atomic_add(v, &a->val);

    return;
}

int osal_atomic_add_return(struct osal_atomic_t *a, int v)
{
    return atomic_add_return(v, &a->val);
}

int osal_atomic_check_set(struct osal_atomic_t *a, int c, int v)
{  
    int ret;

    ret=atomic_cmpxchg(&a->val, c, v);
    if(ret==c)
        return 1;

    return 0;
}

int osal_get(struct osal_atomic_t *a)
{
    return atomic_read(&a->val);
}

void osal_locker_create(struct osal_locker_t *osal_locker)
{
    osal_printf("os: create locker %p\n", osal_locker);

    spin_lock_init(&osal_locker->lock);


    OSAL_ETM_INC(locker);

    return;
}

void osal_locker_destory(struct osal_locker_t *osal_locker)
{
    //pthread_spin_destroy(&osal_locker->lock);
    OSAL_ETM_DEC(locker);

    return;
}

void osal_locker_lock(struct osal_locker_t *osal_locker)
{
    spin_lock(&osal_locker->lock);

    return;
}

void osal_locker_unlock(struct osal_locker_t *osal_locker)
{
    spin_unlock(&osal_locker->lock);
    
    return;
}

int osal_glocker_lock(void)
{
    if (osal_atomic_inc_return(&osal_glocker.recur) > 1)
        return 1;

    spin_lock(&osal_glocker.locker.lock);

    return 0;
}

int osal_glocker_unlock(void)
{
    IOT_ASSERT(osal_get(&osal_glocker.recur) > 0);

    if (osal_atomic_dec_return(&osal_glocker.recur) > 0)
        return 1;

    spin_unlock(&osal_glocker.locker.lock);

    return 0;
}

static void osal_glocker_create(void)
{
    osal_printf("os: create glocker\n");

    spin_lock_init(&osal_glocker.locker.lock);

    osal_set(&osal_glocker.recur, 0);

    OSAL_ETM_INC(locker);

    return;
}    

#if 1
void osal_mutex_create(struct osal_mutex_t *osal_mutex, uint32_t mid)
{
    mutex_init(&osal_mutex->mutex);

    osal_mutex->pid = 0xffffffff;
    osal_mutex->status = OSAL_MUTEX_UNLOCK;
    osal_mutex->holder = (task_hint)0;
    osal_mutex->mid = mid;

    OSAL_ETM_INC(mutex);

    osal_printf("os: create mutex %p mid %d\n", osal_mutex, mid);

    return;
}


void osal_mutex_destory(struct osal_mutex_t *osal_mutex)
{
    osal_printf("os: destory mutex %p mid %d\n", osal_mutex, osal_mutex->mid);

    IOT_ASSERT(osal_mutex->status != OSAL_MUTEX_LOCK);

    OSAL_ETM_DEC(mutex);

    return;
}

void osal_mutex_acquire(struct osal_mutex_t *osal_mutex)
{
    IOT_ASSERT(osal_mutex->pid != current->pid);
    mutex_lock(&osal_mutex->mutex);

    IOT_ASSERT(osal_mutex->status == OSAL_MUTEX_UNLOCK);

    osal_mutex->status = OSAL_MUTEX_LOCK;
    osal_mutex->pid = current->pid;

    return;
}

void osal_mutex_release(struct osal_mutex_t *osal_mutex)
{
    IOT_ASSERT(osal_mutex->status == OSAL_MUTEX_LOCK);

    osal_mutex->status = OSAL_MUTEX_UNLOCK;
    osal_mutex->pid = 0xffffffff;

    mutex_unlock(&osal_mutex->mutex);

    return;
}



void osal_recurse_mutex_create(struct osal_mutex_t *osal_mutex, uint32_t mid)
{
    mutex_init(&osal_mutex->mutex);

    osal_mutex->pid = 0xFFFFFFFF;
    osal_mutex->lock_count = 0;
    osal_mutex->status = OSAL_MUTEX_UNLOCK;
    osal_mutex->holder = (task_hint)0;
    osal_mutex->mid = mid;

    OSAL_ETM_INC(mutex);

    osal_printf("os: create recurse mutex %p mid %d\n", osal_mutex, mid);

    return;
}

void osal_recurse_mutex_acquire(struct osal_mutex_t *osal_mutex)
{
    //iot_printf("%s %p\n", __func__, osal_mutex);
    if(osal_mutex->pid == current->pid){
        osal_mutex->lock_count++;
        return;
    }
    mutex_lock(&osal_mutex->mutex);


    IOT_ASSERT(osal_mutex->status == OSAL_MUTEX_UNLOCK);

    osal_mutex->status = OSAL_MUTEX_LOCK;
    osal_mutex->pid = current->pid;
    osal_mutex->lock_count++;


    return;
}

void osal_recurse_mutex_release(struct osal_mutex_t *osal_mutex)
{
    //iot_printf("%s %p\n", __func__, osal_mutex);

    IOT_ASSERT(osal_mutex->status == OSAL_MUTEX_LOCK);
    IOT_ASSERT(osal_mutex->lock_count >= 1);

    if(osal_mutex->pid == current->pid){
        osal_mutex->lock_count--;

        if(osal_mutex->lock_count == 0){
            osal_mutex->status = OSAL_MUTEX_UNLOCK;
            osal_mutex->pid = 0xFFFFFFFF;
            mutex_unlock(&osal_mutex->mutex);
        }
    }
}
#else
void osal_mutex_create(struct osal_mutex_t *osal_mutex, uint32_t mid)
{
	spin_lock_init(&osal_mutex->lock);

    osal_mutex->status = OSAL_MUTEX_UNLOCK;
    osal_mutex->holder = (task_hint)0;
    osal_mutex->mid = mid;

    OSAL_ETM_INC(mutex);

    osal_printf("os: create mutex %p mid %d\n", osal_mutex, mid);

    return;
}


void osal_mutex_destory(struct osal_mutex_t *osal_mutex)
{
    osal_printf("os: destory mutex %p mid %d\n", osal_mutex, osal_mutex->mid);

    IOT_ASSERT(osal_mutex->status != OSAL_MUTEX_LOCK);

    OSAL_ETM_DEC(mutex);

    return;
}

void osal_mutex_acquire(struct osal_mutex_t *osal_mutex)
{

	spin_lock(&osal_mutex->lock);

    IOT_ASSERT(osal_mutex->status == OSAL_MUTEX_UNLOCK);

    osal_mutex->status = OSAL_MUTEX_LOCK;

    return;
}

void osal_mutex_release(struct osal_mutex_t *osal_mutex)
{
    IOT_ASSERT(osal_mutex->status == OSAL_MUTEX_LOCK);

    osal_mutex->status = OSAL_MUTEX_UNLOCK;

	spin_unlock(&osal_mutex->lock);
    return;
}



void osal_recurse_mutex_create(struct osal_mutex_t *osal_mutex, uint32_t mid)
{
    
	spin_lock_init(&osal_mutex->lock);

    osal_mutex->status = OSAL_MUTEX_UNLOCK;
    osal_mutex->holder = (task_hint)0;
    osal_mutex->mid = mid;
	osal_mutex->lock_count = 0;

    OSAL_ETM_INC(mutex);

    osal_printf("os: create recurse mutex %p mid %d\n", osal_mutex, mid);

    return;
}

void osal_recurse_mutex_acquire(struct osal_mutex_t *osal_mutex)
{
	//iot_printf("osal_recurse_mutex_acquire %x\n", osal_mutex);
	
	if (osal_mutex->status == OSAL_MUTEX_LOCK)
	{
		osal_mutex->lock_count++;
	}
	else
	{
		spin_lock(&osal_mutex->lock);
		osal_mutex->lock_count++;
		osal_mutex->status = OSAL_MUTEX_LOCK;

	}

    return;
}

void osal_recurse_mutex_release(struct osal_mutex_t *osal_mutex)
{
    //iot_printf("osal_recurse_mutex_release %x\n", osal_mutex);

	IOT_ASSERT(osal_mutex->status == OSAL_MUTEX_LOCK);
	IOT_ASSERT(osal_mutex->lock_count >= 1);

    osal_mutex->lock_count--;

	if (osal_mutex->lock_count == 0)
	{
		osal_mutex->status = OSAL_MUTEX_UNLOCK;
		spin_unlock(&osal_mutex->lock);
	}

    return;
}

	

#endif


void osal_semaphore_create(struct osal_semaphore_t *osal_semaphore,
                           int cnt,
                           uint32_t mid)
{
    sema_init(&osal_semaphore->sem, cnt);

    osal_semaphore->mid = mid;

    OSAL_ETM_INC(semaphore);

    osal_printf("os: create semaphore %p cnt %d mid %d\n", osal_semaphore,
                                                           cnt,
                                                           mid);

    return;
}

void osal_semaphore_destory(struct osal_semaphore_t *osal_semaphore)
{
    osal_printf("os: destory semaphore %p mid %d\n", osal_semaphore,
                                                     osal_semaphore->mid);

    OSAL_ETM_DEC(semaphore);

    return;
}

void osal_semaphore_take(struct osal_semaphore_t *osal_semaphore)
{
    osal_printf("os: semaphore take %p mid %d\n", osal_semaphore,
                                                  osal_semaphore->mid);

    if (down_interruptible(&osal_semaphore->sem))
    {
        osal_printf("return");
    }

    return;
}

void osal_semaphore_post(struct osal_semaphore_t *osal_semaphore)
{
    osal_printf("os: semaphore post %p mid %d\n", osal_semaphore,
                                                  osal_semaphore->mid);

    up(&osal_semaphore->sem);

    return;
}
#if 0
void osal_timer_init(struct osal_timer_t *osal_timer)
{
    //init_timer(&osal_timer->timer);
}

void osal_timer_mod(struct osal_timer_t *osal_timer,
                    uint32_t ms,
                    void (*osal_timer_cb)(void *argv),
                    void *argv)
{
    osal_timer->timer.function=osal_timer_cb;
    mod_timer_pending(&osal_timer->timer,jiffies+ms*HZ/1000);
}

void osal_timer_add(struct osal_timer_t *osal_timer,
                    uint32_t ms,
                    void (*osal_timer_cb)(void *argv),
                    void *argv)
{
    osal_timer->timer.function=osal_timer_cb;
    osal_timer->timer.expires=jiffies+ms*HZ/1000;
    osal_timer->timer.data=argv;
    add_timer(&osal_timer->timer);

}

void osal_timer_del(struct osal_timer_t *osal_timer)
{
    del_timer(&osal_timer->timer);
}

int osal_timer_is_pending(struct osal_timer_t *osal_timer)
{
    return timer_pending(&osal_timer->timer);
}
#endif
#if 0
int osal_semaphore_count(struct osal_semaphore_t *osal_semaphore)
{
    int value = osal_get(&osal_semaphore->res_cnt);

    osal_printf("os: semaphore count %d\n", value);

    return value;
}

task_hint osal_task_current_hint(void)
{
    /* hint in POSIX is ID and is task handle in iot_core */

    return (task_hint)pthread_self();
}
#endif

#if 0
void osal_task_create(struct osal_task_t *osal_task,
                      int priority,
                      uint16_t stack_size,
                      char *name,
                      void (*routine)(void *arg),
                      void *arg)
{
	osal_printf("os: create task %p\n", osal_task);

    sem_init(&osal_task->sem, 0, 0);
    pthread_create(&osal_task->task, NULL, routine, arg);

    OSAL_ETM_INC(task);

    return;
}

void osal_task_destory(struct osal_task_t *osal_task)
{
    /*
     * WARN: NOT expect to destory a task in embedded system unless special
     *       purpose. To safely reclaim all resources and this API should call
     *       in task routine after all resources returned.
     */

    WARN(1, "os: destory task %p\n", osal_task);

    sem_destroy(&osal_task->sem);
    pthread_exit(NULL);

    OSAL_ETM_DEC(task);
    return;
}

void osal_task_sleep(struct osal_task_t *osal_task)
{
    osal_printf("os: sleep task %p\n", osal_task);

    sem_wait(&osal_task->sem);

    return;
}

void osal_task_wakeup(struct osal_task_t *osal_task,
                      uint32_t cause,
                      int in_isr)
{
    osal_printf("os: wakeup task %p\n", osal_task);

    sem_post(&osal_task->sem);

    return;
}
#endif


void osal_start(void)
{
    osal_glocker_create();
    //osal_gtimer_create(); //not used now

    return;
}


#if 1 //OSAL unit test code
#include <net80211/ieee80211_sim.h>

struct osal_mutex_t test_mtx;
int test_flag = 0;
struct osal_semaphore_t test_sem;
static struct osal_timer_t test_timer;

static void timer_test_func(void* argv)
{
    //iot_printf("\ntimer : argv=%x\n", (unsigned int)argv);
    //if (argv != 0xabcdef01)
    //    IOT_ASSERT(0);
    test_flag = 1;
}

void timer_unit_test(void)
{
	int ret;
	
	callout_init_v2(&test_timer, timer_test_func,(void *) 0xabcdef01);
	
	callout_reset_v2(&test_timer, 100);
	
	ret = callout_pending(&test_timer);
	if (ret == 1)
		iot_printf ("timer pending test : passed\n");
	else
		iot_printf ("timer pending test : failed\n");

	while (test_flag == 0)
	{
		iot_printf ("test_flag == 0\n");
	    msleep(10);
    }

	iot_printf ("timer 1st test : passed\n");

	ret = callout_pending(&test_timer);	
	if (ret == 0)
		iot_printf ("timer non-pending test : passed\n");
	else
		iot_printf ("timer non-pending test : failed\n");
	
	callout_stop_v2(&test_timer);
	
	test_flag = 0;
	callout_reset_v2(&test_timer, 100);
	while (test_flag == 0)
	{
		iot_printf ("test_flag == 0\n");
	    msleep(10);
    }
	
	iot_printf ("timer 2nd test : passed\n");
	
}

struct workqueue_struct *wq;
struct wuqi_work_struct wuqi_wk={{0}},wuqi_wk2={{0}},wuqi_wk3={{0}};

static void wk_hander(struct work_struct *work)
{
    struct osal_semaphore_t *argv= (struct osal_semaphore_t *)WORK_ARGV(work);
    //iot_printf("%s(), argv:0x%x, &test_sem:0x%x\n", __func__,(unsigned int)argv,(unsigned int)&test_sem);
    IOT_ASSERT(argv==&test_sem);

    iot_printf("work insert/argv test:passed\n");
}

static void wk_hander2(struct work_struct *work)
{
    struct osal_mutex_t *argv= (struct osal_mutex_t *)WORK_ARGV(work);
    //iot_printf("%s(), argv:0x%x, &test_mtx:0x%x\n", __func__,(unsigned int)argv,(unsigned int)&test_mtx);
    IOT_ASSERT(argv==&test_mtx);

    iot_printf("work insert/argv test2:passed\n");
}

static void wk_hander3(struct work_struct *work)
{
    struct wuqi_work_struct *wuqi_wk = container_of(work, struct wuqi_work_struct, work);
    struct osal_mutex_t *argv= (struct osal_mutex_t *)wuqi_wk->argv;

    //iot_printf("%s(), argv:0x%x, &test_mtx:0x%x\n", __func__,(unsigned int)argv,(unsigned int)&test_mtx);
    IOT_ASSERT(argv==&test_mtx);

    if(wuqi_wk->comp_func){
        wuqi_wk->comp_func();
    }

    iot_printf("work insert/argv test3:passed\n");
}

static void wk_comp(void)
{
    iot_printf("%s executed!\n",__func__);
}


static int wq_test(void)
{
    iot_printf("wq_test start\n");

    wq = wk_task_queue_create("unittestwq",9);

    wk_init(&wuqi_wk, wk_hander, &test_sem);

    wk_queue_insert(wq,&wuqi_wk);

    wk_init(&wuqi_wk2, wk_hander2, NULL);

    wk_queue_insert_ex(wq,&wuqi_wk2,&test_mtx);

    wk_init_comp(&wuqi_wk3, wk_hander3, &test_mtx, wk_comp);

    wk_queue_insert(wq,&wuqi_wk3);

    return 0;
}

#define MEM_PATTERN (0x11223344)
void test_kmalloc(void)
{
    int size=120*1024;
    unsigned int *memory,*ptr,*ptr2;
    int i;

    memory = (int *)wq_mem_alloc(size, 0);
    ptr=ptr2=memory;

    //iot_printf("start memory test,memory:0x%x, value:0x%x\n",(unsigned int)memory,*memory);
    for(i=0;i<size/4;i++)
           *ptr++=MEM_PATTERN;

    for(i=0;i<size/4;i++)
           IOT_ASSERT(*ptr2++==MEM_PATTERN);

    //iot_printf("after write memory test,memory:0x%x, value:0x%x\n",(unsigned int)memory,*memory);
    kfree(memory);
    iot_printf("kmalloc test pass!\n");
}


void osal_unit_test(void)
{
    int i;
    struct osal_atomic_t data;
    int val1, val2, val3;
    struct osal_locker_t lock;
    //struct osal_timer_t timer;
    //struct osal_task_t task1, task2, task3, task4, task5;
    

    //os_delay test
    for (i=3; i>0; i--)
    {
        iot_printf ("os_delay test : %d (1/10 sec)\n", i);
        //os_delay(1000);
        msleep(100);
    }
    
    //atomic test
    osal_set(&data, 100);
    osal_atomic_add(&data, 20);
    val1 = osal_get(&data);
    val2 = osal_atomic_add_return(&data, 30);
    osal_atomic_check_set(&data, 150, 200);
    osal_atomic_check_set(&data, 150, 210);
    val3 = osal_get(&data);

    if ((val1 == 120) && (val2 == 150) && (val3 == 200))
        iot_printf ("atomic test : passed %d %d %d\n", val1, val2, val3);
    else
        iot_printf ("atomic test : failed %d %d %d\n", val1, val2, val3);
    
    //lock test
    osal_locker_create(&lock);
    osal_locker_lock(&lock);
    osal_locker_unlock(&lock);
    
    osal_glocker_create();
    osal_glocker_lock();
    osal_glocker_unlock();
    iot_printf ("lock test : passed\n");
#if 0
    //timer test
    test_flag = 0;
    osal_timer_init(&timer);
    iot_printf ("timer test : start 1 second timer and wait");
    osal_timer_add(&timer,
                    1000,
                    timer_test_func,
                    test_str);
    if (!osal_timer_is_pending(&timer))
        IOT_ASSERT(0);
    //if (osal_timer_is_idle(&timer))
    //    IOT_ASSERT(0);
        
    while(test_flag == 0)
    {
        iot_printf (".");
        //os_delay(100);
        msleep(100);
    }
    
    if (osal_timer_is_pending(&timer))
        IOT_ASSERT(0);
    //if (!osal_timer_is_idle(&timer))
    //    IOT_ASSERT(0);

    osal_timer_del(&timer);

    iot_printf ("\ntimer test : passed\n");
#endif

    //mutex test    
    osal_mutex_create(&test_mtx, 100);
    
    osal_mutex_acquire(&test_mtx);
    iot_printf ("acquired mutex\n");
    iot_printf ("releasing mutex\n");
    osal_mutex_release(&test_mtx);
    
    iot_printf ("mutex test : passed\n");

    //sem test
    osal_semaphore_create(&test_sem, 2, 101);
    
    osal_semaphore_take(&test_sem);
    osal_semaphore_take(&test_sem);
    osal_semaphore_post(&test_sem);
    osal_semaphore_post(&test_sem);
    
    iot_printf ("sem test : passed\n");

    wq_test();

    test_kmalloc();

    return;
}
#endif

#ifndef _TASK_QUEUE_H
#define _TASK_QUEUE_H

//#define WORK_ARGV(_wk)              ((_wk)->argv)
#define WORK_ARGV(_wk)              (((container_of(_wk, struct wuqi_work_struct, work)))->argv)


struct wuqi_work_struct {
    struct list_head list;

    struct osal_atomic_t state;

    void (*func)(struct work_struct *work);
    void (*comp_func)(void);
    void *argv;

    bool init_done;
    bool wait_done;

    uint32_t work_id;

    int waiting_flush;
    struct osal_semaphore_t cancel;
    struct work_struct  work;
    struct workqueue_struct *wq;
};

void wk_init(struct wuqi_work_struct *wq_wk,
             void (*func)(struct work_struct *work),
             void *argv);
			 
void wk_deinit(struct wuqi_work_struct *wk);


void wk_init_comp(struct wuqi_work_struct *wk,
                  void (*func)(struct work_struct *work),
                  void *argv,
                  void (*comp_func)(void));

int wk_queue_insert(struct workqueue_struct *wq, struct wuqi_work_struct *wuqi_wk);
int wk_queue_insert_ex(struct workqueue_struct *wq, struct wuqi_work_struct *wuqi_wk, void *argv);

struct workqueue_struct *wk_task_queue_create(const char *name, int priority);
void wk_task_queue_destory(struct workqueue_struct *wq);
void wk_task_queue_drain(struct workqueue_struct *wq);
void wk_task_queue_flush(struct workqueue_struct *wq);
void wk_task_queue_block(struct workqueue_struct *wq, int blocked);
int wk_queue_remove(struct workqueue_struct *wq,
                    struct wuqi_work_struct *wk,
                    int synced);

#endif

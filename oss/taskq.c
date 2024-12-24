#include "oss.h"
#include "taskq.h"

struct workqueue_struct *wk_task_queue_create(const char *name, int priority)
{
    return alloc_workqueue(name, 0, 0);
}

void wk_init(struct wuqi_work_struct *wuqi_wk,
             void (*func)(struct work_struct *work),
             void *argv)
{
    wuqi_wk->argv=argv;
    wuqi_wk->comp_func=NULL;
    INIT_WORK(&wuqi_wk->work,func);
}

//USB TODO : find out a way to handle comp_func
void wk_init_comp(struct wuqi_work_struct *wuqi_wk,
                  void (*func)(struct work_struct *work),
                  void *argv,
                  void (*comp_func)(void))
{
    wuqi_wk->argv=argv;
    wuqi_wk->comp_func=comp_func;
    INIT_WORK(&wuqi_wk->work,func);
}

void wk_deinit(struct wuqi_work_struct *wk)
{
    //TODO : fix this for simulator
    return;
}

int wk_queue_insert(struct workqueue_struct *wq, struct wuqi_work_struct *wuqi_wk)
{
    queue_work(wq, &wuqi_wk->work);
    return 0;
}

int wk_queue_insert_ex(struct workqueue_struct *wq, struct wuqi_work_struct *wuqi_wk, void *argv)
{
    wuqi_wk->argv=argv;
    queue_work(wq, &wuqi_wk->work);
    return 0;
}

void wk_task_queue_destory(struct workqueue_struct *wq)
{
    destroy_workqueue(wq);
}


void wk_task_queue_drain(struct workqueue_struct *wq)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,1,0)
    drain_workqueue(wq);
#else
    flush_workqueue(wq);
#endif
}


void wk_task_queue_flush(struct workqueue_struct *wq)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,1,0)
    drain_workqueue(wq);
#else
    flush_workqueue(wq);
#endif
}

void wk_task_queue_block(struct workqueue_struct *wq, int blocked)
{
	//TODO
}


int wk_queue_remove(struct workqueue_struct *wq,
                    struct wuqi_work_struct *wk,
                    int synced)
{
    int ret;
    ret=cancel_work_sync(&wk->work);
    return ret;
}

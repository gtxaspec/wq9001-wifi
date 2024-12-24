#ifndef _OSS_H
#define _OSS_H

#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/skbuff.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include <linux/random.h>
#include <linux/version.h>
#include <linux/kthread.h>

#define ANSI_COLOR_RED
#define ANSI_COLOR_GREEN
#define ANSI_COLOR_YELLOW
#define ANSI_COLOR_BLUE
#define ANSI_COLOR_MAGENTA
#define ANSI_COLOR_CYAN
#define ANSI_COLOR_WHITE
#define ANSI_COLOR_RESET

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,0,8)
typedef unsigned int            u_int;
#endif
typedef uint64_t                counter_u64_t;

#define IOT_ASSERT(a) BUG_ON(!(a))


#define ISR_ATTR
#define FAST_ATTR



#ifdef TASKNAME_PRINT
 #define iot_printf(fmt, ...)               \
     do {                                    \
             pr_cont("[%d][%s] ",current->pid,current->comm); \
             pr_cont(fmt, ##__VA_ARGS__);     \
    } while (0)
#else
#define iot_printf      printk
#endif


#define iot_snprintf 	snprintf
#define iot_vprintf 	vprintk
#define iot_vsnprintf 	vsnprintf
#define iot_strlen 		strlen



#define iot_delay_us udelay

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
#define os_rand get_random_int
#else
#define os_rand get_random_u32
#endif

#define os_mem_cpy memcpy
#define os_mem_cmp memcmp
#define os_mem_set memset
#define os_delay msleep
#define ovbcopy(f, t, l) memmove((t), (f), (l))
#define os_mem_move memmove

#include "osal.h"
#include "_sclist.h"
#include "_queue.h"

#include "mmal.h"
#include "mbuf.h"
#include "net_if.h"
#include "net_if_media.h"
#include "_errno.h"
#include "taskq.h"



#define IFNAMSIZ            16

#define WLAN_P_RAW          0x0006      /* WLAN RAW pakcet only		*/
#define M_NO_ENCRYPT        0x1         /* no encrypt on this frame */
#define M_DIRECT_FRM        0x2         /* direct frame from wsupp */
#define M_IS_ICMP           0x4         /*frame proto is icmp*/
#define M_IS_PING_REPLY     0x8         /* frame is ping reply */
#define M_IS_AUTH           0x10        /* frame is auth */
#define M_IS_ASSOC_RESP     0x20        /* frame is assoc resp */
#define M_IS_REASSOC_RESP   0x40        /* frame is reassoc resp*/
#define M_AMSDU             0x80        /*A-MSDU subframe*/

#define AF_IEEE80211    0xFF    /* NOT SUPPORT, dummy */

#define inet_htons(x)        le16_to_cpu(x)

#define gp_timer_get_current_val(x) (jiffies)

#if 0
#include "sdk.h"

#include "_cpu.h"
#include "_types.h"
#include "_errors.h"
#include "_align.h"
#include "_math.h"
#include "_bitops.h"
#include "_byteops.h"
#include "_ioc.h"
#include "_list.h"

#include "debug.h"
#include "osal.h"
#include "mfile.h"
#include "skbuf.h"
#include "ether.h"
#include "skipc.h"
#include "un.h"
#include "in.h"
#include "packet.h"
#include "net_if.h"
#include "net_if_media.h"
#include "net.h"
#include "rt.h"
#include "ioctl.h"
#include "device.h"

#include "wproto.h"
#include "wdrv.h"
#include "wsupp.h"
#endif

#define _LITTLE_ENDIAN 0
#define _BIG_ENDIAN    1
#define _BYTE_ORDER         _LITTLE_ENDIAN
#define WQ_MEM_GFP  in_interrupt() ? GFP_ATOMIC : GFP_KERNEL

void oss_start(void);

extern void dump_bytes(char *note, uint8_t *data, int len);
extern u32 *wq_mem_alloc(int len, int zero);
//#define in_irq()    0
#endif

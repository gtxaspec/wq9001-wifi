#ifndef _MMAL_H
#define _MMAL_H

#define CONFIG_MMAL_DEBUG
//#define CONFIG_MMAL_PRINTF
//#define CONFIG_MMAL_ETM

#ifdef CONFIG_MMAL_PRINTF
#define mmal_printf                         iot_printf
#else
#define mmal_printf(_f, args...)
#endif

#define MM_BASIC_ALIGN                      (4)

#define MM_CHUNK_LVL_NUM                    (3)
#define MM_CHUNK_MEM_NUM                    (6)

struct memory_chunk_info_t {
    struct {
        int size;
        int number;
        int is_anchor;
    } member[MM_CHUNK_MEM_NUM];
};

struct memory_chunk_t {
#ifdef CONFIG_MMAL_DEBUG
#define MM_CHUNK_COOKIE_S                   (0xf094870f)
#define MM_CHUNK_COOKIE_E                   (0x0f8745f0)
#if (WQ_CONFIG_64 == 1)
    uint64_t cookie_s;
    uint64_t caller;
    uint64_t size;                            /* request size */
#else
    uint32_t cookie_s;
    uint32_t caller;
    uint32_t size;                            /* request size */
#endif
#endif

    SCLIST_ENTRY(memory_chunk_t) entry;
    void *chunk_mem;

    uint8_t buffer[0];
};

struct memory_chunk_member_t {
    SCLIST_HEAD(memory_chunk_t) head;
    int size;
};

struct memory_chunk_ctx_t {
    struct memory_chunk_member_t *chunk_lvl[MM_CHUNK_LVL_NUM + 1];
    struct memory_chunk_member_t chunk_mem[MM_CHUNK_MEM_NUM + 1];

    struct osal_locker_t big_lock;
    void *pool_start;
    int pool_sz;
};

enum memory_type_e {
    MEMPOOL_TYPE_SLOW,          /* slower than MEMPOOL_TYPE_NORMAL */
    MEMPOOL_TYPE_NORMAL = 0,    /* SRAM, EMEM, DRAM */
    MEMPOOL_TYPE_UNKNOW,        /* reclaim memory or allocate for any type */
    MEMPOOL_TYPE_FAST,          /* DMEM */
};

struct memory_pool_info_t {
    struct memory_pool_info_t *next;
#if (WQ_CONFIG_64 == 1)
    uint64_t caller;
    uint64_t addr;
#else 
    uint32_t caller;
    uint32_t addr;
#endif
    uint32_t size;
};

struct memory_pool_t {
    struct memory_pool_t *next;
    struct memory_pool_info_t *alloc;

#define _MEM_TYPE_MSK       0xf
#define _MEM_TYPE_SH        0
#define _MEM_SUPP_DMA       BIT(31)
    uint32_t flags;
    uint32_t total_size;
    uint32_t used_size;
    uint8_t *start;
    uint8_t *cur;
};

struct memory_pool_ctx_t {
    struct memory_pool_t first;

    uint32_t pool_num;
    uint32_t pool_total_size;
    uint32_t pool_user_alloc;
};

#define MEMPOOL_CLR_ATTR(_pool)                     \
                ((_pool)->flags = 0x0)
#define MEMPOOL_GET_TYPE(_pool)                     \
                (((_pool)->flags >> _MEM_TYPE_SH) & _MEM_TYPE_MSK)
#define MEMPOOL_SET_TYPE(_pool, _type)              \
                ((_pool)->flags |= (((_type) & _MEM_TYPE_MSK) << _MEM_TYPE_SH))
#define MEMPOOL_GET_SUPP_DMA(_pool)                 \
                ((_pool)->flags & _MEM_SUPP_DMA)
#define MEMPOOL_SET_SUPP_DMA(_pool, _supp)          \
                ((_pool)->flags |= _MEM_SUPP_DMA)

void mmal_pool_attach(enum memory_type_e type,
                      int suppDma,
                      uint8_t *ptr,
                      uint32_t size);
void *mmal_pool_alloc(enum memory_type_e type,
                      int suppDma,
                      int size,
                      int align,
                      int reset);
void mmal_pool_info(char *note);

void mmal_chunk_init(struct memory_chunk_info_t *chunk_info);
void mmal_chunk_info(void);

//TODO:Porting needs to check
#define WEAK_SYMBOL_ATTR __attribute__((weak))
#define CALLER_ADDR()               __builtin_return_address(0)


#if 0
void *mmal_malloc(unsigned int size) WEAK_SYMBOL_ATTR;
#else
void *_mmal_malloc_dbg(unsigned int size, const char *function_name, int line_num) WEAK_SYMBOL_ATTR;
#define mmal_malloc(x) _mmal_malloc_dbg(x, __func__, __LINE__)

#endif

void mmal_free(void *ptr) WEAK_SYMBOL_ATTR;
#ifdef CONFIG_MMAL_ETM
void mmal_etm_init(void);
void mmal_etm_info(void);
#else
#define mmal_etm_init()
#define mmal_etm_info()
#endif
#endif

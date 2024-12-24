#include "oss.h"

static struct memory_pool_ctx_t memory_pool_ctx;
extern struct memory_chunk_info_t memory_chunk_info;

#define MEM_RECORED_CHECK 1
#ifdef MEM_RECORED_CHECK
#define MEM_RECORED_NUM 300
typedef struct _mem_record
{
    int size;
    char func[100];
    int line_num;
    void *addr;
}mem_record_s;

mem_record_s mem_record[MEM_RECORED_NUM]={{0}};

void add_mem_record(unsigned int size, const char *func, int line_num, void* addr)
{
    int idx;

    for(idx=0;idx<MEM_RECORED_NUM;idx++)
    {
        if(mem_record[idx].size==0 && mem_record[idx].line_num==0){
            mem_record[idx].size=size;
            mem_record[idx].line_num=line_num;
            strcpy(mem_record[idx].func,func);
            mem_record[idx].addr=addr;
            break;
        }
    }

    if(idx==MEM_RECORED_NUM)
        iot_printf("Out of mem_record\n");
}


void del_mem_record(void* addr)
{
    int idx;

    for(idx=0;idx<MEM_RECORED_NUM;idx++)
    {
        if(mem_record[idx].addr==addr){
            mem_record[idx].addr=0;
            mem_record[idx].size=0;
            mem_record[idx].line_num=0;
            memset(mem_record[idx].func,0,sizeof(mem_record[idx].func));
            break;
        }
    }
}

void dump_mem_record()
{
    int idx;

    iot_printf("===== MMAL allocated records start ======\n");
    for(idx=0;idx<MEM_RECORED_NUM;idx++)
    {
        if(mem_record[idx].addr!=0){
            iot_printf("<%s;%d> size:%d, addr:0x%p\n",mem_record[idx].func,mem_record[idx].line_num,mem_record[idx].size,mem_record[idx].addr);
        }
    }
    iot_printf("===== MMAL allocated records end ======\n");
}
#else
#define add_mem_record(size,func,line_num,addr)
#define del_mem_record(ptr)
#define dump_mem_record()
#endif


static inline void *_alloc_memory_pool(struct memory_pool_t *pool,
                                       unsigned int n)
{
    uint8_t *ptr = pool->cur;
    int size = ALIGN(n, 4);

    if (((ptr + size) - pool->start) > pool->total_size)
        return NULL;
    else {
        pool->cur = ptr + size;

        return (void *)ptr;
    }
}

static inline struct memory_pool_t *_find_memory_pool(struct memory_pool_ctx_t *pool_ctx,
                                                      enum memory_type_e type,
                                                      int suppDma,
                                                      int size)
{
    struct memory_pool_t *pool = &pool_ctx->first;

    /* give the lower priority pool if request type is UNKNOW */

    do {
        /* type match or not */
        if ((MEMPOOL_GET_TYPE(pool) == type) ||
            ((type == MEMPOOL_TYPE_UNKNOW) &&
             (MEMPOOL_GET_TYPE(pool) <= MEMPOOL_TYPE_UNKNOW))) {
            /* size is allowed or not */
            if (((pool->cur + size) - pool->start) < pool->total_size) {
                /* DMA support or not */
                if ((!suppDma) ||
                    (suppDma && MEMPOOL_GET_SUPP_DMA(pool)))
                    break;
            }
        }

        pool = pool->next;
    } while (pool);

    return pool;
}

void mmal_pool_reset()
{
    os_mem_set((void *)&memory_pool_ctx, 0, sizeof(memory_pool_ctx));
}


void mmal_pool_attach(enum memory_type_e type,
                      int suppDma,
                      uint8_t *ptr,
                      uint32_t size)
{
    struct memory_pool_ctx_t *pool_ctx = &memory_pool_ctx;
    struct memory_pool_t *first, *pool, *pre_pool;
#if (WQ_CONFIG_64 == 1)
        IOT_ASSERT(ptr && (size > 8) && (((uint64_t)ptr & 0x3) == 0));
#else
        IOT_ASSERT(ptr && (size > 8) && (((uint32_t)ptr & 0x3) == 0));
#endif

    mmal_printf("mmal: pool attach type %d dma %d size %d\n", type,
                                                              suppDma,
                                                              size);

    first = &pool_ctx->first;
    pool = first;

    /* not first memory block attached */
    if (pool_ctx->pool_num) {
        do {
            if (((ptr >= pool->start) &&
                 (ptr < (pool->start + pool->total_size))) ||
                (((ptr + size) > pool->start) &&
                 ((ptr + size) <= (pool->start + pool->total_size)))) {
                /* overlapping check fail */
                IOT_ASSERT(0);
            }

            pre_pool = pool;
            pool = pool->next;
        } while (pool);

        /* allocate context from first pool */
        pool = _alloc_memory_pool(first, sizeof(struct memory_pool_t));
        IOT_ASSERT(pool);

        pre_pool->next = pool;    
    }

    pool->next = NULL;
    pool->alloc = NULL;
    pool->total_size = size;
    pool->used_size = 0;
    pool->start =
    pool->cur = ptr;

    MEMPOOL_CLR_ATTR(pool);
    MEMPOOL_SET_TYPE(pool, type);
    MEMPOOL_SET_SUPP_DMA(pool, suppDma);

    pool_ctx->pool_num++;
    pool_ctx->pool_total_size += size;

    return;
}

void *mmal_pool_alloc(enum memory_type_e type,
                      int suppDma,
                      int size,
                      int align,
                      int reset)
{
#if (WQ_CONFIG_64 == 1)
    uint64_t caller = (uint64_t)CALLER_ADDR();
#else
    uint32_t caller = (uint32_t)CALLER_ADDR();
#endif
    struct memory_pool_ctx_t *pool_ctx = &memory_pool_ctx;
    struct memory_pool_t *pool;
    struct memory_pool_info_t *new, *pool_info, *pre_pool_info;
    uint8_t *p = NULL;
    int allocSize;

    IOT_ASSERT(size);

    mmal_printf("mmal: pool alloc type %d DMA %d sz %d align %d reset %d\n",
                type, suppDma, size, align, reset);

    /* not only size but also start address sync to alignment requirement */

    if (align == 0)
        align = MM_BASIC_ALIGN;

    allocSize = size + align + sizeof(struct memory_pool_info_t);
    pool = _find_memory_pool(pool_ctx, type, suppDma, allocSize);
    if (!pool) {
        WARN_ON(1);
        iot_printf("mmal: type %d suppDma %d allocSize %d\n", type,
                                                              suppDma,
                                                              allocSize);
        mmal_pool_info("ALLOC_POOL_FAIL");
    } else {
        new = (struct memory_pool_info_t *)_alloc_memory_pool(pool, allocSize);
        if (!new) {
            WARN_ON(1);
            mmal_pool_info("ALLOC_MEM_FAIL");
        } else {
            p = (uint8_t *)new + sizeof(struct memory_pool_info_t);
#if (WQ_CONFIG_64 == 1)
            p = (uint8_t *)ALIGN((uint64_t)p, align);
            os_mem_set((void *)p, 0, size); //Memo: set to 0 to avoid memory not init
            new->addr = (uint64_t)p;
#else
            p = (uint8_t *)ALIGN((uint32_t)p, align);
            os_mem_set((void *)p, 0, size); //Memo: set to 0 to avoid memory not init
            new->addr = (uint32_t)p;
#endif
            new->next = NULL;
            new->caller = caller;
            new->size = size;

            pool_info = pool->alloc;
            if (pool_info) {
                do {
                    pre_pool_info = pool_info;
                    pool_info = pool_info->next;
                } while (pool_info);
                pre_pool_info->next = (struct memory_pool_info_t *)new;
            } else
                pool->alloc = new;

            pool_ctx->pool_user_alloc += size;
            pool->used_size += allocSize;
        }
    }

    return (void *)(p);
}

void mmal_pool_info(char *note)
{
    struct memory_pool_ctx_t *pool_ctx = &memory_pool_ctx;
    struct memory_pool_t *pool = &pool_ctx->first;
    struct memory_pool_info_t *pool_info;

    if (note)
        iot_printf("MEM-POOL - %s\n", note);
    iot_printf("Pool Number   - %d\n", pool_ctx->pool_num);
    iot_printf("Total Size    - %d\n", pool_ctx->pool_total_size);
    iot_printf("User Allocate - %d\n", pool_ctx->pool_user_alloc);

    if (pool == NULL)
        return;

    do {
#if(WQ_CONFIG_64 == 1)
        iot_printf("[0x%08llx - 0x%08llx] type %d %s total %dB, now %ldB, used %ldB\n",
                   (uint64_t)pool->start,
                   (uint64_t)(pool->start + pool->total_size),
                   MEMPOOL_GET_TYPE(pool),
                   MEMPOOL_GET_SUPP_DMA(pool) ? "supp DMA" : " ",
                   pool->total_size,
                   (pool->total_size - (pool->cur - pool->start)),
                   pool->cur - pool->start);
#else
        iot_printf("[0x%08x - 0x%08x] type %d %s total %dB, now %dB, used %dB\n",
                   (uint32_t)pool->start,
                   (uint32_t)(pool->start + pool->total_size),
                   MEMPOOL_GET_TYPE(pool),
                   MEMPOOL_GET_SUPP_DMA(pool) ? "supp DMA" : " ",
                   pool->total_size,
                   (pool->total_size - (pool->cur - pool->start)),
                   pool->cur - pool->start);
#endif
        pool_info = pool->alloc;
        while(pool_info) {
        #if (WQ_CONFIG_64 == 1)
            iot_printf(" |CALLER 0x%llx ADDR 0x%llx SZ %d\n", pool_info->caller,
                                                          pool_info->addr,
                                                          pool_info->size);
        #else
            iot_printf(" |CALLER 0x%x ADDR 0x%x SZ %d\n", pool_info->caller,
                                              pool_info->addr,
                                              pool_info->size);
        #endif
            pool_info = pool_info->next;
        }

        pool = pool->next;
    } while(pool);

    return;
}

static struct memory_chunk_ctx_t memory_chunk_ctx;

static inline int __mmal_chunk_alloc_size(int size)
{
    int sz;

    sz = ALIGN(size, MM_BASIC_ALIGN) + sizeof(struct memory_chunk_t);

#ifdef CONFIG_MMAL_DEBUG

#if (WQ_CONFIG_64 == 1)
    sz += sizeof(uint64_t);
#else
    sz += sizeof(uint32_t);
#endif

#endif
    return sz;
}

int _mmal_chunk_calc_pool_size(struct memory_chunk_info_t *chunk_info)
{
    int i, pool_sz = 0;

    for (i = 0; i < MM_CHUNK_MEM_NUM; i++) {
        int sz = __mmal_chunk_alloc_size(chunk_info->member[i].size);

        pool_sz += (sz * chunk_info->member[i].number);
    }

    return pool_sz;
}

static inline void _mmal_chunk_init(struct memory_chunk_member_t *chunk_mem,
                                    struct memory_chunk_t *chunk)
{
#ifdef CONFIG_MMAL_DEBUG
    chunk->caller = 0;
    chunk->cookie_s = MM_CHUNK_COOKIE_S;
    chunk->size = ULONG_MAX;
#endif

    chunk->chunk_mem = chunk_mem;

    return;
}

void mmal_chunk_init(struct memory_chunk_info_t *chunk_info)
{
    struct memory_chunk_ctx_t *chunk_ctx = &memory_chunk_ctx;
    struct memory_chunk_member_t *chunk_mem;
    struct memory_chunk_t *chunk;
    int i, j, a = 0;
    int pre_sz = 0;;

    chunk_ctx->pool_sz = _mmal_chunk_calc_pool_size(chunk_info);

    chunk_ctx->pool_start = (void *)mmal_pool_alloc(MEMPOOL_TYPE_UNKNOW,
                                                    0,
                                                    chunk_ctx->pool_sz,
                                                    1,
                                                    0);
    IOT_ASSERT(chunk_ctx->pool_start);

    osal_locker_create(&chunk_ctx->big_lock);
    chunk = (struct memory_chunk_t *)chunk_ctx->pool_start;
    for (i = 0, chunk_mem = chunk_ctx->chunk_mem;
         i < MM_CHUNK_MEM_NUM;
         i++, chunk_mem++) {
        int num = chunk_info->member[i].number;
        int sz = chunk_info->member[i].size;
        int chunk_sz = __mmal_chunk_alloc_size(chunk_info->member[i].size);

        IOT_ASSERT(num);
        IOT_ASSERT(sz > pre_sz);

        pre_sz = sz;
        chunk_mem->size = sz;
        SCLIST_INIT(chunk_mem->head);

        for (j = 0; j < num; j++) {
            _mmal_chunk_init(chunk_mem, chunk);
            SCLIST_INSERT_TAIL(chunk_mem->head, chunk, entry);

            chunk = (struct memory_chunk_t *)((uint8_t *)chunk + chunk_sz);
        }
		
		//init low_ water mark
        chunk_mem->head.__low_water_mark = num;

        if (chunk_info->member[i].is_anchor) {
            IOT_ASSERT(a != MM_CHUNK_LVL_NUM);
            chunk_ctx->chunk_lvl[a++] = chunk_mem;
        }
    }

    IOT_ASSERT((uint8_t *)chunk <=
               (uint8_t *)chunk_ctx->pool_start + chunk_ctx->pool_sz);

    /* size=0 means last member */
    SCLIST_INIT(chunk_mem->head);
    chunk_mem->size = 0;

    IOT_ASSERT(a == MM_CHUNK_LVL_NUM);
    chunk_ctx->chunk_lvl[a] = NULL;

    return;
}

void mmal_get_stat(uint32_t *stat)
{
    struct memory_chunk_ctx_t *chunk_ctx = &memory_chunk_ctx;
    struct memory_chunk_member_t *chunk_mem;
    int i;

    for (i = 0, chunk_mem = chunk_ctx->chunk_mem;
         i < MM_CHUNK_MEM_NUM;
         i++, chunk_mem++) {
        stat[i]=SCLIST_CNT(chunk_mem->head);
    }
}


void mmal_chunk_info(void)
{
    struct memory_chunk_ctx_t *chunk_ctx = &memory_chunk_ctx;
    struct memory_chunk_member_t *chunk_mem;
    int i;

#if (WQ_CONFIG_64 == 1)
    iot_printf("Pool %p - %p/%d chunk-hdr_sz %ld\n",
                chunk_ctx->pool_start,
                (uint8_t *)chunk_ctx->pool_start + chunk_ctx->pool_sz,
               chunk_ctx->pool_sz,
               sizeof(struct memory_chunk_t));

#else
    iot_printf("Pool %p - %p/%d chunk-hdr_sz %d\n",
                chunk_ctx->pool_start,
                (uint8_t *)chunk_ctx->pool_start + chunk_ctx->pool_sz,
               chunk_ctx->pool_sz,
               sizeof(struct memory_chunk_t));
#endif
    for (i = 0; i < MM_CHUNK_LVL_NUM; i++) {
        chunk_mem = chunk_ctx->chunk_lvl[i];
        iot_printf("|chunk-lvl-%d sz %d\n", i, chunk_mem->size);
    }

    iot_printf("|\n");
    for (i = 0, chunk_mem = chunk_ctx->chunk_mem;
         i < MM_CHUNK_MEM_NUM;
         i++, chunk_mem++) {
        iot_printf("|chunk-mem-%p sz=%d free=%d/%d  lwm=%d\n", chunk_mem,
                                                   chunk_mem->size, 
                                                   SCLIST_CNT(chunk_mem->head),
                                                   memory_chunk_info.member[i].number,
												   chunk_mem->head.__low_water_mark
                                                   );
#if 0
        {
            struct memory_chunk_t *chunk;

            SCLIST_FOREACH(chunk_mem->head, chunk, entry) {
                iot_printf(" @chunk %p caller 0x%x cookie 0x%x sz %d buf %p member %p\n",
                                    chunk,
                                    chunk->caller,
                                    chunk->cookie_s,
                                    chunk->size == ULONG_MAX ? 0 : chunk->size,
                                    chunk->buffer,
                                    chunk->chunk_mem);
            }
        }
#endif
    }

    return;
}



void *_mmal_malloc(unsigned int size)
{
    struct memory_chunk_ctx_t *chunk_ctx = &memory_chunk_ctx;
    struct memory_chunk_member_t *chunk_mem;
    struct memory_chunk_t *chunk = NULL;
    int i = 0;
	#ifdef CONFIG_MMAL_DEBUG
    	#if (WQ_CONFIG_64 == 1)
            uint64_t fence_pattern;
    	#else
            uint32_t fence_pattern;
        #endif
	#endif
#if (WQ_CONFIG_64 == 1)
    WARN(size == 0, "mmal: alloc size ZERO, caller 0x%llx\n", (uint64_t)CALLER_ADDR());
#else
    WARN(size == 0, "mmal: alloc size ZERO, caller 0x%x\n", (uint32_t)CALLER_ADDR());
#endif
    chunk_mem = chunk_ctx->chunk_lvl[MM_CHUNK_LVL_NUM - 1];
    do {
        if (size < chunk_ctx->chunk_lvl[i]->size) {
            if (i == 0)
                chunk_mem = chunk_ctx->chunk_mem;
            else
                chunk_mem = chunk_ctx->chunk_lvl[i - 1];

            break;
        }
    } while (chunk_ctx->chunk_lvl[++i]);

    do {
        if (chunk_mem->size >= size) {
            if (SCLIST_CNT(chunk_mem->head)) {
                osal_locker_lock(&chunk_ctx->big_lock);
                SCLIST_REMOVE_HEAD(chunk_mem->head, chunk, entry);
				//update low water mark
                if (chunk_mem->head.__count < chunk_mem->head.__low_water_mark)
                    chunk_mem->head.__low_water_mark = chunk_mem->head.__count;
                osal_locker_unlock(&chunk_ctx->big_lock);

                if (chunk)
                    break;
            }
        }
    } while((++chunk_mem)->size);

    if (chunk) {
        mmal_printf("mmal: malloc %p sz %d chunk sz %d\n", chunk->buffer,
                                                           size,
                                                           chunk_mem->size);

#ifdef CONFIG_MMAL_DEBUG
        BUG_ON(chunk->cookie_s != MM_CHUNK_COOKIE_S);
        BUG_ON(chunk->size != ULONG_MAX);
        BUG_ON(size > chunk_mem->size);

        //*((uint32_t *)(chunk->buffer + size)) = MM_CHUNK_COOKIE_E;
        fence_pattern = MM_CHUNK_COOKIE_E;
        os_mem_cpy(chunk->buffer + size, &fence_pattern, sizeof(fence_pattern));
#if (WQ_CONFIG_64 == 1)
        chunk->caller = (uint64_t)CALLER_ADDR();
#else
        chunk->caller = (uint32_t)CALLER_ADDR();
#endif
        chunk->size = size;
#endif
        os_mem_set(chunk->buffer,0,size);//Memo: set to 0 to avoid memory not init
        return (void *)chunk->buffer;
    } else {
        WARN_ON(1);

        if (chunk_mem->size == 0) {
#if (WQ_CONFIG_64 == 1)
            iot_printf("mmal: can't service request size %d caller 0x%llx\n", size, (uint64_t)CALLER_ADDR());
#else
            iot_printf("mmal: can't service request size %d caller 0x%x\n", size, (uint32_t)CALLER_ADDR());
#endif
            mmal_pool_info("mmal_pool_info");
            mmal_chunk_info();
            dump_mem_record();

            IOT_ASSERT(0);
        }

        return NULL;
    }
}


void *_mmal_malloc_dbg(unsigned int size, const char *function_name, int line_num)
{
    void *addr=NULL;
    //iot_printf("mmal_malloc %s(%d) line:%d\n", function_name, size, line_num);
    addr=_mmal_malloc(size);
    if(addr)
        add_mem_record(size,function_name,line_num,addr);
    return addr;
}

void mmal_free(void *ptr)
{
    struct memory_chunk_ctx_t *chunk_ctx = &memory_chunk_ctx;
    struct memory_chunk_member_t *chunk_mem;
    struct memory_chunk_t *chunk;
    #ifdef CONFIG_MMAL_DEBUG
        #if (WQ_CONFIG_64 == 1)
            uint64_t fence_pattern;
        #else
            uint32_t fence_pattern;
        #endif
    #endif

    if (ptr == NULL)
        return;

    chunk = (struct memory_chunk_t *)((uint8_t *)ptr -
                                      sizeof(struct memory_chunk_t));
    chunk_mem = (struct memory_chunk_member_t *)(chunk->chunk_mem);

    mmal_printf("mmal: free %p chunk sz %d\n", chunk->buffer, chunk_mem->size);

#ifdef CONFIG_MMAL_DEBUG
    BUG_ON(chunk->cookie_s != MM_CHUNK_COOKIE_S);
    BUG_ON(chunk->size == ULONG_MAX);
    //BUG_ON(*((uint32_t *)(chunk->buffer + chunk->size)) != MM_CHUNK_COOKIE_E);
    fence_pattern = MM_CHUNK_COOKIE_E;
    BUG_ON (os_mem_cmp(chunk->buffer + chunk->size, &fence_pattern , sizeof(fence_pattern)) != 0);
    BUG_ON(chunk->size > chunk_mem->size);

    chunk->size = ULONG_MAX;
#endif

    osal_locker_lock(&chunk_ctx->big_lock);
    SCLIST_INSERT_TAIL(chunk_mem->head, chunk, entry);
    osal_locker_unlock(&chunk_ctx->big_lock);

    del_mem_record(ptr);
    return;
}

void mmal_info(void)
{
    mmal_pool_info("mmal_pool_info");
    mmal_chunk_info();
    dump_mem_record();
}

#include "oss.h"
#include <linux/version.h>
#include <linux/vmalloc.h>

static int mbuf_empty_cnt = 0;
static int mbuf_skb_empty_cnt = 0;
static struct mbuf_op_ctx_t mbuf_op_ctx;

extern void dump_bytes(char *note, uint8_t *data, int len);
extern int wq_ipc_headroom_size();
extern void wq_free_skb(struct sk_buff *skb);
extern int wq_get_desc_size(void);

struct sk_buff *wq_alloc_skb(unsigned int length);

#define TXPKT_MAC_HEADROOM_NEEDED       (32 + wq_get_desc_size())  //sizeof(DESC) + 8(IV) + 24(3->11 header, 36-12)
#define SKB_MAX_LEN    3893                //max size of skb 

uint8_t mbuf_get_pre_leading(void);
void m_freem_c(struct mbuf *m);

void kvfree(const void *addr)
{
    if (is_vmalloc_addr(addr))
        vfree(addr);
    else
        kfree(addr);
}

void mbuf_reset_stat(void)
{
    mbuf_empty_cnt = 0;
    mbuf_skb_empty_cnt = 0;
}

void mbuf_stat(void)
{
    iot_printf("mbuf alloc count: %d %d\n", mbuf_empty_cnt, mbuf_skb_empty_cnt);
}

struct mbuf *_m_gethdr_with_skb(int how, short type, struct sk_buff *skb, int len)
{
    struct mbuf *m;

    m = (struct mbuf *)wq_mem_alloc(sizeof(struct mbuf), 1);
    if (!m) {
        mbuf_empty_cnt++;
        return NULL;
    }

    m->m_data = skb->data;
    m->m_len = len;
    m->m_type = type;
    m->skb = skb;
    m->poison = MBUF_DEBUG_POSION;

    //set magic pattern in the end of iot packet for corruption detection
    //*(uint32_t*)(skb->head + MBUF_ALLOC_SIZE - 4) = 0xf05a00ff;

    /* TODO: reserved leading space according to type */
   // m->m_data += pre_leading;

    //mbuf_printf("mbuf: alloc mbuf %p pre_leading %d\n", m, pre_leading);

    return m;    
}

struct mbuf *_m_gethdr(int how, short type)
{
    struct mbuf *m;
    struct sk_buff *skb;

    //int pre_leading = mbuf_op_ctx.pre_leading;
    m = (struct mbuf *)wq_mem_alloc(sizeof(struct mbuf), 1);
    if (!m) {
        mbuf_empty_cnt++;
        return NULL;
    }
    skb = wq_alloc_skb(MBUF_ALLOC_SIZE);
    skb_reserve(skb, TXPKT_MAC_HEADROOM_NEEDED + wq_ipc_headroom_size());
    
    if (!skb) {
        mbuf_skb_empty_cnt++;
        kvfree(m);
        return NULL;
    }

    m->m_data = skb->data;
    m->m_type = type;
    m->skb = skb;
    m->poison = MBUF_DEBUG_POSION;

    //set magic pattern in the end of iot packet for corruption detection
    //*(uint32_t*)(skb->head + MBUF_ALLOC_SIZE - 4) = 0xf05a00ff;

    /* TODO: reserved leading space according to type */
   // m->m_data += pre_leading;

    mbuf_printf("mbuf: alloc mbuf %p pre_leading %d\n", m, pre_leading);

    return m;
}

struct mbuf *_m_gethdr_amsdu(int how, short type)
{
    struct mbuf *m;
    struct sk_buff *skb;

    //int pre_leading = mbuf_op_ctx.pre_leading;
    m =(struct mbuf *)wq_mem_alloc(sizeof(struct mbuf), 1);
    if (!m) {
        mbuf_empty_cnt++;
        return NULL;
    }
    skb = wq_alloc_skb(SKB_MAX_LEN);
    skb_reserve(skb, TXPKT_MAC_HEADROOM_NEEDED + wq_ipc_headroom_size());
    
    if (!skb) {
        mbuf_skb_empty_cnt++;
        kvfree(m);
        return NULL;
    }

    m->m_data = skb->data;
    m->m_type = type;
    m->skb = skb;
    m->poison = MBUF_DEBUG_POSION;

    //set magic pattern in the end of iot packet for corruption detection
    //*(uint32_t*)(skb->head + MBUF_ALLOC_SIZE - 4) = 0xf05a00ff;

    /* TODO: reserved leading space according to type */
   // m->m_data += pre_leading;

    mbuf_printf("mbuf: alloc mbuf %p pre_leading %d\n", m, pre_leading);

    return m;
}

struct sk_buff *wq_alloc_skb_amsdu(int len_skb)
{
    struct sk_buff *skb;
    skb = wq_alloc_skb(len_skb);
    skb_reserve(skb, TXPKT_MAC_HEADROOM_NEEDED + wq_ipc_headroom_size());

    if (!skb) {
        mbuf_skb_empty_cnt++;
        printk("%s:%d skb_amsdu alloc failed\n",__func__,__LINE__);
        return NULL;
    }

    return skb;
}

#ifdef DEBUG_MBUF_TRACKING
#define MBUF_TRACKING_TABLE_SIZE 23
extern void os_enable_irq();
extern void os_disable_irq();

struct mbuf_tracking {
    void* ra1;
    void* ra0;
    struct mbuf* m;
};

struct mbuf_tracking mbuf_tracking_table[MBUF_TRACKING_TABLE_SIZE];

void init_mbuf_tracking_table(void)
{
    int i;

    for (i = 0; i < MBUF_TRACKING_TABLE_SIZE; i++)
    {
        mbuf_tracking_table[i].m = NULL;
    }
}

void add_to_mbuf_tracking_table(struct mbuf* m, void* ra1, void* ra0)
{
    int i;

    os_disable_irq();
    for (i = 0; i < MBUF_TRACKING_TABLE_SIZE; i++)
    {
        if (mbuf_tracking_table[i].m == NULL)
        {
            mbuf_tracking_table[i].m = m;
            mbuf_tracking_table[i].ra1 = ra1;
            mbuf_tracking_table[i].ra0 = ra0;
            os_enable_irq();
            return;
        }
    }
    os_enable_irq();

    IOT_ASSERT(0); //TABLE FULL, shall not happen
    return;
}

void del_from_mbuf_tracking_table(struct mbuf* m)
{
    int i;

    os_disable_irq();
    for (i = 0; i < MBUF_TRACKING_TABLE_SIZE; i++)
    {
        if (mbuf_tracking_table[i].m == m)
        {
            mbuf_tracking_table[i].m = NULL;
            os_enable_irq();
            return;
        }
    }
    os_enable_irq();

    IOT_ASSERT(0); //mbuf not found, shall not happen
    return;
}

void print_mbuf_tracking_table(void)
{
    int i;

    iot_printf("print_mbuf_tracking_table start\n");
    for (i = 0; i < MBUF_TRACKING_TABLE_SIZE; i++)
    {
        if (mbuf_tracking_table[i].m != NULL)
        {
            iot_printf("%p->%p:%p\n", mbuf_tracking_table[i].ra1, mbuf_tracking_table[i].ra0, mbuf_tracking_table[i].m);
        }
    }
    iot_printf("print_mbuf_tracking_table done\n");
}

struct mbuf *m_gethdr_debug(int how, short type, void* ra)
{
    struct mbuf* m;
    m = _m_gethdr(how, type);

    if (m != NULL)
        add_to_mbuf_tracking_table(m, ra, __builtin_return_address(0));

    //iot_printf("m_gethdr %p->%p:%p\n", ra, __builtin_return_address(0), m);
    return m;
}

struct mbuf *_m_gethdr_amsdu_debug(int how, short type, void* ra)
{
    struct mbuf* m;
    m = _m_gethdr_amsdu(how, type);

    if (m != NULL)
        add_to_mbuf_tracking_table(m, ra, __builtin_return_address(0));

    //iot_printf("m_gethdr %p->%p:%p\n", ra, __builtin_return_address(0), m);
    return m;
}
#else
void print_mbuf_tracking_table(void)
{
}

void del_from_mbuf_tracking_table(struct mbuf* m)
{
}
#endif

/* Free chained mbuf in m->m_next */
void m_freem_c(struct mbuf *m)
{
    struct mbuf *free_m;
    struct mbuf *next_m;

    free_m = m;
    while (free_m != NULL) {
        next_m = free_m->m_next;


        mbuf_printf("mbuf: free_c mbuf %p\n", free_m);
        del_from_mbuf_tracking_table(free_m);

        //check magic pattern in the end of iot packet for corruption detection
        //IOT_ASSERT((*(uint32_t*)(free_m->skb->head + MBUF_ALLOC_SIZE - 4)) == 0xf05a00ff);
        
        wq_free_skb(free_m->skb);
        kvfree(free_m);

        free_m = next_m;
    }

    return;
}

void m_freem(struct mbuf *m)
{
    struct mbuf *free_m;
    struct mbuf *next_m;

    /* for frames which are not transmitted/completed */
    if (m->cbfunc) {
        m->cbfunc(m->cbni, m->cbarg, 0);
    }
#if 1
    //Debug information for error condition
    if(m->poison != MBUF_DEBUG_POSION){
       iot_printf("mbuf at :0x%p, data at 0x%p, len:%d\n",m, m->m_data,m->m_len);
       dump_bytes("mbuf position",(uint8_t *) &m->poison,4);
       dump_bytes("mbuf",(uint8_t *)m,sizeof(struct mbuf));
    }

    BUG_ON(m->poison != MBUF_DEBUG_POSION);
#endif
    if(m->m_next){
        return m_freem_c(m);    
    }

    free_m = m;
    while (free_m != NULL) {
        next_m = free_m->m_nextpkt;

        mbuf_printf("mbuf: free mbuf %p\n", free_m);
        del_from_mbuf_tracking_table(free_m);

        //check magic pattern in the end of iot packet for corruption detection
        //if(*(uint32_t*)(free_m->skb->head + MBUF_ALLOC_SIZE - 4) != 0xf05a00ff){
        //    mbuf_dump(free_m);
        //    IOT_ASSERT(0);
        //}

        wq_free_skb(free_m->skb);
        kvfree(free_m);

        free_m = next_m;
    }

    return;
}

void m_freem_wo_skb(struct mbuf *m)
{
    struct mbuf *free_m;

    /* for frames which are not transmitted/completed */
    if (m->cbfunc) {
        m->cbfunc(m->cbni, m->cbarg, 0);
    }
#if 1
    //Debug information for error condition
    if(m->poison != MBUF_DEBUG_POSION){
       iot_printf("mbuf at :0x%p, data at 0x%p, len:%d\n",m, m->m_data,m->m_len);
       dump_bytes("mbuf position",(uint8_t *) &m->poison,4);
       dump_bytes("mbuf",(uint8_t *)m,sizeof(struct mbuf));
    }

    BUG_ON(m->poison != MBUF_DEBUG_POSION);
#endif

    IOT_ASSERT(m->m_next == NULL);
    IOT_ASSERT(m->m_nextpkt == NULL);

    free_m = m;

    del_from_mbuf_tracking_table(free_m);
	
	//check magic pattern in the end of iot packet for corruption detection
    //if(*(uint32_t*)(free_m->skb->head + MBUF_ALLOC_SIZE - 4) != 0xf05a00ff){
    //    mbuf_dump(free_m);
    //    IOT_ASSERT(0);
    //}

    kvfree(free_m);
		
    return;
}

struct mbuf *m_dup(const struct mbuf *m, int how)
{
    struct mbuf *new_m;

    if (m->m_flags_ext & M_AMSDU)
        new_m = m_gethdr_amsdu(how, MT_DATA);
    else
        new_m = m_gethdr(how, MT_DATA);

    BUG_ON(m->m_next);

    if (new_m) {

        /* TODO: other new mbuf for chain */
        IOT_ASSERT(m->m_nextpkt == NULL);

        new_m->m_flags = m->m_flags;
        new_m->m_flags_ext = m->m_flags_ext;
        new_m->m_type = m->m_type;

        /* let new allocated mbuf have the same headroom as original one */
        new_m->m_data = new_m->skb->head + (m->m_data - m->skb->head);
        
        os_mem_cpy(new_m->m_data, m->m_data, m->m_len);
        new_m->m_len = m->m_len;

        new_m->m_pkthdr.rcvif = m->m_pkthdr.rcvif;
        new_m->m_pkthdr.csum_data = m->m_pkthdr.csum_data;
        new_m->m_pkthdr.ether_vtag = m->m_pkthdr.ether_vtag;
        new_m->m_pkthdr.tso_segsz = m->m_pkthdr.tso_segsz;
        //new_m->rxs = m->rxs;
        //new_m->c_pktflags = m->c_pktflags;
    	new_m->c_nf = m->c_nf;
    	new_m->c_rssi = m->c_rssi;
    	//new_m->c_freq = m->c_freq;
    }

    return new_m;
}

struct mbuf *m_prepend(struct mbuf *m, uint32_t len, int how)
{
    if (M_LEADINGSPACE(m) >= len) {
        m->m_data -= len;
        m->m_len += len;

        return m;
    } else {
        return NULL;
    }
}

int m_append(struct mbuf *m, uint32_t len, const uint8_t *cp)
{
    uint8_t *data_tail = m->m_data + m->m_len;

    if (M_TRAILINGSPACE(m) >= len) {
        os_mem_cpy(data_tail, cp, len);
        m->m_len += len;
    } else {
        IOT_ASSERT(0);
    }

    return 0;
}

void m_catpkt(struct mbuf *m, struct mbuf *n)		/* concatenate */
{
    //make sure there is enough space to concatenate
    IOT_ASSERT((uint32_t)M_TRAILINGSPACE(m) >= n->m_len);
    
    /* copy data from one into the other */
    os_mem_cpy(mtod(m, caddr_t) + m->m_len, mtod(n, caddr_t), n->m_len);
    /* update buf len*/
    m->m_len += n->m_len;

    m_freem(n);
}

void mbuf_pre_leading(int pre_leading)
{
    struct mbuf_op_ctx_t *op_ctx = &mbuf_op_ctx;

    if (pre_leading > op_ctx->pre_leading) {
        mbuf_printf("mbuf: set pre_leading %d -> %d\n",
                                              op_ctx->pre_leading, pre_leading);

        op_ctx->pre_leading = pre_leading;
    }

    return;
}

uint8_t mbuf_get_pre_leading(void)
{
    return mbuf_op_ctx.pre_leading;    
}

void mbuf_dump_data(char *note, struct mbuf *m)
{
	char buf[512];
	char i2hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	int i;
	int len;
	
	len = m->m_len;
	if (len > 100)
		len = 100;
	
	for (i=0; i<len; i++)
	{
		buf[i*3] = i2hex[m->m_data[i] >> 4];
		buf[i*3+1] = i2hex[m->m_data[i] & 0xf];
		buf[i*3+2] = ' ';
	}
	buf[i*3] = 0;
	iot_printf("%s len=%d buf:%s\n", note, m->m_len, buf);

}

void mbuf_init(int tag0_num, int tag1_num, int tag2_num, int tag3_num)
{
#if (WQ_CONFIG_64 == 1)
    iot_printf("mbuf_sz=%ld MBUF_ALLOC_SIZE=%ld\n", sizeof(struct mbuf), MBUF_ALLOC_SIZE);
#else
    iot_printf("mbuf_sz=%d MBUF_ALLOC_SIZE=%d\n", sizeof(struct mbuf), MBUF_ALLOC_SIZE);
#endif

#ifdef DEBUG_MBUF_TRACKING
    init_mbuf_tracking_table();
#endif

    return;
}

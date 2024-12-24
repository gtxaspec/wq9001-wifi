#ifndef _SCLIST_H
#define _SCLIST_H

#define SCLIST_HEAD(elemType)                        \
    struct {                                         \
        struct elemType *__listHead, *__listTail;    \
        int __count;                                 \
        int __low_water_mark;                        \
    }

#define SCLIST_ENTRY(elemType)         \
    struct {                           \
        struct elemType *next;         \
    }

#define SCLIST_INIT(head)                           \
    do {                                            \
        head.__listHead = head.__listTail = NULL;   \
        head.__count = 0;                           \
    } while(0)

#define SCLIST_CNT(head)             (head.__count)
#define SCLIST_EMPTY(head)           ((head).__count == 0)
#define SCLIST_FIRST(head)           ((head).__listHead)

#define SCLIST_FOREACH(head, elem, field)   \
    for (elem = head.__listHead;            \
         elem != NULL;                      \
         elem = elem->field.next)
		
#define SCLIST_INSERT_TAIL(head, elem, field)                       \
    do {                                                            \
        elem->field.next = NULL;				                    \
        if (head.__listHead == NULL) { head.__listHead = elem; }    \
        else { head.__listTail->field.next = elem; }                \
        head.__listTail = elem; head.__count++;                     \
    } while(0)

#define SCLIST_REMOVE_HEAD(head, elem, field)   \
    do {                                        \
        elem = head.__listHead;                 \
        if (elem) {                             \
            head.__listHead = elem->field.next; \
            elem->field.next = NULL;            \
            head.__count--;                     \
        }                                       \
    } while(0)

#define SCLIST_PEEK(head, elem)                 \
    do {                                        \
        if (SCLIST_EMPTY(head)) {               \
            elem = NULL;                        \
        } else {                                \
            elem = head.__listHead;             \
        }                                       \
    } while(0)
#endif

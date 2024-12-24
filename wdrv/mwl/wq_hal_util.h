#ifndef _HAWK_UTIL_H
#define _HAWK_UTIL_H

/* node table arrangement
 * slot 0: STA group key
 * slot 1: AP group key
 * slot 2 ~ (WQ_NODE_TABLE_SZ-2): pairwise key
 * slot WQ_NODE_TABLE_SZ-1(=WEP_KEY_IDX): STA WEP key
 */
#define WEP_KEY_IDX   (WQ_NODE_TABLE_SZ - 1)

#endif //#ifndef _HAWK_UTIL_H

#ifndef CH02_LRU_H
#define CH02_LRU_H
#include "dlist.h"
// 仅访问次序，不含按key定位。p必须是本表存活的非哨兵结点。
bool MoveToFront(DList &L, DNode *p);
// 空表失败、out不变；out不得与数据域别名。
bool EvictTail(DList &L, ElemType &out);
// 教学驱动新增条目：cap>0且size<=cap，先申请成功再淘汰。
// 成功后s是新增结点的借用指针；失败时s不变。它不得别名表内链接字段。
bool AddRecent(DList &L, int cap, ElemType x, DNode *&s);
#endif

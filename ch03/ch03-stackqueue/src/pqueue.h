#ifndef CH03_PQUEUE_H
#define CH03_PQUEUE_H
#include "../include/elemtype.h"

// 优先队列 ADT：本次课只给接口，不给实现。
// ★ 实现见第 6 章 6.4（二叉堆）——今天只定“做什么”，存储结构留到学完树之后。
// 语义与普通队列完全不同：出队取的不是最早的，而是优先级最高者，与入队顺序无关。
// 这正是 ADT 的价值：接口相同，行为由语义规定，调用方不必知道内部怎么存。

// #region representation
typedef struct { ElemType data; int priority; } PQItem;

// 具体存储（数组 + 堆序）第 6 章确定，这里只给出占位表示，本课不实现任何操作。
typedef struct { PQItem *data; int size; int capacity; } PriorityQueue;
// #endregion representation

// #region interface
bool PQ_Init(PriorityQueue &Q, int cap);
void PQ_Destroy(PriorityQueue &Q);
bool PQ_Empty(const PriorityQueue &Q);
int  PQ_Length(const PriorityQueue &Q);

bool PQ_Push(PriorityQueue &Q, const PQItem &x);   // 插入
bool PQ_Pop (PriorityQueue &Q, PQItem &out);       // 取出并删除优先级最高者
const PQItem &PQ_Top(const PriorityQueue &Q);      // 只看不取
// #endregion interface
#endif

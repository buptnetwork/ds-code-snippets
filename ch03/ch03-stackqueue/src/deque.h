#ifndef CH03_DEQUE_H
#define CH03_DEQUE_H
#include "../include/elemtype.h"

// 双端队列（Deque）：两端都能插入、都能删除的受限线性表。用循环数组实现最自然。
// 关键观察：只用 PushBack+PopBack 就是栈；只用 PushBack+PopFront 就是队列——栈和队列都是它的特例。
// front = 队头元素下标；rear = 队尾元素的下一个位置；size = 当前元素个数（与循环队列一致）。
// #region representation
typedef struct {
    ElemType *data;
    int front;          // 队头元素下标
    int rear;           // 队尾元素的下一个位置
    int size;
    int capacity;
} Deque;
// #endregion representation

// 错误返回约定：任一 Push 在满或内存不足时返回 false 且原队不变；任一 Pop 在空队时返回 false。
// #region interface
bool DQ_Init(Deque &D, int cap);
void DQ_Destroy(Deque &D);

bool DQ_Empty (const Deque &D);
bool DQ_Full  (const Deque &D);
int  DQ_Length(const Deque &D);

bool DQ_PushFront(Deque &D, const ElemType &e);   // 队头插入
bool DQ_PushBack (Deque &D, const ElemType &e);   // 队尾插入
bool DQ_PopFront (Deque &D, ElemType &out);       // 队头删除
bool DQ_PopBack  (Deque &D, ElemType &out);       // 队尾删除

const ElemType &DQ_Front(const Deque &D);   // 只看不取：队头（空队时诊断并 exit）
const ElemType &DQ_Back (const Deque &D);   // 只看不取：队尾
// #endregion interface
#endif

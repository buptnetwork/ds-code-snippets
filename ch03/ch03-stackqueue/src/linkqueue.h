#ifndef CH03_LINKQUEUE_H
#define CH03_LINKQUEUE_H
#include "../include/elemtype.h"

// 链式队列：单链表 + 头结点，同时保存队头与队尾两个指针。
// 队列两端都要 O(1)：队头=链表头（出队=删头结点），队尾由 rear 直达（入队=尾插）。
// 建议加头结点：有了它，空队与非空队入队写法完全相同，无需为“空队改 front”单列分支。
// 对照链式栈只有一个 top——队列两端都动，所以必须两个指针。
// #region representation
typedef struct QNode {
    ElemType data;
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front;       // 指向头结点
    QNode *rear;        // 指向尾结点；空队时与 front 同为头结点
    int size;
} LinkQueue;
// #endregion representation

// 错误返回约定：EnQueue 分配失败返回 false（链式队列唯一的“队满”来源）；
// DeQueue 空队返回 false 且 out 不变。
// #region interface
bool LQ_Init(LinkQueue &Q);
void LQ_Destroy(LinkQueue &Q);

bool LQ_Empty (const LinkQueue &Q);
int  LQ_Length(const LinkQueue &Q);

bool LQ_EnQueue(LinkQueue &Q, const ElemType &e);
bool LQ_DeQueue(LinkQueue &Q, ElemType &out);
// #endregion interface
#endif

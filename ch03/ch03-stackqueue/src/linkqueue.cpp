// linkqueue.cpp：链式队列实现（带头结点）。语言子集 = C 核心 + 引用形参 / 常量引用。
#include <cstdio>
#include <cstdlib>
#include "linkqueue.h"

// #region init
bool LQ_Init(LinkQueue &Q) {
    QNode *h = (QNode *)malloc(sizeof(QNode));          // 头结点，不存数据
    if (h == NULL) { Q.front = Q.rear = NULL; Q.size = 0; return false; }
    h->next = NULL;
    Q.front = Q.rear = h;                               // 空队：front 与 rear 都指向头结点
    Q.size = 0;
    return true;
}
// #endregion init

// #region query
bool LQ_Empty (const LinkQueue &Q) { return Q.front == Q.rear; }
int  LQ_Length(const LinkQueue &Q) { return Q.size; }
// #endregion query

// #region enqueue
bool LQ_EnQueue(LinkQueue &Q, const ElemType &e) {
    QNode *p = (QNode *)malloc(sizeof(QNode));
    if (p == NULL) return false;                        // 唯一的“队满”来源
    p->data = e; p->next = NULL;
    Q.rear->next = p;                                   // ① 挂到尾结点后面（空队时 rear 即头结点，写法相同）
    Q.rear = p;                                         // ② 尾指针后移
    Q.size++;
    return true;
}
// #endregion enqueue

// #region dequeue
bool LQ_DeQueue(LinkQueue &Q, ElemType &out) {
    if (Q.front == Q.rear) return false;                // 空队
    QNode *p = Q.front->next;                           // ① 第一个数据结点
    out = p->data;
    Q.front->next = p->next;                            // ② 头结点跨过它
    if (Q.rear == p) Q.rear = Q.front;                  // ③ ★ 删的是最后一个结点，rear 必须回退
    free(p);                                            // ④
    Q.size--;
    return true;
}
// #endregion dequeue

// #region destroy
void LQ_Destroy(LinkQueue &Q) {
    ElemType x;
    while (LQ_DeQueue(Q, x)) { }                        // 复用 DeQueue，利用“空队返回 false”终止
    free(Q.front);                                      // 最后释放头结点，不能漏（链式栈没有这一步）
    Q.front = Q.rear = NULL;
    Q.size = 0;
}
// #endregion destroy

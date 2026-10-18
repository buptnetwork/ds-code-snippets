// linkqueue_rear_bug.cpp：反例 3（单元 5）——LQ_DeQueue 漏掉 rear 回退。
// ★ 未定义行为：删除最后一个数据结点后，rear 仍指向已 free 的结点（悬空指针）。
//   下一次入队执行 Q.rear->next = p 就是“写已释放内存”。
// 用 AddressSanitizer 编译可稳定报 heap-use-after-free：
//   g++ -std=c++17 -g -fsanitize=address linkqueue_rear_bug.cpp -o linkqueue_rear_bug
// 不加 ASan 时可能“看起来正常运行”——那是因为被释放的内存尚未被复用，绝不能据此认为这行可省。
#include <cstdio>
#include <cstdlib>

typedef int ElemType;
typedef struct QNode { ElemType data; struct QNode *next; } QNode;
typedef struct { QNode *front; QNode *rear; int size; } LinkQueue;

static bool LQ_Init(LinkQueue &Q) {
    QNode *h = (QNode *)malloc(sizeof(QNode));
    if (!h) return false;
    h->next = NULL; Q.front = Q.rear = h; Q.size = 0; return true;
}

static bool LQ_EnQueue(LinkQueue &Q, const ElemType &e) {
    QNode *p = (QNode *)malloc(sizeof(QNode));
    if (!p) return false;
    p->data = e; p->next = NULL;
    Q.rear->next = p;            // ★ 若上一次出队漏掉 rear 回退，这里就写到了已释放的结点
    Q.rear = p; Q.size++; return true;
}

// #region dequeue-buggy
static bool LQ_DeQueue_BUGGY(LinkQueue &Q, ElemType &out) {
    if (Q.front == Q.rear) return false;
    QNode *p = Q.front->next;
    out = p->data;
    Q.front->next = p->next;
    // 缺失：if (Q.rear == p) Q.rear = Q.front;   ← 删最后一个结点时 rear 必须回退
    free(p);                     // 释放后 rear 仍指向这块内存 → 悬空
    Q.size--; return true;
}
// #endregion dequeue-buggy

int main() {
    LinkQueue Q = {}; LQ_Init(Q);
    ElemType x;
    LQ_EnQueue(Q, 5);                       // 队列：[5]，rear → 结点5
    LQ_DeQueue_BUGGY(Q, x);                 // 出队 5，free 结点5，但 rear 仍指向它（悬空）
    printf("出队 %d 后，rear 已悬空（指向已释放结点）\n", x);
    LQ_EnQueue(Q, 9);                       // ★ 触发 heap-use-after-free：Q.rear->next = 新结点
    printf("若程序还能打印这行，说明被释放内存尚未被复用——但行为已未定义。\n");
    return 0;
}

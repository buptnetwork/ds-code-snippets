// circqueue.cpp：循环队列实现（方案二 size 计数器）。语言子集 = C 核心 + 引用形参 / 常量引用 / 返回引用。
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include "circqueue.h"

// #region lifecycle
bool CQ_Init(CircQueue &Q, int cap) {
    Q = {};                                             // 先置零，得到干净的零状态
    if (cap < 1) cap = 1;                               // 至少 1 个槽
    if ((size_t)cap > SIZE_MAX / sizeof(ElemType)) return false;   // 字节数溢出防护
    Q.data = (ElemType *)malloc(sizeof(ElemType) * (size_t)cap);
    if (Q.data == NULL) { Q.front = Q.rear = Q.size = Q.capacity = 0; return false; }
    Q.front = Q.rear = Q.size = 0;
    Q.capacity = cap;
    return true;
}

void CQ_Destroy(CircQueue &Q) {
    free(Q.data);
    Q.data = NULL;
    Q.front = Q.rear = Q.size = Q.capacity = 0;         // 恢复零状态，可重复销毁
}

// 只重置 front/rear/size，不清数据：有效元素由 front 与 size 界定，界定之外不许读。
void CQ_Clear(CircQueue &Q) { Q.front = Q.rear = Q.size = 0; }
// #endregion lifecycle

// #region query
bool CQ_Empty (const CircQueue &Q) { return Q.size == 0; }
bool CQ_Full  (const CircQueue &Q) { return Q.size == Q.capacity; }
int  CQ_Length(const CircQueue &Q) { return Q.size; }   // 方案二：直接返回，O(1)
// #endregion query

// #region enqueue
bool CQ_EnQueue(CircQueue &Q, const ElemType &e) {
    if (CQ_Full(Q)) return false;                       // 真满：队列不变，由调用者决定
    Q.data[Q.rear] = e;
    Q.rear = (Q.rear + 1) % Q.capacity;                 // 关键：绕回
    Q.size++;
    return true;
}
// #endregion enqueue

// #region dequeue
bool CQ_DeQueue(CircQueue &Q, ElemType &out) {
    if (CQ_Empty(Q)) return false;                      // 下溢：把决定权交给调用者
    out = Q.data[Q.front];
    Q.front = (Q.front + 1) % Q.capacity;               // 关键：绕回
    Q.size--;
    return true;
}
// #endregion dequeue

// #region head
ElemType &CQ_Head(CircQueue &Q) {
    if (Q.size == 0) { fprintf(stderr, "CQ_Head on empty queue\n"); exit(1); }
    return Q.data[Q.front];
}
const ElemType &CQ_Head(const CircQueue &Q) {
    if (Q.size == 0) { fprintf(stderr, "CQ_Head on empty queue\n"); exit(1); }
    return Q.data[Q.front];
}
// #endregion head

// #region print
// 数组视图：按物理下标打印，仅“逻辑队列覆盖到的格子”显示值，其余（含出队留下的旧值）打 _。
// 队列视图：从 front 起按逻辑顺序打印 size 个元素。两个视图必须同时出现。
void CQ_Print(const char *tag, const CircQueue &Q) {
    printf("[%s] front=%d rear=%d size=%d cap=%d arr=[", tag, Q.front, Q.rear, Q.size, Q.capacity);
    for (int i = 0; i < Q.capacity; ++i) {
        bool used = false;
        for (int k = 0; k < Q.size; ++k)
            if ((Q.front + k) % Q.capacity == i) { used = true; break; }
        if (used) printf("%d", Q.data[i]); else printf("_");
        if (i + 1 < Q.capacity) printf(", ");
    }
    printf("] queue(front->rear)=[");
    for (int k = 0; k < Q.size; ++k) {
        printf("%d%s", Q.data[(Q.front + k) % Q.capacity], k + 1 < Q.size ? ", " : "");
    }
    printf("]\n");
}
// #endregion print

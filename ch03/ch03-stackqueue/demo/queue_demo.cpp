// queue_demo.cpp：用单元 0 的统一操作序列演示循环队列（方案二）与链式队列。
// 统一序列：En 3 → En 8 → En 5 → De → De → En 9 → En 2 → En 7 → En 1 → En 6，容量 5。
// 循环队列在 En 1 后装满，En 6 应失败且两个视图均不变；链式队列无固定容量，En 6 仍成功。
#include <cstdio>
#include "../src/circqueue.h"
#include "../src/linkqueue.h"

static const int SEQ[] = {3, 8, 5, 9, 2, 7, 1, 6};   // 入队值（出队穿插其间，见 main）

int main() {
    puts("=== 循环队列（cap=5，方案二 size）：统一序列，每步打印两个视图 ===");
    CircQueue Q = {};
    CQ_Init(Q, 5);
    CQ_Print("init     ", Q);

    // 前三次入队
    for (int i = 0; i < 3; ++i) { CQ_EnQueue(Q, SEQ[i]); char t[16]; snprintf(t, sizeof t, "En %d     ", SEQ[i]); CQ_Print(t, Q); }
    // 两次出队
    for (int i = 0; i < 2; ++i) { ElemType x; CQ_DeQueue(Q, x); char t[16]; snprintf(t, sizeof t, "De ->%d  ", x); CQ_Print(t, Q); }
    // 余下入队：9 2 7 1 6（6 应被拒绝）
    for (int i = 3; i < 8; ++i) {
        bool ok = CQ_EnQueue(Q, SEQ[i]);
        char t[24]; snprintf(t, sizeof t, "En %d %s", SEQ[i], ok ? "ok  " : "FAIL");
        CQ_Print(t, Q);
    }
    printf("最终 size=%d（预期 5），队满=%s（预期 1）\n", CQ_Length(Q), CQ_Full(Q) ? "true" : "false");
    CQ_Destroy(Q);

    puts("\n=== 链式队列：同一序列，无固定容量 ===");
    LinkQueue L = {};
    LQ_Init(L);
    for (int i = 0; i < 3; ++i) LQ_EnQueue(L, SEQ[i]);
    for (int i = 0; i < 2; ++i) { ElemType x; LQ_DeQueue(L, x); printf("De -> %d\n", x); }
    for (int i = 3; i < 8; ++i) { bool ok = LQ_EnQueue(L, SEQ[i]); printf("En %d %s\n", SEQ[i], ok ? "ok" : "FAIL"); }
    printf("最终 size=%d（预期 6：5 9 2 7 1 6 全部接受）\n", LQ_Length(L));
    printf("出队顺序：");
    ElemType x; while (LQ_DeQueue(L, x)) printf("%d ", x);
    printf("（预期 5 9 2 7 1 6，FIFO）\n");
    LQ_Destroy(L);
    return 0;
}

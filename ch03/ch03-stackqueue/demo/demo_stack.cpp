// demo_stack.cpp：对单元 0 的统一操作序列，用顺序栈与链式栈各跑一遍。
// 统一演示序列：Push 3 -> Push 8 -> Push 5 -> Pop -> Push 9 -> Pop -> Pop
// 初始容量刻意设为 1，以便观察 capacity 从 1->2->4 的扩容过程。
// 编译：c++ -std=c++17 -Wall -Wextra demo_stack.cpp ../src/seqstack.cpp ../src/linkstack.cpp -o demo_stack
#include <cstdio>
#include "../src/seqstack.h"
#include "../src/linkstack.h"

// 顺序栈按“栈底在前”打印（自然遍历方向：data[0..top-1]）。
static void dumpSeq(const char *step, const SeqStack &S) {
    printf("[seq ] %-10s size=%d capacity=%d stack(bottom->top)=[", step, S.top, S.capacity);
    for (int i = 0; i < S.top; ++i) printf("%s%d", i ? ", " : "", S.data[i]);
    printf("]\n");
}

// 链式栈按“栈顶在前”打印（自然遍历方向：top->next->...）。
static void dumpLink(const char *step, const LinkStack &S) {
    printf("[link] %-10s size=%d stack(top->bottom)=[", step, S.size);
    int i = 0;
    for (const StackNode *p = S.top; p != NULL; p = p->next, ++i)
        printf("%s%d", i ? ", " : "", p->data);
    printf("]\n");
}

int main() {
    const int ops[]   = {3, 8, 5, 0, 9, 0, 0};   // 非零=Push 该值；0=Pop
    const char *name[]= {"Push 3","Push 8","Push 5","Pop","Push 9","Pop","Pop"};

    SeqStack S = {};
    if (!SS_Init(S, 1)) { fprintf(stderr, "seq init failed\n"); return 1; }
    LinkStack L;
    LS_Init(L);

    dumpSeq("init", S);
    dumpLink("init", L);
    for (int k = 0; k < 7; ++k) {
        ElemType x;
        if (ops[k] != 0) { SS_Push(S, ops[k]); LS_Push(L, ops[k]); }
        else             { SS_Pop(S, x);       LS_Pop(L, x); }
        dumpSeq(name[k], S);
        dumpLink(name[k], L);
    }

    SS_Destroy(S);
    LS_Destroy(L);
    return 0;
}

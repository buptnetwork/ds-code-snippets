// seqstack.cpp：顺序栈实现。语言子集 = C 核心 + 引用形参 / 常量引用 / 返回引用。
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstddef>
#include <cstdint>
#include "seqstack.h"

// #region lifecycle
bool SS_Init(SeqStack &S, int cap0) {
    S = {};                                             // 先置零，得到干净的零状态
    if (cap0 < 1) cap0 = 1;                             // 至少 1 个槽
    if ((size_t)cap0 > SIZE_MAX / sizeof(ElemType)) return false;   // 字节数溢出防护
    S.data = (ElemType *)malloc(sizeof(ElemType) * (size_t)cap0);
    if (S.data == NULL) { S.top = S.capacity = 0; return false; }
    S.top = 0;
    S.capacity = cap0;
    return true;
}

void SS_Destroy(SeqStack &S) {
    free(S.data);
    S.data = NULL;
    S.top = S.capacity = 0;                             // 恢复零状态，可重复销毁
}

void SS_Clear(SeqStack &S) { S.top = 0; }               // 只重置 top，空间保留
// #endregion lifecycle

// #region query
bool SS_Empty (const SeqStack &S) { return S.top == 0; }
int  SS_Length(const SeqStack &S) { return S.top; }     // 约定 A 下 top 就是元素个数
// #endregion query

// #region push
bool SS_Push(SeqStack &S, ElemType e) {
    if (S.capacity < 1) return false;                   // 只对成功初始化的栈追加
    if (S.top == S.capacity) {                          // 满了：扩容触发条件（非硬上限）
        if (S.capacity > INT_MAX / 2) return false;     // 乘法溢出防护
        int newcap = S.capacity * 2;
        if ((size_t)newcap > SIZE_MAX / sizeof(ElemType)) return false;
        ElemType *p = (ElemType *)realloc(S.data, sizeof(ElemType) * (size_t)newcap);
        if (p == NULL) return false;                    // 失败：原块仍有效，S.data 不动
        S.data = p;                                     // 成功：接管新入口，旧指针失效
        S.capacity = newcap;
    }
    S.data[S.top++] = e;                                // 先用后加
    return true;
}
// #endregion push

// #region pop
bool SS_Pop(SeqStack &S, ElemType &out) {
    if (S.top == 0) return false;                       // 下溢：把决定权交给调用者
    out = S.data[--S.top];                              // 先减后用，与 Push 严格对称
    return true;
}
// #endregion pop

// #region top
ElemType &SS_Top(SeqStack &S) {
    if (S.top == 0) { fprintf(stderr, "SS_Top on empty stack\n"); exit(1); }
    return S.data[S.top - 1];                           // 约定 A：栈顶在 top-1
}
// #endregion top

// 只读重载：逻辑同上，空栈亦 exit(1)（课件从略，完整代码见源文件）。
const ElemType &SS_Top(const SeqStack &S) {
    if (S.top == 0) { fprintf(stderr, "SS_Top on empty stack\n"); exit(1); }
    return S.data[S.top - 1];
}

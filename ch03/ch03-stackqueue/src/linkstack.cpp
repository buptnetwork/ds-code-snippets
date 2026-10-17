// linkstack.cpp：链式栈实现。栈顶在链表头，push/pop 均不遍历。
#include <cstdio>
#include <cstdlib>
#include <climits>
#include "linkstack.h"

// #region lifecycle
bool LS_Init(LinkStack &S) { S.top = NULL; S.size = 0; return true; }

bool LS_Empty (const LinkStack &S) { return S.top == NULL; }
int  LS_Length(const LinkStack &S) { return S.size; }
// #endregion lifecycle

// #region push
bool LS_Push(LinkStack &S, ElemType e) {
    if (S.size == INT_MAX) return false;                // 计数超界防护
    StackNode *p = (StackNode *)malloc(sizeof(StackNode));
    if (p == NULL) return false;                        // 分配失败，原栈不变
    p->data = e;
    p->next = S.top;        // ① 新结点先指向原栈顶
    S.top = p;              // ② 再让栈顶指针上移；两行顺序不能反
    S.size++;
    return true;
}
// #endregion push

// #region pop
bool LS_Pop(LinkStack &S, ElemType &out) {
    if (S.top == NULL) return false;                    // 下溢
    StackNode *p = S.top;                               // ① 记住待删结点
    out = p->data;                                      // ② 取出数据
    S.top = p->next;                                    // ③ 栈顶下移
    free(p);                                            // ④ 释放，不能漏
    S.size--;
    return true;
}
// #endregion pop

// #region top
ElemType &LS_Top(LinkStack &S) {
    if (S.top == NULL) { fprintf(stderr, "LS_Top on empty stack\n"); exit(1); }
    return S.top->data;
}
// #endregion top

// #region destroy
void LS_Destroy(LinkStack &S) {
    ElemType x;
    while (LS_Pop(S, x)) { }                            // 复用 Pop，下溢返回 false 即终止
    // 循环结束时 S.top == NULL, S.size == 0
}
// #endregion destroy

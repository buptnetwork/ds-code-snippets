#ifndef CH03_STACK_DOUBLE_H
#define CH03_STACK_DOUBLE_H

// 顺序栈（double 元素）：用于表达式求值中的操作数栈。
// 结构体名、函数前缀与 seqstack.h 完全独立，可在同一作用域共同使用。
// 约定同 SeqStack：top == 元素个数，空栈 top==0。

// #region representation-d
typedef struct {
    double *data;
    int top;        // 元素个数
    int capacity;
} StackD;
// #endregion representation-d

// #region interface-d
bool SD_Init(StackD &S, int cap0);
void SD_Destroy(StackD &S);
void SD_Clear(StackD &S);
bool SD_Empty(const StackD &S);
int  SD_Length(const StackD &S);
bool SD_Push(StackD &S, double e);
bool SD_Pop(StackD &S, double &out);
// #endregion interface-d

#endif

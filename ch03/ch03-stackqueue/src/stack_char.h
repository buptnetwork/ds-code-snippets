#ifndef CH03_STACK_CHAR_H
#define CH03_STACK_CHAR_H

// 顺序栈（char 元素）：用于中缀转后缀中的运算符栈。
// 结构体名、函数前缀与 seqstack.h / stack_double.h 完全独立。

// #region representation-c
typedef struct {
    char *data;
    int top;        // 元素个数
    int capacity;
} StackC;
// #endregion representation-c

// #region interface-c
bool SC_Init(StackC &S, int cap0);
void SC_Destroy(StackC &S);
void SC_Clear(StackC &S);
bool SC_Empty(const StackC &S);
int  SC_Length(const StackC &S);
bool SC_Push(StackC &S, char e);
bool SC_Pop(StackC &S, char &out);
char SC_Top(const StackC &S);     // 空栈返回 '\0'（调用方应先判 SC_Empty）
// #endregion interface-c

#endif

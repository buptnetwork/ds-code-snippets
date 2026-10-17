#ifndef CH03_SEQSTACK_H
#define CH03_SEQSTACK_H
#include "../include/elemtype.h"

// 顺序栈：动态数组 + 一个整型 top。
// 本课程约定 A：top == 栈中元素个数，即“栈顶元素的下一个位置”。
// 因此：空栈 top==0；判满触发扩容的条件是 top==capacity；栈顶元素是 data[top-1]。
// #region representation
typedef struct {
    ElemType *data;     // 动态数组
    int top;            // 元素个数
    int capacity;
} SeqStack;
// #endregion representation

// 生命周期与别名约定：对象先置零并成功初始化；不能重复初始化仍占有资源的对象，
// 也不能浅拷贝成两个所有者；SS_Pop 的 out 不得引用栈内元素。
// SS_Push 的 e 是 int 值副本，故 SS_Push(S, SS_Top(S)) 会在扩容前保存待压入的值。
// 错误返回约定：Push 失败返回 false 且原栈不变；Pop 空栈返回 false 且 out 不变；
// 空栈访问 SS_Top 属编程错误（违反前置条件），打印诊断后 exit(1)。
// #region interface
bool SS_Init(SeqStack &S, int cap0);
void SS_Destroy(SeqStack &S);
void SS_Clear(SeqStack &S);
bool SS_Empty (const SeqStack &S);          // 常量引用：只读
int  SS_Length(const SeqStack &S);
bool SS_Push(SeqStack &S, ElemType e);          // S 按引用修改，int 元素按值复制
bool SS_Pop (SeqStack &S, ElemType &out);       // out 带回被弹出的元素
ElemType &SS_Top(SeqStack &S);                  // 返回引用：可读可写栈顶
const ElemType &SS_Top(const SeqStack &S);      // 只读版本
// #endregion interface
#endif

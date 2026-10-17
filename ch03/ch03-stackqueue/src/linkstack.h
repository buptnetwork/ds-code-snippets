#ifndef CH03_LINKSTACK_H
#define CH03_LINKSTACK_H
#include "../include/elemtype.h"

// 链式栈：单链表存放元素。关键设计：栈顶放在链表头，push=头插、pop=删头，均 O(1)。
// 不需要头结点：栈只在头部操作，所有插删都是同一种情况，没有需要统一的分支。
// #region representation
typedef struct StackNode {
    ElemType data;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;     // 指向栈顶结点；空栈时为 NULL
    int size;           // 维护元素个数，使 Length 为 O(1)
} LinkStack;
// #endregion representation

// 错误返回约定同顺序栈：Push 分配失败或计数超界返回 false；Pop 空栈返回 false，out 不变。
// #region interface
bool LS_Init(LinkStack &S);
void LS_Destroy(LinkStack &S);

bool LS_Empty (const LinkStack &S);
int  LS_Length(const LinkStack &S);

bool LS_Push(LinkStack &S, ElemType e);
bool LS_Pop (LinkStack &S, ElemType &out);
ElemType &LS_Top(LinkStack &S);
// #endregion interface
#endif

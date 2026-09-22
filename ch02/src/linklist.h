#ifndef CH02_LINKLIST_H
#define CH02_LINKLIST_H
#include "../include/elemtype.h"

// #region node
typedef struct LNode {
    ElemType data;
    LNode *next;
} LNode;
// #endregion node

// #region representation
typedef struct {
    LNode *head; // 成功初始化后指向独立头结点。
    int size;
} LinkList;
// #endregion representation

// 普通操作要求成功初始化、无环、独占所有权；不得浅拷贝或重复初始化。
// 非法位置、计数超界、分配失败时原表和 out 不变；out 不与数据域别名。
bool LinkListInit(LinkList &L);
void LinkListDestroy(LinkList &L);
int LinkListLength(const LinkList &L);
bool LinkListEmpty(const LinkList &L);
bool LinkListGet(const LinkList &L, int pos, ElemType &out);
bool LinkListSet(LinkList &L, int pos, ElemType x);
int LinkListFind(const LinkList &L, ElemType x);
bool LinkListInsert(LinkList &L, int pos, ElemType x);
bool LinkListErase(LinkList &L, int pos, ElemType &out);
void LinkListTraverse(const LinkList &L, void (*visit)(ElemType));
// 建表函数只用于尚未拥有结点的对象；中途失败释放已建部分。
bool CreateHead(LinkList &L, const ElemType a[], int n);
bool CreateTail(LinkList &L, const ElemType a[], int n);

// 无头结点对照，同样使用包装对象与引用。
typedef struct { LNode *head; int size; } PlainList;
bool Insert_A(PlainList &L, int pos, ElemType x);
#endif

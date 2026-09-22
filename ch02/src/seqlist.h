#ifndef CH02_SEQLIST_H
#define CH02_SEQLIST_H
#include "../include/elemtype.h"

// #region representation
typedef struct {
    ElemType *data;
    int size;
    int capacity;
} SeqList;
// #endregion representation

// 对象先初始化，不得浅拷贝为两个所有者，不得重复初始化仍拥有资源的对象。
// 失败时原表及 out 不变；out 不得与表内元素别名。
// #region interface
bool SeqListInit(SeqList &L);
void SeqListDestroy(SeqList &L);
int SeqListLength(const SeqList &L);
bool SeqListEmpty(const SeqList &L);
bool SeqListGet(const SeqList &L, int pos, ElemType &out);
bool SeqListSet(SeqList &L, int pos, ElemType x);
int SeqListFind(const SeqList &L, ElemType x);
bool SeqListInsert(SeqList &L, int pos, ElemType x);
bool SeqListErase(SeqList &L, int pos, ElemType &out);
bool SeqListReserve(SeqList &L, int newcap);
bool SeqListPushBack(SeqList &L, ElemType x);
void SeqListTraverse(const SeqList &L, void (*visit)(ElemType));
// #endregion interface
#endif

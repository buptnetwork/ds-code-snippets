#ifndef CH02_DLIST_H
#define CH02_DLIST_H
#include "../include/elemtype.h"

// #region node
typedef struct DNode {
    ElemType data;
    DNode *prev;
    DNode *next;
} DNode;
// #endregion node
// 单哨兵循环双链表，独占所有权，禁止浅拷贝。
struct DList { DNode *head; int size; };
bool DListInit(DList &L);
void DListDestroy(DList &L);
// p 必须是本表存活的在链结点（插入允许哨兵，删除不允许）。
// 不扫描验证归属；失败时表和 out 不变，out 不与结点数据别名。
bool DListInsertAfter(DList &L, DNode *p, ElemType x);
bool DListEraseNode(DList &L, DNode *p, ElemType &out);
void DListTraverse(const DList &L, void (*visit)(ElemType));
// 仅检查已知存活且链接有效的表，不能用于探测野指针或已释放内存。
bool DListCheck(const DList &L);
// 内部链接原语：s 尚未入链；p 在链，Unlink 不得传哨兵。
void DAttachAfter(DNode *p, DNode *s);
void DUnlink(DNode *p);
#endif

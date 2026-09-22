#include "dlist.h"
#include <climits>
#include <cstdlib>
#ifdef CH02_TEST_ALLOC
#include "../demo/alloc_control.h"
#endif

// #region init
bool DListInit(DList &L) {
    L.head = NULL;
    L.size = 0;
    DNode *h = (DNode *)malloc(sizeof(DNode));
    if (!h) return false;
    h->next = h;
    h->prev = h;
    L.head = h;
    return true;
}
// #endregion init

// #region attach
void DAttachAfter(DNode *p, DNode *s) {
    s->next = p->next;
    s->prev = p;
    p->next->prev = s;
    p->next = s;
}
// #endregion attach
// #region unlink
void DUnlink(DNode *p) {
    p->prev->next = p->next;
    p->next->prev = p->prev;
}
// #endregion unlink

// #region insert
bool DListInsertAfter(DList &L, DNode *p, ElemType x) {
    if (!L.head || !p || L.size == INT_MAX) return false;
    DNode *s = (DNode *)malloc(sizeof(DNode));
    if (!s) return false;
    s->data = x;
    DAttachAfter(p, s);
    ++L.size;
    return true;
}
// #endregion insert
// #region erase
bool DListEraseNode(DList &L, DNode *p, ElemType &out) {
    if (!L.head || !p || p == L.head) return false;
    out = p->data;
    DUnlink(p);
    free(p);
    --L.size;
    return true;
}
// #endregion erase

// #region destroy
void DListDestroy(DList &L) {
    if (!L.head) return;
    DNode *p = L.head->next;
    while (p != L.head) {
        DNode *next = p->next;
        free(p);
        p = next;
    }
    free(L.head);
    L.head = NULL;
    L.size = 0;
}
// #endregion destroy
void DListTraverse(const DList &L, void (*visit)(ElemType)) {
    for (DNode *p = L.head->next; p != L.head; p = p->next)
        visit(p->data);
}
bool DListCheck(const DList &L) {
    if (!L.head) return L.size == 0;
    int count = 0;
    DNode *p = L.head;
    do {
        if (!p->next || !p->prev) return false;
        if (p->next->prev != p || p->prev->next != p) return false;
        p = p->next;
        if (p != L.head) {
            if (count == L.size) return false;
            ++count;
        }
    } while (p != L.head);
    return count == L.size;
}

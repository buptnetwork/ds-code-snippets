#include "linklist.h"
#include <climits>
#include <cstdlib>
#ifdef CH02_TEST_ALLOC
#include "../demo/alloc_control.h"
#endif

// #region init
bool LinkListInit(LinkList &L) {
    L.head = NULL;
    L.size = 0;
    LNode *h = (LNode *)malloc(sizeof(LNode));
    if (!h) return false;
    h->next = NULL;
    L.head = h;
    return true;
}
// #endregion init

// #region destroy
void LinkListDestroy(LinkList &L) {
    LNode *p = L.head;
    while (p != NULL) {
        LNode *tmp = p->next;
        free(p);
        p = tmp;
    }
    L.head = NULL;
    L.size = 0;
}
// #endregion destroy

int LinkListLength(const LinkList &L) { return L.size; }
bool LinkListEmpty(const LinkList &L) { return L.size == 0; }

// #region access
bool LinkListGet(const LinkList &L, int pos, ElemType &out) {
    if (pos < 0 || pos >= L.size) return false;
    LNode *p = L.head->next;
    for (int i = 0; i < pos; ++i) p = p->next;
    out = p->data;
    return true;
}
// #endregion access
bool LinkListSet(LinkList &L, int pos, ElemType x) {
    if (pos < 0 || pos >= L.size) return false;
    LNode *p = L.head->next;
    for (int i = 0; i < pos; ++i) p = p->next;
    p->data = x;
    return true;
}
// #region find
int LinkListFind(const LinkList &L, ElemType x) {
    int pos = 0;
    for (LNode *p = L.head->next; p; p = p->next, ++pos)
        if (p->data == x) return pos;
    return -1;
}
// #endregion find

// #region insert
bool LinkListInsert(LinkList &L, int pos, ElemType x) {
    if (pos < 0 || pos > L.size || L.size == INT_MAX) return false;
    LNode *p = L.head;
    for (int i = 0; i < pos; ++i) p = p->next;
    LNode *s = (LNode *)malloc(sizeof(LNode));
    if (!s) return false;
    s->data = x;
    s->next = p->next;
    p->next = s;
    ++L.size;
    return true;
}
// #endregion insert

// #region erase
bool LinkListErase(LinkList &L, int pos, ElemType &out) {
    if (pos < 0 || pos >= L.size) return false;
    LNode *p = L.head;
    for (int i = 0; i < pos; ++i) p = p->next;
    LNode *q = p->next;
    out = q->data;
    p->next = q->next;
    free(q);
    --L.size;
    return true;
}
// #endregion erase

void LinkListTraverse(const LinkList &L, void (*visit)(ElemType)) {
    for (LNode *p = L.head->next; p; p = p->next) visit(p->data);
}

// #region create-head
bool CreateHead(LinkList &L, const ElemType a[], int n) {
    if (n < 0 || (n > 0 && a == NULL)) return false;
    if (!LinkListInit(L)) return false;
    for (int i = 0; i < n; ++i) {
        LNode *s = (LNode *)malloc(sizeof(LNode));
        if (!s) { LinkListDestroy(L); return false; }
        s->data = a[i];
        s->next = L.head->next;
        L.head->next = s;
        ++L.size;
    }
    return true;
}
// #endregion create-head

// #region create-tail
bool CreateTail(LinkList &L, const ElemType a[], int n) {
    if (n < 0 || (n > 0 && a == NULL)) return false;
    if (!LinkListInit(L)) return false;
    LNode *tail = L.head;
    for (int i = 0; i < n; ++i) {
        LNode *s = (LNode *)malloc(sizeof(LNode));
        if (!s) { LinkListDestroy(L); return false; }
        s->data = a[i];
        s->next = NULL;
        tail->next = s;
        tail = s;
        ++L.size;
    }
    return true;
}
// #endregion create-tail

// #region plain-insert
bool Insert_A(PlainList &L, int pos, ElemType x) {
    if (pos < 0 || pos > L.size || L.size == INT_MAX) return false;
    LNode *s = (LNode *)malloc(sizeof(LNode));
    if (!s) return false;
    s->data = x;
    if (pos == 0) {
        s->next = L.head;
        L.head = s;
    } else {
        LNode *p = L.head;
        for (int i = 0; i < pos - 1; ++i) p = p->next;
        s->next = p->next;
        p->next = s;
    }
    ++L.size;
    return true;
}
// #endregion plain-insert

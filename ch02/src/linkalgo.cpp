#include "linkalgo.h"
#include <climits>
#include <cstddef>

// #region midpoint
LNode *FindMid(const LinkList &L) {
    LNode *slow = L.head->next, *fast = slow;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
// #endregion midpoint

// #region kth
LNode *FindKthFromEnd(const LinkList &L, int k) {
    if (k <= 0) return NULL;
    LNode *fast = L.head->next;
    for (int i = 0; i < k; ++i) {
        if (fast == NULL) return NULL;
        fast = fast->next;
    }
    LNode *slow = L.head->next;
    while (fast != NULL) { slow = slow->next; fast = fast->next; }
    return slow;
}
// #endregion kth

// #region cycle
LNode *HasCycle(LNode *first) {
    LNode *slow = first, *fast = first;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return slow;
    }
    return NULL;
}
// #endregion cycle
// #region entry
LNode *FindCycleEntry(LNode *first) {
    LNode *meet = HasCycle(first);
    if (!meet) return NULL;
    LNode *p = first, *q = meet;
    while (p != q) { p = p->next; q = q->next; }
    return p;
}
// #endregion entry

// #region reverse
LNode *Reverse(LNode *first) {
    LNode *prev = NULL, *cur = first;
    while (cur != NULL) {
        LNode *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}
// #endregion reverse
// #region recursive
LNode *ReverseRec(LNode *first) {
    if (first == NULL || first->next == NULL) return first;
    LNode *newHead = ReverseRec(first->next);
    first->next->next = first;
    first->next = NULL;
    return newHead;
}
// #endregion recursive

// #region merge
LNode *Merge(LNode *a, LNode *b) {
    LNode dummy;
    LNode *tail = &dummy;
    dummy.next = NULL;
    while (a != NULL && b != NULL) {
        if (a->data <= b->data) { tail->next = a; a = a->next; }
        else                    { tail->next = b; b = b->next; }
        tail = tail->next;
    }
    tail->next = (a != NULL) ? a : b;
    return dummy.next;
}
// #endregion merge
// #region merge-owner
bool MergeInto(LinkList &A, LinkList &B) {
    if (&A == &B || A.size > INT_MAX - B.size) return false;
    A.head->next = Merge(A.head->next, B.head->next);
    A.size += B.size;
    B.head->next = NULL;
    B.size = 0;
    return true;
}
// #endregion merge-owner

// #region intersection
static int Len(LNode *p) {
    int n = 0;
    while (p) { ++n; p = p->next; }
    return n;
}
LNode *GetIntersection(LNode *a, LNode *b) {
    int la = Len(a), lb = Len(b);
    while (la > lb) { a = a->next; --la; }
    while (lb > la) { b = b->next; --lb; }
    while (a != b) { a = a->next; b = b->next; }
    return a;
}
// #endregion intersection
// #region switch
LNode *GetIntersectionSwitch(LNode *a, LNode *b) {
    LNode *p = a, *q = b;
    while (p != q) {
        p = (p == NULL) ? b : p->next;
        q = (q == NULL) ? a : q->next;
    }
    return p;
}
// #endregion switch

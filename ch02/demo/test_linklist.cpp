#include "../src/linklist.h"
#include "../src/dlist.h"
#include <cassert>
#include <climits>
#include <cstdio>
#include <initializer_list>
#include "alloc_control.h"

static void Expect(const LinkList &L, std::initializer_list<int> values) {
    assert(L.head && L.size == (int)values.size());
    LNode *p = L.head->next;
    for (int x : values) { assert(p && p->data == x); p = p->next; }
    assert(!p);
}
int main() {
    LinkList L;
    allocationBudget = 0;
    assert(!LinkListInit(L) && !L.head && L.size == 0);
    LinkListDestroy(L);
    allocationBudget = -1;
    assert(LinkListInit(L));
    Expect(L, {});
    int out = 777;
    assert(!LinkListGet(L, 0, out) && out == 777);
    assert(!LinkListErase(L, 0, out) && out == 777);
    assert(!LinkListInsert(L, -1, 10));
    assert(!LinkListInsert(L, 1, 10));
    allocationBudget = 0;
    assert(!LinkListInsert(L, 0, 10));
    Expect(L, {});
    allocationBudget = -1;
    for (int x : {10, 20, 30, 40}) assert(LinkListInsert(L, L.size, x));
    assert(LinkListInsert(L, 1, 99));
    Expect(L, {10, 99, 20, 30, 40});
    assert(LinkListErase(L, 3, out) && out == 30);
    Expect(L, {10, 99, 20, 40});
    assert(LinkListGet(L, 1, out) && out == 99);
    assert(LinkListSet(L, 1, -1) && LinkListFind(L, -1) == 1);
    assert(LinkListFind(L, 123) == -1);
    allocationBudget = 0;
    assert(!LinkListInsert(L, 2, 88));
    Expect(L, {10, -1, 20, 40});
    allocationBudget = -1;
    assert(LinkListErase(L, 0, out) && out == 10);
    assert(LinkListErase(L, L.size - 1, out) && out == 40);
    assert(LinkListErase(L, 0, out) && out == -1);
    assert(LinkListErase(L, 0, out) && out == 20);
    Expect(L, {});
    LinkListDestroy(L);
    LinkListDestroy(L);
    assert(!L.head && L.size == 0 && liveBlocks == 0);

    int a[] = {1, 2, 3};
    for (auto build : {CreateHead, CreateTail}) {
        for (int budget = 0; budget < 4; ++budget) {
            allocationBudget = budget;
            assert(!build(L, a, 3));
            assert(!L.head && L.size == 0 && liveBlocks == 0);
        }
    }
    allocationBudget = -1;
    assert(CreateHead(L, a, 3)); Expect(L, {3, 2, 1}); LinkListDestroy(L);
    assert(CreateTail(L, a, 3)); Expect(L, {1, 2, 3}); LinkListDestroy(L);
    assert(!CreateTail(L, NULL, 1) && !L.head);
    assert(!CreateHead(L, a, -1) && !L.head);

    PlainList plain = {NULL, 0};
    assert(Insert_A(plain, 0, 10));
    assert(Insert_A(plain, 0, 99));
    assert(Insert_A(plain, 2, 20));
    assert(plain.size == 3 && plain.head->data == 99);
    while (plain.head) { LNode *p = plain.head; plain.head = p->next; free(p); }

    DList D;
    allocationBudget = 0;
    assert(!DListInit(D) && !D.head && D.size == 0);
    allocationBudget = -1;
    assert(DListInit(D) && DListCheck(D));
    out = 777;
    assert(!DListEraseNode(D, D.head, out) && out == 777);
    assert(DListInsertAfter(D, D.head, 10));
    assert(DListInsertAfter(D, D.head->prev, 20));
    assert(DListInsertAfter(D, D.head, 99));
    assert(DListCheck(D) && D.size == 3);
    allocationBudget = 0;
    assert(!DListInsertAfter(D, D.head, 5));
    assert(DListCheck(D) && D.size == 3);
    allocationBudget = -1;
    assert(DListEraseNode(D, D.head->next->next, out) && out == 10);
    assert(DListCheck(D));
    assert(DListEraseNode(D, D.head->prev, out) && out == 20);
    assert(DListEraseNode(D, D.head->next, out) && out == 99);
    assert(DListCheck(D) && D.size == 0);
    DListDestroy(D); DListDestroy(D);
    assert(liveBlocks == 0);
    std::puts("链表：序列、边界、失败清理、双向一致性与重复销毁检查通过");
}

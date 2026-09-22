#include "../src/lru.h"
#include <cassert>
#include <cstdio>
#include <initializer_list>
#include "alloc_control.h"

static void Expect(const DList &L, std::initializer_list<int> values) {
    assert(DListCheck(L) && L.size == (int)values.size());
    DNode *p = L.head->next;
    for (int x : values) { assert(p != L.head && p->data == x); p = p->next; }
    assert(p == L.head);
}
static void Print(const DList &L) {
    for (DNode *p = L.head->next; p != L.head; p = p->next)
        std::printf("%s%c", p == L.head->next ? "" : ",", p->data);
    std::printf(" size=%d\n", L.size);
}
int main() {
    DList L;
    assert(DListInit(L));
    int out = 777;
    assert(!EvictTail(L, out) && out == 777);
    assert(!MoveToFront(L, L.head));
    DNode *a = NULL, *b = NULL, *c = NULL, *d = NULL;
    assert(AddRecent(L, 3, 'b', b));
    assert(AddRecent(L, 3, 'c', c));
    assert(AddRecent(L, 3, 'a', a));
    Expect(L, {'a', 'c', 'b'}); Print(L);
    assert(MoveToFront(L, c));
    Expect(L, {'c', 'a', 'b'}); Print(L);
    assert(MoveToFront(L, c)); // 已在表头，次序不变。
    allocationBudget = 0;
    assert(!AddRecent(L, 3, 'd', d) && d == NULL);
    Expect(L, {'c', 'a', 'b'});
    allocationBudget = -1;
    assert(AddRecent(L, 3, 'd', d));
    b = NULL; // b所指条目已被淘汰，不再使用旧别名。
    Expect(L, {'d', 'c', 'a'}); Print(L);
    assert(EvictTail(L, out) && out == 'a'); a = NULL;
    assert(EvictTail(L, out) && out == 'c'); c = NULL;
    Expect(L, {'d'});
    assert(MoveToFront(L, d)); Expect(L, {'d'});
    assert(EvictTail(L, out) && out == 'd'); d = NULL;
    Expect(L, {});
    out = 777;
    assert(!EvictTail(L, out) && out == 777);
    DListDestroy(L);
    assert(liveBlocks == 0);
    std::puts("LRU链表侧：次序、空表、单结点、已在表头、分配失败检查通过");
}

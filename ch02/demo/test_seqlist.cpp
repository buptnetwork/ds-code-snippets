#include "../src/seqlist.h"
#include <cassert>
#include <cstdio>
#include "alloc_control.h"

int main() {
    SeqList L;
    assert(SeqListInit(L));
    assert(SeqListEmpty(L) && SeqListLength(L) == 0);
    int out = 777;
    assert(!SeqListGet(L, 0, out) && out == 777);
    assert(!SeqListErase(L, 0, out) && out == 777);
    assert(!SeqListInsert(L, -1, 1));
    assert(!SeqListInsert(L, 1, 1));
    allocationBudget = 0;
    assert(!SeqListPushBack(L, 10));
    assert(L.data == NULL && L.size == 0 && L.capacity == 0);
    allocationBudget = -1;
    assert(SeqListPushBack(L, 10));
    ElemType *old = L.data;
    allocationBudget = 0;
    assert(!SeqListPushBack(L, 20));
    assert(L.data == old && L.size == 1 && L.capacity == 1);
    assert(L.data[0] == 10);
    allocationBudget = -1;
    assert(SeqListInsert(L, 0, 5));
    assert(SeqListPushBack(L, 20));
    assert(SeqListReserve(L, 16));
    assert(L.size == 3 && L.data[0] == 5 && L.data[2] == 20);
    assert(!SeqListReserve(L, -1));
    assert(SeqListReserve(L, 2) && L.capacity == 16);
    assert(SeqListGet(L, 1, out) && out == 10);
    assert(SeqListSet(L, 1, -1) && SeqListFind(L, -1) == 1);
    assert(SeqListFind(L, 99) == -1);
    out = 777;
    assert(!SeqListErase(L, L.size, out) && out == 777);
    assert(SeqListErase(L, 0, out) && out == 5);
    assert(SeqListErase(L, L.size - 1, out) && out == 20);
    assert(SeqListErase(L, 0, out) && out == -1);
    SeqListDestroy(L);
    SeqListDestroy(L);
    assert(L.data == NULL && L.size == 0 && L.capacity == 0);
    assert(liveBlocks == 0);
    std::puts("顺序表：边界、扩容、失败不变与重复销毁检查通过");
}

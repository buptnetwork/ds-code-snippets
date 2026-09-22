#include "../src/seqlist.h"
#include <cstdio>
#include <initializer_list>

static void Print(const SeqList &L) {
    std::printf("[");
    for (int i = 0; i < L.size; ++i)
        std::printf("%s%d", i ? "," : "", L.data[i]);
    std::printf("] size=%d capacity=%d\n", L.size, L.capacity);
}

int main() {
    // #region call
    SeqList L;
    SeqListInit(L);
    for (int x : {10, 20, 30, 40}) {
        if (!SeqListPushBack(L, x)) {
            SeqListDestroy(L);
            return 1;
        }
    }
    Print(L);
    if (!SeqListInsert(L, 1, 99)) {
        SeqListDestroy(L);
        return 1;
    }
    Print(L);
    ElemType out;
    if (!SeqListErase(L, 3, out)) {
        SeqListDestroy(L);
        return 1;
    }
    Print(L);
    SeqListDestroy(L);
    // #endregion call
}

#include "seqlist.h"
#include <climits>
#include <cstdint>
#include <cstdlib>
#ifdef CH02_TEST_ALLOC
#include "../demo/alloc_control.h"
#endif

// #region lifecycle
bool SeqListInit(SeqList &L) {
    L.data = NULL;
    L.size = L.capacity = 0;
    return true;
}
void SeqListDestroy(SeqList &L) {
    free(L.data);
    L.data = NULL;
    L.size = L.capacity = 0;
}
// #endregion lifecycle

int SeqListLength(const SeqList &L) { return L.size; }
bool SeqListEmpty(const SeqList &L) { return L.size == 0; }

// #region access
bool SeqListGet(const SeqList &L, int pos, ElemType &out) {
    if (pos < 0 || pos >= L.size) return false;
    out = L.data[pos];
    return true;
}
bool SeqListSet(SeqList &L, int pos, ElemType x) {
    if (pos < 0 || pos >= L.size) return false;
    L.data[pos] = x;
    return true;
}
// #endregion access

// #region find
int SeqListFind(const SeqList &L, ElemType x) {
    for (int i = 0; i < L.size; ++i)
        if (L.data[i] == x) return i;
    return -1;
}
// #endregion find

// #region reserve
bool SeqListReserve(SeqList &L, int newcap) {
    if (newcap < 0) return false;
    if (newcap <= L.capacity) return true;
    if ((size_t)newcap > SIZE_MAX / sizeof(ElemType)) return false;
    ElemType *p = (ElemType *)realloc(
        L.data, (size_t)newcap * sizeof(ElemType));
    if (p == NULL) return false;
    L.data = p;
    L.capacity = newcap;
    return true;
}
// #endregion reserve

// #region insert
bool SeqListInsert(SeqList &L, int pos, ElemType x) {
    if (pos < 0 || pos > L.size) return false;
    if (L.size == L.capacity) {
        if (L.capacity > INT_MAX / 2) return false;
        int newcap = (L.capacity == 0) ? 1 : L.capacity * 2;
        if (!SeqListReserve(L, newcap)) return false;
    }
    for (int i = L.size; i > pos; --i)
        L.data[i] = L.data[i - 1];
    L.data[pos] = x;
    ++L.size;
    return true;
}
// #endregion insert

// #region erase
bool SeqListErase(SeqList &L, int pos, ElemType &out) {
    if (pos < 0 || pos >= L.size) return false;
    out = L.data[pos];
    for (int i = pos; i < L.size - 1; ++i)
        L.data[i] = L.data[i + 1];
    --L.size;
    return true;
}
// #endregion erase

bool SeqListPushBack(SeqList &L, ElemType x) {
    return SeqListInsert(L, L.size, x);
}
// visit 非空，不得修改表结构。
void SeqListTraverse(const SeqList &L, void (*visit)(ElemType)) {
    for (int i = 0; i < L.size; ++i) visit(L.data[i]);
}

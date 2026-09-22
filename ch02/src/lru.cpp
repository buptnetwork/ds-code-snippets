#include "lru.h"
#include <cstdlib>
#ifdef CH02_TEST_ALLOC
#include "../demo/alloc_control.h"
#endif

// #region move
bool MoveToFront(DList &L, DNode *p) {
    if (!L.head || !p || p == L.head) return false;
    DUnlink(p);
    DAttachAfter(L.head, p);
    return true;
}
// #endregion move
// #region evict
bool EvictTail(DList &L, ElemType &out) {
    if (!L.head || L.size == 0) return false;
    return DListEraseNode(L, L.head->prev, out);
}
// #endregion evict
// #region add
bool AddRecent(DList &L, int cap, ElemType x, DNode *&s) {
    if (!L.head || cap <= 0 || L.size > cap) return false;
    DNode *fresh = (DNode *)malloc(sizeof(DNode));
    if (!fresh) return false;
    fresh->data = x;
    if (L.size == cap) {
        ElemType discarded;
        EvictTail(L, discarded);
    }
    DAttachAfter(L.head, fresh);
    ++L.size;
    s = fresh;
    return true;
}
// #endregion add

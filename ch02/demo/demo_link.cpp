#include "../src/linklist.h"
#include <cstdio>

static void Print(const LinkList &L) {
    std::printf("[");
    for (LNode *p = L.head->next; p; p = p->next)
        std::printf("%s%d", p == L.head->next ? "" : ",", p->data);
    std::printf("] size=%d\n", L.size);
}
int main() {
    int a[] = {10, 20, 30};
    LinkList L;
    if (!CreateTail(L, a, 3)) return 1;
    Print(L);
    if (!LinkListInsert(L, 0, 99)) { LinkListDestroy(L); return 1; }
    Print(L);
    ElemType out;
    if (!LinkListErase(L, 2, out)) { LinkListDestroy(L); return 1; }
    Print(L);
    std::printf("out=%d\n", out);
    LinkListDestroy(L);
}

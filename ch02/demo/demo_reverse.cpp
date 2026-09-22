#include "../src/linkalgo.h"
#include <cassert>
#include <cstdio>

static void PrintNode(const char *label, LNode *p) {
    std::printf("%s=", label);
    if (p) std::printf("%d ", p->data);
    else std::printf("- ");
}
static void PrintChain(LNode *p) {
    while (p) { std::printf("%d->", p->data); p = p->next; }
    std::printf("NULL");
}
// 追踪版额外打印两截状态；正确算法的权威来源为 linkalgo.cpp#reverse。
static LNode *ReverseTrace(LNode *first) {
    LNode *prev = NULL, *cur = first;
    int step = 0;
    while (cur) {
        LNode *next = cur->next;
        std::printf("第%d轮改链前：", ++step);
        PrintNode("prev", prev); PrintNode("cur", cur); PrintNode("next", next);
        std::puts("");
        cur->next = prev;
        prev = cur;
        cur = next;
        std::printf("已反转："); PrintChain(prev);
        std::printf(" | 未处理："); PrintChain(cur); std::puts("");
    }
    return prev;
}
int main() {
    int a[] = {1, 2, 3, 4};
    LinkList L;
    if (!CreateTail(L, a, 4)) return 1;
    L.head->next = ReverseTrace(L.head->next);
    assert(L.head->next->data == 4 && L.size == 4);
    L.head->next = Reverse(L.head->next);
    LNode *p = L.head->next;
    for (int x : a) { assert(p && p->data == x); p = p->next; }
    assert(!p);
    LinkListDestroy(L);
}

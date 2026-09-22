#include "../src/linklist.h"
#include <cstdio>
#include <cstdlib>

// 仅阅读与手工追踪。三个片段互相独立，不由默认驱动执行。
// 修正后自行增加测试；禁止把偶然崩溃当作正确性证据。
// #region leak
void BugLeak(LNode *p) { // p 的后继存在，且没有其他所有者。
    p->next = p->next->next;
}
// #endregion leak
// #region use-after-free
void BugDestroy(LinkList &L) {
    for (LNode *p = L.head; p != NULL; p = p->next)
        free(p);
    L.head = NULL;
    L.size = 0;
}
// #endregion use-after-free
// #region lost-chain
void BugInsert(LNode *p, LNode *s) { // s 是未入链的新结点。
    p->next = s;
    s->next = p->next;
}
// #endregion lost-chain
int main() {
    std::puts("仅阅读题：默认不执行缺陷片段。先画状态图并修正，再测试。 ");
}

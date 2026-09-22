#include "../src/linkalgo.h"
#include <cassert>
#include <cstdio>
int main() {
    int a[] = {0, 1, 2, 3, 4, 5, 6};
    LinkList L;
    if (!CreateTail(L, a, 7)) return 1;
    LNode *nodes[7], *p = L.head->next;
    for (int i = 0; i < 7; ++i) { nodes[i] = p; p = p->next; }
    nodes[6]->next = nodes[3];
    LNode *slow = nodes[0], *fast = nodes[0];
    std::puts("步 slow fast 相遇");
    std::puts("0 0 0 初始不判");
    int step = 0;
    do {
        slow = slow->next;
        fast = fast->next->next;
        std::printf("%d %d %d %s\n", ++step, slow->data, fast->data,
                    slow == fast ? "是" : "否");
    } while (slow != fast);
    assert(step == 4 && slow == nodes[4]);
    LNode *entry = FindCycleEntry(nodes[0]);
    assert(entry == nodes[3]);
    std::printf("入环点=%d\n", entry->data);
    nodes[6]->next = NULL; // 先断环，才能使用普通销毁。
    LinkListDestroy(L);
}

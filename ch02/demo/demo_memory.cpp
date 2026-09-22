#include "../src/linklist.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <initializer_list>

// 教学追踪版：与 LinkListDestroy 相同的释放顺序，额外打印仍存活的结点。
// 不运行任何缺陷版本；输出只辅助追踪，不证明不存在所有内存缺陷。
// #region trace
static void DestroyTrace(LinkList &L) {
    LNode *p = L.head;
    bool isHead = true;
    while (p) {
        LNode *next = p->next;
        if (isHead) std::puts("释放头结点");
        else std::printf("释放数据结点 %d\n", p->data);
        free(p);
        p = next;
        isHead = false;
    }
    L.head = NULL;
    L.size = 0;
    std::printf("销毁后 head为空=%d size=%d\n", L.head == NULL, L.size);
}
// #endregion trace
int main() {
    int a[] = {10, 20, 30};
    for (int n : {0, 1, 3}) {
        LinkList L;
        if (!CreateTail(L, a, n)) return 1;
        std::printf("数据结点数=%d\n", n);
        DestroyTrace(L);
        DestroyTrace(L);
        assert(!L.head && L.size == 0);
    }
}

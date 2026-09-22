#include "../src/linkalgo.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <vector>
#include "alloc_control.h"

int main() {
    // 普通算法仅传无环结构；保存身份，以免只比较数据而漏掉结点丢失。
    for (int n = 0; n <= 12; ++n) {
        std::vector<int> a(n);
        for (int i = 0; i < n; ++i) a[i] = i / 2;
        LinkList L;
        assert(CreateTail(L, a.data(), n));
        std::vector<LNode *> nodes;
        for (LNode *p = L.head->next; p; p = p->next) nodes.push_back(p);
        assert(FindMid(L) == (n ? nodes[n / 2] : NULL));
        for (int k = -1; k <= n + 1; ++k)
            assert(FindKthFromEnd(L, k) == (k > 0 && k <= n ? nodes[n - k] : NULL));
        assert(!HasCycle(L.head->next) && !FindCycleEntry(L.head->next));
        L.head->next = Reverse(L.head->next);
        LNode *p = L.head->next;
        for (int i = n - 1; i >= 0; --i) { assert(p == nodes[i]); p = p->next; }
        assert(!p);
        L.head->next = ReverseRec(L.head->next);
        p = L.head->next;
        for (LNode *node : nodes) { assert(p == node); p = p->next; }
        assert(!p && L.size == n);
        LinkListDestroy(L);
    }
    // 含纯环、尾部自环、a=3/c=4；每组先断环后销毁。
    for (int a = 0; a < 12; ++a) for (int c = 1; c <= 12; ++c) {
        int n = a + c;
        std::vector<int> values(n, 1);
        LinkList L;
        assert(CreateTail(L, values.data(), n));
        std::vector<LNode *> nodes;
        for (LNode *p = L.head->next; p; p = p->next) nodes.push_back(p);
        nodes.back()->next = nodes[a];
        assert(HasCycle(nodes[0]) && FindCycleEntry(nodes[0]) == nodes[a]);
        nodes.back()->next = NULL;
        LinkListDestroy(L);
    }
    // 合并两条独立链：含空链、重复值，并验证跨链相等时A在前。
    for (int m = 0; m < 6; ++m) for (int n = 0; n < 6; ++n) {
        std::vector<int> av(m), bv(n);
        for (int i = 0; i < m; ++i) av[i] = i / 2;
        for (int i = 0; i < n; ++i) bv[i] = i / 2;
        LinkList A, B;
        assert(CreateTail(A, av.data(), m));
        assert(CreateTail(B, bv.data(), n));
        std::vector<LNode *> expected;
        for (LNode *p = A.head->next; p; p = p->next) expected.push_back(p);
        for (LNode *p = B.head->next; p; p = p->next) expected.push_back(p);
        std::stable_sort(expected.begin(), expected.end(),
                         [](LNode *x, LNode *y) { return x->data < y->data; });
        assert(!GetIntersection(A.head->next, B.head->next));
        assert(!MergeInto(A, A));
        assert(MergeInto(A, B));
        assert(A.size == m + n && B.size == 0 && B.head->next == NULL);
        LNode *p = A.head->next;
        for (LNode *node : expected) { assert(p == node); p = p->next; }
        assert(!p);
        LinkListDestroy(A); LinkListDestroy(B);
    }
    // 三个独立拥有者：A独有前缀、B独有前缀、共享尾。构造只读入口。
    for (int x = 0; x < 4; ++x) for (int y = 0; y < 4; ++y)
        for (int z = 0; z < 4; ++z) {
            int values[] = {7, 7, 7};
            LinkList A, B, T;
            assert(CreateTail(A, values, x));
            assert(CreateTail(B, values, y));
            assert(CreateTail(T, values, z));
            LNode *at = A.head, *bt = B.head;
            while (at->next) at = at->next;
            while (bt->next) bt = bt->next;
            LNode *shared = T.head->next;
            at->next = shared; bt->next = shared;
            assert(GetIntersection(A.head->next, B.head->next) == shared);
            assert(GetIntersectionSwitch(A.head->next, B.head->next) == shared);
            at->next = NULL; bt->next = NULL; // 先断开借用关系。
            LinkListDestroy(A); LinkListDestroy(B); LinkListDestroy(T);
        }
    assert(liveBlocks == 0);
    std::puts("链表算法：中点、k边界、144组环、反转、合并身份与共享尾检查通过");
}

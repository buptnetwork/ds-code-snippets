// monoqueue.cpp：单调队列求滑动窗口最大值（单元 10）。
// 用例 a=[1,3,-1,-3,5,3,6,7]，k=3，答案 [3,3,5,5,6,7]。
// 与上次课单调栈同构：队列存下标，候选值从队头到队尾严格递减，队头即当前窗口最大值。
// 两端都要删除（队尾弹被压制的、队头弹滑出的），所以必须用双端队列。
#include <cstdio>
#include <cstdlib>
#include "../src/deque.h"

// #region sliding-window
// 每个下标至多入队一次、出队一次，所有 while 总次数不超过 n，时间 O(n)、空间 O(k)。
void SlidingWindowMax(const int a[], int n, int k, int ans[]) {
    Deque D = {};
    DQ_Init(D, n > 0 ? n : 1);                          // 存下标，容量 n 足够
    int t = 0;
    for (int i = 0; i < n; ++i) {
        // ① 队尾维护单调性：弹出所有 a[下标] <= a[i] 的候选（等值保留更靠右、过期更晚者）
        while (!DQ_Empty(D) && a[DQ_Back(D)] <= a[i]) { ElemType x; DQ_PopBack(D, x); }
        DQ_PushBack(D, i);
        // ② 队头移除过期：下标 <= i-k 已滑出窗口
        if (!DQ_Empty(D) && DQ_Front(D) <= i - k) { ElemType x; DQ_PopFront(D, x); }
        // ③ 窗口成形后记录答案（队头下标对应最大值）
        if (i >= k - 1) ans[t++] = a[DQ_Front(D)];
    }
    DQ_Destroy(D);
}
// #endregion sliding-window

int main() {
    const int a[] = {1, 3, -1, -3, 5, 3, 6, 7};
    const int n = 8, k = 3;
    int ans[8];
    SlidingWindowMax(a, n, k, ans);

    printf("数组: [");
    for (int i = 0; i < n; ++i) printf("%d%s", a[i], i < n-1 ? ", " : "");
    printf("]，k=%d\n", k);
    printf("答案: [");
    for (int i = 0; i < n - k + 1; ++i) printf("%d%s", ans[i], i < n-k ? ", " : "");
    printf("]\n");
    printf("预期: [3, 3, 5, 5, 6, 7]\n");

    puts("\n=== 边界用例 ===");
    int b1[] = {5}, r1[1]; SlidingWindowMax(b1, 1, 1, r1);
    printf("[5], k=1 -> [%d]（预期 5）\n", r1[0]);
    int b2[] = {1, 2, 3, 4}, r2[4]; SlidingWindowMax(b2, 4, 4, r2);
    printf("[1,2,3,4], k=4 -> [%d]（预期 4）\n", r2[0]);
    int b3[] = {4, 3, 2, 1}, r3[2]; SlidingWindowMax(b3, 4, 3, r3);
    printf("[4,3,2,1], k=3 -> [%d, %d]（预期 4, 3：递减数组每个窗口的最大值是窗口首元素）\n", r3[0], r3[1]);
    return 0;
}

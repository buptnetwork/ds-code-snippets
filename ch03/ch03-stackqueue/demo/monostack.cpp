// monostack.cpp：单调栈演示——下一个更大元素 + 柱状图最大矩形。
// 用例数组：[2, 1, 5, 6, 2, 3]
// NGE 答案：[5, 5, 6, -1, 3, -1]；最大矩形面积：10（高 5、宽 2）。
#include <cstdio>
#include <cstdlib>
#include "../src/seqstack.h"

// #region nge
// 下一个更大元素：栈中存下标，对应值从底到顶非递增（允许相等）。
// 每个下标至多入栈一次、出栈一次，总时间 O(n)。
void NextGreaterElement(const int a[], int n, int ans[]) {
    SeqStack S = {};
    SS_Init(S, n > 0 ? n : 1);
    for (int i = 0; i < n; ++i) {
        while (!SS_Empty(S) && a[SS_Top(S)] < a[i]) {
            int j; SS_Pop(S, j);
            ans[j] = a[i];
        }
        SS_Push(S, i);
    }
    ElemType j;
    while (SS_Pop(S, j)) ans[j] = -1;
    SS_Destroy(S);
}
// #endregion nge

// #region largest-rect
// 柱状图最大矩形：栈中高度从底到顶非递减（相等时保留）。
// 两端各补一根高度 0 的哨兵柱，消除边界分支。
// 等高连续区间的完整宽度由最左侧代表覆盖，全局答案仍正确。
int LargestRectangle(const int h[], int n) {
    // 扩展数组：两端各加一个 0
    int *ext = (int *)malloc(sizeof(int) * (size_t)(n + 2));
    if (!ext) return -1;
    ext[0] = 0;
    for (int i = 0; i < n; ++i) ext[i + 1] = h[i];
    ext[n + 1] = 0;
    int m = n + 2;

    SeqStack S = {};
    SS_Init(S, m);
    int best = 0;
    for (int i = 0; i < m; ++i) {
        while (!SS_Empty(S) && ext[SS_Top(S)] > ext[i]) {
            int j; SS_Pop(S, j);
            int l = SS_Empty(S) ? -1 : SS_Top(S);
            int w = i - l - 1;
            int area = ext[j] * w;
            if (area > best) best = area;
        }
        SS_Push(S, i);
    }
    SS_Destroy(S);
    free(ext);
    return best;
}
// #endregion largest-rect

int main() {
    const int a[] = {2, 1, 5, 6, 2, 3};
    const int n = 6;

    puts("=== 下一个更大元素 ===");
    printf("数组: [");
    for (int i = 0; i < n; ++i) printf("%d%s", a[i], i < n-1 ? ", " : "");
    printf("]\n");

    int ans[6];
    NextGreaterElement(a, n, ans);
    printf("答案: [");
    for (int i = 0; i < n; ++i) printf("%d%s", ans[i], i < n-1 ? ", " : "");
    printf("]\n");
    printf("预期: [5, 5, 6, -1, 3, -1]\n");

    puts("\n=== 柱状图最大矩形 ===");
    printf("高度: [");
    for (int i = 0; i < n; ++i) printf("%d%s", a[i], i < n-1 ? ", " : "");
    printf("]\n");
    int area = LargestRectangle(a, n);
    printf("最大面积: %d（预期 10，高 5 × 宽 2）\n", area);

    puts("\n=== 边界用例 ===");
    int h1[] = {1, 1, 1, 1};
    printf("[1,1,1,1] -> %d（预期 4）\n", LargestRectangle(h1, 4));
    int h2[] = {5};
    printf("[5]       -> %d（预期 5）\n", LargestRectangle(h2, 1));
    printf("[]        -> %d（预期 0）\n", LargestRectangle(NULL, 0));

    return 0;
}

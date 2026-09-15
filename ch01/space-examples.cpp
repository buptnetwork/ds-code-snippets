/*
 * 第 1 章 绪论 · 空间复杂度四例
 * 原地逆序 / 辅助数组 / 线性递归 / 二分递归
 * 共用页：1.12.1
 *
 * 运行方式：
 *   g++ -std=c++17 -Wall -Wextra -o /tmp/space snippets/ch01/space-examples.cpp && /tmp/space
 * slides 引用方式：
 *   <<< @/snippets/ch01/space-examples.cpp#inplace {lines:true}
 *   <<< @/snippets/ch01/space-examples.cpp#recur {lines:true}
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// #region inplace
/* ① S(n) = O(1)：原地逆序（in-place），只用常数个临时变量 i / j / t */
void reverse(int a[], int n)
{
    for (int i = 0, j = n - 1; i < j; i++, j--) {
        int t = a[i]; a[i] = a[j]; a[j] = t;
    }
}

/* ② S(n) = O(n)：借助辅助数组，b 是额外的 n 个单元 */
void reverse_copy(const int a[], int b[], int n)
{
    for (int i = 0; i < n; i++)
        b[i] = a[n - 1 - i];
}
// #endregion inplace

// #region recur
/* ③ S(n) = O(n)：线性递归，深度 n —— 每一层栈帧都要算！
 * 没有开数组、只有一个返回值，但栈上压了 n 个帧，空间就是 O(n)。
 */
int sum_rec(const int a[], int n)
{
    if (n == 0) return 0;
    return a[n - 1] + sum_rec(a, n - 1);
}

/* ④ S(n) = O(log n)：二分型递归，每层把规模砍半，深度只有 log n
 * （第 6 章平衡树、第 9 章快速排序会再见到这个 log n 的栈深度）
 */
int bsearch_rec(const int a[], int lo, int hi, int key)
{
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;          /* 防 (lo+hi) 溢出 */
    if (a[mid] == key) return mid;
    return a[mid] < key ? bsearch_rec(a, mid + 1, hi, key)
                        : bsearch_rec(a, lo, mid - 1, key);
}
// #endregion recur

/* ------- 以下为自测代码，不参与 slides 引用 ------- */

int main(void)
{
    int a[6] = { 1, 2, 3, 4, 5, 6 };
    int b[6];

    reverse_copy(a, b, 6);
    for (int i = 0; i < 6; i++) assert(b[i] == 6 - i);

    reverse(a, 6);
    for (int i = 0; i < 6; i++) assert(a[i] == b[i]);   /* 两种逆序结果一致 */

    int s[5] = { 1, 2, 3, 4, 5 };
    assert(sum_rec(s, 5) == 15);
    assert(sum_rec(s, 0) == 0);                          /* 边界：空 */

    int sorted[6] = { 1, 2, 3, 4, 5, 6 };
    assert(bsearch_rec(sorted, 0, 5, 1) == 0);
    assert(bsearch_rec(sorted, 0, 5, 6) == 5);
    assert(bsearch_rec(sorted, 0, 5, 99) == -1);
    assert(bsearch_rec(sorted, 0, -1, 1) == -1);         /* 边界：空区间 */

    printf("all tests passed\n");
    return 0;
}

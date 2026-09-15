/*
 * 第 1 章 绪论 · 冒泡排序（带提前终止）+ 归并排序，供倍增实验用
 * 共用页：1.15.2 三口径典型 / 1.18.4 倍增实验
 *
 * 运行方式：
 *   g++ -std=c++17 -O2 -Wall -Wextra -o /tmp/sorts snippets/ch01/sorts.cpp && /tmp/sorts
 * slides 引用方式：
 *   <<< @/snippets/ch01/sorts.cpp#bubble {lines:true}
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// #region bubble
/* 冒泡排序（带「本趟无交换即提前结束」优化）
 * 最好 O(n)：已有序，一趟扫完没交换就退出
 * 最坏 O(n²)：完全逆序
 * 平均 O(n²)
 */
void bubble_sort(int a[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
                swapped = 1;
            }
        if (!swapped) break;              /* 本趟无交换 => 已有序，提前结束 */
    }
}
// #endregion bubble

// #region merge
/* 归并排序：平均 / 最坏均为 O(n log n)，空间 O(n)（辅助数组）+ O(log n)（递归栈） */
static void merge(int a[], int tmp[], int lo, int mid, int hi)
{
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi)  tmp[k++] = a[j++];
    for (int t = lo; t <= hi; t++) a[t] = tmp[t];
}

static void msort(int a[], int tmp[], int lo, int hi)
{
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    msort(a, tmp, lo, mid);
    msort(a, tmp, mid + 1, hi);
    merge(a, tmp, lo, mid, hi);
}

void merge_sort(int a[], int n)
{
    if (n <= 1) return;
    int *tmp = (int *)malloc(n * sizeof(int));
    msort(a, tmp, 0, n - 1);
    free(tmp);
}
// #endregion merge

/* ------- 以下为自测代码，不参与 slides 引用 ------- */

static int is_sorted(const int a[], int n)
{
    for (int i = 0; i + 1 < n; i++) if (a[i] > a[i + 1]) return 0;
    return 1;
}

int main(void)
{
    /* 一般情况 */
    int a[8] = { 5, 2, 9, 1, 5, 6, 0, 3 };
    int b[8]; for (int i = 0; i < 8; i++) b[i] = a[i];
    bubble_sort(a, 8); assert(is_sorted(a, 8));
    merge_sort(b, 8);  assert(is_sorted(b, 8));

    /* 边界：n = 0 / n = 1 */
    int e[1] = { 7 };
    bubble_sort(e, 0); merge_sort(e, 0);
    bubble_sort(e, 1); merge_sort(e, 1); assert(e[0] == 7);

    /* 边界：全部相同 */
    int s[5] = { 4, 4, 4, 4, 4 };
    bubble_sort(s, 5); assert(is_sorted(s, 5));
    merge_sort(s, 5);  assert(is_sorted(s, 5));

    /* 边界：已升序（冒泡最好情况，一趟退出）/ 已降序（最坏情况） */
    int asc[5] = { 1, 2, 3, 4, 5 }; bubble_sort(asc, 5); assert(is_sorted(asc, 5));
    int desc[5] = { 5, 4, 3, 2, 1 }; merge_sort(desc, 5); assert(is_sorted(desc, 5));

    printf("all tests passed\n");
    return 0;
}

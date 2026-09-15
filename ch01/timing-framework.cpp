/*
 * 第 1 章 绪论 · 计时与倍增实验框架，输出 n / time / ratio 三列
 * 共用页：1.18.3 计时 / 1.18.4 倍增实验验证复杂度（作业实验一主线）
 *
 * 运行方式（实验一律统一 -O2，并把编译选项写进报告）：
 *   g++ -std=c++17 -O2 -Wall -Wextra -o /tmp/timing snippets/ch01/timing-framework.cpp && /tmp/timing
 * slides 引用方式：
 *   <<< @/snippets/ch01/timing-framework.cpp#measure {lines:true}
 *   <<< @/snippets/ch01/timing-framework.cpp#main {lines:true}
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---- 被测算法（与 sorts.cpp 一致，此处内联以便单文件运行）---- */
static void bubble_sort(int a[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t; swapped = 1;
            }
        if (!swapped) break;
    }
}

static void msort(int a[], int tmp[], int lo, int hi);
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
static void merge_sort(int a[], int n)
{
    if (n <= 1) return;
    int *tmp = (int *)malloc(n * sizeof(int));
    msort(a, tmp, 0, n - 1);
    free(tmp);
}

/* ---- 数据生成（不计时）---- */
static int *gen_random(int n)
{
    int *d = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) d[i] = rand();
    return d;
}

// #region measure
/* 计时：只包核心算法。数据生成、输入输出、malloc 全部排除在计时区间之外。
 * 单次太短（< 10ms）时应重复多轮再取平均，本框架对每个 n 重复 rep 次。
 */
double measure(void (*algo)(int *, int), int *data, int n, int rep)
{
    int *buf = (int *)malloc(n * sizeof(int));
    double acc = 0;
    for (int r = 0; r < rep; r++) {
        for (int i = 0; i < n; i++) buf[i] = data[i];   /* 每轮复位：在计时区间之外 */
        clock_t start = clock();
        algo(buf, n);                                    /* 只包核心算法 */
        clock_t end = clock();
        acc += (double)(end - start) / CLOCKS_PER_SEC;
    }
    free(buf);
    return acc / rep;
}
// #endregion measure

// #region main
/* 倍增实验：n 取一串倍增的规模，算相邻比值 t(2n)/t(n)，由 k≈log₂(比值) 反推指数 */
static void doubling(void (*algo)(int *, int), const char *name,
                     int n0, int nmax, int rep)
{
    printf("\n=== %s ===\n%8s %12s %8s\n", name, "n", "time(s)", "ratio");
    double prev = 0;
    for (int n = n0; n <= nmax; n *= 2) {
        int *data = gen_random(n);              /* 数据生成不计时 */
        double t = measure(algo, data, n, rep);
        printf("%8d %12.5f %8.2f\n", n, t, prev > 0 ? t / prev : 0.0);
        prev = t;
        free(data);                             /* 上次课：有分配必有释放 */
    }
}

int main(void)
{
    srand(20260915);
    /* 冒泡 O(n²)：规模控制在 30 秒内，比值应 ≈ 4 */
    doubling(bubble_sort, "冒泡排序（预期 ratio≈4 → O(n^2)）", 1000, 16000, 1);
    /* 归并 O(n log n)：比值应 ≈ 2.1~2.3 */
    doubling(merge_sort, "归并排序（预期 ratio≈2.1~2.3 → O(n log n)）", 1000, 128000, 1);
    printf("\n反推：k ≈ log2(ratio)。比值≈4 → k≈2；比值≈2 → k≈1。\n");
    return 0;
}
// #endregion main

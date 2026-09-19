/*
 * 第 1 章 绪论 · 动态数组：+1 扩容 vs 倍增扩容（含插入计时主程序）
 * 共用主题：聚合分析 / 摊还实验（作业实验二主线）。
 * 计数与计时分开运行；每个规模计时5组，输出原始数据与中位数。
 *
 * 运行方式：
 *   g++ -std=c++17 -O2 -Wall -Wextra -o /tmp/grow snippets/ch01/dynarray-grow.cpp && /tmp/grow
 * slides 引用方式：
 *   <<< @/snippets/ch01/dynarray-grow.cpp#push {lines:true}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <algorithm>
#include <chrono>

static int *allocate(int capacity)
{
    if (capacity <= 0 || (size_t)capacity > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "容量超出可分配范围\n");
        exit(EXIT_FAILURE);
    }
    int *p = (int *)malloc((size_t)capacity * sizeof(int));
    if (!p) { perror("数组分配失败"); exit(EXIT_FAILURE); }
    return p;
}

// #region struct
typedef struct {
    int    *data;      /* 元素存储区 */
    int     size;      /* 当前元素个数 */
    int     capacity;  /* 当前容量 */
} DynArray;

static void da_init(DynArray *d)
{
    d->data = allocate(1);                 /* 初始容量 1 */
    d->size = 0;
    d->capacity = 1;
}
// #endregion struct

// #region push
/* 尾部插入：factor = 1 表示 +1；factor = 2 表示倍增。
 * 返回本次复制的旧元素个数；新元素始终写入1次，二者不要混称为复制。
 * 不在函数内累计计数，计时调用可忽略返回值。
 */
static int da_push(DynArray *d, int v, int factor)
{
    int moved = 0;
    if (d->size == d->capacity) {
        if ((factor == 1 && d->capacity == INT_MAX) ||
            (factor == 2 && d->capacity > INT_MAX / 2)) {
            fprintf(stderr, "扩容会超出整数范围\n");
            exit(EXIT_FAILURE);
        }
        int newcap = (factor == 1) ? d->capacity + 1 : d->capacity * 2;
        int *nd = allocate(newcap);
        memcpy(nd, d->data, (size_t)d->size * sizeof(int));
        moved = d->size;
        free(d->data);
        d->data = nd;
        d->capacity = newcap;
    }
    d->data[d->size++] = v;
    return moved;
}
// #endregion push

static void da_free(DynArray *d) { free(d->data); d->data = NULL; }

/* ------- 以下为对比与计时代码，不参与 slides 引用 ------- */

static void check_data(const DynArray *d, int n)
{
    assert(d->size == n);
    assert(d->size <= d->capacity);
    for (int i = 0; i < n; i++) assert(d->data[i] == i);
}

static unsigned long long expected_copies(int n, int factor)
{
    if (factor == 1) return (unsigned long long)n * (n ? n - 1 : 0) / 2;
    unsigned long long capacity = 1;
    while (capacity < (unsigned)n) capacity *= 2;
    return capacity - 1;
}

/* 计数运行：累计实际搬迁量并核对结果，不用这一轮的耗时评价性能。 */
static unsigned long long count_run(int n, int factor)
{
    DynArray d; da_init(&d);
    unsigned long long copies = 0;
    for (int i = 0; i < n; i++) copies += da_push(&d, i, factor);
    check_data(&d, n);
    assert(copies == expected_copies(n, factor));
    da_free(&d);
    return copies;
}

static volatile unsigned long long sink;

/* 计时运行：初始分配、结果核对与最终销毁在外；扩容申请/复制/释放在内。
 * 不累计操作计数。结果在计时后被读取，避免把计算当成无用工作删除。
 */
static double run(int n, int factor)
{
    DynArray d; da_init(&d);
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < n; i++) da_push(&d, i, factor);
    auto end = std::chrono::steady_clock::now();
    check_data(&d, n);
    unsigned long long checksum = 0;
    for (int i = 0; i < n; i++) checksum += d.data[i];
    sink = checksum;
    da_free(&d);
    return std::chrono::duration<double>(end - start).count();
}

int main(void)
{
    for (int factor = 1; factor <= 2; factor++)
        for (int n = 0; n <= 128; n++) count_run(n, factor);
    assert(expected_copies(100000, 1) + 100000 == 5000050000ULL);
    assert(count_run(100000, 2) + 100000 == 231071);
    printf("计数与边界自测通过；以下用 steady_clock 测每次完整插入序列。\n");
    for (int n = 1000; n <= 32000; n *= 2) {
        for (int factor = 1; factor <= 2; factor++) {
            unsigned long long copies = count_run(n, factor);
            run(n, factor);                 /* 预热，不计入统计 */
            double samples[5];
            printf("n=%d %s 原始秒数：", n, factor == 1 ? "+1" : "x2");
            for (int r = 0; r < 5; r++) samples[r] = run(n, factor);
            for (double t : samples) printf(" %.9f", t);
            std::sort(samples, samples + 5);
            double t = samples[2];
            printf("\n  写入=%d 复制=%llu 总计=%llu 中位数=%.9fs t/n=%.3es\n",
                   n, copies, copies + n, t, t / n);
            if (t < 0.001) printf("  提醒：单次测量不足1ms，不据此强判耗时比值。\n");
        }
    }
    printf("\n计数模型：+1总成本=n(n+1)/2；倍增总成本<3n（n>=1）。\n");
    printf("分配器成本未计入元素次数，但计入插入耗时；有限测量不证明渐近界。\n");
    return 0;
}

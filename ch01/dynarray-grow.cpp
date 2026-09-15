/*
 * 第 1 章 绪论 · 动态数组：+1 扩容 vs 倍增扩容（含插入计时主程序）
 * 共用页：1.16.2 聚合分析 / 1.18.4 摊还实测（作业实验二主线）
 *
 * 运行方式：
 *   g++ -std=c++17 -O2 -Wall -Wextra -o /tmp/grow snippets/ch01/dynarray-grow.cpp && /tmp/grow
 * slides 引用方式：
 *   <<< @/snippets/ch01/dynarray-grow.cpp#push {lines:true}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>

// #region struct
typedef struct {
    int    *data;      /* 元素存储区 */
    int     size;      /* 当前元素个数 */
    int     capacity;  /* 当前容量 */
    long    copies;    /* 累计复制（搬家）次数——用来验证摊还分析 */
} DynArray;

static void da_init(DynArray *d)
{
    d->data = (int *)malloc(sizeof(int));   /* 初始容量 1 */
    d->size = 0;
    d->capacity = 1;
    d->copies = 0;
}
// #endregion struct

// #region push
/* 尾部插入：满则扩容。factor = 2 是倍增扩容；factor = 1 表示每次只 +1。
 * 基本操作 = 元素的写入 / 复制次数，全部计入 d->copies。
 */
static void da_push(DynArray *d, int v, int factor)
{
    if (d->size == d->capacity) {
        int newcap = (factor == 1) ? d->capacity + 1     /* +1 扩容 */
                                   : d->capacity * 2;    /* 倍增扩容 */
        int *nd = (int *)malloc(newcap * sizeof(int));
        memcpy(nd, d->data, d->size * sizeof(int));      /* 把旧元素全部搬过去 */
        d->copies += d->size;                            /* 搬了 size 个 */
        free(d->data);
        d->data = nd;
        d->capacity = newcap;
    }
    d->data[d->size++] = v;
    d->copies += 1;                                      /* 本次写入 */
}
// #endregion push

static void da_free(DynArray *d) { free(d->data); d->data = NULL; }

/* ------- 以下为对比与计时代码，不参与 slides 引用 ------- */

static double run(int n, int factor, long *out_copies)
{
    DynArray d; da_init(&d);
    clock_t t0 = clock();
    for (int i = 0; i < n; i++) da_push(&d, i, factor);
    clock_t t1 = clock();
    *out_copies = d.copies;
    da_free(&d);
    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

int main(void)
{
    int ns[] = { 10000, 100000 };
    printf("%10s | %14s %10s | %14s %10s\n",
           "n", "+1 copies", "+1 time", "x2 copies", "x2 time");
    for (int k = 0; k < 2; k++) {
        int n = ns[k];
        long c1, c2;
        double t1 = run(n, 1, &c1);
        double t2 = run(n, 2, &c2);
        printf("%10d | %14ld %10.4f | %14ld %10.4f\n", n, c1, t1, c2, t2);
    }
    printf("\n结论：+1 扩容 copies ≈ n^2/2（O(n^2)），倍增扩容 copies < 3n（O(n)）。\n");
    return 0;
}

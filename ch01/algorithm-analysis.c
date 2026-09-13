/*
 * 第 1 章 绪论 · 算法分析示例代码
 *
 * slides 引用方式：
 *   <<< @/snippets/ch01/algorithm-analysis.c#search {*}{lines:true}
 * 自测：
 *   cc -Wall -Wextra -o /tmp/ch01 snippets/ch01/algorithm-analysis.c && /tmp/ch01
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define N 8

// #region mul
/* 矩阵乘法：统计基本语句频度
 * T(n) = 2n³ + 3n² + 2n + 1  =>  O(n³)
 */
void mul(int a[][N], int b[][N], int c[][N], int n)
{
    for (int i = 0; i < n; i++)                   /* n + 1 次 */
        for (int j = 0; j < n; j++) {             /* n(n + 1) 次 */
            c[i][j] = 0;                          /* n² 次 */
            for (int k = 0; k < n; k++)
                c[i][j] += a[i][k] * b[k][j];     /* n³ 次 */
        }
}
// #endregion mul

// #region search
/* 顺序查找
 * 最好 O(1)：第 1 个就命中
 * 最坏 O(n)：在末尾或不存在
 * 平均：等概率时比较 (n + 1) / 2 次
 */
int search(const int a[], int n, int key)
{
    for (int i = 0; i < n; i++)
        if (a[i] == key)
            return i;        /* 找到，返回下标 */
    return -1;               /* 未找到 */
}
// #endregion search

// #region reverse
/* 数组逆置（原地工作）：S(n) = O(1) */
void reverse(int a[], int n)
{
    for (int i = 0; i < n / 2; i++) {
        int t = a[i];
        a[i] = a[n - 1 - i];
        a[n - 1 - i] = t;
    }
}
// #endregion reverse

// #region reverse2
/* 数组逆置（借助辅助数组）：S(n) = O(n) */
void reverse2(int a[], int n)
{
    int *b = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        b[i] = a[n - 1 - i];
    for (int i = 0; i < n; i++)
        a[i] = b[i];
    free(b);
}
// #endregion reverse2

/* ------- 以下为自测代码，不参与 slides 引用 ------- */

int main(void)
{
    /* search */
    int a[5] = { 11, 22, 33, 44, 55 };
    assert(search(a, 5, 11) == 0);
    assert(search(a, 5, 55) == 4);
    assert(search(a, 5, 99) == -1);

    /* reverse / reverse2 结果必须一致 */
    int x[5] = { 1, 2, 3, 4, 5 };
    int y[5] = { 1, 2, 3, 4, 5 };
    reverse(x, 5);
    reverse2(y, 5);
    for (int i = 0; i < 5; i++) {
        assert(x[i] == y[i]);
        assert(x[i] == 5 - i);
    }

    /* mul：单位矩阵乘法应得回原矩阵 */
    int m1[N][N] = { { 0 } }, id[N][N] = { { 0 } }, out[N][N] = { { 0 } };
    for (int i = 0; i < N; i++) {
        id[i][i] = 1;
        for (int j = 0; j < N; j++)
            m1[i][j] = i * N + j;
    }
    mul(m1, id, out, N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            assert(out[i][j] == m1[i][j]);

    printf("all tests passed\n");
    return 0;
}

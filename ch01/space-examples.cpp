/*
 * 第 1 章 绪论 · 辅助空间与递归栈
 * 同一逆序任务：原地交换 / 借助辅助数组；选读：线性递归求和。
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

/* ② S(n) = Θ(n)：借助独立的 n 元素辅助数组 b，再复制回 a。
 * a 与 b 不重叠；b 即使由调用方提供，也计入这项逆序任务的辅助空间。
 */
void reverse_copy(int a[], int b[], int n)
{
    for (int i = 0; i < n; i++)
        b[i] = a[n - 1 - i];
    for (int i = 0; i < n; i++)
        a[i] = b[i];
}
// #endregion inplace

// #region recur
/* ③ 普通调用栈模型：n+1 个同时存活的帧（含 n=0 终止帧），S(n)=Θ(n)。
 * 前提为 n>=0 且求和不溢出；实际栈字节数与编译器优化不由此模型保证。
 */
int sum_rec(const int a[], int n)
{
    if (n == 0) return 0;
    return a[n - 1] + sum_rec(a, n - 1);
}

// #endregion recur

/* ------- 以下为自测代码，不参与 slides 引用 ------- */

int main(void)
{
    for (int n = 0; n <= 64; n++) {
        int a[64], c[64], b[64];
        for (int i = 0; i < n; i++) a[i] = c[i] = i;
        reverse(a, n);
        reverse_copy(c, b, n);
        for (int i = 0; i < n; i++) {
            assert(a[i] == n - 1 - i);
            assert(a[i] == c[i]);          /* 同样将逆序结果写回输入数组 */
        }
    }
    reverse(NULL, 0);
    reverse_copy(NULL, NULL, 0);

    int s[5] = { 1, 2, 3, 4, 5 };
    assert(sum_rec(s, 5) == 15);
    assert(sum_rec(s, 0) == 0);                          /* 边界：空 */

    assert(sum_rec(NULL, 0) == 0);
    assert(sum_rec(s, 1) == 1);

    printf("逆序与求和结果测试通过；这些断言不测量实际栈空间。\n");
    return 0;
}

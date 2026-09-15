/*
 * 第 1 章 绪论 · 二进制计数器加 1，统计总位翻转次数
 * 共用页：1.16.3（记账法/聚合分析的第二个例子）
 *
 * 运行方式：
 *   g++ -std=c++17 -Wall -Wextra -o /tmp/bincounter snippets/ch01/bincounter.cpp && /tmp/bincounter
 * slides 引用方式：
 *   <<< @/snippets/ch01/bincounter.cpp#inc {lines:true}
 */

#include <stdio.h>

#define K 8                       /* 计数器位宽 */

// #region inc
/* 对 k 位二进制计数器做 +1，返回本次翻转的位数。
 * 基本操作 = 位翻转次数：从最低位起，遇 1 翻成 0 继续进位，遇 0 翻成 1 停止。
 * 最坏单次 O(k)（全为 1 时全部进位），但连续 n 次的总翻转数 < 2n。
 */
int increment(unsigned char bits[], int k)
{
    int i = 0, flips = 0;
    while (i < k && bits[i] == 1) {   /* 该位是 1：翻成 0，进位 */
        bits[i] = 0;
        flips++;
        i++;
    }
    if (i < k) {                      /* 遇到 0：翻成 1，停止 */
        bits[i] = 1;
        flips++;
    }
    return flips;
}
// #endregion inc

/* ------- 以下为统计代码，不参与 slides 引用 ------- */

static void print_bits(unsigned char bits[], int k)
{
    for (int i = k - 1; i >= 0; i--) putchar(bits[i] ? '1' : '0');
}

int main(void)
{
    unsigned char bits[K] = { 0 };

    printf("前 16 次 +1 的逐次翻转：\n");
    printf("%4s %10s %8s\n", "step", "value", "flips");
    for (int step = 1; step <= 16; step++) {
        int f = increment(bits, K);
        printf("%4d  ", step);
        print_bits(bits, K);
        printf("  %5d\n", f);
    }

    /* 聚合验证：连续 n 次 +1 的总翻转数 < 2n */
    unsigned char b2[K] = { 0 };
    int n = 200;                       /* 200 < 2^K，不会溢出 */
    long sum = 0;
    for (int i = 0; i < n; i++) sum += increment(b2, K);
    printf("\n连续 %d 次 +1，总翻转 = %ld，2n = %d  =>  %s\n",
           n, sum, 2 * n, sum < 2 * n ? "总翻转 < 2n，摊还 O(1) 成立" : "异常");
    printf("按位聚合：第 j 位翻转 n/2^j 次，Σ = n + n/2 + n/4 + … < 2n\n");
    return 0;
}

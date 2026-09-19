/*
 * 第 1 章 绪论 · 二进制计数器加 1，统计总位翻转次数
 * 选读主题：记账法与二进制计数器。
 *
 * 运行方式：
 *   g++ -std=c++17 -Wall -Wextra -o /tmp/bincounter snippets/ch01/bincounter.cpp && /tmp/bincounter
 * slides 引用方式：
 *   <<< @/snippets/ch01/bincounter.cpp#inc {lines:true}
 */

#include <stdio.h>
#include <assert.h>

#define K 8                       /* 计数器位宽 */

// #region inc
/* 对 k 位二进制计数器做 +1，返回本次翻转的位数。
 * 基本操作 = 位翻转次数：从最低位起，遇 1 翻成 0 继续进位，遇 0 翻成 1 停止。
 * 最坏单次 O(k)（全为1时进位后回到0）；从全0开始，连续n>=1次总翻转数<2n。
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

    /* 核对每个前缀，包含255→0的溢出边界；实验只核对，不替代一般证明。 */
    unsigned char b2[K] = { 0 };
    int n = 512;
    long sum = 0, by_bit[K] = {0};
    for (int step = 1; step <= n; step++) {
        unsigned char before[K];
        for (int j = 0; j < K; j++) before[j] = b2[j];
        int flips = increment(b2, K), observed = 0, value = 0;
        for (int j = 0; j < K; j++) {
            int changed = before[j] != b2[j];
            observed += changed;
            by_bit[j] += changed;
            assert(by_bit[j] == step / (1 << j));
            value += b2[j] * (1 << j);
        }
        assert(flips == observed && value == step % (1 << K));
        sum += flips;
        assert(sum < 2L * step);
    }
    printf("\n前%d次的计数与溢出边界通过，总翻转=%ld < 2n=%d\n", n, sum, 2*n);
    for (int j = 0; j < K; j++) printf("第%d位翻转%ld次\n", j, by_bit[j]);
    printf("按位聚合公式：第j位翻转floor(n/2^j)次；有限k位求和<2n（n>=1）。\n");
    return 0;
}

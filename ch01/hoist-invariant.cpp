/*
 * 第 1 章 绪论 · 循环不变计算外提（loop-invariant code motion）
 * 未外提版与外提版功能相同；源码成本模型与编译后的实测分开解释。
 * 共用主题：重复调用的成本与循环不变计算外提。
 *
 * 运行方式（务必统一编译选项，本例故意用 -O0 以保留 strlen 的重复调用）：
 *   g++ -std=c++17 -O0 -Wall -Wextra -o /tmp/hoist snippets/ch01/hoist-invariant.cpp && /tmp/hoist
 * slides 引用方式：
 *   <<< @/snippets/ch01/hoist-invariant.cpp#bad {lines:true}
 *   <<< @/snippets/ch01/hoist-invariant.cpp#good {lines:true}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <assert.h>

static volatile unsigned long long sink; /* 保留结果读写，但不阻止编译器外提 strlen */

// #region bad
/* 未外提版：长度为 n 的字符串，源码条件中的 strlen 求值 n+1 次。
 * 按逐字符扫描模型，每次检查 n+1 个字符，总检查 (n+1)² 次。
 * 标准库可向量化，编译器也可能外提调用；计数模型不是底层指令数。
 */
void process_bad(const char *str)
{
    for (size_t i = 0; i < strlen(str); i++)
        sink += str[i];
}
// #endregion bad

// #region good
/* 外提版：先缓存长度，逐字符模型只做 n+1 次长度检查，整体 Θ(n)。
 * 本例字符串不变，strlen 无需保留重复调用的副作用，外提保持语义。
 * 一般函数即使返回值不变，也须检查副作用，不能一概外提。
 */
void process_good(const char *str)
{
    size_t len = strlen(str);     /* 只算一次 */
    for (size_t i = 0; i < len; i++)
        sink += str[i];
}
// #endregion good

/* ------- 以下为计时对比代码，不参与 slides 引用 ------- */

static double timeit(void (*fn)(const char *), const char *s, int rounds)
{
    clock_t t0 = clock();
    for (int r = 0; r < rounds; r++) fn(s);
    clock_t t1 = clock();
    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

int main(void)
{
    sink = 0; process_bad(""); process_good(""); assert(sink == 0);
    sink = 0; process_bad("abc"); unsigned long long small = sink;
    sink = 0; process_good("abc"); assert(sink == small);

    int n = 20000;                          /* 字符串长度 */
    char *s = (char *)malloc((size_t)n + 1);
    if (!s) { perror("字符串分配失败"); return EXIT_FAILURE; }
    unsigned long long expected = 0;
    for (int i = 0; i < n; i++) {
        s[i] = 'a' + (i % 26);
        expected += s[i];
    }
    s[n] = '\0';

    int rounds = 20;
    sink = 0; double tb = timeit(process_bad,  s, rounds); unsigned long long sb = sink;
    sink = 0; double tg = timeit(process_good, s, rounds); unsigned long long sg = sink;
    assert(sb == expected * rounds && sg == expected * rounds);

    printf("n=%d, rounds=%d，clock() CPU时间（每版一组教学对照）\n", n, rounds);
    printf("未外提：%.6f s；外提后：%.6f s\n", tb, tg);
    if (tb > 0 && tg > 0) printf("本次时间比值：%.2f\n", tb / tg);
    else printf("时间不足以计算有效比值\n");
    printf("结果核对通过：%llu == %llu\n", sb, sg);
    printf("不承诺固定加速比；-O2可能消除重复调用，比较时记录编译器和选项。\n");

    free(s);                                /* 上次课：有分配必有释放 */
    return 0;
}

/*
 * 第 1 章 绪论 · 循环不变量未外提（failure to hoist loop-invariant computation）
 * 错误版与外提版各自可计时对比，实测差距非常直观。
 * 共用页：1.11.5
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

static long sink;                 /* 防止编译器把整段循环优化掉 */

// #region bad
/* 错误版：for 条件里的 strlen 每轮迭代前都重新求值
 * strlen 是 O(n)，被算了 n 次  =>  T(n) = n²
 */
void process_bad(const char *str)
{
    for (int i = 0; i < (int)strlen(str); i++)
        sink += str[i];
}
// #endregion bad

// #region good
/* 正确版：把循环不变量外提到循环前，用局部变量缓存
 * 条件退化为 O(1) 的比较  =>  整体回到 O(n)
 * 判据：条件里的调用若在循环体内不会改变返回值，就必须外提。
 */
void process_good(const char *str)
{
    int len = (int)strlen(str);    /* 只算一次 */
    for (int i = 0; i < len; i++)
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
    int n = 20000;                          /* 字符串长度 */
    char *s = (char *)malloc(n + 1);
    for (int i = 0; i < n; i++) s[i] = 'a' + (i % 26);
    s[n] = '\0';

    int rounds = 20;
    sink = 0; double tb = timeit(process_bad,  s, rounds); long sb = sink;
    sink = 0; double tg = timeit(process_good, s, rounds); long sg = sink;

    printf("n = %d, rounds = %d\n", n, rounds);
    printf("未外提 O(n^2): %.4f s\n", tb);
    printf("外提后 O(n)  : %.4f s\n", tg);
    printf("加速比        : %.1f 倍\n", tg > 0 ? tb / tg : 0.0);
    printf("结果一致校验  : %s (%ld == %ld)\n", sb == sg ? "OK" : "MISMATCH", sb, sg);

    free(s);                                /* 上次课：有分配必有释放 */
    return 0;
}

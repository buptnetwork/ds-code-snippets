/*
 * 第 1 章 绪论 · 计时与倍增实验：已学循环骨架，不要求排序前置知识。
 * 单调时钟、批量重复、5组中位数；有限数据只检查模型，不证明渐近界。
 * 不同骨架完成不同工作，不能按绝对耗时评选“最好算法”。
 *
 * 运行方式（实验一律统一 -O2，并把编译选项写进报告）：
 *   g++ -std=c++17 -O2 -Wall -Wextra -o /tmp/timing snippets/ch01/timing-framework.cpp && /tmp/timing
 * slides 引用方式：
 *   <<< @/snippets/ch01/timing-framework.cpp#measure {lines:true}
 *   <<< @/snippets/ch01/timing-framework.cpp#main {lines:true}
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
#include <chrono>
#include <cmath>

/* volatile 输入读取保留循环中的访存工作，避免被常量折叠或移出批量循环。
 * 这是可解释的教学基准，会影响优化，不代表生产实现的绝对性能。
 */
using Kernel = unsigned long long (*)(const volatile int *, int);
static volatile unsigned long long sink;

// #region kernels
static unsigned long long linear_work(const volatile int a[], int n)
{
    unsigned long long sum = 0;
    for (int i = 0; i < n; i++) sum += a[i];
    return sum;
}

static unsigned long long triangle_work(const volatile int a[], int n)
{
    unsigned long long equal_pairs = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            equal_pairs += (a[i] == a[j]);
    return equal_pairs;
}

static unsigned long long nlogn_work(const volatile int a[], int n)
{
    unsigned long long sum = 0;
    for (int i = 0; i < n; i++)
        for (long long j = 1; j <= n; j *= 2)
            sum += a[j - 1];
    return sum;
}
// #endregion kernels

/* ---- 数据生成（不计时）---- */
static int *gen_random(int n)
{
    int *d = (int *)malloc((size_t)n * sizeof(int));
    if (!d) { perror("数据分配失败"); exit(EXIT_FAILURE); }
    for (int i = 0; i < n; i++) d[i] = rand() % 1000;
    return d;
}

// #region measure
/* 计时包含被测函数与批量调用的少量开销，不含造数据和打印。
 * 本例只读输入，不需复位；换成修改输入的算法时必须另行设计复位边界。
 */
static double run_batch(Kernel algo, const volatile int *data, int n, int reps)
{
    unsigned long long checksum = 0;
    auto start = std::chrono::steady_clock::now();
    for (int r = 0; r < reps; r++) checksum += algo(data, n);
    auto end = std::chrono::steady_clock::now();
    sink = checksum;
    return std::chrono::duration<double>(end - start).count();
}

/* 先把批量时长校准到约10ms，再测5组，返回单次时间的中位数。 */
double measure(Kernel algo, const volatile int *data, int n,
               int *used_reps = nullptr, double *raw_batch_seconds = nullptr)
{
    int reps = 1;
    const int max_reps = 1 << 20;
    double elapsed = run_batch(algo, data, n, reps);
    while (elapsed < 0.01 && reps < max_reps) {
        reps *= 2;
        elapsed = run_batch(algo, data, n, reps);
    }
    if (elapsed < 0.01)
        fprintf(stderr, "n=%d：批量时长仍不足10ms，请谨慎解释比值\n", n);
    double samples[5];
    for (int r = 0; r < 5; r++) {
        double batch = run_batch(algo, data, n, reps);
        if (raw_batch_seconds) raw_batch_seconds[r] = batch;
        samples[r] = batch / reps;
    }
    std::sort(samples, samples + 5);
    if (used_reps) *used_reps = reps;
    return samples[2];
}
// #endregion measure

// #region main
/* 局部比值与归一化值提供证据，不把指定ratio当作测试断言。 */
static void doubling(Kernel algo, const char *name, int nmax)
{
    printf("\n%s\n%8s %12s %8s %8s %12s %12s %12s\n",
           name, "n", "time(s)", "ratio", "reps", "t/n", "t/(n log2n)", "t/n^2");
    double prev = 0;
    for (int n = 1000; n <= nmax; n *= 2) {
        int *data = gen_random(n);
        int reps;
        double raw[5];
        double t = measure(algo, data, n, &reps, raw);
        printf("%8d %12.8f ", n, t);
        if (prev > 0 && t > 0) printf("%8.3f ", t / prev);
        else printf("%8s ", "--");
        printf("%8d %12.3e %12.3e %12.3e\n", reps,
               t / n, t / (n * std::log2(n)), t / ((double)n * n));
        printf("  5组原始批量秒数（每组%d次）：", reps);
        for (double batch : raw) printf(" %.9f", batch);
        putchar('\n');
        prev = t;
        free(data);
    }
}

int main(void)
{
    int test[] = {1, 1, 2, 2};
    assert(linear_work(test, 4) == 6);
    assert(triangle_work(test, 4) == 2);
    assert(nlogn_work(test, 4) == 16);
    assert(linear_work(nullptr, 0) == 0);
    assert(triangle_work(nullptr, 0) == 0);
    assert(nlogn_work(nullptr, 0) == 0);
    assert(triangle_work(test, 1) == 0);
    int ones[64];
    for (int &value : ones) value = 1;
    for (int n = 0; n <= 64; n++) {
        unsigned long long levels = 0;
        for (int v = n; v > 0; v /= 2) levels++;
        assert(linear_work(ones, n) == (unsigned)n);
        assert(triangle_work(ones, n) == (unsigned)(n * (n - 1) / 2));
        assert(nlogn_work(ones, n) == n * levels);
    }
    printf("三个循环骨架的结果与边界自测通过\n");
    srand(20260915);
    printf("steady_clock；5组批量时间的中位数；读取volatile输入的教学基准\n");
    doubling(linear_work, "单层扫描：基本操作n次", 128000);
    doubling(triangle_work, "三角循环：元素比较n(n-1)/2次", 16000);
    doubling(nlogn_work, "线性外层×翻倍内层：n(floor(log2n)+1)次", 128000);
    printf("\n校验值=%llu；有限测量只支持或质疑模型，不证明渐近界。\n", sink);
    return 0;
}
// #endregion main

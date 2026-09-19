/*
 * 第 1 章 绪论 · 计数四步法示例 1–8 合集（每例注释写出 T(n)）
 * 共用页：1.11.2 例 1–3 / 1.11.3 例 4–5 / 1.11.4 例 6 / 1.11.5 例 7 / 1.11.6 例 8
 *
 * 运行方式：
 *   g++ -std=c++17 -O2 -Wall -Wextra -o /tmp/count snippets/ch01/count-examples.cpp && /tmp/count
 * slides 引用方式（按需取单个 region）：
 *   <<< @/snippets/ch01/count-examples.cpp#ex4 {lines:true}
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

static long cnt;                 /* 逻辑操作计数，不是机器指令数；不用于计时 */
static long sink;
static long length_calls;
static long processed;
static int  a[1024];
static int  c[64][64];

// #region ex1
/* 例 1｜O(1)：与 n 无关，共 3 次基本操作  =>  T(n) = 3  =>  O(1) */
void ex1(void)
{
    int x = 0, y = 1;
    cnt += 2;                    /* 两次初始化写入 */
    x = x + y;
    cnt++;                       /* 一次赋值；本例不另计加法 */
    sink += x;
}
// #endregion ex1

// #region ex2
/* 例 2｜O(n)：单层循环，循环体执行 n 次  =>  T(n) = n  =>  O(n) */
void ex2(int n)
{
    long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
        cnt++;
    }
    sink += sum;
}
// #endregion ex2

// #region ex3
/* 例 3｜O(n²)：完全嵌套  =>  T(n) = n × n = n²  =>  O(n²) */
void ex3(int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            c[i][j] = 0;
            cnt++;
        }
}
// #endregion ex3

// #region ex4
/* 例 4｜O(n²)：三角形嵌套（内层上界是 i，依赖外层）
 * T(n) = 0 + 1 + … + (n-1) = n(n-1)/2 = n²/2 - n/2  =>  O(n²)
 * 关键：常数因子 1/2 被丢掉，「只跑一半」和「跑满」是同一个阶。
 */
void ex4(int n)
{
    long t = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            t++;
    cnt += t;
}
// #endregion ex4

// #region ex5
/* 例 5｜O(log n)：乘除型循环（循环变量翻倍）
 * 第 k 次后 i = 2^k；退出条件 2^k > n  =>  k > log₂n
 * T(n) = ⌊log₂n⌋ + 1  =>  O(log n)（大 O 里对数不写底数）
 */
void ex5(int n)
{
    long t = 0;
    long long i = 1;             /* n 为 int，最后一次翻倍仍可表示 */
    while (i <= n) {
        i = i * 2;
        t++;
    }
    cnt += t;
}
// #endregion ex5

// #region ex6
/* 例 6｜加法法则 + 乘法法则
 * 前段 O(n)；后段外层 n × 内层 log n = O(n log n)；
 * 整体 O(n) + O(n log n) = O(n log n)（顺序取大，嵌套相乘）
 */
void ex6(int n)
{
    long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
        cnt++;
    }
    for (int i = 0; i < n; i++)
        for (long long j = 1; j <= n; j *= 2)
            cnt++;
    sink += sum;
}
// #endregion ex6

// #region ex7
/* 例 7：用逐字符扫描模拟长度计算，每次检查 n+1 个字符（含终止符）。
 * 未外提调用 n+1 次，字符检查数为 (n+1)^2；外提后为 n+1。
 * 这不是 libc strlen 的底层指令计数；两种写法处理的字符数相同。
 */
static size_t counted_length(const char *str)
{
    length_calls++;
    size_t len = 0;
    for (;;) {
        cnt++;
        if (str[len] == '\0') return len;
        len++;
    }
}

void process(char ch) { sink += ch; processed++; }

void ex7_bad(const char *str)
{
    for (size_t i = 0; i < counted_length(str); i++)
        process(str[i]);
}

/* 外提保持字符串处理语义；计数器仅为教学插桩，不属于功能契约。 */
void ex7_good(const char *str)
{
    size_t len = counted_length(str);
    for (size_t i = 0; i < len; i++)
        process(str[i]);
}
// #endregion ex7

// #region ex8
/* 例 8｜依赖输入的循环（为 1.15 最好/最坏/平均埋线）
 * 统计元素比较：首次命中下标 p 时 p+1 次，不存在时 n 次；空数组为 0。
 */
static bool matches(int value, int key)
{
    cnt++;
    return value == key;
}

int ex8(int n, int key)
{
    int i = 0;
    while (i < n && !matches(a[i], key))
        i++;
    return i < n ? i : -1;
}
// #endregion ex8

/* ------- 以下为自测代码，不参与 slides 引用 ------- */

int main(void)
{
    for (int i = 0; i < 1024; i++) a[i] = i;
    cnt = 0; ex1(); assert(cnt == 3);
    printf("ex1 变量写入 = %ld\n", cnt);

    for (int n = 0; n <= 64; n++) {
        long levels = 0;
        for (int v = n; v > 0; v /= 2) levels++;
        cnt = 0; ex2(n); assert(cnt == n);
        if (n == 16) printf("ex2 循环体 = %ld\n", cnt);
        cnt = 0; ex3(n); assert(cnt == (long)n * n);
        if (n == 16) printf("ex3 数组赋值 = %ld\n", cnt);
        cnt = 0; ex4(n); assert(cnt == (long)n * (n - 1) / 2);
        if (n == 16) printf("ex4 递增 = %ld\n", cnt);
        cnt = 0; ex5(n); assert(cnt == levels);
        if (n == 16) printf("ex5 翻倍 = %ld\n", cnt);
        cnt = 0; ex6(n); assert(cnt == n + n * levels);
        if (n == 16) printf("ex6 两段循环体合计 = %ld\n", cnt);
        for (int key = -1; key <= n; key++) {
            cnt = 0;
            int found = ex8(n, key);
            int expected = key >= 0 && key < n ? key : -1;
            assert(found == expected);
            assert(cnt == (expected >= 0 ? expected + 1 : n));
        }
    }

    char s[33];
    for (int n = 0; n <= 32; n++) {
        memset(s, 'a', n); s[n] = '\0';
        cnt = length_calls = processed = sink = 0;
        ex7_bad(s);
        assert(cnt == (long)(n + 1) * (n + 1));
        assert(length_calls == n + 1 && processed == n);
        long bad = cnt, result = sink;
        cnt = length_calls = processed = sink = 0;
        ex7_good(s);
        assert(cnt == n + 1 && length_calls == 1 && processed == n);
        assert(sink == result);
        if (n == 5) printf("ex7 字符检查：未外提=%ld，外提=%ld\n", bad, cnt);
    }
    a[0] = a[1] = 2;
    cnt = 0; assert(ex8(3, 2) == 0 && cnt == 1);
    printf("ex8 首次命中与短路边界通过；全部计数断言通过\n");
    return 0;
}

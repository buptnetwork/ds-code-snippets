/*
 * 第 1 章 绪论 · 计数四步法示例 1–8 合集（每例注释写出 T(n)）
 * 共用页：1.11.2 例 1–3 / 1.11.3 例 4–5 / 1.11.4 例 6 / 1.11.5 例 7 / 1.11.6 例 8
 *
 * 运行方式：
 *   g++ -std=c++17 -O2 -Wall -Wextra -o /tmp/count snippets/ch01/count-examples.cpp && /tmp/count
 * slides 引用方式（按需取单个 region）：
 *   <<< @/snippets/ch01/count-examples.cpp#ex4 {lines:true}
 */

#include <stdio.h>
#include <string.h>

static long cnt;                 /* 统一用 cnt 累加基本操作次数 */
static int  a[1024];
static int  c[64][64];

// #region ex1
/* 例 1｜O(1)：与 n 无关，共 3 次基本操作  =>  T(n) = 3  =>  O(1) */
void ex1(void)
{
    int x = 0, y = 1;
    x = x + y;                   /* 哪怕是 1000 行顺序语句，只要与 n 无关就是 O(1) */
    cnt += x;
}
// #endregion ex1

// #region ex2
/* 例 2｜O(n)：单层循环，循环体执行 n 次  =>  T(n) = n  =>  O(n) */
void ex2(int n)
{
    long sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];
    cnt += sum;
}
// #endregion ex2

// #region ex3
/* 例 3｜O(n²)：完全嵌套  =>  T(n) = n × n = n²  =>  O(n²) */
void ex3(int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            c[i][j] = 0;
    cnt++;
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
    int i = 1;
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
    long sum = 0, t = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];
    for (int i = 0; i < n; i++)
        for (int j = 1; j <= n; j *= 2)
            t++;
    cnt += sum + t;
}
// #endregion ex6

// #region ex7
/* 例 7｜O(n²)：循环条件里藏 O(n)——循环不变量未外提
 * strlen 本身 O(n)，for 条件每轮迭代前都重新求值，被算了 n 次
 * =>  T(n) = n × n = n²（看起来一层循环，其实是乘法法则的隐蔽形态）
 */
void process(char ch) { cnt += ch; }

void ex7_bad(char *str)
{
    for (int i = 0; i < (int)strlen(str); i++)   /* 条件每轮重算 strlen */
        process(str[i]);
}

/* 正确版：把循环不变量外提，条件退化为 O(1) 比较 =>  整体回到 O(n) */
void ex7_good(char *str)
{
    int len = (int)strlen(str);                  /* 只算一次 */
    for (int i = 0; i < len; i++)
        process(str[i]);
}
// #endregion ex7

// #region ex8
/* 例 8｜依赖输入的循环（为 1.15 最好/最坏/平均埋线）
 * 次数取决于 key 在哪：可能 1 次，也可能 n 次 —— 到底报哪个？下半节课回答。
 */
int ex8(int n, int key)
{
    int i = 0;
    while (i < n && a[i] != key)
        i++;
    return i;
}
// #endregion ex8

/* ------- 以下为自测代码，不参与 slides 引用 ------- */

int main(void)
{
    int n = 16;
    for (int i = 0; i < n; i++) a[i] = i;

    cnt = 0; ex1();          printf("ex1 done\n");
    cnt = 0; ex2(n);         printf("ex2 done (循环体执行 n=%d 次)\n", n);
    cnt = 0; ex3(n);         printf("ex3 done\n");
    cnt = 0; ex4(n);         printf("ex4 T = %ld (期望 n(n-1)/2=%d)\n", cnt, n * (n - 1) / 2);
    cnt = 0; ex5(n);         printf("ex5 T = %ld (期望 ⌊log₂n⌋+1=5)\n", cnt);
    cnt = 0; ex6(n);         printf("ex6 done\n");

    char s[] = "hello";
    cnt = 0; ex7_bad(s);  long bad = cnt;
    cnt = 0; ex7_good(s); long good = cnt;
    printf("ex7 bad=%ld good=%ld (处理字符总数应相同)\n", bad, good);

    cnt = 0; printf("ex8 hit index = %d\n", ex8(n, 5));

    printf("all examples run\n");
    return 0;
}

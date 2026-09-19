/*
 * 第 1 章 绪论 · 对拍框架（stress testing）
 * 契约：返回 key 首次出现的下标；不存在返回 -1。
 * 故意错误版本返回最后一次命中；先固定反例，再随机对拍。
 * 共用页：1.18.2
 *
 * 运行方式：
 *   g++ -std=c++17 -Wall -Wextra -o /tmp/stress snippets/ch01/stress-test.cpp && /tmp/stress
 * slides 引用方式：
 *   <<< @/snippets/ch01/stress-test.cpp#frame {lines:true}
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #region naive
/* 参考实现：遇到第一次匹配立即返回。 */
int reference_search(const int a[], int n, int key)
{
    for (int i = 0; i < n; i++)
        if (a[i] == key) return i;
    return -1;
}
// #endregion naive

// #region buggy
/* 故意错误：后续命中覆盖已有结果，违反“首次命中”契约。
 * 固定反例：[2,2,1] 查找2，期望0，实际1。
 */
int bad_search(const int a[], int n, int key)
{
    int found = -1;
    for (int i = 0; i < n; i++)
        if (a[i] == key) found = i;
    return found;
}
// #endregion buggy

// #region frame
/* 小规模、小值域，保留原始输入。发现差异返回1，未发现返回0。
 * 随机测试是补充；通过有限轮次不构成正确性证明。
 */
int stress(int (*my_search)(const int *, int, int), int rounds)
{
    for (int t = 0; t < rounds; t++) {
        int n = rand() % 10;
        int a[10];
        for (int i = 0; i < n; i++) a[i] = rand() % 5;
        int key = rand() % 6;
        int expected = reference_search(a, n, key);
        int actual = my_search(a, n, key);
        if (actual != expected) {
            printf("第%d轮：n=%d key=%d 原始输入=[", t + 1, n, key);
            for (int i = 0; i < n; i++) printf("%s%d", i ? "," : "", a[i]);
            printf("] 期望=%d 实际=%d\n", expected, actual);
            return 1;
        }
    }
    printf("%d轮测试通过，不等于正确性证明\n", rounds);
    return 0;
}
// #endregion frame

int main(void)
{
    int a[] = {2, 2, 1};
    assert(reference_search(NULL, 0, 2) == -1);
    assert(reference_search(a, 1, 2) == 0);
    assert(reference_search(a, 1, 1) == -1);
    assert(reference_search(a, 3, 1) == 2);
    assert(reference_search(a, 3, 9) == -1);
    assert(reference_search(a, 3, 2) == 0);
    assert(bad_search(a, 3, 2) == 1);
    printf("固定反例：[2,2,1] key=2，期望0，错误实现返回1\n");

    srand(12345);                    /* 相同环境便于复现，不保证跨平台轮数 */
    if (stress(reference_search, 1000) != 0) return 1;
    printf("对拍故意错误的查找实现：\n");
    if (!stress(bad_search, 1000))
        printf("本轮未随机发现；固定反例仍已证明契约违反\n");
    return 0;
}

/*
 * 第 1 章 绪论 · 对拍框架（stress testing）
 * 含一个「在含重复元素时出错」的排序，供现场演示对拍抓 bug。
 * 共用页：1.18.2
 *
 * 运行方式：
 *   g++ -std=c++17 -Wall -Wextra -o /tmp/stress snippets/ch01/stress-test.cpp && /tmp/stress
 * slides 引用方式：
 *   <<< @/snippets/ch01/stress-test.cpp#frame {lines:true}
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// #region naive
/* 朴素但确信正确的排序（插入排序）——对拍的「标准答案」 */
void naive_sort(int a[], int n)
{
    for (int i = 1; i < n; i++) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) { a[j + 1] = a[j]; j--; }
        a[j + 1] = key;
    }
}
// #endregion naive

// #region buggy
/* 待测算法：一个「自作聪明」的冒泡排序。
 * BUG：本趟遇到一对相等元素，就误判为「已无逆序」而提前退出，
 * 于是当数组里混有重复元素、且相等对出现在尚未排好序的位置时，排序不完整。
 * 元素全不相同时它表现正常 —— 正是这种「大多数时候对」的 bug 最难肉眼发现。
 */
void bad_sort(int a[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (a[j] == a[j + 1]) { swapped = 0; break; }   /* 误判：见相等即收工 */
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}
// #endregion buggy

// #region frame
/* 对拍骨架（可直接用于作业）：
 * ① 小规模（n ≤ 9）——抓到反例才看得懂；② 小值域（0..4）——制造重复元素。
 * 同时跑待测算法与朴素算法，输出不一致就打印反例并退出。
 */
int stress(void (*my_sort)(int *, int), int rounds)
{
    for (int t = 0; t < rounds; t++) {
        int n = rand() % 10;                       /* 0..9，含空数组与单元素 */
        int a[10], b[10];
        for (int i = 0; i < n; i++) { a[i] = rand() % 5; b[i] = a[i]; }
        my_sort(a, n);
        naive_sort(b, n);
        for (int i = 0; i < n; i++)
            if (a[i] != b[i]) {
                printf("第 %d 轮发现反例 n=%d\n", t, n);
                printf("  待测输出: "); for (int k = 0; k < n; k++) printf("%d ", a[k]);
                printf("\n  正确答案: "); for (int k = 0; k < n; k++) printf("%d ", b[k]);
                printf("\n");
                return 1;
            }
    }
    printf("%d 轮全部通过\n", rounds);
    return 0;
}
// #endregion frame

int main(void)
{
    srand(12345);                                  /* 固定种子，课堂可复现 */
    printf("=== 对拍朴素算法自身（应通过）===\n");
    stress(naive_sort, 1000);
    printf("\n=== 对拍待测的 bad_sort（应抓到反例）===\n");
    stress(bad_sort, 1000);
    return 0;
}

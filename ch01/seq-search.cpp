/*
 * 第 1 章 绪论 · 顺序查找（含 assert 版本）
 * 共用页：1.10.3 三种写法 / 1.15.1 最好最坏平均 / 1.18.2 断言
 *
 * 运行方式：
 *   g++ -std=c++17 -Wall -Wextra -o /tmp/seq-search snippets/ch01/seq-search.cpp && /tmp/seq-search
 * slides 引用方式：
 *   <<< @/snippets/ch01/seq-search.cpp#plain {lines:true}
 *   <<< @/snippets/ch01/seq-search.cpp#assert {lines:true}
 */

#include <assert.h>
#include <stdio.h>

// #region plain
/* 顺序查找：从头到尾逐个比较，命中返回下标，找不到返回 -1
 * 最好 O(1)：第 1 个就命中
 * 最坏 O(n)：在末尾或根本不存在
 * 平均：key 一定存在且等概率时，比较 (n + 1) / 2 次  =>  O(n)
 */
int seq_search(const int a[], int n, int key)
{
    for (int i = 0; i < n; i++)
        if (a[i] == key)
            return i;        /* 找到，返回首次出现的下标 */
    return -1;               /* 全部比完仍未找到 */
}
// #endregion plain

// #region assert
/* 带前置条件断言的版本：assert 只用来抓「程序员的错」，
 * 在 -DNDEBUG 发布版会被整体删除，因此绝不能拿它处理用户输入。
 */
int seq_search_checked(const int a[], int n, int key)
{
    assert(n >= 0);                    /* 前置条件：元素个数非负 */
    assert(n == 0 || a != NULL);       /* 有元素时指针不能为空 */
    for (int i = 0; i < n; i++)
        if (a[i] == key)
            return i;
    return -1;
}
// #endregion assert

/* ------- 以下为自测代码，不参与 slides 引用 ------- */

int main(void)
{
    int a[5] = { 11, 22, 33, 44, 55 };

    /* 一般情况 */
    assert(seq_search(a, 5, 11) == 0);   /* 目标在首位：最好情况 */
    assert(seq_search(a, 5, 33) == 2);
    assert(seq_search(a, 5, 55) == 4);   /* 目标在末位 */
    assert(seq_search(a, 5, 99) == -1);  /* 目标不存在：最坏情况 */

    /* 边界情况：n = 0 空输入，不得越界 */
    assert(seq_search(a, 0, 11) == -1);
    /* 边界情况：n = 1 单元素 */
    assert(seq_search(a, 1, 11) == 0);
    assert(seq_search(a, 1, 22) == -1);

    /* 带断言版本结果一致 */
    assert(seq_search_checked(a, 5, 44) == 3);

    printf("all tests passed\n");
    return 0;
}

/*
 * 第 1 章 绪论 · 六类常见内存错误（反面教材，注释标明错在哪）
 *
 * 现场演示（AddressSanitizer 让工具替我说话，Linux / 常见 macOS 环境可用）：
 *   g++ -std=c++17 -Wall -Wextra -fsanitize=address -g \
 *       -o /tmp/mem-errors snippets/ch01/mem-errors.cpp && /tmp/mem-errors
 * 注意：个别新版 macOS 上 ASan 运行时可能启动即挂死（环境兼容问题，非代码问题）。
 *   演示机若中招，去掉 -fsanitize=address 直接运行，逐类取消注释观察
 *   "崩 / 不崩 / 读到旧值" 的现象，教学效果不受影响。
 *
 * slides 引用方式（逐类取用）：
 *   <<< @/snippets/ch01/mem-errors.cpp#leak {lines:true}
 *
 * main 默认只演示「内存泄漏」与「越界访问」两类；
 * 其余四类会让程序当场崩溃，需要时取消注释单独观察。
 */

#include <stdio.h>
#include <stdlib.h>

// #region null
/* ① 忘记判空：分配失败时解引用 NULL，当场崩溃 */
void demo_null(void)
{
    int *p = (int *)malloc((size_t)-1);   /* 故意申请不可能满足的大小 */
    *p = 1;                               /* 没判空 → 解引用 NULL */
    free(p);
}
// #endregion null

// #region leak
/* ② 内存泄漏：分配了却没有释放
 *    真实场景：函数中途 if 提前 return，那条路径忘了 free */
void demo_leak(void)
{
    int *p = (int *)malloc(16 * sizeof(int));
    if (p == NULL) return;
    p[0] = 1;
    /* 忘了 free(p) —— 这块内存直到进程结束都无法回收 */
}
// #endregion leak

// #region doublefree
/* ③ 重复释放：同一块内存 free 两次，未定义行为 */
void demo_double_free(void)
{
    int *p = (int *)malloc(sizeof(int));
    if (p == NULL) return;
    free(p);
    free(p);           /* 第二次 free 是错误；free 后应紧跟 p = NULL */
}
// #endregion doublefree

// #region dangling
/* ④ 悬空指针：free 之后继续用 —— 对象已死，名字还在 */
void demo_dangling(void)
{
    int *p = (int *)malloc(sizeof(int));
    if (p == NULL) return;
    *p = 42;
    free(p);
    printf("%d\n", *p);   /* 访问已释放内存，最阴险的一类：往往还能读到旧值 */
}
// #endregion dangling

// #region oob
/* ⑤ 越界访问：下标超出申请范围 */
void demo_oob(void)
{
    int *a = (int *)malloc(4 * sizeof(int));
    if (a == NULL) return;
    for (int i = 0; i <= 4; i++)   /* i == 4 时越界！ */
        a[i] = i;
    free(a);
}
// #endregion oob

// #region uninit
/* ⑥ 使用未初始化内存：malloc 不清零，读到的值是垃圾 */
void demo_uninit(void)
{
    int *p = (int *)malloc(sizeof(int));   /* 内容是随机垃圾值，不是 0 */
    if (p == NULL) return;
    printf("%d\n", *p);   /* 结果不确定，时对时错 */
    free(p);
}
// #endregion uninit

int main(void)
{
    demo_leak();     /* 程序退出时 ASan 报告泄漏的字节数 */
    demo_oob();      /* ASan 报告越界访问的位置（报告后程序终止） */

    /* 其余四类，取消注释单独观察：
         demo_null();         解引用 NULL
         demo_double_free();  重复释放
         demo_dangling();     use-after-free
         demo_uninit();       读未初始化内存 */
    return 0;
}

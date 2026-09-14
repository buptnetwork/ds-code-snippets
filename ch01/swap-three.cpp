/*
 * 第 1 章 绪论 · swap 三版本：值 / 指针 / 引用（现场编译运行）
 *
 * 运行方式：
 *   g++ -std=c++17 -Wall -Wextra -o /tmp/swap-three snippets/ch01/swap-three.cpp && /tmp/swap-three
 * slides 引用方式：
 *   <<< @/snippets/ch01/swap-three.cpp#fns {lines:true}
 *   <<< @/snippets/ch01/swap-three.cpp#main {lines:true}
 */

#include <stdio.h>

// #region fns
/* ① 值传递：形参是实参的副本，函数内交换对调用方无效 */
void swap_val(int a, int b) { int t = a; a = b; b = t; }

/* ② 指针传递：能改实参；调用方要写 &x；函数内应判空 */
void swap_ptr(int *a, int *b)
{
    if (!a || !b) return;
    int t = *a; *a = *b; *b = t;
}

/* ③ 引用传递：能改实参；调用方写 x；函数内不必判空 */
void swap_ref(int &a, int &b) { int t = a; a = b; b = t; }
// #endregion fns

// #region main
int main(void)
{
    int x = 1, y = 2;
    swap_val(x, y);    printf("swap_val: x=%d y=%d   <- 没换！\n", x, y);
    swap_ptr(&x, &y);  printf("swap_ptr: x=%d y=%d\n", x, y);
    swap_ref(x, y);    printf("swap_ref: x=%d y=%d\n", x, y);
    return 0;
}
// #endregion main

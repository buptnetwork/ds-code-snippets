/*
 * 第 1 章 绪论 · 复数 ADT 路线一 · 调用示例
 *
 * slides 引用方式：
 *   <<< @/snippets/ch01/complex-opaque/main.cpp#use {lines:true}
 */

#include "complex.h"

#include <stdio.h>

// #region use
int main(void)
{
    Complex *c1 = complex_create(1.0, 2.0);   /* 对象在堆上 */
    Complex *c2 = complex_create(3.0, -1.0);
    if (!c1 || !c2) return 1;                 /* create 可能失败，必须查 */

    Complex *s = complex_add(c1, c2);         /* 4 + i */
    if (!s) { complex_destroy(c1); complex_destroy(c2); return 1; }

    printf("(1+2i) + (3-i) = %.1f + %.1fi\n", complex_real(s), complex_imag(s));

    complex_destroy(s);                       /* 有 create 必有 destroy */
    complex_destroy(c2);
    complex_destroy(c1);
    return 0;
}
// #endregion use

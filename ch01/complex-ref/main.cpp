/*
 * 第 1 章 绪论 · 复数 ADT 路线二 · 调用示例
 *
 * slides 引用方式：
 *   <<< @/snippets/ch01/complex-ref/main.cpp#use {lines:true}
 */

#include "complex.h"

#include <stdio.h>

// #region use
int main(void)
{
    Complex c1, c2;                  /* 对象在栈上 */
    complex_init(c1, 1.0, 2.0);      /* 对应 InitComplex */
    complex_init(c2, 3.0, -1.0);

    Complex s = complex_add(c1, c2); /* 不需要 destroy */
    printf("%.1f + %.1fi\n",
           complex_real(s), complex_imag(s));
    return 0;
}
// #endregion use

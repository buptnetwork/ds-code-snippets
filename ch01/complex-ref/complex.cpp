/*
 * 第 1 章 绪论 · 复数 ADT 路线二 · 实现
 *
 * slides 引用方式：
 *   <<< @/snippets/ch01/complex-ref/complex.cpp#impl {lines:true}
 */

#include "complex.h"

// #region impl
void complex_init(Complex &z, double re, double im)
{
    z.re = re;                     /* z 是要被改写的对象本身，不是副本 */
    z.im = im;
}

Complex complex_add(const Complex &a, const Complex &b)
{
    Complex r;
    complex_init(r, a.re + b.re, a.im + b.im);
    return r;                      /* 返回对象本身；对象归调用方（栈上） */
}
// #endregion impl

double complex_real(const Complex &z) { return z.re; }
double complex_imag(const Complex &z) { return z.im; }

Complex complex_mul(const Complex &a, const Complex &b)
{
    Complex r;
    complex_init(r, a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
    return r;
}

bool complex_equal(const Complex &a, const Complex &b)
{
    return a.re == b.re && a.im == b.im;
}

/*
 * 第 1 章 绪论 · 复数 ADT 路线一 · 实现
 * struct 定义藏在本文件里 —— 用户拿到的只是指针，封装是彻底的。
 *
 * slides 引用方式：
 *   <<< @/snippets/ch01/complex-opaque/complex.cpp#impl {lines:true}
 */

#include "complex.h"

#include <stdlib.h>

// #region impl
struct Complex { double re, im; };   /* 用户看不见 */

Complex *complex_create(double re, double im)
{
    Complex *z = (Complex *)malloc(sizeof *z);
    if (z) { z->re = re; z->im = im; }   /* NULL 必须判 */
    return z;
}

void complex_destroy(Complex *z) { free(z); }
// #endregion impl

double complex_real(const Complex *z) { return z->re; }
double complex_imag(const Complex *z) { return z->im; }

Complex *complex_add(const Complex *a, const Complex *b)
{
    return complex_create(a->re + b->re, a->im + b->im);
}

Complex *complex_mul(const Complex *a, const Complex *b)
{
    return complex_create(a->re * b->re - a->im * b->im,
                          a->re * b->im + a->im * b->re);
}

int complex_equal(const Complex *a, const Complex *b)
{
    return a->re == b->re && a->im == b->im;
}

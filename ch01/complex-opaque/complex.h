/*
 * 第 1 章 绪论 · 复数 ADT 路线一（不透明类型 + 指针形参）· 接口
 *
 * slides 引用方式：
 *   <<< @/snippets/ch01/complex-opaque/complex.h#api cpp {lines:true}
 * 编译自测（与 complex.cpp、main.cpp 一起）：
 *   g++ -std=c++17 -Wall -Wextra snippets/ch01/complex-opaque/complex.cpp \
 *       snippets/ch01/complex-opaque/main.cpp -o /tmp/complex-opaque && /tmp/complex-opaque
 */

#ifndef COMPLEX_H
#define COMPLEX_H

// #region api
typedef struct Complex Complex;      /* 不透明类型 */

Complex *complex_create(double re, double im);
double   complex_real(const Complex *z);
double   complex_imag(const Complex *z);
Complex *complex_add(const Complex *a, const Complex *b);
Complex *complex_mul(const Complex *a, const Complex *b);
int      complex_equal(const Complex *a, const Complex *b);
void     complex_destroy(Complex *z);
// #endregion api

#endif

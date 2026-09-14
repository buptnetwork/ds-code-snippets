/*
 * 第 1 章 绪论 · 复数 ADT 路线二（类型公开 + 引用形参）· 接口
 *
 * slides 引用方式：
 *   <<< @/snippets/ch01/complex-ref/complex.h#api cpp {lines:true}
 * 编译自测（与 complex.cpp、main.cpp 一起）：
 *   g++ -std=c++17 -Wall -Wextra snippets/ch01/complex-ref/complex.cpp \
 *       snippets/ch01/complex-ref/main.cpp -o /tmp/complex-ref && /tmp/complex-ref
 */

#ifndef COMPLEX_H
#define COMPLEX_H

// #region api
struct Complex { double re, im; };   /* 调用方可见 */

void    complex_init(Complex &z, double re, double im);
double  complex_real(const Complex &z);   /* 只读 */
double  complex_imag(const Complex &z);
Complex complex_add(const Complex &a, const Complex &b);
Complex complex_mul(const Complex &a, const Complex &b);
bool    complex_equal(const Complex &a, const Complex &b);
// #endregion api

#endif

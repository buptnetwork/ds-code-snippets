#ifndef CH03_EXPR_H
#define CH03_EXPR_H
#include <cstddef>

// 表达式处理：后缀求值、中缀转后缀、双栈一趟求值。
// 输入约定：记号以空白分隔；操作数为非负整数字面量，按 double 求值。
// 失败时返回 false，result / out 不变（EvalPostfix / EvalInfix）或 out 内容未定义（InfixToPostfix）。

// #region expr-interface
bool EvalPostfix(const char *post, double &result);
bool InfixToPostfix(const char *infix, char *out, size_t outcap);
bool EvalInfix(const char *infix, double &result);
// #endregion expr-interface

#endif

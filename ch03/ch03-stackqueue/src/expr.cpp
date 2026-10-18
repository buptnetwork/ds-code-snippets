// expr.cpp：表达式求值与中缀转后缀实现。
// 语言子集 = C 核心 + 引用形参 / 常量引用；不引入类、模板、STL 容器、异常。
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <cstddef>
#include "stack_double.h"
#include "stack_char.h"
#include "expr.h"

// 运算符优先级；'(' 在栈内视为最低（由调用处单独判断）。
static int prec(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return -1;
    }
}

static bool is_op(char c) {
    return c=='+' || c=='-' || c=='*' || c=='/' || c=='^';
}

// 结合性：'^' 右结合，其余（+ - * /）左结合。
// 决定规则 ④ 弹出时用 > 还是 >=（见 infix-rule4）。
static bool right_assoc(char c) { return c == '^'; }

// 向 out 追加一个记号（前面加空格，末尾补 '\0'）。
static bool out_tok(char *out, size_t outcap, size_t &oi, const char *tok, size_t len) {
    size_t need = (oi > 0 ? 1u : 0u) + len + 1u;
    if (oi + need > outcap) return false;
    if (oi > 0) out[oi++] = ' ';
    memcpy(out + oi, tok, len);
    oi += len;
    out[oi] = '\0';
    return true;
}

static bool out_ch(char *out, size_t outcap, size_t &oi, char c) {
    return out_tok(out, outcap, oi, &c, 1u);
}

// #region eval-postfix
// 后缀表达式求值。
// 记号以空白分隔；操作数为非负整数字面量，按 double 求值。
// 不接收负数或小数字面量（运算结果可以为负数或小数）。
// 失败返回 false，result 不变；超出有限浮点范围也拒绝。
bool EvalPostfix(const char *post, double &result) {
    if (post == NULL) return false;
    StackD S = {};
    if (!SD_Init(S, 16)) return false;
    bool ok = true;
    for (size_t i = 0; post[i] && ok; ++i) {
        char c = post[i];
        if (isspace((unsigned char)c)) continue;
        if (isdigit((unsigned char)c)) {
            double v = 0;
            while (isdigit((unsigned char)post[i])) {
                v = v * 10 + (post[i] - '0');
                ++i;
            }
            if (post[i] && !isspace((unsigned char)post[i])) { ok = false; break; }
            --i;                                        // 外层 ++i 回到分隔符
            ok = std::isfinite(v) && SD_Push(S, v);
        } else {
            if (!is_op(c) || c == '^') { ok = false; break; }   // 求值只支持 + - * /
            if (post[i + 1] && !isspace((unsigned char)post[i + 1])) { ok = false; break; }
            if (SD_Length(S) < 2) { ok = false; break; }
            double a, b, v = 0;
            if (!SD_Pop(S, b) || !SD_Pop(S, a)) { ok = false; break; }
            if (c == '/' && b == 0) { ok = false; break; }
            switch (c) {
                case '+': v = a + b; break;
                case '-': v = a - b; break;     // 左操作数在前
                case '*': v = a * b; break;
                case '/': v = a / b; break;
                default: break;
            }
            ok = std::isfinite(v) && SD_Push(S, v);
        }
    }
    if (ok && SD_Length(S) == 1) ok = SD_Pop(S, result);
    else ok = false;
    SD_Destroy(S);
    return ok;
}
// #endregion eval-postfix

// #region infix-to-postfix
// 中缀转后缀（算符优先法）。
// 输入：记号以空白分隔的中缀表达式字符串。
// 输出：记号以空白分隔的后缀表达式，写入 out（容量 outcap，含末尾 '\0'）。
// 括号不匹配、非法记号、缓冲区不足均返回 false。
bool InfixToPostfix(const char *infix, char *out, size_t outcap) {
    if (!infix || !out || outcap < 2) return false;
    out[0] = '\0';
    StackC OP = {};
    if (!SC_Init(OP, 32)) return false;

    size_t oi = 0;
    bool ok = true;

    for (size_t i = 0; infix[i] && ok; ) {
        char c = infix[i];
        if (isspace((unsigned char)c)) { ++i; continue; }

        if (isdigit((unsigned char)c)) {
            size_t start = i;
            while (isdigit((unsigned char)infix[i])) ++i;
            if (infix[i] && !isspace((unsigned char)infix[i])) { ok = false; break; }
            ok = out_tok(out, outcap, oi, infix + start, i - start);
        } else if (c == '(') {
            ok = SC_Push(OP, c); ++i;
        } else if (c == ')') {
            while (ok && !SC_Empty(OP) && SC_Top(OP) != '(') {
                char t; SC_Pop(OP, t);
                ok = out_ch(out, outcap, oi, t);
            }
            if (!ok) break;
            if (SC_Empty(OP)) { ok = false; break; }    // 右括号多余
            char t; SC_Pop(OP, t);                      // 弹出 '('，不输出
            ++i;
        } else if (is_op(c)) {
            if (infix[i + 1] && !isspace((unsigned char)infix[i + 1])) { ok = false; break; }
            // #region infix-rule4
            // 规则 ④：弹出所有"应先于当前运算符执行"的栈顶（'(' 除外），再压入当前运算符。
            // 左结合（+ - * /）：同级也要先走，用 >=；
            // 右结合（^）：同级要留在栈里后走，用 >。
            // 例：8-3-2 用 >= 得 3（左结合）；2^3^2 用 > 得 512（右结合）。
            while (ok && !SC_Empty(OP) && SC_Top(OP) != '(' &&
                   (right_assoc(c) ? prec(SC_Top(OP)) >  prec(c)
                                   : prec(SC_Top(OP)) >= prec(c))) {
                char t; SC_Pop(OP, t);
                ok = out_ch(out, outcap, oi, t);
            }
            // #endregion infix-rule4
            if (ok) ok = SC_Push(OP, c);
            ++i;
        } else {
            ok = false; break;                          // 非法记号
        }
    }

    // 规则 ⑤：弹出剩余运算符
    while (ok && !SC_Empty(OP)) {
        char t; SC_Pop(OP, t);
        if (t == '(') { ok = false; break; }            // 左括号多余
        ok = out_ch(out, outcap, oi, t);
    }

    SC_Destroy(OP);
    return ok;
}
// #endregion infix-to-postfix

// #region eval-infix
// 双栈一趟求值中缀式（算符优先法）。
// 与 InfixToPostfix 规则逐条对应，唯一差别：输出运算符时立刻用它算一次。
// 失败返回 false，result 不变。
bool EvalInfix(const char *infix, double &result) {
    if (!infix) return false;
    StackD OPND = {};
    StackC OPTR = {};
    if (!SD_Init(OPND, 16) || !SC_Init(OPTR, 32)) {
        SD_Destroy(OPND); SC_Destroy(OPTR); return false;
    }
    bool ok = true;

    // reduce：弹出运算符和两个操作数，计算后压回 OPND。
    auto reduce = [&]() -> bool {
        char op; double a, b, v = 0;
        if (!SC_Pop(OPTR, op)) return false;
        if (!SD_Pop(OPND, b) || !SD_Pop(OPND, a)) return false;
        if (op == '/' && b == 0) return false;
        switch (op) {
            case '+': v = a + b; break;
            case '-': v = a - b; break;
            case '*': v = a * b; break;
            case '/': v = a / b; break;
            case '^': v = pow(a, b); break;
            default: return false;
        }
        return std::isfinite(v) && SD_Push(OPND, v);
    };

    for (size_t i = 0; infix[i] && ok; ) {
        char c = infix[i];
        if (isspace((unsigned char)c)) { ++i; continue; }

        if (isdigit((unsigned char)c)) {
            double v = 0;
            while (isdigit((unsigned char)infix[i])) { v = v * 10 + (infix[i] - '0'); ++i; }
            if (infix[i] && !isspace((unsigned char)infix[i])) { ok = false; break; }
            ok = std::isfinite(v) && SD_Push(OPND, v);
        } else if (c == '(') {
            ok = SC_Push(OPTR, c); ++i;
        } else if (c == ')') {
            while (ok && !SC_Empty(OPTR) && SC_Top(OPTR) != '(') ok = reduce();
            if (!ok) break;
            if (SC_Empty(OPTR)) { ok = false; break; }
            char t; SC_Pop(OPTR, t); ++i;
        } else if (is_op(c)) {
            if (infix[i + 1] && !isspace((unsigned char)infix[i + 1])) { ok = false; break; }
            while (ok && !SC_Empty(OPTR) && SC_Top(OPTR) != '(' &&
                   (right_assoc(c) ? prec(SC_Top(OPTR)) >  prec(c)
                                   : prec(SC_Top(OPTR)) >= prec(c)))
                ok = reduce();
            if (ok) ok = SC_Push(OPTR, c);
            ++i;
        } else {
            ok = false; break;
        }
    }

    while (ok && !SC_Empty(OPTR)) {
        if (SC_Top(OPTR) == '(') { ok = false; break; }
        ok = reduce();
    }

    if (ok && SD_Length(OPND) == 1) {
        double v; ok = SD_Pop(OPND, v);
        if (ok) result = v;
    } else {
        ok = false;
    }

    SD_Destroy(OPND); SC_Destroy(OPTR);
    return ok;
}
// #endregion eval-infix

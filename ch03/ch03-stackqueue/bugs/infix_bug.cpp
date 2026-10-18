// infix_bug.cpp：预期错误——规则 ④ 把 >= 写成 >，导致左结合变右结合。
// 用 "8 - 3 - 2" 演示：正确得 3（左结合），写反得 7（右结合）。
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cmath>
#include <cstddef>
#include "../src/stack_double.h"
#include "../src/stack_char.h"

static int prec(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return -1;
    }
}
static bool is_op(char c) { return c=='+'||c=='-'||c=='*'||c=='/'||c=='^'; }

static bool out_ch(char *out, size_t cap, size_t &oi, char c) {
    if (oi + 2 > cap) return false;
    if (oi > 0) out[oi++] = ' ';
    out[oi++] = c; out[oi] = '\0';
    return true;
}
static bool out_tok(char *out, size_t cap, size_t &oi, const char *s, size_t len) {
    size_t need = (oi > 0 ? 1u : 0u) + len + 1u;
    if (oi + need > cap) return false;
    if (oi > 0) out[oi++] = ' ';
    memcpy(out + oi, s, len); oi += len; out[oi] = '\0';
    return true;
}

// BUG 版本：规则 ④ 用 > 而不是 >=
static bool InfixToPostfixBuggy(const char *infix, char *out, size_t outcap) {
    if (!infix || !out || outcap < 2) return false;
    out[0] = '\0';
    StackC OP = {};
    if (!SC_Init(OP, 32)) return false;
    size_t oi = 0; bool ok = true;
    for (size_t i = 0; infix[i] && ok; ) {
        char c = infix[i];
        if (isspace((unsigned char)c)) { ++i; continue; }
        if (isdigit((unsigned char)c)) {
            size_t s = i;
            while (isdigit((unsigned char)infix[i])) ++i;
            if (infix[i] && !isspace((unsigned char)infix[i])) { ok = false; break; }
            ok = out_tok(out, outcap, oi, infix + s, i - s);
        } else if (c == '(') {
            ok = SC_Push(OP, c); ++i;
        } else if (c == ')') {
            while (ok && !SC_Empty(OP) && SC_Top(OP) != '(') { char t; SC_Pop(OP, t); ok = out_ch(out, outcap, oi, t); }
            if (!ok) break;
            if (SC_Empty(OP)) { ok = false; break; }
            char t; SC_Pop(OP, t); ++i;
        } else if (is_op(c)) {
            if (infix[i+1] && !isspace((unsigned char)infix[i+1])) { ok = false; break; }
            // BUG：用 > 而不是 >=，同级运算符不弹出，变成右结合
            while (ok && !SC_Empty(OP) && SC_Top(OP) != '(' && prec(SC_Top(OP)) > prec(c)) {
                char t; SC_Pop(OP, t); ok = out_ch(out, outcap, oi, t);
            }
            if (ok) ok = SC_Push(OP, c);
            ++i;
        } else { ok = false; break; }
    }
    while (ok && !SC_Empty(OP)) {
        char t; SC_Pop(OP, t);
        if (t == '(') { ok = false; break; }
        ok = out_ch(out, outcap, oi, t);
    }
    SC_Destroy(OP);
    return ok;
}

// 用正确版本求值（复用 expr.cpp 的 EvalPostfix 逻辑）
static bool EvalPost(const char *post, double &result) {
    if (!post) return false;
    StackD S = {};
    if (!SD_Init(S, 16)) return false;
    bool ok = true;
    for (size_t i = 0; post[i] && ok; ++i) {
        char c = post[i];
        if (isspace((unsigned char)c)) continue;
        if (isdigit((unsigned char)c)) {
            double v = 0;
            while (isdigit((unsigned char)post[i])) { v = v*10+(post[i]-'0'); ++i; }
            if (post[i] && !isspace((unsigned char)post[i])) { ok=false; break; }
            --i; ok = std::isfinite(v) && SD_Push(S, v);
        } else {
            if (!is_op(c)||c=='^') { ok=false; break; }
            if (post[i+1]&&!isspace((unsigned char)post[i+1])) { ok=false; break; }
            if (SD_Length(S)<2) { ok=false; break; }
            double a,b,v=0;
            if (!SD_Pop(S,b)||!SD_Pop(S,a)) { ok=false; break; }
            if (c=='/'&&b==0) { ok=false; break; }
            switch(c){case'+':v=a+b;break;case'-':v=a-b;break;case'*':v=a*b;break;case'/':v=a/b;break;default:break;}
            ok = std::isfinite(v)&&SD_Push(S,v);
        }
    }
    if (ok&&SD_Length(S)==1) ok=SD_Pop(S,result); else ok=false;
    SD_Destroy(S); return ok;
}

int main() {
    puts("=== 预期错误：规则 ④ 用 > 而不是 >= ===");
    const char *tests[] = {"8 - 3 - 2", "2 ^ 3 ^ 2", "1 + 2 * ( 7 - 4 ) - 6 / 3"};
    for (const char *infix : tests) {
        char post[256];
        bool ok = InfixToPostfixBuggy(infix, post, sizeof post);
        printf("  infix : %s\n", infix);
        printf("  postfix(buggy): %s\n", ok ? post : "FAILED");
        if (ok) {
            double r = -999;
            if (EvalPost(post, r)) printf("  result: %.4g\n", r);
            else printf("  eval FAILED\n");
        }
        putchar('\n');
    }
    puts("正确结果：8-3-2=3（左结合）；buggy 版得 7（右结合）。");
    puts("2^3^2：buggy 版用 > 反而正确（右结合），但左结合运算符全错。");
    return 0;
}

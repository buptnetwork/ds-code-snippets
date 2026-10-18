// evalpostfix_bug.cpp：预期错误——操作数顺序写反（b/a 而非 a/b）。
// 用 "6 3 /" 演示：正确得 2，写反得 0.5。
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cmath>
#include <cstddef>
#include "../src/stack_double.h"

// 故意写反的版本：先弹出的是 a，后弹出的是 b（正确应先弹 b 再弹 a）。
static bool EvalPostfixBuggy(const char *post, double &result) {
    if (!post) return false;
    StackD S = {};
    if (!SD_Init(S, 16)) return false;
    bool ok = true;
    for (size_t i = 0; post[i] && ok; ++i) {
        char c = post[i];
        if (isspace((unsigned char)c)) continue;
        if (isdigit((unsigned char)c)) {
            double v = 0;
            while (isdigit((unsigned char)post[i])) { v = v * 10 + (post[i] - '0'); ++i; }
            if (post[i] && !isspace((unsigned char)post[i])) { ok = false; break; }
            --i;
            ok = std::isfinite(v) && SD_Push(S, v);
        } else {
            if (c != '+' && c != '-' && c != '*' && c != '/') { ok = false; break; }
            if (post[i+1] && !isspace((unsigned char)post[i+1])) { ok = false; break; }
            if (SD_Length(S) < 2) { ok = false; break; }
            double a, b, v = 0;
            // BUG：顺序写反——先弹出的是左操作数，后弹出的是右操作数
            if (!SD_Pop(S, a) || !SD_Pop(S, b)) { ok = false; break; }
            if (c == '/' && b == 0) { ok = false; break; }
            switch (c) {
                case '+': v = a + b; break;
                case '-': v = a - b; break;
                case '*': v = a * b; break;
                case '/': v = a / b; break;   // 实际计算 b/a（写反）
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

int main() {
    puts("=== 预期错误：操作数顺序写反 ===");
    const char *tests[] = {"6 3 /", "6 3 -", "1 2 7 4 - * + 6 3 / -"};
    for (const char *s : tests) {
        double r = -999;
        bool ok = EvalPostfixBuggy(s, r);
        printf("  \"%s\" -> %s  result=%.4g\n", s, ok ? "OK" : "FAIL", r);
    }
    puts("\n正确结果：6 3 / = 2；写反后 = 0.5。");
    puts("减法同理：6 3 - 正确 = 3；写反 = -3。");
    return 0;
}

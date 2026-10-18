// expr_demo.cpp：表达式求值演示。
// 主表达式：1 + 2 * ( 7 - 4 ) - 6 / 3，后缀：1 2 7 4 - * + 6 3 / -，结果 5。
#include <cstdio>
#include <cstring>
#include "../src/expr.h"

static void show_postfix(const char *infix) {
    char post[256];
    if (InfixToPostfix(infix, post, sizeof post))
        printf("  infix : %s\n  postfix: %s\n", infix, post);
    else
        printf("  infix : %s\n  ERROR: InfixToPostfix failed\n", infix);
}

static void eval_post(const char *post) {
    double r = -999;
    if (EvalPostfix(post, r))
        printf("  EvalPostfix(\"%s\") = %.4g\n", post, r);
    else
        printf("  EvalPostfix(\"%s\") FAILED\n", post);
}

static void eval_inf(const char *infix) {
    double r = -999;
    if (EvalInfix(infix, r))
        printf("  EvalInfix (\"%s\") = %.4g\n", infix, r);
    else
        printf("  EvalInfix (\"%s\") FAILED\n", infix);
}

int main() {
    const char *MAIN_INFIX  = "1 + 2 * ( 7 - 4 ) - 6 / 3";
    const char *MAIN_POST   = "1 2 7 4 - * + 6 3 / -";

    puts("=== 主表达式 ===");
    show_postfix(MAIN_INFIX);
    eval_post(MAIN_POST);
    eval_inf(MAIN_INFIX);

    puts("\n=== 结合性验证：8 - 3 - 2（应得 3）===");
    char post2[64];
    InfixToPostfix("8 - 3 - 2", post2, sizeof post2);
    printf("  postfix: %s\n", post2);
    eval_post(post2);
    eval_inf("8 - 3 - 2");

    puts("\n=== 右结合验证：2 ^ 3 ^ 2（应得 512）===");
    char post3[64];
    InfixToPostfix("2 ^ 3 ^ 2", post3, sizeof post3);
    printf("  postfix: %s\n", post3);
    eval_inf("2 ^ 3 ^ 2");

    puts("\n=== 非法输入（应全部 FAILED）===");
    const char *bad[] = {"1 +", "1 2 3 +", "", "1 0 /", "1 2 ?", "( 1 + 2", "1 + 2 )"};
    for (const char *s : bad) {
        double r = -999;
        bool ok = EvalPostfix(s, r);
        printf("  EvalPostfix(\"%s\") -> %s  result=%.4g\n", s, ok ? "OK" : "FAIL", r);
    }

    puts("\n=== 操作数顺序陷阱：6 3 / 应得 2，写反得 0.5 ===");
    eval_post("6 3 /");
    // 模拟写反：b / a
    printf("  若写反（b/a）：3 / 6 = %.4g\n", 3.0 / 6.0);

    return 0;
}

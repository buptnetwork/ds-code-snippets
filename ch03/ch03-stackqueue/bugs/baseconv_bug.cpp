// baseconv_bug.cpp：预期错误示例（单元 8 反例）—— 缺少 N==0 的边界分支。
// 后果：Conversion(0, 8) 时 while(N>0) 一次都不进，栈为空，输出一个空行而不是 "0"。
// 本文件故意保留缺陷，用于课堂对照；正确实现见 demo/baseconv.cpp。
// 编译：c++ -std=c++17 -Wall -Wextra baseconv_bug.cpp ../src/seqstack.cpp -o baseconv_bug
#include <cstdio>
#include "../src/seqstack.h"

static char digit(int x) { return (char)(x < 10 ? ('0' + x) : ('A' + x - 10)); }

// #region buggy
void ConversionBuggy(int N, int base) {
    // 缺陷：这里少了 if (N == 0) { putchar('0'); putchar('\n'); return; }
    SeqStack S = {};
    if (!SS_Init(S, 8)) { fprintf(stderr, "分配失败\n"); return; }
    while (N > 0) {                       // N==0 时循环体一次都不执行
        if (!SS_Push(S, N % base)) { SS_Destroy(S); return; }
        N /= base;
    }
    ElemType x;
    while (SS_Pop(S, x)) putchar(digit(x));   // 栈空 → 什么都不输出
    putchar('\n');                            // 只留下一个空行
    SS_Destroy(S);
}
// #endregion buggy

int main() {
    printf("N=0 -> 8 : ["); ConversionBuggy(0, 8); printf("]  <-- 应为 0，实为空行\n");
    printf("N=1348 -> 8 : ["); ConversionBuggy(1348, 8); printf("]\n");
    return 0;
}

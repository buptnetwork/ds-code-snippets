// baseconv.cpp：应用二 —— 进制转换（除基取余法）。
// 把十进制非负整数 N 转换为 d 进制（2 <= d <= 16）。产生顺序（低位先）与书写顺序（高位先）相反，故用栈反转。
// 编译：c++ -std=c++17 -Wall -Wextra baseconv.cpp ../src/seqstack.cpp -o baseconv
#include <cstdio>
#include "../src/seqstack.h"

static char digit(int x) { return (char)(x < 10 ? ('0' + x) : ('A' + x - 10)); }

// #region conversion
void Conversion(int N, int base) {                 // 前置：N>=0，2<=base<=16
    if (N == 0) { putchar('0'); putchar('\n'); return; }   // 边界：0 不进循环
    SeqStack S = {};
    if (!SS_Init(S, 8)) { fprintf(stderr, "分配失败\n"); return; }
    while (N > 0) {
        if (!SS_Push(S, N % base)) { SS_Destroy(S); return; }   // 余数入栈（低位先）
        N /= base;
    }
    ElemType x;
    while (SS_Pop(S, x)) putchar(digit(x));        // 下溢作终止；高位先输出
    putchar('\n'); SS_Destroy(S);
}
// #endregion conversion

// 不用显式栈的递归版本：先递归处理高位，回溯时输出低位。
// 顺序反转照样发生 —— 因为函数调用本身就用了一个栈（第 2 次课 3.3 展开）。
// #region recursive
void Conv2(int N, int base) {
    if (N == 0) return;
    Conv2(N / base, base);          // 先递归（高位）
    putchar(digit(N % base));       // 后输出（低位）
}

void ConversionRecursive(int N, int base) {
    if (N == 0) putchar('0');
    else Conv2(N, base);
    putchar('\n');
}
// #endregion recursive

int main() {
    printf("1348 -> 8 : "); Conversion(1348, 8);
    printf("0    -> 8 : "); Conversion(0, 8);
    printf("7    -> 8 : "); Conversion(7, 8);
    printf("8    -> 8 : "); Conversion(8, 8);
    printf("255  -> 16: "); Conversion(255, 16);
    printf("1348 -> 2 : "); Conversion(1348, 2);
    printf("\n-- 递归版本对照 --\n");
    printf("1348 -> 8 : "); ConversionRecursive(1348, 8);
    printf("0    -> 8 : "); ConversionRecursive(0, 8);
    printf("255  -> 16: "); ConversionRecursive(255, 16);
    return 0;
}

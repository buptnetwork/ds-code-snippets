// hanoi.cpp：汉诺塔递归演示。
// Hanoi(3, 'A', 'B', 'C') 输出 7 步；移动次数 2^n - 1，递归深度 n。
#include <cstdio>

// #region hanoi
// 把 n 个盘从 A 经 B 移到 C。
// 递归调用后面还有事要做（打印 + 第二次递归），属于"情况二"——显式栈改写明显更麻烦。
void Hanoi(int n, char A, char B, char C) {
    if (n == 0) return;
    Hanoi(n - 1, A, C, B);          // 先把上面 n-1 个从 A 移到 B
    printf("%c -> %c\n", A, C);     // 移动最大盘
    Hanoi(n - 1, B, A, C);          // 再把 n-1 个从 B 移到 C
}
// #endregion hanoi

int main() {
    puts("=== Hanoi(3, A, B, C)：7 步 ===");
    Hanoi(3, 'A', 'B', 'C');
    puts("\n移动次数 = 2^3 - 1 = 7；递归深度 = 3。");
    puts("调用次数是指数级，但最大同时存活深度只有 n。");
    return 0;
}

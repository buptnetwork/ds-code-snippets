// recursion_demo.cpp：递归调用栈帧追踪演示。
// 同时演示两个递归例子：阶乘（热身，先接受递归写法）与进制转换（本次课主线）。
// 在函数入口和出口各打印一行，缩进量 = 递归深度。
// 输出本身就把调用栈画出来了——这是调试递归最实用的手段。
#include <cstdio>

static char digit(int x) { return x < 10 ? (char)('0' + x) : (char)('A' + x - 10); }

static int depth = 0;
static void indent() { for (int i = 0; i < depth; ++i) fputs("  ", stdout); }

// #region fact
int Fact(int n) {                 // 前置：0 ≤ n ≤ 12（13! 超出 int）
    if (n <= 1) return 1;         // 出口：0! = 1! = 1
    return n * Fact(n - 1);       // 同形小问题：(n−1)!
}
// #endregion fact

// #region fact-trace
// 追踪版：与上面 Fact 逻辑相同，只多缩进日志，为课件提供真实输出。
int FactTrace(int n) {
    if (n <= 1) {
        indent(); printf("enter Fact(%d) -> return 1（递归出口）\n", n);
        return 1;
    }
    indent(); printf("enter Fact(%d)\n", n);
    ++depth;
    int sub = FactTrace(n - 1);
    --depth;
    indent(); printf("return %d * %d = %d, leave\n", n, sub, n * sub);
    return n * sub;
}
// #endregion fact-trace

// #region conv2-trace
// 递归核心：先递归处理高位，返回时输出当前余数。
// 每层调用对应一个栈帧，N 在各帧中互不干扰。
void Conv2(int N, int base) {
    if (N == 0) {
        indent(); printf("enter Conv2(N=0) -> return（递归出口）\n");
        return;
    }
    indent(); printf("enter Conv2(N=%d)\n", N);
    ++depth;
    Conv2(N / base, base);      // ① 先递归（求高位）
    --depth;
    indent(); printf("output %c, leave（N=%d, N%%%d=%d）\n",
                     digit(N % base), N, base, N % base);
}
// #endregion conv2-trace

// 包装函数：统一处理零输入和末尾换行。
void ConversionRecursive(int N, int base) {
    if (N == 0) putchar('0');
    else Conv2(N, base);
    putchar('\n');
}

int main() {
    puts("=== Fact(4) 调用栈追踪 ===");
    puts("缩进量 = 递归深度；enter 对应压栈，leave 对应弹栈。");
    depth = 0;
    FactTrace(4);
    printf("Fact(4) = %d\n", Fact(4));

    puts("\n=== 阶乘边界用例 ===");
    printf("Fact(0) = %d, Fact(1) = %d, Fact(12) = %d\n", Fact(0), Fact(1), Fact(12));
    // 13! = 6227020800 超出 int；不用阶乘的大 n 测栈深度。

    puts("\n=== Conv2(1348, 8) 调用栈追踪 ===");
    puts("缩进量 = 递归深度；enter 对应压栈，leave 对应弹栈。");
    depth = 0;
    ConversionRecursive(1348, 8);

    puts("\n=== 边界用例（结果行）===");
    depth = 0; printf("N=0  -> "); ConversionRecursive(0, 8);
    depth = 0; printf("N=7  -> "); ConversionRecursive(7, 8);
    depth = 0; printf("N=8  -> "); ConversionRecursive(8, 8);

    return 0;
}

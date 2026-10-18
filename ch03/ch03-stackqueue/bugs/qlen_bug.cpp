// qlen_bug.cpp：反例 2（单元 4）——循环队列求队长漏加 capacity。
// ★ 预期错误：C 语言对负数取模，结果符号与被除数相同，(rear-front)%cap 在跨边界时得负数。
// 用 front=2, rear=1, cap=5 实跑：错误式得 -1，正确式 (rear-front+cap)%cap 得 4。
#include <cstdio>

static int len_wrong(int front, int rear, int cap) { return (rear - front) % cap; }            // 漏加 capacity
static int len_right(int front, int rear, int cap) { return (rear - front + cap) % cap; }        // 先加保证非负

int main() {
    puts("=== 循环队列求队长：(rear-front)%cap  vs  (rear-front+cap)%cap ===");
    struct Case { int front, rear, cap, expect; };
    Case cs[] = {
        {2, 1, 5, 4},   // 跨越边界：队列为 5 9 2 7（承接单元 4 演示），实际 4 个
        {2, 2, 5, 0},   // front==rear 且 size 方案下为空（牺牲一格方案的空）
        {0, 3, 5, 3},   // 未跨越边界，两式一致
        {4, 1, 5, 2},   // 跨越边界
    };
    for (unsigned i = 0; i < sizeof(cs)/sizeof(cs[0]); ++i) {
        int w = len_wrong(cs[i].front, cs[i].rear, cs[i].cap);
        int r = len_right(cs[i].front, cs[i].rear, cs[i].cap);
        printf("front=%d rear=%d cap=%d | 错误式=%d  正确式=%d  实际=%d  %s\n",
               cs[i].front, cs[i].rear, cs[i].cap, w, r, cs[i].expect,
               (w == cs[i].expect) ? "" : "← 错误式失真");
    }
    printf("\n要点：rear-front 可能为负（如 1-2=-1），-1%%5 在 C 里得 -1 而非 4；\n");
    printf("必须先 +capacity 保证被除数非负，再取模。这是考试高频失分点。\n");
    return 0;
}

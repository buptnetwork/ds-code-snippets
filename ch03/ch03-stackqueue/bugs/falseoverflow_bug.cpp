// falseoverflow_bug.cpp：反例 1（单元 3）——朴素顺序队列的“假溢出”。
// ★ 这是朴素实现的固有缺陷，不是代码写错：rear 撞到数组末端即判满，
//   可前面出队空出的下标 0、1 明明还能用，却够不着。问题在布局，不在容量。
// 用单元 0 统一序列（容量 5）实跑，展示“数组有空位却入队失败”。
#include <cstdio>
#include <cstdlib>

// #region naive-representation
// 最朴素：定长数组 + 两个下标，不取模、不回绕。
struct NaiveQueue {
    int *data;   // 元素数组
    int front;   // 队头下标：出队取这里
    int rear;    // 队尾下一位置：入队写这里
    int capacity; // 容量
};
// #endregion naive-representation

static void NQ_Print(const NaiveQueue &q) {
    printf("front=%d rear=%d arr=[", q.front, q.rear);
    for (int i = 0; i < q.capacity; ++i) {
        bool used = (i >= q.front && i < q.rear);       // 仅 [front,rear) 是有效元素
        if (used) printf("%d", q.data[i]); else printf("_");
        if (i + 1 < q.capacity) printf(", ");
    }
    printf("]  有效元素=%d，空闲格=%d\n", q.rear - q.front, q.capacity - (q.rear - q.front));
}

int main() {
    const int CAP = 5;
    NaiveQueue q; q.data = (int *)malloc(sizeof(int) * CAP); q.front = q.rear = 0; q.capacity = CAP;

    puts("=== 朴素顺序队列：统一序列 En3 En8 En5 De De En9 En2 En7（容量 5）===");
    NQ_Print(q);
    int seq[] = {3, 8, 5, 9, 2, 7};
    // 前三次入队
    for (int i = 0; i < 3; ++i) { q.data[q.rear++] = seq[i]; printf("En %d      ", seq[i]); NQ_Print(q); }
    // 两次出队：front 前移，下标 0、1 被“废弃”
    for (int i = 0; i < 2; ++i) { int x = q.data[q.front++]; printf("De ->%d    ", x); NQ_Print(q); }
    // 继续入队 9 2 7
    for (int i = 3; i < 6; ++i) {
        if (q.rear == q.capacity) { printf("En %d FAIL  ", seq[i]); NQ_Print(q); printf("  ↑ 假溢出：rear==capacity 判满，但下标 0、1 仍空着\n"); }
        else { q.data[q.rear++] = seq[i]; printf("En %d      ", seq[i]); NQ_Print(q); }
    }
    printf("\n结论：队列只有 %d 个元素（容量 %d），却已无法再入队——空间够但用不上。\n", q.rear - q.front, q.capacity);
    free(q.data);
    return 0;
}

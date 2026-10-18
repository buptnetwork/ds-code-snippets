// deque_demo.cpp：双端队列演示（单元 6）——四个核心操作 + 兑现上次课撤销栈的遗留问题。
// 撤销结构：新操作 PushBack；Ctrl+Z 撤销 PopBack；超过上限（最多记 5 步）PopFront 丢弃最旧。
// 纯栈做不到“从底部丢弃最旧”，这正是需要双端队列的地方。
#include <cstdio>
#include "../src/deque.h"

// #region undo
// 最多记住 limit 步的撤销结构：满了先从队头丢最旧，再从队尾压入新操作。
static void UndoPush(Deque &D, int limit, int op) {
    if (DQ_Length(D) == limit) { int old; DQ_PopFront(D, old); printf("  超限，丢弃最旧操作 %d\n", old); }
    DQ_PushBack(D, op);
}
static void UndoPop(Deque &D) {
    int op;
    if (DQ_PopBack(D, op)) printf("  撤销操作 %d\n", op);
    else printf("  无可撤销操作\n");
}
// #endregion undo

static void Show(const char *tag, const Deque &D) {
    printf("%s 长度=%d 内容(旧->新)=[", tag, DQ_Length(D));
    // 从队头到队尾即“最旧->最新”
    Deque tmp = D;                // 演示用浅拷贝只读遍历；正式代码应避免拷贝资源所有者
    int v; bool first = true;
    while (DQ_PopFront(tmp, v)) { printf("%s%d", first ? "" : ", ", v); first = false; }
    printf("]\n");
}

int main() {
    puts("=== 双端队列四个核心操作 ===");
    Deque D = {}; DQ_Init(D, 8);
    DQ_PushBack(D, 2); DQ_PushBack(D, 3);     // 尾插：2 3
    DQ_PushFront(D, 1);                        // 头插：1 2 3
    DQ_PushBack(D, 4);                         // 尾插：1 2 3 4
    Show("PushFront(1) PushBack(2,3,4) 后：", D);
    int x; DQ_PopFront(D, x); printf("PopFront -> %d\n", x);
    DQ_PopBack(D, x);  printf("PopBack  -> %d\n", x);
    Show("各删一端后：", D);
    printf("只用 PushBack+PopBack=栈；只用 PushBack+PopFront=队列。\n");
    DQ_Destroy(D);

    puts("\n=== 撤销结构（最多记 5 步）：兑现上次课遗留问题 ===");
    Deque U = {}; DQ_Init(U, 5);
    for (int op = 1; op <= 8; ++op) { printf("新操作 %d：\n", op); UndoPush(U, 5, op); Show("  ", U); }
    printf("\n连续撤销两次：\n"); UndoPop(U); UndoPop(U); Show("  ", U);
    printf("\n说明：操作 1、2、3 因超限被从队头丢弃；纯栈无法从底部删除，必须用双端队列。\n");
    DQ_Destroy(U);
    return 0;
}

// pushorder_bug.cpp：预期错误示例（单元 5 反例）—— LS_Push 两行赋值顺序写反。
// 正确顺序：p->next = S.top; 然后 S.top = p;
// 写反后：S.top = p 先执行，再 p->next = S.top 就等于 p->next = p，新结点指向自己，
//         原链表全部丢失（泄漏），且任何遍历都会死循环。
// 警告：不要遍历这个链表——会挂起。本文件的 main 只做一次“自环检测”，不遍历，可安全运行。
// 编译：c++ -std=c++17 -Wall -Wextra pushorder_bug.cpp -o pushorder_bug
#include <cstdio>
#include <cstdlib>
#include "../src/linkstack.h"

// #region buggy
bool LS_PushBuggy(LinkStack &S, ElemType e) {
    StackNode *p = (StackNode *)malloc(sizeof(StackNode));
    if (p == NULL) return false;
    p->data = e;
    S.top = p;              // ✗ 先移动了栈顶
    p->next = S.top;        // ✗ 此时 S.top 已是 p，于是 p->next = p（自己指向自己）
    S.size++;
    return true;
}
// #endregion buggy

int main() {
    LinkStack S; S.top = NULL; S.size = 0;   // 手工置空，使本文件不依赖 linkstack.cpp
    LS_PushBuggy(S, 3);
    LS_PushBuggy(S, 8);     // 再压一个：上一个结点已被丢弃，且新结点自环
    // 只做常数步检测，绝不 while 遍历（否则会死循环挂起）
    if (S.top != NULL && S.top->next == S.top)
        printf("检测到自环：S.top->next == S.top，链表已断裂（此前压入的结点全部泄漏）\n");
    // 故意不调用 LS_Destroy：遍历释放同样会死循环。此处仅演示，接受泄漏。
    return 0;
}

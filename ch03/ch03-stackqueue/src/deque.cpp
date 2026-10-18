// deque.cpp：基于循环数组的双端队列实现。语言子集 = C 核心 + 引用形参 / 常量引用。
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include "deque.h"

// #region lifecycle
bool DQ_Init(Deque &D, int cap) {
    D = {};
    if (cap < 1) cap = 1;
    if ((size_t)cap > SIZE_MAX / sizeof(ElemType)) return false;
    D.data = (ElemType *)malloc(sizeof(ElemType) * (size_t)cap);
    if (D.data == NULL) { D.front = D.rear = D.size = D.capacity = 0; return false; }
    D.front = D.rear = D.size = 0;
    D.capacity = cap;
    return true;
}

void DQ_Destroy(Deque &D) {
    free(D.data);
    D.data = NULL;
    D.front = D.rear = D.size = D.capacity = 0;
}

bool DQ_Empty (const Deque &D) { return D.size == 0; }
bool DQ_Full  (const Deque &D) { return D.size == D.capacity; }
int  DQ_Length(const Deque &D) { return D.size; }
// #endregion lifecycle

// #region pushfront
bool DQ_PushFront(Deque &D, const ElemType &e) {
    if (DQ_Full(D)) return false;
    D.front = (D.front - 1 + D.capacity) % D.capacity;  // front 逆时针退一格；+capacity 防负数取模
    D.data[D.front] = e;
    D.size++;
    return true;
}
// #endregion pushfront

// #region pushback
bool DQ_PushBack(Deque &D, const ElemType &e) {
    if (DQ_Full(D)) return false;
    D.data[D.rear] = e;
    D.rear = (D.rear + 1) % D.capacity;                 // 与循环队列入队完全相同
    D.size++;
    return true;
}
// #endregion pushback

// #region popfront
bool DQ_PopFront(Deque &D, ElemType &out) {
    if (DQ_Empty(D)) return false;
    out = D.data[D.front];
    D.front = (D.front + 1) % D.capacity;               // 与循环队列出队完全相同
    D.size--;
    return true;
}
// #endregion popfront

// #region popback
bool DQ_PopBack(Deque &D, ElemType &out) {
    if (DQ_Empty(D)) return false;
    D.rear = (D.rear - 1 + D.capacity) % D.capacity;    // rear 逆时针退一格；+capacity 防负数取模
    out = D.data[D.rear];
    D.size--;
    return true;
}
// #endregion popback

// #region peek
const ElemType &DQ_Front(const Deque &D) {
    if (D.size == 0) { fprintf(stderr, "DQ_Front on empty deque\n"); exit(1); }
    return D.data[D.front];
}
const ElemType &DQ_Back(const Deque &D) {
    if (D.size == 0) { fprintf(stderr, "DQ_Back on empty deque\n"); exit(1); }
    return D.data[(D.rear - 1 + D.capacity) % D.capacity];   // 队尾元素在 rear 的前一格
}
// #endregion peek

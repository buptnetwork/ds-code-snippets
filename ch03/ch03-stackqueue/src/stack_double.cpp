// stack_double.cpp：double 元素顺序栈实现。逻辑与 seqstack.cpp 相同，类型独立。
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstddef>
#include <cstdint>
#include "stack_double.h"

bool SD_Init(StackD &S, int cap0) {
    S = {};
    if (cap0 < 1) cap0 = 1;
    if ((size_t)cap0 > SIZE_MAX / sizeof(double)) return false;
    S.data = (double *)malloc(sizeof(double) * (size_t)cap0);
    if (S.data == NULL) { S.top = S.capacity = 0; return false; }
    S.top = 0;
    S.capacity = cap0;
    return true;
}

void SD_Destroy(StackD &S) {
    free(S.data);
    S.data = NULL;
    S.top = S.capacity = 0;
}

void SD_Clear(StackD &S) { S.top = 0; }

bool SD_Empty(const StackD &S) { return S.top == 0; }
int  SD_Length(const StackD &S) { return S.top; }

bool SD_Push(StackD &S, double e) {
    if (S.capacity < 1) return false;
    if (S.top == S.capacity) {
        if (S.capacity > INT_MAX / 2) return false;
        int newcap = S.capacity * 2;
        if ((size_t)newcap > SIZE_MAX / sizeof(double)) return false;
        double *p = (double *)realloc(S.data, sizeof(double) * (size_t)newcap);
        if (p == NULL) return false;
        S.data = p;
        S.capacity = newcap;
    }
    S.data[S.top++] = e;
    return true;
}

bool SD_Pop(StackD &S, double &out) {
    if (S.top == 0) return false;
    out = S.data[--S.top];
    return true;
}

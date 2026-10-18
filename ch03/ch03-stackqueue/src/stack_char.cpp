// stack_char.cpp：char 元素顺序栈实现。逻辑与 seqstack.cpp 相同，类型独立。
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstddef>
#include <cstdint>
#include "stack_char.h"

bool SC_Init(StackC &S, int cap0) {
    S = {};
    if (cap0 < 1) cap0 = 1;
    if ((size_t)cap0 > SIZE_MAX / sizeof(char)) return false;
    S.data = (char *)malloc(sizeof(char) * (size_t)cap0);
    if (S.data == NULL) { S.top = S.capacity = 0; return false; }
    S.top = 0;
    S.capacity = cap0;
    return true;
}

void SC_Destroy(StackC &S) {
    free(S.data);
    S.data = NULL;
    S.top = S.capacity = 0;
}

void SC_Clear(StackC &S) { S.top = 0; }

bool SC_Empty(const StackC &S) { return S.top == 0; }
int  SC_Length(const StackC &S) { return S.top; }

bool SC_Push(StackC &S, char e) {
    if (S.capacity < 1) return false;
    if (S.top == S.capacity) {
        if (S.capacity > INT_MAX / 2) return false;
        int newcap = S.capacity * 2;
        if ((size_t)newcap > SIZE_MAX / sizeof(char)) return false;
        char *p = (char *)realloc(S.data, sizeof(char) * (size_t)newcap);
        if (p == NULL) return false;
        S.data = p;
        S.capacity = newcap;
    }
    S.data[S.top++] = e;
    return true;
}

bool SC_Pop(StackC &S, char &out) {
    if (S.top == 0) return false;
    out = S.data[--S.top];
    return true;
}

char SC_Top(const StackC &S) {
    if (S.top == 0) return '\0';
    return S.data[S.top - 1];
}

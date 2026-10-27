#include "hstring.h"
#include <climits>
#include <cstdlib>
#include <cstring>

bool SS_Assign(SString &S, const char *cs) {
    if (cs == NULL) return false;
    size_t n = strlen(cs);
    if (n > (size_t)MAXLEN) return false;
    if (n > 0) memcpy(S.ch, cs, n);
    S.length = (int)n;
    return true;
}

// #region lifecycle
bool HS_Init(HString &S) {
    S.ch = NULL;
    S.length = S.capacity = 0;
    return true;
}

void HS_Destroy(HString &S) {
    free(S.ch);
    S.ch = NULL;
    S.length = S.capacity = 0;
}
// #endregion lifecycle

// 故障注入只改变分配结果，不出现在课堂展示的操作代码中。
#ifdef CH04_TEST_ALLOC
static bool fail_next_allocation = false;
void HS_TestFailNextAllocation() { fail_next_allocation = true; }
#endif

static void *resize_buffer(void *ptr, size_t bytes) {
#ifdef CH04_TEST_ALLOC
    if (fail_next_allocation) {
        fail_next_allocation = false;
        return NULL;
    }
#endif
    return realloc(ptr, bytes);
}

// #region ensure
// 前提：S 合法。容量从 8 开始倍增；失败时保留原指针与内容。
static bool ensure(HString &S, int need) {
    if (need < 0) return false;
    if (S.ch != NULL && S.capacity >= need) return true;
    int newcap = S.capacity > 0 ? S.capacity : 8;
    while (newcap < need) {
        if (newcap > INT_MAX / 2) {
            newcap = need;
            break;
        }
        newcap *= 2;
    }
    // resize_buffer 的正常实现为 realloc；测试版可注入失败。
    char *p = (char *)resize_buffer(S.ch, (size_t)newcap + 1);
    if (p == NULL) return false;
    p[S.length] = '\0';
    S.ch = p;
    S.capacity = newcap;
    return true;
}
// #endregion ensure

// #region assign
bool HS_Assign(HString &S, const char *cs) {
    if (cs == NULL) return false;
    size_t size = strlen(cs);
    if (size > (size_t)INT_MAX) return false;
    int n = (int)size;
    if (!ensure(S, n)) return false;
    if (n > 0) memcpy(S.ch, cs, n);
    S.ch[n] = '\0';
    S.length = n;
    return true;
}
// #endregion assign

// #region concat
bool HS_Concat(HString &dst, const HString &S, const HString &T) {
    if (&dst == &S || &dst == &T) return false;
    if (S.length > INT_MAX - T.length) return false;
    int n = S.length + T.length;
    if (!ensure(dst, n)) return false;
    if (S.length > 0) memcpy(dst.ch, S.ch, S.length);
    if (T.length > 0) memcpy(dst.ch + S.length, T.ch, T.length);
    dst.ch[n] = '\0';
    dst.length = n;
    return true;
}
// #endregion concat

// #region compare
int HS_Compare(const HString &S, const HString &T) {
    int n = S.length < T.length ? S.length : T.length;
    for (int i = 0; i < n; ++i)
        if (S.ch[i] != T.ch[i])
            return (unsigned char)S.ch[i] - (unsigned char)T.ch[i];
    return S.length - T.length;
}
// #endregion compare

// #region substring
bool HS_SubString(HString &sub, const HString &S, int pos, int len) {
    if (&sub == &S) return false;
    if (pos < 0 || pos > S.length || len < 0 || len > S.length - pos)
        return false;
    if (!ensure(sub, len)) return false;
    if (len > 0) memcpy(sub.ch, S.ch + pos, len);
    sub.ch[len] = '\0';
    sub.length = len;
    return true;
}
// #endregion substring

#include "match.h"

// nxt[k] 是 T[0..k] 最长相同真前后缀的长度。
// #region build-next
void BuildNext(const char *T, int m, int *nxt) {
    if (m == 0) return;
    nxt[0] = 0;
    int len = 0;
    for (int k = 1; k < m; ++k) {
        while (len > 0 && T[k] != T[len])
            len = nxt[len - 1];
        if (T[k] == T[len]) ++len;
        nxt[k] = len;
    }
}
// #endregion build-next

// #region kmp
int KMP_Index(const char *S, int n, const char *T, int m,
              const int *nxt, long long &cmp) {
    cmp = 0;
    if (m == 0) return 0;
    if (m > n) return -1;
    // #region kmp-loop
    int i = 0, j = 0;
    while (i < n && j < m) {
        ++cmp;
        if (S[i] == T[j]) {
            ++i;
            ++j;
        } else if (j == 0) {
            ++i;
        } else {
            j = nxt[j - 1];
        }
    }
    return j == m ? i - m : -1;
    // #endregion kmp-loop
}
// #endregion kmp

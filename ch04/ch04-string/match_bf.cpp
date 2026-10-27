#include "match.h"

// #region bf
int BF_Index(const char *S, int n, const char *T, int m, long long &cmp) {
    // #region bf-boundaries
    cmp = 0;
    if (m == 0) return 0;
    if (m > n) return -1;
    // #endregion bf-boundaries
    // #region bf-loop
    int i = 0, j = 0;
    while (i < n && j < m && i - j <= n - m) {
        ++cmp;
        if (S[i] == T[j]) {
            ++i;
            ++j;
        } else {
            i = i - j + 1;
            j = 0;
        }
    }
    return j == m ? i - m : -1;
    // #endregion bf-loop
}
// #endregion bf

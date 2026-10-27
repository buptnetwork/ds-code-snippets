#include "match.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// 本演示只使用下方固定短例。32 格工作数组足够，不是算法的通用长度上限。
static void show_case(const char *S, const char *T) {
    const int n = (int)strlen(S);
    const int m = (int)strlen(T);
    if (m > 32) {
        fprintf(stderr, "演示工作数组不足。\n");
        exit(EXIT_FAILURE);
    }
    // #region match-call
    int nxt[32];
    BuildNext(T, m, nxt);
    long long bf_cmp, kmp_cmp;
    int bf = BF_Index(S, n, T, m, bf_cmp);
    int kmp = KMP_Index(S, n, T, m, nxt, kmp_cmp);
    // #endregion match-call
    printf("S=\"%s\" T=\"%s\"\n", S, T);
    printf("nxt=[");
    for (int k = 0; k < m; ++k) printf("%s%d", k ? "," : "", nxt[k]);
    puts("]");
    printf("[BF ] index=%d cmp=%lld\n", bf, bf_cmp);
    printf("[KMP] index=%d cmp=%lld\n", kmp, kmp_cmp);
}

int main(int argc, char **argv) {
    const char *mode = argc == 2 ? argv[1] : "all";
    if (argc > 2 || (strcmp(mode, "all") && strcmp(mode, "main") && strcmp(mode, "repeat"))) {
        fprintf(stderr, "用法：demo_match [all|main|repeat]\n");
        return EXIT_FAILURE;
    }
    if (!strcmp(mode, "all") || !strcmp(mode, "repeat")) show_case("aaaaaaab", "aaab");
    if (!strcmp(mode, "all") || !strcmp(mode, "main")) show_case("abaabaabcac", "abaabcac");
    return EXIT_SUCCESS;
}

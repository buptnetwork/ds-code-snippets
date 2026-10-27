// 第 3 次课教师演示：原文中的非重叠替换，不扫描新生成内容。
// 使用以 '\0' 结尾的输入，最多 256 字节；结果最多 256 字节。
// 固定工作区避免把内存管理变成此课重点，不是通用文本工具。
#include "match.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

static const int LIMIT = 256;
static int checks = 0;
#define CHECK(expr) do { ++checks; if (!(expr)) { \
    std::fprintf(stderr, "检查失败，第 %d 行：%s\n", __LINE__, #expr); \
    std::exit(EXIT_FAILURE); } } while (0)

// 调用者已经确认 0 < m <= LIMIT、0 <= n <= LIMIT；pos 至少 LIMIT 格。
// 每次命中后从哪里继续，决定是否保留重叠匹配。
static int collect(const char *S, int n, const char *T, int m,
                   bool overlap, int *pos) {
    int nxt[LIMIT];
    BuildNext(T, m, nxt);
    int count = 0, j = 0;
    for (int i = 0; i < n; ++i) {
        while (j > 0 && S[i] != T[j]) j = nxt[j - 1];
        if (S[i] == T[j]) ++j;
        if (j == m) {
            pos[count++] = i - m + 1;
            j = overlap ? nxt[m - 1] : 0;
        }
    }
    return count;
}

// out 不与输入共享缓冲区；失败时保持 out 原值。
// 空模式拒绝；空替换串允许，表示删除；所有位置均为字节下标。
static bool replace_all(const char *S, const char *T, const char *V,
                        char (&out)[LIMIT + 1]) {
    if (S == NULL || T == NULL || V == NULL) return false;
    size_t ns = std::strlen(S), ms = std::strlen(T), vs = std::strlen(V);
    if (ns > LIMIT || ms == 0 || ms > LIMIT || vs > LIMIT) return false;
    int n = (int)ns, m = (int)ms, v = (int)vs;
    int pos[LIMIT];
    int k = collect(S, n, T, m, false, pos);
    // 每个量至多 256，long long 足以容纳中间乘加；先检查再写入。
    long long length = n + (long long)k * (v - m);
    if (length < 0 || length > LIMIT) return false;
    char result[LIMIT + 1];
    int read = 0, write = 0;
    // #region replacement-copy
    for (int x = 0; x < k; ++x) {
        int gap = pos[x] - read;
        std::memcpy(result + write, S + read, gap);
        write += gap;
        std::memcpy(result + write, V, v);
        write += v;
        read = pos[x] + m;
    }
    std::memcpy(result + write, S + read, n - read);
    write += n - read;
    result[write] = '\0';
    // #endregion replacement-copy
    std::memcpy(out, result, (size_t)write + 1);
    return true;
}

static void print_positions(const int *pos, int count) {
    std::printf("[");
    for (int x = 0; x < count; ++x)
        std::printf("%s%d", x ? "," : "", pos[x]);
    std::printf("]\n");
}

static void show(const char *S, const char *T, const char *V) {
    char out[LIMIT + 1] = "";
    if (!replace_all(S, T, V, out)) {
        std::fprintf(stderr, "替换失败：空模式或超出演示工作区\n");
        std::exit(EXIT_FAILURE);
    }
    std::printf("\"%s\" : \"%s\" -> \"%s\" => \"%s\"\n", S, T, V, out);
}

// 独立参考：逐起点判断，不使用 KMP 或预先收集的位置。
static std::string reference(const std::string &S, const std::string &T,
                             const std::string &V) {
    std::string out;
    for (size_t i = 0; i < S.size();) {
        if (S.compare(i, T.size(), T) == 0) {
            out += V;
            i += T.size();
        } else {
            out += S[i++];
        }
    }
    return out;
}

static std::string word(int length, int bits) {
    std::string s(length, 'a');
    for (int i = 0; i < length; ++i) if ((bits >> i) & 1) s[i] = 'b';
    return s;
}

static void run_checks() {
    char out[LIMIT + 1] = "原值";
    CHECK(!replace_all("abc", "", "X", out));
    CHECK(std::strcmp(out, "原值") == 0);
    CHECK(!replace_all(NULL, "a", "X", out));
    CHECK(replace_all("ab ab", "ab", "xaby", out));
    CHECK(std::strcmp(out, "xaby xaby") == 0);
    CHECK(replace_all("aaaaaa", "aa", "X", out));
    CHECK(std::strcmp(out, "XXX") == 0);
    CHECK(replace_all("ab ab", "ab", "", out));
    CHECK(std::strcmp(out, " ") == 0);
    CHECK(replace_all("abc", "z", "X", out));
    CHECK(std::strcmp(out, "abc") == 0);
    CHECK(replace_all("ab", "abc", "X", out));
    CHECK(std::strcmp(out, "ab") == 0);
    CHECK(replace_all("", "a", "X", out));
    CHECK(out[0] == '\0');
    std::string full(LIMIT, 'a'), too_long(LIMIT + 1, 'a');
    CHECK(replace_all(full.c_str(), "a", "X", out));
    CHECK(std::strlen(out) == LIMIT);
    CHECK(!replace_all(full.c_str(), "a", "XX", out));
    CHECK(std::string(out) == std::string(LIMIT, 'X'));
    CHECK(!replace_all(too_long.c_str(), "a", "X", out));
    CHECK(!replace_all("a", too_long.c_str(), "X", out));
    CHECK(!replace_all("a", "a", too_long.c_str(), out));
    CHECK(replace_all("数据结构数据", "数据", "X", out));
    CHECK(std::strcmp(out, "X结构X") == 0);
    const char *values[] = {"", "X", "aa", "aba"};
    int cases = 0;
    for (int n = 0; n <= 6; ++n) for (int a = 0; a < (1 << n); ++a) {
        std::string S = word(n, a);
        for (int m = 1; m <= 4; ++m) for (int b = 0; b < (1 << m); ++b) {
            std::string T = word(m, b);
            int pos[LIMIT];
            int count = collect(S.c_str(), n, T.c_str(), m, true, pos);
            int expected_count = 0;
            for (int i = 0; i + m <= n; ++i) {
                if (S.compare(i, m, T) == 0) {
                    CHECK(expected_count < count);
                    CHECK(pos[expected_count++] == i);
                }
            }
            CHECK(count == expected_count);
            for (const char *V : values) {
                CHECK(replace_all(S.c_str(), T.c_str(), V, out));
                CHECK(out == reference(S, T, V));
                ++cases;
            }
        }
    }
    std::printf("PASS: %d checks；%d 组替换与重叠查找、容量边界\n", checks, cases);
}

int main(int argc, char **argv) {
    if (argc == 2 && std::strcmp(argv[1], "--check") == 0) {
        run_checks();
        return 0;
    }
    if (argc != 1) {
        std::fprintf(stderr, "用法：replace_demo [--check]\n");
        return EXIT_FAILURE;
    }
    int pos[LIMIT];
    int count = collect("aaaaaa", 6, "aa", 2, true, pos);
    std::printf("全部重叠位置：");
    print_positions(pos, count);
    count = collect("aaaaaa", 6, "aa", 2, false, pos);
    std::printf("非重叠位置：");
    print_positions(pos, count);
    show("aaaaaa", "aa", "X");
    show("ab ab", "ab", "xaby");
    show("ab ab", "ab", "");
    return 0;
}

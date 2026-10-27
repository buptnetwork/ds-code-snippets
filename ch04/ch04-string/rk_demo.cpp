// 第 3 次课教师演示：数字串的全窗口 RK，不在首次命中时返回。
// 字符 '0'..'9' 映射到数字 0..9；固定 d=10、q=13、长度上限 256。
// 不是通用哈希库，BM、AC 不在本工程增加实现。
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

static const int LIMIT = 256;
static const int D = 10, Q = 13;
static int checks = 0;
#define CHECK(expr) do { ++checks; if (!(expr)) { \
    std::fprintf(stderr, "检查失败，第 %d 行：%s\n", __LINE__, #expr); \
    std::exit(EXIT_FAILURE); } } while (0)

struct Window {
    int hash;
    int comparisons;
    bool matched;
};
struct Trace {
    int target = 0;
    int count = 0;
    int candidates = 0;
    int comparisons = 0;
    Window windows[LIMIT] = {};
};

static bool digits(const char *s) {
    if (s == NULL || std::strlen(s) > LIMIT) return false;
    for (int i = 0; s[i]; ++i) if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

// 失败保留 trace；空模式拒绝，空主串或模式更长时输出零个窗口。
static bool scan(const char *S, const char *T, Trace &trace) {
    if (!digits(S) || !digits(T) || T[0] == '\0') return false;
    int n = (int)std::strlen(S), m = (int)std::strlen(T);
    Trace result;
    int high = 1;
    for (int x = 0; x < m; ++x) {
        result.target = (result.target * D + T[x] - '0') % Q;
        if (x > 0) high = high * D % Q;
    }
    if (m <= n) {
        int hash = 0;
        for (int x = 0; x < m; ++x) hash = (hash * D + S[x] - '0') % Q;
        for (int i = 0; i <= n - m; ++i) {
            Window &w = result.windows[result.count++];
            w.hash = hash;
            // #region hash-verify
            if (hash == result.target) {
                ++result.candidates;
                int j = 0;
                while (j < m) {
                    ++w.comparisons;
                    if (S[i + j] != T[j]) break;
                    ++j;
                }
                w.matched = (j == m);
            }
            // #endregion hash-verify
            result.comparisons += w.comparisons;
            if (i < n - m) {
                // #region rolling-step
                int removed = S[i] - '0';
                int added = S[i + m] - '0';
                int value = (hash - removed * high) * D + added;
                hash = (value % Q + Q) % Q;
                // #endregion rolling-step
                // hash、high 均在 0..12；value 在 -1080..129，int 足够。
            }
        }
    }
    trace = result;
    return true;
}

static void check_case(const std::string &S, const std::string &T) {
    Trace t;
    CHECK(scan(S.c_str(), T.c_str(), t));
    int m = (int)T.size(), n = (int)S.size();
    CHECK(t.count == (n >= m ? n - m + 1 : 0));
    int candidates = 0, comparisons = 0;
    for (int i = 0; i < t.count; ++i) {
        // 独立从窗口起点计算，不使用前一个窗口的滚动状态。
        int hash = 0;
        for (int j = 0; j < m; ++j) hash = (hash * 10 + S[i + j] - '0') % 13;
        CHECK(t.windows[i].hash == hash);
        CHECK(t.windows[i].matched == (S.compare(i, m, T) == 0));
        int local = 0;
        if (hash == t.target) {
            ++candidates;
            for (int j = 0; j < m; ++j) {
                ++local;
                if (S[i + j] != T[j]) break;
            }
        }
        CHECK(t.windows[i].comparisons == local);
        comparisons += local;
    }
    CHECK(t.candidates == candidates);
    CHECK(t.comparisons == comparisons);
}

static void run_checks() {
    Trace t;
    CHECK(scan("31415402", "415", t));
    const int hashes[] = {2, 11, 12, 11, 7, 12};
    CHECK(t.target == 12 && t.count == 6);
    for (int i = 0; i < 6; ++i) CHECK(t.windows[i].hash == hashes[i]);
    CHECK(t.candidates == 2 && t.comparisons == 5);
    CHECK(t.windows[2].matched && t.windows[2].comparisons == 3);
    CHECK(!t.windows[5].matched && t.windows[5].comparisons == 2);
    CHECK(!scan("123", "", t));
    CHECK(t.count == 6 && t.comparisons == 5);
    CHECK(!scan("12a", "12", t));
    CHECK(!scan("123", "a", t));
    CHECK(!scan(NULL, "1", t));
    CHECK(!scan("1", NULL, t));
    CHECK(!scan(std::string(LIMIT + 1, '9').c_str(), "9", t));
    CHECK(!scan("9", std::string(LIMIT + 1, '9').c_str(), t));
    check_case("", "1");
    check_case("12", "123");
    check_case("00000", "000");
    check_case("1212121", "121");
    check_case(std::string(LIMIT, '9'), "999");
    check_case(std::string(LIMIT, '9'), std::string(LIMIT, '9'));
    int cases = 0;
    for (int x = 0; x < 1000; ++x) {
        char S[5];
        std::snprintf(S, sizeof S, "%03d", x);
        for (int y = 0; y < 100; ++y) {
            char T[4];
            std::snprintf(T, sizeof T, "%02d", y);
            check_case(S, T);
            ++cases;
        }
    }
    std::printf("PASS: %d checks；%d 组数字串与碰撞、边界用例\n", checks, cases);
}

int main(int argc, char **argv) {
    if (argc == 2 && std::strcmp(argv[1], "--check") == 0) {
        run_checks();
        return 0;
    }
    if (argc != 1) {
        std::fprintf(stderr, "用法：rk_demo [--check]\n");
        return EXIT_FAILURE;
    }
    const char *S = "31415402", *T = "415";
    Trace t;
    if (!scan(S, T, t)) return EXIT_FAILURE;
    std::printf("S=\"%s\" T=\"%s\" d=10 q=13 target=%d\n", S, T, t.target);
    for (int i = 0; i < t.count; ++i) {
        const Window &w = t.windows[i];
        const char *state = w.matched ? "真匹配" : w.hash == t.target ? "碰撞，拒绝" : "排除";
        std::printf("i=%d window=%.*s hash=%d verify=%d %s\n",
                    i, (int)std::strlen(T), S + i, w.hash, w.comparisons, state);
    }
    std::printf("全窗口：candidates=%d verify_cmp=%d\n", t.candidates, t.comparisons);
    return 0;
}

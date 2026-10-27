#include "match.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

static int checks = 0;
static void check(bool ok, int line) {
    ++checks;
    if (!ok) {
        fprintf(stderr, "FAIL: test_match.cpp:%d\n", line);
        exit(EXIT_FAILURE);
    }
}
#define CHECK(expr) check((expr), __LINE__)

// 用定义穷举真前后缀，与线性构建使用不同计算方法。
static std::vector<int> by_definition(const std::string &T) {
    std::vector<int> out(T.size());
    for (size_t k = 0; k < T.size(); ++k)
        for (size_t len = 1; len <= k; ++len)
            if (T.compare(0, len, T, k + 1 - len, len) == 0) out[k] = (int)len;
    return out;
}
static std::vector<std::string> words(int max_length) {
    std::vector<std::string> result = {""};
    for (int len = 1; len <= max_length; ++len)
        for (int mask = 0; mask < (1 << len); ++mask) {
            std::string word(len, 'a');
            for (int i = 0; i < len; ++i) if (mask & (1 << i)) word[i] = 'b';
            result.push_back(word);
        }
    return result;
}
static void one_case(const std::string &S, const std::string &T,
                     long long expected_bf = -1, long long expected_kmp = -1) {
    int n = (int)S.size(), m = (int)T.size();
    std::vector<int> nxt(m);
    BuildNext(T.data(), m, nxt.data());
    long long bf_cmp = -1, kmp_cmp = -1;
    size_t found = S.find(T);
    int expected = found == std::string::npos ? -1 : (int)found;
    CHECK(BF_Index(S.data(), n, T.data(), m, bf_cmp) == expected);
    CHECK(KMP_Index(S.data(), n, T.data(), m, nxt.data(), kmp_cmp) == expected);
    CHECK(bf_cmp >= 0 && kmp_cmp >= 0);
    CHECK(kmp_cmp <= 2LL * n);
    if (m == 0 || m > n) CHECK(bf_cmp == 0 && kmp_cmp == 0);
    else CHECK(bf_cmp <= (n - m + 1LL) * m);
    if (expected_bf >= 0) CHECK(bf_cmp == expected_bf);
    if (expected_kmp >= 0) CHECK(kmp_cmp == expected_kmp);
}

int main() {
    one_case("aaaaaaab", "aaab", 20, 12);
    one_case("abaabaabcac", "abaabcac", 17, 12);
    one_case("aaaaaaaa", "aaab", 20);
    one_case("", "", 0, 0);
    one_case("", "a", 0, 0);
    one_case("abc", "abcd", 0, 0);
    one_case("abc", "a", 1, 1);
    one_case("abc", "c", 3, 3);
    one_case("abc", "x", 3, 3);
    one_case("abc", "", 0, 0);
    one_case("aaaa", "aa", 2, 2);  // 首次匹配，不生成全部位置
    one_case(std::string("a\0b\0c", 5), std::string("\0b", 2));
    one_case(std::string("a\xff\xfe", 3), std::string("\xff\xfe", 2));
    one_case(u8"数据结构", u8"结构");

    // 空输入不读 NULL；空模式构表既不写数组，也不读模式。
    long long cmp = -1;
    BuildNext(NULL, 0, NULL);
    CHECK(BF_Index(NULL, 0, NULL, 0, cmp) == 0 && cmp == 0);
    CHECK(KMP_Index(NULL, 0, NULL, 0, NULL, cmp) == 0 && cmp == 0);
    int nxt_one[] = {0};
    CHECK(BF_Index(NULL, 0, "a", 1, cmp) == -1 && cmp == 0);
    CHECK(KMP_Index(NULL, 0, "a", 1, nxt_one, cmp) == -1 && cmp == 0);
    const char raw_S[] = {'a', 'b', 'c'};  // 没有结束符也可匹配
    const char raw_T[] = {'b', 'c'};
    int raw_next[2];
    BuildNext(raw_T, 2, raw_next);
    CHECK(BF_Index(raw_S, 3, raw_T, 2, cmp) == 1);
    CHECK(KMP_Index(raw_S, 3, raw_T, 2, raw_next, cmp) == 1);

    const auto patterns = words(8);
    for (const auto &T : patterns) {
        std::vector<int> guarded(T.size() + 2, -99);
        BuildNext(T.data(), (int)T.size(), guarded.data() + 1);
        CHECK(guarded.front() == -99 && guarded.back() == -99);
        CHECK(std::vector<int>(guarded.begin() + 1, guarded.end() - 1) == by_definition(T));
    }
    for (const std::string T : {"abaabcac", "ababb", "aaaa", "abab"}) {
        std::vector<int> nxt(T.size());
        BuildNext(T.data(), (int)T.size(), nxt.data());
        CHECK(nxt == by_definition(T));
    }
    const auto cases = words(6);
    for (const auto &S : cases)
        for (const auto &T : cases) one_case(S, T);
    printf("PASS: %d checks；511 个前缀表、16129 组匹配与字节边界用例\n", checks);
}

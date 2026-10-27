#include "hstring.h"
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

// 不使用 assert：Release / NDEBUG 下仍执行每一次检查。
static int checks = 0;
static void check(bool ok, int line) {
    ++checks;
    if (!ok) {
        fprintf(stderr, "FAIL: test_string.cpp:%d\n", line);
        exit(EXIT_FAILURE);
    }
}
#define CHECK(expr) check((expr), __LINE__)

static bool valid(const HString &S) {
    if (S.ch == NULL) return S.length == 0 && S.capacity == 0;
    return S.length >= 0 && S.capacity >= S.length && S.ch[S.length] == '\0';
}
static bool equals(const HString &S, const std::string &value) {
    return valid(S) && (size_t)S.length == value.size() &&
           (value.empty() || memcmp(S.ch, value.data(), value.size()) == 0);
}
static int sign(int n) { return (n > 0) - (n < 0); }

static void empty_and_boundaries() {
    HString S, T, dst;
    CHECK(HS_Init(S)); CHECK(HS_Init(T)); CHECK(HS_Init(dst));
    CHECK(equals(S, "") && S.ch == NULL);
    CHECK(HS_Compare(S, T) == 0);
    CHECK(HS_SubString(dst, S, 0, 0));
    CHECK(equals(dst, "") && dst.ch != NULL);
    CHECK(HS_Concat(dst, S, T)); CHECK(equals(dst, ""));
    CHECK(HS_Assign(S, "")); CHECK(equals(S, "") && S.ch != NULL);
    CHECK(!HS_Assign(S, NULL)); CHECK(equals(S, ""));
    CHECK(HS_Assign(S, "Data Structure"));
    CHECK(S.length == 14 && S.capacity == 16);
    CHECK(HS_Concat(dst, S, T)); CHECK(equals(dst, "Data Structure"));
    CHECK(HS_Concat(dst, T, S)); CHECK(equals(dst, "Data Structure"));
    CHECK(HS_SubString(dst, S, 5, 6)); CHECK(equals(dst, "Struct"));
    const int bad[][2] = {{5,10}, {15,0}, {14,1}, {-1,0}, {0,-1}, {INT_MAX,0}, {1,INT_MAX}};
    for (const auto &args : bad) {
        char *old = dst.ch;
        CHECK(!HS_SubString(dst, S, args[0], args[1]));
        CHECK(equals(dst, "Struct") && dst.ch == old);
    }
    CHECK(!HS_SubString(S, S, 0, 1)); CHECK(equals(S, "Data Structure"));
    CHECK(!HS_Concat(S, S, T)); CHECK(equals(S, "Data Structure"));
    CHECK(!HS_Concat(S, T, S)); CHECK(equals(S, "Data Structure"));
    CHECK(HS_SubString(dst, S, 14, 0)); CHECK(equals(dst, ""));
    CHECK(HS_Concat(dst, S, S)); CHECK(equals(dst, "Data StructureData Structure"));
    HS_Destroy(S); HS_Destroy(T); HS_Destroy(dst);
    CHECK(valid(S) && S.ch == NULL);
    HS_Destroy(S); CHECK(valid(S));
}

static void fixed_capacity() {
    SString S{};
    CHECK(SS_Assign(S, "Data Structure"));
    CHECK(S.length == 14 && memcmp(S.ch, "Data Structure", 14) == 0);
    std::string full(MAXLEN, 'a');
    CHECK(SS_Assign(S, full.c_str())); CHECK(S.length == MAXLEN);
    CHECK(!SS_Assign(S, (full + 'b').c_str()));
    CHECK(S.length == MAXLEN && memcmp(S.ch, full.data(), MAXLEN) == 0);
    CHECK(!SS_Assign(S, NULL)); CHECK(S.length == MAXLEN);
    CHECK(SS_Assign(S, "")); CHECK(S.length == 0);
}

static void allocation_failures() {
    HString S, T, dst;
    HS_Init(S); HS_Init(T); HS_Init(dst);
    HS_TestFailNextAllocation();
    CHECK(!HS_Assign(S, "")); CHECK(S.ch == NULL && valid(S));
    HS_TestFailNextAllocation();
    CHECK(!HS_Concat(dst, S, T)); CHECK(dst.ch == NULL && valid(dst));
    HS_TestFailNextAllocation();
    CHECK(!HS_SubString(dst, S, 0, 0)); CHECK(dst.ch == NULL && valid(dst));
    CHECK(HS_Assign(S, "keep"));
    char *old = S.ch;
    int cap = S.capacity;
    HS_TestFailNextAllocation();
    CHECK(!HS_Assign(S, "Data Structure"));
    CHECK(S.ch == old && S.capacity == cap && equals(S, "keep"));
    CHECK(HS_Assign(T, "Data Structure"));
    CHECK(HS_Assign(dst, "keep")); old = dst.ch; cap = dst.capacity;
    HS_TestFailNextAllocation();
    CHECK(!HS_Concat(dst, S, T));
    CHECK(dst.ch == old && dst.capacity == cap && equals(dst, "keep"));
    HS_TestFailNextAllocation();
    CHECK(!HS_SubString(dst, T, 0, 14));
    CHECK(dst.ch == old && dst.capacity == cap && equals(dst, "keep"));
    CHECK(HS_SubString(dst, T, 0, 14)); CHECK(equals(dst, "Data Structure"));
    HS_Destroy(S); HS_Destroy(T); HS_Destroy(dst);
}

static void byte_content() {
    HString S, T, dst;
    HS_Init(S); HS_Init(T); HS_Init(dst);
    CHECK(HS_Assign(S, "abc")); CHECK(HS_Assign(T, "abd"));
    // 合法对象内部允许零字节；逻辑长度仍为 3。
    S.ch[1] = '\0'; T.ch[1] = '\0';
    CHECK(HS_Compare(S, T) < 0);
    CHECK(HS_Concat(dst, S, T));
    CHECK(equals(dst, std::string("a\0ca\0d", 6)));
    CHECK(HS_SubString(dst, S, 1, 2)); CHECK(equals(dst, std::string("\0c", 2)));
    CHECK(HS_Assign(S, "x")); CHECK(HS_Assign(T, "x"));
    S.ch[0] = (char)0x80; T.ch[0] = (char)0x7f;
    CHECK(HS_Compare(S, T) > 0);
    HS_Destroy(S); HS_Destroy(T); HS_Destroy(dst);
}

static void generated_cases() {
    std::vector<std::string> words = {""};
    for (int len = 1; len <= 5; ++len)
        for (int bits = 0; bits < (1 << len); ++bits) {
            std::string word(len, 'a');
            for (int i = 0; i < len; ++i) if (bits & (1 << i)) word[i] = 'b';
            words.push_back(word);
        }
    words.insert(words.end(), {"Apple", "apple", "abc", "abd", "z", "abcdefg"});
    HString S, T, dst;
    HS_Init(S); HS_Init(T); HS_Init(dst);
    for (const auto &a : words) {
        CHECK(HS_Assign(S, a.c_str())); CHECK(equals(S, a));
        for (int pos = 0; pos <= (int)a.size(); ++pos)
            for (int len = 0; len <= (int)a.size() - pos; ++len) {
                CHECK(HS_SubString(dst, S, pos, len));
                CHECK(equals(dst, a.substr(pos, len)));
            }
        for (const auto &b : words) {
            CHECK(HS_Assign(T, b.c_str()));
            CHECK(sign(HS_Compare(S, T)) == sign(a.compare(b)));
            CHECK(HS_Concat(dst, S, T)); CHECK(equals(dst, a + b));
            CHECK(equals(S, a) && equals(T, b));
        }
    }
    for (int n : {7, 8, 9, 15, 16, 17, 255, 256, 1024}) {
        std::string text(n, 'a');
        CHECK(HS_Assign(S, text.c_str())); CHECK(equals(S, text));
        CHECK(HS_Assign(S, "a")); CHECK(equals(S, "a"));
    }
    HS_Destroy(S); HS_Destroy(T); HS_Destroy(dst);
}

int main() {
    fixed_capacity(); empty_and_boundaries(); allocation_failures();
    byte_content(); generated_cases();
    printf("PASS: %d checks（含空串、范围、别名限制、分配失败、字节语义与生成用例）\n", checks);
}

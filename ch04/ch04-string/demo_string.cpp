#include "hstring.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// 固定输入，无文件与网络依赖；重新运行即可复位。失败用非零状态退出。
static void require(bool ok) {
    if (!ok) {
        fprintf(stderr, "演示失败：操作返回 false。\n");
        exit(EXIT_FAILURE);
    }
}

// 长度是输出依据，既不把 NULL 交给 %s，也不依赖内部是否含零字节。
static void print_bytes(const char *data, int length) {
    putchar('"');
    if (length > 0) fwrite(data, 1, (size_t)length, stdout);
    puts("\"");
}

static void representations() {
    SString fixed{};
    HString dynamic;
    HS_Init(dynamic);
    char cstr[] = "Data Structure";
    require(SS_Assign(fixed, cstr));
    require(HS_Assign(dynamic, cstr));
    printf("[SString] len=%d data=", fixed.length);
    print_bytes(fixed.ch, fixed.length);
    printf("[HString] len=%d cap=%d data=", dynamic.length, dynamic.capacity);
    print_bytes(dynamic.ch, dynamic.length);
    printf("[cstr   ] strlen=%zu sizeof=%zu data=", strlen(cstr), sizeof(cstr));
    print_bytes(cstr, (int)strlen(cstr));
    HS_Destroy(dynamic);
}

static void c_strings() {
    // #region array
    char a[8] = "abc";
    printf("[array] strlen=%zu sizeof=%zu\n", strlen(a), sizeof(a));
    a[0] = 'A';
    printf("[array] modified=%s\n", a);
    const char *p = "abc";
    printf("[literal] data=%s\n", p);
    // p[0] = 'A';  // 编译错误：不能通过 const char* 写入
    // #endregion array

    // #region bounded
    char buf[8];
    const char *source = "Data Structure";
    int r = snprintf(buf, sizeof(buf), "%s", source);
    if (r < 0) {
        puts("[snprintf] 输出错误");
    } else if ((size_t)r >= sizeof(buf)) {
        printf("[snprintf] 截断: data=\"%s\" required=%d capacity=%zu\n",
               buf, r, sizeof(buf));
    } else {
        printf("[snprintf] 完整: %s\n", buf);
    }
    // #endregion bounded
}

static void operations() {
    // #region concat-call
    HString left, right, result;
    HS_Init(left); HS_Init(right); HS_Init(result);
    require(HS_Assign(left, "Data"));
    require(HS_Assign(right, " Structure"));
    require(HS_Concat(result, left, right));
    printf("[concat] len=%d cap=%d data=", result.length, result.capacity);
    print_bytes(result.ch, result.length);
    // #endregion concat-call

    HString sub;
    HS_Init(sub);
    // #region substring-call
    require(HS_SubString(sub, result, 5, 6));
    printf("[substring 5,6] len=%d data=", sub.length);
    print_bytes(sub.ch, sub.length);
    // #endregion substring-call
    bool ok = HS_SubString(sub, result, 5, 10);
    require(!ok);
    printf("[substring 5,10] ok=%d unchanged=", ok);
    print_bytes(sub.ch, sub.length);
    require(HS_SubString(sub, result, 14, 0));
    printf("[substring 14,0] len=%d data=", sub.length);
    print_bytes(sub.ch, sub.length);
    ok = HS_SubString(sub, result, 15, 0);
    require(!ok);
    printf("[substring 15,0] ok=%d\n", ok);
    HS_Destroy(sub);
    HS_Destroy(left); HS_Destroy(right); HS_Destroy(result);
}

static void comparisons() {
    const char *pairs[][2] = {
        {"abc", "abd"}, {"ab", "abc"}, {"abc", "abc"},
        {"", "a"}, {"Apple", "apple"}
    };
    HString left, right;
    HS_Init(left); HS_Init(right);
    for (const auto &pair : pairs) {
        require(HS_Assign(left, pair[0]));
        require(HS_Assign(right, pair[1]));
        int r = HS_Compare(left, right);
        printf("[compare] \"%s\" %c \"%s\"\n", pair[0], r < 0 ? '<' : r > 0 ? '>' : '=', pair[1]);
    }
    HS_Destroy(left); HS_Destroy(right);
}

int main(int argc, char **argv) {
    const char *section = argc == 2 ? argv[1] : "all";
    if (argc > 2 || (strcmp(section, "all") && strcmp(section, "representations") &&
        strcmp(section, "cstr") && strcmp(section, "operations") && strcmp(section, "compare"))) {
        fprintf(stderr, "用法：demo_string [all|representations|cstr|operations|compare]\n");
        return EXIT_FAILURE;
    }
    if (!strcmp(section, "all") || !strcmp(section, "representations")) representations();
    if (!strcmp(section, "all") || !strcmp(section, "cstr")) c_strings();
    if (!strcmp(section, "all") || !strcmp(section, "operations")) operations();
    if (!strcmp(section, "all") || !strcmp(section, "compare")) comparisons();
    return EXIT_SUCCESS;
}

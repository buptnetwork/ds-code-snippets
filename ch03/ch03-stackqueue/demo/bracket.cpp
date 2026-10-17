// bracket.cpp：应用一 —— 括号匹配。
// 判定只含 ( ) [ ] { } 的字符串是否正确配对嵌套。
// 编译：c++ -std=c++17 -Wall -Wextra bracket.cpp ../src/seqstack.cpp -o bracket
#include <cstdio>
#include "../src/seqstack.h"

static bool matched(char left, char right) {
    return (left == '(' && right == ')')
        || (left == '[' && right == ']')
        || (left == '{' && right == '}');
}

// #region bracket-scan
bool BracketCheck(const char *s) {
    SeqStack S = {};
    if (!SS_Init(S, 8)) return false;
    bool ok = true;
    for (int i = 0; s[i] != '\0'; ++i) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            if (!SS_Push(S, c)) { ok = false; break; }   // 左括号：入栈
        }
// #endregion bracket-scan
// #region bracket-right
        else if (c == ')' || c == ']' || c == '}') {      // 右括号：需判断
            if (SS_Empty(S)) { ok = false; break; }       // ① 右括号多余
            char t = (char)SS_Top(S);                     // ② 只看，不弹
            if (!matched(t, c)) { ok = false; break; }    // ③ 类型不配
            ElemType dummy;
            SS_Pop(S, dummy);                             // ④ 配上了才弹
        }
        // 其他字符：本例忽略
    }
    if (ok && !SS_Empty(S)) ok = false;                   // ⑤ 左括号多余
    SS_Destroy(S);                                        // 无论成败都要释放
    return ok;
}
// #endregion bracket-right

int main() {
    const char *cases[] = {"([]{})", "([)]", "(()", ")(", ""};
    for (int i = 0; i < 5; ++i)
        printf("\"%s\" -> %s\n", cases[i], BracketCheck(cases[i]) ? "OK" : "FAIL");
    return 0;
}

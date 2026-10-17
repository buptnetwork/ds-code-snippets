// bracket_bug.cpp：预期错误示例（单元 7 反例）—— 缺少遍历结束后的 ⑤ “栈非空 → 失败” 检查。
// 后果：未闭合的左括号被误判为合法，如 "(((" 与 "(()" 都会错误地返回 OK。
// 本文件故意保留缺陷，用于课堂对照；正确实现见 demo/bracket.cpp。
// 编译：c++ -std=c++17 -Wall -Wextra bracket_bug.cpp ../src/seqstack.cpp -o bracket_bug
#include <cstdio>
#include "../src/seqstack.h"

static bool matched(char left, char right) {
    return (left == '(' && right == ')')
        || (left == '[' && right == ']')
        || (left == '{' && right == '}');
}

// #region buggy
bool BracketCheckBuggy(const char *s) {
    SeqStack S = {};
    if (!SS_Init(S, 8)) return false;
    bool ok = true;
    for (int i = 0; s[i] != '\0'; ++i) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            if (!SS_Push(S, c)) { ok = false; break; }
        } else if (c == ')' || c == ']' || c == '}') {
            if (SS_Empty(S)) { ok = false; break; }       // ① 右括号多余（这一半还在）
            char t = (char)SS_Top(S);
            if (!matched(t, c)) { ok = false; break; }    // ③ 类型不配
            ElemType dummy; SS_Pop(S, dummy);
        }
    }
    // 缺陷：这里少了 ⑤ —— if (ok && !SS_Empty(S)) ok = false;
    SS_Destroy(S);
    return ok;
}
// #endregion buggy

int main() {
    const char *cases[] = {"(((", "(()", "([]{})"};
    for (int i = 0; i < 3; ++i)
        printf("\"%s\" -> %s\n", cases[i], BracketCheckBuggy(cases[i]) ? "OK" : "FAIL");
    return 0;
}

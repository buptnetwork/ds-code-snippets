#ifndef CH04_MATCH_H
#define CH04_MATCH_H

// 第 2 次课：BF 与 KMP 的首次精确字节匹配。
// n、m 非负；正长度输入至少有对应数量的可读字节，不依赖结束符。
// 从主串下标 0 开始搜索；返回 0-based 起点，未找到 -1，空模式 0。
// cmp 只统计主串与模式的字符比较，包括失败，不含预处理；每次调用重置。
// m>0 时 nxt 有 m 格，KMP 调用前必须已按同一 T 正确构建。
// BuildNext 的输出不与 T 缓冲区重叠；m=0 时不读 T 或 nxt。

// #region interfaces
int BF_Index(const char *S, int n, const char *T, int m, long long &cmp);
void BuildNext(const char *T, int m, int *nxt);
int KMP_Index(const char *S, int n, const char *T, int m,
              const int *nxt, long long &cmp);
// #endregion interfaces

#endif

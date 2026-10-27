#ifndef CH04_HSTRING_H
#define CH04_HSTRING_H

// 第 4 章第 1 次课：C++17、公开结构体、引用参数。
// 本工程只实现本次课代表操作；匹配算法属于后续次课。

// #region fixed
const int MAXLEN = 255;
struct SString {
    char ch[MAXLEN];       // 有效内容为 [0,length)，不承诺结束符
    int length;
};
// #endregion fixed

// 由有效 C 字符串赋值；超过 MAXLEN 或 cs 为 NULL 时失败且保留原值。
// cs 不得指向 S.ch；成功后只保证前 length 个字节有效。
bool SS_Assign(SString &S, const char *cs);

// #region dynamic
struct HString {
    char *ch;
    int length;           // 有效字节数，权威长度
    int capacity;         // 有效字节容量，不含额外结束符那一格
};
// #endregion dynamic

// 合法状态：未分配时为 NULL、0、0；分配后持有 capacity+1 字节，
// 0 <= length <= capacity，ch[length]=='\0'。内部允许有零字节。
// 除 Init 外，接口均要求对象已经初始化且处于合法状态。
// 不允许浅拷贝拥有缓冲区的结构体；不同对象的缓冲区不能共享。
// Init 只用于尚未持有资源的对象；Destroy 后回到未分配空串。
bool HS_Init(HString &S);
void HS_Destroy(HString &S);

// cs 为有效 C 字符串且不指向 S.ch 内部；NULL 返回 false。
// 返回 false（参数错误、长度溢出或分配失败）时保留原对象。
bool HS_Assign(HString &S, const char *cs);

// #region operations
// 目标与源必须是不同对象且缓冲区独立；源 S 与 T 可以是同一个对象。
bool HS_Concat(HString &dst, const HString &S, const HString &T);
// 按 unsigned char 字节值比较；只看返回值的负、零、正。
int HS_Compare(const HString &S, const HString &T);
// 0-based；0 <= pos <= S.length，0 <= len <= S.length-pos。
// 不支持原地取子串；非法范围、同对象或分配失败时 sub 不变。
bool HS_SubString(HString &sub, const HString &S, int pos, int len);
// #endregion operations

#ifdef CH04_TEST_ALLOC
// 仅测试构建提供：让下一次真正的缓冲区分配失败，之后自动恢复。
void HS_TestFailNextAllocation();
#endif

#endif

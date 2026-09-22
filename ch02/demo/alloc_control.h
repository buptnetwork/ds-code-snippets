#ifndef CH02_ALLOC_CONTROL_H
#define CH02_ALLOC_CONTROL_H
#include <cstdlib>

// 仅供独立测试：所有标准头文件必须在本文件之前包含。
// -1 不注入失败；0 下一次申请失败；正数表示还允许的申请次数。
// 这是可重复的故障注入，不是内存诊断器，也不检查所有非法访问。
inline long allocationBudget = -1;
inline long liveBlocks = 0;
inline bool AllowAllocation() {
    if (allocationBudget == 0) return false;
    if (allocationBudget > 0) --allocationBudget;
    return true;
}
inline void *TestMalloc(std::size_t bytes) {
    if (!AllowAllocation()) return NULL;
    void *p = std::malloc(bytes);
    if (p) ++liveBlocks;
    return p;
}
inline void *TestRealloc(void *old, std::size_t bytes) {
    if (!AllowAllocation()) return NULL;
    const bool fresh = old == NULL;
    void *p = std::realloc(old, bytes); // 本章只请求正字节数。
    if (p && fresh) ++liveBlocks;
    return p;
}
inline void TestFree(void *p) {
    if (p) --liveBlocks;
    std::free(p);
}
#define malloc TestMalloc
#define realloc TestRealloc
#define free TestFree
#endif

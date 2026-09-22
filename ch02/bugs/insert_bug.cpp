#include <cassert>
#include <cstdio>

// 独立阅读题：容量已足够，只检查搬移方向；不含越界或未初始化读取。
// #region bug
static void InsertBug(int data[], int &size, int pos, int x) {
    for (int i = pos; i < size; ++i)
        data[i + 1] = data[i];
    data[pos] = x;
    ++size;
}
// #endregion bug

int main() {
    int data[8] = {10, 20, 30, 40};
    int size = 4;
    InsertBug(data, size, 1, 99);
    // 仅验证缺陷复现，不提供作业修复答案。
    assert(size == 5 && data[2] == 20 && data[3] == 20 && data[4] == 20);
    for (int i = 0; i < size; ++i)
        std::printf("%s%d", i ? "," : "", data[i]);
    std::puts("");
}

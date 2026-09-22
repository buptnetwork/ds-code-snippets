#include "../src/linklist.h"
#include "../src/dlist.h"
#include <cstddef>
#include <cstdio>
int main() {
    std::printf("编译器：%s\n", __VERSION__);
#if defined(__aarch64__) || defined(__arm64__)
    std::puts("架构：arm64");
#elif defined(__x86_64__)
    std::puts("架构：x86_64");
#else
    std::puts("架构：其他（请结合运行环境记录）");
#endif
    // #region probe
    std::printf("int=%zu pointer=%zu\n", sizeof(int), sizeof(void *));
    std::printf("LNode=%zu data@%zu next@%zu\n", sizeof(LNode),
                offsetof(LNode, data), offsetof(LNode, next));
    std::printf("DNode=%zu data@%zu prev@%zu next@%zu\n", sizeof(DNode),
                offsetof(DNode, data), offsetof(DNode, prev), offsetof(DNode, next));
    // #endregion probe
}

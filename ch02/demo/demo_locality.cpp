#include "../src/linklist.h"
#include <algorithm>
#include <array>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

// 实验程序，不是容器实现；仅支持 GCC/Clang 的编译器屏障扩展。
// --check 只核对构造与求和，不产生性能数据。正式测量前补记CPU、缓存、系统负载。
#if !defined(__GNUC__) && !defined(__clang__)
#error 此实验需要GCC或Clang编译器屏障
#endif
static void Barrier() { __asm__ __volatile__("" ::: "memory"); }
static volatile std::int64_t sink = 0; // 仅计时结束后消费，不逐元素volatile。
using Clock = std::chrono::steady_clock;

// #region sum-array
__attribute__((noinline)) static std::int64_t SumArray(const int *a, std::size_t n) {
    std::int64_t sum = 0;
    for (std::size_t i = 0; i < n; ++i) sum += a[i];
    return sum;
}
// #endregion sum-array
// #region sum-list
__attribute__((noinline)) static std::int64_t SumList(const LNode *p) {
    std::int64_t sum = 0;
    for (; p; p = p->next) sum += p->data;
    return sum;
}
// #endregion sum-list

// 同一池重复重连，布局和data永不改变；构造成本全部排除在计时外。
static LNode *Relink(LNode *pool, const std::vector<std::size_t> &order, bool shuffled) {
    std::size_t n = order.size();
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t at = shuffled ? order[i] : i;
        pool[at].next = i + 1 == n ? NULL : &pool[shuffled ? order[i + 1] : i + 1];
    }
    return &pool[shuffled ? order[0] : 0];
}

struct Sample { double ms; std::int64_t checksum; };
// #region measure
static Sample Measure(int group, const int *a, std::size_t n,
                      LNode *first, int repeats) {
    std::int64_t checksum = 0;
    Barrier();
    auto start = Clock::now();
    for (int i = 0; i < repeats; ++i) {
        Barrier(); // 每次遍历都保留；三组同样处理。
        checksum += group == 0 ? SumArray(a, n) : SumList(first);
    }
    Barrier();
    auto stop = Clock::now();
    sink = checksum;
    return {std::chrono::duration<double, std::milli>(stop - start).count(), checksum};
}
// #endregion measure
static void Verify(const Sample &s, std::int64_t expected, int repeats) {
    if (s.checksum != expected * repeats) throw std::runtime_error("求和校验失败");
}

static int Run(std::size_t n, bool checkOnly) {
    // 三块工作空间同时存在；测试范围1..1e7避免字节乘法与校验和溢出。
    int *a = (int *)std::malloc(n * sizeof(int));
    LNode *pool = (LNode *)std::malloc(n * sizeof(LNode));
    if (!a || !pool) { std::free(a); std::free(pool); throw std::bad_alloc(); }
    try {
        std::vector<std::size_t> order(n);
        std::iota(order.begin(), order.end(), 0);
        const unsigned seed = 20260922;
        std::mt19937 generator(seed);
        // Fisher–Yates。固定工具链+标准库+种子可复现具体排列。
        for (std::size_t i = n - 1; i > 0; --i) {
            std::uniform_int_distribution<std::size_t> pick(0, i);
            std::swap(order[i], order[pick(generator)]);
        }
        std::int64_t expected = 0;
        for (std::size_t i = 0; i < n; ++i) {
            a[i] = (int)(i % 1000) - 500;
            pool[i].data = a[i];
            expected += a[i];
        }
        // 排列为双射，两种链接均到NULL且访问恰好n个结点。
        for (bool shuffled : {false, true}) {
            LNode *first = Relink(pool, order, shuffled);
            std::size_t count = 0;
            for (LNode *p = first; p; p = p->next) {
                if (++count > n) throw std::runtime_error("链接未终止");
            }
            if (count != n || SumList(first) != expected || SumArray(a, n) != expected)
                throw std::runtime_error("构造或求和错误");
        }
        if (checkOnly) {
            std::printf("局部性构造检查通过：n=%zu，同池顺序/打乱均覆盖全部结点；未计时。\n", n);
        } else {
            std::printf("编译器=%s；选项以Makefile的-O2构建为准；seed=%u\n", __VERSION__, seed);
            std::printf("n=%zu int=%zu node=%zu index=%zu 主要空间=%zu B\n",
                        n, sizeof(int), sizeof(LNode), sizeof(std::size_t),
                        n * (sizeof(int) + sizeof(LNode) + sizeof(std::size_t)));
            std::array<int, 3> repeats = {1, 1, 1};
            std::array<std::array<double, 10>, 3> times{};
            // 各组校准到每批约>=5ms；每次重连后预热。
            for (int g = 0; g < 3; ++g) {
                LNode *first = Relink(pool, order, g == 2);
                sink = g == 0 ? SumArray(a, n) : SumList(first);
                while (true) {
                    Sample s = Measure(g, a, n, first, repeats[g]);
                    Verify(s, expected, repeats[g]);
                    if (s.ms >= 5.0 || repeats[g] == 65536) break;
                    repeats[g] *= 2;
                }
            }
            std::puts("round,group,repeats,ms_per_traversal");
            for (int round = 0; round < 10; ++round) {
                for (int j = 0; j < 3; ++j) {
                    int g = (round + j) % 3;
                    LNode *first = Relink(pool, order, g == 2);
                    sink = g == 0 ? SumArray(a, n) : SumList(first);
                    Sample s = Measure(g, a, n, first, repeats[g]);
                    Verify(s, expected, repeats[g]);
                    times[g][round] = s.ms / repeats[g];
                    std::printf("%d,%c,%d,%.9f\n", round, 'A' + g, repeats[g], times[g][round]);
                }
            }
            for (int g = 0; g < 3; ++g) {
                std::sort(times[g].begin(), times[g].end());
                std::printf("%c median_ms=%.9f\n", 'A' + g, (times[g][4] + times[g][5]) / 2);
            }
            std::printf("校验和=%lld\n", (long long)expected);
        }
    } catch (...) { std::free(pool); std::free(a); throw; }
    std::free(pool); // 池的唯一拥有者；不沿链逐结点free。
    std::free(a);
    return 0;
}
int main(int argc, char **argv) {
    bool checkOnly = argc == 2 && std::strcmp(argv[1], "--check") == 0;
    std::size_t n = checkOnly ? 1000 : 10000;
    if (argc > 2) { std::fputs("用法：demo_locality [n|--check]\n", stderr); return 1; }
    if (argc == 2 && !checkOnly) {
        char *end;
        errno = 0;
        unsigned long long value = std::strtoull(argv[1], &end, 10);
        if (errno || *end || argv[1][0] == '-' || value < 1 || value > 10000000) {
            std::fputs("n须为1..10000000；先评估演示机内存。\n", stderr); return 1;
        }
        n = (std::size_t)value;
    }
    try { return Run(n, checkOnly); }
    catch (const std::exception &e) { std::fprintf(stderr, "实验终止：%s\n", e.what()); return 1; }
}

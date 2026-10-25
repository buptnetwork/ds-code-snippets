// scheduler.cpp：打印机任务调度模拟（单元 9）。循环队列容量固定，队满则 REJECT（限流）。
// 模型：每个时刻先“到达”后“服务”；服务端每时刻处理一个时间单位。
// 用自实现 LCG 保证跨平台可复现；三种容量（3/5/20）跑同一条到达序列以公平对比。
// 统计：平均周转时间（完成时刻 − 到达时刻，含自身服务时间）、最大队长、被拒绝任务数。
#include <cstdio>
#include "../src/circqueue.h"

static const int T = 20;            // 模拟时刻数
static const int ARRIVE_PROB = 60;  // 每时刻到达概率（百分比）
static const int MAXJ = 64;
static const unsigned long SEED = 42UL;   // 固定随机种子（课件须注明）

static unsigned long rng_state;
static void rng_seed(unsigned long s) { rng_state = s; }
static int rnd(int m) { rng_state = rng_state * 1103515245UL + 12345UL; return (int)((rng_state >> 16) % (unsigned)m); }

static int arriveT[MAXJ], remain[MAXJ];

// 跑一次模拟；trace 非空时打印每时刻轨迹。返回三项指标打包到 out 参数。
static void Run(int cap, bool trace, double *avgWait, int *maxQlen, int *rejected) {
    rng_seed(SEED);                                  // 每种容量都从同一种子开始 → 同一到达序列
    CircQueue Q = {}; CQ_Init(Q, cap);
    int nextId = 1, completed = 0, waitSum = 0, rej = 0, maxq = 0;

    for (int t = 1; t <= T; ++t) {
        // ① 到达
        int arrived = 0, alen = 0; bool rejectedThis = false;
        if (rnd(100) < ARRIVE_PROB) {
            arrived = nextId++; alen = 1 + rnd(4);     // 时长 1..4；每次到达分配唯一 id
            arriveT[arrived] = t; remain[arrived] = alen;
            if (!CQ_EnQueue(Q, arrived)) { rejectedThis = true; rej++; }
        }
        // ② 服务（每时刻一个单位）
        int done = 0;
        if (!CQ_Empty(Q)) {
            int id = CQ_Head(Q);
            if (--remain[id] == 0) { CQ_DeQueue(Q, id); done = id; completed++; waitSum += (t - arriveT[id]); }
        }
        int qlen = CQ_Length(Q);
        if (qlen > maxq) maxq = qlen;

        if (trace) {
            printf("t=%-2d ", t);
            if (arrived && !rejectedThis) printf("arrive J%d(len=%d)  ", arrived, alen);
            else if (rejectedThis)       printf("REJECT J%d (full)   ", arrived);
            else                          printf("arrive -            ");
            if (done) printf("done J%d  ", done); else printf("done -    ");
            printf("qlen=%d\n", qlen);
        }
    }
    *avgWait = completed ? (double)waitSum / completed : 0.0;
    *maxQlen = maxq; *rejected = rej;
    CQ_Destroy(Q);
}

int main() {
    puts("=== 打印机任务调度模拟（cap=5，seed=42，T=20，到达概率 60%，时长 1..4）===");
    double aw; int mq, rj;
    Run(5, true, &aw, &mq, &rj);
    printf("\ncap=5 汇总：平均周转=%.2f，最大队长=%d，被拒绝=%d\n", aw, mq, rj);

    puts("\n=== 三种容量对比（同一到达序列）===");
    printf("%-6s %-12s %-10s %-10s\n", "cap", "平均周转", "最大队长", "被拒绝");
    for (int i = 0; i < 3; ++i) {
        int caps[3] = {3, 5, 20};
        Run(caps[i], false, &aw, &mq, &rj);
        printf("%-6d %-12.2f %-10d %-10d\n", caps[i], aw, mq, rj);
    }
    printf("\n说明：以上为固定种子 %lu 的真实输出，可复现；趋势（拒绝减少/周转增加）\n", SEED);
    printf("依赖具体到达序列，有限样本不保证每组都单调；周转 = 完成时刻 − 到达时刻，只统计已完成任务。\n");
    return 0;
}

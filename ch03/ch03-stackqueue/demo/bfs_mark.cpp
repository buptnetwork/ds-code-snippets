// bfs_mark.cpp：对照 4（单元 8）——BFS“入队时标记” vs “出队时标记”的入队次数。
// 2×2 全通网格，S=(0,0)、E=(1,1)，A=(0,1)、B=(1,0)，按“右、下、左、上”检查，运行至队空。
// 入队时标记：每格至多入队一次，共 4 次。出队时标记：E 会被 A、B 各加入一次，共 5 次。
// ★ 这是重复入队对照，不宣称出队标记版遍历结果必然错误——它配合“出队遇已标记就跳过”仍正确，只是有重复队列项。
#include <cstdio>
#include "../src/circqueue.h"

static const int R = 2, C = 2;
static const int DR[4] = {0, 1, 0, -1};   // 右、下、左、上
static const int DC[4] = {1, 0, -1, 0};

// 版本一：成功入队时立即标记
static int BfsMarkOnEnqueue() {
    bool seen[R][C] = {};
    CircQueue Q = {}; CQ_Init(Q, R * C + 2);
    int enq = 0;
    CQ_EnQueue(Q, 0); seen[0][0] = true; enq++;         // S 入队即标记
    while (!CQ_Empty(Q)) {
        int cur; CQ_DeQueue(Q, cur);
        int cr = cur / C, cc = cur % C;
        for (int d = 0; d < 4; ++d) {
            int nr = cr + DR[d], nc = cc + DC[d];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
            if (seen[nr][nc]) continue;
            CQ_EnQueue(Q, nr * C + nc); seen[nr][nc] = true; enq++;
        }
    }
    CQ_Destroy(Q);
    return enq;
}

// 版本二：出队时才标记；出队遇到已标记位置就跳过，否则标记并扩展
static int BfsMarkOnDequeue() {
    bool seen[R][C] = {};
    CircQueue Q = {}; CQ_Init(Q, R * C + 2);
    int enq = 0;
    CQ_EnQueue(Q, 0); enq++;                            // S 入队，但不标记
    while (!CQ_Empty(Q)) {
        int cur; CQ_DeQueue(Q, cur);
        int cr = cur / C, cc = cur % C;
        if (seen[cr][cc]) continue;                     // 已处理过（重复项）→ 跳过
        seen[cr][cc] = true;                            // 出队时才标记
        for (int d = 0; d < 4; ++d) {
            int nr = cr + DR[d], nc = cc + DC[d];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
            if (seen[nr][nc]) continue;                 // 仅凭“已标记”去重，挡不住仍在队列里的重复
            CQ_EnQueue(Q, nr * C + nc); enq++;
        }
    }
    CQ_Destroy(Q);
    return enq;
}

int main() {
    puts("=== 2×2 全通网格，S=(0,0) E=(1,1)，右下左上，运行至队空 ===");
    int e1 = BfsMarkOnEnqueue();
    int e2 = BfsMarkOnDequeue();
    printf("入队时标记：总入队次数 = %d（预期 4：S A B E 各一次）\n", e1);
    printf("出队时标记：总入队次数 = %d（预期 5：E 被 A、B 各加入一次）\n", e2);
    printf("\n结论：本课采用“成功入队时立即标记”，保证每格只入队一次；\n");
    printf("出队标记配合额外去重可以正确遍历，但会有重复队列项，不能一概称为必然错误。\n");
    return 0;
}

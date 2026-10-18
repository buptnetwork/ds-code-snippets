// maze_bfs.cpp：BFS 求迷宫最短路径 + 二叉树层序遍历（单元 8）。
// 主迷宫 4×5（1 为墙），S=(0,0)、E=(3,4)，四方向按“右、下、左、上”固定顺序检查。
// 队列复用本章循环队列（int），以 r*列数+c 编码格子；容量取网格总格数（每格至多入队一次）。
// 路径回溯用顺序栈反转——回扣第 1 次课进制转换的“顺序反转”用法。
#include <cstdio>
#include "../src/circqueue.h"
#include "../src/seqstack.h"

// #region maze
static const int R = 4, C = 5;
static const int MAZE[R][C] = {
    {0, 0, 1, 0, 0},   // r0:  S  .  #  .  .
    {1, 0, 1, 0, 1},   // r1:  #  .  #  .  #
    {0, 0, 0, 0, 1},   // r2:  .  .  .  .  #
    {0, 1, 1, 0, 0},   // r3:  .  #  #  .  E
};
static const int DR[4] = {0, 1, 0, -1};   // 右、下、左、上
static const int DC[4] = {1, 0, -1, 0};
// #endregion maze

// #region bfs-loop
// 完整遍历（不提前结束），填满 dist 与 prev；返回终点层号，-1 表示不可达。
int BfsShortest(int sr, int sc, int er, int ec, int dist[], int prev[]) {
    static bool seen[R][C];
    for (int r = 0; r < R; ++r) for (int c = 0; c < C; ++c) { seen[r][c] = false; dist[r*C+c] = -1; prev[r*C+c] = -1; }
    CircQueue Q = {};
    if (!CQ_Init(Q, R * C)) return -2;                  // 容量 = 总格数，初始化须检查
    int s = sr * C + sc;
    if (!CQ_EnQueue(Q, s)) { CQ_Destroy(Q); return -2; }
    seen[sr][sc] = true; dist[s] = 0;                   // 起点入队即标记
    while (!CQ_Empty(Q)) {
        int cur; CQ_DeQueue(Q, cur);
        int cr = cur / C, cc = cur % C;
        for (int d = 0; d < 4; ++d) {                   // 右、下、左、上
            int nr = cr + DR[d], nc = cc + DC[d];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;   // 越界
            if (MAZE[nr][nc] == 1) continue;                        // 墙
            if (seen[nr][nc]) continue;                             // 已发现
            int ni = nr * C + nc;
            if (!CQ_EnQueue(Q, ni)) { CQ_Destroy(Q); return -2; }   // 入队失败须检查并清理
            seen[nr][nc] = true;                                    // ★ 成功入队时立即标记
            dist[ni] = dist[cur] + 1;
            prev[ni] = cur;
        }
    }
    CQ_Destroy(Q);
    return dist[er * C + ec];
}
// #endregion bfs-loop

// #region levelorder
struct BNode { int val; BNode *left, *right; };
// 层序遍历就是树上的 BFS：队列存结点值（int 队列），byVal 映射回指针。
// 每个非根结点只有一个父结点，沿左右孩子扩展不会重复访问，故不需要 seen 标记。
void LevelOrder(BNode *root, BNode *byVal[], int cap) {
    CircQueue Q = {};
    CQ_Init(Q, cap);
    CQ_EnQueue(Q, root->val);
    printf("层序：");
    while (!CQ_Empty(Q)) {
        int v; CQ_DeQueue(Q, v);
        BNode *p = byVal[v];
        printf("%d ", v);
        if (p->left)  CQ_EnQueue(Q, p->left->val);
        if (p->right) CQ_EnQueue(Q, p->right->val);
    }
    printf("\n");
    CQ_Destroy(Q);
}
// #endregion levelorder

int main() {
    puts("=== 迷宫（# 为墙，S=(0,0)，E=(3,4)）===");
    for (int r = 0; r < R; ++r) {
        printf("  r%d ", r);
        for (int c = 0; c < C; ++c) printf("%c ", MAZE[r][c] ? '#' : ((r==0&&c==0)?'S':((r==3&&c==4)?'E':'.')));
        printf("\n");
    }

    int dist[R*C], prev[R*C];
    int d = BfsShortest(0, 0, 3, 4, dist, prev);
    printf("\nBFS 最短步数 dist[E] = %d（预期 7）\n", d);

    puts("\n=== 各格层号（= 到该格的最短步数；x 为墙）===");
    printf("      c0 c1 c2 c3 c4\n");
    for (int r = 0; r < R; ++r) {
        printf("  r%d  ", r);
        for (int c = 0; c < C; ++c) {
            if (MAZE[r][c]) printf(" x");
            else printf(" %d", dist[r*C+c]);
        }
        printf("\n");
    }

    // 沿前驱回溯得到倒序路径，用栈反转为正序（回扣第 1 次课进制转换）
    puts("\n=== 用栈反转前驱链，得到 S→E 正序路径 ===");
    SeqStack S = {};
    SS_Init(S, R * C);
    for (int cur = 3*C+4; cur != -1; cur = prev[cur]) SS_Push(S, cur);
    printf("路径：");
    int cell, nodes = 0;
    while (SS_Pop(S, cell)) {
        if (nodes++) printf("->");
        printf("(%d,%d)", cell / C, cell % C);
    }
    printf("\n步数 = %d（预期 7）\n", nodes - 1);
    SS_Destroy(S);

    // 层序遍历：1(2(4,5),3(-,6))
    puts("\n=== 二叉树层序遍历（第 6 章预告）===");
    BNode n1{1,nullptr,nullptr}, n2{2,nullptr,nullptr}, n3{3,nullptr,nullptr},
          n4{4,nullptr,nullptr}, n5{5,nullptr,nullptr}, n6{6,nullptr,nullptr};
    n2.left=&n4; n2.right=&n5; n3.right=&n6; n1.left=&n2; n1.right=&n3;
    BNode *byVal[7] = {nullptr,&n1,&n2,&n3,&n4,&n5,&n6};
    LevelOrder(&n1, byVal, 7);
    printf("预期：1 2 3 4 5 6\n");
    return 0;
}

#ifndef CH02_LINKALGO_H
#define CH02_LINKALGO_H
#include "linklist.h"
// 所有链接有效，执行期间结点存活且链接不被并发修改。
// 除判环/入环点外，普通算法要求无环；表对象须已成功初始化。
LNode *FindMid(const LinkList &L); // 偶数长度返回偏右中点。
LNode *FindKthFromEnd(const LinkList &L, int k);
LNode *HasCycle(LNode *first); // 返回相遇结点，不一定是入口。
LNode *FindCycleEntry(LNode *first);
// 反转须独占修改权，返回新的首元结点；递归版另用 O(n) 栈。
LNode *Reverse(LNode *first);
LNode *ReverseRec(LNode *first);
// 两链无环、非递减、不共享结点；数据结点所有权交给结果。
LNode *Merge(LNode *a, LNode *b);
bool MergeInto(LinkList &A, LinkList &B); // B 保留独立空头结点。
// 两链可共享尾部，仅借用读取；长度各不超过 INT_MAX。
LNode *GetIntersection(LNode *a, LNode *b);
LNode *GetIntersectionSwitch(LNode *a, LNode *b);
#endif

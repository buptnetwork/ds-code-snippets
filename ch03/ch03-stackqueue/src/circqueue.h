#ifndef CH03_CIRCQUEUE_H
#define CH03_CIRCQUEUE_H
#include "../include/elemtype.h"

// 循环队列：定长数组 + front/rear/size，下标取模回绕。
// 本课程实现采用“方案二：size 计数器”，与第 2 章顺序表、本章顺序栈的 top 一脉相承：
//   front = 队头元素下标；rear = 队尾元素的下一个位置；size = 当前元素个数。
// 判空 size==0、判满 size==capacity，可用容量为 capacity（全用满），不需要牺牲一格。
// 仅用于单线程；对象先置零并成功初始化，不浅拷贝或重复初始化资源所有者。
// #region representation
typedef struct {
    ElemType *data;
    int front;          // 队头元素下标
    int rear;           // 队尾元素的下一个位置
    int size;           // 当前元素个数（方案二）
    int capacity;
} CircQueue;
// #endregion representation

// 错误返回约定沿用全章：EnQueue 满或内存不足返回 false 且原队不变；
// DeQueue 空队返回 false 且 out 不变；空队访问 Head 属编程错误，打印诊断后 exit(1)。
// DeQueue 的 out 不得与队列内元素别名。
// #region interface
bool CQ_Init(CircQueue &Q, int cap);
void CQ_Destroy(CircQueue &Q);
void CQ_Clear(CircQueue &Q);

bool CQ_Empty (const CircQueue &Q);        // 常量引用：只读
bool CQ_Full  (const CircQueue &Q);
int  CQ_Length(const CircQueue &Q);

bool CQ_EnQueue(CircQueue &Q, const ElemType &e);   // 引用形参：要改 Q
bool CQ_DeQueue(CircQueue &Q, ElemType &out);       // out 带回队头元素
ElemType &CQ_Head(CircQueue &Q);                    // 返回引用：可读可写队头
const ElemType &CQ_Head(const CircQueue &Q);

// 调试打印：同时给出“数组视图（按物理下标，未用格打 _）”与“队列视图（按逻辑顺序）”。
void CQ_Print(const char *tag, const CircQueue &Q);
// #endregion interface
#endif

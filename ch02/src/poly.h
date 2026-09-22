#ifndef CH02_POLY_H
#define CH02_POLY_H
// #region node
typedef struct PNode {
    double coef;
    int expn;
    PNode *next;
} PNode;
// #endregion node
struct Term { double coef; int expn; };
// 仅初始化未拥有结点的指针；输入指数非负严格递增，系数有限且绝对值<=1e6。
// 丢弃 |coef|<1e-9 的项；失败返回 false，p 为 NULL，已申请部分清理。
bool PolyCreate(PNode *&p, const Term terms[], int n);
void PolyDestroy(PNode *&p);
// 两个独立头结点，链无环、不共享；无近零项，系数满足上述教学尺度。
// 结果归A；B部分结点转给A、其余释放，B头释放并置pb=NULL。
void PolyAdd(PNode *pa, PNode *&pb);
#endif

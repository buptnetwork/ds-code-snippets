#include "poly.h"
#include <cmath>
#include <cstdlib>
#ifdef CH02_TEST_ALLOC
#include "../demo/alloc_control.h"
#endif

bool PolyCreate(PNode *&p, const Term terms[], int n) {
    p = NULL;
    if (n < 0 || (n > 0 && !terms)) return false;
    for (int i = 0; i < n; ++i) {
        if (terms[i].expn < 0 || (i && terms[i - 1].expn >= terms[i].expn)) return false;
        if (!std::isfinite(terms[i].coef) || std::fabs(terms[i].coef) > 1e6) return false;
    }
    p = (PNode *)malloc(sizeof(PNode));
    if (!p) return false;
    p->next = NULL;
    PNode *tail = p;
    for (int i = 0; i < n; ++i) {
        if (std::fabs(terms[i].coef) < 1e-9) continue;
        PNode *s = (PNode *)malloc(sizeof(PNode));
        if (!s) { PolyDestroy(p); return false; }
        s->coef = terms[i].coef;
        s->expn = terms[i].expn;
        s->next = NULL;
        tail->next = s;
        tail = s;
    }
    return true;
}
void PolyDestroy(PNode *&p) {
    while (p) {
        PNode *next = p->next;
        free(p);
        p = next;
    }
}

// #region add-order
void PolyAdd(PNode *pa, PNode *&pb) {
    const double eps = 1e-9;
    PNode *p = pa->next, *q = pb->next;
    PNode *pre = pa;
    while (p && q) {
        if (p->expn < q->expn) {
            pre = p;
            p = p->next;
        } else if (p->expn > q->expn) {
            PNode *t = q->next;
            q->next = p;
            pre->next = q;
            pre = q;
            q = t;
// #endregion add-order
// #region add-equal
        } else {
            p->coef += q->coef;
            if (std::fabs(p->coef) < eps) {
                pre->next = p->next;
                free(p);
                p = pre->next;
            } else {
                pre = p;
                p = p->next;
            }
            PNode *t = q->next;
            free(q);
            q = t;
        }
    }
// #endregion add-equal
// #region add-finish
    if (q) pre->next = q;
    free(pb);
    pb = NULL;
}
// #endregion add-finish

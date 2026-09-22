#include "../src/poly.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <initializer_list>
#include <limits>
#include "alloc_control.h"

static void Expect(PNode *p, std::initializer_list<Term> expected) {
    assert(p);
    p = p->next;
    for (Term t : expected) {
        assert(p && p->expn == t.expn && std::fabs(p->coef - t.coef) < 1e-9);
        p = p->next;
    }
    assert(!p);
}
int main() {
    Term a[] = {{3, 0}, {2, 2}, {5, 8}};
    Term b[] = {{-2, 2}, {4, 5}, {1, 8}};
    PNode *pa, *pb;
    assert(PolyCreate(pa, a, 3));
    assert(PolyCreate(pb, b, 3));
    PNode *moved = pb->next->next;
    assert(liveBlocks == 8);
    PolyAdd(pa, pb);
    Expect(pa, {{3, 0}, {4, 5}, {6, 8}});
    assert(!pb && pa->next->next == moved && liveBlocks == 4);
    for (PNode *p = pa->next; p; p = p->next)
        std::printf("%s%gx^%d", p == pa->next ? "" : " + ", p->coef, p->expn);
    std::printf("\npb为空=%d；剩余块=%ld（A头+3项）\n", pb == NULL, liveBlocks);
    PolyDestroy(pa); PolyDestroy(pb);
    assert(liveBlocks == 0);

    Term cancel[] = {{-3, 0}, {-2, 2}, {-5, 8}};
    assert(PolyCreate(pa, a, 3) && PolyCreate(pb, cancel, 3));
    PolyAdd(pa, pb); Expect(pa, {}); assert(!pb);
    PolyDestroy(pa);
    for (int side = 0; side < 2; ++side) {
        assert(PolyCreate(pa, a, side ? 3 : 0));
        assert(PolyCreate(pb, a, side ? 0 : 3));
        PolyAdd(pa, pb); Expect(pa, {{3, 0}, {2, 2}, {5, 8}});
        assert(!pb); PolyDestroy(pa);
    }
    assert(PolyCreate(pa, NULL, 0) && PolyCreate(pb, NULL, 0));
    PolyAdd(pa, pb); Expect(pa, {}); PolyDestroy(pa);
    Term small[] = {{1e-10, 0}, {1, 2}};
    assert(PolyCreate(pa, small, 2)); Expect(pa, {{1, 2}}); PolyDestroy(pa);
    Term near[] = {{-1 + 1e-10, 2}};
    assert(PolyCreate(pa, small, 2) && PolyCreate(pb, near, 1));
    PolyAdd(pa, pb); Expect(pa, {}); PolyDestroy(pa);
    Term invalid[] = {{std::numeric_limits<double>::infinity(), 0}};
    assert(!PolyCreate(pa, invalid, 1) && !pa);
    for (int budget = 0; budget < 4; ++budget) {
        allocationBudget = budget;
        assert(!PolyCreate(pa, a, 3) && !pa && liveBlocks == 0);
    }
    allocationBudget = -1;
    assert(liveBlocks == 0);
    std::puts("多项式：三分支、相消、空式、近零规则、失败清理检查通过");
}

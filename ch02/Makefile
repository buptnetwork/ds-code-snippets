CXX := c++
CXXFLAGS := -std=c++17 -g -Wall -Wextra -Wpedantic
OUT := .build.local

.PHONY: all lesson1 lesson2 lesson3 check
all: lesson1 lesson2 lesson3
lesson1: $(OUT)/demo_basic $(OUT)/test_seqlist $(OUT)/insert_bug
lesson2: $(OUT)/demo_link $(OUT)/demo_memory $(OUT)/sizeof_probe $(OUT)/test_linklist $(OUT)/list_bug
lesson3: $(OUT)/demo_cycle $(OUT)/demo_reverse $(OUT)/test_linkalgo $(OUT)/demo_poly $(OUT)/demo_lru $(OUT)/demo_locality

$(OUT):
	mkdir -p $@

$(OUT)/demo_basic: demo/demo_basic.cpp src/seqlist.cpp src/seqlist.h include/elemtype.h | $(OUT)
	$(CXX) $(CXXFLAGS) demo/demo_basic.cpp src/seqlist.cpp -o $@

$(OUT)/test_seqlist: demo/test_seqlist.cpp src/seqlist.cpp src/seqlist.h demo/alloc_control.h include/elemtype.h | $(OUT)
	$(CXX) $(CXXFLAGS) -DCH02_TEST_ALLOC demo/test_seqlist.cpp src/seqlist.cpp -o $@

$(OUT)/insert_bug: bugs/insert_bug.cpp | $(OUT)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OUT)/demo_link: demo/demo_link.cpp src/linklist.cpp src/linklist.h include/elemtype.h | $(OUT)
	$(CXX) $(CXXFLAGS) demo/demo_link.cpp src/linklist.cpp -o $@

$(OUT)/demo_memory: demo/demo_memory.cpp src/linklist.cpp src/linklist.h include/elemtype.h | $(OUT)
	$(CXX) $(CXXFLAGS) demo/demo_memory.cpp src/linklist.cpp -o $@

$(OUT)/sizeof_probe: demo/sizeof_probe.cpp src/linklist.h src/dlist.h include/elemtype.h | $(OUT)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OUT)/test_linklist: demo/test_linklist.cpp src/linklist.cpp src/dlist.cpp src/linklist.h src/dlist.h demo/alloc_control.h include/elemtype.h | $(OUT)
	$(CXX) $(CXXFLAGS) -DCH02_TEST_ALLOC demo/test_linklist.cpp src/linklist.cpp src/dlist.cpp -o $@

$(OUT)/list_bug: bugs/list_bug.cpp src/linklist.h include/elemtype.h | $(OUT)
	$(CXX) $(CXXFLAGS) $< -o $@

ALGO_SRC := src/linkalgo.cpp src/linklist.cpp
ALGO_HDR := src/linkalgo.h src/linklist.h include/elemtype.h

$(OUT)/demo_cycle: demo/demo_cycle.cpp $(ALGO_SRC) $(ALGO_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/demo_cycle.cpp $(ALGO_SRC) -o $@

$(OUT)/demo_reverse: demo/demo_reverse.cpp $(ALGO_SRC) $(ALGO_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/demo_reverse.cpp $(ALGO_SRC) -o $@

$(OUT)/test_linkalgo: demo/test_linkalgo.cpp $(ALGO_SRC) $(ALGO_HDR) demo/alloc_control.h | $(OUT)
	$(CXX) $(CXXFLAGS) -DCH02_TEST_ALLOC demo/test_linkalgo.cpp $(ALGO_SRC) -o $@

$(OUT)/demo_poly: demo/demo_poly.cpp src/poly.cpp src/poly.h demo/alloc_control.h | $(OUT)
	$(CXX) $(CXXFLAGS) -DCH02_TEST_ALLOC demo/demo_poly.cpp src/poly.cpp -o $@

$(OUT)/demo_lru: demo/demo_lru.cpp src/lru.cpp src/dlist.cpp src/lru.h src/dlist.h include/elemtype.h demo/alloc_control.h | $(OUT)
	$(CXX) $(CXXFLAGS) -DCH02_TEST_ALLOC demo/demo_lru.cpp src/lru.cpp src/dlist.cpp -o $@

$(OUT)/demo_locality: demo/demo_locality.cpp src/linklist.h include/elemtype.h | $(OUT)
	$(CXX) -std=c++17 -O2 -Wall -Wextra -Wpedantic $< -o $@

# 不定义NDEBUG：测试中的assert同时驱动操作。check不运行性能测量或缺陷函数。
check: all
	$(OUT)/test_seqlist
	$(OUT)/insert_bug
	$(OUT)/test_linklist
	$(OUT)/test_linkalgo
	$(OUT)/demo_cycle
	$(OUT)/demo_reverse
	$(OUT)/demo_poly
	$(OUT)/demo_lru
	$(OUT)/demo_locality --check

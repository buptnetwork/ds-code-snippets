CXX := c++
CXXFLAGS := -std=c++17 -g -Wall -Wextra -Wpedantic
OUT := .build.local

SEQ_SRC := src/seqstack.cpp
SEQ_HDR := src/seqstack.h include/elemtype.h
LINK_SRC := src/linkstack.cpp
LINK_HDR := src/linkstack.h include/elemtype.h

# 第 2 次课（栈的应用与递归）新增源文件
EXPR_SRC := src/expr.cpp
EXPR_HDR := src/expr.h
SD_SRC := src/stack_double.cpp
SD_HDR := src/stack_double.h
SC_SRC := src/stack_char.cpp
SC_HDR := src/stack_char.h

# 第 3 次课（队列及其应用）新增源文件
CQ_SRC := src/circqueue.cpp
CQ_HDR := src/circqueue.h include/elemtype.h
LQ_SRC := src/linkqueue.cpp
LQ_HDR := src/linkqueue.h include/elemtype.h
DQ_SRC := src/deque.cpp
DQ_HDR := src/deque.h include/elemtype.h
PQ_HDR := src/pqueue.h include/elemtype.h   # 仅接口、无实现，不参与任何二进制

.PHONY: all lesson1 lesson2 lesson3 asan check clean
all: lesson1 lesson2 lesson3

# 本次课（第 1 次课）目标：正确实现三个演示 + 三个预期错误反例
lesson1: $(OUT)/demo_stack $(OUT)/bracket $(OUT)/baseconv \
         $(OUT)/bracket_bug $(OUT)/baseconv_bug $(OUT)/pushorder_bug

$(OUT):
	mkdir -p $@

$(OUT)/demo_stack: demo/demo_stack.cpp $(SEQ_SRC) $(SEQ_HDR) $(LINK_SRC) $(LINK_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/demo_stack.cpp $(SEQ_SRC) $(LINK_SRC) -o $@

$(OUT)/bracket: demo/bracket.cpp $(SEQ_SRC) $(SEQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/bracket.cpp $(SEQ_SRC) -o $@

$(OUT)/baseconv: demo/baseconv.cpp $(SEQ_SRC) $(SEQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/baseconv.cpp $(SEQ_SRC) -o $@

$(OUT)/bracket_bug: bugs/bracket_bug.cpp $(SEQ_SRC) $(SEQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) bugs/bracket_bug.cpp $(SEQ_SRC) -o $@

$(OUT)/baseconv_bug: bugs/baseconv_bug.cpp $(SEQ_SRC) $(SEQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) bugs/baseconv_bug.cpp $(SEQ_SRC) -o $@

# pushorder_bug 只依赖头文件（自带缺陷版 Push）；main 仅做常数步自环检测，不遍历、不会挂起。
$(OUT)/pushorder_bug: bugs/pushorder_bug.cpp $(LINK_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) bugs/pushorder_bug.cpp -o $@

# 第 2 次课目标：表达式求值/转换演示 + 单调栈 + 递归演示 + 两个预期错误反例
lesson2: $(OUT)/expr_demo $(OUT)/monostack $(OUT)/recursion_demo $(OUT)/hanoi \
         $(OUT)/evalpostfix_bug $(OUT)/infix_bug

$(OUT)/expr_demo: demo/expr_demo.cpp $(EXPR_SRC) $(EXPR_HDR) $(SD_SRC) $(SD_HDR) $(SC_SRC) $(SC_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/expr_demo.cpp $(EXPR_SRC) $(SD_SRC) $(SC_SRC) -o $@

$(OUT)/monostack: demo/monostack.cpp $(SEQ_SRC) $(SEQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/monostack.cpp $(SEQ_SRC) -o $@

# recursion_demo / hanoi 自包含，只依赖标准库
$(OUT)/recursion_demo: demo/recursion_demo.cpp | $(OUT)
	$(CXX) $(CXXFLAGS) demo/recursion_demo.cpp -o $@

$(OUT)/hanoi: demo/hanoi.cpp | $(OUT)
	$(CXX) $(CXXFLAGS) demo/hanoi.cpp -o $@

$(OUT)/evalpostfix_bug: bugs/evalpostfix_bug.cpp $(SD_SRC) $(SD_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) bugs/evalpostfix_bug.cpp $(SD_SRC) -o $@

$(OUT)/infix_bug: bugs/infix_bug.cpp $(SD_SRC) $(SD_HDR) $(SC_SRC) $(SC_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) bugs/infix_bug.cpp $(SD_SRC) $(SC_SRC) -o $@

# 第 3 次课目标：队列演示 + BFS + 单调队列 + 调度 + 双端队列 + 两个预期错误反例
lesson3: $(OUT)/queue_demo $(OUT)/maze_bfs $(OUT)/monoqueue $(OUT)/scheduler \
         $(OUT)/bfs_mark $(OUT)/deque_demo \
         $(OUT)/falseoverflow_bug $(OUT)/qlen_bug $(OUT)/linkqueue_rear_bug

$(OUT)/queue_demo: demo/queue_demo.cpp $(CQ_SRC) $(CQ_HDR) $(LQ_SRC) $(LQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/queue_demo.cpp $(CQ_SRC) $(LQ_SRC) -o $@

$(OUT)/maze_bfs: demo/maze_bfs.cpp $(CQ_SRC) $(CQ_HDR) $(SEQ_SRC) $(SEQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/maze_bfs.cpp $(CQ_SRC) $(SEQ_SRC) -o $@

$(OUT)/monoqueue: demo/monoqueue.cpp $(DQ_SRC) $(DQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/monoqueue.cpp $(DQ_SRC) -o $@

$(OUT)/scheduler: demo/scheduler.cpp $(CQ_SRC) $(CQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/scheduler.cpp $(CQ_SRC) -o $@

$(OUT)/bfs_mark: demo/bfs_mark.cpp $(CQ_SRC) $(CQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/bfs_mark.cpp $(CQ_SRC) -o $@

$(OUT)/deque_demo: demo/deque_demo.cpp $(DQ_SRC) $(DQ_HDR) | $(OUT)
	$(CXX) $(CXXFLAGS) demo/deque_demo.cpp $(DQ_SRC) -o $@

# falseoverflow_bug / qlen_bug / linkqueue_rear_bug 均自包含，只依赖标准库
$(OUT)/falseoverflow_bug: bugs/falseoverflow_bug.cpp | $(OUT)
	$(CXX) $(CXXFLAGS) bugs/falseoverflow_bug.cpp -o $@

$(OUT)/qlen_bug: bugs/qlen_bug.cpp | $(OUT)
	$(CXX) $(CXXFLAGS) bugs/qlen_bug.cpp -o $@

$(OUT)/linkqueue_rear_bug: bugs/linkqueue_rear_bug.cpp | $(OUT)
	$(CXX) $(CXXFLAGS) bugs/linkqueue_rear_bug.cpp -o $@

# 反例 3 是未定义行为，须用 AddressSanitizer 才能稳定复现 heap-use-after-free；
# 故不进默认 check，单独用 'make asan' 验证（前缀 - 忽略 ASan 的非零退出）。
asan: | $(OUT)
	$(CXX) -std=c++17 -g -fsanitize=address bugs/linkqueue_rear_bug.cpp -o $(OUT)/linkqueue_rear_bug_asan
	-$(OUT)/linkqueue_rear_bug_asan

# 运行全部演示：正确实现应给出单元 0 约定的输出，反例应给出各自标注的错误结论。
check: lesson1 lesson2 lesson3
	$(OUT)/demo_stack
	$(OUT)/bracket
	$(OUT)/baseconv
	$(OUT)/bracket_bug
	$(OUT)/baseconv_bug
	$(OUT)/pushorder_bug
	$(OUT)/expr_demo
	$(OUT)/monostack
	$(OUT)/recursion_demo
	$(OUT)/hanoi
	$(OUT)/evalpostfix_bug
	$(OUT)/infix_bug
	$(OUT)/queue_demo
	$(OUT)/maze_bfs
	$(OUT)/monoqueue
	$(OUT)/scheduler
	$(OUT)/bfs_mark
	$(OUT)/deque_demo
	$(OUT)/falseoverflow_bug
	$(OUT)/qlen_bug
	@echo "linkqueue_rear_bug 为未定义行为，请用 'make asan' 单独验证 heap-use-after-free"

clean:
	rm -rf $(OUT)

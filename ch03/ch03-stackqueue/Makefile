CXX := c++
CXXFLAGS := -std=c++17 -g -Wall -Wextra -Wpedantic
OUT := .build.local

SEQ_SRC := src/seqstack.cpp
SEQ_HDR := src/seqstack.h include/elemtype.h
LINK_SRC := src/linkstack.cpp
LINK_HDR := src/linkstack.h include/elemtype.h

.PHONY: all lesson1 check clean
all: lesson1

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

# 运行全部演示：正确实现应给出单元 0 约定的输出，反例应给出各自标注的错误结论。
check: lesson1
	$(OUT)/demo_stack
	$(OUT)/bracket
	$(OUT)/baseconv
	$(OUT)/bracket_bug
	$(OUT)/baseconv_bug
	$(OUT)/pushorder_bug

clean:
	rm -rf $(OUT)

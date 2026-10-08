# 第 1 章 绪论 · 示例程序构建
# 用法：make 构建全部 / make check 运行自检 / make clean 清理
#
# 与 CMakeLists.txt 对齐：C++17、-Wall -Wextra（ch01 不加 -Wpedantic）、-g。
# 不定义 NDEBUG —— 这些示例用 assert 自检，Release 会让断言静默失效。
# 部分程序按源文件头注释指定优化档（-O0 / -O2），下面分组还原。
CXX      := c++
CC       := cc
CXXFLAGS := -std=c++17 -g -Wall -Wextra
CFLAGS   := -std=c11 -g -Wall -Wextra
OUT      := .build.local

# 单文件示例（各自带 main、只依赖系统头）——按优化档分组
DEFAULT := bincounter seq-search space-examples swap-three stress-test memory-insight
OPT_O2  := count-examples sorts dynarray-grow timing-framework
OPT_O0  := hoist-invariant          # 故意 -O0：保留重复 strlen 供讲解
# 反面教材：故意堆越界写（UB，供 ASan 现场演示），只构建、不纳入 check
UNSAFE  := mem-errors

ALL := $(OUT)/complex-opaque $(OUT)/complex-ref $(OUT)/algorithm-analysis \
       $(addprefix $(OUT)/,$(DEFAULT) $(OPT_O2) $(OPT_O0) $(UNSAFE))

# check 运行的安全程序（与 CMakeLists 的 CTest 用例一致，排除 mem-errors）
CHECK := complex-opaque complex-ref algorithm-analysis $(DEFAULT) $(OPT_O2) $(OPT_O0)

.PHONY: all check clean
all: $(ALL)

# ---- 多文件工程模板：不透明指针 vs 引用两种封装 ----
$(OUT)/complex-opaque: complex-opaque/complex.cpp complex-opaque/main.cpp complex-opaque/complex.h
	@mkdir -p $(OUT)
	$(CXX) $(CXXFLAGS) complex-opaque/complex.cpp complex-opaque/main.cpp -o $@

$(OUT)/complex-ref: complex-ref/complex.cpp complex-ref/main.cpp complex-ref/complex.h
	@mkdir -p $(OUT)
	$(CXX) $(CXXFLAGS) complex-ref/complex.cpp complex-ref/main.cpp -o $@

# ---- C 示例：走 C 编译器 ----
$(OUT)/algorithm-analysis: algorithm-analysis.c
	@mkdir -p $(OUT)
	$(CC) $(CFLAGS) algorithm-analysis.c -o $@

# ---- 单文件示例：默认档（无 -O，等价 -O0）----
$(addprefix $(OUT)/,$(DEFAULT) $(UNSAFE)): $(OUT)/%: %.cpp
	@mkdir -p $(OUT)
	$(CXX) $(CXXFLAGS) $< -o $@

# ---- 单文件示例：-O2 档 ----
$(addprefix $(OUT)/,$(OPT_O2)): $(OUT)/%: %.cpp
	@mkdir -p $(OUT)
	$(CXX) $(CXXFLAGS) -O2 $< -o $@

# ---- 单文件示例：-O0 档 ----
$(addprefix $(OUT)/,$(OPT_O0)): $(OUT)/%: %.cpp
	@mkdir -p $(OUT)
	$(CXX) $(CXXFLAGS) -O0 $< -o $@

# 逐个运行安全示例；任一非零退出即失败。mem-errors 有意排除（会崩溃）。
check: all
	@for b in $(CHECK); do \
		printf '== %s ==\n' "$$b"; \
		$(OUT)/$$b || exit 1; \
	done
	@printf 'ch01: %s 个自检全部通过\n' "$(words $(CHECK))"

clean:
	rm -rf $(OUT)

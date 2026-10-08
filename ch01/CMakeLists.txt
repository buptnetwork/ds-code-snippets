cmake_minimum_required(VERSION 3.16)
project(ch01_snippets C CXX)

# 与 Makefile 保持一致：C++17 + -Wall -Wextra（-g 由 Debug 构建类型自动加入）。
# 注意：ch01 的 Makefile 未使用 -Wpedantic，也未定义任何编译宏，这里如实还原。
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

# 所有可执行文件统一输出到 <build>/bin，方便在 CLion 里定位。
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 定义一个示例目标的辅助函数：
#   add_snippet(<目标名> SRC <源文件...> [OPT <优化档>])
#   OPT -> 追加优化选项，用于还原各源文件头注释指定的编译档位（-O0 / -O2）。
function(add_snippet target)
  cmake_parse_arguments(ARG "" "OPT" "SRC" ${ARGN})
  add_executable(${target} ${ARG_SRC})
  if(ARG_OPT)
    target_compile_options(${target} PRIVATE ${ARG_OPT})
  endif()
endfunction()

# ---- 多文件工程模板（实验课细讲）：不透明指针 vs 引用两种封装 ----
# Makefile 的 demo 目标 = complex.o + main.o；下面两个子目录各是一套完整实现。
add_snippet(complex-opaque SRC complex-opaque/complex.cpp complex-opaque/main.cpp)
add_snippet(complex-ref    SRC complex-ref/complex.cpp complex-ref/main.cpp)

# ---- 单文件示例程序（各自带 main，只依赖系统头，无跨文件依赖）----
add_snippet(algorithm-analysis SRC algorithm-analysis.c)          # C 文件，走 C 编译器
add_snippet(bincounter         SRC bincounter.cpp)
add_snippet(count-examples     SRC count-examples.cpp   OPT -O2)  # 头注释指定 -O2
add_snippet(seq-search         SRC seq-search.cpp)
add_snippet(space-examples     SRC space-examples.cpp)
add_snippet(swap-three         SRC swap-three.cpp)
add_snippet(stress-test        SRC stress-test.cpp)
add_snippet(memory-insight     SRC memory-insight.cpp)
add_snippet(sorts              SRC sorts.cpp            OPT -O2)  # 头注释指定 -O2
add_snippet(hoist-invariant    SRC hoist-invariant.cpp  OPT -O0)  # 头注释：故意 -O0 保留重复 strlen
add_snippet(dynarray-grow      SRC dynarray-grow.cpp    OPT -O2)  # 头注释指定 -O2
add_snippet(timing-framework   SRC timing-framework.cpp OPT -O2)  # 头注释指定 -O2

# 反面教材：mem-errors.cpp 的 main 会做堆越界写（未定义行为），仅供 ASan 现场演示。
# 照常编译成目标方便查看，但【不登记为测试】，避免普通构建下运行崩溃。
add_snippet(mem-errors         SRC mem-errors.cpp)

# ---- CTest：Makefile 本身没有 check 目标，这里把可安全运行、退出码为 0 的程序登记为用例 ----
# 重要：这些程序用 assert 驱动自检，请用 Debug 构建（不要定义 NDEBUG，否则断言失效、测试形同虚设）。
enable_testing()
add_test(NAME complex-opaque     COMMAND complex-opaque)
add_test(NAME complex-ref        COMMAND complex-ref)
add_test(NAME algorithm-analysis COMMAND algorithm-analysis)
add_test(NAME bincounter         COMMAND bincounter)
add_test(NAME count-examples     COMMAND count-examples)
add_test(NAME seq-search         COMMAND seq-search)
add_test(NAME space-examples     COMMAND space-examples)
add_test(NAME swap-three         COMMAND swap-three)
add_test(NAME stress-test        COMMAND stress-test)
add_test(NAME memory-insight     COMMAND memory-insight)
add_test(NAME sorts              COMMAND sorts)
add_test(NAME hoist-invariant    COMMAND hoist-invariant)
add_test(NAME dynarray-grow      COMMAND dynarray-grow)
add_test(NAME timing-framework   COMMAND timing-framework)
# mem-errors 有意排除（见上方说明）

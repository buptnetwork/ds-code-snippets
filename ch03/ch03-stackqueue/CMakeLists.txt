cmake_minimum_required(VERSION 3.16)
project(ch03_snippets CXX)

# 与 Makefile 保持一致：C++17、严格告警。
# -g 由 CMake 的构建类型（默认 Debug）自动加入，无需在此重复。
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

# 可选：用 AddressSanitizer 复现 Makefile 的 `make asan`。
# 默认 OFF；在 CLion 的 Settings → Build → CMake 选项里加 -DCH03_ENABLE_ASAN=ON，
# 即可对所有目标启用 ASan，用来稳定观察 linkqueue_rear_bug 的 heap-use-after-free。
option(CH03_ENABLE_ASAN "Build with AddressSanitizer (reproduce `make asan`)" OFF)
if(CH03_ENABLE_ASAN)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address)
endif()

# 所有可执行文件统一输出到 <build>/bin，方便在 CLion 里定位。
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 源码内部一律用相对路径 include（../src、../include），无需额外包含目录。
# 这里把 include/ 暴露出来仅用于 IDE 索引，可选。
include_directories(${CMAKE_SOURCE_DIR}/include)

# 定义一个示例目标的辅助函数：add_snippet(<目标名> SRC <源文件...>)
# ch03 的 Makefile 未使用任何编译宏或单独优化档，故无需额外开关。
function(add_snippet target)
  cmake_parse_arguments(ARG "" "" "SRC" ${ARGN})
  add_executable(${target} ${ARG_SRC})
endfunction()

# 说明：src/pqueue.h 仅接口、无实现，不参与任何二进制（与 Makefile 一致）。

# ---- 第 1 次课：栈（正确实现 3 个 + 预期错误反例 3 个）----
add_snippet(demo_stack   SRC demo/demo_stack.cpp src/seqstack.cpp src/linkstack.cpp)
add_snippet(bracket      SRC demo/bracket.cpp    src/seqstack.cpp)
add_snippet(baseconv     SRC demo/baseconv.cpp   src/seqstack.cpp)
add_snippet(bracket_bug  SRC bugs/bracket_bug.cpp  src/seqstack.cpp)
add_snippet(baseconv_bug SRC bugs/baseconv_bug.cpp src/seqstack.cpp)
# pushorder_bug 只依赖头文件（自带缺陷版 Push）；main 仅做常数步自环检测，不遍历、不会挂起。
add_snippet(pushorder_bug SRC bugs/pushorder_bug.cpp)

# ---- 第 2 次课：栈的应用与递归（演示 4 个 + 预期错误反例 2 个）----
add_snippet(expr_demo       SRC demo/expr_demo.cpp src/expr.cpp src/stack_double.cpp src/stack_char.cpp)
add_snippet(monostack       SRC demo/monostack.cpp src/seqstack.cpp)
add_snippet(recursion_demo  SRC demo/recursion_demo.cpp)   # 自包含，只依赖标准库
add_snippet(hanoi           SRC demo/hanoi.cpp)           # 自包含，只依赖标准库
add_snippet(evalpostfix_bug SRC bugs/evalpostfix_bug.cpp src/stack_double.cpp)
add_snippet(infix_bug       SRC bugs/infix_bug.cpp src/stack_double.cpp src/stack_char.cpp)

# ---- 第 3 次课：队列及其应用（演示 6 个 + 预期错误反例 3 个）----
add_snippet(queue_demo SRC demo/queue_demo.cpp src/circqueue.cpp src/linkqueue.cpp)
add_snippet(maze_bfs   SRC demo/maze_bfs.cpp   src/circqueue.cpp src/seqstack.cpp)
add_snippet(monoqueue  SRC demo/monoqueue.cpp  src/deque.cpp)
add_snippet(scheduler  SRC demo/scheduler.cpp  src/circqueue.cpp)
add_snippet(bfs_mark   SRC demo/bfs_mark.cpp   src/circqueue.cpp)
add_snippet(deque_demo SRC demo/deque_demo.cpp src/deque.cpp)
add_snippet(falseoverflow_bug SRC bugs/falseoverflow_bug.cpp)   # 自包含，只依赖标准库
add_snippet(qlen_bug          SRC bugs/qlen_bug.cpp)            # 自包含，只依赖标准库
# linkqueue_rear_bug 为未定义行为（heap-use-after-free），须用 ASan 才能稳定复现。
# 照常编译成目标方便查看，但【不登记为测试】，避免普通构建下运行崩溃；
# 需复现时启用 -DCH03_ENABLE_ASAN=ON 再运行该目标。
add_snippet(linkqueue_rear_bug SRC bugs/linkqueue_rear_bug.cpp)

# ---- 还原 Makefile 的 `check` 目标，交给 CTest / CLion 测试运行器 ----
# check 运行除 linkqueue_rear_bug 外的全部 20 个程序：正确实现给出单元约定的输出，
# 反例给出各自标注的错误结论，均退出码 0，故逐一登记为用例。
enable_testing()
# 第 1 次课
add_test(NAME demo_stack   COMMAND demo_stack)
add_test(NAME bracket      COMMAND bracket)
add_test(NAME baseconv     COMMAND baseconv)
add_test(NAME bracket_bug  COMMAND bracket_bug)
add_test(NAME baseconv_bug COMMAND baseconv_bug)
add_test(NAME pushorder_bug COMMAND pushorder_bug)
# 第 2 次课
add_test(NAME expr_demo       COMMAND expr_demo)
add_test(NAME monostack       COMMAND monostack)
add_test(NAME recursion_demo  COMMAND recursion_demo)
add_test(NAME hanoi           COMMAND hanoi)
add_test(NAME evalpostfix_bug COMMAND evalpostfix_bug)
add_test(NAME infix_bug       COMMAND infix_bug)
# 第 3 次课
add_test(NAME queue_demo        COMMAND queue_demo)
add_test(NAME maze_bfs          COMMAND maze_bfs)
add_test(NAME monoqueue         COMMAND monoqueue)
add_test(NAME scheduler         COMMAND scheduler)
add_test(NAME bfs_mark          COMMAND bfs_mark)
add_test(NAME deque_demo        COMMAND deque_demo)
add_test(NAME falseoverflow_bug COMMAND falseoverflow_bug)
add_test(NAME qlen_bug          COMMAND qlen_bug)
# linkqueue_rear_bug 有意排除（UB，须 ASan，见上方说明）

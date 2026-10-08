cmake_minimum_required(VERSION 3.16)
project(ch02_snippets CXX)

# 与 Makefile 保持一致：C++17、严格告警。
# -g 由 CMake 的构建类型（默认 Debug）自动加入，无需在此重复。
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

# 所有可执行文件统一输出到 <build>/bin，方便在 CLion 里定位。
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 源码内部一律用相对路径 include（../src、../include），无需额外包含目录。
# 这里把 include/ 暴露出来仅用于 IDE 索引，可选。
include_directories(${CMAKE_SOURCE_DIR}/include)

# 定义一个示例目标的辅助函数：
#   add_snippet(<目标名> SRC <源文件...> [TESTALLOC] [OPTIMIZE])
#   TESTALLOC -> 追加 -DCH02_TEST_ALLOC（测试用的分配控制）
#   OPTIMIZE  -> 追加 -O2（性能/局部性演示）
function(add_snippet target)
  cmake_parse_arguments(ARG "TESTALLOC;OPTIMIZE" "" "SRC" ${ARGN})
  add_executable(${target} ${ARG_SRC})
  if(ARG_TESTALLOC)
    target_compile_definitions(${target} PRIVATE CH02_TEST_ALLOC)
  endif()
  if(ARG_OPTIMIZE)
    target_compile_options(${target} PRIVATE -O2)
  endif()
endfunction()

# ---- 单元 1：顺序表 ----
add_snippet(demo_basic   SRC demo/demo_basic.cpp src/seqlist.cpp)
add_snippet(test_seqlist SRC demo/test_seqlist.cpp src/seqlist.cpp TESTALLOC)
add_snippet(insert_bug   SRC bugs/insert_bug.cpp)

# ---- 单元 2：链表 ----
add_snippet(demo_link     SRC demo/demo_link.cpp src/linklist.cpp)
add_snippet(demo_memory   SRC demo/demo_memory.cpp src/linklist.cpp)
add_snippet(sizeof_probe  SRC demo/sizeof_probe.cpp)
add_snippet(test_linklist SRC demo/test_linklist.cpp src/linklist.cpp src/dlist.cpp TESTALLOC)
add_snippet(list_bug      SRC bugs/list_bug.cpp)

# ---- 单元 3：链表算法与应用 ----
add_snippet(demo_cycle    SRC demo/demo_cycle.cpp src/linkalgo.cpp src/linklist.cpp)
add_snippet(demo_reverse  SRC demo/demo_reverse.cpp src/linkalgo.cpp src/linklist.cpp)
add_snippet(test_linkalgo SRC demo/test_linkalgo.cpp src/linkalgo.cpp src/linklist.cpp TESTALLOC)
add_snippet(demo_poly     SRC demo/demo_poly.cpp src/poly.cpp TESTALLOC)
add_snippet(demo_lru      SRC demo/demo_lru.cpp src/lru.cpp src/dlist.cpp TESTALLOC)
add_snippet(demo_locality SRC demo/demo_locality.cpp OPTIMIZE)

# ---- 还原 Makefile 的 `check` 目标，交给 CTest / CLion 测试运行器 ----
enable_testing()
add_test(NAME test_seqlist      COMMAND test_seqlist)
add_test(NAME insert_bug        COMMAND insert_bug)
add_test(NAME test_linklist     COMMAND test_linklist)
add_test(NAME test_linkalgo     COMMAND test_linkalgo)
add_test(NAME demo_cycle        COMMAND demo_cycle)
add_test(NAME demo_reverse      COMMAND demo_reverse)
add_test(NAME demo_poly         COMMAND demo_poly)
add_test(NAME demo_lru          COMMAND demo_lru)
add_test(NAME demo_locality_check COMMAND demo_locality --check)

# 数据结构课程 · 示例代码（学生版）

本仓库是《数据结构》课程的配套示例代码，从课件工程中独立出来，方便单独获取、编译与运行。
每章一个目录，均为**独立工程**，请分别打开、分别构建。

## 目录结构

```
.
├── ch01/   第 1 章 绪论：算法分析与复杂度（C / C++ 单文件示例 + 多文件工程模板）
└── ch02/   第 2 章 线性表：顺序表、链表、链表算法与应用（C++17 多目标工程）
```

- **ch01**：根目录下每个 `.cpp` / `.c` 都是各自带 `main`、只依赖系统头文件的独立小程序；
  另含 `complex-opaque/`、`complex-ref/` 两套「多文件工程」示例（不透明指针 vs 引用两种封装）。
- **ch02**：`src/` 是数据结构实现，`include/` 是公共类型，`demo/` 是演示与测试程序，`bugs/` 是反面教材。

## 环境准备

| 工具 | 要求 | 检查命令 |
|---|---|---|
| C++ 编译器 | 支持 C++17（g++ 或 clang++） | `g++ --version` / `clang++ --version` |
| CMake | ≥ 3.16 | `cmake --version` |
| make | 任意（仅 ch02 命令行构建需要） | `make --version` |

- macOS：安装 Xcode Command Line Tools（`xcode-select --install`）即可获得 clang++、make；CMake 可用 `brew install cmake`。
- Windows：建议安装 Visual Studio 或 MinGW-w64，并安装 CMake。

---

## 用 CLion 打开

> CLion 以 **CMake 工程**为单位。`snippets` 根目录没有顶层 `CMakeLists.txt`，
> **请分别打开 `ch01` 或 `ch02` 目录**，不要打开仓库根目录。

1. `File → Open…`，选择 `ch01/`（或 `ch02/`）目录，CLion 会自动识别其中的 `CMakeLists.txt` 并加载工程。
2. 首次打开时在弹窗中确认工具链（Toolchain），等待 CMake 配置完成。
3. **将构建类型设为 Debug**（右上角 `CMake`  profile → Build type: `Debug`）。
   > 这些示例大量使用 `assert` 自检；Release 会定义 `NDEBUG` 使断言失效，测试将形同虚设。
4. 在右上角目标下拉框选择要运行的可执行目标（如 `demo_basic`、`sorts`），点 ▶ 运行 / 🐞 调试。
5. 运行全部测试：菜单 `Run → Run All Tests`（工程已用 CTest 登记用例）。

---

## 用 VS Code 打开

推荐安装扩展：**C/C++**（Microsoft）与 **CMake Tools**（Microsoft）。

1. `File → Open Folder…`，选择 `ch01/`（或 `ch02/`）目录（同样**按章分别打开**）。
2. 打开后 CMake Tools 会检测到 `CMakeLists.txt`：
   - 底部状态栏选择一个 **Kit**（编译器）；
   - 选择构建变体 **Debug**（状态栏或命令面板 `CMake: Select Variant`）；
   - 状态栏的目标选择器可切换要构建/运行的可执行文件；
   - `F7` 或 `CMake: Build` 构建，🐞 图标调试当前目标。
3. 也可以完全用命令行（见下节），VS Code 仅作编辑器。

---

## 用命令行构建

### ch01（make 或 CMake 二选一）

方式 A —— Makefile：

```bash
cd ch01
make            # 构建全部 15 个示例，产物在 .build.local/
make check      # 运行 14 个自检程序（mem-errors 是故意的 UB 演示，不在其中）
make clean      # 清理
.build.local/sorts   # 运行某个示例
```

方式 B —— CMake（便于 CLion / VS Code）：

```bash
cd ch01
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug   # 配置
cmake --build build                            # 编译（产物在 build/bin/）
ctest --test-dir build --output-on-failure     # 运行全部测试
./build/bin/sorts                              # 运行某个示例
```

只想快速跑根目录下的某个单文件示例，也可直接编译（产物放进已被忽略的 .build.local/）：

```bash
cd ch01
mkdir -p .build.local
g++ -std=c++17 -Wall -Wextra -g count-examples.cpp -o .build.local/count-examples
./.build.local/count-examples
```

> **多文件工程示例**：`complex-opaque/`、`complex-ref/` 各带一个「十行模板」Makefile —— `cd` 进去后 `make` 生成 `demo`、`make run` 运行、`make clean` 清理（与课件页的统一编译命令一致）。根目录的综合 Makefile 也会把它们分别构建为 `.build.local/complex-opaque`、`.build.local/complex-ref`。

### ch02（make 或 CMake 二选一）

方式 A —— Makefile：

```bash
cd ch02
make            # 构建全部目标，产物在 .build.local/
make check      # 运行测试用例
make clean      # 清理
.build.local/demo_basic   # 运行某个演示
```

方式 B —— CMake（同 ch01，便于 CLion / VS Code）：

```bash
cd ch02
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## 重要提示

1. **务必用 Debug 构建**：示例依赖 `assert` 自检，Release/`NDEBUG` 会让断言静默失效。
2. **`ch01/mem-errors.cpp` 是故意的反面教材**：它的 `main` 会做堆越界写（未定义行为），
   仅供配合 AddressSanitizer 现场演示，正常运行会崩溃，属预期现象；它未登记为测试用例。
3. **构建产物不要提交**：仓库已附带 `.gitignore`，忽略 `build/`、`cmake-build-*/`、`.build.local/`、
   `*.o` 及各类可执行文件等。若你手动编译出无扩展名的可执行文件，请放进 `build/` 或自行加入 `.gitignore`。
4. **ch02 测试类目标**（`test_seqlist`、`test_linklist`、`test_linkalgo`、`demo_poly`、`demo_lru`）
   需要 `-DCH02_TEST_ALLOC` 编译宏；用上面的 Makefile 或 CMake 构建会自动加上，手动 `g++` 时请自行补 `-DCH02_TEST_ALLOC`。
5. `demo_locality` 以 `-O2` 构建、用 `--check` 参数运行（演示缓存局部性对性能的影响）。

## 常见问题

- **CLion 打开后没有目标 / 报找不到 `CMakeLists.txt`**：多半是打开了仓库根目录，请改为打开 `ch01` 或 `ch02`。
- **测试全部“通过”但没有实际校验**：检查是否误用了 Release 构建（`NDEBUG` 使 `assert` 失效），改回 Debug。

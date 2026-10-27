CXX := c++
CXXFLAGS := -std=c++17 -g -Wall -Wextra -Wpedantic
OUT := .build.local

.PHONY: all lesson1 run check sanitize
all: lesson1
lesson1: $(OUT)/demo_string $(OUT)/test_string

$(OUT):
	mkdir -p $@

$(OUT)/demo_string: demo_string.cpp hstring.cpp hstring.h | $(OUT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) demo_string.cpp hstring.cpp $(LDFLAGS) -o $@

$(OUT)/test_string: test_string.cpp hstring.cpp hstring.h | $(OUT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DCH04_TEST_ALLOC test_string.cpp hstring.cpp $(LDFLAGS) -o $@

run: $(OUT)/demo_string
	$(OUT)/demo_string

check: lesson1
	$(OUT)/test_string
	$(OUT)/demo_string

# 内存检测针对正常实现；任何非零退出都视为失败。
sanitize: | $(OUT)
	$(CXX) -std=c++17 -g -Wall -Wextra -Wpedantic -DCH04_TEST_ALLOC -fsanitize=address,undefined -fno-omit-frame-pointer test_string.cpp hstring.cpp -o $(OUT)/test_string_sanitize
	$(OUT)/test_string_sanitize

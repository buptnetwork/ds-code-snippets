CXX := c++
CXXFLAGS := -std=c++17 -g -Wall -Wextra -Wpedantic
OUT := .build.local

.PHONY: all lesson1 lesson2 lesson3 run check check-string check-match check-applications sanitize
all: lesson1 lesson2 lesson3
lesson1: $(OUT)/demo_string $(OUT)/test_string
lesson2: $(OUT)/demo_match $(OUT)/test_match
lesson3: $(OUT)/replace_demo $(OUT)/rk_demo

$(OUT)/replace_demo: replace_demo.cpp match_kmp.cpp match.h | $(OUT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) replace_demo.cpp match_kmp.cpp $(LDFLAGS) -o $@

$(OUT)/rk_demo: rk_demo.cpp | $(OUT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) rk_demo.cpp $(LDFLAGS) -o $@

check-applications: lesson3
	$(OUT)/replace_demo --check
	$(OUT)/rk_demo --check
	$(OUT)/replace_demo
	$(OUT)/rk_demo

$(OUT)/demo_match: demo_match.cpp match_bf.cpp match_kmp.cpp match.h | $(OUT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) demo_match.cpp match_bf.cpp match_kmp.cpp $(LDFLAGS) -o $@

$(OUT)/test_match: test_match.cpp match_bf.cpp match_kmp.cpp match.h | $(OUT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) test_match.cpp match_bf.cpp match_kmp.cpp $(LDFLAGS) -o $@

check-match: lesson2
	$(OUT)/test_match
	$(OUT)/demo_match

$(OUT):
	mkdir -p $@

$(OUT)/demo_string: demo_string.cpp hstring.cpp hstring.h | $(OUT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) demo_string.cpp hstring.cpp $(LDFLAGS) -o $@

$(OUT)/test_string: test_string.cpp hstring.cpp hstring.h | $(OUT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DCH04_TEST_ALLOC test_string.cpp hstring.cpp $(LDFLAGS) -o $@

run: $(OUT)/demo_string
	$(OUT)/demo_string

check: check-string check-match check-applications

check-string: lesson1
	$(OUT)/test_string
	$(OUT)/demo_string

# 内存检测针对正常实现；任何非零退出都视为失败。
sanitize: | $(OUT)
	$(CXX) -std=c++17 -g -Wall -Wextra -Wpedantic -DCH04_TEST_ALLOC -fsanitize=address,undefined -fno-omit-frame-pointer test_string.cpp hstring.cpp -o $(OUT)/test_string_sanitize
	$(OUT)/test_string_sanitize

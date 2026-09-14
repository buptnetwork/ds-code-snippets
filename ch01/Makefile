# 第 1 章 绪论 · 多文件工程模板（实验课细讲）
# 用法：make 生成 demo / make run 运行 / make clean 清理
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
OBJS     = complex.o main.o

demo: $(OBJS)
	$(CXX) $(OBJS) -o demo

%.o: %.cpp complex.h
	$(CXX) $(CXXFLAGS) -c $<

run: demo
	./demo

clean:
	rm -f *.o demo

.PHONY: run clean

# 第 1 章 绪论 · 多文件工程「十行模板」（实验课细讲）
# 在本目录下：make 生成 demo / make run 运行 / make clean 清理
# 与课件页的统一编译命令一致：g++ -std=c++17 -Wall -Wextra -g
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
	rm -rf demo.dSYM

.PHONY: run clean

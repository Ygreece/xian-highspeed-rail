CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = railway
SRCS = main.c graph.c dijkstra.c
OBJS = $(SRCS:.c=.o)

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 编译
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(OBJS) $(TARGET)

# 运行测试
test: $(TARGET)
	./$(TARGET) -a

# 显示帮助
help: $(TARGET)
	./$(TARGET) -h

# 列出站点
list: $(TARGET)
	./$(TARGET) -l

.PHONY: all clean test help list

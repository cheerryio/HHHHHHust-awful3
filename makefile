# 指令编译器和选项
CC=gcc
CFLAGS=-Wall
 
# 目标文件
TARGET=compi
SRCS := $(shell find $(SOURCEDIR) -name '*.c' | grep -v csrc.c)
 
INC = -I./src/ast -I./src/baselib
link = -lzlog -lm

OBJS = $(SRCS:.c=.o)
 
$(TARGET):$(OBJS)
#	@echo TARGET:$@
#	@echo OBJECTS:$^
	$(CC) -o $@ $^ $(link)
 
clean:
	rm -rf $(TARGET) $(OBJS)
 
%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<
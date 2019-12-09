DEBUG  	 := 1
CC       := clang
LINK     := clang
DEP      := Makefile.dep

CFLAGS   := -std=gnu99 -c
WARNING  := -Wall -Wno-parentheses -Wno-pointer-arith -Wno-unused -Wundef 
STRICT   := -Wfatal-errors -Werror 
OPTIM    := -O -DNDEBUG -fomit-frame-pointer -fno-stack-protector -ffast-math

ifeq ($(DEBUG), 1)
CFLAGS += $(WARNING) -g
else
CFLAGS += $(WARNING) $(OPTIM)
endif

PWD      := $(shell pwd)
TARGET   := pycom
SRCS     := $(wildcard *.c) $(wildcard utils/*.c)
OBJS     := $(subst .c,.o,$(SRCS))

.PHONY: $(TARGET)

$(TARGET): gram.tab.o $(OBJS)
	@echo "    LD -o" $(shell basename $@)
	@$(LINK) -o $@ $^

%.o: %.c
	@set -e
	@echo "    CC" $<
	@$(CC) $(CFLAGS) -o $@ $<

gram.tab.c: gram.y
	bison -d -t -v gram.y

dep:
	@rm -f $(DEP)
	@echo $(SRCS) | xargs -n 1 cpp -MM >>$(DEP)

run:
	./pycom ../tests/except/fin0.py

count:
	wc -l *.c *.h *.y utils/*.c utils/*.h

clean:
	@rm -f $(OBJS) $(DEP) $(TARGET) gram.tab.* gram.output core tags

ifeq ($(DEP), $(wildcard $(DEP)))
    include $(DEP)
endif

DEBUG  	 := 1
CC       := gcc
LINK     := gcc
DEP      := Makefile.dep

CFLAGS   := -std=gnu99 -c
WARNING  := -Wall -Wno-parentheses -Wno-pointer-arith -Wno-unused -Wundef 
STRICT   := -Wfatal-errors -Werror 
OPTIM    := -O2 -DNDEBUG -fomit-frame-pointer -fno-stack-protector

ifeq ($(DEBUG), 1)
CFLAGS += $(WARNING) -g
else
CFLAGS += $(WARNING) $(OPTIM)
endif

PWD      := $(shell pwd)
TARGET   := pyvm
SRCS     := $(wildcard *.c) $(wildcard utils/*.c)
OBJS     := $(subst .c,.o,$(SRCS))

.PHONY: $(TARGET)

$(TARGET): $(OBJS)
	@echo "    LD -o" $(shell basename $@)
	@$(LINK) -o $@ $^ -lm -lpthread

%.o: %.c
	@set -e
	@echo "    CC" $<
	@$(CC) $(CFLAGS) -o $@ $<

dep:
	@rm -f $(DEP)
	@for SRC in $(SRCS);                       	\
	do                                         	\
	    OBJ=`echo $$SRC | sed -e 's/.c$$/.o/'`; \
	    cpp -MT $$OBJ -MM $$SRC >>$(DEP);      	\
	done

run:
	./pyvm ../tests/except/dislike0.py

count:
	wc -l *.c *.h utils/*.c utils/*.h

clean:
	@rm -f $(OBJS) $(DEP) $(TARGET) core tags *.stackdump

ifeq ($(DEP), $(wildcard $(DEP)))
    include $(DEP)
endif

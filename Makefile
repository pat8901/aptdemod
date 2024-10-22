# *** Deprecated *** 
# This file is longer used. Please use CMakeLists.txt instead. 
# This file is here only for reference. It will be removed in a future pr.

BUILD_DIR = build
CC = gcc

CFLAGS = -Wextra -Wall -g -I"/usr/include" -I"./include"

LDLIBS = -L"/usr/lib/x86_64-linux-gnu/" -lsndfile -lfftw3 -lm

SRCS = src-core/main.c src-core/audio.c src-core/demod.c src-core/utils.c src-core/image.c

OBJS = $(SRCS:.c=.o)

EXE = $(BUILD_DIR)/AptDecode

all: $(EXE)

# Rule to build the target executable
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*
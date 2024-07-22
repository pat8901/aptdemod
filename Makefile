BUILD_DIR = build
CC = gcc

CFLAGS = -Wextra -Wall -g -I"/usr/include" -I"./include"

LDLIBS = -L"/usr/lib/x86_64-linux-gnu/" -lsndfile -lfftw3

SRCS = src/apt.c src/algebra.c src/utils.c

OBJS = $(SRCS:.c=.o)

EXE = $(BUILD_DIR)/apt_decoder

all: $(EXE)

# Rule to build the target executable
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*
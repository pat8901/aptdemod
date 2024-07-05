CC = gcc

CFLAGS = -Wextra -Wall -g -I"E:\programs\c\satellite_communication_system\lib\libsndfile\include"

LDLIBS = -L"E:\programs\c\satellite_communication_system\lib\libsndfile\lib" -lm 

SRCS = apt.c

OBJS = $(SRCS:.c=.o)

EXE = my_program.exe

all: $(EXE)

# Rule to build the target executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
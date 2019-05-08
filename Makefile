CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c99 -g -D _POSIX_C_SOURCE=200809L
SRC=${wildcard src/*.c}
OBJS=${SRC:.c=.o}
EXEC=minimake

.PHONY: all clean

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

clean:
	$(RM) $(OBJS) $(EXEC)

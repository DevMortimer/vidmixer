CC = gcc
CFLAGS = -Wall -std=c99 -g
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
EXEC = vidmixer
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC)

CC = g++
TARGET = app.out
OBJS = main.o AIPlayer.o board.o stone.o

CFLAGS = -Wall 
LDFLAGS = -lc

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJS) $(TARGET)

test :
	./$(TARGET)
SRC := writer-main.c 
TARGET = writer
OBJS := $(SRC:.c=.o)
CFLAGS = -Wall
CC = $(CROSS_COMPILE)gcc

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET) $(LDFLAGS)

clean:
	-rm -f *.o $(TARGET) *.elf *.map

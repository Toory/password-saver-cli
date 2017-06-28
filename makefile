CC = gcc
CFLAGS = -Wall
SOURCES = password_saver.c aes.c linked_list.c 
OBJECTS = $(SOURCES:.c=.o)
TARGET = password_saver

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core

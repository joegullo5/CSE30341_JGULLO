# set gcc as the compiler
CC = gcc

#adds -g and -Wall for debugging and sets standard to gnu99
CFLAGS  = -g -Wall -std=gnu99

# variable to store name of c file and executable
TARGET = singleshell


all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
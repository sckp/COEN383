# set the compiler
CC = gcc
#set the flags
CFLAGS = -Wall
#set the object files
OBJFILES = main.o queue.o jobhelper.o
# specify the name of the desired output file
TARGET = scheduler

all: $(TARGET)

#make the files
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

# clean the files from directory
clean:
	rm -f $(OBJFILES) $(TARGET) *~

# set the compiler
CC = gcc

# set the flags
CFLAGS = -Wall

# specify which libraries to use
LDFLAGS = 

#set the object files
OBJFILES = main.o

# specify the name of the desired output file
TARGET = pipes

all: $(TARGET)

#make the files
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)


# clean the files from directory
clean:
	rm -f $(OBJFILES) $(TARGET) *~

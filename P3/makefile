# set the compiler
CC = g++
# set the flags
CFLAGS = -Wall
# specify which libraries to use
LDFLAGS = -pthread
#set the object files
OBJFILES = main.o customer.o helper.o seller.o
# specify the name of the desired output file
TARGET = sellers

all: $(TARGET)

#make the files
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

# clean the files from directory
clean:
	rm -f $(OBJFILES) $(TARGET) *~

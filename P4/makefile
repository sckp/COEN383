# set the compiler

CC = gcc

# set the flags

CFLAGS = -Wall

# specify which libraries to use

LDFLAGS = -pthread

#set the object files

OBJFILES = main.o helper.o FCFS.o LFU.o LRU.o MFU.o RAN.o

# specify the name of the desired output file

TARGET = scheduler




all: $(TARGET)




#make the files

$(TARGET): $(OBJFILES)

	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)




# clean the files from directory

clean:

	rm -f $(OBJFILES) $(TARGET) *~

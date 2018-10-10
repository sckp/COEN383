typedef struct {
    char pid;
    int index;
} Timeslice;

typedef struct {
    int processCount;
    int timesliceCount;
    Process* process;
    Timeslice* timeslice;
} Record;


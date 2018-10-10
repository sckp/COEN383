typedef struct
{
    // start process properties
    char id;
    int priority;
    float arrival;
    float runtime;
    // end process properties

    // records that convenient for this project
    int age;		// Number of quanta waited before priority bump
    int timeFinished;   // Time that the process finished
    float timeRemaining;
    float responseTime;   // Time from entering the active queue to starting to run
    float waitTime;       // Time from entering the active queue that the process spends NOT running
    float turnaroundTime; // Time from entering the active queue to terminating
} Process;


#include "helper.h"

int main(int argc, char* argv[]) {

    
    int clock = 0;
    
    //COUNTS TO DETERMINE HIT MISS RATIO
    float hitmiss_average = 0.0;
    int hits = 0;
    int misses = 0;
    
    //USED TO DETERMINE JOB SIZE AT INITIALIZATION
    int *job_sizes = malloc(sizeof(int)*4);
    job_sizes[0] = 5; //5 MB
    job_sizes[1] = 11; //11 MB
    job_sizes[2] = 17; //17 MB
    job_sizes[3] = 31; //31 MB

    //FLAG USED FOR EVICT LATER IN PROGRAM WHEN NO SPACE
    int flg = -1;
    if (argc != 2){
        printf("Invalid option.\n");
        return -1;
    }
    if(strcmp(argv[1], "fcfs") == 0){
        flg = 0;
    }else if(strcmp(argv[1], "lru") == 0){
        flg = 1;
    }else if(strcmp(argv[1], "lfu") == 0){
        flg = 2;
    }else if(strcmp(argv[1], "mfu") == 0){
        flg = 3;
    }else if(strcmp(argv[1], "ran") == 0){
        flg = 4;
    }else {
        printf("Invalid option.\n");
        return -1;
    }
    
    srand(0);
    
    for(int i = 0; i < 5; i++) {
        //HIT MISS COUNT RESET FOR EACH RUN
        hits = 0;
        misses = 0;
        
        //PAGE LIST
        List page_list;
        new_list(&page_list);
        
        //500 JOBS
        Job jobs[500];

        //INITIALIZE JOBS
        for(int i=0; i<500; i++) {
            jobs[i].pid = i;
            jobs[i].page_num= job_sizes[rand()%4];
            jobs[i].arrival_time = rand()%60;
            jobs[i].service_duration = rand()%60;
            jobs[i].page_now = 0;
            jobs[i].state = READY;
        }

        //TODO SORT JOBS BY ARRIVAL TIME
        
        int job_index = 0;
        
        //ASSUMING RUN TIME IS 100
        for( clock = 0; clock < 100; clock++) {
            //CHECK JOBS STILL REMAINING, ARRIVAL TIME WITHIN RUN TIME, and JOB IS READY
            while( job_index < 500 && jobs[job_index].arrival_time <= clock && jobs[job_index].state == READY) {
                // TODO look for free pages
                // bring job int memory
                //update all page and job info
            }

            // CHECK FOR NEW JOBS EVERY 100 MS
            for(int i = 0; i < 10; i++) {
                for(int j = 0; j < job_index; j++) {
                    if(jobs[j].service_duration > 0) {
                        //TODO give jobs a new page/next page
                        
                        //PAGE IN MEMEMORY
                        if(in_memory( &page_list, jobs[j].pid, jobs[j].page_now)) {
                            //TODO GET PAGE FROM JOB ID
                            //CHECK IF PAGE IN MEM AND IF IT IS CHECK ITS STATEtp = get_page_from_pid(&pl,Q[j].pid,Q[j].curr_page);
                        }
                        //PAGE NOT IN MEMORY
                        else {
                            Page* page = find_free(&page_list);
                            //EVICT A PAGE
                            if(page == NULL) {
                                if (flg == 0) {
                                    FCFS(&page_list);
                                }
                                if (flg == 1) {
                                    LRU(&page_list);
                                }
                                if (flg == 2) {
                                    LFU(&page_list);
                                }
                                if (flg == 2) {
                                    MFU(&page_list);
                                }
                                if (flg == 4) {
                                    //.cRAN(&page_list);
                                }
                                misses++;
                                jobs[j].io_num = 2 + rand() % 3;
                                page = find_free(&page_list);
                            }
                            
                            jobs[j].state = WAITING;
                            jobs[j].io_num = 2 + rand() % 3;
                            jobs[j].service_duration += jobs[j].io_num;
                            page->state = LOADING;
                            page->pid = jobs[j].pid;
                            page->page_num = jobs[j].page_now;
                            page->frequency_num = 0;
                        }
                    }
                }
            }

            //LET RUNNING JOBS RUN
            
            for(int i = 0; i < job_index; i++) {
                if(jobs[i].service_duration > 0 && jobs[i].state == RUNNING) {
                    jobs[i].service_duration -= 1.0;
                    if(jobs[i].service_duration <= 0) {
                        free_memory(&page_list, jobs[i].pid);
                        jobs[i].state = DONE;
                    }
                }
            }
        }
        printf("Jobs Run: %d,: Hits: %d, Misses: %d\n", i+1, hits, misses);
        hitmiss_average += hits/(1.0*misses);
    }
    printf("Hit Miss Ratio: %f\n",(hitmiss_average /5));
}


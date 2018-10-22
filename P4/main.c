#include "helper.h"

int main(int argc, char* argv[]) {

    
    int clock = 0;
    
    //COUNTS TO DETERMINE HIT MISS RATIO
    float hitmiss_average = 0.0;
    int hits = 0;
    int misses = 0;
    
    //USED TO DETERMINE JOB SIZE AT INITIALIZATION
    int *job_sizes = (int*)malloc(sizeof(int)*4);
    job_sizes[0] = 5; //5 MB
    job_sizes[1] = 11; //11 MB
    job_sizes[2] = 17; //17 MB
    job_sizes[3] = 31; //31 MB

    //FLAG USED FOR EVICT LATER IN PROGRAM WHEN NO SPACE
    int flg = -1;
    if (argc != 2){
        printf("Invalid option. Enter fcfs, lru, lfu, mfu, or ran.\n");
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
        printf ("****************** STARTING RUN #%d *********************\n", i+1);
        //HIT MISS COUNT RESET FOR EACH RUN
        hits = 0;
        misses = 0;
        
        //PAGE LIST
        List page_list;
        new_list(&page_list);
        
        Job jobs[JOB_NUMBER];

        //INITIALIZE JOBS
        for(int i=0; i<JOB_NUMBER; i++) {
            jobs[i].pid = i;
            jobs[i].page_num= job_sizes[rand()%4];
            jobs[i].arrival_time = rand()%60;
            jobs[i].service_duration = rand()%60;
            jobs[i].page_now = 0;
            jobs[i].state = READY;
        }

        //SORT JOBS BY ARRIVAL TIME
        job_sort(jobs, JOB_NUMBER);

        int job_index = 0;
        
        //ASSUMING RUN TIME IS 100
        for( clock = 0; clock < 100; clock++) {
            //CHECK JOBS STILL REMAINING, ARRIVAL TIME WITHIN RUN TIME, and JOB IS READY
            while( job_index < JOB_NUMBER && jobs[job_index].arrival_time <= clock && jobs[job_index].state == READY) {
                
                //CHECK FOR ATLEAST 4 FREE PAGES
                Page *temp = page_list.head;
                int free_count = 0;
                while(temp != NULL) {
                    if (temp->pid == -1) {
                        free_count += 1;
                    }
                    temp = temp->next;
                }
                //BRING JOB INTO MEMORY
                Page *free_page = NULL;
                temp = page_list.head;
                if (free_count >= 4){
                    //ASSIGN FREE PAGE
                    while(temp != NULL) {
                        if (temp->pid == -1) {
                            free_page = temp;
                            break;
                        }
                        temp = temp->next;
                    }
                    
                    //UPDATE ALL PAGE AND JOB INFO
                    free_page->pid = jobs[job_index].pid;
                    free_page->page_num = jobs[job_index].page_now;
                    free_page->state = LOADING;
                    
                    jobs[job_index].io_num = 2 + rand()%3;
                    jobs[job_index].service_duration += jobs[job_index].io_num;
                    job_index++;
                    
                    misses++;
                } else {
                   // printf("Out of memory\n");
                    break;
                }
            }

            // CHECK FOR NEW JOBS EVERY 100 MS
            for(int i = 0; i < 10; i++) {
                for(int j = 0; j < job_index; j++) {
                    if(jobs[j].service_duration > 0) {
                        //TODO UPDATE CURRENT PAGE FOR JOB
                        int new_page_num = rand()%10;
                        if (new_page_num < 7) {
                            new_page_num = jobs[j].page_now+(rand()%3)-1 < 0;
                        } else {
                            new_page_num = rand()%(jobs[j].page_num);
                            while(new_page_num <= 1) {
                                new_page_num = rand()%(jobs[j].page_num);
                            }
                        }
                        if (new_page_num < 0) {
                            new_page_num = 0;
                        } else if (new_page_num >= 100) {
                            new_page_num = jobs[j].page_num -1;
                        }
                        jobs[j].page_now = new_page_num;
                        
                        //PAGE IN MEMEMORY
                        if(in_memory( &page_list, jobs[j].pid, jobs[j].page_now)) {
                            //GET PAGE FROM JOB
                            Page *temp = page_list.head;
                            Page *pnew = NULL;
                            
                            //GET PAGE FROM JOB ID
                            while (temp != NULL) {
                                if (temp->pid == jobs[j].pid && temp->page_num == jobs[j].page_now) {
                                    pnew = temp;
                                    break;
                                }
                                temp = temp->next;
                            }
                            if (pnew->state == IN_MEM) {
                                pnew->frequency_num++;
                                pnew->last_timestamp = clock;
                                hits++;
                            } else if (pnew->state == LOADING) {
                                jobs[j].io_num--;
                                if (jobs[j].io_num <= 0) {
                                    pnew->first_timestamp = clock+(0.1*i);
                                    pnew->state = IN_MEM;
                                    pnew->last_timestamp = clock+(0.1*i);
                                    jobs[j].state = RUNNING;
                                    pnew->page_num = jobs[j].page_now;
                                }
                            }
                            
                            //CHECK IF PAGE IN MEM AND IF IT IS CHECK ITS STATE
                        }
                        //PAGE NOT IN MEMORY
                        else {
                            Page* page = find_free(&page_list);
                            //EVICT A PAGE
                            if(page == NULL) {
                                //printf("Memory full:\n");
                                if (flg == 0) {
                                    FCFS(&page_list);
                                }
                                if (flg == 1) {
                                    LRU(&page_list);
                                }
                                if (flg == 2) {
                                    LFU(&page_list);
                                }
                                if (flg == 3) {
                                    MFU(&page_list);
                                }
                                if (flg == 4) {

                                }
                                misses++;
                                jobs[j].io_num = 2 + rand() % 3;
//                                Page * temp = page_list.head;
//                                while (temp != NULL) {
//                                  //  printf(temp->pid >= 0 ? "PID: %d FREQUENCY: %d LAST TIMESTAMP:%d"
                                    //: "", temp->pid, temp->frequency_num, temp->last_timestamp\n);
//                                    temp = temp->next;
//                                }
//                                printf("\n");
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

            //LET RUNNING JOBS RU
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
        printf("\nJobs Run: %d Hits: %d Misses: %d\n\n", i+1, hits, misses);
        hitmiss_average += hits/(1.0*misses);
    }
    printf("Hit Miss Ratio: %f\n\n",(hitmiss_average /5));
}


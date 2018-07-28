#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//these are shared by all threads
//pthread_t tid[2];
int start_points[4];
double grand_sum=0.0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//this holds information for each newly created thread, excluding the main thread
struct thread_info{
    pthread_t tid;
    int threadnum;
    int index;    
};
void sharedResource(int threadnum, int index){
    double sum;
    int start = start_points[index];
    int end = start_points[index+1];
    while(start <= end){
        sum += sqrt(start);
        start++;
    }
    
    pthread_mutex_lock(&lock);
    printf("\nThread %d running", threadnum);
	grand_sum += sum;
    pthread_mutex_unlock(&lock);
}

void * do_partial(void * arg){
    struct thread_info *tinfo = arg;
    sharedResource(tinfo->threadnum, tinfo->index);
    return NULL;
}


int main(int argc, char *argv[]){
    struct thread_info *tinfo;
	// we need to collect argument and convert to integer
	int error, n, i=0;    
    n = atoi(argv[1]);
    
    //initialize lock
    if(pthread_mutex_init(&lock,NULL) != 0){
        printf("\nMutex has failed\n");
        return -1;
    }
	//main program calculates start_points
    for(int count=0; count < (sizeof(start_points)/sizeof(double)); count++)
        start_points[count] = 1 + (double)count*(n/3.0);
    
    //initialization of i number of threads
    //allocate memory for structure
    tinfo = calloc(2, sizeof(struct thread_info));
    
    //create 2 threads and compute sum
	while(i<2){
        tinfo[i].threadnum = i +1; 
        tinfo[i].index = i +1; 
        error = pthread_create(&tinfo[i].tid,NULL,&do_partial, &tinfo[i]);
        if(error != 0)
            perror("pthread_create");
        i++;	
	}
    
    sharedResource(3,0); // main thread accesses shared resource 
    //join threads 
    pthread_join(tinfo[0].tid, NULL);
    pthread_join(tinfo[1].tid, NULL);
    pthread_mutex_destroy(&lock);
    
    printf("\nThe grand sum is: %f\n", grand_sum);
    
}

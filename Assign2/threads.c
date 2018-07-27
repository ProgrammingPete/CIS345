#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//these are shared by all threads
//pthread_t tid[2];
double start_points[4];
double grand_sum=0.0;
pthread_mutex_t lock;

//Might have to create sum kind of struct to pass arguments into the
// do_partial method.
struct thread_info{ // used for pthread_create
    pthread_t tid2;
    int threadnum;
    int arg;    
};
void * do_partial(void * arg){
    struct thread_info *tinfo = arg;
    double sum;
    pthread_mutex_lock(&lock);
    printf("\nThread %d running", tinfo->threadnum);
	sum = sqrt(start_points[tinfo->arg])+sqrt(start_points[tinfo->arg+1]);
	grand_sum = sum;
    pthread_mutex_unlock(&lock);
    return NULL;
}


int main(int argc, char *argv[]){
    struct thread_info *tinfo;
	// we need to collect argument and convert to integer
	int error, n, i=0, j=0;    
    n = atoi(argv[1]);
    n = (double)n;
    
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
    
    //create 3 threads and compute sum
	while(i<2){
        tinfo[i].threadnum = i +1; 
        tinfo[i].arg = i +1; 
        error = pthread_create(&tinfo[i].tid2,NULL,&do_partial, &tinfo[i]);
        if(error != 0)
            perror("pthread_create");
        i++;	
	}
    
    do_partial(0); // PROBLEM WITH THIS?? 
    pthread_join(tinfo[0].tid2, NULL);
    pthread_join(tinfo[1].tid2, NULL);
    pthread_mutex_destroy(&lock);
    
    printf("\nThe grand sum is: %f\n", grand_sum);
    
}

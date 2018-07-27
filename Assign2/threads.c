#include<pthread.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<ctype.h>
#include<math.h>

//these are shared by all threads
pthread_t tid[2];
int start_points[4];
double grand_sum=0;
pthread_mutex_t lock;

void * do_partial(){
	double sum = 0;
	double res = sqrt(4);
	sum = sqrt(start_points[0])+sqrt(start_points[1]);
	grand_sum = sum;
}


int main(int argc, char *argv[]){
	// we need to collect argument
	int i = 0;
	int error, n;
	if(argc = 2){
		n = atoi(argv[1]);
	} 
	
	while(i<2){
	error = pthread_create(&(tid[i]),NULL,&(do_partial), NULL);
	if(error != 0)
		printf("\n Thread can't be created : [%s]", strerror(error));
	i++;
	
	}
}

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{
    //pipe fds. fd[0] corresponds to read, fd[1]  corresponds to write
    int pipefd1[2], pipefd2[2];
    char mssgParent[100] = "Hi, I am your parent from CIS 345";
    char mssgChild[100] =  "Message from child 2 recieved";
    int pid, pid1;
    char buf;
    
    //creation of the childs
    pid = fork();
    pid1 = fork();
    
    //creation of the pipes with passed fiel descriptors
    if(pipe(pipefd1)==-1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipefd2)==-1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    if(pid == 0){
        //the first child
        if(read(pipefd1[0], &buf,1) > 0){
            write(pipefd1[1], mssgChild, strlen(mssgChild));
            write(STDOUT_FILENO,&buf,1); //also write buff to standard output
        }
        _exit(EXIT_SUCCESS);
    }
    else if(pid > 0){
        //first parent
        write(pipefd1[1], mssgParent, strlen(mssgParent));
        if(read(pipefd1[0], &buf, 1) >0){
            printf("Parent 1: Message Recieved\n");
        }
        wait(NULL);
        exit(EXIT_SUCCESS);      
        
    }
    if(pid1 == 0){
        //the second child
        if(read(pipefd2[0], &buf,1) > 0){
            write(pipefd2[1], mssgChild, strlen(mssgChild));
            write(STDOUT_FILENO,&buf,1); //also write buff to standard output
        }
        _exit(EXIT_SUCCESS);
    }
    else if(pid1 > 0){
        //second parent
        write(pipefd2[1], mssgParent, strlen(mssgParent));
        if(read(pipefd2[0], &buf, 1) >0){
            printf("Parent 2: Message Recieved\n");
        }
        wait(NULL);
        exit(EXIT_SUCCESS); 
    }
    
    
    
}

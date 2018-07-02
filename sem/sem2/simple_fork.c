//
// Created by Administrator on 2018/7/2.
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define DELAY_TIME 3

int main(void){

    pid_t result;
    result = fork();
    if(result<0){
        perror("fork error\n");
    } else if(result == 0){
        printf("child---> child process will wait \n");
        sleep(DELAY_TIME);
        printf("child---> result is %d,pid is %d",result,getpid());
    } else{
        printf("father---> result is %d,pid is %d",result,getpid());
    }
    exit(0);
}
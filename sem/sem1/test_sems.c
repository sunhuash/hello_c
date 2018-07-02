//
// Created by Administrator on 2018/7/2.
//


#include"comn.h"

int main() {
    int semid = creat_sem(1);
    init(semid, 0, 1);
    int id = fork();
    int _semid = get_sem();
    if (id < 0) {
        perror("fork");
    } else if (id == 0) {
        while (1) {
            p(_semid, 0);
            printf("A");
            fflush(stdout);
            usleep(12345);
            printf("A");
            fflush(stdout);
            usleep(13405);
            v(_semid, 0);
        }
    } else {
        while (1) {
            p(semid, 0);
            printf("B");
            fflush(stdout);
            usleep(12245);
            printf("B");
            fflush(stdout);
            usleep(23405);
            v(semid, 0);
        }
        wait(NULL);
    }
    destory_sem(semid);
    return 0;
}
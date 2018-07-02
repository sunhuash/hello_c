//
// Created by Administrator on 2018/7/2.
//

#ifndef MYSTARTDEMO_COMN_H
#define MYSTARTDEMO_COMN_H

#define PATH "."
#define PROJ_ID 0x6666

#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

union semun{
    int     val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};
int creat_sem(int nsems);
int get_sem();
int init(int semid, int which, int _val);
int destory_sem(int semid);
int p(int semid, int which);
int v(int semid, int which);

#endif //MYSTARTDEMO_COMN_H

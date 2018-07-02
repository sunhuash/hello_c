//
// Created by Administrator on 2018/7/2.
//

#include "comn.h"

/**
 * 创建信号量具体实现
 * @param nsems
 * @param flag
 * @return
 */
int comn_sem(int nsems, int flag) {
    //系统建立IPC通讯（如消息队列、共享内存时）必须指定一个ID值。通常情况下，该id值通过ftok函数得到。
    //PATH是你指定的文件名(该文件必须是存在而且可以访问的)，PROJ_ID是子序号，失败返回-1
    key_t key = ftok(PATH, PROJ_ID);
    if (key < 0) {
        perror("ftok");
    }
    //信号量对应于某一种资源，取一个非负的整形值。信号量值（常用sem_id表示）指的是当前可用的该资源的数量，若等于0则意味着目前没有可用的资源。
    //创建信号量或获得在系统中已存在的信号量，此时需要调用 semget() 函数。不同进程通过使用同一个信号量键值来获得同一个信号量。
    int semid = semget(key, nsems, flag);
    if (semid < 0) {
        perror("semget");
        return -1;
    }
    return semid;
}

/**
 * 创建信号量
 * @param nsems
 * @return
 */
int creat_sem(int nsems) {
    return comn_sem(nsems, IPC_CREAT | IPC_EXEL | 0666);
}

/**
 * 取得信号量
 * @return
 */
int get_sem() {
    return comn_sem(0, IPC_CREAT);
}

/**
 * 初始化信号量属性
 * @param semid
 * @param which
 * @param _val
 * @return
 */
int init(int semid, int which, int _val) {
    union semun un;
    un.val = _val;
    //semctl
    if (semctl(semid, which, SETVAL, un) < 0) {
        perror("semctl");
    }
    return 0;
}

/**
 * 释放信号量
 * @param semid
 * @return
 */
int destory_sem(int semid) {
    //如果不需要信号量，则从系统中删除它，此时使用semctl()函数的 IPC_RMID操作。需要注意的是，在程序中不应该出现对已经被删除的信号量的操作。
    if (semctl(semid, 0, IPC_RMID) < 0) {
        perror("semctl");
    }
    return 0;
}

int comn(int semid, int which, unsigned nosps) {
    struct sembuf sbuf;
    sbuf.sem_num = which;
    sbuf.sem_op = nosps;
    sbuf.sem_flg = 0;
    if (semop(semid, &sbuf, 1) < 0) {
        perror("semop");
        return -1;
    }
    return 0;
}

int p(int semid, int which) {
    return comn(semid, which, -1);
}

int v(int semid, int which) {
    return comn(semid, which, 1);
}
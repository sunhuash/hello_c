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

/*定义联合体*/
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

/*函数声明*/
int init_sem(int sem_id, int init_value);

int del_sem(int sem_id);

int sem_p(int sem_id);

int sem_v(int sem_id);

int main(void) {

    pid_t result;
    int sem_id;
    /*创建一个信号量*/
    /**
     * semget函数
     * 第一个参数：信号量的标识
     * 第二个参数：需要创建的信号量数目
     * 第三个参数：信号量的标识
     */
    sem_id = semget(ftok(".","a"),1,0666|IPC_CREAT);
    init_sem(sem_id,0);
    /*创建子进程*/
    result = fork();
    if (result < 0) {
        perror("fork error\n");
    } else if (result == 0) {
        printf("child---> child process will wait \n");
        sleep(DELAY_TIME);//睡眠3秒
        printf("child---> result is %d,pid is %d", result, getpid());
        sem_v(sem_id);
    } else {
        sem_p(sem_id);
        printf("father---> result is %d,pid is %d", result, getpid());
        sem_v(sem_id);
        del_sem(sem_id);
    }
    exit(0);
}
/**
 * semctl函数：
 * 第一个参数：信号量标识符
 * 第二个参数：信号量集合中的序号
 * 第三个参数：
 *          SETVAL 将信号量值设为第四个参数的值
 *          GETVAL 返回信号量的当前值
 *          IPC_RMID 从系统中删除信号量
 *          STAT 存值更复杂
 *
 * */

int init_sem(int sem_id, int init_value) {
    union semun sem_union;
    sem_union.val = init_value;/*init_value为初始化值*/
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
        perror("Initialze semaphore");
        return -1;
    }
    return 0;
}

int del_sem(int sem_id) {
    union semun sem_union;
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
        perror("DELETE semaphore");
        return -1;
    }
}

/*
 * semop函数
 * 第二个参数：操作的信号量数组
 * 第三个参数：操作数组中的操作个数
 */
/*P操作函数*/
int sem_p(int sem_id) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;//信号量编号，这里单个信号量的编号应该为0
    sem_b.sem_op = -1;//信号量操作，取值为-1表示P操作
    sem_b.sem_flg = SEM_UNDO;/*在进程没释放信号量而退出时，系统自动释放该进程中
                          未释放的信号量*/
    if (semop(sem_id, &sem_b, 1) == -1) {/*进行P操作*/
        perror("P operation");
        return -1;
    }
    return 0;
}

/*V操作函数*/
int sem_v(int sem_id) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;//信号量操作，取值为+1表示V操作
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) { /*进行V操作*/
        perror("V operation");
        return -1;
    }
    return 0;
}
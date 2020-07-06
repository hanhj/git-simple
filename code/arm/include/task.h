#ifndef TASK_H
#define TASK_H
#include "os.h" ///<for thread
#include "baseapp.h"

typedef struct task_attr_ {
    uint32   Id;
    char name[20];
    int timeout;/**<  When timeout, main loop should reset thread  */
    int restart_times;/**<  restart the child times when child is  timeout  */
    void * ( *entry ) ( void* );/**<  Entry of thread  */
#ifdef CONFIG_LINUX
    pthread_t  thread_id;/**<  Thread id  */
#else
    Task_Handle thread_id;
#endif
    //void (*alive)(pthread_t id);/**<  Live flag, each thread should call this function before task_timeout  */
    int alive_flag;
    int exit_flag;
    //int exit;/**<  Exit flag, thread need quit loop when exit = 1.  */
} TASK;

class Task
{

public:
//BaseApp* apps[MAXTASK];
    TASK tasks[MAXTASK];
    int16 task_num;
    Task();
    ~Task();
    int16 Init();
    int16 CheckHeart();
    int16 StartThread ( uint32 Id );
    int16 StopThread ( uint32 Id );
    int16 Regist ( BaseApp*, void * ( *entry ) ( void* ) );
		int16 Regist ( uint32 id );
		int16 UnRegist ( uint32 id );
    void  Heart ( uint32 Id ) ;
};

#define CHECK_TASK_EXIT(arg) if (arg->exit_flag) break;
extern Task *g_task;
#endif

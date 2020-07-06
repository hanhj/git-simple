
#include <string.h>
#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "debug.h"
#include "task.h"
#include "timer.h"
#include "os.h"// for mutex

static struct mutex task_mutex;
Task::Task()
{
    task_num = 0;
}

Task::~Task()
{
    task_num = 0;
}

int16 Task::Init()
{
    mutex_init ( &task_mutex, "task mutex" );
    memset(tasks,0,sizeof(TASK)*MAXTASK);
    return 1;
}

int16 Task::StartThread ( uint32 Id )
{
    mutex_lock ( &task_mutex );
    TASK* task;
    int16 i;

    for ( i = 0;i < MAXTASK;i++ ) {
        task = &tasks[i];

        if ( Id == task->Id ) {
            if ( task != 0 ) {
                task->exit_flag = 0;
#ifdef CONFIG_LINUX
								if(task->entry!=NULL){
              	pthread_create ( &task->thread_id, NULL, task->entry, task );
              }
#else
                Task_Params taskParams;
                Task_Params_init ( &taskParams );
                taskParams.priority = i + 1;
								taskParams.stackSize = 0x1000;
								task->thread_id=Task_create( (ti_sysbios_knl_Task_FuncPtr)task->entry,							&taskParams, NULL);
								//task->thread_id=Task_create( task->entry,&taskParams, NULL);
#endif

                if ( task->thread_id != (pthread_t)NULL ) {
                    PFUNC ( TEM_INFO ,DEBUG_NORMAL, "%s create ok [%lu]\n", task->name, task->thread_id );
                } else {
                		if(task->entry!=NULL)
                    PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "%s create fail !!!!!!!!! [%lu]\n", task->name, task->thread_id );
                }
            } else {
                PFUNC ( TEM_WARNING ,DEBUG_NORMAL, "%s Task:%x not exist\n", Id );
                mutex_unlock ( &task_mutex );
                return 0;
            }

            mutex_unlock ( &task_mutex );

            return 1;
        }
    }

    mutex_unlock ( &task_mutex );

    return 0;
}

int16 Task::StopThread ( uint32 Id )
{
    mutex_lock ( &task_mutex );
    int16 i;

    for ( i = 0;i < MAXTASK;i++ ) {
        if ( Id == tasks[i].Id ) {
            TASK* task = &tasks[i];
#ifdef CONFIG_LINUX
            //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
            task->exit_flag = 1;
            //delay ( 1 );
            //pthread_join(task->thread_id,NULL);
            //pthread_cancel ( task->thread_id );
            //pthread_kill(task->thread_id,SIGKILL);
#else
            Task_delete ( ( ti_sysbios_knl_Task_Handle * ) task->thread_id );
#endif
            mutex_unlock ( &task_mutex );
            PFUNC ( TEM_WARNING ,DEBUG_NORMAL, "%s destroy ok [%lu]\n", task->name, task->thread_id );
            return 1;
        }
    }

    mutex_unlock ( &task_mutex );

    return 0;
}

int16 Task::Regist ( BaseApp*p_app, void * ( *entry ) ( void* ) )
{
    int16 i,ret;
    ret=0;
    mutex_lock ( &task_mutex );

    for(i=0;i<MAXTASK;i++)
    {
    	if(tasks[i].Id==0){
		    strcpy ( tasks[i].name, p_app->name );
		    tasks[i].Id = p_app->Id;
		    tasks[i].timeout = p_app->timeout;
		    tasks[i].restart_times = p_app->restart_times;
		    tasks[i].entry = entry;
		    PFUNC ( TEM_INFO,DEBUG_NORMAL, "regist app:%s id:%x task:%lu  in:%d\n", tasks[i].name, tasks[i].Id, tasks[i].entry ,i);
		  	task_num++;
		  	ret =1;
		  	break;
  		}
  	}
   mutex_unlock ( &task_mutex );
   return ret;    
}
//注册非基于BaseApp的线程
int16 Task::Regist ( uint32 id )
{
    char str[20];
    int16 i,ret;
    ret=0;
    mutex_lock ( &task_mutex );

    for(i=0;i<MAXTASK;i++)
    {
    	if(tasks[i].Id==0){
		    sprintf(str,"thread %ld ",id);
		    strcpy ( tasks[i].name,str );
		    tasks[i].Id = id;
		    tasks[i].timeout = DEFAULT_TIMEOUT;
		    tasks[i].restart_times = DEFAULT_RESTART_TIMES;
		    tasks[i].entry = NULL;
		    PFUNC ( TEM_INFO,DEBUG_NORMAL, "regist app:%s id:%x task:%lu  in:%d\n", tasks[i].name, tasks[i].Id, tasks[i].entry ,i);
		  	task_num++;
		  	ret =1;
		  	break;
  		}
  	}
   mutex_unlock ( &task_mutex );
   return ret;
}

int16 Task::UnRegist ( uint32 id )
{
    int16 i,ret;
    ret=0;
    mutex_lock ( &task_mutex );
    for(i=0;i<MAXTASK;i++)
    {
    	if(tasks[i].Id==id){
		    PFUNC ( TEM_WARNING,DEBUG_NORMAL, "unregist app:%s id:%x task:%lu in:%d  \n", tasks[i].name, tasks[i].Id, tasks[i].entry ,i);
		    tasks[i].Id = 0;
		    memset(&tasks[i],0,sizeof(TASK));
		  	task_num--;
		  	ret =1;
		  	break;
  		}
  	}
   mutex_unlock ( &task_mutex );
   return ret;
}

int16 Task::CheckHeart ( void )
{
    int i , j;
    static int retry[MAXTASK] = {0, 0, 0, 0, 0, 0, 0, 0};
    static int time_cnt[MAXTASK] = {0, 0, 0, 0, 0, 0, 0, 0};

    for ( i = 0;i < MAXTASK;i++ ) {
    		if(tasks[i].Id==0)
    			continue;
        if ( tasks[i].alive_flag == 1 ) {
            retry[i] = 0;
            time_cnt[i] = 0;
            tasks[i].alive_flag = 0;
            //PFUNC ( TEM_INFO,DEBUG_NORMAL, "Receive %s  Task heart!\n", tasks[i].name );
        } else {
            time_cnt[i]++;
            //PFUNC ( TEM_INFO,DEBUG_NORMAL, "%s %d %d\n", tasks[i].name, time_cnt[i],retry[i]);

            if ( time_cnt[i] >= tasks[i].timeout ) {
                time_cnt[i] = 0;
                retry[i]++;

	       		if ( retry[i] >= tasks[i].restart_times ){
	           	return -1;
	           	}

                PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "No:%d %s Task timeout and to be cancelled!\n", i, tasks[i].name );
                PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "Now I will restart app!\n" );
                
                    for ( j = 0;j < MAXTASK;j++ ) {
                    	if(tasks[j].Id==0)
                    		continue;
                        if ( StopThread ( tasks[j].Id ) ) {
                            tasks[j].thread_id = 0;
                        }
                    }
                    delay(5);
                    for ( j = 0;j < MAXTASK;j++ ) {
                    	if(tasks[j].Id==0)
                    	continue;
                    	mdelay(10);
                        StartThread ( tasks[j].Id );
                        //retry[j] = 0;
                        time_cnt[j] = 0;
                        tasks[j].alive_flag = 0;
                    }

                    return 1;
                    
            }
            
        }
    }

    return 0;
}

void Task::Heart ( uint32 Id )
{
    mutex_lock ( &task_mutex );
    int i ;

    for ( i = 0;i < MAXTASK;i++ ) {
        if ( tasks[i].Id == Id ) {
            tasks[i].alive_flag = 1;
            ///PFUNC(TEM_INFO,DEBUG_NORMAL," - %s Send heart\n",tasks[i].name);
            break;
        }
    }

    mutex_unlock ( &task_mutex );
}


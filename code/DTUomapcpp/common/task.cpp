#include <string.h>
#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "debug.h"
#include "task.h"
//#include "timer.h"
#include "os.h"// for mutex

static struct mutex task_mutex;
Task::Task(){
task_num=0;
}
Task::~Task(){
task_num=0;
}

int16 Task::Init(){	
mutex_init(&task_mutex, "task mutex");
return 1;
}

int16 Task::StartThread(uint32 Id)
{
mutex_lock(&task_mutex);
TASK* task;
int16 i;
	for(i=0;i<task_num;i++)	{
			task=&tasks[i];
		  if(Id==task->Id){
				if(task!=0)
				{
					#ifdef CONFIG_LINUX
					pthread_create(&task->thread_id,NULL,task->entry,NULL);
					#else
					Task_Params taskParams;
					Task_Params_init(&taskParams);
					taskParams.priority=i+1;
					taskParams.stackSize = 4096;//4096;四个线程 总共8192
					task->thread_id=Task_create( (ti_sysbios_knl_Task_FuncPtr)task->entry,
							&taskParams, NULL);
					#endif

					if(task->thread_id!=NULL)
					{
					//	PFUNC(TEM_WARNING,"%s create ok [%lu]\n",task->name,task->thread_id);
					}
					else
					{
						//PFUNC(TEM_ERROR,"%s create fail !!!!!!!!! [%lu]\n",task->name,task->thread_id);
					}
			}
			else
			{
			//	PFUNC(TEM_WARNING,"%s Task:%x not exist\n",Id);
				mutex_unlock(&task_mutex);
				return 0;
			}
			mutex_unlock(&task_mutex);
			return 1;
		}
	}
mutex_unlock(&task_mutex);
return 0;
}

int16 Task::StopThread(uint32 Id)
{
mutex_lock(&task_mutex);
int16 i;
for(i=0;i<task_num;i++)	{
	if(Id==tasks[i].Id){
			TASK* task=&tasks[i];
			//PFUNC(TEM_WARNING,"%s destroy ok [%lu]\n",task->name,task->thread_id);
			#ifdef CONFIG_LINUX
			pthread_cancel(task->thread_id);
			#else
			Task_delete((ti_sysbios_knl_Task_Handle *)task->thread_id);
			#endif
			mutex_unlock(&task_mutex);
			return 1;
		}
	}
mutex_unlock(&task_mutex);
return 0;
}

int16 Task::Regist(BaseApp*p_app,void *(*entry)(void*))
{
	mutex_lock(&task_mutex);
	strcpy(tasks[task_num].name,p_app->name);
	tasks[task_num].Id=p_app->Id;
	tasks[task_num].timeout=p_app->timeout;
	tasks[task_num].restart_times=p_app->restart_times;
	tasks[task_num].entry=entry;
	//PFUNC(TEM_INFO,"task num% d app:%s id:%x task:%lu  \n",task_num,tasks[task_num].name,tasks[task_num].Id,tasks[task_num].entry);
	task_num++;
	if(task_num<=MAXTASK){
			mutex_unlock(&task_mutex);
			return 1;
		}
		mutex_unlock(&task_mutex);
		return 0;
}

int16 Task::CheckHeart(void)
{
	int i ,j;
	static int retry[MAXTASK]={0,0,0,0,0,0,0,0};
	static int time_cnt[MAXTASK]={0,0,0,0,0,0,0,0};
	for (i=0;i< task_num;i++ )
	{
		if(tasks[i].alive_flag==1)
		{
			retry[i]=0;
			time_cnt[i]=0;
			tasks[i].alive_flag=0;
			//PFUNC(TEM_INFO,"Receive %s  Task heart!\n",tasks[i].name);
		}
		else
		{
			time_cnt[i]++;
			if(time_cnt[i]>=tasks[i].timeout)
			{
				time_cnt[i]=0;
				retry[i]++;
				if(retry[i]>=tasks[i].restart_times)
				{
					retry[i]=0;
					//PFUNC(TEM_ERROR,"No:%d %s Task timeout and to be cancelled!\n",i,tasks[i].name);
					//PFUNC(TEM_ERROR,"Now I will restart app!\n");
					for(j=0;j<task_num;j++){
						if(StopThread(tasks[j].Id)){
							tasks[j].thread_id=0;
							}
						}
					sleep(1)	;
					for( j=0;j<task_num;j++){
							sleep(1);
							StartThread(tasks[j].Id);
							retry[j]=0;
							time_cnt[j]=0;
							tasks[j].alive_flag=0;
						}
				 return 1;
				}
			}
		}
	}
return 1;
}
void Task::Heart(uint32 Id)
{
mutex_lock(&task_mutex);
  int i ;
	for (i=0;i< task_num;i++ )
	{
		if(tasks[i].Id==Id)
		{
			tasks[i].alive_flag=1;
			///PFUNC(TEM_INFO," - %s Send heart\n",tasks[i].name);
			break;
		}
	}
mutex_unlock(&task_mutex);
}
#ifndef CONFIG_LINUX
void sleep(int32 time){
Task_sleep(time);
}//单位 ms
#endif

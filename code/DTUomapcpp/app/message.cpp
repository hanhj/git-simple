/*
 * message.cpp
 *
 *  Created on: 2013-8-13
 *      Author: Administrator
 */


#include <stdlib.h>//for free function
#include <stdio.h>//for printf function
#include <string.h>//for memcpy function
#include "product.h"
#include "message.h"
#include "task.h"
#include "utils.h"
#include "os.h"// for mutex
#include "debug.h"
#include "varm.h"
Message msg;
static struct mutex msg_mutex;

static struct cond msg_cond;
/** /brief需要定义一个全局函数Task类处理该类的入口
*/
void* message_main ( void*arg )
{
    msg.Run ();
    return NULL;
}
/*
static void  callback_time(void)
{
    Node*node;
    static int16 count = 0;
    static int8 c = 0;
    node = NULL;
    msg.Heart();

    if ( count != msg.Msg.GetNum() ) {
        count = msg.Msg.GetNum();
        node = msg.Msg.Pop ( 0 );
        c = 0;
    }

    if ( c == 0 ) {
        count = msg.Msg.GetNum();
        c = 1;
    } else {
        if ( count == msg.Msg.GetNum() ) {
            if ( msg.Msg.GetNum() != 0 && node == msg.Msg.Pop ( 0 ) ) {//消息数量未变，并且最后一条消息也未变
                node = msg.Msg.Pop ( 0 );
                node->data.Owner = 0;
                //PFUNC(TEM_WARNING,"Message not deal above %ds ,I will delete it\r\n",NO_DEAL_MSG_TIME);
            }
        }

        c = 0;
    }

    //PFUNC(TEM_INFO,"message call time %d\r\n",count);
}
*/
///////////////公有函数//////////////////////////////////////////////////
Message::Message()
{
    strcpy ( name, "Message" );
    Id = TASK_MSG;

    for ( int i = 0;i < MAXTASK;i++ ) {
        call_id[i] = 0;
        call_fun[i] = NULL;
    }

    call_num = 0;

    mutex_init ( &msg_mutex, "msg mutex" );
  //  thread_cond_init ( &msg_cond, "msg cond" );

}

Message::~Message()
{
    Msg.Flush();
    mutex_destroy ( &msg_mutex );
    thread_cond_destroy ( &msg_cond );
 //   PFUNC ( TEM_WARNING, "Destroy Message\r\n" );
}

int16 Message::Init()
{
    Msg.Flush();
    return 1;
}

int16 Message::Run ( )
{
    Init();
    //timer.SetTimer ( &timer_id, 3, callback_time );

    while ( 1 ) {
        //thread_wait_cond(&msg_cond);
        CheckMsg();
        mdelay ( 10 );
        //delay(1);
       // CHECK_TASK_EXIT ( ( ( TASK* ) arg ) );
    }

    //timer.KillTimer ( timer_id );

   // PFUNC ( TEM_WARNING, "Exit Timer Run\r\n" );
  //  return 1;
    //timer.KillTimer ( timer_id );
}

void Message::CheckMsg(){
int16 i;
Node*node;
//PFUNC(TEM_INFO,"---%d\r\n",Msg.GetNum());		
if(Msg.GetNum()!=0)
	{
		node=Msg.Pop(0);
		for(i=0;i<MAXTASK;i++)
		{
		//PFUNC(TEM_INFO,"%d---%d,owner:%x %d %lu\r\n",i,Msg.GetNum(),node->data.Owner,call_id[i],call_fun[i]);	
			if(call_id[i]!=0&&(node->data.Owner&call_id[i])){
				node->data.Owner&=~call_id[i];
				if(call_fun[i]!=NULL)
					(call_fun[i])(node->data);
			}				
			else{
				//printf("No:%d Task no register\r\n",i+1);
				}
		}
		if(node->data.Owner==0){
		node=Msg.Pop(1);	
		free(node->data.Data);
		free(node);
		//PFUNC(TEM_WARNING,"free node\r\n");
		}
	}
}

int16 Message::GetMsg(uint32 Id,MessageData<BaseDataType>**message){
mutex_lock(&msg_mutex);	
Node*node;
if(Msg.GetNum()!=0){
	node=Msg.Pop(0);
	if(node->data.Owner&Id){
		node->data.Owner&=~Id;

		*message=&node->data;
		mutex_unlock(&msg_mutex);
		return 1;
	}
	mutex_unlock(&msg_mutex);
	return 0;
	}
	mutex_unlock(&msg_mutex);
return 0;	
}

void Message::SendMsg(uint32 msg_to,int32 msg_id,int32 data_id,BaseDataType*data,CP56Time2a *time)
{
#if LINK_TO_ARM
	if(varm_fin_init==1){
		mutex_lock ( &msg_mutex );
		MessageData<BaseDataType> msg_data;
		msg_data.Owner = msg_to;
		msg_data.MessageId = msg_id;
		msg_data.Id = data_id;
		msg_data.Data = data;
		memcpy ( ( char * ) &msg_data.Time, ( char* ) time, sizeof ( msg_data.Time ) );
		msg_data.Size = user_data.GetDataSize ( data_id );
		Msg.Push ( msg_data );
		thread_signal ( &msg_cond );
		mutex_unlock ( &msg_mutex );
	}
#endif
}

void Message::SendMsg ( uint32 msg_to, int32 msg_id, int32 data_id, char*data, CP56Time2a* time )
{
#if LINK_TO_ARM
    mutex_lock ( &msg_mutex );
    MessageData<BaseDataType> msg_data;
    msg_data.Owner = msg_to;
    msg_data.MessageId = msg_id;
    msg_data.Id = data_id;
    //msg_data.Data = data;
    memcpy ( ( char * ) &msg_data.Time, ( char* ) time, sizeof ( msg_data.Time ) );
    msg_data.Size = user_data.GetDataSize ( data_id );
    Msg.Push ( msg_data ,data);
    thread_signal ( &msg_cond );
    mutex_unlock ( &msg_mutex );
#endif
}

int16 Message::RegistMsg ( uint32 thread_id, int16 ( *get_msg ) ( MessageData<BaseDataType> ) )
{
    int16 i;
    mutex_lock ( &msg_mutex );

    for ( i = 0;i < MAXTASK;i++ ) {
        if ( call_id[i] == 0 ) {
            call_id[i] = thread_id;
            call_fun[i] = get_msg;
            call_num++;
            //PFUNC ( TEM_INFO, "No:%d Task id:%x regist callid:%d fun:%lu\r\n", call_num, thread_id, call_id[i], call_fun[i] );
            mutex_unlock ( &msg_mutex );
            return 1;
        }
    }

    mutex_unlock ( &msg_mutex );

    return 0;
}


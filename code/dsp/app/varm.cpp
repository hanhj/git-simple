//#include <unistd.h>///<for sleep
#include <stdlib.h>//for free function
#include <stdio.h>//for printf function
#include <string.h>//for memcpy function
//#include "timer.h"
#include "message.h"
#include "task.h"
#include "os.h"// for mutex
#include "varm.h"
#include "syslink.h"
#include "appcommon.h"
Varm varm;
static char data_buf[32*1024];
static char msg_buf[32*1024];
static struct mutex varm_mutex;
uint32 OnGetData(char *buf,uint32 len);
int16 OnSetData(char *buf,uint32 len);
uint32 OnCheckNormalMsg(char *buf,uint32 len);
extern Semaphore_Handle varm_sem;
/** /brief 需要定义一个全局函数Task类处理该类的入口
*/
void* varm_main(void *){
varm.Run();
return NULL;
}
/** /brief 需要定义一个全局函数供注册消息类处理消息
*/
static int16 GetMsg2(MessageData<BaseDataType>message){
varm.GetMsg(message);
return 1;
}	
/** /brief 定义一个全局函数处理时间回调函数
*/
static void  callback_time(void)
{
	PFUNC(TEM_INFO,"varm call time %d\r\n");
}


Varm::Varm(){	
strcpy(name,"Varm");
Id=TASK_VARM;
}
Varm::~Varm(){	
mutex_destroy(&varm_mutex);
}

int16 Varm::Init(){	
	//msg.RegistMsg(Id,GetMsg2);///<向message类注册线程号以及消息回调函数	
	mutex_init(&varm_mutex, "varm mutex");
	syslink.SysLinkInit();
	syslink.SetOnWriteFun(OnSetData);
	syslink.SetOnReadFun(OnGetData);
	syslink.SetOnCheckFun(OnCheckNormalMsg);
	MsgSendFlag=0;
	msg_len=0;
return 1;
}

int16 Varm::Run(){
	Uint32 event;
	Init();

	while(1){
	CheckMsg();
	sleep(10);
	//Heart();
				event = syslink.RecvCmd();
				if(event==APP_E_FAILURE){
					 printf("wait event erro \r\n");
					 goto leave;
				}
				switch (event&APP_CMD_MASK){
				  case CMD_WRITE_DATA://remote write 
					   syslink.OnWrite(data_buf,event&APP_SPTR_MASK);
					break;
				  case CMD_READ_DATA://remote read 
					   syslink.OnRead(data_buf ,(event&APP_SPTR_MASK));
					 break;
				  case CMD_CHECK_MSG://check local soe
					  if(syslink.OnCheckEvent(msg_buf,msg_len))
					  MsgSendFlag=SEND;
					  printf("check data \r\n");
					  break;
				  case APP_CMD_SHUTDOWN://finish server
						goto leave;
			 }		
	}
leave:syslink.SysLinkFnit();	
return 1;
}
static m;
void Varm::CheckMsg(){
//	DataType1 d1;	
//	CP56Time2a time;
//	d1.Data=4;
//	//timer.GetCP56Time(&time);
//	memset((char*)&time,1,sizeof(time));
//	msg.SendMsg(TASK_VARM,MSG_YX,YX_0218,&d1,&time);//向VARM线程发送消息
//	d1.Data=5;


		int total_len,item_len,i;
		char *dataptr,*buf,*buf2;
		MessageData<BaseDataType>*message;
		DataHead Head;
		Item items[10];
		
		item_len=0;
		total_len=0;
		buf=&msg_buf[0];
		i=0;

		if(MsgSendFlag==READY)	
			return;
			m++;
			if(m>4){
		//Semaphore_post(varm_sem);
		m=0;
		}
		Head.Num=msg.Msg.GetNum();
		if(Head.Num>5)
			Head.Num=5;
		total_len=sizeof(Head.Num);
		dataptr=buf+sizeof(Head.Num)+sizeof(Item)*(Head.Num);
		buf2=dataptr;
		//dataptr=buf+sizeof(Head.Num)+6*(Head.Num);
		//buf[1]=msg.GetMsg(TASK_VARM,&message);
/*
		buf[0]=Head.Num;
		buf[2]=(int32)node;
		buf[3]=(int32)node>>8;
		buf[4]=(int32)node>>16;
		buf[5]=(int32)node>>24;
		buf[6]=node->data.Owner;
		buf[7]=node->data.Owner>>8;
		buf[8]=node->data.Owner>>16;
		buf[9]=node->data.Owner>>24;
		buf[10]=(int32)(node->data.Data);
		buf[11]=(int32)(node->data.Data)>>8;
		buf[12]=(int32)(node->data.Data)>>16;
		buf[13]=(int32)(node->data.Data)>>24;
		buf[14]=node->data.Id;
		buf[15]=node->data.Id>>8;
		buf[16]=node->data.Id>>16;
		buf[17]=node->data.Id>>24;
		buf[18]=node->data.flag;
*/	


	


			memcpy(buf,&Head.Num,sizeof(Head.Num));
			buf+=sizeof(Head.Num);
			//i=0;
		while(msg.GetMsg(TASK_VARM,&message)!=0){

			total_len+=sizeof(Item);
			//total_len+=6;
			item_len+=sizeof(Item);
			//item_len+=6;
			items[i].Id=message->Id;
			items[i].Len=message->Size+sizeof(message->Time);
			//items[i].Len=message->Size+7;
	
			memcpy(dataptr,(char *)&(((DataType1*)(message->Data))->Data),message->Size);
			dataptr+=message->Size;
			memcpy(dataptr,(char*)&message->Time,sizeof(message->Time));
			//memcpy(dataptr,(char*)&message->Time,7);
			dataptr+=sizeof(message->Time);
			//dataptr+=7;

			total_len+=items[i].Len;
			i++;
			//PFUNC(TEM_INFO,"get msgID=%d,msgDataType=%d,msgOwner=%d,dataid=%x,data=%f\n",message->MessageId,message->Data->DataType,message->Owner,message->Id,((DataType4*)(message->Data))->Data);
			msg.CheckMsg();//clear this msg from msg list
			if(i>=Head.Num)
				break;
		}	
			memcpy(buf,items,item_len);
			buf+=item_len;

//
//		while(msg.GetMsg(TASK_VARM,&message)!=0){
//
//			total_len+=sizeof(Item);
//			//total_len+=6;
//			item_len+=sizeof(Item);
//			//item_len+=6;
//			items[i].Id=message->Id;
//			items[i].Len=message->Size+sizeof(message->Time);
//			//items[i].Len=message->Size+7;
//	
//			//memcpy(dataptr,(char *)&(((DataType1*)(message->Data))->Data),message->Size);
//			dataptr+=message->Size;
//			//memcpy(dataptr,(char*)&message->Time,sizeof(message->Time));
//			//memcpy(dataptr,(char*)&message->Time,7);
//			dataptr+=sizeof(message->Time);
//			//dataptr+=7;
//
//			total_len+=items[i].Len;
//			i++;
//			//PFUNC(TEM_INFO,"get msgID=%d,msgDataType=%d,msgOwner=%d,dataid=%x,data=%f\n",message->MessageId,message->Data->DataType,message->Owner,message->Id,((DataType4*)(message->Data))->Data);
//			msg.CheckMsg();//clear this msg from msg list
//			if(i>=Head.Num)
//				break;
//		}	
//			//memcpy(buf,items,item_len);
//
//		buf[0]=Head.Num;
//		buf[2]=sizeof(Head.Num);
//		buf[3]=sizeof(Item);
//		buf[4]=item_len;
//		buf[5]=0;
//		buf[6]=(int32)(buf);   
//		buf[7]=(int32)(buf)>>8;
//		buf[8]=(int32)(buf)>>16;
//		buf[9]=(int32)(buf)>>24;
//		buf[10]=(int32)(buf2);
//		buf[11]=(int32)(buf2)>>8;
//		buf[12]=(int32)(buf2)>>16;
//		buf[13]=(int32)(buf2)>>24;
//		buf[14]=(int32)(dataptr);    
//		buf[15]=(int32)(dataptr)>>8; 
//		buf[16]=(int32)(dataptr)>>16;
//		buf[17]=(int32)(dataptr)>>24;
//		buf[18]=total_len;
//		buf[19]=total_len>>16;
		if(i>0)	
			{
			MsgSendFlag=READY;
			msg_len=total_len;
			}
		else
			{
			MsgSendFlag=0;
			msg_len=0;
			}	

//PFUNC(TEM_INFO,"varm check msg\r\n");
}

uint32 OnCheckNormalMsg(char *buf,uint32 len){
	
		if(varm.MsgSendFlag==READY)	
			{
			return 1;
			}
		else
			{
			return 0;
			}	
}

uint32 OnGetData(char *buf,uint32 len)
{
char *dataptr,*itemptr;
int i,total_len;
DataHead *Head;
Item *items;
		Head=(DataHead *)buf;
		dataptr=buf+sizeof(Head->Num)+Head->Num*sizeof(Item);		
		itemptr=buf+sizeof(Head->Num);		
		Data<DataType1>* data;
		for(i=0;i<Head->Num;i++){
		items=(Item*)itemptr;
		data=(Data<DataType1> *)user_data.GetDataPtr(items->Id);
	
		memcpy(dataptr,(void *)&data->Data->Data,items->Len);
		dataptr+=items->Len;
		itemptr+=sizeof(Item);
		}	
		total_len=dataptr-buf;
return total_len;
}
int16 OnSetData(char *buf,uint32 len)
{
char *dataptr,*itemptr;
int i;
DataHead* Head;
Item *items;
		Head=(DataHead *)buf;
		dataptr=buf+sizeof(Head->Num)+Head->Num*sizeof(Item);		
		itemptr=buf+sizeof(Head->Num);		
		Data<DataType1>* data;
		for(i=0;i<Head->Num;i++){
		items=(Item*)itemptr;
		data=(Data<DataType1> *)user_data.GetDataPtr(items->Id);
		memcpy((void *)&data->Data->Data,dataptr,items->Len);
		dataptr+=items->Len;
		itemptr+=sizeof(Item);
		}	

return 1;
}
int16 Varm::OnGetWaveMsg(){
return 1;
}

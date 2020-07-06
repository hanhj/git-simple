#include <stdlib.h>//for free function
#include <stdio.h>//for printf function
#include <string.h>//for memcpy function
#include "product.h"
#include "message.h"
#include "task.h"
#include "os.h"// for mutex
#include "varm.h"
#include "syslink.h"
#include "appcommon.h"
#include "scada.h"
#include "fa.h"

Varm varm;
#pragma DATA_SECTION("DATAINDDR2")
static char data_buf[32*1024];
#pragma DATA_SECTION("DATAINDDR2")
static char msg_buf[32*1024];
#pragma DATA_SECTION("DATAINDDR2");
static struct mutex varm_mutex;
uint32 OnGetData(char *buf,uint32 len);
int16 OnSetData(char *buf,uint32 len);
uint32 OnCheckNormalMsg(char *buf,uint32 len);

uint16 varm_fin_init=0;
/** 
 * /brief 需要定义一个全局函数Task类处理该类的入口
*/
void* varm_main(void *arg){
varm.Run();
return NULL;
}

Varm::Varm(){	
strcpy(name,"Varm");
Id=TASK_VARM;
}
Varm::~Varm(){	
mutex_destroy(&varm_mutex);
}

int16 Varm::Init(){	
	mutex_init(&varm_mutex, "varm mutex");
	syslink.SysLinkInit();
	syslink.SetOnWriteFun(OnSetData);
	syslink.SetOnReadFun(OnGetData);
	syslink.SetOnCheckFun(OnCheckNormalMsg);
	MsgSendFlag=0;
	msg_len=0;
	varm_fin_init = 1;
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
					// printf("wait event erro \r\n");
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
					//  printf("check data \r\n");
					  break;
				  case APP_CMD_SHUTDOWN://finish server
						goto leave;
			 }		
	}
leave:syslink.SysLinkFnit();	
return 1;
}

/*
*************************************************************************************
*  @brief: 处理本线程所获得的从其他线程所发来的消息.将获得的消息填充到发送缓冲区,并置位标志,等待发送..
*  @param[in]
*  @param[out]
*  @return:
*  @note	
**************************************************************************************
*/
void Varm::CheckMsg(){

		int total_len,item_len,i;
		char *dataptr,*buf;
		MessageData<BaseDataType>*message;
		DataHead Head;
		Item items[10];
		
		item_len=0;
		total_len=0;
		buf=&msg_buf[0];
		i=0;

		if(MsgSendFlag==READY)	
			return;

		Head.Num=msg.Msg.GetNum();
		if(Head.Num>5)
			Head.Num=5;
		total_len=sizeof(Head.Num);
		dataptr=buf+sizeof(Head.Num)+sizeof(Item)*(Head.Num);

		memcpy(buf,&Head.Num,sizeof(Head.Num));
		buf+=sizeof(Head.Num);

		while(msg.GetMsg(TASK_VARM,&message)!=0){

		total_len+=sizeof(Item);
		item_len+=sizeof(Item);
		items[i].Id=message->Id;
		items[i].Len=message->Size+sizeof(message->Time);
	
		memcpy(dataptr,(char *)&(((DataType1*)(message->Data))->Data),message->Size);
		dataptr+=message->Size;
		memcpy(dataptr,(char*)&message->Time,sizeof(message->Time));
		dataptr+=sizeof(message->Time);

		total_len+=items[i].Len;
		i++;
		msg.CheckMsg();//clear this msg from msg list
		if(i>=Head.Num)
			break;
		}	
			memcpy(buf,items,item_len);
			buf+=item_len;


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
			scada.ReadCfg(items->Id);//add by zf 20131213
#if ENABLE_FA
			fa.ReadCfg(items->Id);//add by zf 20140318
#endif
			dataptr+=items->Len;
			itemptr+=sizeof(Item);
		}	

return 1;
}
int16 Varm::OnGetWaveMsg(){
return 1;
}

#include <string.h>///<memcpy
//#include <unistd.h>///<for sleep
#include "scada.h"
#include "message.h"///<用到message类中的消息
#include "timer.h"///<用到Timer类中的计时器
#include "task.h"///<用到Task类中的g_task全局变量中Heart函数
#include "debug.h"
#include "varm.h"

Scada scada;
/** /brief 需要定义一个全局函数供注册消息类处理消息
*/
static int16 GetMsg2(MessageData<BaseDataType>message){
scada.GetMsg(message);
return 1;
}	

/** /brief 需要定义一个全局函数Task类处理该类的入口
*/
void* scada_main(void *){
scada.Run();
return NULL;
}
/** /brief 定义一个全局函数处理时间回调函数
*/
static void  callback_time(void)
{
	PFUNC(TEM_INFO,"scada call time\r\n");
	scada.Heart();

}
///////////////公有函数//////////////////////////////////////////////////
Scada::Scada(){
	strcpy(name,"Scada");
	Id=TASK_SCADA;
}

int16 Scada::Init(){
//msg.RegistMsg(Id,GetMsg2);///<向message类注册线程号以及消息回调函数
return 1;
}
extern Semaphore_Handle varm_sem;
int16 Scada::Run(){
Semaphore_pend(varm_sem, BIOS_WAIT_FOREVER);	
Init();
ReadCfg();//读取配置信息
//timer.SetTimer(&timer_id,1,callback_time);
while(1)
{
	//PFUNC(TEM_INFO,"scada run\r\n");
	sleep(3000);
	Caculate();
	//GetYx();
	//CHECK_TASK_EXIT(((TASK*)arg));
}
//timer.KillTimer(timer_id);
return 1;
}

int16 Scada::GetMsg(MessageData<BaseDataType>message){
	PFUNC(TEM_INFO,"deal %s Message\r\n",name);
	if(message.MessageId==MSG_YK)
		{
			DataType1*yk_data;
			yk_data=(DataType1*)&message.Data;
			ControlYk(message.Id,yk_data);
		}
	if(message.MessageId==MSG_PARA)
		{
			//if(message.Id==NO1_FIRST_SECTION_CURRENT_SET){
				if(message.Id==PARA_5001){	
			memcpy(&scada_cfg,(ScadaPara*)&message.Data,sizeof(scada_cfg));
		}
		}
return 1;		
}

/////////////////////私有函数/////////////////////////////////////////////////
int16 Scada::Caculate(){
//	Data<BaseDataType> yx;
//	DataType1 d1;
//	//DataType1 *dd1;
//	CP56Time2a time;
//	d1.Data=22;
//	yx.Data=&d1;
//	timer.GetCP56Time(&time);
//	msg.SendMsg(TASK_VARM,MSG_YX,YX_0218,&d1,&time);//向VARM线程发送消息
//	user_data.SetData(YX_0218,&yx);	
//	user_data.GetData(YX_0218,&yx);	
	//dd1=(DataType1*)yx.Data;
	//PFUNC(TEM_INFO,"%d ",dd1->Data);
	DataType1 d1;	
	CP56Time2a time;
	d1.Data=4;
	//timer.GetCP56Time(&time);
	memset((char*)&time,3,sizeof(time));
	//Semaphore_pend(varm_sem, BIOS_WAIT_FOREVER);
	msg.SendMsg(TASK_VARM,MSG_YX,YX_0218,&d1,&time);//向VARM线程发送消息	
	Data<BaseDataType> yc;
	DataType4 d2;
	//DataType4 *dd2;
	yc.Data=&d2;
	d2.Data=22.23;
	user_data.SetData(YC_4001,&yc);	
	user_data.GetData(YC_4001,&yc);
	//dd2=(DataType4*)yc.Data;
	//PFUNC(TEM_INFO,"%f ",dd2->Data);
	msg.SendMsg(TASK_VARM,MSG_YC,YC_4001,&d2,&time);//向VARM线程发送消息		
//
//sleep(5);
	Data<BaseDataType> para;
	EthPara d3;
	//EthPara *dd3;
	para.Data=&d3;	
	d3.Data.LocalPort=600;
	d3.Data.ProSel=3;
	user_data.SetData(PARA_5102,&para);	
	user_data.GetData(PARA_5102,&para);
	//dd3=(EthPara*)para.Data;
	//PFUNC(TEM_INFO,"%d ",dd3->Data.LocalPort);
	
	UserRealDataDefPara d4;
	d4.Data.Ua.BaseValue=220.0;

	
	//msg.SendMsg(TASK_VARM,MSG_PARA,PARA_5102,&d3,&time);//向FA线程发送消息	
	//msg.SendMsg(TASK_VARM,MSG_PARA,PARA_5212,&d4,&time);//向FA线程发送消息	
	return 1;
}
int16 Scada::GetYx(){
	return 1;
}

int16 Scada::ControlYk(int16 DataNo,DataType1*){
return 1;	
}

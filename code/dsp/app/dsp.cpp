#include "scada.h"
#include "fa.h"
#include "task.h"
#include "message.h"
#include "varm.h"
#include "interruptapp.h"


Task task,*g_task;
int main(int argc, char* argv[])
{
	g_task=&task;
	//interrupt_int();


	task.Init();

	task.Regist(&scada,scada_main);

	task.Regist(&varm,varm_main);

	task.StartThread(scada.Id);

	task.StartThread(varm.Id);





}


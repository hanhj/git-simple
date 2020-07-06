#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>//exit
#include "timer.h"
#include "task.h"
#include "message.h"
#include "com.h"
#include "protocol.h"
#include "statis.h"
#include "vdsp.h"
#include "os.h"
#include "utils.h"

#define FIFO_WR APP_ROOT"fifo_wr"
#define FIFO_RD APP_ROOT"fifo_rd"
Task task, *g_task;
int process_fp[2];
int main ( void )
{
    int cmd;

    g_task = &task;
    task.Init();
    task.Regist ( &msg, message_main );
    task.Regist ( &timer, timer_main );
    task.Regist ( &comm, comm_main );
    task.Regist ( &protocol, protocol_main );
    task.Regist ( &statis, statis_main );
#ifdef CONFIG_SYSLINK
    task.Regist ( &vdsp, vdsp_main );
#endif
    task.StartThread ( msg.Id );//要放在第一个位置，因为在init函数中要清消息注册表
    mdelay ( 10 );
    task.StartThread ( timer.Id );
    mdelay ( 10 );
    task.StartThread ( comm.Id );
    mdelay ( 10 );
    task.StartThread ( protocol.Id );
    mdelay ( 10 );
    task.StartThread ( statis.Id );
#ifdef CONFIG_SYSLINK
    mdelay ( 10 );
    task.StartThread ( vdsp.Id );
#endif
    user_data.LoadPara();
    
#ifdef WITH_MANAGE
	process_fp[0]=myopen(FIFO_WR,O_RDONLY);
	if(process_fp[0]<0)
		PFUNC(TEM_WARNING,DEBUG_NORMAL,"can't open %s\n",FIFO_WR);
	process_fp[1]=myopen(FIFO_RD,O_WRONLY);
	if(process_fp[1]<0)
		PFUNC(TEM_WARNING,DEBUG_NORMAL,"can't open %s\n",FIFO_RD);
#endif		
    while ( 1 ) {
        mdelay ( 1000 );
        if(task.CheckHeart()<0)//检查线程出错，如果错误超过DEFAULT_RESTART_TIMES=2后退出程序
        	break;
//与父进程通信代码////////////////////////////////////////
#ifdef WITH_MANAGE
		if ( recv_cmd ( process_fp[0], &cmd ) == 0 ) {        	
            switch ( cmd ) {

            case ALIVE:
                //PFUNC ( TEM_INFO, DEBUG_NORMAL,"check alive cmd comming 0x%x\n", ALIVE );
                send_cmd ( process_fp[1], ALIVE_ACK );
                break;

            case QUIT:
                goto quit;
                break;

            case UPDATING:

            case SUCCESS:

            case FAILED_NO_AUTH:

            case FAILED_OLD_VERSION:

            case FAILED_TAR:
                /* TODO Notify GUI Draw */
                break;

            default:
                break;
            }
        }
#endif
//////////////////////////////////////////////////////////////////////
    }

quit:
#ifdef WITH_MANAGE	
	close(process_fp[0]);
	close(process_fp[1]);
#endif	
    exit ( 0 );

}

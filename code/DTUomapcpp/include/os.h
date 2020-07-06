#ifndef OS_H
#define OS_H

#include "types.h"
#include "user.h"
#ifdef CONFIG_LINUX
#include <pthread.h>
#define	STDIN_FILENO	0
#define	STDOUT_FILENO	1
#define	STDERR_FILENO	2

#endif


#ifndef CONFIG_LINUX
#include <xdc/std.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>

/* package header files */
#include <ti/ipc/Ipc.h>
#include <ti/ipc/MultiProc.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>


#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>
/* package header files */
#include <ti/ipc/Ipc.h>
#include <ti/ipc/MultiProc.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
//#include <ti/sysbios/hal/Timer.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sdo/utils/List.h> /* List module in IPC */
#include <xdc/cfg/global.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/timers/timer64/Timer.h>
#endif

struct mutex
{
	uint8 lock[64];
	int lock_count;
	char name[32];
};

struct cond {
	char name[32];
	int count;
	#ifdef CONFIG_LINUX
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	#endif
};

int thread_cond_init(struct cond *cond, char *name);
int thread_signal(struct cond *cond);
int thread_wait_cond(struct cond *cond);
int thread_cond_destroy(struct cond *cond);
/**
 * 初始化mutex
 *
 * @param m
 *
 * @return
 */
int mutex_init(struct mutex *m, const char *name);

/**
 * 销毁mutex
 *
 * @param m
 *
 * @return
 */
int mutex_destroy(struct mutex *m);

/**
 * 加锁
 *
 * @param m
 *
 * @return
 */
int mutex_lock(struct mutex *m);

/**
 * 解锁
 *
 * @param m
 *
 * @return
 */
int mutex_unlock(struct mutex *m);

#ifdef CONFIG_LINUX
#include <pthread.h>
#define DECLARE_MUTEX(M) static pthread_mutex_t M = PTHREAD_MUTEX_INITIALIZER
#define MUTEX_LOCK(M) pthread_mutex_lock(&M)
#define MUTEX_UNLOCK(M) pthread_mutex_unlock(&M)
#endif
typedef void (*sighandle_t)( int sig_num );

/**
 * 初始化信号设置
 * @param signum 待处理信号
 * @param sigmask 屏蔽信号
 * @handle 信号处理函数
 */
void init_signal(int signum, int sigmask, sighandle_t handle);

/**
 * 求文件大小
 * @param name 文件名
 * @return -1:失败
 *          0：成功
 */
int file_size(const char *name);

/************************ read meter interface ******************/
enum {
	TAB_485,
	TAB_PLC,
};

void init_read_port(int tb_id, int baudrate, int databit, int stopbit, int checkbit);

void close_read_port(int tb_id);

void set_read_port(int tb_id, int port, int baudrate, int databit, int stopbit, int checkbit);

int recv_read_port(int tb_id, int port, uint8 *c);

int send_read_port(int tb_id, int port, uint8 *buf, uint16 len);

int map_port(int tb_id, int meter_port);
#endif

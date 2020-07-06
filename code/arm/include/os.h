#ifndef OS_H
#define OS_H

#include "types.h"
#include "user.h"
#ifdef CONFIG_LINUX
#include <pthread.h>
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#endif

struct mutex {
    uint8 lock[64];
    int lock_count;
    char name[32];
#ifndef CONFIG_LINUX
    Semaphore_Handle sem;
#endif
};

struct cond {
    char name[32];
    int count;
#ifdef CONFIG_LINUX
    pthread_mutex_t mutex;
    pthread_cond_t cond;
#endif
};

int thread_cond_init ( struct cond *cond, const char *name );
int thread_signal ( struct cond *cond );
int thread_signal_all ( struct cond *cond );
int thread_wait_cond ( struct cond *cond );
int thread_cond_destroy ( struct cond *cond );
/**
 * 初始化mutex
 *
 * @param m
 *
 * @return
 */
int mutex_init ( struct mutex *m, const char *name );

/**
 * 销毁mutex
 *
 * @param m
 *
 * @return
 */
int mutex_destroy ( struct mutex *m );

/**
 * 加锁
 *
 * @param m
 *
 * @return
 */
int mutex_lock ( struct mutex *m );

/**
 * 解锁
 *
 * @param m
 *
 * @return
 */
int mutex_unlock ( struct mutex *m );

#ifdef CONFIG_LINUX
#define DECLARE_MUTEX(M) static pthread_mutex_t M = PTHREAD_MUTEX_INITIALIZER
#define MUTEX_LOCK(M) pthread_mutex_lock(&M)
#define MUTEX_UNLOCK(M) pthread_mutex_unlock(&M)
#endif
typedef void ( *sighandle_t ) ( int sig_num );

/**
 * 初始化信号设置
 * @param signum 待处理信号
 * @param sigmask 屏蔽信号
 * @handle 信号处理函数
 */
void init_signal ( int signum, int sigmask, sighandle_t handle );

int send_cmd ( int fd, int cmd );
int recv_cmd ( int fd, int *cmd );
/**
 * 求文件大小
 * @param name 文件名
 * @return -1:失败
 *          0：成功
 */
int file_size ( const char *name );
/*
 * this structure is for RTC time 
 */
struct rtc_time_stu {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};
/**
 * 设置RTC值
 *
 * @param buf 格式[秒分时天月年]
 *
 * @return
 */
int set_rtc(struct rtc_time_stu *rtc_time);
/**
 * 设置RTC值
 *
 * @param buf 格式[秒分时天月年]
 *
 * @return
 */
int set_alm_rtc(struct rtc_time_stu *rtc_time);
/**
 * 重启终端
 *
 * @return
 */
int restart(void);
/**
 * 拷贝文件
 *
 * @param src    源文件
 * @param dst    目标文件
 *
 * @return 0: 成功
 *         1: 失败
 */
int copy(const char *src, const char *dst);
/**
 * 同步文件系统，将缓存文件写到Flash
 *
 * @return
 */
int sync_fs(void);
#include "delay.h"
extern int process_fp[2];//进程间通讯句柄
#endif

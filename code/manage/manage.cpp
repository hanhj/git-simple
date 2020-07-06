/*
 * manage_app/main.c
 *
 * Manage process
 *
 * Copyright (C) 2010 ZXJY qianwei@zxjydomain.com
 *           yousheng@zxjydomain.com
 *
 */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <dirent.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <fcntl.h>


#include "types.h"
#include "user.h"
#include "custom.h"
#include "debug.h"
#include "localtime.h"
#include "gpio.h"
#include "os.h"
#include "utils.h"


#define MAX_PROCESS     1

#define DEFAULT_VITALITY 8//20

#define UDISK  "/mnt/udisk"

#ifndef CHK_POWER_PIN
/*FIXME:*/
//#include "ac_sample.h"
//PowerPara g_power;
#endif

enum {
    BOOTSTRAP,
    UBOOT,
    KERNEL
};

enum {
    ERASE,
    WRITE
};

char *g_cmd[3][2] = {
    {"mtd_debugx erase /dev/mtd0 0x20000  0x20000", "mtd_debugx write /dev/mtdblock0 0x20000 0x%x /home/root/data/update/UBL_NAND.ais"},
    {"mtd_debug erase /dev/mtd0 0x40000  0xa50000", "mtd_debug write /dev/mtdblock0 0x40000 0x%x /home/root/data/update/u-boot_head.bin"},
    {"mtd_debug erase /dev/mtd0 0x100000 0x350000", "mtd_debug write /dev/mtdblock0 0x100000 0x%x /home/root/data/update/uImage_omapl138"},
};

static int file_size ( char *path );
static int get_file_name ( char *str, char *name );
static int do_cmd ( int cmd, int subcmd );
static int kernel_update ( void );
static int bootstrap_update ( void );
static int uboot_update ( void );
static int firmware_update ( void );
static int probe_udisk ( void );
static int untar ( const char *package );

static void init_processinfo ( void );
static int do_update ( void );
static int start_app ( int index );
static int fill_processinfo ( int i, pid_t pid, char *p, int vitality );
static void monitor_child ( void );
static void read_child ( void );
static int send_msg_to_quit();
static int all_active_pro_exit ( void );
static void start_watchdog ( void );
static void local_update ( void );

static void kill_app ( int index );
static int remote_update();
static void feed_dog ( union sigval t );
void SigChild ( int sig_num );

static volatile int need_reboot = 0;
static int up_ok = 0;
static int version = 0;
static int new_ver = 0;
static timer_t feedog_timer;
static int fifo_fd[MAX_PROCESS][2];


#define CONFIG_EXEC APP_ROOT"arm"
static char *process_name[MAX_PROCESS] = {
#ifdef CONFIG_EMULATE
    CONFIG_EXEC,
#else
    CONFIG_EXEC,
#endif
};
#define FIFO_WR APP_ROOT"fifo_wr"
#define FIFO_RD APP_ROOT"fifo_rd"



typedef struct tag_PROCESS_INFO {
    pid_t pid;
    char name[64];
    int vitality;
} process_info_t;

process_info_t g_process_info[MAX_PROCESS];

static char dev_path[5];
static char mount_dev_path[12];
static char proc_buf[600];
static int dev_read_size;
static int c_is_num ( char *ch )
{
    if ( ch == NULL )
        return 0;

    return ( ( *ch >= '0' ) && ( *ch <= '9' ) );
}

static int c_is_cha ( char *ch )
{
    if ( ch == NULL )
        return 0;

    return ( ( *ch >= 'a' ) && ( *ch <= 'z' ) );

}

static int s_is_cha_num ( char *ch )
{
    if ( ch == NULL )
        return 0;

    if ( ch + 1 > proc_buf + dev_read_size - 1 )
        return 0;

    if ( ( c_is_cha ( ch ) && c_is_num ( ch + 1 ) ) )
        return 1;

    return 0;
}

static int s_is_cha ( char *ch )
{
    if ( ch == NULL )
        return 0;

    if ( ch > proc_buf + dev_read_size - 1 )
        return 0;

    if ( c_is_cha ( ch ) )
        return 1;

    return 0;
}

static int find_valid_char_num ( void )
{
    char *p = proc_buf;

    while ( ( p = strstr ( p, "sd" ) ) != NULL ) {
        if ( s_is_cha_num ( p + 2 ) ) {
            memcpy ( dev_path, p, 4 );
            dev_path[4] = '\0';
            PFUNC ( TEM_INFO,DEBUG_NORMAL, "%s\n", dev_path );
            return 1;
        }

        if ( ( p + 2 ) <= proc_buf + dev_read_size - 1 )
            p += 2;
        else
            return 0;
    }

    return 0;
}

static int find_valid_char ( void )
{
    char *p = proc_buf;

    while ( ( p = strstr ( p, "sd" ) ) != NULL ) {
        if ( s_is_cha ( p + 2 ) ) {
            memcpy ( dev_path, p, 3 );
            dev_path[3] = '\0';
            PFUNC ( TEM_INFO,DEBUG_NORMAL, "%s\n", dev_path );
            return 1;
        }

        if ( ( p + 2 ) <= proc_buf + dev_read_size - 1 )
            p += 2;
        else
            return 0;
    }

    return 0;
}

static int find_dev_path ( void )
{
    memset ( dev_path, 0 , 5 );

    if ( find_valid_char_num() )
        return 1;
    else if ( find_valid_char() )
        return 1;
    else
        return 0;
}

static int build_dev_path ( void )
{
    int fd;

    fd = open ( "/proc/partitions", O_RDONLY );

    if ( fd == -1 )
        return 0;

    dev_read_size = read ( fd, proc_buf, sizeof ( proc_buf ) );

    close ( fd );

    if ( ( dev_read_size == 0 ) && dev_read_size >= ( (int)sizeof ( proc_buf ) - 1 ) )
        return 0;

    proc_buf[dev_read_size] = '\0';

    memset ( mount_dev_path , 0, sizeof ( mount_dev_path ) );

    if ( find_dev_path() ) {
        sprintf ( mount_dev_path, "%s%s", "/dev/", dev_path );
        PFUNC ( TEM_INFO,DEBUG_NORMAL, "%s\n", mount_dev_path );
        return 1;
    }

    return 0;
}

int main ( int argc, char *argv[] )
{
    time_t time1, time2 ;

    init_signal ( SIGCHLD, SIGALRM, SigChild );
    start_watchdog();
    init_processinfo();
    time ( &time1 );

    while ( 1 ) {
        time ( &time2 );

        if ( abs ( time2 - time1 ) > 0 ) {//1秒检测一次主程序，共有10次机会
            monitor_child();
            PFUNC ( TEM_INFO,DEBUG_NORMAL, "monitor %d\n" , time2 );
            time1 = time2;
        }

        read_child();

        if ( probe_udisk() != 1 ) {
            up_ok = 0;
        } else {
            if ( up_ok != 0 )  //update finished
                continue;

            sleep ( 10 );

            local_update();

            up_ok = 1;
        }

        sleep ( 1 );
    }

    return 0;
}

static void do_reboot ( void )
{
    need_reboot = 1;
    reboot ( 0x1234567 );
}

static int para_in_out ( void )
{
    int ret;
    char cmd[300];
	sprintf(cmd,"cp /mnt/udisk/ParaCopy.sh %s",APP_ROOT);
    if ( ( ret = system ( cmd ) ) < 0 ) {
        return -1;
    }

    if ( WEXITSTATUS ( ret ) != 0 )
        return -1;

    sync();
    sprintf(cmd,"chmod 777 %s/ParaCopy.sh",APP_ROOT);
    system ( cmd );
    sprintf(cmd,"%s/ParaCopy.sh",APP_ROOT);
		if ( ( ret = system ( cmd ) ) < 0 ) {
        return -1;
    }
    sprintf(cmd,"rm %s/ParaCopy.sh",APP_ROOT);
    system ( cmd );
    return 0;
}

static void local_update()
{
    int i;
    char path[128];
    FILE *fp;
    int ret = 0;
    PFUNC ( TEM_WARNING,DEBUG_NORMAL, "local update\n" );

    if ( !build_dev_path() ) {
        PFUNC ( TEM_INFO,DEBUG_NORMAL, "cant find dev\n" );
        goto out;
    }

    PFUNC ( TEM_INFO,DEBUG_NORMAL, "%s\n" , mount_dev_path );

    if ( ( ret = mount ( mount_dev_path, UDISK, "vfat", MS_NOEXEC, 0 ) ) < 0 ) {
        PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "mount error %d\n" , ret );
        sleep ( 1 );
        umount ( UDISK );
        sleep ( 10 );
        goto out;
    }


    ret = para_in_out();

    if ( ret < 0 ) {
        PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "copy para failed\n" );
    } else {
        PFUNC ( TEM_INFO,DEBUG_NORMAL, "para copy success\n" );
    }

    sprintf ( path, "%s/update.tgz", UDISK );

    if ( ( fp = fopen ( path, "r" ) ) == NULL ) {
        PFUNC ( TEM_WARNING,DEBUG_NORMAL, "update file \"%s\" not found\n", path );
        goto umount_out;
    } else fclose ( fp );

    if ( untar ( path ) < 0 ) {
        PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "update file \"%s\" untar error\n", path );
        umount ( UDISK );
        up_ok = 1;

        for ( i = 0; i < MAX_PROCESS; i++ )
            send_cmd ( fifo_fd[i][1], FAILED_TAR );

        goto out;
    }

    PFUNC ( TEM_INFO,DEBUG_NORMAL, "untar success\n" );

    send_msg_to_quit();

    if ( do_update() == -1 ) {
        PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "do_update error\n" );
        goto umount_out;
    }

    /* 不做版本检查
    if ((check_result = update_check()) == 1) { //new app
     send_msg_to_quit();
     if (do_update() == -1) {
      PFUNC(TEM_ERROR ,DEBUG_NORMAL, "do_update error\n");
      return;
     }
     PFUNC(TEM_INFO,DEBUG_NORMAL, "untar finished\n");

    }
    else {
     switch (check_result) {
      case -1:
       update_cmd = FAILED_TAR;
       break;
      case -2:
       update_cmd = FAILED_NO_AUTH;
       break;
      case -3:
       update_cmd = FAILED_OLD_VERSION;
       break;
      default:
       update_cmd = FAILED_TAR;
       break;
     }
     for (i = 0; i < MAX_PROCESS; i++)
      send_cmd(fifo_fd[i][1], update_cmd);
    }
    */

umount_out:
    PFUNC ( TEM_INFO,DEBUG_NORMAL, "umount disk\n" );

    umount ( UDISK );

out:
    //beep_us ( 2000*1000 );
    ;
}
static void init_processinfo ( void )
{
    int i;
    pid_t pid;

    memset ( g_process_info, 0, sizeof ( g_process_info ) );
    mkfifo(FIFO_WR,666);
    mkfifo(FIFO_RD,666);

    for ( i = 0; i < MAX_PROCESS; i++ ) {
        fill_processinfo ( i, 0, process_name[i], 0 );
	    fifo_fd[i][0]=open(FIFO_RD,O_RDWR);
	    if(fifo_fd[i][0]<0)
	    	perror("open fifo");
	    fifo_fd[i][1]=open(FIFO_WR,O_RDWR);
	    if(fifo_fd[i][1]<0)
	    	perror("open fifo");
    }


    for ( i = 0; i < MAX_PROCESS; i++ ) {
        if ( ( pid = fork() ) == 0 ) {   //child process
            char *argv[] = {NULL, NULL};
            argv[0] = g_process_info[i].name;

            if ( execve ( g_process_info[i].name, argv, NULL ) == -1 )
                exit ( EXIT_FAILURE );
        } else if ( pid > 0 ) {  //parent process
            g_process_info[i].pid       = pid;
            g_process_info[i].vitality = DEFAULT_VITALITY;
            PFUNC ( TEM_INFO,DEBUG_NORMAL, "%s is started\n", g_process_info[i].name );
        } else {
            exit ( 1 );
        }
    }
}

static int start_app ( int index )
{
    pid_t pid;

    if ( ( pid = fork() ) == 0 ) {    //child process
        char *argv[] = {NULL, NULL};

        PFUNC ( TEM_INFO,DEBUG_NORMAL, "****** %s started *******\n", g_process_info[index].name );
        argv[0] = g_process_info[index].name;

        if ( execve ( g_process_info[index].name, argv, NULL ) == -1 )
            exit ( EXIT_FAILURE );

    } else if ( pid > 0 ) {     //parent process
	    fifo_fd[index][0]=open(FIFO_RD,O_RDWR);
	    if(fifo_fd[index][0]<0)
	    	perror("open fifo");
	    fifo_fd[index][1]=open(FIFO_WR,O_RDWR);
	    if(fifo_fd[index][1]<0)
	    	perror("open fifo");        
        fill_processinfo ( index, pid, g_process_info[index].name, DEFAULT_VITALITY );
    } else {   //fork failed
        return -1;
    }

    return 0;
}


static int copy_bin_file ( void )
{
    int ret;
    PFUNC ( TEM_WARNING,DEBUG_NORMAL, "\n" );

    if ( ( ret = system ( "cp -rf /home/root/data/update/* /" ) ) < 0 ) {
        PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "do_update error\n" );
        return -1;
    }

    if ( WEXITSTATUS ( ret ) != 0 )
        return -1;

    sync();

    return 0;
}


static int do_update ( void )
{
    sleep ( 2 );

    if ( all_active_pro_exit() ) {
        PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "until process exist before update\n" );
        return -1;
    }

    if ( copy_bin_file() < 0 ) {
        PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "do_update error\n" );
        return -1;
    }

    firmware_update();//更新固件版本号，bootstrap, uboot, kernel

    //system ( "rm -rf /home/root/firmware" );
    system ( "rm -rf /home/root/data/update/*" );
    version = new_ver;

    return 0;
}

static int untar ( const char *package )
{
    int ret;
    char buffer[128];
    sprintf ( buffer, "tar zxfv %s -C /home/root/data/update", package );
    PFUNC ( TEM_WARNING,DEBUG_NORMAL, "buffer=%s\n", buffer );

    if ( ( ret = system ( buffer ) ) < 0 ) {
        PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "tar error\n" );
        return -1;
    }

    if ( WEXITSTATUS ( ret ) != 0 )
        return -1;

    sync();

    return 0;
}


/* probe u-disk
 * return: -1, failed
 *     1, u-disk be probed
 *     0, u-disk not found
 **/
static int probe_udisk ( void )
{
    if ( access ( "/proc/scsi/usb-storage", F_OK ) == -1 ) {
        if ( errno == EACCES || errno == ENOENT )
            return 0;
        else
            return -1;
    } else {
        return 1;
    }
}

static int all_active_pro_exit ( void )
{
    int alive_count = 0;
    int i;
    time_t quit_time;
#define QUIT_MAX_TIME 60

    time ( &quit_time );

wait_allexit:

    for ( i = 0 ; i < MAX_PROCESS; i++ ) {
        if ( g_process_info[i].vitality != 0 ) {
            alive_count++;
        }
    }

    PFUNC ( TEM_INFO, DEBUG_NORMAL,"all_active_pro_exit\r\n" );

    if ( ( time ( NULL ) - quit_time ) >= QUIT_MAX_TIME )
    	for ( i = 0 ; i < MAX_PROCESS; i++ ) {
        kill_app(i);
	}
    if ( alive_count > 0 ) {
        alive_count = 0;
        goto wait_allexit;
    }

    return alive_count;
}

static int send_msg_to_quit()
{
    int i;

    for ( i = 0 ; i < MAX_PROCESS; i++ )
        send_cmd ( fifo_fd[i][1], QUIT );

    return 0;
}

void kill_app ( int index )
{
    int status;
    int kill_count;
    pid_t pid;

//    for ( i = 0 ; i < MAX_PROCESS; i++ ) 
    {
        kill_count = 5;
        pid = g_process_info[index].pid;
	    PFUNC ( TEM_WARNING,DEBUG_NORMAL, "kill app:%s\n", g_process_info[index].name);

        if ( pid <= 0 )
            return ;

        while ( kill_count-- ) {
            if ( kill ( pid, 0 ) == 0 ) {
                kill ( pid, 9 );
                sleep ( 1 );
                PFUNC ( TEM_WARNING,DEBUG_NORMAL, "!!!! pid = %d, pid name:%s\n", pid, g_process_info[index].name );
                wait4 ( pid, &status, WNOHANG, NULL );
                sleep ( 2 );
            } else {
                break;
            }
        }

        if ( kill_count <= 0 ) {
            do_reboot();
        } else {
            g_process_info[index].vitality = 0;
            g_process_info[index].pid = 0;
        }
    }
}

static void restart_app ( int index )
{
#define QUIT_TIME_OUT (3*60)
#define QUIT_TIMES 2
    /*应用程序的在3分钟之内的退出次数超过2次，认为应用有不可修复的异常，重启终端*/
    static time_t app_exit_period;
    static int times = 0;

    if ( times == 0 ) {
        app_exit_period = time ( NULL );
    }

    times++;

    if ( abs ( time ( NULL ) - app_exit_period ) <= QUIT_TIME_OUT ) {
        if ( times >= QUIT_TIMES ) {
            PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "\n" );
            do_reboot();
            need_reboot = 1;
        }
    } else
        times = 0;

    close ( fifo_fd[index][0] );
	close ( fifo_fd[index][1] );	


    start_app ( index );
}

/*
 * return:1, some process quit
 *   0, no process quit
 *
 **/
static void monitor_child ( void )
{
    int i;

    for ( i = 0; i < MAX_PROCESS; i++ ) {
        send_cmd ( fifo_fd[i][1], ALIVE );
        --g_process_info[i].vitality;
        //PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "vitality=%d\n", g_process_info[i].vitality );
    }

    for ( i = 0; i < MAX_PROCESS; i++ ) {
        if ( g_process_info[i].vitality <= 0 ) {
            PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "kill=%d\n", g_process_info[i].vitality );
            kill_app(i);
            restart_app ( i );
        }
    }
}

static void read_child ( void )
{
    int i;

    for ( i = 0; i < MAX_PROCESS; i++ ) {
        int cmd;
        int ret = recv_cmd ( fifo_fd[i][0], &cmd );

        if ( ret != 0 )
            continue;

        PFUNC ( TEM_INFO ,DEBUG_NORMAL, "recv_cmd 0x%x\n", cmd );

        switch ( cmd & 0xFFFF ) {

        case ALIVE_ACK:
            //PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "child ack alive 0x%x\n", ALIVE_ACK );
            g_process_info[i].vitality = DEFAULT_VITALITY;
            break;

        case UPGRADE:
            //PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "remote upgrade cmd comming 0x%x\n", UPGRADE );
            remote_update();
            break;
            
        case RESTART_APP:
            kill_app(i);
            restart_app ( i );
            break;
            

        case NO_FEEDING_DOG:
            //PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "need hard reset cmd comming 0x%x\n", NO_FEEDING_DOG );
            need_reboot = 1;
            break;

        default:
            break;
        }
    }
}

/*
 * return: -1, failed
 *     0, success
 * */
static int fill_processinfo ( int i, pid_t pid, char *p, int vitality )
{
    if ( i >= MAX_PROCESS )
        return -1;

    g_process_info[i].vitality = vitality;

    g_process_info[i].pid           = pid;

    strcpy ( g_process_info[i].name, p );

    return 0;
}

static int start_feedog ( int time )
{

    struct itimerspec ts;

    ts.it_interval.tv_sec = time;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = time;
    ts.it_value.tv_nsec = 0;

    return timer_settime ( feedog_timer, 0, &ts, NULL );
}

static int create_feedog ( void )
{

    struct sigevent evp;

    memset   ( &evp,   0,   sizeof   ( evp ) );
    evp.sigev_signo = SIGUSR1;
    evp.sigev_value.sival_ptr = &feedog_timer;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = feed_dog;


    return timer_create ( CLOCK_REALTIME, &evp, &feedog_timer );
}
//停止内核喂狗
void shutdown_pwm()
{
system("echo 0 > /sys/class/misc/omapl138_feedog/feedog");
}
static int l_state = 0; //when run app, feedog pin is high
extern GPIO gpio;
void start_watchdog ( void )
{
    gpio.Write( FEED_DOG_PIN, l_state );

    if ( create_feedog() )
        PFUNC ( TEM_INFO,DEBUG_NORMAL, "timer_create" );

    start_feedog ( 1 );

    //TODO:
    /* stop timer control and start gpio control ,feed dogs */
    shutdown_pwm();
    gpio.Init(FEED_DOG_PIN,GPIO_DIR_OUT,0);
}

void feed_dog ( union sigval t )
{
    if ( need_reboot )
        return;

    l_state = l_state ? 0 : 1 ;

	gpio.Write( FEED_DOG_PIN, l_state );
}

int remote_update()
{


    if ( untar ( UPGRADE_FILE_NAME ) < 0 ) {
        PFUNC ( TEM_WARNING,DEBUG_NORMAL, "untar fail\n" );
        return -1;
    }

	//不做版本检查
    /*
    if (( update_check()) == 1) { //new app
     send_msg_to_quit();
     if (do_update() == -1) {
      PFUNC(TEM_ERROR ,DEBUG_NORMAL, "do_update error\n");
      return -1;
     }
    }
    */
    
    send_msg_to_quit();

    if ( do_update() == -1 ) {
        PFUNC ( TEM_ERROR ,DEBUG_NORMAL, "do_update error\n" );
        return -1;
    }

    beep_us ( 2000*1000 ); //beep 2 s
    do_reboot();

    return 0;
}


void SigChild ( int sig_num )
{
    pid_t pid;
    int i;

    while ( ( pid = waitpid ( -1, NULL, WNOHANG ) ) > 0 ) {
        for ( i = 0; i < MAX_PROCESS; i++ ) {
            if ( pid == g_process_info[i].pid ) {
                g_process_info[i].pid = 0;
                g_process_info[i].vitality = 0;
                PFUNC ( TEM_INFO,DEBUG_NORMAL, "%s child signal is catched\n", g_process_info[i].name );
                break;
            }
        }

        if ( i == MAX_PROCESS ) {
            PFUNC ( TEM_WARNING,DEBUG_NORMAL, "All process exit\n" );

            for ( i = 0; i < MAX_PROCESS; i++ ) {
                kill_app(i);
                restart_app ( i );
            }
        }
    }
}

int file_size ( char *path )
{

    struct stat attr;
#if 1

    if ( stat ( path, &attr ) < 0 )
        return 0;

#endif
    return attr.st_size;
}

int get_file_name ( char *str, char *name )
{
    char *str1, *token, *last_token;
    char s[128];

    last_token = NULL;
    strcpy ( s, str );

    for ( str1 = s; ;str1 = NULL ) {
        token = strtok ( str1, " " );

        if ( token == NULL )
            break;

        last_token = token;
    }

    if ( last_token != NULL ) {
        strcpy ( name, last_token );
        return 0;
    } else {
        return -1;
    }
}

/*
升级状态标志
----------------|---——————|---——————|---——————|---——————|---——————|---——————|---——————|
| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
--------------------------------|------------------------------------------------------------------------------------------------
| 备用 | 备用 | 备用 | 备用 | 备用 | 备用 |执行命令成功标志|文件存在标志 |
---------------------------------------------------------------------------------------------------------------------------------
*/
int do_cmd ( int cmd, int subcmd )
{
    int ret, size;
    int sta;
    char cmds[128], name[128];
    sta = 0;
    get_file_name ( g_cmd[cmd][WRITE], name );
    size = file_size ( name );

    if ( size <= 0 ) {
        PFUNC ( TEM_WARNING,DEBUG_NORMAL, "%s not found\n", name );
        return sta;
    }

    sta |= 1;

    if ( subcmd == WRITE ) {
        sprintf ( cmds, g_cmd[cmd][subcmd], size );
        PFUNC ( TEM_WARNING,DEBUG_NORMAL, "cmds=%s \n", cmds );
    } else {
        strcpy ( cmds, g_cmd[cmd][subcmd] );
        PFUNC ( TEM_WARNING,DEBUG_NORMAL, "cmds=%s \n", cmds );
    }


    if ( ( ret = system ( cmds ) ) < 0 )
        return sta;

    if ( WEXITSTATUS ( ret ) != 0 )
        return sta;

    sta |= 2;

    return sta;
}

#define update_firmware(TYPE, RET) do {  \
 if ((RET = do_cmd(TYPE, ERASE)) ==0) \
  break;    \
 if ((RET = do_cmd(TYPE, WRITE)) == 0)  \
  break;    \
} while(0)

int kernel_update ( void )
{
    int ret = 0;
    update_firmware ( KERNEL, ret );
    return ret;
}

int bootstrap_update ( void )
{
    int ret = 0;
    update_firmware ( BOOTSTRAP, ret );
    return ret;
}

int uboot_update ( void )
{
    int ret = 0;
    update_firmware ( UBOOT, ret );
    return ret;
}

int firmware_update ( void )
{
    int i, j, k, ret[3];
    i = j = 0;
    ret[0] = kernel_update();
    ret[1] = uboot_update();
    ret[2] = bootstrap_update();

    for ( k = 0;k < 3;k++ ) {
        i += ( ret[k] & 1 );
        j += ( ( ret[k] >> 1 ) & 1 );
    }

    PFUNC ( TEM_WARNING,DEBUG_NORMAL, "ret1 = %d, ret2 = %d, ret3 = %d\n", ret[0], ret[1], ret[2] );

    if ( i == j && i != 0 ) {
#if 0
        update_firm_ver();
#endif
        return 0;
    } else
        return -1;
}

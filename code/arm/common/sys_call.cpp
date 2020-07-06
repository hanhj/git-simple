#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "debug.h"
#include "os.h"

#define BUF_SIZ 512

int restart(void)
{
	return reboot(LINUX_REBOOT_CMD_RESTART);
}

int sync_fs(void)
{
	sync();
	return 0;
}

/*用于子进程退出捕获,设置捕获信号类型与处理函数*/
void init_signal(int signum, int sigmask, sighandle_t handle)
{
	struct sigaction sig_act; //SIG_CHLD

	memset(&sig_act, 0, sizeof(sig_act));
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_handler = handle;
	sig_act.sa_flags = ((SA_RESTART) & ~SA_RESETHAND);
	if (sigmask > 0)
		sigaddset(&(sig_act.sa_mask), sigmask);
	sigaction(signum, &sig_act, NULL);
/*忽略终端 I/O信号,STOP信号*/
	signal(SIGTTOU, SIG_IGN);//SIGTTOU（表示后台进程写控制终端）
	signal(SIGTTIN, SIG_IGN);//SIGTTIN（表示后台进程读控制终端）
	signal(SIGTSTP, SIG_IGN);//SIGTSTP（表示终端挂起）
	signal(SIGPIPE, SIG_IGN);//

}

/************************* copy file **************************/

ssize_t safe_read(int fd, void *buf, size_t count)
{
	ssize_t n;

	do {
		n = read(fd, buf, count);
	} while (n < 0 && errno == EINTR);

	return n;
}

static ssize_t safe_write(int fd, const void *buf, size_t count)
{
	ssize_t n;

	do {
		n = write(fd, buf, count);
	} while (n < 0 && errno == EINTR);

	return n;
}

static ssize_t bb_full_write(int fd, const void *buf, size_t len)
{
	ssize_t cc;
	ssize_t total;

	total = 0;
	while (len > 0) {
		cc = safe_write(fd, buf, len);
		if (cc < 0)
			return cc;	/* write() returns -1 on failure. */
		total += cc;
		buf = ((const char *)buf) + cc;
		len -= cc;
	}
	return total;
}

static int copyfd_eof(int fd1, int fd2)
{
	char buf[BUF_SIZ];
	int total, size;
	int ret = -1;

	size = 0;
	total = 0;
	if (fd1 < 0 || fd2 < 0)
		return -1;

	while (!size || total < size) {
		int nread, nwrite;

		nread = safe_read( fd1, buf, (!size || (size - total > BUF_SIZ)) ? BUF_SIZ : size - total );
		if (nread > 0) {
			nwrite = bb_full_write( fd2, buf, nread );
			if (nwrite < nread) {
				ret = -1;
				break;
			}
			total += nwrite;
			ret = total;
		} else if (nread < 0) {
			ret = -1;
			break;
		} else if (nread == 0) {
			ret = total;
			break;
		}
	}
	return ret;
}

int copy(const char *src, const char *dst)
{
	int src_fd, dst_fd;
	struct stat dest_stat, src_stat;
	int dest_exists = 0;

	if (lstat(dst, &dest_stat) < 0) {
		PFUNC(TEM_WARNING , DEBUG_NORMAL, "errno = %d, ENOENT = %d %s,%s\n", errno, ENOENT,src,dst);
		if (ENOENT != errno) {
			PFUNC(TEM_ERROR , DEBUG_NORMAL, "unable stat %s\n", dst);
			return -1;
		}
	} else {
		dest_exists = 1;
	}

	if (stat(src, &src_stat) < 0) {
		return -1;
	}

	src_fd = open(src, O_RDONLY);
	if (src_fd == -1) {
		PFUNC(TEM_ERROR , DEBUG_NORMAL, "unable to open `%s'", src);
		return(-1);
	}

	if (dest_exists) {
		dst_fd = open(dst, O_WRONLY|O_TRUNC);
		if (dst_fd == -1) {
			if (unlink(dst) < 0) {
				PFUNC(TEM_WARNING , DEBUG_NORMAL, "unable remove %s\n", dst);
				close(src_fd);
				return -1;
			}
			goto dest_removed;
		}
	} else {
dest_removed:
		dst_fd = open(dst, O_WRONLY|O_CREAT, src_stat.st_mode);
		if (dst_fd == -1) {
			PFUNC(TEM_ERROR , DEBUG_NORMAL, "unable to open %s\n", dst);
			close(src_fd);
			return(-1);
		}
	}
	if (copyfd_eof(src_fd, dst_fd) < 0) {
		close(src_fd);
		close(dst_fd);
		return -1;
	}

	close(src_fd);
	close(dst_fd);
	return 0;
}

int file_size(const char *name)
{
	struct stat file_stat;
	int size;

	if (lstat(name, &file_stat) < 0) {
		PFUNC(TEM_ERROR , DEBUG_NORMAL, "lsate %s error\n", name);
		return 0;
	}
	size = file_stat.st_size;
	/*
	if (size > 1024 * 1024) {
		PFUNC(TEM_WARNING , DEBUG_NORMAL, "The size of %s is %f M bytes\n", name, size/(1024.0*1024.0));
	} else if (size > 1024) {
		PFUNC(TEM_WARNING , DEBUG_NORMAL, "The size of %s is %f K bytes\n", name, size/1024.0);
	} else {
		PFUNC(TEM_WARNING , DEBUG_NORMAL, "The size of %s is %d bytes\n", name, size);
	}
	*/
	return size;
}


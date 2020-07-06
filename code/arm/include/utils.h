#ifndef _UTILS_H_
#define _UTILS_H_
/***************************INCLUDING FILES*********************************************/

/***************************************************************************************/
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
/*************************DEFINITION AND MICRO******************************************/
#define ENCODE 1
#define DECODE 0

#define CR 0x0d
#define LF 0x0a
#define CTRLZ 26
/***************************************************************************************/
/****************************EXTERN VARIABLES*******************************************/

/***************************************************************************************/
/****************************EXTERN FUNCTIONS*******************************************/
/**
 * 提取data中的n位有效数字，以整数形势输出
 *
 * @param data		待提取浮点数
 * @param n		有效数字位数
 * @param ldata		n位有效数字，以整数形势输出
 * @return		幂指数
 */
int8 significant_digits(float data, uint8 n, int32 *ldata);

/**
 * 将一个二进制数组转为长整型整数
 *
 * @param p		待转换二进制数据
 * @return		长整型整数
 */
int32 get_long( uint8 *p );

/**
 * 将一个二进制数组转为长整型整数
 *
 * @param p		待转换二进制数据
 * @return		short型整数
 */
int16 get_short( uint8 *p );

/**
 * bcd码转换为16进制数据
 *
 * @param bcd		bcd码数据
 * @return		二进制数据
 */
uint8 bcd2hex(uint8 bcd);
/****************************************************************************
* 名    称：Long2Float
* 功    能：将长整形数据按小数点位数转成对应的小数
* 入口参数：t 输入的浮点数，prec 小数点位数
* 出口参数：输出的格式化小数
说明比如将3320转换成3.220输出，小数点位数指示需要保存几位小数点。
* ****************************************************************************/
double Long2Float ( int32 t, int8 prec);
uint32 Float2Long ( double t, int8 prec);
/**
 * 数据排序
 *
 * @param p		需要排序的数据
 * @param p_size		需要排序数据的字节数
 * @return		null
 */
void sort(uint8 *p,uint8 p_size);
void sort_16(int16 *p,int16 p_size);
void sort_float_maxtomin(float_t *p,int16 p_size);//冒泡排序
/**
 *  时间转换为uint32_t型数据
 *
 * @param p_bcd		各个时间
 * @return：		转换后的uint32_t型数据
 */
uint32 time2d ( uint8 sec, uint8 min, uint8 hour, uint8 day, uint8 mon, uint8 year );
/**
 * n个二进制转n位BCD码
 *
 * @param bcd	转换后的bcd码存放首地址
 * @param n		转换后的bcd码的个数
 * @param hex	待转换二进制数据
 * @return
 */
void hex2bcd_n(uint8 *hex,uint8 *bcd,uint8 n);
/**
 * n个二进制转2n位ascii码
 *
 * @param dst	转换后的码存放首地址
 * @param size		需要转换的字节个数
 * @param hex	待转换二进制数据
 * @return
 */
void hex2ascii(uint8 *hex,uint8 *dst,uint8 size);
void rev_hex2ascii(uint8 *hex,uint8 *dst,uint8 size);
/**
 * n个BCD码转二进制数据
 *
 * @param bcd	输入的bcd码数据
 * @param n		bcd码的个数
 * @param hex	转换后的二进制数据
 * @return：
 */
void bcd2hex_n(uint8 *bcd,uint8 *hex,uint8 n);
/**
 * 求校验和
 *
 * @param buf		待求校验和数据
 * @param len		数据字节数
 * @return		校验和
 */
uint8 check_sum(const uint8* buf,uint16 len);
uint16 check_sum_16(const uint8 *buf, uint16 len);

/**
 * 二进制转BCD码
 *
 * @param hex		单字节二进制数据
 * @return 0：		失败
 *	   1：		成功
 */
uint8 hex2bcd(uint8 hex);

/**
 * 二进制转n位BCD码
 *
 * @param p_bcd		转换后的bcd码存放首地址
 * @param n		转换后的bcd码的位数
 * @param l_data	待转换二进制数据
 * @return TRUE：	转换成功
 */
uint8 hex2nbcd(uint8 *p_bcd,uint8 n,uint32 l_data);
/**
 * 对第n位清零
 *
 * @param data		待处理数据，单字节
 * @param n		位索引
 * @return 0：		失败
 *	   1：		成功
 */
uint8 clr_bit(uint8* data,uint8 n);
/**
 * 单字节第n位置位
 *
 * @param data		输入数据地址
 * @param n		位索引
 * @return 0：		失败
 *	   1：		成功
 */
uint8 set_bit(uint8* data,uint8 n);
/**
 * 单字节第n位判断是否置位
 *
 * @param data		输入数据地址
 * @param n		位索引
 * @return 0：		
 *	   1：		
 */
uint8 is_set(uint8 data,uint8 n);
/**
 * n位BCD码转二进制数据
 *
 * @param p_bcd		输入的bcd码数据
 * @param n		bcd码的位数
 * @return：		二进制数
 */
uint32 nbcd2hex(uint8* p_bcd,uint8 n);

/**
 * 二进制数组数据转长整型二进制数
 *
 * @param p		输入的二进制数组
 * @return：		二进制数
 */
uint32 get_ulong( uint8 *p );

/**
 * 16位crc校验和
 *
 * @param p		输入的二进制数组
 * @param len		数据长度
 * @return：		校验和
 */
uint16 crc16(uint8 *ptr, int len );

/**
 * 16位crc校验和
 *
 * @param crc 		上一次crc校验值
 * @param data		待计算的输入数
 * @return：		校验和
 */
uint16 _crc_xmodem_update ( uint16 crc, uint8 data );

/**
 * 二进制数组转short型数据
 *
 * @param p		输入的二进制数组
 * @return：		short型数据
 */
uint16 get_ushort( uint8 *p );

/**
 * 字符串打印
 *
 * @param buf		字符串地址
 * @param len		字符串长度
 */
void print_str(const uint8 *buf,int len);

/**
 * 16进制打印
 *
 * @param buf		数据地址
 * @param len		数据长度
 */
void printx(const uint8 * buf,int len);

/**
 * 将long型数据拆成字节串
 *
 * @param data		输入数据
 * @param p		字节串地址
 */
void put_long( int32 data, uint8 *p );

/**
 * 将short型数据拆成字节串
 *
 * @param data		输入数据
 * @param p		字节串地址
 */
void put_short( int16 data, uint8 *p );

/**
 * 将u_long型数据拆成字节串
 *
 * @param data		输入数据
 * @param p		字节串地址
 */
void put_ulong( uint32 data, uint8 *p );

/**
 * 将u_short型数据拆成字节串
 *
 * @param data		输入数据
 * @param p		字节串地址
 */
void put_ushort( uint16 data, uint8 *p );

/**
 * 文件拷贝
 *
 * @param src		原文件名
 * @param dest		目标文件名
 * @return: 0:		成功
 *	   -1：		失败
 */
int copy_file(const char *src, const char *dest);

/**
 * 保存文件
 *
 * @param file_name	被保存的文件名
 * @param buff		被保存数据的首地址
 * @param length	被保存数据的字节数
 * @param attribute	追加写，从头写等
 * @param offset	文件内的字节偏移量
 * @return: >=0:	成功
 *	   -1：		失败
 */
int save_file(const char* file_name, void *buff,long length,const char* attribute,uint32 offset);

/**
 * 读取文件
 *
 * @param file_name	被保存的文件名
 * @param buff		被保存数据的首地址
 * @param length	被保存数据的字节数
 * @param attribute	追加写，从头写等
 * @param offset	文件内的字节偏移量
 * @return: >=0:	成功
 *	   -1：		失败
 */
int read_file(const char* file_name, void *rcv_buff,long length,const char* attribute,uint32 offset);

/**
 * 存储区比较，低字节在前
 * @param src_buf
 * @param dst_buf
 * @param cmp_len
 * @return: 1		src > dst
 *	    0		src = dst
 *	    -1		src < dst
 */
int compare_buf(uint8* src_buf,uint8* dst_buf,uint16 cmp_len) ;

int float_equal(float f1, float f2);

int reverse_memcpy(uint8 *p_dest, uint8 *p_src, int len);

typedef struct tag_para_array {
	/**
	 * 参数保存文件名称
	 */
	const char *name;
	/**
	 * 参数起始地址
	 */
	void *paddr;
	/**
	 * 实际长度
	 */
	int len;
	/**
	 * 在同一文件中保存的偏移
	 */
	int offset;
} para_array_t;

/**
 * 字符串替换
 * @param src_buf
 * @param dst_buf
 * @param key
 * @return: 1		替换成功
 *	    0		替换失败

 */
BOOL str_replace(char *source, char *key, char *dst_buf);
/**
 * 字符串比较
 * @param source
 * @param key
 * @return: 1		比较成功
 *	    0		比较失败

 */
uint8 str_compare ( char* source, uint8 num, char* key );
/**
 * 字符串查找
 * @param source
 * @param key
 * @return: 字符串指针 成功
 *	    0		失败

 */
char *str_find(char *source, char *key);
/**
 * 判断一个文件是否存在
 *
 * @param file		文件名
 * @return 0：		成功
 *	   -1：		失败
 */
int isFileExist(char* file);


#define LEAP_YEAR(TIME_YEAR) (((TIME_YEAR%4 == 0) && (TIME_YEAR % 100 != 0))|| (TIME_YEAR%400==0) ? 1:0)

#define GET_BITMASK(VAL,_BIT_MASK_) (VAL & _BIT_MASK_)
#define SET_BITMASK(VAL,_BIT_MASK_) (VAL = VAL | _BIT_MASK_)
#define CLR_BITMASK(VAL,_BIT_MASK_) (VAL = VAL & ~_BIT_MASK_)

#define sbi(val, bit) ((val) |= (1 << (bit)))
#define cbi(val, bit) ((val) &= ~(1 << (bit)))


#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

//用于创建一个不被子进程继承句柄的file
int myopen(const char *path, int oflag, ...  );
//用于创建一个不被子进程继承句柄的file
FILE *myfopen(const char *path, const char *mode);
//用于创建一个不被子进程继承句柄的socket
int mysocket(int socket_family, int socket_type, int protocol)	;
#ifndef htonl
uint32 htonl(uint32 hostlong);
uint32 ntohl(uint32 netlong);
#endif
#define TABLE_SIZE(table) (sizeof(table)/sizeof(table[0]))
//将可见字符转换为二进制数
uint8 str2hex(char *buf,char*src_data,int len);
//将二进制数转换为可见字符
void hex2str(char *buf,char*src_data,int len);

void beep_us(int32 time);
#endif

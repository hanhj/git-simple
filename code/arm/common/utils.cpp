#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include "utils.h"
#include "debug.h"
#include "gpio.h"


static uint16 crc_ta[16] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
};

uint16 crc16(uint8 *ptr, int len)
{
	uint16 crc;
	uint8 da;

	crc = 0;

	if (ptr == NULL)
		return 0;
	while (len-- != 0) {
		da = crc >> 12;
		crc <<= 4;
		crc ^= crc_ta[da^ ( *ptr/16 ) ];

		da = crc >> 12;
		crc <<= 4;
		crc ^= crc_ta[da^ ( *ptr&0x0f ) ];
		ptr++;
	}

	return( crc );
}

uint16 _crc_xmodem_update ( uint16 crc, uint8 data )
{
	short i;

	crc = crc ^ ( ( uint16 ) data << 8 );

	for (i = 0; i < 8; i++) {
		if (crc & 0x8000)
			crc = ( crc << 1 ) ^ 0x1021;
		else
			crc <<= 1;
	}

	return crc;
}

uint16 check_sum_16(const uint8 *buf, uint16 len )
{
	int i;
	int16 sum = 0;
	for ( i = 0; i < len; i++ ) {
		sum = sum + buf[i];
	}
	return sum;
}


uint8 check_sum(const uint8* buf, uint16 len)
{
	int i;
	uint8 cs=0;
	for (i=0;i<len;i++) {
		cs+=buf[i];
	}
	return cs;
}

uint8 hex2bcd(uint8 hex)
{
	uint8 bcd;
	bcd= ((hex/10)<<4)+(hex%10);
	return bcd;
}

uint8 bcd2hex(uint8 bcd)
{
	uint8 hex;
	hex = (bcd>>4)*10+(bcd&0x0f);
	return hex;
}

double Long2Float ( int32 t, int8 prec)
{
    int32 temp1, temp2, m;
    char tmp[10], i;
    char tmpstr[20];
    char *str;

	str = tmpstr;
    strcpy ( tmp, "%ld.%01ld" );
    if ( prec < 1 || prec > 6 ) {
        prec = 2;
    }
    m = 1;
    for ( i = 0;i < prec;i++ ) {
        m = m * 10;
    }
    if ( t < 0 ) {
        t = -t;
        *str = '-';
        str++;
    }
    temp1 = t / m;
    temp2 = t % m;
    if ( temp2 < 0L ) {
        temp2 = ( -1 ) * temp2;
    }
    tmp[6] = '0' + prec;
    sprintf ( str, tmp, temp1, temp2 );
    //MT_LOG(TEM_INFO,"...str=%s,%f\n",str,atof(str));
    return atof(str);
}

uint32 Float2Long ( double t, int8 prec)
{
	char buffer[40];
	uint32 ret = 0;
	int i,len;
	double base = 10.0;
	
	memset(buffer,0,40);
	sprintf(buffer, "%.3f", t*pow(base,prec));
	//MT_LOG(TEM_INFO,"...buffer=%s\n",buffer);
	len = strlen(buffer)-4;
	for (i = 0; i < len; i++) {
		//MT_LOG(TEM_INFO,"...buffer[%d]=%d\n",i,buffer[i]-'0');
		ret += (buffer[i] - '0') * pow(10, len - 1 - i);
	}
    return ret;
}

uint32 nbcd2hex(uint8* p_bcd,uint8 n)
{
	uint32 l_data_h=0,l_data_l=0;
	uint32 power = 10;
	uint8 i;
	for (i=0;i<n/2;i++) {
		l_data_h+= ((p_bcd[i]>>4)&0x0f)*power;
		power*=100;
	}
	power =1;
	for (i=0;i<(n+1)/2;i++) {
		l_data_l += (p_bcd[i]&0x0f)*power;
		power*=100;
	}
	return(l_data_h+l_data_l);
}

uint32 time2d ( uint8 sec, uint8 min, uint8 hour, uint8 day, uint8 mon, uint8 year )
{
	/*
	//0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15
	//sec						//min					        //hour
	//16	17	18	19	20	21	22	23	24	25	26	27	28	29	30	31
	//              day						//mon				//year
	*/
	int32 tmp, res;
	tmp = 0;
	res = 0;
	tmp = ( sec );
	res |= tmp;
	tmp = ( min );
	tmp = tmp << 6;
	res |= tmp;
	tmp = ( hour );
	tmp = tmp << 12;
	res |= tmp;
	tmp = ( day );
	tmp = tmp << 17;
	res |= tmp;
	tmp = ( mon );
	tmp = tmp << 22;
	res |= tmp;
	tmp = ( year );
	tmp = tmp << 26;
	res |= tmp;
	return res;

}

uint8 set_bit(uint8* data,uint8 n)
{
	if (n>7)
		return 0;
	*data|=(1<<n);
	return 1;
}

uint8 clr_bit(uint8* data,uint8 n)
{
	if (n>7)
		return 0;
	*data&=~(1<<n);
	return 1;
}

uint8 is_set(uint8 data,uint8 n)
{
	return data&(1<<n)?1:0;
}
uint8 hex2nbcd(uint8 *p_bcd,uint8 n,uint32 l_data)
{
	uint8 i;
	uint8 data;
	for (i=0;i<n/2;i++) {
		data=l_data%100;
		p_bcd[i]=hex2bcd(data);
		l_data/=100;
	}
	if (n%2 == 1) {	     //奇数位，半字节赋值
		p_bcd[i]&=0xf0;
		p_bcd[i] |= hex2bcd(l_data%100)&0x0f;
	}
	return 0;
}

void hex2bcd_n(uint8 *hex,uint8 *bcd,uint8 n)
{
	int i;
	for(i=0;i<n;i++)
		bcd[i] = hex2bcd(hex[i]);
}

void bcd2hex_n(uint8 *bcd,uint8 *hex,uint8 n)
{
	int i;
	for(i=0;i<n;i++)
		hex[i] = bcd2hex(bcd[i]);
}

int8 significant_digits(float data, uint8 n, int32 *ldata)
{
	int i;
	int8 n_d;	   //幂指数
	int32 l_data=*ldata;
	int32 tl_data;
	data = fabs(data); //取绝对值，不考虑符号
	if (data>=1.0) {
		for (i=1;i<40;i++) {  //float最大范围 3.4E-38~3.4E38，最大39次循环即可
			data/=10;
			if (data<1.0) {
				break;
			}
		}
		tl_data = (int32)(data*pow(10,n+1));
		l_data = tl_data/10;
		if (tl_data - l_data*10>=5)
			l_data+=1;
		n_d = i-n;
	} else {
		for (i=1;i<40;i++) {  //同上
			data*=10;
			if (data>=1.0) {
				break;
			}
		}
		l_data = (int32)(data*pow(10,n-1));
		n_d = -i-n+1;
	}
	*ldata = l_data;
	return n_d;
}
/*transfer data between int and char,these function is based on little-endin*/
uint16 get_ushort( uint8 *p )
{
	uint16 ret;
	ret = *(p+1);
	ret<<=8;
	ret&=0xff00;
	ret+=*p;

//	ret = *p & 0x00FF | (*(p+1)) << 8 & 0xFF00;
	return ret;
}

void put_ushort( uint16 data, uint8 *p )
{
	*p = data & 0x00FF;
	*(p+1) = (data>> 8)&0x00ff;
}

uint32 get_ulong( uint8 *p )
{
	uint32 ret=0;
	int i;
	for (i=3;i>=0;i--) {
		ret<<=8;
		ret+=*(p+i);
	}
	return ret;
}

void put_ulong( uint32 data, uint8 *p )
{
	*p = data & 0x00FF;
	*(p+1) = (data & 0xFF00) >> 8;
	*(p+2) = (data & 0xFF0000) >> 16;
	*(p+3) = (data & 0xFF000000) >> 24;
}

int16 get_short( uint8 *p )
{
	int16 ret;
	ret = (*p & 0x00FF) | (((*(p+1)) << 8 )& 0xFF00);
	return ret;
}

void put_short(int16 data, uint8 *p )
{
	*p = data & 0x00FF;
	*(p+1) = (data & 0xFF00) >> 8;
}

int32 get_long( uint8 *p )
{
	int32 ret;

	ret = (*p & 0x00FF) | ((*(p+1) << 8) & 0xFF00) | ((*(p+2) << 16) & 0x00FF0000) | ((*(p+3) << 24) & 0xFF000000) ;
	return ret;
}

void put_long( int32 data, uint8 *p )
{
	*p = data & 0x00FF;
	*(p+1) = (data & 0x0000FF00) >> 8;
	*(p+2) = (data & 0x00FF0000) >> 16;
	*(p+3) = (data & 0xFF000000) >> 24;
}

void printx(const uint8 * buf,int len)
{
	int i;

	for (i = 0;i < len;i++) {
		fprintf(stderr, "%02x ",buf[i]);
		if(i % 16 == 0 && i != 0)
			fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
	return ;
}

void print_str(const uint8 *buf,int len)
{
	int i;
	for (i=0;i<len;i++) {
		fprintf(stderr, "%c",buf[i]);
	}
	return;
}

int compare_buf(uint8* src_buf,uint8* dst_buf,uint16 cmp_len)  //存储区比较，低字节在前
{
	int i;
	for (i=cmp_len-1;i>=0;i--) {
		if (src_buf[i]>dst_buf[i]) {
			return 1;
		} else if (src_buf[i]<dst_buf[i]) {
			return -1;
		} else
			continue;
	}
	return 0;
}

int copy_file(const char *src, const char *dest)
{
	return copy(src, dest);
}

/*************************** remove files ************************************/
char * last_char_is(const char *s, int c)
{
	char *sret = (char *)s;
	if (sret) {
		sret = strrchr(sret, c);
		if (sret != NULL && *(sret+1) != 0)
			sret = NULL;
	}
	return sret;
}

char *concat_path_file(const char *path, const char *filename)
{
	char *lc;
	char *str_ptr = NULL;

	if (!path)
		path = "";
	lc = last_char_is(path, '/');
	while (*filename == '/')
		filename++;
	str_ptr = (char *)malloc(256);
	if (NULL != str_ptr)
		sprintf(str_ptr, "%s%s%s", path, (lc==NULL ? "/" : ""), filename);

	return str_ptr;
}

char *concat_subpath_file(const char *path, const char *f)
{
	if (f && *f == '.' && (!f[1] || (f[1] == '.' && !f[2])))
		return NULL;
	return concat_path_file(path, f);
}

int save_file(const char* file_name, void* buff,long length,const char* attribute,uint32 offset)
{
	FILE* pfile;
	int ret=-1;
	pfile = myfopen(file_name,attribute);
	if (pfile == NULL) {
		PFUNC(TEM_ERROR, DEBUG_NORMAL,"Save %s fail\n", file_name);
		return -1;
	}
	fseek(pfile,offset,SEEK_SET);
	ret = fwrite(buff,length,1,pfile);
	fclose(pfile);

	//sync_fs();
	return ret;
}

int read_file(const char* file_name, void *rcv_buff,long length,const char* attribute,uint32 offset)
{
	FILE* pfile;
	int ret = -1;
	pfile = myfopen(file_name,attribute);
	if (pfile == NULL) {
		PFUNC(TEM_WARNING,DEBUG_NORMAL, "Read %s fail\n", file_name);
		return -1;
	}
	fseek(pfile,offset,SEEK_SET);
	ret = fread(rcv_buff,1,length,pfile);
	fclose(pfile);

	return ret;
}

int float_equal(float f1, float f2)
{
	return (fabs(f1 - f2) < 0.001) ? 1 : 0;
}


int reverse_memcpy(uint8 *p_dest, uint8 *p_src, int len)
{
	int i;
	for (i = 0; i < len; i++)
		p_dest[i] = p_src[len - 1 - i];
	return 0;
}

void hex2ascii(uint8 *hex,uint8 *dst,uint8 size)
{
	int i;
	for(i=0;i<size;i++) {
		dst[2*i]   = ((hex[i] >> 4) & 0xf) + '0';
		dst[2*i+1] = (hex[i] & 0xf) + '0';
	}
}

void rev_hex2ascii(uint8 *hex,uint8 *dst,uint8 size)
{
	int i;
	for(i=0;i<size;i++) {
		dst[2*i]   = ((hex[size - i] >> 4) & 0xf) + '0';
		dst[2*i+1] = (hex[size - i] & 0xf) + '0';
	}
}

/**
*排序函数
**/
void sort(uint8 *p,uint8 p_size)
{
	uint8 i,j,temp;
	uint8 *p1=p;
	if (p_size<=1)
		return;
	for (i=1;i<=p_size;i++) {
		for (j=p_size-1;j>=i;j--) {
			if (p1[j]<p1[j-1]) {
				temp=p1[j-1];
				p1[j-1]=p1[j];
				p1[j]=temp;
			}
		}
	}
}

void sort_16(int16 *p,int16 p_size) //冒泡排序(小到大)
{
	int16 i,j,temp;
	int16 *p1=p;
	if (p_size<=1)
		return;
	for (i=0;i<p_size;i++) {
		for (j=0;j<p_size-i;j++) {
			if (p1[j]>p1[j+1]) {
				temp=p1[j];
				p1[j]=p1[j+1];
				p1[j+1]=temp;
			}
		}
	}
}

void sort_float_maxtomin(float *p,int16 p_size) //冒泡排序(大到小)
{
	int16 i,j;
	float temp;
	float *p1=p;
	if (p_size<=1)
		return;
	for (i=0;i<p_size;i++) {
		for (j=0;j<p_size-i;j++) {
			if (p1[j]<p1[j+1]) {
				temp=p1[j];
				p1[j]=p1[j+1];
				p1[j+1]=temp;
			}
		}
	}
}
/**
*字符串函数									
**/
char *str_find(char *source, char *key)
{
	char *p = key;
	while (*source != '\0')
	{
		if (*source++ == *p){
			++p;
			if (*p == '\0'){
				return source - (p-key);
			}
		}
		else{
			p = key;
		}
	}
	return NULL;
}


void str_copy(char *source, char *dest, int len)
{
	while (len-- > 0){
		*source++ = *dest++;
	}
}


void str_move(char *source, int offset)
{
	if (offset > 0){
		int len = 0;
		while (*source){len++; source++;}
		while (len-- >= 0) {
			*(source+offset) = *source;
			source--;
		}
	}
	else if (offset < 0){
		while (*source){
		*(source+offset) = *source;
		source++;
		}
		*(source+offset) = '\0';
	}
}


int str_len(char *source)
{
	int len = 0;
	while (*source) {len++; source++;}
	return len;
}




BOOL str_replace(char *source, char *key, char *dest)
{
	char *p = str_find(source, key);
	if (NULL == p){
		return FALSE;
	}

	int klen = str_len(key);
	int dlen = str_len(dest);
	
	str_move(p+klen, dlen-klen);
	str_copy(p, dest, dlen);
	
    return TRUE;
}

uint8 str_compare ( char* source, uint8 num, char* key )					//比较收到数据与预期数据
{
	uint8 i = 0;
	if(source==NULL||key==NULL)return 0;
	//PFUNC(TEM_INFO,DEBUG_NORMAL,"compare:%s and %s num:%d\n",source,key,num)	;
	while (* ( key + i ) != '\0') {
		if (source[num+i] != key[i]) {										//从接收数据的NUM字节开始比较
			return 0;													//不匹配则返回0
		}
		i++;
	}
	return 1;															//匹配则返回1；
}

//用于创建一个不被子进程继承句柄的file
int myopen(const char *path, int oflag, ...  )
{
	int fd=open(path,oflag);
	if(fd>0){	
		int val=fcntl(fd,F_GETFD);
		val|=FD_CLOEXEC;
		fcntl(fd,F_SETFD,val);
	}else{
		perror("open");
	}
	return fd;

}
//用于创建一个不被子进程继承句柄的socket
int mysocket(int socket_family, int socket_type, int protocol)	
{
	int fd=socket(socket_family,socket_type,protocol);	
	if(fd>0){
		int val=fcntl(fd,F_GETFD);
		val|=FD_CLOEXEC;
		fcntl(fd,F_SETFD,val);
	}else{
		perror("socket");
	}
	return fd;
}
//用于创建一个不被子进程继承句柄的file
FILE *myfopen(const char *path, const char *mode)
{
	FILE* fp=fopen(path,mode);
	if(fp!=NULL){	
		int fd;
		fd=fileno(fp);
		int val=fcntl(fd,F_GETFD);
		val|=FD_CLOEXEC;
		fcntl(fd,F_SETFD,val);
	}else{
	}
	return fp;
}

uint8 HexTab[]={"0123456789ABCDEF"};
//将可见字符转换为二进制数
uint8 str2hex(char *buf,char*src_data,int len)
{
	uint8 data;
	int i;
	for(i=0;i<len;i++){
	data=0x00;
	if ((src_data[2*i+1]>='0')&&(src_data[2*i+1]<='9'))
		data=src_data[2*i+1]-'0';
	else if	((src_data[2*i+1]>='A')&&(src_data[2*i+1]<='F'))
		data=src_data[2*i+1]-'A'+10;
	else if	((src_data[2*i+1]>='a')&&(src_data[2*i+1]<='f'))
		data=src_data[2*i+1]-'a'+10;
	if ((src_data[2*i]>='0')&&(src_data[2*i]<='9'))
		data=data|(src_data[2*i]-'0')<<4; 
	else if	((src_data[2*i]>='A')&&(src_data[2*i]<='F'))
		data=data|(src_data[2*i]-'A'+10)<<4; 
	else if	((src_data[2*i]>='a')&&(src_data[2*i]<='f'))
		data=data|(src_data[2*i]-'a'+10)<<4; 
	buf[i]=data;	
	}	
	return 0;
}
//将二进制数转换为可见字符
void hex2str(char *buf,char*src_data,int len)
{
	int i;
	for(i=0;i<len;i++)	
	{
	buf[2*i] = HexTab[src_data[i]>>4&0x0f]; 
	buf[2*i+1] = HexTab[src_data[i]&0x0f];
	}
}

/**
 * 判断一个文件是否存在
 *
 * @param file		文件名
 * @return 0：		成功
 *	   -1：		失败
 */
int isFileExist(char* file)
{
	return access(file,R_OK);
}

void beep_us(int32 time)
{
gpio.Init(BEEP,GPIO_DIR_OUT,1);
gpio.Write(BEEP,0);
udelay(time);
gpio.Write(BEEP,1);
}

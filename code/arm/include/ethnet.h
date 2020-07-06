#ifndef _ETHNET__H_

#define _ETHNET__H_
#include  <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include "datatype.h" ///<�õ��������Ͷ���
#include "baseapp.h" ///<�õ������ඨ��
#include "user.h"
#pragma pack(1)
typedef struct sock_ctrl {

    struct sockaddr_in addr;
    int sockfd;
    socklen_t socklen;
} SOCK_CTRL;

#define SERVER_NAME "server_socket"
#define MAX_CLIENT 2

typedef struct eth_para_ {
    uint8 LocalIp[4];/**<  本地ip  */
    uint16 LocalPort;/**<  本地端口  */
    uint8 Mask[4];/**<  子网掩码  */
    uint8 Gate[4];/**<  网关  */
    uint8 Mac[6];/**<  mac地址  */
    uint8 Dns[4];/**<  域名解析服务器地址 */
    uint8 HostIp[4];/**<  主机ip  */
    uint16 HostPort;/**<  主机端口  */
    uint8 BakHostIp[4];/**<  备份主机ip  */
    uint16 BakHostPort;/**<  备份主机端口  */
    int8 Mode;/**<  工作模式[服务器模式，客户端模式，混合模式  */
} ETH_PARA;

class Ethnet
{

public:
    SOCK_CTRL server;
    ETH_PARA eth_para;
	uint8 align1;
	uint8 align2;
	uint8 align3;
    SOCK_CTRL client[MAX_CLIENT];
    int GetSockAddr ( char *addr );
    int GetConnectNum ( void );
    int ShowClientList ( void );
    void CloseSock ( int *fd );
    void CloseAllSock ( void );
    int CreateServerSock (const char *hostip, const int hostport, const int backlog );
    int CheckNewConnect ( void );
    int CreateClientConnect ( char *hostip, int hostport, SOCK_CTRL *s );
    int InitSockList ( void );
    int CheckInFdSet ( int fd );
    int Read ( int *fd, char *buf, int *len );
    int Write ( int *fd, char *buf, int *len );
    int IfConfig ( char w_r, char *ip, char *mask, char *gateway );
    int ResetMac ( char *mac );
    int IfCtrl ( char *name, char ctrl );
    int GetEthnetStatus ( void );
    Ethnet ( void );

private:
    int maxfd;
    int connect_num;
    fd_set read_flag_set;
    fd_set write_flag_set;
    fd_set error_flag_set;

    struct timeval waitd;
    int SockWrite ( int *fd, char *buf, int *len );
    int SockRead ( int *fd, char *buf, int *len );
    int makeAddr ( const char* name, int hostport, struct sockaddr_in *pAddr, socklen_t* pSockLen );
    int SetSocketSytle ( int sock , int block );
    SOCK_CTRL * SearchClientSockList ( int fd );
};
#endif
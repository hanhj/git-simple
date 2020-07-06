#ifndef WIRELESS_SOCKET_H
#define WIRELESS_SOCKET_H

class WIRELESS_SOCKET:public SOCKET{
public:
	int 	init(int port,void *parameter);
	int		up(void*);//启动以太口
	int		down(void*);//关闭以太口	
private:
	int create_socket(int id);

};
#endif
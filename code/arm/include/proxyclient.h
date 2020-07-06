#ifndef PROXYCLIENT_H
#define PROXYCLIENT_H
extern int connect_ms_by_proxy(int sockfd, uint8 proxy_type, uint32 hostip, uint16 hostport,ProxyCfg*proxy);
#endif
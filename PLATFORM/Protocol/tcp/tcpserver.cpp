/*
 * tcpserver.cpp
 *
 *  Created on: Sep 21, 2022
 *      Author: tct
 */
#include "tcpserver.h"
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define CONNECTNUM 10
/**
 * @brief      构造函数
 * @details
 * @param		string ip, 对端IP
 * 				UINT16 port
 * @return     long long 获取到的系统时间
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
TcpServerType::TcpServerType(UINT16 localPortCfg, void (*callback)(TcpServerType* ptr, struct NetParaType* psrc, UINT8* pdata, UINT16 len)): localPort(localPortCfg), rsvcb(callback)
{
	listenfd = 0;
	cout << "Initial TCP server port : " << localPortCfg << endl;
    pthread_create(&listenPid, NULL, PthreadListen, (void*)this);
	cout << "TCP server lising to port : " << localPortCfg << endl;

}

/**
 * @brief      打印当前监测的端口
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
void TcpServerType::PrintLocolPort(void)
{
	cout << "base: TCP server port : " << localPort << endl;
}

/**
 * @brief      析构函数
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
TcpServerType::~TcpServerType()
{
	UINT16 i;

	for(i = 0; i < clientIPTab.size(); i++)
	{
	    close(clientIPTab[i].accfd);
	}
}

/**
 * @brief      多线程函数静态马甲
 * @details
 * @param
 * @return     STATUS_T
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
void* TcpServerType::PthreadCtrl(void *arg)
{
	TcpServerType* ptr = (TcpServerType*)arg;
	STATUS_T ret = RET_UNKNOWN_ERR;

	while(1)
	{
		ret = ptr->RecvData();
		if(RET_NO_ERR != ret)
		{

			break;
		}
	}

	return NULL;
}

/**
 * @brief      多线程函数静态马甲
 * @details
 * @param
 * @return     STATUS_T
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
void* TcpServerType::PthreadListen(void *arg)
{
	TcpServerType* ptr = (TcpServerType*)arg;

	while(1)
	{
		ptr->ListenClient();
	}

	return NULL;
}

/**
 * @brief      接收函数
 * @details
 * @param
 * @return     STATUS_T
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
STATUS_T TcpServerType::RecvData(void)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	int size;
	UINT8 data[1000] = {0};
	long int len = 1000;
	struct NetParaType* pconf;
	int index = this->clientIPTab.size();

	pconf = &this->clientIPTab.back();
//	if(rsvcb ！= NULL)
	while(1)
	{
		size = read(pconf->accfd, data, len);
		if(0 >= size)
		{
			/*close connection*/
			close(pconf->accfd);
			/*destory fd*/
			vector<struct NetParaType>::iterator it = this->clientIPTab.begin() + index - 1;
			OfflineNotify(&this->clientIPTab[index-1]);
			this->clientIPTab.erase(it);

			ret = RET_NOCONN_ERR;
			break;
		}

		if(NULL != rsvcb)
			rsvcb(this, pconf, data, size);
	}

	return ret;
}

/**
 * @brief      匹配IP
 * @details
 * @param
 * @return     STATUS_T
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
struct NetParaType* TcpServerType::ISItAClient(string checkip)
{
	struct NetParaType* ptr = NULL;
	UINT16 i, res;

	for(i = 0; i < clientIPTab.size(); i++)
	{
		res = clientIPTab[i].ip.compare(checkip);
		if(0 == res)
		{
			ptr = &clientIPTab[i];
			break;
		}
	}

	return ptr;
}

/**
 * @brief      发送函数
 * @details
 * @param
 * @return     STATUS_T
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
STATUS_T TcpServerType::SendData(string ip, UINT8* pdata, UINT16 len)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	struct NetParaType* ptr;
	/*检查发送目标合法性*/
	ptr = ISItAClient(ip);
	if(NULL != ptr)
	{
		write(ptr->accfd, pdata, len);
	}
	else
	{
		cout << "Send package to : " << ip << "is error, no that client" << endl;
	}

	return ret;
}

/**
 * @brief      清理离线的设备
 * @details
 * @param
 * @return     STATUS_T
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
void TcpServerType::CleanIPService(void)
{

}

/**
 * @brief      tcpserver初始化连接函数
 * @details
 * @param
 * @return     STATUS_T
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
STATUS_T TcpServerType::ListenClient(void)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	int result;
    socklen_t clilen = 16;
    struct sockaddr_in servaddr,cliaddr;
    struct NetParaType netpara;
    int on = 1;

    memset((UINT8*)&netpara, 0, sizeof(netpara));
    listenfd = socket(AF_INET,SOCK_STREAM,0);
	//避免上次结束程序时，端口未被及时释放的问题
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(localPort);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    result = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(-1 == result)
    {
        perror("bind");
        return RET_UNKNOWN_ERR;
    }

    result = listen(listenfd, CONNECTNUM);
    if(-1 == result)
    {
        perror("listen");
        return RET_UNKNOWN_ERR;
    }

    printf("Waiting for clients to connect...\n");

    while(1)
    {
    	netpara.accfd = accept(listenfd,(struct sockaddr *)&cliaddr, &clilen);
        if(-1 == netpara.accfd)
        {
            perror("listen");
            return RET_UNKNOWN_ERR;
        }

        char cip[16] = {0};
		inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cip, 16);
		netpara.ip = cip;
		netpara.port = ntohs(cliaddr.sin_port);
		cout << "client ip is " << netpara.port << " addr is " << netpara.ip << endl;
        clientIPTab.push_back(netpara);

        OnlineNotify(&netpara);

        pthread_create(&netpara.pid, NULL, PthreadCtrl, (void*)this);
        pthread_detach(netpara.pid);
    }

    close(listenfd);
    return ret;
}

/**
 * @brief      离线回调函数
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
void TcpServerType::OfflineNotify(struct NetParaType* ptr)
{

}

/**
 * @brief      上线回调函数
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
void TcpServerType::OnlineNotify(struct NetParaType* ptr)
{

}

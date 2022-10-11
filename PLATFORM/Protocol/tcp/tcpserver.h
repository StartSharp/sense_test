/*
 * tcpserver.h
 *
 *  Created on: Sep 21, 2022
 *      Author: tct
 */

#ifndef TCP_TCPSERVER_H_
#define TCP_TCPSERVER_H_

#include <string>
#include <vector>
#include "commontypes.h"
#include <pthread.h>

using namespace std;

struct NetParaType{
	string ip;
	UINT16 port;
	int accfd;
	pthread_t pid;
	UINT32 sendSerial;
	UINT32 recvSerial;
};

class TcpServerType{
private:
	static void* PthreadCtrl(void *arg);
	static void* PthreadListen(void *arg);

protected:
	vector<struct NetParaType> clientIPTab;
	UINT16 localPort;
	pthread_t listenPid;
	int listenfd;
	void (*rsvcb)(TcpServerType* ptr, struct NetParaType* psrc, UINT8* pdata, UINT16 len);
public:
	TcpServerType(UINT16 localPortCfg, void (*callback)(class TcpServerType* ptr, struct NetParaType* psrc, UINT8* pdata, UINT16 len));
	virtual ~TcpServerType();
	virtual void OnlineNotify(struct NetParaType* ptr);
	virtual void OfflineNotify(struct NetParaType* ptr);
	struct NetParaType* ISItAClient(string checkip);
	STATUS_T ListenClient();
	STATUS_T SendData(string ip, UINT8* pdata, UINT16 len);
	STATUS_T RecvData();

	void PrintLocolPort(void);
	void CleanIPService(void);
};

#endif /* TCP_TCPSERVER_H_ */

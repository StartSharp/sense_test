/*
 * comm.h
 *
 *  Created on: Sep 23, 2022
 *      Author: tct
 */

#ifndef SCCTRLER_COMM_H_
#define SCCTRLER_COMM_H_

#include <iostream>
#include "Protocol/tcp/tcpserver.h"
#include "commontypes.h"
#include "comm.h"

using namespace std;
#define ENDIAN_CHANGE

#ifdef ENDIAN_CHANGE
#define SC_AUTO_DATA_PROCESS(data, dest) 		AutoChangeEndian(&(data), sizeof((data)), &(dest));
#else
#define SC_AUTO_DATA_PROCESS(data, dest)		dest = data;
#endif

enum SCSendDatatype{
	scCmdType,
};

class SCCommType : public TcpServerType{
public:
	static void RecvDisposal(TcpServerType* ptr, struct NetParaType* psrc, UINT8* pdata, UINT16 len);
	STATUS_T SendPackage(UINT8* ip, UINT8* data, UINT16 size, enum SCSendDatatype type);
	void PrintLocolPort(void);
//	SCCommType(UINT16 localPortCfg, void (*callback)(TcpServerType* ptr, struct NetParaType* psrc, UINT8* pdata, UINT16 len));
	SCCommType(UINT16 localPortCfg, void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len), void(*OnlineFunc)(void* phandler, struct NetParaType* ptr), void(*OfflineFunc)(void* phandler, struct NetParaType* ptr));
	SCCommType(UINT16 localPortCfg, void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len));
	virtual ~SCCommType();
	void OfflineNotify(struct NetParaType* ptr);
	void OnlineNotify(struct NetParaType* ptr);

private:
	void (*OnlineCB)(void* phandler, struct NetParaType* ptr);
	void (*OfflineCB)(void* phandler, struct NetParaType* ptr);
	void (*RecvDispach)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);
};

#endif /* SCCTRLER_COMM_H_ */

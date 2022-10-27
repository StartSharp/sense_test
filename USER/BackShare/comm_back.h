/*
 * comm_back.h
 *
 *  Created on: 2022年10月13日
 *      Author: bg
 */

#ifndef BACKSHARE_COMM_BACK_H_
#define BACKSHARE_COMM_BACK_H_

#include <iostream>
#include "Protocol/tcp/tcpserver.h"
#include "commontypes.h"
#include "xpack/json.h"
//#include "USER.h"//就不能加这两句
//#include "BackShare/BackShare.h"


class BackShareType:public TcpServerType
{
public:
	static void RecvDisposal(class TcpServerType* ptr, struct NetParaType* psrc, UINT8* pdata, UINT16 len);
	void PrintLocolPort(void);
	STATUS_T SendPackage(string ip, UINT8* data, UINT16 size);//UINT8* ip
	BackShareType(UINT16 localPortCfg,void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len));
//	UINT32 case_act_count;
//	struct case_cmd_para{
//		string cmd_para;
//		XPACK(O(cmd_para));
//	};
	//SCCommType(UINT16 localPortCfg, void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len));
	//BackShareType(UINT16 localPortCfg, void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len));
private:
	//void (*RecvDispach)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);
	void (*RecvDispach)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);
};



#endif /* BACKSHARE_COMM_BACK_H_ */

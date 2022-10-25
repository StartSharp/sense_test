/**
@file  BackShare.h
@brief  
@author 交控研究院
@version 1.0.0.0
@date Sep 27, 2022
*/
/*
 * BackShare.h
 *
 *  Created on: Sep 27, 2022
 *      Author: tct
 */

#ifndef BACKSHARE_BACKSHARE_H_
#define BACKSHARE_BACKSHARE_H_

#include "commontypes.h"

#include "commontypes.h"
#include <iostream>
#include "Protocol/tcp/tcpserver.h"
#include "comm_back.h"
#include "USER.h"
#include "PlatformManage/PlatformManage.h"
/**
* @brief 
* @param 
* @param 
* @return
*/
struct BShare_all_info_type{
	int online_scctrler_cnt;
	char ipaddr[160];

	//UINT16 platform_version;
}ALIGN(1);


typedef void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);


class BackShareManager
{
public:
	BackShareManager(UINT16 localTCPPort,scctrler_manager *p_sc,usecase_dispsal *p_ucase,PlatformManage *p_plat);//,scctrler_manager *p_sc,usecase_dispsal *p_ucase
	STATUS_T CmdSend(UINT8* ip, UINT8* data, UINT16 size, UINT16 type);//打包
	STATUS_T BShare(UINT16 id, string action, int para);//发送

//	void getGlobalInfo(scctrler_manager *p_sc,usecase_dispsal *p_ucase,PlatformManage *p_plat);
//	void getActInfo(scctrler_manager *p_sc,usecase_dispsal *p_ucase);
//	void getFeedInfo(scctrler_manager *p_sc,usecase_dispsal *p_ucase);
	static void* getData(void *arg);//scctrler_manager *p_sc,usecase_dispsal *p_ucase,PlatformManage *p_plat
	//STATUS_T BSCtrl(UINT16 id, string action, int para);
	//STATUS_T CmdSend(UINT8* ip, UINT8* data, UINT16 size, UINT16 type);
	static void BSRecvCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);//处理数据函数
	~BackShareManager();


protected:
	BackShareType tcp_server;							/*TCP连接相关参数*/
	struct BShare_all_info_type overall_info; 			/*总体信息*/

	scctrler_manager *p_sc;
	usecase_dispsal *p_ucase;
	PlatformManage *p_plat;

	vector<pfunc> recv_dispach_func_tab;  					/*数据接收回调函数*/
	UINT32 case_act_count=0;
//	UINT8* p_plat_state;
//	UINT8* p_ver_case_count;
//	UINT8* p_sc_info;
//	UINT8* p_veh_info;
//	UINT8* p_case_state;//用例可用状态
//	UINT8* p_act;
//	UINT8* p_feed;

	UINT8* p_all;//整体数据的指针=new UINT8[BUFSIZ]
	UINT8* p_temp;//临时数据指针=new UINT8[BUFSIZ]
	UINT8* p_temp1;//临时数据指针=new UINT8[BUFSIZ]

	int size_all;//
	UINT8* case_act_state;//=new UINT8 [300]

	UINT32 global_len;
	UINT32 act_len;
	UINT16 act_count;
	UINT32 feed_len;
	UINT16 feed_count;


};

#endif /* BACKSHARE_BACKSHARE_H_ */

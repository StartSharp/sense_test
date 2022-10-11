/**
@file  SCCtrlerManage.h
@brief  
@author 交控研究院
@version 1.0.0.0
@date Sep 27, 2022
*/
/*
 * SCCtrlerManage.h
 *
 *  Created on: Sep 27, 2022
 *      Author: tct
 */

#ifndef SCCTRLERMANAGE_SCCTRLERMANAGE_H_
#define SCCTRLERMANAGE_SCCTRLERMANAGE_H_

#include <string>
#include "commontypes.h"
#include "PLATFORM.h"
#include "comm.h"

#include "PLATFORM.h"

using namespace std;

typedef void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);

struct actuator_information{
	UINT16 id;								/*执行机构ID*/
	UINT8 onlineState;							/*上线状态*/
	UINT8 workState;							/*工作状态*/
	UINT8 related_controller_addr[16];		/*关联的场景控制器*/
	UINT8 cur_state[10];					/*当前状态*/
	UINT8 cur_command[10];					/*当前命令*/
	UINT8 feedback_state[10];				/*反馈状态*/
}ALIGN(1);

struct feedback_information{
	UINT16 id;
	UINT8 onlineSta;
	UINT8 workSta;
	UINT8 related_controller_addr[16];
	char sampling_value[10];
}ALIGN(1);

struct para_ctrl_type{
	string actuator_parameter_input;
	UINT16 actuator_control_instruction_output;
	UINT16 actuator_feedback_id;
	string actuator_inspection_standard;
};


struct actuator_conf{
	UINT16 id;
	string actuator_name;
	string actuator_describe;
	string actuator_control_type_explain;			/*控制方法*/
	int actuator_install_location;					/*安装位置	数据库中需修改类型*/
	string actuatot_initial_state_control;			/*初始状态控制*/
	int actuator_initial_state_para;				/*初始参数	数据库中需修改类型*/
	vector<struct para_ctrl_type> para_ctrl_tab;	/*参数配置	数据库中需修改类型*/
	string actuator_control_type;
	string actuator_inspect_type;
	int actuator_timeout;
};

struct feedback_conf{
	UINT16 id;
	string target_name;
	string target_describe;
};

struct feedback_type{
	struct feedback_information fb_info;
	struct feedback_conf fb_conf;
};

struct actuator_type{
	struct actuator_information act_info;
	struct actuator_conf act_conf;
};

struct scctrler_all_info_type{
	int online_scctrler_cnt;				/*上线的场景控制器数量*/
	char ipaddr[160];						/*IP地址存储地址 用分号隔开*/
	UINT8 vehicle_scctrler_online_sta;		/*车载场景控制器是否上线*/
	UINT8 sensing_device_online_sta;		/*感知设备是否上线*/
	UINT8 sensing_device_work_sta;			/*感知设备是否工作正常*/
	UINT16 sensing_device_version;			/*感知设备版本号*/
}ALIGN(1);

class scctrler_manager{
public:
	scctrler_manager(UINT16 localTCPPort);
	~scctrler_manager();
	UINT16 FindFBID(UINT16 id);
	UINT16 FindActID(UINT16 id);
	UINT16 FindAcionIndex(struct actuator_conf* ptr, string action);
	STATUS_T AddFBData(void);
	STATUS_T AddActData(void);
	STATUS_T CmdSend(UINT8* ip, UINT8* data, UINT16 size, UINT16 type);
	STATUS_T SCCtrl(UINT16 id, string action, int para);
	STATUS_T GetFBSample(UINT16 id, char* pbuf, UINT16 bufsize);
	void UpdateSCOverallInfo(void);
	void AddIP(string ip);
	void DeleteIP(string ip);
	INT16 GetSCOverallInfo(UINT8* pdata, UINT16 datasize);
	INT16 GetActState(UINT8* pbuf, UINT16 datasize, UINT16* psize);
	INT16 GetFBState(UINT8* pbuf, UINT16 datasize, UINT16* psize);
	INT16 GetVehicleState(UINT8* pdata, UINT16 datasize);

	void ActorFBCheck(void);

	static void OfflineStaUpdate(void* phandler, struct NetParaType* ptr);
	static void OnlineStaUpdate(void* phandler, struct NetParaType* ptr);
	static void SCRecvCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);

	static void SCMonitorCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);
	static void SCPoweronCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);
	static void SCRunTestCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);
	static void SCCMDFeedbackCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len);
protected:
	vector<struct feedback_type> fb_data_tab;				/*反馈类场景元素表*/
	vector<struct actuator_type> act_data_tab;				/*控制类场景元素表*/
	vector<pfunc> recv_dispach_func_tab;  			/*数据接收回调函数*/
	SCCommType tcp_server;									/*TCP连接相关参数*/
	struct scctrler_all_info_type overall_info; 			/*总体信息*/
	vector<string> scipTab;									/*上线场景控制器IP 中间结果*/
	string vehicle_sc_ip;									/*车载场景控制器IP*/
	mysql_op scdb;

};

/**
* @brief 
* @param 
* @param 
* @return
*/


#endif /* SCCTRLERMANAGE_SCCTRLERMANAGE_H_ */

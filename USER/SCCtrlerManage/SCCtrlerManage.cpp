/**
* @file SCCtrlerManage.cpp
* @brief 
* @author 交控研究院
* @date Sep 27, 2022
* @version V1.1
* @copyright TCT
* @par 修改日志
* <table>
* <tr><th>Date        <th>Version  <th>Author     <th>Description
* <tr><th>Sep 27, 2022     <th>V1.0     <th>交控研究院   <th>创建文件
* </table>
*/
#include "SCCtrlerManage.h"
#include <iostream>
#include "EndianChange.h"

using namespace std;

#define VEHICLE_SCENE_CTRLER_NUM    0x1234

#define VECHILE_STATE_SIZE				10

#define AUTO	0
#define MAN		1

struct SCCtrlFrameType{
	UINT16 ctrlID;
	UINT16 action;
	UINT32 actionPara;
}ALIGN(1);

struct SCCmdFrameType{
	UINT16 serial;
	UINT16 cmdCode;
	UINT8 para[sizeof(struct SCCtrlFrameType)];
}ALIGN(1);

enum CmdType{
	actCtrl = 0,
};

struct SCManageType{
	UINT16 nodeNum;
	UINT16 datatype;
};

struct SCMonitorType{
	UINT16 id;									/*元素ID*/
	UINT8 onlineState;							/*上线状态*/
	UINT8 workState;							/*工作状态*/
	UINT16 cur_command;							/*当前命令*/
	UINT32 cur_para;							/*当前命令参数*/
	UINT16 cur_state;							/*当前状态*/
	UINT32 cur_state_para;						/*当前状态参数*/
	UINT64 time_stamp1;							/*被控时间戳1*/
	UINT64 time_stamp2;							/*被控时间戳2*/
}ALIGN(1);


struct SCSampleType{
	UINT16 id;									/*元素ID*/
	UINT8 onlineSta;							/*上线状态*/
	UINT8 workSta;								/*工作状态*/
	UINT32 value;								/*采样值*/
	UINT64 time_stamp1;							/*采样时间戳1*/
	UINT64 time_stamp2;							/*采样时间戳2*/
}ALIGN(1);

struct VehicleInfo{
	unsigned char cur_speed;
	unsigned int cur_locate;
	unsigned char obstacles_count;
	unsigned char nearest_obstacles_type;
	unsigned short nearest_obstacles_distance;
	unsigned char identified_count;
	unsigned short identified_distance;
	unsigned char signal_count;
	unsigned char signal_color;
	unsigned short nearest_signals_distance;
}ALIGN(1);

/**
* @brief    构造函数
* @param 	struct actuator_type* pFBData 场景元素数据指针
* @return 	STATUS_T 返回是否执行成功
*/
scctrler_manager::scctrler_manager(UINT16 localTCPPort):tcp_server(localTCPPort, SCRecvCB, OnlineStaUpdate, OfflineStaUpdate)
{
	scdb.mysql_conn();
	scdb.query_db(scdb.ConnectPointer, (char*)"actuator_information");
	scdb.query_db(scdb.ConnectPointer, (char*)"feedback_information");

	cout <<"数据库长度 " << scdb.v_act_info.size() << endl;
	cout <<"数据库长度 " << scdb.v_feed_info.size() << endl;

	AddFBData();
	AddActData();

	recv_dispach_func_tab.push_back(SCMonitorCB);			/*第1个回调*/
	recv_dispach_func_tab.push_back(SCPoweronCB);			/*第2个回调*/
	recv_dispach_func_tab.push_back(SCRunTestCB);			/*第3个回调*/
	recv_dispach_func_tab.push_back(SCCMDFeedbackCB);		/*第4个回调*/


}

/**
* @brief    析构函数
* @param
* @return 	STATUS_T 返回是否执行成功
*/
scctrler_manager:: ~scctrler_manager()
{
	delete(&tcp_server);
}

/**
* @brief	增加反馈类场景元素
* @param 	struct actuator_type* pFBData 场景元素数据指针
* @return 	STATUS_T 返回是否执行成功
*/
STATUS_T scctrler_manager::AddFBData(void)
{
	STATUS_T ret = RET_UNKNOWN_ERR;

//	/*产生一组模拟数据*/
//	fbdata.fb_conf.id = 134;
//	fbdata.fb_conf.target_name = "被测平台最近障碍物距离";
//	fbdata.fb_conf.target_describe = "被测平台最近障碍物距离";
//	fbdata.fb_info.id = 134;


	/*从数据库中读取*/
	for(vector<mysql_op::feedback_information>::iterator it = scdb.v_feed_info.begin(); it != scdb.v_feed_info.end(); it++)
	{
		struct feedback_type fbdata = {0};
		fbdata.fb_conf.id = (*it).id;
		fbdata.fb_info.id =  (*it).id;
		fbdata.fb_conf.target_name = (*it).targer_name;
		fbdata.fb_conf.target_describe = (*it).target_describe;

		/*添加到本地数组*/
		fb_data_tab.push_back(fbdata);
	}

	/**/

	ret = RET_NO_ERR;
	return ret;
}

/**
* @brief	增加控制类场景元素
* @param 	struct actuator_type* pActData 场景元素数据直指针
* @return 	STATUS_T 返回是否执行成功
*/
STATUS_T scctrler_manager::AddActData(void)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
//	struct actuator_type pActData;
//
//	/*产生一组模拟数据*/
//	pActData.act_conf.id = 0;
//	pActData.act_conf.actuator_name = "被测平台最近障碍物距离";
//	pActData.act_conf.actuator_describe = "被测平台最近障碍物距离";
//	pActData.act_conf.actuator_control_type_explain = "共两种状态，on构建小障碍物侵线场景";
//	pActData.act_conf.actuator_install_location = 15;
//	pActData.act_conf.actuatot_initial_state_control = "off";
//	pActData.act_conf.actuator_initial_state_para = 0;
//	struct para_ctrl_type para;
//	para.actuator_parameter_input = "on";
//	para.actuator_control_instruction_output = 1;
//	para.actuator_feedback_id = 0;
//	para.actuator_inspection_standard = "";
//	pActData.act_conf.para_ctrl_tab.push_back(para);
//
//	para.actuator_parameter_input = "off";
//	para.actuator_control_instruction_output = 2;
//	para.actuator_feedback_id = 0;
//	para.actuator_inspection_standard = "";
//	pActData.act_conf.para_ctrl_tab.push_back(para);
//	pActData.act_conf.actuator_control_type = 1;
//	pActData.act_conf.actuator_inspect_type = 1;
//	pActData.act_conf.actuator_timeout = 10;
//	pActData.act_info.id = 0;

	/*从数据库中读取*/
	for(vector<mysql_op::actuator_information>::iterator it = scdb.v_act_info.begin(); it != scdb.v_act_info.end(); it++)
	{
		if((it == scdb.v_act_info.begin()) || ((*it).actuator_id != (*(it -1)).actuator_id))
		{
			struct actuator_type act_data = {0};
			act_data.act_conf.id = (*it).actuator_id;
			act_data.act_conf.actuator_name = (*it).actuator_name;
			act_data.act_conf.actuator_describe = (*it).actuator_describe;
			act_data.act_conf.actuator_control_type_explain = (*it).actuator_control_type_explain;
			act_data.act_conf.actuator_install_location = (*it).actuator_install_location;
			act_data.act_conf.actuatot_initial_state_control = (*it).actuatot_initial_state_control;
			act_data.act_conf.actuator_initial_state_para = (*it).actuator_initial_state_para;

			act_data.act_conf.actuator_control_type = (*it).actuator_control_type;
			act_data.act_conf.actuator_inspect_type = (*it).actuator_inspect_type;
			act_data.act_conf.actuator_timeout = (*it).actuator_timeout;
			act_data.act_info.id = (*it).actuator_id;

			struct para_ctrl_type para;
			para.actuator_parameter_input = (*it).actuator_parameter_input;
			para.actuator_control_instruction_output = (*it).actuator_control_instruction_output;
			para.actuator_feedback_id = (*it).actuator_feedback_id;
			para.actuator_inspection_standard = (*it).actuator_inspection_standard;
			act_data.act_conf.para_ctrl_tab.push_back(para);

			/*添加到本地数组*/
			act_data_tab.push_back(act_data);
		}
		else
		{
			struct para_ctrl_type para;
			para.actuator_parameter_input = (*it).actuator_parameter_input;
			para.actuator_control_instruction_output = (*it).actuator_control_instruction_output;
			para.actuator_feedback_id = (*it).actuator_feedback_id;
			para.actuator_inspection_standard = (*it).actuator_inspection_standard;
			(*(act_data_tab.end() - 1)).act_conf.para_ctrl_tab.push_back(para);
		}
	}

	return ret;
}

/**
* @brief	查找ID对应的vector下标
* @param 	id
* @return 	返回找到的ID
*/
UINT16 scctrler_manager::FindFBID(UINT16 id)
{
	UINT16 i;
	UINT16 index = 0xFFFF;

	for(i = 0; i < fb_data_tab.size(); i++)
	{
		if(fb_data_tab[i].fb_info.id == id)
		{
			index = i;
			break;
		}
	}

	return index;
}

/**
* @brief	查找ID对应的vector下标
* @param 	id
* @return 	返回找到的ID
*/
UINT16 scctrler_manager::FindFBID(string name)
{
	UINT16 i;
	UINT16 index = 0xFFFF;

	for(i = 0; i < fb_data_tab.size(); i++)
	{
		if(fb_data_tab[i].fb_conf.target_name == name)
		{
			index = i;
			break;
		}
	}

	return index;
}

/**
* @brief	查找ID对应的vector下标
* @param 	id
* @return 	返回找到的ID对应的vector下标 为找到返回0xffff
*/
UINT16 scctrler_manager::FindActID(UINT16 id)
{
	UINT16 i;
	UINT16 index = 0xFFFF;

	for(i = 0; i < act_data_tab.size(); i++)
	{
		if(act_data_tab[i].act_info.id == id)
		{
			index = i;
			break;
		}
	}

	return index;
}

/**
* @brief	查找ID对应的vector下标
* @param 	id
* @return 	返回找到的ID对应的vector下标 为找到返回0xffff
*/
UINT16 scctrler_manager::FindActID(string name)
{
	UINT16 i;
	UINT16 index = 0xFFFF;

	for(i = 0; i < act_data_tab.size(); i++)
	{
		if(act_data_tab[i].act_conf.actuator_name == name)
		{
			index = i;
			break;
		}
	}

	return index;
}

/**
* @brief	查找ID对应的vector下标
* @param 	id
* @return 	返回找到的ID对应的vector下标 为找到返回0xffff
*/
UINT16 scctrler_manager::FindAcionIndex(struct actuator_conf* ptr, string action)
{
	UINT16 i;
	UINT16 index = 0xFFFF;

	for(i = 0; i < ptr->para_ctrl_tab.size(); i++)
	{
		if(0 == action.compare(ptr->para_ctrl_tab[i].actuator_parameter_input))
		{
			index = i;
			break;
		}
	}

	return index;
}

/**
* @brief	场景元素命令包发送
* @param 	UINT16 id, 			场景元素ID
* @param	string action, 		动作
* @param	string para			参数
* @return 	STATUS_T 返回是否执行成功
*/
STATUS_T scctrler_manager::CmdSend(UINT8* ip, UINT8* data, UINT16 size, UINT16 type)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	struct SCCmdFrameType cmd;;

	cmd.serial = 0;
	SC_AUTO_DATA_PROCESS(type, cmd.cmdCode);
	memcpy(cmd.para, data, size);

	tcp_server.SendPackage(ip, (UINT8*)&cmd, sizeof(struct SCCmdFrameType), scCmdType);

	return ret;
}

/**
* @brief	场景元素控制
* @param 	UINT16 id, 			场景元素ID
* @param	string action, 		动作
* @param	string para			参数
* @return 	STATUS_T 返回是否执行成功
*/
STATUS_T scctrler_manager::SCCtrl(UINT16 id, string action, int para)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	struct SCCtrlFrameType cmd = {0};

	/*查表找到ID 对应的动作下发的数据*/
	UINT16 index = FindActID(id);
	UINT16 cmdIndex = FindAcionIndex(&act_data_tab[index].act_conf, action);

	/*上线及工作状态验证*/
	if((normal == act_data_tab[index].act_info.onlineState) && (normal == act_data_tab[index].act_info.workState))
	{
		if(strcmp(action.c_str(), (char*)act_data_tab[index].act_info.cur_command) != 0)
		{
			if(act_data_tab[index].act_info.related_controller_addr)
			{
				/*拼装数据包*/
				SC_AUTO_DATA_PROCESS(id, cmd.ctrlID);
				SC_AUTO_DATA_PROCESS(act_data_tab[index].act_conf.para_ctrl_tab[cmdIndex].actuator_control_instruction_output, cmd.action);
				SC_AUTO_DATA_PROCESS(*(UINT32*)&para, cmd.actionPara);

				/*数据发送*/
				CmdSend(act_data_tab[index].act_info.related_controller_addr, (UINT8*)&cmd, sizeof(cmd), actCtrl);
			}
			else
			{

			}
		}
		else
		{
		}
	}
	else
	{

	}

	return ret;
}

/**
* @brief	控制类场景元素检查
* @param 	string name, 			控制类场景元素名称
* @return 	返回监测的结果 -1 表示未配置该场景元素 0表示场景元素正常 1表示未上线 2表示工作异常
*/
INT16 scctrler_manager::ActCheck(string name)
{
	UINT16 index = FindActID(name);
	INT16 res = -1;
	if(0xFFFF != index)
	{
		for(vector<struct actuator_type>::iterator it = act_data_tab.begin(); it != act_data_tab.end(); it++)
		{
			if(normal == act_data_tab[index].act_info.onlineState)
			{
				if(normal == act_data_tab[index].act_info.workState)
				{
					res = 0;
				}
				else
				{
					res = 2;
				}
			}
			else
			{
				res = 1;
			}
		}
	}
	else
	{
		res = -1;
	}

	return res;
}

/**
* @brief	场景元素控制
* @param 	UINT16 id, 			场景元素ID
* @param	string action, 		动作
* @param	string para			参数
* @return 	STATUS_T 返回是否执行成功
*/
STATUS_T scctrler_manager::SCCtrl(string name, string action, int para)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	struct SCCtrlFrameType cmd = {0};

	/*查表找到ID 对应的动作下发的数据*/
	UINT16 index = FindActID(name);
	UINT16 cmdIndex = FindAcionIndex(&act_data_tab[index].act_conf, action);

	/*上线及工作状态验证*/
	if(normal == act_data_tab[index].act_info.onlineState)
	{
		if(normal == act_data_tab[index].act_info.workState)
		{
			/*动作查表验证*/
			if(strcmp(action.c_str(), (char*)act_data_tab[index].act_info.cur_command) != 0)
			{
				/*IP地址验证*/
				if(act_data_tab[index].act_info.related_controller_addr)
				{
					/*拼装数据包*/
					SC_AUTO_DATA_PROCESS(act_data_tab[index].act_conf.id, cmd.ctrlID);
					SC_AUTO_DATA_PROCESS(act_data_tab[index].act_conf.para_ctrl_tab[cmdIndex].actuator_control_instruction_output, cmd.action);
					SC_AUTO_DATA_PROCESS(*(UINT32*)&para, cmd.actionPara);

					/*数据发送*/
					CmdSend(act_data_tab[index].act_info.related_controller_addr, (UINT8*)&cmd, sizeof(cmd), actCtrl);
					ret = RET_NO_ERR;
				}
				else
				{
					/*IP地址正常*/
				}
			}
			else
			{
				/*重复命令不下发*/
			}
		}
		else
		{
			ret = RET_WORK_ERROR;
		}
	}
	else
	{
		ret = RET_NOCONN_ERR;
	}

	return ret;
}

template<typename TT>
STATUS_T scctrler_manager::GetFBSample(TT id, char* pbuf, UINT16 bufsize)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	UINT16 index = 0xFFFF;

	if(10 <= bufsize)
	{
		index = FindFBID(id);
		if(0xFFFF != index)
		{
			ret = RET_NO_ERR;
			strcpy(pbuf, fb_data_tab[index].fb_info.sampling_value);
		}
		else
		{
			ret = RET_ID_ERR;
		}
	}
	else
	{
		ret = RET_PARAM_ERR;
	}

	return ret;
}
#if 0  /*使用模板类替换*/
/**
* @brief	获取反馈采样结果
* @param 	UINT16 id, 			场景元素ID
* @return 	STATUS_T 返回是否执行成功
*/
STATUS_T scctrler_manager::GetFBSample(UINT16 id, char* pbuf, UINT16 bufsize)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	UINT16 index = 0xFFFF;

	if(10 <= bufsize)
	{
		index = FindFBID(id);
		if(0xFFFF != index)
		{
			ret = RET_NO_ERR;
			strcpy(pbuf, fb_data_tab[index].fb_info.sampling_value);
		}
		else
		{
			ret = RET_ID_ERR;
		}
	}
	else
	{
		ret = RET_PARAM_ERR;
	}

	return ret;
}
#endif
/**
* @brief	删除ip信息
* @param
* @return
*/
void scctrler_manager::DeleteIP(string ip)
{
	for(vector<string>::iterator it = scipTab.begin(); it < scipTab.end(); it++)
	{
		if((*it) == ip)
		{
			overall_info.online_scctrler_cnt--;
			scipTab.erase(it);
			/*若该IP对应着一个车载场景控制器 则设置其为离线*/
			if(0 == ip.compare(vehicle_sc_ip))
			{
				overall_info.vehicle_scctrler_online_sta = abnormal;
				vehicle_sc_ip.clear();
				overall_info.sensing_device_online_sta = abnormal;
				cout << "vehicle sc offline" << endl;
			}
			cout << "Delete IP success" << endl;
		}
		else
		{
			cout << "Delete IP failed, no that IP" << endl;
		}
	}
}

/**
* @brief	更新ip信息
* @param
* @return
*/
void scctrler_manager::AddIP(string ip)
{
	vector<string>::iterator it;
	for(it = scipTab.begin(); it < scipTab.end(); it++)
	{
		string connected_ip = (*it);
		connected_ip.erase(connected_ip.end() - 1);
		if(connected_ip != ip)
		{
			continue;
		}
		else
		{
			break;
		}
	}

	if(it == scipTab.end())
	{
		ip += ';';
		/*加入到iptab中*/
		scipTab.push_back(ip);
		/*更新场景控制器数量*/
		overall_info.online_scctrler_cnt++;
		/*地址列表发送时更新*/
		cout << "ADD IP Success!" << endl;
	}
}

/**
* @brief	当场景元素下线后
* @param 	struct NetParaType* ptr 包含下线IP 端口号等信息
* @return 	STATUS_T 返回是否执行成功
*/
void scctrler_manager::OfflineStaUpdate(void* phandler, struct NetParaType* ptr)
{
	/*利用container_of方法查询*/
	scctrler_manager* pSCAddr = container_of((SCCommType*)phandler, scctrler_manager, tcp_server);

	/*寻找ip对应的设备*/
	UINT16 i;

	pSCAddr->DeleteIP(ptr->ip);

	for(i = 0; i < pSCAddr->act_data_tab.size(); i++)
	{
		if(strcmp((char*)pSCAddr->act_data_tab[i].act_info.related_controller_addr, ptr->ip.c_str()) == 0)
		{
			/*修改设备状态*/
			pSCAddr->act_data_tab[i].act_info.onlineState = abnormal;
			memset(pSCAddr->act_data_tab[i].act_info.related_controller_addr, 0, 16);
		}
		else
		{

		}
	}

	for(i = 0; i < pSCAddr->fb_data_tab.size(); i++)
	{
		if(strcmp((char*)pSCAddr->fb_data_tab[i].fb_info.related_controller_addr, ptr->ip.c_str()) == 0)
		{
			pSCAddr->fb_data_tab[i].fb_info.onlineSta = abnormal;
			memset(pSCAddr->fb_data_tab[i].fb_info.related_controller_addr, 0, 16);
		}
		else
		{

		}
	}

}

/**
* @brief	当场景元素上线后
* @param 	UINT16 id, 			场景元素ID
* @param	string action, 		动作
* @param	string para			参数
* @return 	STATUS_T 返回是否执行成功
*/
void scctrler_manager::OnlineStaUpdate(void* phandler, struct NetParaType* ptr)
{

}

/**
* @brief	场景元素控制
* @param 	UINT16 id, 			场景元素ID
* @param	string action, 		动作
* @param	string para			参数
* @return 	STATUS_T 返回是否执行成功
*/
void scctrler_manager::ActorFBCheck(void)
{

}

/**
* @brief	获取场景控制器总体信息
* @param 	UINT8* pdata		输出数据存放地址
* @param	UINT16 datasize		输入数据存放地址的数据大小
* @return 	STATUS_T 返回是否执行成功
*/
INT16 scctrler_manager::GetSCOverallInfo(UINT8* pdata, UINT16 datasize, UINT16* psize)
{
	INT16 len = sizeof(struct scctrler_all_info_type);
	struct scctrler_all_info_type* ptr = (struct scctrler_all_info_type*)pdata;
	UINT16 ip_cnt;

	*psize = len;
	if(datasize < len)
	{
		memcpy(pdata, &overall_info, len);
		SC_AUTO_DATA_PROCESS(ptr->online_scctrler_cnt, ptr->online_scctrler_cnt);
		SC_AUTO_DATA_PROCESS(ptr->sensing_device_version, ptr->sensing_device_version);
		/*需要更新IPTAB到ip数据表中 并按照分号分割*/
		memset(ptr->ipaddr, 0, sizeof(ptr->ipaddr));
		ip_cnt = (ptr->online_scctrler_cnt <= 10)? scipTab.size(): 10;

		for(int i; i < ip_cnt; i++)
		{
			string ip_str = scipTab[i] + ';';
			strcat(ptr->ipaddr, scipTab[i].c_str());
		}
	}
	else
	{
		len = -1;
	}

	return len;
}

/**
* @brief	获取执行机构信息表
* @param 	void* pbuf 			数据存放地址
* @param	UINT16 datasize, 	数据存放地址可用长度
* @param	UINT16* psize		输出数据长度
* @return 	INT16 返回场景元素个数 异常返回-1
*/
INT16 scctrler_manager::GetActState(UINT8* pbuf, UINT16 datasize, UINT16* psize)
{
	 INT16 cnt = act_data_tab.size();
	 *psize = cnt * sizeof(struct actuator_information);
	 struct actuator_information* ptr = (struct actuator_information*)pbuf;
	 if(*psize <= datasize)
	 {
		 for(vector<struct actuator_type>::iterator it = act_data_tab.begin(); it != act_data_tab.end(); it++)
		 {
			 memcpy(ptr, &(*it).act_info, sizeof(struct actuator_information));
			 SC_AUTO_DATA_PROCESS(ptr->id, ptr->id);
		 }
	 }
	 else
	 {
		 cnt = -1;
		 *psize = 0;
	 }

	return cnt;
}

/**
* @brief	获取反馈机构信息表
* @param 	void* pbuf 			数据存放地址
* @param	UINT16 datasize, 	数据存放地址可用长度
* @param	UINT16* psize		输出数据长度
* @return 	INT16 返回场景元素个数 异常返回-1
*/
INT16 scctrler_manager::GetFBState(UINT8* pbuf, UINT16 datasize, UINT16* psize)
{
	 INT16 cnt = fb_data_tab.size();
	 *psize = cnt * sizeof(struct feedback_information);
	 struct feedback_information* ptr = (struct feedback_information*)pbuf;
	 if(*psize <= datasize)
	 {
		 for(vector<struct feedback_type>::iterator it = fb_data_tab.begin(); it != fb_data_tab.end(); it++)
		 {
			 memcpy(ptr, &(*it).fb_info, sizeof(struct feedback_information));
			 SC_AUTO_DATA_PROCESS(ptr->id, ptr->id);
		 }
	 }
	 else
	 {
		 cnt = -1;
		 *psize = 0;
	 }

	return cnt;
}

/**
* @brief	获取搭载平台信息
* @param 	UINT8* pdata		输出数据存放地址
* @param	UINT16 datasize		输入数据存放地址的数据大小
* @return 	返回拷贝的数据长度
*/
INT16 scctrler_manager::GetVehicleState(UINT8* pdata, UINT16 datasize, UINT16* psize)
{
	struct VehicleInfo* ptr = (struct VehicleInfo*)pdata;
	INT16 len = sizeof(struct VehicleInfo);
	*psize = len;
	char temp[10];
	string tab[VECHILE_STATE_SIZE] = {
			"列车当前速度",
			"列车当前位置",
			"识别的障碍物数量",
			"最近的障碍物类型",
			"最近的障碍物距离",
			"识别的标识数量",
			"最近的表示距离",
			"信号机数量识别",
			"最近信号机颜色",
			"最近信号机距离",
	};
	int resTab[VECHILE_STATE_SIZE] = {0};

	if(len <= datasize)
	{
		for(UINT16 i; i < VECHILE_STATE_SIZE; i++)
		{
			GetFBSample(tab[i], temp, 10);
			resTab[i] = atoi(temp);
		}

		ptr->cur_speed = resTab[0];
		ptr->cur_locate = resTab[1];
		ptr->obstacles_count = resTab[2];
		ptr->nearest_obstacles_type = resTab[3];
		ptr->nearest_obstacles_distance = resTab[4];
		ptr->identified_count = resTab[5];
		ptr->identified_distance = resTab[6];
		ptr->signal_count = resTab[7];
		ptr->signal_color = resTab[8];
		ptr->nearest_signals_distance = resTab[9];

		SC_AUTO_DATA_PROCESS(ptr->cur_speed, ptr->cur_speed);
		SC_AUTO_DATA_PROCESS(ptr->cur_locate, ptr->cur_locate);
		SC_AUTO_DATA_PROCESS(ptr->obstacles_count, ptr->obstacles_count);
		SC_AUTO_DATA_PROCESS(ptr->nearest_obstacles_type, ptr->nearest_obstacles_type);
		SC_AUTO_DATA_PROCESS(ptr->nearest_obstacles_distance, ptr->nearest_obstacles_distance);
		SC_AUTO_DATA_PROCESS(ptr->identified_count, ptr->identified_count);
		SC_AUTO_DATA_PROCESS(ptr->identified_distance, ptr->identified_distance);
		SC_AUTO_DATA_PROCESS(ptr->signal_count, ptr->signal_count);
		SC_AUTO_DATA_PROCESS(ptr->signal_color, ptr->signal_color);
		SC_AUTO_DATA_PROCESS(ptr->nearest_signals_distance, ptr->nearest_signals_distance);
	}
	else
	{
		len = -1;
	}

	return len;
}

/**
* @brief	信息处理的回调函数
* @param	void* phandler,				scctrler_manager对象地址
* @param	struct NetParaType* psrc, 	发送端信息
* @param	UINT8* pdata, 				数据本体
* @param 	UINT16 len					数据长度
* @return 	STATUS_T 返回是否执行成功
*/
void scctrler_manager::SCMonitorCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len)
{
	cout << "Received SC Monitor data" << endl;
	scctrler_manager* pSCAddr = (scctrler_manager*)phandler;
	UINT16 i, index;

#ifdef ENDIAN_CHANGE
	AUTO_CHANGE_ENDIAN(*(UINT16*)pdata);
	AUTO_CHANGE_ENDIAN(*((UINT16*)pdata + 1));
#endif
	UINT16 ctrlSize = *(UINT16*)pdata;
	UINT16 sampleSize = *((UINT16*)pdata + 1);
	struct SCMonitorType* pctrler = (struct SCMonitorType*)&pdata[4];
	struct SCSampleType* psampler = (struct SCSampleType*)&pdata[4 + ctrlSize * sizeof(struct SCMonitorType)];

	/*解析控制类场景元素数据*/
	for(i = 0; i < ctrlSize; i++)
	{
		SC_AUTO_DATA_PROCESS(pctrler->id, pctrler->id);
		index = pSCAddr->FindActID(pctrler->id);
		if(0xFFFF != index)
		{
			struct actuator_information* ptr_act_info = &pSCAddr->act_data_tab[index].act_info;
			/*设置设备状态为上线*/
			ptr_act_info->onlineState = normal;
			/*工作状态更新*/
			SC_AUTO_DATA_PROCESS(pctrler->workState, ptr_act_info->workState);
			/*IP更新*/
			strcpy((char*)ptr_act_info->related_controller_addr, psrc->ip.c_str());
			pSCAddr->AddIP(psrc->ip);
		}
		else
		{
			cout << "SC Message: not found that CTRLER id : " << pctrler->id << endl;
		}
	}

	/*解析反馈类场景元素数据*/
	for(i = 0; i < sampleSize; i++)
	{
		SC_AUTO_DATA_PROCESS(psampler->id, psampler->id);
		index = pSCAddr->FindFBID(psampler->id);
		if(0xFFFF != index)
		{
			struct feedback_information* ptr_fb_info = &pSCAddr->fb_data_tab[index].fb_info;
			/*设置设备状态为上线*/
			ptr_fb_info->onlineSta = normal;
			/*工作状态更新*/
			SC_AUTO_DATA_PROCESS(ptr_fb_info->workSta, ptr_fb_info->workSta);
			/*IP更新*/
			strcpy((char*)ptr_fb_info->related_controller_addr, psrc->ip.c_str());
			/*采样值更新*/
			SC_AUTO_DATA_PROCESS(psampler->value, psampler->value);
			strcpy(ptr_fb_info->sampling_value, to_string(psampler->value).c_str());
			pSCAddr->AddIP(psrc->ip);

			/*车载场景控制器上线 先简单处理*/
			if(140 == psampler->id)
			{
				pSCAddr->overall_info.sensing_device_online_sta = normal;
				pSCAddr->overall_info.sensing_device_work_sta = normal;
				pSCAddr->overall_info.sensing_device_version = (UINT16)psampler->value;
			}
		}
		else
		{
			cout << "SC Message: not found that FB id : " << psampler->id <<  endl;
		}
	}
}

/**
* @brief	信息处理的回调函数
* @param	void* phandler,				scctrler_manager对象地址
* @param	struct NetParaType* psrc, 	发送端信息
* @param	UINT8* pdata, 				数据本体
* @param 	UINT16 len					数据长度
* @return 	STATUS_T 返回是否执行成功
*/
void scctrler_manager::SCPoweronCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len)
{
	cout << "Received SC Power on data" << endl;
}

/**
* @brief	信息处理的回调函数
* @param	void* phandler,				scctrler_manager对象地址
* @param	struct NetParaType* psrc, 	发送端信息
* @param	UINT8* pdata, 				数据本体
* @param 	UINT16 len					数据长度
* @return 	STATUS_T 返回是否执行成功
*/
void scctrler_manager::SCRunTestCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len)
{
	cout << "Received SC Run test data" << endl;
}

/**
* @brief	信息处理的回调函数
* @param	void* phandler,				scctrler_manager对象地址
* @param	struct NetParaType* psrc, 	发送端信息
* @param	UINT8* pdata, 				数据本体
* @param 	UINT16 len					数据长度
* @return 	STATUS_T 返回是否执行成功
*/
void scctrler_manager::SCCMDFeedbackCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len)
{
	cout << "Received SC CMD Feedback data" << endl;
}

/**
* @brief	总体场景控制器管理层数据处理函数
* @param	void* phandler,				SCCommType对象地址
* @param	struct NetParaType* psrc, 	发送端信息
* @param	UINT8* pdata, 				数据本体
* @param 	UINT16 len					数据长度
* @return 	STATUS_T 返回是否执行成功
*/
void scctrler_manager::SCRecvCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len)
{
	cout << "Received SC manager data" << endl;

	/*利用container_of方法查询*/
	scctrler_manager* pSCAddr = container_of((SCCommType*)phandler, scctrler_manager, tcp_server);
	struct SCManageType* ptr = (struct SCManageType*)pdata;

#ifdef ENDIAN_CHANGE
	AUTO_CHANGE_ENDIAN(ptr->datatype);
	AUTO_CHANGE_ENDIAN(ptr->nodeNum);
#endif
	/*若场景控制器为车载场景控制器 则设置车载控制器上线 车载控制器状态与场景元素列表怎么关联*/
	if(VEHICLE_SCENE_CTRLER_NUM ==  ptr->nodeNum)
	{
		/*车载场景控制器上线*/
		pSCAddr->overall_info.vehicle_scctrler_online_sta = normal;
		pSCAddr->vehicle_sc_ip = psrc->ip;

	}

	/*根据数据类型决定数据处理函数*/
	if(ptr->datatype < pSCAddr->recv_dispach_func_tab.size())
	{
		if(pSCAddr->recv_dispach_func_tab[ptr->datatype])
		{
			pSCAddr->recv_dispach_func_tab[ptr->datatype](pSCAddr, psrc, (UINT8*)(ptr+1), len);
		}
		else
		{

		}
	}
	else
	{

	}

}


/**
* @brief	复位所有场景元素设备
* @return
*/
STATUS_T scctrler_manager::ResetAllDevices(void)
{
	struct SCCtrlFrameType cmd = {0};
	STATUS_T ret = RET_UNKNOWN_ERR;

	for(vector<struct actuator_type>::iterator it = act_data_tab.begin(); it != act_data_tab.end(); it++)
	{
		UINT16 cmdIndex = FindAcionIndex(&(*it).act_conf, (*it).act_conf.actuatot_initial_state_control);
		/*上线及工作状态验证*/
		if((normal == (*it).act_info.onlineState) && (normal == (*it).act_info.workState))
		{
			/*当前状态查表验证*/
			if(strcmp((*it).act_conf.actuatot_initial_state_control.c_str(), (char*)(*it).act_info.cur_command) != 0)
			{
				/*IP地址验证*/
				if((*it).act_info.related_controller_addr)
				{
					/*拼装数据包*/
					SC_AUTO_DATA_PROCESS((*it).act_conf.id, cmd.ctrlID);
					SC_AUTO_DATA_PROCESS((*it).act_conf.para_ctrl_tab[cmdIndex].actuator_control_instruction_output, cmd.action);
					SC_AUTO_DATA_PROCESS((*it).act_conf.actuator_initial_state_para, cmd.actionPara);

					/*数据发送*/
					CmdSend((*it).act_info.related_controller_addr, (UINT8*)&cmd, sizeof(cmd), actCtrl);
					ret = RET_NO_ERR;
				}
				else
				{

				}
			}
			else
			{
			}
		}
		else
		{

		}
	}

	return ret;
}


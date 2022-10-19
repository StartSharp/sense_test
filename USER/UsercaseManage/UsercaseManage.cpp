/**
* @file UsercaseManage.cpp
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
#include "UsercaseManage.h"
#include <iostream>
#include <pthread.h>
#include "EndianChange.h"

#include <sys/time.h>

using namespace std;

enum case_work_sta_type{
	work_well = 0,
	work_failed = 1,
	work_waiting = 2,
	work_running = 3,
};

enum case_work_info_type{
	no_error = 0,
	act_offline = 1,
	pretest_failed = 2,
	act_worksta_error = 3,
	usr_cmd = 4,
	unknowed_error = 5,
	plat_busy = 6,
	case_not_exist = 7,
};

struct feedback_info_json_type{
	UINT16 id;
	UINT16 sta;
	UINT16 info;
	XPACK(O(id, sta, info));
};

struct feedback_list_json_type{
	vector<struct feedback_info_json_type> list;
	XPACK(O(list));
};

struct usercase_step_json{
	vector<struct usercase_step> case_step;
	XPACK(O(case_step));
};

struct usercase_conf_json{
	vector<struct usecase_conf> case_conf;
	XPACK(A(case_conf, "test_vehicle"),O(case_conf));
};

struct usercase_precondition_json{
	vector<struct usecase_precondition> case_condition;
	XPACK(A(case_condition, "pre_inspection"),O(case_condition));
};

struct case_id_cmd_json{
	vector<struct usecase_tab_para> caseIDList;
	XPACK(O(caseIDList));
};

/**
 * @brief      获取系统时间ms
 * @details
 * @param
 * @return     long long 获取到的系统时间
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
long long GetSysTimeMS(void)
{
	long long timeMS = 0;
	struct timeval sysCurrentTime;

	gettimeofday(&sysCurrentTime, NULL);
	timeMS = ((long long)sysCurrentTime.tv_sec*1000000 + sysCurrentTime.tv_usec)/1000;

	return timeMS;
}

/**
 * @brief      获取系统时间s
 * @details
 * @param
 * @return     long long 获取到的系统时间
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
long long GetSysTimeS(void)
{
	struct timeval sysCurrentTime;

	gettimeofday(&sysCurrentTime, NULL);

	return sysCurrentTime.tv_sec;
}

/**
* @brief 初始化用例信息数据
* @return 
*/
usecase_dispsal::usecase_dispsal(scctrler_manager* p_sc, validation_type* p_val):p_sc_manager(p_sc), p_validation(p_val)
{
	pthread_t pid;
	/*查用例信息表*/
	usecase_update();
	stop_cmd = 0;

	plat_state.platSta = PLAT_IDLE;
	plat_state.curUseCaseID = 0;
	plat_state.startTime = 0;
	plat_state.startTimeMS = 0;
	plat_state.endTime = 0;
	plat_state.duration = 0;

	/*启动周期检查线程*/
	pthread_create(&pid, NULL, usecase_precheck, this);
	pthread_detach(pid);

	sleep(2);
}

/**
* @brief 检查用例ID是否存在
* @param UINT16 usecase_id 用例ID
* @return 返回是否存在该用例ID 存在返回下标 不存在返回-1
*/
INT16 usecase_dispsal::IsitACaseID(UINT16 usecase_id)
{
	UINT16 index;
	for(index = 0; index < case_info.size(); index++)
	{
		if(case_info[index].case_id == usecase_id)
		{
			return index;
		}
	}
	return -1;
}

/**
* @brief 用例状态预检 独立线程 时间触发 周期性检查用例可用性
* @param void* argv 用例操作句柄
* @return 返回是否执行成功
*/
void* usecase_dispsal::usecase_precheck(void* argv)
{
	usecase_dispsal* phandler = (usecase_dispsal*)argv;
	UINT16 index = 0, line = index/8, row = index%8;
	memset(phandler->pretest_state_tab, 0, 10);

	while(1)
	{
		index = 0;
		/*循环所有用例*/
		for(vector<struct usecase_info>::iterator it = phandler->case_info.begin(); it != phandler->case_info.end(); it++)
		{
			line = index / 8;
			row = index % 8;
			/*检查用例定义的预检项目*/


			/*检查所有用到的场景元素是否工作正常*/
			for(vector<struct usercase_step>::iterator act_it = (*it).case_step.begin(); act_it != (*it).case_step.end(); act_it++)
			{
				INT16 ret = phandler->p_sc_manager->ActCheck((*act_it).actuator);
				if(0 != ret)
				{
					phandler->pretest_state_tab[line] &= ~((UINT8)1u << row);
					break;
				}
				else
				{
					phandler->pretest_state_tab[line] |= (UINT8)1u << row;
				}
			}

			index++;
		}

		sleep(1);
	}

	return NULL;
}

/**
* @brief 用例执行函数 阻塞式运行 执行完成后退出
* @param 	void* argv
* @return
*/
void* usecase_dispsal::usecase_run(void* argv)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	usecase_dispsal* para = (usecase_dispsal*)argv;
	vector<struct usercase_step>::iterator it;
	UINT16 index = para->IsitACaseID(para->plat_state.curUseCaseID);
	string cur_usecase_name = (char*)para->plat_state.curUseCaseName;
	cout << "start usecase: " << para->plat_state.curUseCaseID << "case name: " << cur_usecase_name << endl;

	for(it = para->case_info[index].case_step.begin(); it != para->case_info[index].case_step.end(); )
	{
		do
		{
			if(para->plat_state.duration == (unsigned long long)(*it).case_time)
			{
				cout << "time "<< para->plat_state.duration <<" cmd : " <<"+" << (*it).actuator << "+" << "+" <<(*it).act << "+" <<"+"<<(*it).act_para<<"+" << endl;
				/*控制场景元素完成指定操作*/
				ret = para->p_sc_manager->SCCtrl((*it).actuator, (*it).act, (*it).act_para);
				ret = RET_NO_ERR;
				if(RET_NO_ERR != ret)
				{
					para->plat_state.sta = work_failed;
					if(RET_WORK_ERROR == ret)
					{
						para->plat_state.info = act_worksta_error;
					}
					else if(RET_NOCONN_ERR == ret)
					{
						para->plat_state.info = act_offline;
					}
					else
					{
						para->plat_state.info = unknowed_error;
					}

					break;
				}

				it++;
				if(it == para->case_info[index].case_step.end())
					break;
			}
			else
			{
				break;
			}
		}
		while(1);

		/*执行结构命令执行失败*/
		if(work_failed == para->plat_state.sta)break;

		/*用户设置停止命令*/
		if(1 == para->stop_cmd)
		{
			para->plat_state.sta = work_failed;
			para->plat_state.info = usr_cmd;
			para->stop_cmd = 0;
			cout << "user stop" <<endl;
			break;
		}

		sleep(1);
		para->plat_state.duration++;
	}

	if(it == para->case_info[index].case_step.end())
	{
		/*工作正常*/
		para->plat_state.sta = work_well;
		para->plat_state.info = no_error;
	}

	cout << "useccase finished" << endl;
	para->plat_state.platSta = PLAT_IDLE;
	para->plat_state.curUseCaseID = 0;
	para->plat_state.startTime = 0;
	para->plat_state.startTimeMS = 0;
	para->plat_state.endTime = 0;
	para->plat_state.duration = 0;

	return NULL;
}

/**
* @brief 请求外部发送的命令执行请求
* @param UINT16 usecase_id 用例ID
* @return 返回是否执行成功
*/
STATUS_T usecase_dispsal::usecase_cmd_resolve(UINT16 case_id)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	pthread_t pid;
	UINT16 line, row;
//	UINT16 case_id = atoi(usecase_id.c_str());
	UINT16 index = IsitACaseID(case_id);
	if(PLAT_IDLE == plat_state.platSta)
	{
		if(0 <= index)
		{
			/*需验证用例当前可用*/
			line = index / 8;
			row = index % 8;
			if(((pretest_state_tab[line] >> row) & (1u)) == 1)
			{
				/*初始化状态*/
				plat_state.curUseCaseID = case_id;
				plat_state.sta = work_running;
				plat_state.info = no_error;
				plat_state.platSta = PLAT_BUSY;
				plat_state.startTime = GetSysTimeS();
				plat_state.startTimeMS = GetSysTimeMS();
				plat_state.duration = 0;
				plat_state.endTime = plat_state.startTime + atoi(case_info[index].case_time_total.c_str());
				strcpy(plat_state.curUseCaseName, case_info[index].case_name.c_str());
				pthread_create(&pid, NULL, usecase_run, this);
				pthread_detach(pid);

				string record = "(" + to_string(plat_state.startTimeMS) + "," + to_string(plat_state.curUseCaseID)  + "," +
						"'NULL'" + "," + "'NULL'" + "," + "'NULL'" + "," + "'NULL'" + ")";

				/*记录写入数据库*/
				usercase_db.insert_db(usercase_db.ConnectPointer, "test_history", "(test_time,case_id,case_information,test_result,test_process_data_file,log_file)", record.c_str());

				/*触发数据验证模块*/


				ret = RET_NO_ERR;
			}
			else
			{
				/*当前用例不可用*/
				plat_state.sta = work_failed;
				plat_state.info = pretest_failed;
				ret = RET_EQU_NOT_READY;
			}
		}
		else
		{
			/*当前用例不存在*/
			plat_state.sta = work_failed;
			plat_state.info = case_not_exist;
			ret = RET_ID_ERR;
		}
	}
	else
	{
		/*当前平台正忙*/
		plat_state.sta = work_failed;
		plat_state.info = plat_busy;
		ret = RET_BUSY;
	}

	return ret;
}

/**
* @brief 外部请求后装载case tab
* @param UINT16 usecase_id 用例ID
* @return 返回是否执行成功
*/
STATUS_T usecase_dispsal::usecase_case_tab_load(string case_cmd)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	pthread_t pid;

	/*检查平台是否在忙 且用例tab是否为空*/
	if((PLAT_IDLE == plat_state.platSta) && (0 == case_id_cmd_tab.size()))
	{
		/*用例ID解析*/
		struct case_id_cmd_json case_id_cmd;
		xpack::json::decode(case_cmd, case_id_cmd);
	    for(vector<struct usecase_tab_para>::iterator iter = case_id_cmd.caseIDList.begin(); iter != case_id_cmd.caseIDList.end(); iter++)
	    {
			/*用例ID load进入load table*/
	    	struct usecase_tab_para cmd;
	    	cmd.run_time = (*iter).run_time;
	    	cmd.usecase_id = (*iter).usecase_id;
	    	cmd.sta = work_waiting;
	    	cmd.info = no_error;
	    	case_id_cmd_tab.push_back(cmd);
	    }

		/*启动case_tab执行函数*/
	    pthread_create(&pid, NULL, usecase_case_tab_dispach, this);
	    pthread_detach(pid);

	    ret = RET_NO_ERR;
	}
	else
	{
		ret = RET_BUSY;
	}

	return ret;
}

/**
* @brief 获取用例运行信息
* @param char* pbuf_json, 数据
* 		 UINT16 pbuf_size 长度
* @return 返回是否执行成功
*/
INT16 usecase_dispsal::get_usecase_run_sta(UINT8* pbuf_json, UINT16 pbuf_size, UINT16* psize)
{
	struct feedback_list_json_type fb_data_list;
	INT16 size;
	vector<struct usecase_tab_para>::iterator it;
	for(it = case_id_cmd_tab.begin(); it != case_id_cmd_tab.end(); it++)
	{
		struct feedback_info_json_type fb_info;
		fb_info.id = (*it).usecase_id;
		fb_info.sta = (*it).sta;
		fb_info.info = (*it).info;
		fb_data_list.list.push_back(fb_info);
	}

	string data_str = xpack::json::encode(fb_data_list);
	size = data_str.size();
	*psize = size;
	if(pbuf_size >= data_str.size())
		strcpy((char*)pbuf_json, data_str.c_str());
	else
		size = -1;

	return size;
}

/**
* @brief 请求外部发送的命令执行请求
* @param UINT16 usecase_id 用例ID
* @return 返回是否执行成功
*/
void* usecase_dispsal::usecase_case_tab_dispach(void* argv)
{
	usecase_dispsal* phandler = (usecase_dispsal*)argv;
	UINT16 index, line, row;

	/*从用例TAB中取待执行的用例*/
	vector<struct usecase_tab_para>::iterator it;
	for(it = phandler->case_id_cmd_tab.begin(); it != phandler->case_id_cmd_tab.end(); it++)
	{
		for(int i = (*it).run_time; i > 0; i--)
		{
			/*用例可用状态检查*/
			index = phandler->IsitACaseID((*it).usecase_id);
			line = index / 8;
			row = index % 8;
			if(((phandler->pretest_state_tab[line] >> row) & (1u)) == 1)
			{
				/*执行用例*/
				phandler->usecase_cmd_resolve((*it).usecase_id);
			}
			else
			{
				phandler->plat_state.sta = work_failed;
				phandler->plat_state.info = pretest_failed;
			}

			/*检查是否执行完毕*/
			while(PLAT_BUSY == phandler->plat_state.platSta)
			{
				(*it).sta = phandler->plat_state.sta;
				(*it).info = phandler->plat_state.info;
				sleep(1);
			}

			(*it).sta = phandler->plat_state.sta;
			(*it).info = phandler->plat_state.info;
			sleep(10);/*每条用例执行完成后等待10s 再开始下一条用例*/
		}
	}

	/*清除所有用例*/
	phandler->case_id_cmd_tab.clear();

	return NULL;
}

/**
* @brief 更新用例数据
* @return
*/
void usecase_dispsal::usecase_update(void)
{
	/*连接数据库*/
	usercase_db.mysql_conn();
	usercase_db.query_db(usercase_db.ConnectPointer, (char*)"case_information");

	for(vector<mysql_op::case_information>::iterator it = usercase_db.v_case_info.begin(); it != usercase_db.v_case_info.end(); it++)
	{
		struct usecase_info info_temp;
		info_temp.case_id = (*it).case_id;
		info_temp.case_name = (*it).case_name;
		info_temp.case_describe = (*it).case_describe;
		info_temp.case_preconditions = (*it).case_preconditions	;
		info_temp.case_time_total = (*it).case_time_total;

		/*用例执行步骤json转对象*/
	    struct usercase_step_json setp_json;
	    xpack::json::decode((*it).case_step, setp_json);
	    for(vector<struct usercase_step>::iterator iter = setp_json.case_step.begin(); iter != setp_json.case_step.end(); iter++)
	    {
			struct usercase_step step_temp;
			step_temp.case_time = (*iter).case_time;
			step_temp.actuator = (*iter).actuator;
			step_temp.act = (*iter).act;
			step_temp.act_para = (*iter).act_para;
			info_temp.case_step.push_back(step_temp);
	    }
	    /*用例配置json转对象*/
	    struct usercase_conf_json conf_json;
	    xpack::json::decode((*it).test_vehicle, conf_json);
	    for(vector<struct usecase_conf>::iterator iter = conf_json.case_conf.begin(); iter != conf_json.case_conf.end(); iter++)
	    {
			struct usecase_conf conf_temp;
			conf_temp.case_type = (*iter).case_type;
			conf_temp.vehicle_location = (*iter).vehicle_location;
			info_temp.case_conf.push_back(conf_temp);
	    }
	    /*预检项目json转对象*/
	    struct usercase_precondition_json condition_json;
	    xpack::json::decode((*it).pre_inspection, condition_json);
	    for(vector<struct usecase_precondition>::iterator iter = condition_json.case_condition.begin(); iter != condition_json.case_condition.end(); iter++)
	    {
	    	struct usecase_precondition condition_temp;
	    	condition_temp.pretest_id = (*iter).pretest_id;
	    	condition_temp.pretest_name = (*iter).pretest_name;
	    	condition_temp.pretest_state = (*iter).pretest_state;
			info_temp.pre_inspection.push_back(condition_temp);
	    }

		case_info.push_back(info_temp);
	}
}

/**
* @brief 用例执行紧急停止命令
* @return
*/
void usecase_dispsal::usecase_emergency_stop(void)
{
	if(PLAT_BUSY == plat_state.platSta)
	{
		/*用例执行停止*/
		stop_cmd = 1;

		/*测试分析停止*/

		/*场景控制器执行复位操作*/
		p_sc_manager->ResetAllDevices();

		/*中间数据清理
		 * 1.数据库中的中间结果数据 包括测试记录 测试过程数据
		 * 2.用例执行过程中产生的中间数据*/

	}
	else
	{
		/*nothing to do*/
	}
}

/**
* @brief 获取用例可用状态
* @param UINT8* pbuf, 		数据存放地址
* 		 UINT16 buf_size	数据存放区域大小
* @return 返回拷贝的数据长度
*/
INT16 usecase_dispsal::get_pretest_tab(UINT8* pbuf, UINT16 buf_size, UINT16* psize)
{
	INT16 size = sizeof(pretest_state_tab);

	*psize = size;
	if(buf_size >= size)
	{
		memcpy(pbuf, pretest_state_tab, size);
	}
	else
	{
		size = -1;
	}

	return size;
}

/**
* @brief 获取用例执行状态
* @param UINT8* pbuf, 		数据存放地址
* 		 UINT16 buf_size	数据存放区域大小
* @return 返回拷贝的数据长度
*/
INT16 usecase_dispsal::get_plat_usercase_state(UINT8* pbuf, UINT16 buf_size, UINT16* psize)
{
	struct usecase_state_type* p_usecase_state = (struct usecase_state_type*)pbuf;
	INT16 size = sizeof(plat_state);
	*psize = size;
	if(buf_size >= size)
	{
		COMMON_AUTO_DATA_PROCESS(plat_state.platSta, p_usecase_state->platSta);
		COMMON_AUTO_DATA_PROCESS(plat_state.curUseCaseID, p_usecase_state->curUseCaseID);
		COMMON_AUTO_DATA_PROCESS(plat_state.curUseCaseType, p_usecase_state->curUseCaseType);
		COMMON_AUTO_DATA_PROCESS(plat_state.startTime, p_usecase_state->startTime);
		COMMON_AUTO_DATA_PROCESS(plat_state.endTime, p_usecase_state->endTime);
		COMMON_AUTO_DATA_PROCESS(plat_state.duration, p_usecase_state->duration);
		memcpy(p_usecase_state->curUseCaseName, plat_state.curUseCaseName, sizeof(plat_state.curUseCaseName));
	}
	else
	{
		size = -1;
	}

	return size;
}

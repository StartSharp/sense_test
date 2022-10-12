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
usecase_dispsal::usecase_dispsal(scctrler_manager* phandler):p_sc_manager(phandler)
{
	/*查用例信息表*/
	usecase_update();
	stop_cmd = 0;

	/*启动周期检查线程*/
	usecase_precheck();
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
* @return 返回是否执行成功
*/
void usecase_dispsal::usecase_precheck(void)
{

	/*检查用例定义的预检项目*/

	/*检查所有用到的场景元素是否正常*/

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
				if(ret != RET_NO_ERR)
				{
					para->stop_cmd = 1;
				}


				it++;
			}
			else
			{
				break;
			}
		}
		while(1);

		/*用户设置停止命令*/
		if(1 == para->stop_cmd)
		{
			para->stop_cmd = 0;
			cout << "user stop" <<endl;
			break;
		}

		sleep(1);
		para->plat_state.duration++;
	}

	cout << "useccase finished" << endl;
	para->plat_state.platSta = PLAT_FREE;
	para->plat_state.curUseCaseID = 0;
	para->plat_state.startTime = 0;
	para->plat_state.endTime = 0;
	para->plat_state.duration = 0;

//	pthread_detach(pthread_self());
//	while(1)
//	{}

	return NULL;
}

/**
* @brief 请求外部发送的命令执行请求
* @param UINT16 usecase_id 用例ID
* @return 返回是否执行成功
*/
STATUS_T usecase_dispsal::usecase_cmd_resolve(UINT16 usecase_id)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	pthread_t pid;
	UINT16 index = IsitACaseID(usecase_id);
	if(0 <= index)
	{
		/*需验证用例当前可用*/

		plat_state.curUseCaseID = usecase_id;
		plat_state.platSta = PLAT_BUSY;
		plat_state.startTime = GetSysTimeS();
		plat_state.duration = 0;
		plat_state.endTime = plat_state.startTime + atoi(case_info[index].case_time_total.c_str());
		strcpy(plat_state.curUseCaseName, case_info[index].case_name.c_str());
		pthread_create(&pid, NULL, usecase_run, this);
//		pthread_detach(pid);
		pthread_join(pid, NULL);

		ret = RET_NO_ERR;
	}
	else
	{
		ret = RET_ID_ERR;
	}

	return ret;
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
INT16 usecase_dispsal::get_pretest_tab(UINT8* pbuf, UINT16 buf_size)
{
	INT16 size = sizeof(pretest_state_tab);

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
INT16 usecase_dispsal::get_plat_usercase_state(UINT8* pbuf, UINT16 buf_size)
{
	struct usecase_state_type* p_usecase_state = (struct usecase_state_type*)pbuf;
	INT16 size = sizeof(plat_state);
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

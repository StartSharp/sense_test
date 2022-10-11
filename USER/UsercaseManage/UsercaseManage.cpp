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

using namespace std;

struct usercase_run_type{
	usecase_dispsal* phandler;
	UINT16 case_index;
};

/**
* @brief 初始化用例信息数据
* @return 
*/
usecase_dispsal::usecase_dispsal()
{
	/*查用例信息表*/
	usecase_update();

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

}

/**
* @brief 用例执行函数 阻塞式运行 执行完成后退出
* @param 	void* argv
* @return
*/
void* usecase_dispsal::usecase_run(void* argv)
{
	struct usercase_run_type* para = (struct usercase_run_type*)argv;
	vector<struct usercase_step>::iterator it;

	for(it = para->phandler->case_info[para->case_index].case_step.begin(); it != para->phandler->case_info[para->case_index].case_step.end(); it++)
	{
		sleep((*it).case_time);

		/*控制场景元素完成指定操作*/
	}

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
	if(0 == index)
	{
		struct usercase_run_type usercase_run_para;
		usercase_run_para.phandler = this;
		usercase_run_para.case_index = index;

		pthread_create(&pid, NULL, usecase_run, &usercase_run_para);
		pthread_detach(pid);

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

	    struct usercase_conf_json conf_json;
	    xpack::json::decode((*it).test_vehicle, conf_json);
	    for(vector<struct usecase_conf>::iterator iter = conf_json.case_conf.begin(); iter != conf_json.case_conf.end(); iter++)
	    {
			struct usecase_conf conf_temp;
			conf_temp.case_type = (*iter).case_type;
			conf_temp.vehicle_location = (*iter).vehicle_location;
			info_temp.case_conf.push_back(conf_temp);
	    }

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
* @brief 获取用例可用状态
* @param UINT8* pbuf, 		数据存放地址
* 		 UINT16 buf_size	数据存放区域大小
* @return 返回拷贝的数据长度
*/
UINT16 usecase_dispsal::get_pretest_tab(UINT8* pbuf, UINT16 buf_size)
{

}

/**
* @brief 获取用例执行状态
* @param UINT8* pbuf, 		数据存放地址
* 		 UINT16 buf_size	数据存放区域大小
* @return 返回拷贝的数据长度
*/
UINT16 usecase_dispsal::get_plat_usercase_state(UINT8* pbuf, UINT16 buf_size)
{

}

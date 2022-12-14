/**
@file  UsercaseManage.h
@brief  
@author 交控研究院
@version 1.0.0.0
@date Sep 27, 2022
*/
/*
 * UsercaseManage.h
 *
 *  Created on: Sep 27, 2022
 *      Author: tct
 */

#ifndef USERCASEMANAGE_USERCASEMANAGE_H_
#define USERCASEMANAGE_USERCASEMANAGE_H_

#include "commontypes.h"
#include "string"
#include "vector"
#include "SCCtrlerManage/SCCtrlerManage.h"
#include "Validation/Validation.h"

#include "PLATFORM.h"

using namespace std;

#define PLAT_BUSY	1
#define PLAT_IDLE	2

/*用例执行参数*/
struct usecase_tab_para{
	UINT16 usecase_id;			/*用例ID*/
	UINT16 run_time;			/*执行次数*/
	UINT8 sta;					/*当前状态*/
	UINT8 info;					/*错误枚举*/
	XPACK(A(usecase_id, "id"),O(usecase_id, run_time));
};

/*用例执行步骤表*/
struct usercase_step{
	int case_time;											/*执行时刻*/
	string actuator;										/*执行机构*/
	string act;												/*执行动作*/
	int act_para;											/*执行中下发的参数*/
	XPACK(O(case_time, actuator, act, act_para));
};

/*用例配置信息*/
struct usecase_conf{
	string case_type;										/*测试类型*/
	string vehicle_location;								/*车辆绝对位置*/
	XPACK(A(case_type, "test_type", vehicle_location, "vehicle_locate"),O(case_type, vehicle_location));
};

/*用例预检状态*/
struct usecase_precondition{
	int pretest_id;											/*预检用例ID*/
	string pretest_name;									/*预检项目*/
	string pretest_state;									/*预检状态*/
	XPACK(A(pretest_id, "pre_inspection_project_id", pretest_name, "pre_inspection_project", pretest_state, "pre_inspection_state"));
};

/*用例信息表*/
struct usecase_info{
	int case_id;											/*用例ID*/
	string case_name;										/*用例名称*/
	string case_describe;									/*用例描述*/
	string case_preconditions;								/*前置条件*/
	string case_time_total;									/*用例执行总时长*/
	vector<struct usercase_step> case_step;					/*用例执行步骤*/
	vector<struct usecase_conf> case_conf;					/*用例测试配置*/
	vector<struct usecase_precondition> pre_inspection;		/*预检信息表*/

};

/*当前用例执行信息*/
struct usecase_state_type{
	unsigned char platSta;
	unsigned short curUseCaseID;
	unsigned char curUseCaseType;
	char curUseCaseName[50];
	unsigned long long startTime;
	unsigned long long endTime;
	unsigned long long duration;//已进行时间
	struct other_info{
		unsigned long long startTimeMS;
		UINT16 sta;					/*用例执行结果*/
		UINT16 info;				/*运行结果信息*/
	}others;
}ALIGN(1);

class usecase_dispsal{
public:
	usecase_dispsal(scctrler_manager* p_sc, validation_type* p_val);
	~usecase_dispsal();

	INT16 IsitACaseID(UINT16 usecase_id);
	static void* usecase_precheck(void* argv);
	static void* usecase_run(void* argv);
	STATUS_T usecase_cmd_resolve(UINT16 usecase_id);
	void usecase_update(void);
	void usecase_emergency_stop(void);

	STATUS_T usecase_case_tab_load(string case_cmd);
	static void* usecase_case_tab_dispach(void* argv);

	INT16 get_pretest_tab(UINT8* pbuf, UINT16 buf_size, UINT16* psize);
	INT16 get_plat_usercase_state(UINT8* pbuf, UINT16 buf_size, UINT16* psize);
	INT16 get_usecase_run_sta(UINT8* pbuf_json, UINT16 pbuf_size, UINT16* psize);

protected:
	vector<struct usecase_info> case_info;						/*用例配置 从数据库中读取*/
	mysql_op usercase_db;										/*用例数据库对象*/
	struct usecase_state_type plat_state;						/*平台测试用例的状态*/
	UINT8 pretest_state_tab[10];								/*平台预检状态*/
	scctrler_manager* p_sc_manager;								/*绑定的场景控制管理器*/
	validation_type* p_validation;								/*绑定的验证平台管理器*/
	UINT8 stop_cmd;												/*停止命令*/
	vector<struct usecase_tab_para> case_id_cmd_tab;			/*用例执行请求池 不允许追加*/
};

/**
* @brief 
* @param 
* @param 
* @return
*/


#endif /* USERCASEMANAGE_USERCASEMANAGE_H_ */

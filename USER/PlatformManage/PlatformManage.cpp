/**
* @file PlatformManage.cpp
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
#include "PlatformManage.h"
#include <iostream>
//#include <pthread.h>
#include<unistd.h>
#include<sys/time.h>
#include "EndianChange.h"

using namespace std;

/**
* @brief 
* @param 
* @param 
* @param 
* @param 
* @param 
* @param 
* @return 
*/
PlatformManage::PlatformManage(){

	pthread_create(&t_upd_sys_run_time,NULL,update_sys_running_time,(void*)this);//NULL
	pthread_detach(t_upd_sys_run_time);
//	pthread_create(&t_upd_plat_info,NULL,update_plat_info,NULL);
//	pthread_detach(t_upd_plat_info);

}


void* PlatformManage::update_sys_running_time(void *arg)//当系统上电，即触发函数开始计算
{
	PlatformManage* ptr=(PlatformManage*)arg;
	while(1)
	{
		sleep(1);
		ptr->plat_m_info.system_running_time++;
	}

}

void PlatformManage::update_case_test_count()
{
	//1.判断测试用例ID的变化。后端事件触发后判断是否为用例执行的事件
	//plat_m_info.case_test_count=case_test_count;
	plat_m_info.case_test_count++;
}

//void* PlatformManage::update_plat_info(void *arg)
//{
//	PlatformManage* ptr=(PlatformManage*)arg;
//
//
//}
//
//UINT64 PlatformManage::get_sys_time()
//{
//	//return system_running_time;
//}
//
//UINT32 PlatformManage::get_case_test_count()
//{
//	//case_test_count++;
//	//return case_test_count;
//}
//
//void PlatformManage::get_plat_info()
//{
//
//}
//
//UINT32 PlatformManage::get_plat_ver()
//{
//	//return version;
//}

INT16 PlatformManage::get_plat_mama_info(UINT8* pbuf, UINT16 buf_size)
{
	struct plat_mana_info* p_plat_mana = (struct plat_mana_info*)pbuf;
	INT16 size = sizeof(plat_m_info);
	if(buf_size >= size)
	{
		COMMON_AUTO_DATA_PROCESS(plat_m_info.version, p_plat_mana->version);
		COMMON_AUTO_DATA_PROCESS(plat_m_info.system_running_time, p_plat_mana->system_running_time);
		COMMON_AUTO_DATA_PROCESS(plat_m_info.case_test_count, p_plat_mana->case_test_count);


	}
	else
	{
		size = -1;
	}

	return size;
}


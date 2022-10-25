/**
@file  PlatformManage.h
@brief  
@author 交控研究院
@version 1.0.0.0
@date Sep 27, 2022
*/
/*
 * PlatformManage.h
 *
 *  Created on: Sep 27, 2022
 *      Author: tct
 */

#ifndef PLATFORMMANAGE_PLATFORMMANAGE_H_
#define PLATFORMMANAGE_PLATFORMMANAGE_H_

#include "commontypes.h"

#include<pthread.h>
#include<time.h>

/**
* @brief 
* @param 
* @param 
* @return
*/

class PlatformManage{
public:
	struct plat_mana_info{
		UINT32 version=1;
		UINT64 system_running_time=0;//long long /time_t
		UINT32 case_test_count=0;
	}ALIGN(1);


	pthread_t t_upd_sys_run_time;
	//pthread_t t_upd_plat_info;
	PlatformManage();
//	struct plat_info{//搭载平台信息
//		UINT8 cur_speed;
//		UINT32 cur_locate;
//		UINT8 obstacle_count;
//		UINT8 nearest_obstacle_type;
//		UINT16 nearesr_obstacle_distance;
//		UINT8 signs_count;
//		UINT16 nearest_signs_distance;
//		UINT8 signals_count;
//		UINT8 nearest_signals_color;
//		UINT16 nearest_signals_distance;
//
//	};
	//plat_info p_info;//共同处理这一个搭载平台信息的对象

	plat_mana_info plat_m_info;
	static void* update_sys_running_time(void *arg);
	void update_case_test_count();
	//static void* update_plat_info(void *arg);
//	UINT64 get_sys_time();
//	UINT32 get_case_test_count();
//	void get_plat_info();
//	UINT32 get_plat_ver();



	INT16 get_plat_mama_info(UINT8* pbuf, UINT16 buf_size);

};



#endif /* PLATFORMMANAGE_PLATFORMMANAGE_H_ */

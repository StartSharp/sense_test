/*
 * mysql_op.h
 *
 *  Created on: 2022年10月8日
 *      Author: bg
 */

#ifndef MYSQL_OP_MYSQL_OP_H_
#define MYSQL_OP_MYSQL_OP_H_

#include<mysql/mysql.h>
#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include<stdio.h>
#include "commontypes.h"

using namespace std;

//MYSQL mysql;  
//MYSQL_ROW row; 
//MYSQL_FIELD* field = NULL;      
//MYSQL_RES* result;   

class mysql_op//mysql_op
{
	public:
		MYSQL *ConnectPointer=mysql_init(NULL);


		void mysql_conn();
		void query_db(MYSQL *Pointer,char str_que[]);
		void insert_db(MYSQL *Pointer,char str_ins_table[],char ins_var[],const char ins_data[]);
		void update_db(MYSQL *Pointer,char str_upd[],char upd_var[],char upd_condition[]);
		void delete_db(MYSQL *Pointer,char str_del[],char del_condition[]);

		struct parameter_configuration{
			int id;
			string detect_cycle;
		};


		struct plat_login{//用户名密码表

			int id;
			string username;
			string password;
			int privilege;
//			char username[];
//			//char password;
		};

		struct actuator_information{//场景元素配置表（执行机构信息表）
			UINT16 id;
			UINT16 actuator_id;
			string actuator_name;
			string actuator_describe;
			string actuator_control_type_explain;
			int actuator_install_location;
			string actuatot_initial_state_control;
			int actuator_initial_state_para;
			string actuator_parameter_input;
			UINT16 actuator_control_instruction_output;
			string actuator_control_type;
			string actuator_inspect_type;
			UINT16 actuator_feedback_id;
			string actuator_inspection_standard;
			int actuator_timeout;
		};

		struct feedback_information{//反馈机构表
			UINT16 id;
			string targer_name;
			string target_describe;
		};

		struct detection_target_configuration{//检测目标配置表
			int id;
			string target_name;
			string data_source;
			string error_torlerance_range;
		};

		struct test_history{//历史数据记录表
			int id;
			int test_time;
			int case_id;
			string case_information;
			string test_result;
			string test_process_data_file;
			string log_file;
		};

		struct case_information{//用例信息表
			int id;
			int case_id;
			string case_name;
			string case_describe;
			string case_preconditions;
			//string cur_state;
			string case_time_total;
			string case_step;//这仨要不要写成json
			string test_vehicle;
			string pre_inspection;
		};

		struct test_process_data{//测试过程数据表
			int id;
			long long begin_timestamp;
			long long current_timestamp;
			int obstacle_count_static;
			int obstacle_count_dynamic;
			int obstacle_count_result;
			int nearest_obstacle_type_static;
			int nearest_obstacle_type_dynamic;
			int nearest_obstacle_type_result;
			int nearest_obstacle_distance_static;
			int nearest_obstacle_distance_dynamic;
			int nearest_obstacle_distance_result;
			int signs_count_static;
			int signs_count_dynamic;
			int signs_count_result;
			int nearest_signs_distance_static;
			int nearest_signs_distance_dynamic;
			int nearest_signs_distance_result;
			int signals_count_static;
			int signals_count_dynamic;
			int signals_count_result;
			int nearest_signals_color_static;
			int nearest_signals_color_dynamic;
			int nearest_signals_color_result;
			int nearest_signals_distance_static;
			int nearest_signals_distance_dynamic;
			int nearest_signals_distance_result;
			double recognition_accuracy;
			double false_positive_rate;
			double underreporting_rate;
		};

		vector<parameter_configuration> v_para_config;
		vector<plat_login> v_login;
		vector<actuator_information> v_act_info;
		vector<feedback_information> v_feed_info;
		vector<detection_target_configuration> v_tar_config;
		vector<test_history> v_test_his;
		vector<case_information> v_case_info;
		vector<test_process_data> v_test_process;


};



#endif /* MYSQL_OP_MYSQL_OP_H_ */

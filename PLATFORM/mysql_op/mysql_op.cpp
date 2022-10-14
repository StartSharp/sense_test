/*
 * mysql_op.cpp
 *
 *  Created on: 2022年10月8日
 *      Author: bg
 */

using namespace std;

#include<mysql/mysql.h>
#include <iostream>
#include <string.h>
#include <stack>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include "mysql_op.h"
//#include<mysql.h>
#include <typeinfo>


void mysql_op::mysql_conn(){
	//mysql_options(&ConnectPointer,MYSQL_SET_CHARSET_NAME,"GBK");
	//ConnectPointer=mysql_real_connect(ConnectPointer,"127.0.0.1","root","TCT666666","test_platformdb",3306,NULL,0);//127.0.0.1,TCT666666,test_platformdb
	//ConnectPointer=mysql_real_connect(ConnectPointer,"172.16.217.24","root","666666","server_platformdb",3306,NULL,0);//127.0.0.1,TCT666666,test_platformdb
	ConnectPointer=mysql_real_connect(ConnectPointer,"172.16.224.169","root","1234qwer","server_platformdb",3306,NULL,0);//127.0.0.1,TCT666666,test_platformdb
	if (ConnectPointer){
		printf("connect database successfully\n");
	}else{
		printf("failed to connect database\n");
	}
}

void mysql_op::query_db(MYSQL *Pointer,char str_que[]){
	//ResultNum=mysql_query(&ConnectPointer,"SELECT * from platform_login");
		char Exec[100]={"SELECT * FROM "};

//		cout<<typeid(Exec).name()<<endl;

		char column[100][100];
//		char* zz;
//		string s1="aaa";
//		string* s2;
//		cout<<typeid(column).name()<<endl;
//		cout<<typeid(zz).name()<<endl;
//		cout<<typeid(s1).name()<<endl;
//		cout<<typeid(s2).name()<<endl;
		strcat(Exec,str_que);
		int ResultNum=0;
		MYSQL_RES *Result;
		MYSQL_ROW Row;
		int numFields;
		ResultNum=mysql_query(Pointer,Exec);
		if (ResultNum!=0){//成功返回0,失败返回1
			printf("search data failed\n");
			//return -1;
		}
		Result=mysql_store_result(Pointer);
		if (Result==NULL){
			printf("get search data failed\n");
			//return -1;
		}

		numFields=mysql_num_fields(Result);//统计table字段，获取结果集中的列数
		for(int i=0;i<numFields;i++){
			//strcpy(column[i],mysql_fetch_field(Result)->name);//获取所有列名]
			column[0][i]=*mysql_fetch_field(Result)->name;
		}

//		switch(str_que)
//		{
//			case "plat_login":
//
//				break;
//			case "actuator_information":
//
//				break;
//			case "feedback_information":
//
//				break;
//			case "target_information":
//
//				break;
//			case "test_history":
//
//				break;
//			case "test_process_data":
//
//				break;
//			default:
//				break;
//		}












		if(strcmp(str_que,"plat_login")==0)
		{
			v_login.clear();
			int count=0;
			while((Row = mysql_fetch_row(Result))!=NULL){//逐行返回结果集，最后一行后返回null
						//for(int j=0;j<numFields;j++){

			//			cout<<typeid(Row[0]).name()<<endl;
			//			cout<<typeid(Row[1]).name()<<endl;
			//			cout<<typeid(Row[2]).name()<<endl;

						plat_login login;
			//			login.id=atoi(Row[0]);
			//			login.username=Row[1];
			//			login.password=Row[2];
			//			printf("%d\t,%s\t,%s\t\n", login.id,login.username,login.password);

						//v_login.clear();

						v_login.push_back(login);
						v_login[count].id=atoi(Row[0]);
						v_login[count].username=Row[1];
						v_login[count].password=(Row[2]);
						v_login[count].privilege=atoi(Row[3]);

						cout<<v_login[count].id<<','<<v_login[count].username<<','<<v_login[count].password<<','<<v_login[count].privilege<<endl;
			//			printf("%d\n",v_login[count].id);
			//			printf("%s\n",v_login[count].username.c_str());
			//			printf("%s\n",v_login[count].password.c_str());
					    //printf("%d; %s; %s\n", v_login[count].id, v_login[count].username, v_login[count].password);//
			//				printf("%s = %s\t\n", column[j], Row[j]);
						count++;
						//}
					}


			//		while ((Row=mysql_fetch_row(Result))!=NULL){
			//
			//			printf("id:%s\tname:%s\n",Row[0],Row[1]);
			//		}
					//return 0;
					//mysql_free_result(Result);

			for(int i=0;i<numFields;i++)
			{
				//column
			}



		}

		else if(strcmp(str_que,"actuator_information")==0)
		{
			v_act_info.clear();
			int count=0;
			while((Row = mysql_fetch_row(Result))!=NULL){//逐行返回结果集，最后一行后返回null


				actuator_information a_info;



				v_act_info.push_back(a_info);
				v_act_info[count].id=(UINT16)atoi(Row[0]);
				v_act_info[count].actuator_id=(UINT16)atoi(Row[1]);
				v_act_info[count].actuator_name=(Row[2]);
				v_act_info[count].actuator_describe=Row[3];
				v_act_info[count].actuator_control_type_explain=Row[4];
				v_act_info[count].actuator_install_location=atoi(Row[5]);
				v_act_info[count].actuatot_initial_state_control=Row[6];
				v_act_info[count].actuator_initial_state_para=atoi(Row[7]);
				v_act_info[count].actuator_parameter_input=Row[8];
				v_act_info[count].actuator_control_instruction_output=(UINT16)atoi(Row[9]);
				v_act_info[count].actuator_control_type=Row[10];
				v_act_info[count].actuator_inspect_type=Row[11];
				v_act_info[count].actuator_feedback_id=(UINT16)atoi(Row[12]);
				v_act_info[count].actuator_inspection_standard=Row[13];
				v_act_info[count].actuator_timeout=atoi(Row[14]);


				cout<<v_act_info[count].id<<','<<v_act_info[count].actuator_id<<','<<v_act_info[count].actuator_name<<','<<v_act_info[count].actuator_describe
						<<','<<v_act_info[count].actuator_control_type_explain<<','<<v_act_info[count].actuator_install_location<<','<<v_act_info[count].actuatot_initial_state_control
						<<','<<v_act_info[count].actuator_initial_state_para<<','<<v_act_info[count].actuator_parameter_input<<','<<v_act_info[count].actuator_control_instruction_output<<','<<v_act_info[count].actuator_control_type
						<<','<<v_act_info[count].actuator_inspect_type<<','<<v_act_info[count].actuator_feedback_id<<','<<v_act_info[count].actuator_inspection_standard<<','
						<<v_act_info[count].actuator_timeout<<endl;
				count++;
			}
		}


		else if(strcmp(str_que,"parameter_configuration")==0)
		{
			v_para_config.clear();
			int count=0;
			while((Row = mysql_fetch_row(Result))!=NULL)//逐行返回结果集，最后一行后返回null
			{


				parameter_configuration para_config;

				v_para_config.push_back(para_config);
				v_para_config[count].id=atoi(Row[0]);
				v_para_config[count].detect_cycle=(Row[1]);

				cout<<v_para_config[count].id<<','<<v_para_config[count].detect_cycle<<endl;
				count++;
			}
		}


		else if(strcmp(str_que,"feedback_information")==0)
		{
			v_feed_info.clear();
			int count=0;
			while((Row = mysql_fetch_row(Result))!=NULL)//逐行返回结果集，最后一行后返回null
			{


				feedback_information f_info;

				v_feed_info.push_back(f_info);
				v_feed_info[count].id=(UINT16)atoi(Row[0]);
				v_feed_info[count].targer_name=(Row[1]);
				v_feed_info[count].target_describe=(Row[2]);

				cout<<v_feed_info[count].id<<','<<v_feed_info[count].targer_name<<','<<v_feed_info[count].target_describe<<endl;
				count++;
			}
		}

		else if(strcmp(str_que,"detection_target_configuration")==0)
		{
			v_tar_config.clear();
			int count=0;
			while((Row = mysql_fetch_row(Result))!=NULL)//逐行返回结果集，最后一行后返回null
			{


				detection_target_configuration tar_config;

				v_tar_config.push_back(tar_config);
				v_tar_config[count].id=atoi(Row[0]);
				v_tar_config[count].target_name=(Row[1]);
				v_tar_config[count].data_source=Row[2];
				v_tar_config[count].error_torlerance_range=Row[3];

				cout<<v_tar_config[count].id<<','<<v_tar_config[count].target_name<<','<<v_tar_config[count].data_source<<','<<v_tar_config[count].error_torlerance_range<<endl;
				count++;
			}
		}

		else if(strcmp(str_que,"test_history")==0)
		{
			v_test_his.clear();
			int count=0;
			while((Row = mysql_fetch_row(Result))!=NULL)//逐行返回结果集，最后一行后返回null
			{


				test_history t_his;

				v_test_his.push_back(t_his);
				v_test_his[count].id=atoi(Row[0]);
				v_test_his[count].test_time=atoi(Row[1]);
				v_test_his[count].case_id=atoi(Row[2]);
				v_test_his[count].case_information=Row[3];
				v_test_his[count].test_result=Row[4];
				v_test_his[count].test_process_data_file=Row[5];
				v_test_his[count].log_file=Row[6];

				cout<<v_test_his[count].id<<','<<v_test_his[count].test_time<<','<<v_test_his[count].case_id<<','<<v_test_his[count].case_information<<','<<v_test_his[count].test_result<<','<<v_test_his[count].test_process_data_file<<'.'<<v_test_his[count].log_file<<endl;
				count++;
			}
		}

		else if(strcmp(str_que,"case_information")==0)
		{
			v_case_info.clear();
			int count=0;
			while((Row = mysql_fetch_row(Result))!=NULL)//逐行返回结果集，最后一行后返回null
			{


				case_information case_info;
				//test_process_data t_pro_data;


				v_case_info.push_back(case_info);
				v_case_info[count].id=atoi(Row[0]);
				v_case_info[count].case_id=atoi(Row[1]);
				v_case_info[count].case_name=(Row[2]);
				v_case_info[count].case_describe=Row[3];
				v_case_info[count].case_preconditions=Row[4];
				//v_case_info[count].cur_state=Row[5];
				v_case_info[count].case_time_total=Row[5];
				v_case_info[count].case_step=Row[6];
				v_case_info[count].test_vehicle=Row[7];
				v_case_info[count].pre_inspection=Row[8];


				cout<<v_case_info[count].id<<','<<v_case_info[count].case_id<<','<<v_case_info[count].case_name<<','<<
						v_case_info[count].case_describe<<','<<v_case_info[count].case_preconditions<<','<<v_case_info[count].case_time_total<<','<<v_case_info[count].case_step<<','<<v_case_info[count].test_vehicle
						<<','<<v_case_info[count].pre_inspection<<endl;
				count++;
			}
		}

		else if(strcmp(str_que,"test_process_data")==0)
		{
			v_test_process.clear();
			int count=0;
			while((Row = mysql_fetch_row(Result))!=NULL)//逐行返回结果集，最后一行后返回null
			{


				test_process_data t_pro_data;
				//test_process_data t_pro_data;


				v_test_process.push_back(t_pro_data);
				v_test_process[count].id=atoi(Row[0]);
				v_test_process[count].begin_timestamp=atoll(Row[1]);
				v_test_process[count].current_timestamp=atoll(Row[2]);
				v_test_process[count].obstacle_count_static=atoi(Row[3]);
				v_test_process[count].obstacle_count_dynamic=atoi(Row[4]);
				//v_test_process[count].cur_state=Row[5];
				v_test_process[count].obstacle_count_result=atoi(Row[5]);
				v_test_process[count].nearest_obstacle_type_static=atoi(Row[6]);
				v_test_process[count].nearest_obstacle_type_dynamic=atoi(Row[7]);
				v_test_process[count].nearest_obstacle_type_result=atoi(Row[8]);
				v_test_process[count].nearest_obstacle_distance_static=atoi(Row[9]);
				v_test_process[count].nearest_obstacle_distance_dynamic=atoi(Row[10]);
				v_test_process[count].nearest_obstacle_distance_result=atoi(Row[11]);
				v_test_process[count].signs_count_static=atoi(Row[12]);
				v_test_process[count].signs_count_dynamic=atoi(Row[13]);
				v_test_process[count].signs_count_result=atoi(Row[14]);
				v_test_process[count].nearest_signs_distance_static=atoi(Row[15]);
				v_test_process[count].nearest_signs_distance_dynamic=atoi(Row[16]);
				v_test_process[count].nearest_signs_distance_result=atoi(Row[17]);
				v_test_process[count].signals_count_static=atoi(Row[18]);
				v_test_process[count].signals_count_dynamic=atoi(Row[19]);
				v_test_process[count].signals_count_result=atoi(Row[20]);
				v_test_process[count].nearest_signals_color_static=atoi(Row[21]);
				v_test_process[count].nearest_signals_color_dynamic=atoi(Row[22]);
				v_test_process[count].nearest_signals_color_result=atoi(Row[23]);
				v_test_process[count].nearest_signals_distance_static=atoi(Row[24]);
				v_test_process[count].nearest_signals_distance_dynamic=atoi(Row[25]);
				v_test_process[count].nearest_signals_distance_result=atoi(Row[26]);
				v_test_process[count].recognition_accuracy=atof(Row[27]);
				v_test_process[count].false_positive_rate=atof(Row[28]);
				v_test_process[count].underreporting_rate=atof(Row[29]);

				cout<<v_test_process[count].id<<','<<v_test_process[count].begin_timestamp<<','<<v_test_process[count].current_timestamp<<','<<
						v_test_process[count].obstacle_count_static<<','<<v_test_process[count].obstacle_count_dynamic<<','<<v_test_process[count].obstacle_count_result<<','<<v_test_process[count].nearest_obstacle_type_static<<','<<v_test_process[count].nearest_obstacle_type_dynamic
						<<','<<v_test_process[count].nearest_obstacle_type_result<<','<<v_test_process[count].nearest_obstacle_distance_static<<','<<v_test_process[count].nearest_obstacle_distance_dynamic<<','<<v_test_process[count].nearest_obstacle_distance_result<<','<<
						v_test_process[count].signs_count_static<<','<<v_test_process[count].signs_count_dynamic<<','<<v_test_process[count].signs_count_result<<','<<v_test_process[count].nearest_signs_distance_static<<','<<
						v_test_process[count].nearest_signs_distance_dynamic<<','<<v_test_process[count].nearest_signs_distance_result<<','<<v_test_process[count].signals_count_static<<','<<v_test_process[count].signals_count_dynamic<<','<<
						v_test_process[count].signals_count_result<<','<<v_test_process[count].nearest_signals_color_static<<','<<v_test_process[count].nearest_signals_color_dynamic<<','<<v_test_process[count].nearest_signals_color_result<<','<<
						v_test_process[count].nearest_signals_distance_static<<','<<v_test_process[count].nearest_signals_distance_dynamic<<','<<v_test_process[count].nearest_signals_distance_result<<','<<
						v_test_process[count].recognition_accuracy<<','<<v_test_process[count].false_positive_rate<<','<<v_test_process[count].underreporting_rate<<endl;
				count++;
			}
		}


}

void mysql_op::insert_db(MYSQL *Pointer,char str_ins[],char ins_var[],char ins_data[]){
	char Exec[100]={"INSERT INTO "};
	strcat(Exec,str_ins);
	strcat(Exec," ");
	strcat(Exec,ins_var);//要不要加空格？？
	strcat(Exec," ");
	strcat(Exec,"VALUES ");
	strcat(Exec,ins_data);

//	string str_sql="INSERT INTO plat_login (id,username,password) VALUES (5,'kakashi','555555')";
//	char sql[100]={"INSERT INTO plat_login (id,username,password) VALUES (6,'narutal','666666')"};

	int ResultNum=0;
//	MYSQL_RES *Result;
//	MYSQL_ROW Row;
//	int numFields;
	ResultNum =mysql_query(Pointer,Exec);

	if (ResultNum!=0){//成功返回0,失败返回1
		printf("insert data failed\n");
		//return -1;
	}
//	Result=mysql_store_result(Pointer);
//	if (Result==NULL){
//		printf("insert data failed\n");
//		//return -1;
//	}
//	numFields=mysql_num_fields(Result);//统计table字段，获取结果集中的列数

}

void mysql_op::update_db(MYSQL *Pointer,char str_upd[],char upd_var[],char upd_condition[]){
	char Exec[100]={"UPDATE "};
	strcat(Exec,str_upd);
	strcat(Exec," ");
	strcat(Exec,"SET ");
	strcat(Exec,upd_var);
	strcat(Exec," ");
	strcat(Exec,"WHERE ");
	strcat(Exec,upd_condition);
//	int numFields;
	int ResultNum=0;
//	MYSQL_RES *Result;
//	MYSQL_ROW Row;
	ResultNum =mysql_query(Pointer,Exec);

	if (ResultNum!=0){//成功返回0,失败返回1
		printf("update data failed\n");
		//return -1;
	}
//	Result=mysql_store_result(Pointer);
//	if (Result==NULL){
//		printf("update data failed\n");
//		//return -1;
//	}
//	numFields=mysql_num_fields(Result);//统计table字段，获取结果集中的列数
}

void mysql_op::delete_db(MYSQL *Pointer,char str_del[],char del_condition[]){
	char Exec[100]={"DELETE FROM "};
	strcat(Exec,str_del);
	strcat(Exec," ");
	strcat(Exec,"WHERE ");
	strcat(Exec,del_condition);
//	int numFields;
	int ResultNum=0;
//	MYSQL_RES *Result;
//	MYSQL_ROW Row;
	ResultNum =mysql_query(Pointer,Exec);

	if (ResultNum!=0){//成功返回0,失败返回1
		printf("delete data failed\n");
		//return -1;
	}
//	Result=mysql_store_result(Pointer);
//	if (Result==NULL){
//		printf("delete data failed\n");
//		//return -1;
//	}
//	numFields=mysql_num_fields(Result);//统计table字段，获取结果集中的列数
}



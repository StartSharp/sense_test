/**
* @file BackShare.cpp
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
#include "BackShare.h"
#include <iostream>
#include "EndianChange.h"
#include "crc32_back.h"

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

UINT8 back_ip[]={"127.0.0.1"};

struct BSRecvFrameHead{
	UINT16 head;
	UINT32 serial;
	UINT32 len;
//	UINT16 nodeNum;
//	UINT16 type;
}ALIGN(1);

BackShareManager::BackShareManager(UINT16 localTCPPort,scctrler_manager *p_sc,usecase_dispsal *p_ucase,PlatformManage *p_plat):tcp_server(localTCPPort, BSRecvCB)//,scctrler_manager *p_sc,usecase_dispsal *p_ucase
{
	this->p_sc=p_sc;
	this->p_ucase=p_ucase;
	this->p_plat=p_plat;

	p_all=new UINT8[BUFSIZ];//整体数据的指针
	p_temp=new UINT8[BUFSIZ];//临时数据指针
	p_temp1=new UINT8[BUFSIZ];
	case_act_state=new UINT8[300];



	//INT16 run_state_num_temp=ptr_bs->p_ucase->get_usecase_run_sta(ptr_bs->case_act_state, 300, run_sta_num_p);

	pthread_t p_getdata;
	pthread_create(&p_getdata, NULL, getData,(void*)this);//NULL   (void*)this
	pthread_detach(p_getdata);


	//getData(p_sc,p_ucase,p_plat);

	//tcp_server.SendPackage(ip, sendbuf, 8192);
}

BackShareManager::~BackShareManager()
{
	delete(p_all);
	delete(p_temp);
	delete(case_act_state);
}


void* BackShareManager::getData(void *arg)//向用例处理部分，平台运行管理部分，场景控制器管理部分获取//scctrler_manager *p_sc,usecase_dispsal *p_ucase,PlatformManage *p_plat
{
	BackShareManager* ptr_bs=(BackShareManager*)arg;
	//int count=0;
	/*while(1)
	{
//		UINT16 temp3=0;
//		UINT16*  ucase_len_p=&temp3;
//		INT16 ucase_len=ptr_bs->p_ucase->get_plat_usercase_state(ptr_bs->p_temp, BUFSIZ,ucase_len_p);//必须有非空地址初始化，累加,78    80   90//平台信息前半部分
//		if(ucase_len!=-1)
//		{
//			INT16 plat_mana_info_len=ptr_bs->p_plat->get_plat_mama_info(ptr_bs->p_temp+ucase_len, BUFSIZ);//16     16   //平台信息后半部分
		UINT8 test_char[1199]={0};

			UINT16 temp7=0;
			UINT16* run_sta_num_p =&temp7;
			INT16 run_state_num=300;//(*case_act_state)
			INT16 run_state_num_1=ptr_bs->p_ucase->get_usecase_run_sta(test_char, 300, run_sta_num_p);


			cout<<count<<endl;
			count++;
			ptr_bs->tcp_server.SendPackage("192.168.10.1", test_char, run_state_num_1);//1171，写一个public接口获取IP 1199
			usleep(200000);
//		}

	}*/
	while(1)
	{
		//cout<<"发送数据"<<endl;
//在这儿停顿
		UINT16 temp3=0;
		UINT16*  ucase_len_p=&temp3;
		INT16 ucase_len=ptr_bs->p_ucase->get_plat_usercase_state(ptr_bs->p_temp, BUFSIZ,ucase_len_p);//必须有非空地址初始化，累加,78    80   90//平台信息前半部分
		if(ucase_len!=-1)
		{
			//memcpy(p_all,p_plat_state,ucase_len);//这里用p_ucase还是p_plat_state好呢？
			//INT16 plat_mana_info_len=p_plat->get_plat_mama_info(p_ver_case_count, 16);

			INT16 plat_mana_info_len=ptr_bs->p_plat->get_plat_mama_info(ptr_bs->p_temp+ucase_len, BUFSIZ);//16     16   //平台信息后半部分
			if(plat_mana_info_len!=-1)
			{
				//memcpy(p_all,p_ver_case_count,plat_mana_info_len);
				UINT16 temp4=0;
				UINT16* sc_info_len_p=&temp4;
				INT16 sc_info_len=ptr_bs->p_sc->GetSCOverallInfo(ptr_bs->p_temp+ucase_len+plat_mana_info_len, BUFSIZ,sc_info_len_p);//167   169 //场景控制器信息
				if(sc_info_len!=-1)
				{
					UINT16 temp5=0;
					UINT16* veh_info_len_p=&temp5;
					//在这儿停顿
					INT16 veh_info_len=ptr_bs->p_sc->GetVehicleState(ptr_bs->p_temp+ucase_len+plat_mana_info_len+sc_info_len, BUFSIZ,veh_info_len_p);//16   16  //搭载平台信息
					if(veh_info_len!=-1)
					{
						UINT16 temp6=0;
						UINT16* case_state_len_p=&temp6;
						INT16 case_state_len=ptr_bs->p_ucase->get_pretest_tab(ptr_bs->p_temp+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len, BUFSIZ,case_state_len_p);//10    10 //用例可用信息
						if(case_state_len!=-1)
						{

							UINT16 temp7=0;
							UINT16* run_sta_num_p =&temp7;
//							UINT8 temp_arr[300];
////							try{
//								INT16 run_state_num_temp=ptr_bs->p_ucase->get_usecase_run_sta(temp_arr, 300, run_sta_num_p);
////								throw exception();
////							}
////							catch(exception e){
////								cout<<e.what()<<endl;
////							}

							//在这儿停顿
							//INT16 run_state_num_temp=ptr_bs->p_ucase->get_usecase_run_sta(ptr_bs->case_act_state, 300, run_sta_num_p);
							INT16 run_state_num=300;//(*case_act_state)

							//INT16 p_num_temp=ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len;
						//	INT16 run_state_num_1=0;
							INT16 run_state_num_1=ptr_bs->p_ucase->get_usecase_run_sta(ptr_bs->p_temp+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len, 300, run_sta_num_p);
							//INT16 run_state_num_1=ptr_bs->p_ucase->get_usecase_run_sta(ptr_bs->p_temp1, 300, run_sta_num_p);
//							int run_state_num=sizeof(case_act_state);
							//memcpy(ptr_bs->p_temp+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len,ptr_bs->case_act_state,300); //300  //用例执行状态  run_state_num
							//memcpy(ptr_bs->p_temp+p_num_temp,ptr_bs->p_temp1,300);

							ptr_bs->global_len=ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len+run_state_num;//+4
							UINT32 global_len_end=ptr_bs->global_len;
							AUTO_CHANGE_ENDIAN(global_len_end);

							//AUTO_CHANGE_ENDIAN(global_len);


							*(UINT32*)(ptr_bs->p_all)=global_len_end;//ptr_bs->global_len
							memcpy(ptr_bs->p_all+4,ptr_bs->p_temp,ptr_bs->global_len);


							if(run_state_num_1!=-1)
							{
								//执行机构信息
								UINT16 temp1=0;
								UINT16* psize_act=&temp1;//输出长度
								INT16 act_count=ptr_bs->p_sc->GetActState(ptr_bs->p_temp+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len+run_state_num, BUFSIZ, psize_act);//先定义一个1024。+run_state_num后加的   250
								if(*psize_act!=-1)
								{
									UINT32 psize_act_32=(UINT32)(*psize_act+2);
									UINT32 psize_act_32_end=psize_act_32;
									AUTO_CHANGE_ENDIAN(psize_act_32_end);

									UINT16 act_count_u=(UINT16)act_count;
									UINT16 act_count_u_end=act_count_u;
									AUTO_CHANGE_ENDIAN(act_count_u_end);


									memcpy(ptr_bs->p_all+ptr_bs->global_len+4,&psize_act_32_end,sizeof(psize_act_32_end));
									memcpy(ptr_bs->p_all+ptr_bs->global_len+8,&act_count_u_end,sizeof(act_count_u_end));
									memcpy(ptr_bs->p_all+ptr_bs->global_len+10,ptr_bs->p_temp+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len+run_state_num,psize_act_32-2);

									//反馈机构信息
									UINT16 temp2=0;
									UINT16* psize_feed=&temp2;
									INT16 feed_count=ptr_bs->p_sc->GetFBState(ptr_bs->p_temp+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len+(*psize_act)+run_state_num+act_count_u, BUFSIZ, psize_feed);//。+run_state_num后加的   330
									if(*psize_feed!=-1)
									{
										UINT32 psize_feed_32=(UINT32)(*psize_feed+2);
										UINT32 psize_feed_32_end=psize_feed_32;
										AUTO_CHANGE_ENDIAN(psize_feed_32_end);


										UINT16 feed_count_u=(UINT16)feed_count;
										UINT16 feed_count_u_end=feed_count_u;
										AUTO_CHANGE_ENDIAN(feed_count_u_end);

										memcpy(ptr_bs->p_all+ptr_bs->global_len+10+psize_act_32-2,&psize_feed_32_end,sizeof(psize_feed_32_end));
										memcpy(ptr_bs->p_all+ptr_bs->global_len+10+psize_act_32-2+4,&feed_count_u_end,sizeof(feed_count_u_end));
										memcpy(ptr_bs->p_all+ptr_bs->global_len+10+psize_act_32-2+6,ptr_bs->p_temp+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len+(*psize_act)+run_state_num+act_count_u,psize_feed_32-2);
										ptr_bs->size_all=ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len+run_state_num+(*psize_act)+(*psize_feed)+16;//。+run_state_num后加的
										ptr_bs->tcp_server.SendPackage("192.168.10.1", ptr_bs->p_all, ptr_bs->size_all);//1171，写一个public接口获取IP 1199
									}

								}
							}



						}
							//ptr_bs->act_count=ptr_bs->p_sc->act_data_tab.size();
							//ptr_bs->act_len=act_count*(ptr_bs->p_sc->)

					}
				}
			}
		}
		//UINT8 temp=1;


		//ptr_bs->tcp_server.SendPackage("192.168.10.1", &temp, sizeof(temp));
//		sleep(2);
		usleep(200000);
		//temp++;
	}







//		int ucase_len=ptr_bs->p_ucase->get_plat_usercase_state(ptr_bs->p_all, BUFSIZ);//必须有非空地址初始化，累加,78    80
//		if(ucase_len!=-1)
//		{
//			//memcpy(p_all,p_plat_state,ucase_len);//这里用p_ucase还是p_plat_state好呢？
//			//INT16 plat_mana_info_len=p_plat->get_plat_mama_info(p_ver_case_count, 16);
//			INT16 plat_mana_info_len=ptr_bs->p_plat->get_plat_mama_info(ptr_bs->p_all+ucase_len, BUFSIZ);//16     16
//			if(plat_mana_info_len!=-1)
//			{
//				//memcpy(p_all,p_ver_case_count,plat_mana_info_len);
//				INT16 sc_info_len=ptr_bs->p_sc->GetSCOverallInfo(ptr_bs->p_all+ucase_len+plat_mana_info_len, BUFSIZ);//167   169
//				if(sc_info_len!=-1)
//				{
//					INT16 veh_info_len=ptr_bs->p_sc->GetVehicleState(ptr_bs->p_all+ucase_len+plat_mana_info_len+sc_info_len, BUFSIZ);//16   16
//					if(veh_info_len!=-1)
//					{
//						INT16 case_state_len=ptr_bs->p_ucase->get_pretest_tab(ptr_bs->p_all+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len, BUFSIZ);//10    10
//						if(case_state_len!=-1)
//						{
//							int run_state_num=sizeof(case_act_state);
//							memcpy(ptr_bs->p_all+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len,ptr_bs->case_act_state,run_state_num); //300
//
//							UINT16 temp1=0;
//							UINT16* psize_act=&temp1;//输出长度
//							INT16 act_count=ptr_bs->p_sc->GetActState(ptr_bs->p_all+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len+run_state_num, BUFSIZ, psize_act);//先定义一个1024。+run_state_num后加的   250
//							if(*psize_act!=-1)
//							{
//								UINT16 temp2=0;
//								UINT16* psize_feed=&temp2;
//								INT16 feed_count=ptr_bs->p_sc->GetFBState(ptr_bs->p_all+ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len+(*psize_act)+run_state_num, BUFSIZ, psize_feed);//。+run_state_num后加的   330
//								ptr_bs->size_all=ucase_len+plat_mana_info_len+sc_info_len+veh_info_len+case_state_len+run_state_num+(*psize_act)+(*psize_feed);//。+run_state_num后加的
//							}
//						}
//					}
//				}
//			}
//		}
//

//	}

}

//void BackShareManager::getActInfo(scctrler_manager *p_sc,usecase_dispsal *p_ucase)//向场景控制器管理部分获取
//{
//
//}
//
//void BackShareManager::getFeedInfo(scctrler_manager *p_sc,usecase_dispsal *p_ucase)//向场景控制器管理部分获取
//{
//
//}
//
//void BackShareManager::packData(scctrler_manager *p_sc,usecase_dispsal *p_ucase)//装载各个区段数据
//{
//
//}


void BackShareManager::BSRecvCB(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len)//BackShareType*,
{
//	pthread_t p_getdata;
//	pthread_create(&p_getdata, NULL, getData,NULL);//NULL   (void*)this
//	pthread_detach(p_getdata);

	cout << "Received BS manager data" << endl;
		cout<<"执行到recvData"<<endl;
		/*利用container_of方法查询*/
		BackShareManager* pBSAddr = container_of((BackShareType*)phandler, BackShareManager, tcp_server);
		//struct SCManageType* ptr = (struct SCManageType*)pdata;
		UINT16 cmd_id =*(UINT16*)(pdata+10);//截取命令id
		AUTO_CHANGE_ENDIAN(cmd_id);
		cout<<"cmd_id="<<cmd_id<<endl;
		if(cmd_id==1)
		{
			//ptr_back_type->case_act_count++;//用例执行次数自增，传给平台管理单元
			pBSAddr->case_act_count++;
			//pBSAddr->p_plat->plat_m_info.case_test_count++;//暂且先这样干，后续改
			pBSAddr->p_plat->update_case_test_count();
			char cmd_para[500];
			//string cmd_para=
			//传给用例处理单元
	//		for(int i=0;i<20;i++)
	//		{
	//			cmd_para[i]=*(pdata+sizeof(struct BSRecvFrameHead)+2+i);
	//		}

			memcpy(cmd_para,pdata+sizeof(struct BSRecvFrameHead)+2,500);
			string cmd_para_str=cmd_para;
			cout<<"cmd_para_str="<<cmd_para_str<<endl;
			pBSAddr->p_ucase->usecase_cmd_resolve(cmd_id);
			pBSAddr->p_ucase->usecase_case_tab_load(cmd_para_str);
			//string cmd_para_json(&cmd_para[0],&cmd_para[strlen(cmd_para)]);
			//case_cmd_para c_c_para;
			//xpack::json::decode(cmd_para,c_c_para);
			//UINT16* cmd_para_int=(UINT16*)(char *)c_c_para.cmd_para.c_str();

			//等待更新string接口
			//pBSAddr->p_ucase

		}




}



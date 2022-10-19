/*
 * main.cpp
 *
 *  Created on: Sep 21, 2022
 *      Author: tct
 */


#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <iostream>

#include "USER.h"
#include "PLATFORM.h"

using namespace std;
pthread_t tcpserver;
pthread_t scctrler;
vector<TcpServerType*> TcpServerVec;

void callback(class TcpServerType* ptr, struct NetParaType* psrc, UINT8* pdata, UINT16 len)
{

}

void* Thread1(void* argv)
{

	TcpServerType* ptr = NULL;
	ptr = new TcpServerType(7000, callback);
	TcpServerVec.push_back(ptr);

	return NULL;
}
UINT8 g_button = 0;

int main(void)
{
//	pthread_create(&tcpserver, NULL, Thread1, NULL);

	scctrler_manager *p = new scctrler_manager(8550);
	validation_type *p2 = new validation_type();
	usecase_dispsal *p1 = new usecase_dispsal(p, p2);
//	sleep(10);
	string cmd = "{\"caseIDList\":[{\"id\":2,\"run_time\":1},{\"id\":2,\"run_time\":1}]}";
	sleep(2);
	p1->usecase_case_tab_load(cmd);
	char test[500];
	UINT16 len;

//	p1->usecase_cmd_resolve("2");
	while(1)
	{
//		if(g_button == 1)
//		{
//			g_button = 0;
//			sleep(10);
//			p->SCCtrl(0, "on", 0);
//			sleep(10);
//	//		cout << TcpServerVec[0]->ISItAClient("172.16.224.203") << endl;
//			p->SCCtrl(0, "off", 0);
//		}
		p1->get_usecase_run_sta((UINT8*)test, 500, &len);
		string str_test = test;
		cout << str_test << endl;
		sleep(1);
	}

	delete(p);
	return 0;
}


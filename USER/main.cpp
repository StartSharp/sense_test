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
	usecase_dispsal *p1 = new usecase_dispsal(p);
	sleep(10);
	p1->usecase_cmd_resolve(2);
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


		sleep(1);
	}

	delete(p);
	return 0;
}


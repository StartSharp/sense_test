/*
 * comm_back.cpp
 *
 *  Created on: 2022年10月13日
 *      Author: bg
 */

#include <iostream>
#include <string>
#include "comm_back.h"
#include"EndianChange.h"
#include "crc32_back.h"
#include "commontypes.h"
#include "xpack/json.h"
#include "crc32.h"
#include "unistd.h"

using namespace std;

//#define FRAME_HEAD		0x55AA//记得转换大小端
//#define FRAME_TAIL		0xAA55

UINT16 FRAME_HEAD=0x55AA;
UINT16 FRAME_TAIL=0xAA55;

struct BSSendFrameHead{
	UINT16 head;
	UINT32 serial;
	UINT32 len;
	//UINT16 type;
}ALIGN(1);

struct BSSendFrameTail{
	UINT32 crc32;
	UINT16 tail;
}ALIGN(1);

struct BSRecvFrameHead{
	UINT16 head;
	UINT32 serial;
	UINT32 len;
//	UINT16 nodeNum;
//	UINT16 type;
}ALIGN(1);

struct BSRecvFrameTail{
	UINT32 crc32;
	UINT16 tail;
}ALIGN(1);


BackShareType::BackShareType(UINT16 localPortCfg,void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len)):TcpServerType(localPortCfg, RecvDisposal)
{
	cout << "BackShareType INIT COMPLETED" << endl;
	RecvDispach = pfunc;
}

//BackShareType::BackShareType(UINT16 localPortCfg, void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len)):TcpServerType(localPortCfg, RecvDisposal),RecvDispach(pfunc)
//{
//	RecvDispach = pfunc;
//	cout << "BackShareType INIT COMPLETED" << endl;
//}

void BackShareType::RecvDisposal(class TcpServerType* ptr, struct NetParaType* psrc, UINT8* pdata, UINT16 len)//tcp的参数之一
{
	cout<<"已执行RecvDisposal"<<endl;
	//BackShareType* ptr_back_type;
	struct BSRecvFrameHead* phead = (struct BSRecvFrameHead*)pdata;
	struct BSRecvFrameTail* ptail = (struct BSRecvFrameTail*)(pdata + len - sizeof(struct BSRecvFrameTail));

//	AUTO_CHANGE_ENDIAN(*(UINT16*)pdata);
//	AUTO_CHANGE_ENDIAN(*(UINT32*)(pdata+2));
//	AUTO_CHANGE_ENDIAN(*(UINT32*)(pdata+6));
//	AUTO_CHANGE_ENDIAN(*(UINT16*)(pdata+10));
	UINT32 crc_formBack=getCRC32((char*)pdata,0,512);//len-6
	//AUTO_CHANGE_ENDIAN(crc_formBack);
	if(NULL != pdata)
	{
		#ifdef ENDIAN_CHANGE
				AUTO_CHANGE_ENDIAN(phead->head);
				AUTO_CHANGE_ENDIAN(phead->len);
				AUTO_CHANGE_ENDIAN(phead->serial);
				AUTO_CHANGE_ENDIAN(ptail->crc32);
				AUTO_CHANGE_ENDIAN(ptail->tail);
		#endif
		/*包头包尾验证*/
		if((FRAME_HEAD == phead->head) && (FRAME_TAIL == ptail->tail))
		{
			/*帧序号验证*/
			if(phead->serial == psrc->recvSerial)
			{
				psrc->recvSerial++;
				/*数据长度验证*/
				if((UINT8*)ptail == (UINT8*)(phead + 1) + phead->len)//这里的+1应该是指一个结构体的长度
				{

					if(ptail->crc32==crc_formBack)
					{
						if(NULL != ((BackShareType*)ptr)->RecvDispach)
						{
							((BackShareType*)ptr)->RecvDispach(ptr, psrc,pdata, len);//(UINT8*)&phead->nodeNum
						}
						else
						{

						}
					}




//					char cmd_para[20];
////					if(NULL != ((BackShareType*)ptr)->RecvDispach)
////					{
////						((BackShareType*)ptr)->RecvDispach(ptr, psrc, (UINT8*)&phead->nodeNum, len);
////					}
////					else
////					{
////
////					}
//					if(*(UINT16 *)(pdata+sizeof(struct BSRecvFrameHead))==1)//用例执行命令
//					{
//						ptr_back_type->case_act_count++;//用例执行次数自增，传给平台管理单元
//						//传给用例处理单元
//						for(int i=0;i<20;i++)
//						{
//							cmd_para[i]=*(pdata+sizeof(struct BSRecvFrameHead)+2+i);
//						}
//						string cmd_para_json(&cmd_para[0],&cmd_para[strlen(cmd_para)]);
//						//case_cmd_para c_c_para;
//						//xpack::json::decode(cmd_para,c_c_para);
//						//UINT16* cmd_para_int=(UINT16*)(char *)c_c_para.cmd_para.c_str();
//
//
//					}
//
//					else if(*(UINT16 *)(pdata+sizeof(struct BSRecvFrameHead))==2)//用例删除命令
//					{
//
//					}
//
//					else if(*(UINT16 *)(pdata+sizeof(struct BSRecvFrameHead))==3)//场景元素动作命令
//					{
//
//					}
//
//					else if(*(UINT16 *)(pdata+sizeof(struct BSRecvFrameHead))==4)//用例停止命令
//					{
//
//					}

				}
				else
				{

				}
			}
			else
			{
				psrc->recvSerial = phead->serial + 1;
			}
		}
		else
		{

		}
	}
	else
	{

	}

}


void BackShareType::PrintLocolPort(void)
{
	cout << "deived:TCP server port : " << localPort << endl;
}




STATUS_T BackShareType::SendPackage(string ip, UINT8* data, UINT16 size)//后端数据没有数据类型枚举//UINT8* ipUINT8* ip
{
	STATUS_T ret = RET_UNKNOWN_ERR;
		UINT8* pdata = (UINT8*)malloc(size + sizeof(struct BSSendFrameHead) + sizeof(struct BSSendFrameTail));
		UINT8* ptr = pdata;
		string destIP = ip;//原先是这个(char*)ip
		//string destIP=ip;
		/*拼装数据头部*/
//		AUTO_CHANGE_ENDIAN(FRAME_HEAD);
//		AUTO_CHANGE_ENDIAN(size);
		//AUTO_CHANGE_ENDIAN(FRAME_HEAD);

		((struct BSSendFrameHead *)ptr)->head = FRAME_HEAD;
		AUTO_CHANGE_ENDIAN(((struct BSSendFrameHead *)ptr)->head);
		((struct BSSendFrameHead *)ptr)->len = size;
		AUTO_CHANGE_ENDIAN(((struct BSSendFrameHead *)ptr)->len);
		struct NetParaType* pNetPara = ISItAClient(destIP);
		if(pNetPara == NULL)
		{
			return RET_ID_ERR;
		}
//这句
		//AUTO_CHANGE_ENDIAN(pNetPara->sendSerial);
		//AUTO_CHANGE_ENDIAN(((struct BSSendFrameHead *)ptr)->serial);
		//((struct BSSendFrameHead *)ptr)->serial++;
		((struct BSSendFrameHead *)ptr)->serial = pNetPara->sendSerial;
		AUTO_CHANGE_ENDIAN(((struct BSSendFrameHead *)ptr)->serial);//所有给后端的数据都得转换成大端传递,现在报错，以后再改
		//SC_AUTO_DATA_PROCESS(pNetPara->sendSerial, ((struct SCSendFrameHead*)ptr)->serial);
		pNetPara->sendSerial++;
		//((struct BSSendFrameHead *)ptr)->type = type;

		/*拼装PAYLOAD*/
		ptr = ptr + sizeof(struct BSSendFrameHead);
		memcpy(ptr, data, size);

		/*拼装数据尾部*/
		ptr = ptr + size;
		UINT32 crc_back=getCRC32((char*)pdata, 0,size + sizeof(struct BSSendFrameHead));
		//AUTO_CHANGE_ENDIAN(crc_back);
		//AUTO_CHANGE_ENDIAN(FRAME_TAIL);
		((struct BSSendFrameTail *)ptr)->crc32 = crc_back;//改为后端的crc校验，将pdata强转为char*
		((struct BSSendFrameTail *)ptr)->tail = FRAME_TAIL;
		AUTO_CHANGE_ENDIAN(((struct BSSendFrameTail *)ptr)->crc32);
		AUTO_CHANGE_ENDIAN(((struct BSSendFrameTail *)ptr)->tail);

		SendData(destIP, pdata, size + sizeof(struct BSSendFrameHead) + sizeof(struct BSSendFrameTail));

		free(pdata);
		//sleep(0.2);
		return ret;
}



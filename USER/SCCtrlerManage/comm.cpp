/*
 * comm.cpp
 *
 *  Created on: Sep 23, 2022
 *      Author: tct
 */

#include <iostream>
#include <string>
#include "comm.h"
#include "EndianChange.h"

#include "crc32.h"
using namespace std;

#define FRAME_HEAD		0x55AA
#define FRAME_TAIL		0xAA55

struct SCSendFrameHead{
	UINT16 head;
	UINT32 serial;
	UINT16 len;
	UINT16 type;
}ALIGN(1);

struct SCSendFrameTail{
	UINT32 crc32;
	UINT16 tail;
}ALIGN(1);

struct SCRecvFrameHead{
	UINT16 head;
	UINT32 serial;
	UINT16 len;
	UINT16 nodeNum;
	UINT16 type;
}ALIGN(1);

struct SCRecvFrameTail{
	UINT32 crc32;
	UINT16 tail;
}ALIGN(1);

///**
// * @brief      构造函数
// * @details
// * @param
// * @return
// *     - RET_NO_ERR  成功
// *     - ohter       失败
// */
//SCCommType::SCCommType(UINT16 localPortCfg, void (*callback)(TcpServerType* ptr,struct NetParaType* psrc, UINT8* pdata, UINT16 len)):TcpServerType(localPortCfg, callback)
//{
//	cout << "SCCommType INIT COMPLETED" << endl;
//}


/**
 * @brief      构造函数
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
SCCommType::SCCommType(UINT16 localPortCfg, void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len)):TcpServerType(localPortCfg, RecvDisposal),RecvDispach(pfunc)
{
	OnlineCB = NULL;
	OfflineCB = NULL;

	cout << "SCCommType INIT COMPLETED" << endl;
}

/**
 * @brief      构造函数
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
SCCommType::SCCommType(UINT16 localPortCfg, void (*pfunc)(void* phandler, struct NetParaType* psrc, UINT8* pdata, UINT16 len), void(*OnlineFunc)(void* phandler, struct NetParaType* ptr), void(*OfflineFunc)(void* phandler, struct NetParaType* ptr)):TcpServerType(localPortCfg, RecvDisposal)
{
	OnlineCB = OnlineFunc;
	OfflineCB = OfflineFunc;
	RecvDispach = pfunc;
	cout << "SCCommType INIT COMPLETED" << endl;
}

/**
 * @brief      析构函数
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
SCCommType::~SCCommType()
{

}

/**
 * @brief      数据接收函数
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
void SCCommType::RecvDisposal(TcpServerType* ptr, struct NetParaType* psrc, UINT8* pdata, UINT16 len)
{

	/*按协议头和协议尾解包*/
	struct SCRecvFrameHead* phead = (struct SCRecvFrameHead*)pdata;
	struct SCRecvFrameTail* ptail = (struct SCRecvFrameTail*)(pdata + len - sizeof(struct SCRecvFrameTail));
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
				if((UINT8*)ptail == (UINT8*)(phead + 1) + phead->len)
				{
					if(NULL != ((SCCommType*)ptr)->RecvDispach)
					{
						((SCCommType*)ptr)->RecvDispach(ptr, psrc, (UINT8*)&phead->nodeNum, len);
					}
					else
					{

					}
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

/**
* @brief	按数据包格式发送数据
* @param 	UINT8* ip,  			目的地址
* @param	UINT8* data, 			数据载荷
* @param	UINT16 size,			数据长度
* @param	UINT8 type				数据载荷类型
* @return 	STATUS_T 返回是否执行成功
*/
STATUS_T SCCommType::SendPackage(UINT8* ip, UINT8* data, UINT16 size, enum SCSendDatatype type)
{
	STATUS_T ret = RET_UNKNOWN_ERR;
	UINT8* pdata = new UINT8(size + sizeof(struct SCSendFrameHead) + sizeof(struct SCSendFrameTail));
	UINT8* ptr = pdata;
	string destIP = (char*)ip;

	struct NetParaType* pNetPara = ISItAClient(destIP);
	if(ptr == NULL)
	{
		return RET_ID_ERR;
	}

	/*拼装数据头部*/
	((struct SCSendFrameHead*)ptr)->head = FRAME_HEAD;
	SC_AUTO_DATA_PROCESS(((struct SCSendFrameHead*)ptr)->head, ((struct SCSendFrameHead*)ptr)->head);
	SC_AUTO_DATA_PROCESS(size, ((struct SCSendFrameHead*)ptr)->len);
	SC_AUTO_DATA_PROCESS(pNetPara->sendSerial, ((struct SCSendFrameHead*)ptr)->serial);
	pNetPara->sendSerial++;
	SC_AUTO_DATA_PROCESS(type, ((struct SCSendFrameHead*)ptr)->type);

	/*拼装PAYLOAD*/
	ptr = ptr + sizeof(struct SCSendFrameHead);
	memcpy(ptr, data, size);

	/*拼装数据尾部*/
	ptr = ptr + size;
	((struct SCSendFrameTail*)ptr)->crc32 = Crc32(pdata, size + sizeof(struct SCSendFrameHead));
	SC_AUTO_DATA_PROCESS(((struct SCSendFrameTail*)ptr)->crc32, ((struct SCSendFrameTail*)ptr)->crc32);
	((struct SCSendFrameTail*)ptr)->tail = FRAME_TAIL;
	SC_AUTO_DATA_PROCESS(((struct SCSendFrameTail*)ptr)->tail, ((struct SCSendFrameTail*)ptr)->tail);

	SendData(destIP, pdata, size + sizeof(struct SCSendFrameHead) + sizeof(struct SCSendFrameTail));

	delete(pdata);

	return ret;
}
/**
 * @brief      打印当前监测的端口
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
void SCCommType::PrintLocolPort(void)
{
	cout << "deived:TCP server port : " << localPort << endl;
}

/**
 * @brief      上线通知函数
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
void SCCommType::OnlineNotify(struct NetParaType* ptr)
{
	cout << "welcome ip " << ptr->ip << " prot " << ptr->port << endl;
	if(NULL != OnlineCB)
		OnlineCB(this, ptr);
}

/**
 * @brief      离线通知函数
 * @details
 * @param
 * @return
 *     - RET_NO_ERR  成功
 *     - ohter       失败
 */
void SCCommType::OfflineNotify(struct NetParaType* ptr)
{
	cout << "Bye bye client ip: " << ptr->ip << " prot " << ptr->port << endl;
	if(NULL != OfflineCB)
		OfflineCB(this, ptr);
}

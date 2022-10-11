/********************************************************
*                                                      
*  文件名   ： CommonMemory.c         
*  版权说明 ： 北京交控科技有限公司 
*  版本号   ： 1.0
*  创建时间 ： 2012.07.17
*  作者     ： 研发中心软件部
*  功能描述 ： 内存操作函数                                                                    
*  修改记录 ：  
*
********************************************************/ 

#include "CommonMemory.h"
#include "string.h"


/*
* 功能说明 : 内存赋值函数。
* 参数说明 : void *pDest, 目的地址
*            UINT8 c,待赋的值
*            UINT32 count,赋值内存长度
*            UINT32 destBuffSize，待赋值缓冲区总长度。
* 返回值:    0, 未赋值，赋值长度大于报警长度
*            1, 常规赋值
*/
UINT8 CommonMemSet(void* pDest, UINT32 destBuffSize, UINT8 c, UINT32 count)
{
	UINT8 Result = 0; /* 返回结果 */

	if(count > destBuffSize)
	{ 
		/* 赋值长度异常，不赋值*/
		Result = 0;
	}
	else 
	{
		/*内存赋值*/
		memset(pDest,c,count);
		Result = 1;
	}

	return Result;
}

/* 函数功能 : 内存拷贝函数。
*  参数说明 : void *pDest, 目的地址
*             UINT32 destBuffSize, 缓冲区长度，最大可用拷贝的数据长度
*             const void *pSrc, 源地址
*             UINT32 count, 拷贝长度
*  返回值:    0, 未拷贝，拷贝长度大于报警长度
*             1, 拷贝成功
*/
UINT8 CommonMemCpy(void *pDest, UINT32 destBuffSize, const void *pSrc, UINT32 count)
{
	UINT8 Result = 0; /* 返回结果 */

	if(count > destBuffSize)
	{ 
		/* 拷贝长度异常，不拷贝*/
		Result = 0;
	}
	else 
	{
		/*内存复制*/
		memcpy(pDest,pSrc,count);
		Result = 1;

	}

	return Result;
}


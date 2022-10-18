/**
 * @file       common.h
 * @brief      变量类型定义
 * @details    变量类型定义
 * @author     GT-BU
 * @date       2022-01-10
 * @version    V1.0
 * @copyright  交控科技股份有限公司
 */

#ifndef PLATFORM_COMMON_COMMON_H_
#define PLATFORM_COMMON_COMMON_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define ENDIAN_CHANGE

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OK
#define OK 0
#endif

#ifndef ERROR
#define ERROR (-1)
#endif

#ifdef ENDIAN_CHANGE
#define COMMON_AUTO_DATA_PROCESS(data, dest) 		AutoChangeEndian(&(data), sizeof((data)), &(dest));
#else
#define COMMON_AUTO_DATA_PROCESS(data, dest)		dest = data;
#endif

#define ALIGN(n)  __attribute__((packed, aligned(n)))
#define LITTLE_ENDIAN_SQUENCE
/* ERROR FIELD */
#define OPEROK      0
#define EINPUT      1
#define EQUEUE      2
#define ECOUNT      3
#define EQUEST      4
#define EFRAME      5


#ifndef container_of
#define container_of(ptr, type, member) ({                 \
        const typeof( ((type*)0)->member ) *__mptr = (ptr);     \
        (type *)( (char*)__mptr - offsetof(type, member) );})
#endif

typedef char                CHAR;
typedef unsigned char       UCHAR;
typedef signed char         INT8;
typedef unsigned char       UINT8;
typedef short int           INT16;
typedef unsigned short int  UINT16;
typedef int                 INT32;
typedef unsigned int        UINT32;
typedef long long			INT64;
typedef unsigned long long  UINT64;
typedef size_t				SIZE_T;

#ifdef LINUX
typedef int   STATUS;
#endif /* LINUX */

typedef unsigned long GP_THREAD_T;
typedef void *(* GP_THREAD_FUNC_T)(void *);

enum StatType{
	abnormal = 0,
	normal = 1,
};

/**
 * @enum   STATUS_T
 * @brief  返回值类型枚举
 */
typedef enum
{
    RET_NO_ERR			= 0,    /* 无错误 */
	RET_PARAM_ERR		= -1,   /* 参数错误 */
	RET_ID_ERR			= -2,   /* 没有匹配的ID */
	RET_UNREG_ERR		= -3,   /* 节点未注册 */
	RET_TIMEOUT_ERR		= -4,   /* 超时 */
	RET_DATA_ERR		= -5,   /* 无有效数据 */
	RET_SOCK_ERR		= -6,   /* 套接字设置错误 */
	RET_IO_ERR			= -7,   /* 套接字错误,无法接收或发送数据 */
    RET_MEM_ERR			= -8,   /* 无可用内存 */
    RET_SEMA_ERR		= -9,   /* 信号量无效 */
    RET_QUEUE_ERR		= -10,  /* 队列空错误 */
    RET_QUEUE_FULL_ERR	= -11,  /* 队列满错误 */
    RET_MUTEX_ERR		= -12,  /* 互斥量无效 */
    RET_THREAD_ERR		= -13,  /* 线程创建失败 */
    RET_NOCONN_ERR		= -14,  /* 无网络连接 */
	RET_BUSY			= -15,	/* 平台正忙 */
	RET_WORK_ERROR		= -16, 	/* 工作异常 */
    RET_UNKNOWN_ERR		= -99   /* 未知错误 */
} STATUS_T;

/**
 * @enum   BOOL_T
 * @brief  布尔类型枚举
 */
typedef enum
{
	B_FALSE,       /* 否 */
	B_TRUE         /* 是 */
} BOOL_T;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PLATFORM_COMMON_COMMON_H_ */

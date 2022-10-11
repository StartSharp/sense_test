/*******************************************************************************
* 文件名      ：  EndianChange.h
* 版权说明  ：交控科技股份有限公司
* 版本号      ：  1.0
* 创建时间  ：2020年11月9日
* 作者         ：  交控研究院
* 功能描述  ：  
* 使用注意  ：无
* 修改记录  ：无
*******************************************************************************/

/*
 * EndianChange.h
 *
 *  Created on: 2020年11月9日
 *      Author: Steven
 */

#ifndef COMMON_ENDIANCHANGE_H_
#define COMMON_ENDIANCHANGE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "commontypes.h"

#define AUTO_CHANGE_ENDIAN(para) 	AutoChangeEndian(&(para), sizeof((para)), &(para))

/*  函数功能：16位换端函数
 *  输入参数：换端src
 *  输出参数：
 *  返回值    ：转换后的数据
 *  使用注意：无。
 */
UINT16 ChangeEndian16(UINT16 src);

/*  函数功能：32位换端函数
 *  输入参数：换端src
 *  输出参数：
 *  返回值    ：转换后的数据
 *  使用注意：无。
 */
UINT32 ChangeEndian32(UINT32 src);

/*  函数功能：64位换端函数
 *  输入参数：换端src
 *  输出参数：
 *  返回值    ：转换后的数据
 *  使用注意：无。
 */
UINT64 ChangeEndian64(UINT64 src);

/*  函数功能：带参数的大小端转换 只能转换长度为8 16 32 64长度的数据
 *  输入参数：换端src 长度
 *  输出参数：转换后的数据
 *  返回值    ：
 *  使用注意：无。
 */
void AutoChangeEndian(void* psrc, UINT8 len, void* pdes);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* COMMON_ENDIANCHANGE_H_ */

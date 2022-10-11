/*******************************************************************************
* 文件名      ：  LinkList.h
* 版权说明  ：交控科技股份有限公司
* 版本号      ：  1.0
* 创建时间  ：2020年9月28日
* 作者         ：  交控研究院
* 功能描述  ：  
* 使用注意  ：无
* 修改记录  ：无
*******************************************************************************/

/*
 * LinkList.h
 *
 *  Created on: 2020年9月28日
 *      Author: Steven
 */

#ifndef COMMON_LINKLIST_H_
#define COMMON_LINKLIST_H_
#include "commontypes.h"

#define DEBUG_PRINT printf

/*  函数功能：创建一个链表
 *  输入参数：节点数目，每个节点的大小
 *  输出参数：无。
 *  返回值    ：返回值为0申请失败，返回值不为0则正常
 *  使用注意：
 */
void* CreatLink(int n, UINT16 size);

/*  函数功能：删除一个节点
 *  输入参数：节点数目，每个节点的大小
 *  输出参数：无。
 *  返回值    ：
 *  使用注意：
 */
INT8 DeleteLink(void *list, int n);

/*  函数功能：查找一个节点
 *  输入参数：list链表头地址，第n个元素
 *  输出参数：无。
 *  返回值    ：返回其地址
 *  使用注意：
 */
void* FindNode(void *list, int n);

/*  函数功能：插入一个节点
 *  输入参数：list链表头地址，第n个元素
 *  输出参数：无。
 *  返回值    ：返回其地址
 *  使用注意：
 */
INT8 InsertNode(void *list, int n, UINT16 size);

/*  函数功能：统计链表节点数目
 *  输入参数：list链表头地址
 *  输出参数：
 *  返回值    ：返回链表节点数目
 *  使用注意：
 */
UINT16 GetLinkNodeCount(void *list);

#endif /* COMMON_LINKLIST_H_ */

/*******************************************************************************
* 文件名      ：  LinkList.c
* 版权说明  ：交控科技股份有限公司
* 版本号      ：  1.0
* 创建时间  ：  2020年9月28日
* 作者         ：  交控研究院
* 功能描述  ：  
* 使用注意  ：无
* 修改记录  ：无
*******************************************************************************/
#include "LinkList.h"
#include "CommonMemory.h"
#include "commontypes.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

INT8 DeleteLink(void *list, int n);

struct LinkList_t{
    void* next;
};

/*  函数功能：创建一个链表
 *  输入参数：节点数目，每个节点的大小
 *  输出参数：无。
 *  返回值    ：返回值为0申请失败，返回值不为0则正常
 *  使用注意：
 */
void* CreatLink(int n, UINT16 size)
{
    struct LinkList_t *head, *node, *end;/*定义头节点，普通节点，尾部节点；*/
    UINT8 i, j;

    head = (struct LinkList_t*)malloc(size);/*分配地址*/
    if(NULL != head)
    {
        /*memset(head, 0, size);*/
        CommonMemSet(head, size, 0, size);
        end = head;         /*若是空链表则头尾节点一样*/

        for(i = 0; i < n; i++)
        {
            node = (struct LinkList_t*)malloc(size);
            if(NULL != node)
            {
                /*memset(node, 0, size);*/
                CommonMemSet(node, size, 0, size);
                end->next = node;
                end = node;
            }
            else
            {
                for(j = i; j > 0; j--)
                {
                    DeleteLink(head ,j);
                }
            }
        }
        end->next = NULL;/*结束创建*/
    }
    else
    {

    }

    return head;
}

/*  函数功能：删除一个节点
 *  输入参数：
 *              void *list, 节点头指针
 *              int n       节点序号
 *
 *  输出参数：无。
 *  返回值    ：
 *  使用注意：
 */
INT8 DeleteLink(void *list, int n)
{
    struct LinkList_t *node = (struct LinkList_t*)list;
    struct LinkList_t *front;
    int i = 0;
    INT8 ret = -EINPUT;

    while ((i < n) && (NULL != node))
    {
        front = node;
        node = node->next;
        i++;
    }

    if (NULL != node) {
        front->next = node->next;
        free(node);
        node = NULL;
        ret = OPEROK;
    }
    else {
        DEBUG_PRINT("Trying to delete a not exist link node\r\n");
        ret = -EINPUT;
    }

    return ret;
}

/*  函数功能：查找一个节点
 *  输入参数：list链表头地址，第n个元素
 *  输出参数：无。
 *  返回值    ：返回其地址
 *  使用注意：
 */
void* FindNode(void *list, int n)
{
    struct LinkList_t *t = (struct LinkList_t*)list;
    int i = 0;

    while ((i < n) && (NULL != t))
    {
        t = t->next;
        i++;
    }

    return t;
}

/*  函数功能：插入一个节点
 *  输入参数：list链表头地址，第n个元素
 *  输出参数：无。
 *  返回值    ：返回其地址
 *  使用注意：
 */
INT8 InsertNode(void *list, int n, UINT16 size)
{
    struct LinkList_t *t = (struct LinkList_t*)list;
    struct LinkList_t *in;
    int i = 0;
    INT8 ret = -EINPUT;

    while ((i < n) && (NULL != t))
    {
        t = t->next;
        i++;
    }

    if (NULL != t)
    {
        in = (struct LinkList_t*)malloc(size);
        in->next = t->next;/*填充in节点的指针域，也就是说把in的指针域指向t的下一个节点*/
        t->next = in;/*填充t节点的指针域，把t的指针域重新指向in*/
        ret = OPEROK;
    }
    else
    {
        DEBUG_PRINT("Not found the %d-th node!\r\n", n);
        ret =  -EINPUT;
    }

    return ret;
}

/*  函数功能：统计链表节点数目
 *  输入参数：list链表头地址
 *  输出参数：
 *  返回值    ：返回包含头节点的链表节点数目
 *  使用注意：
 */
UINT16 GetLinkNodeCount(void *list)
{
    struct LinkList_t *t = (struct LinkList_t*)list;
    int i = 0;

    while (NULL != t)
    {
        t = t->next;
        i++;
    }

    return i;
}


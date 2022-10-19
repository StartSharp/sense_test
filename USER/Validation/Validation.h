/**
@file  Validation.h
@brief  
@author 交控研究院
@version 1.0.0.0
@date Sep 27, 2022
*/
/*
 * Validation.h
 *
 *  Created on: Sep 27, 2022
 *      Author: tct
 */

#ifndef VALIDATION_VALIDATION_H_
#define VALIDATION_VALIDATION_H_

#include "commontypes.h"
using namespace std;

/*时间切片后的数据采样*/
class simple_data_type{
protected:
	UINT16 standard;			/*标准结果*/
	UINT16 sample;				/*采样结果*/
	UINT8 result;				/*验证结果*/
	UINT16 sampler_id;			/*采样值匹配的ID*/
public:


	INT16 get_data(UINT8* pdata, UINT16 len);

};

class validation_type{
protected:
	UINT16 mask_switch;			/*验证结果屏蔽开关*/

public:
	void validation(void);
	INT16 get_result(UINT8* pbuf, UINT16 buf_size, UINT16* psize);

};


/**
* @brief 
* @param 
* @param 
* @return
*/


#endif /* VALIDATION_VALIDATION_H_ */

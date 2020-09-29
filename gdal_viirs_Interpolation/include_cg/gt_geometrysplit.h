/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  
* Author:   
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c)  保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYSPLIT_H_INCLUDED
#define GTSGEOMETRYSPLIT_H_INCLUDED

#include "gts_cg_port.h"

namespace gts{

class GTS_DLL GTGeometrySplit
{
public:
	GTGeometrySplit(void);
	~GTGeometrySplit(void);

	/*将一个目标图层分割为不同的块, 用于切割的必须是polygon*/
};

}
#endif
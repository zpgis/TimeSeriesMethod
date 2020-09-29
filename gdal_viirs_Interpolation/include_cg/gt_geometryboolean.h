/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  几何对象之间的布尔与剪切
* 
* Author:   范俊甫, fanjf@lreis.ac.cn 2011年-05月-11日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c) 保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYBOOLEAN_H_INCLUDED
#define GTSGEOMETRYBOOLEAN_H_INCLUDED

#include "gts_cg_port.h"
#include "gt_geometryanalysis.h"

namespace gts{

class GTS_DLL GTGeometryBoolean
{
public:
	GTGeometryBoolean(void);
	~GTGeometryBoolean(void);

	/*环与环*/
	static int ringAndRingBoolean(
		GTRing                   *pRing1,
		GTRing                   *pRing2,
		enumGTBooleanType         blType,
		std::vector<GTRing *>    &ring_list,       /*结果指针数组, 注意释放*/
		std::vector<int>         &island_mark,     /*0为外部, 1为内部*/
		bool                      bCheck = false); /*是否检查退化情况*/ 
	

private:

};

}
#endif
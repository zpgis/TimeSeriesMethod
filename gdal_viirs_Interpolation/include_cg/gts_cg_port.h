/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  空间分析类基础宏和系统变量定义
*           包含:空间关系/基础空间计算/布尔运算/Buffer/Overlay/凸包/
*           Voronoi/矢-栅转换/空间变换/线-多边形转换/自相交分解/Dissolve 
* Author:   范俊甫, fanjf@lreis.ac.cn 2010年-12月-15日
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* 版权所有   (c) 中国科学院地理科学与资源研究所. 保留所有权利
****************************************************************/
#ifndef GTSCGPORT_H_INCLUDED
#define GTSCGPORT_H_INCLUDED

/*GT基础数据模型*/
#include "gts_port.h"
#include "gt_datasource.h"
#include "gt_spatialindex.h"

/*OpenMP*/
#ifdef _OPENMP
# include <omp.h>
#	define OPENMP_THREADS_NUM 8	/*增加宏为多核计算时定义核数：刘衍，2011-7-18*/
#endif

#ifndef GTS_XOR
#	define GTS_XOR(a, b) a^b
#endif

#ifndef GTS_NXOR
#	define GTS_NXOR(a, b) !(a^b)
#endif

#ifndef GTS_AND
#	define GTS_AND(a, b) a&&b
#endif

#ifndef GTS_OR
#	define GTS_OR(a, b) a||b
#endif

#ifndef GTS_NOT
#	define GTS_NOT(a) !a
#endif

#ifndef GTS_FREE
#   define GTS_FREE(p) if(p){free(p); p = 0;}
#endif

#ifndef GTS_DELETE
#   define GTS_DELETE(p) if(p){delete p; p = 0;}
#endif

#ifndef GTS_DELETES
#   define GTS_DELETES(p) if(p){delete[] p; p = 0;}
#endif
/*liuyan add at 2011-8-18*/
#define GTS_RADTODGR	(0.0174532925199433)
/*add end*/
namespace gts{

/*布尔操作类型定义*/
typedef enum enumGTBooleanType{
	GT_BOOL_DIFFE  = 1,/*A-B与B-A不同, 即从第一个内将与第二个重叠部分删除*/
	GT_BOOL_INTEC  = 2,/*用于剪切, 求出公共部分*/
	GT_BOOL_XOR    = 3,/*求出所有不同的部分*/
	GT_BOOL_UNION  = 4 /*用于Overlay和合并*/
};

/*Buffer操作类型定义*/
typedef enum enumGTBufferType{
	GT_BUFFER_OUTER  = 1,
	GT_BUFFER_EDGE   = 2
};

typedef bool (*checkCondition)(GTFeature *f1, GTFeature *f2,...);

typedef enum enumGTBufferDissolveType{
	GT_BUFFER_DISSOLVE_NONE			= 0,
	GT_BUFFER_DISSOLVE_ALL			= 1,
	GT_BUFFER_DISSOLVE_LIST			= 2,
	GT_BUFFER_DISSOLVE_INTERSECT	= 3
};

/*Overlay操作类型定义*/
typedef enum enumGTOverlayType{
	GT_OVERLAY_ERASE           = 1,
	GT_OVERLAY_IDENTITY        = 2,
	GT_OVERLAY_INTERSECT       = 3,
	GT_OVERLAY_SPATIALJOIN     = 4,
	GT_OVERLAY_SYMMETRICALDIFF = 5,
	GT_OVERLAY_UNION           = 6,
	GT_OVERLAY_UPDAT           = 7
};
/*liuyan add at 2011-8-18*/
struct GTGCP{
	GTRawPoint pt_maporg;
	GTRawPoint pt_georef;
	double	   dz_value;
};
/*add end*/

using namespace gts;

}
#endif

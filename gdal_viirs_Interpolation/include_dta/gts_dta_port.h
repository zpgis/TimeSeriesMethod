/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  数字高程和流域分析类基础宏和系统变量定义
* Author:   范俊甫, fanjf@lreis.ac.cn, 2010年-12月-24日
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* 版权所有   (c) 中国科学院地理科学与资源研究所. 保留所有权利
****************************************************************/
#ifndef GTSDTAPORT_H_INCLUDED
#define GTSDTAPORT_H_INCLUDED

/*GT基础数据模型*/
#include "gts_port.h"
#include "gt_datasource.h"

/*GDAL*/
#include "ogr_spatialref.h"
#include "gdalwarper.h"
#include "cpl_string.h"
#include "ogr_core.h"
#include "ogr_api.h"
#include "ogrsf_frmts.h"
#include "gdal_priv.h"
#include "gdal_alg.h"
#include "gdal.h"

/*OpenMP*/
#ifdef _OPENMP
# include <omp.h>
#endif

#define __WINDOWS 0

#ifdef __WINDOWS
#define PLATFORM 0
#else
#define PLATFORM 1
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

namespace gts{

/*
 *计算shade的参数结构
 */
typedef struct
{
	double nsres;
	double ewres;
	double sin_altRadians;
	double cos_altRadians_mul_z_scale_factor;
	double azRadians;
	double square_z_scale_factor;
}GTHillshadeAlgData;

/*
 *color relief的参数结构
 *高程值对应RGB和Alpha值
 */
typedef struct
{
	double dfVal; 
	int nR;
	int nG;
	int nB;
	int nA;
}GTDEMColorEntry;

/*
 *color relief的参数结构
 *高程值对应颜色的选择方法
 */
typedef enum
{
	COLOR_SELECTION_MIN_MAX,
	COLOR_SELECTION_INTERPOLATE,
	COLOR_SELECTION_NEAREST_ENTRY,
	COLOR_SELECTION_EXACT_ENTRY,
}GTDEMColorSelectionMode;

/*
 *计算坡度的参数结构
 */
typedef struct
{
	double nsres;
	double ewres;
	double scale;
	int    slopeFormat; /*1为角度, 0为百分比*/
}GTSlopeAlgData;

/*
 *计算坡向的参数结构
 */
typedef struct
{
	int bAngleAsAzimuth;/*正北为0*/
}GTAspectAlgData;

/*
 *计算一般指标的参数结构
 */
typedef struct
{
	double nsres;/*南北分辨率*/
	double ewres;/*南北分辨率*/
	double scale;/*分辨率转化为m的参数*/
}GTDEMGeneralAlgData;

/*
 *CURVATURE类型
 */
typedef enum
{
	CURVATURE_NORMAL,
	CURVATURE_MEAN,
	CURVATURE_DIFFERENCE,
	CURVATURE_HORIZONTAL,
	CURVATURE_PLAN,
	CURVATURE_PROFILE,
	CURVATURE_ROTOR,
	CURVATURE_HORIZONTAL_EXCESS,
	CURVATURE_VERTICAL_EXCESS,
	CURVATURE_MINIMAL,
	CURVATURE_MAXIMAL,
	CURVATURE_TOTAL_GAUSSIAN,
	CURVATURE_TOTAL_RING,
	CURVATURE_TOTAL_ACCUMULATION
}GTDEMCurvatureType;

/*
 *邻域分析算法类型
 */
typedef enum
{
	NeighborStatistic_Majority,
	NeighborStatistic_Maximum,
	NeighborStatistic_Mean,
	NeighborStatistic_Median,
	NeighborStatistic_Minimum,
	NeighborStatistic_Minority,
	NeighborStatistic_Range,
	NeighborStatistic_STD,
	NeighborStatistic_Sum,
	NeighborStatistic_Varity,
}GTDEMNeighborStatisticType;

}
using namespace gts;

#endif

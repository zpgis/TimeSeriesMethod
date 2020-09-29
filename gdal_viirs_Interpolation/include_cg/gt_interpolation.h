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
#ifndef GTSINTERPOLATION_H_INCLUDED
#define GTSINTERPOLATION_H_INCLUDED

#include "gts_cg_port.h"
#include "ogr_spatialref.h"
#include "gdalwarper.h"
#include "cpl_string.h"

namespace gts{

class GTS_DLL GTInterpolation
{
public:
	GTInterpolation(void);
	~GTInterpolation(void);

	/*离散点插值到栅格, 使用 GDALGridCreate*/
	static bool interpolatePointsToGrid(
		GDALGridAlgorithm eAlgorithm,
		const int  	      nPoints,
		const double *    padfX, /*X坐标*/
		const double *    padfY, /*Y坐标*/
		const double *    padfZ, /*Z值*/
		const GTEnvelop&  env,   /*空间范围*/
		const int  	      nXSize,
		const int  	      nYSize,
		enumGTDataType    eType,
		void *            pData);

	static bool interpolatePointsToGridSource(
		GTFeatureLayer   *pLayer,       /*输入点层*/  
		unsigned int      iField,       /*值的区域*/
		const char       *pszFormatTo,  /*目标数据源格式*/
		const char       *pszSourceTo,  /*目标数据源路径*/
		GDALGridAlgorithm eAlgorithm);  /*算法*/

	/*几何对象生成栅格数据, 使用GDALRasterizeGeometries*/
	static bool interpolateGeometryToGrid(
		GTRasterDataset          *poDataset,      /*必须以更新方式打开*/
		std::vector<GTGeometry*> &geomList,       /*几何对象列表*/
		int                       geomCount,      /*几何对象个数*/
		std::vector<double>      &dBurnValueList);/*转换后的数值*/

	static bool interpolateGeometryToGridSource(
		GTFeatureLayer *pLayer,       /*输入点层*/  
		unsigned int    iField,       /*值的区域*/
		const char     *pszFormatTo,  /*目标数据源格式*/
		const char     *pszSourceTo,  /*目标数据源路径*/
		const double    dResol);      /*空间分辨率*/

};

}
#endif

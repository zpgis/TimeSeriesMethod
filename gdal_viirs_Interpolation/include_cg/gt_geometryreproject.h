/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  几何对象的投影转换
* 
* Author:   范俊甫, fanjf@lreis.ac.cn 2011年-05月-11日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c) 保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYREPROJECT_H_INCLUDED
#define GTSGEOMETRYREPROJECT_H_INCLUDED

#include "gts_cg_port.h"
#include "ogr_spatialref.h"
#include "gdalwarper.h"
#include "cpl_string.h"

namespace gts{

class GTS_DLL GTGeometryReproject
{
public:
	/*构造初始化, 如果两个坐标系统相同, 则不能构建*/
	GTGeometryReproject(
		GTSpatialReference* fromSRS, 
		GTSpatialReference* toSRS);

	~GTGeometryReproject();

	/*静态销毁函数, 对象必须new来分配*/
	static void destroyGeometryTransform(GTGeometryReproject *poGeomReproject);

	/*是否有效*/
	bool getIsValid();

	/*原始坐标的WKT串*/
	const char* getFromSRSWKT();

	/*目标坐标的WKT串*/
	const char* getToSRSWKT();

	/*转换几何数据, 输入的几何数据的坐标必须与源坐标系统相同*/
	/*生成新的几何对象由外部GTS_DELETE释放*/
	GTGeometry *geometryCoordinateTransform1(const GTGeometry* pGeometry);

	/*转换几何数据, 输入的几何数据的坐标必须与源坐标系统相同*/
	/*原始几何对象的数据将被替换*/
	bool geometryCoordinateTransform2(GTGeometry* pGeometry);

	/*获取一个Envelop近似在目标坐标系统下的范围*/
	bool getApprEnvelop(const GTEnvelop& srcEnv, GTEnvelop& dstEnv);

	/*获取一个Envelop的反变换近似值*/
	bool getApprInverseEnvelop(const GTEnvelop& srcEnv, GTEnvelop& dstEnv);

protected:
	bool isValid;
	OGRCoordinateTransformation *poCT;
	char *pszSrcWKT;
	char *pszDstWKT;
};

}
#endif

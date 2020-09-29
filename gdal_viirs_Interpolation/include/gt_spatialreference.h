/****************************************************************
 *
 * Project:  地理信息处理工具集
 * Purpose:  空间参考对象类的定义, 封装GDAL/OGR
 * Author:   范俊甫, fanjf@lreis.ac.cn 2010年-11月-23日
 *
 ****************************************************************
 * Copyright (c) All rights reserved.
 * 版权所有  (c) 保留所有权利
 ****************************************************************/
#pragma once
#ifndef GTSPATIALREFERENCE_H_INCLUDED
#define GTSPATIALREFERENCE_H_INCLUDED

#include "gts_port.h"
#include "gt_geometry.h"
#include "../include_p/gdal/cpl_conv.h"
#include "../include_p/gdal/ogr_spatialref.h"

namespace gts{

/*
 *空间投影定义类, 内部使用WKT格式来管理投影信息
 */
class GTS_DLL GTSpatialReference
{
public:
	GTSpatialReference();
	~GTSpatialReference();
	GTSpatialReference(const GTSpatialReference& spRefObj);
	GTSpatialReference& operator = (const GTSpatialReference& spRefObj);
	bool operator == (const GTSpatialReference& spRefObj) const {
		return isSameSpatialReference(spRefObj);
	}

	/*比较两个空间参照系统是否相同*/
	bool isSameSpatialReference(const GTSpatialReference& spRefObj) const;

	/*利用已知的EPSG(SRID)编号进行投影定义的初始化 4326=WGS84
	  EPSG与OGC的SRID是一致的, 也许部分数据库自己的SRID表有差别*/
	enumGTDataErr importFromEPSG(int epsgNo);
	
	/*由Porj4投影定义字符串导入, 比如: "+proj=utm +zone=11 +datum=WGS84"*/
	enumGTDataErr importFromProj4(const char* proj4Str);

	/*由WKT投影定义字符串导入*/
	enumGTDataErr importFromWkt(const char* wktStr);
    
	/*返回已定义投影的WKT表达, 必须使用指针地址传递才能返回给应用程序
	 需要外部调用CPLFree释放srsInfo, 重要：由哪个库分配的最好由那个库释放
	 推荐用法：
	   char *sInfo = 0; 
	   exportToWkt(&sInfo);
	   CPLFree(sInfo)*/
	enumGTDataErr exportToWkt(char** srsInfo) const;

	/*返回XML投影表达, 用法与上面函数类似, 但需要构造XML结构, 程序运行效率低*/
	enumGTDataErr exportToXML(char** srsInfo) const;

	/*获取OGRSpatialReference指针地址*/
	OGRSpatialReference *getOGRSpatialRefPtr() { return &ogrSpatialRef; }
	const OGRSpatialReference *getOGRSpatialRefPtr() const { return &ogrSpatialRef; }

	/*获取EPSG授权号*/
	int getAuthorityCode() const;

	/*是否为大地坐标系*/
	int isGeographic() const { return ogrSpatialRef.IsGeographic(); }

	/*是否为投影坐标系*/
	int isProjected() const { return ogrSpatialRef.IsProjected(); }

	/*获取大地坐标的单位, 即弧度"radian"
	 *或者是角度"degree"(0.0174532925199433)
	 *用法与上面char**函数类似*/
	enumGTUnitType getAngularUnits(char** unitInfo) const;

	/*获取投影坐标的单位, 即:
	 *Meter, Foot, Foot_US, Nautical Mile, Link
	 *Chain, Rod
	 *用法与上面char**函数类似*/
	enumGTUnitType getLinearUnits(char** unitInfo) const;

	/*几何对象的投影转换*/
	/*生成新的几何对象, 需要外部GTS_DELETE释放*/
	static GTGeometry *geometryCoordinateTransform(const GTGeometry* pGeometry, 
		                                           const GTSpatialReference* fromSRS, 
											       const GTSpatialReference* toSRS);

protected:
	OGRSpatialReference ogrSpatialRef;
};

}
#endif

/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  空间数据坐标转换的定义类
* Author:   范俊甫, fanjf@lreis.ac.cn 2011年-03月-21日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有  (c) 保留所有权利
****************************************************************/
#pragma once
#ifndef GTWARP_H_INCLUDED
#define GTWARP_H_INCLUDED

#include "gt_datasource.h"
#include "gt_string.h"
#include "../include_p/gdal/ogr_spatialref.h"
#include "../include_p/gdal/gdalwarper.h"
#include "../include_p/gdal/cpl_string.h"

namespace gts{

/*
 *GTWarp系列类的基类
 */
class GTS_DLL GTWarp
{
public:
	GTWarp(void);
	virtual ~GTWarp(void);
};

/*
 *GTRasterWarp类
 */
class GTS_DLL GTRasterWarp: public GTWarp
{
public:
	GTRasterWarp(void);
	~GTRasterWarp(void);

	/*
	 *用于简单投影变换的静态函数
	 */
	static bool simpleRasterWarp(
		const char* pszFormatTo,            /*目标数据源格式*/
		const char* pszSourceFrom,          /*数据源路径*/
		const char* pszSourceTo,            /*目标数据源路径*/
		const char* pszSRSWKT,              /*目标数据源空间参考*/
		GDALResampleAlg eResampleAlg = GRA_NearestNeighbour, /*重采样方法*/
		double cacheLimit            = 0.0, /*内存缓冲使用大小限制, 单位是MB*/
		enumGTDataType eOutDataType  = GTD_Unknown,  /*输出数据类型*/
		const double*  padfSrcNoData = 0,   /*源的Nodata列表, 要与Band数目对应, 不参与重采样*/
		int iSrcNoDataCount          = 0,   /*源的Nodata列表项个数*/
		const double*  padfDstNoData = 0,   /*目标的Nodata列表, 要与Band数目对应*/
		int iDstNoDataCount          = 0);  /*目标的Nodata列表项个数*/

	/*
	 *用于图像的重采样
	 */
	static bool rasterResampling(
		const char* pszFormatTo,            /*目标数据源格式*/
		const char* pszSourceFrom,          /*数据源路径*/
		const char* pszSourceTo,            /*目标数据源路径*/
		double xResl,                       /*目标数据的x分辨率*/
		double yResl,                       /*目标数据的y分辨率*/
		GDALResampleAlg eResampleAlg = GRA_NearestNeighbour, /*重采样方法*/
		double cacheLimit            = 0.0); /*内存缓冲使用大小限制, 单位是MB*/

	/*
	 *用于由GCPs进行图像的投影变换
	 */
	static bool rasterWarpByGCP(
		const char* pszFormatTo,            /*目标数据源格式*/
		const char* pszSourceFrom,          /*数据源路径*/
		const char* pszSourceTo,            /*目标数据源路径*/
		const char* pszSRSWKT,              /*目标数据源空间参考*/
		int  	    nGCPCount,              /*GCP个数*/
		GDAL_GCP*  	pasGCPList,             /*GCP列表*/
		int  	    nReqOrder,              /*多项式次数1-3*/
		GDALResampleAlg eResampleAlg = GRA_NearestNeighbour, /*重采样方法*/
		double cacheLimit            = 0.0); /*内存缓冲使用大小限制, 单位是MB*/
};

/*
 *矢量/栅格转换的类
 */
class GTS_DLL GTRasterizeAndPolygonize: public GTWarp
{
public:
	GTRasterizeAndPolygonize(void);
	~GTRasterizeAndPolygonize(void);

	/** @brief  将一系列几何对象栅格化
	 *  @return 操作是否成功
	 *  @note   输入的几何对象必须与栅格对象有相同的坐标体系
	 *          几何对象的范围不能超出整个影像的范围
	 *          目前只支持单波段的操作
	 *          输入影像必须以更新打开
	 */
	static bool executeRasterize(
		GTRasterDataset* poDataset,        /*必须以更新方式打开*/
		std::vector<GTGeometry*>& geomList,/*几何对象列表*/
		int geomCount,                     /*几何对象个数*/
		double dBurnValue );               /*转换后的数值*/

	/** @brief  将栅格影像转成多边形数据
	 *  @return 操作是否成功
	 *  @note   
	 */
	static bool executePolygonize(
		const char* pszSourceFrom,  /*数据源路径*/
		int iBandNum,               /*数据源的波段*/
		const char* pszSourceTo,    /*目标Shape文件路径*/
		const char* pszLayerName,   /*层的名字*/
		bool bFourConnected = true, /*联通算法设置, 默认4方向*/
		bool bUsedMaskBand = false);/*是否使用mask波段*/
	
	/** @brief  由散点插值为栅格数据
	 *  @return 操作是否成功
	 *  @note   pData需要预先分配好
	 */
	static bool executeGridCreate(
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

	/** @brief  将无值区域进行插值填充
	 *  @return 操作是否成功
	 *  @note   输入影像必须有mask波段, 0为Nodata标志
	 */
	static bool executeInterpolateNodata(
		GTRasterDataset* poDataset,
		int              iBand, 
		double           dfMaxSearchDist,          /*最大像素搜索距离*/
		int              nSmoothingIterations = 1);/*3*3平滑窗口迭代次数(>=0)*/

	
	/** @brief  移除小块斑点像素团, 将其合并到邻近的像素团
	 *  @return 操作是否成功
	 *  @note   输入影像必须有mask波段, 0为Nodata标志
	 */
	static bool executeSieveFilter(
		GTRasterDataset* poDataset,
		int              iSrcBand,
		int              iDstBand,
		int  	         nSizeThreshold, /*像素大小阈值*/
		int  	         nConnectedness);/*4或8邻居*/

	/** @brief  DEM生成等高线
	 *  @return 操作是否成功
	 *  @note   
	 */
	static bool executeContourGenerate(
		GTRasterDataset* poDataset,
		int              iBand, 
		double  	     dfContourInterval,/*高程间隔*/
		double  	     dfContourBase,    /*基础高程值*/
		bool  	         bUseNoData,
		double  	     dfNoDataValue,
		const char*      pszSourceTo,      /*目标Shape文件路径*/
		const char*      pszLayerName);    /*层的名字*/
};

/*
 *栅格切割与拼接类
 */
class GTS_DLL GTRasterClipAndMosaic: public GTWarp
{
public:
	GTRasterClipAndMosaic(void);
	~GTRasterClipAndMosaic(void);

	/** @brief  从一幅指定的遥感影像中切割指定矩形的数据
	 *  @return 操作是否成功
	 *  @note   矩形范围必须与原始数据具有相同坐标系统, 如果
	 *          输入的影像没有坐标参数, 则无法完成. 输出的数据类型等
	 *          都与原始影像相同
	 *          对于斜轴影像, 该函数不适用
	 */
	static bool rasterClipByEnvelop(
		const char* pszFormatTo,    /*目标数据源格式*/
		const char* pszSourceFrom,  /*数据源路径*/
		const char* pszSourceTo,    /*目标数据源路径*/
		GTEnvelop& clipEnv,         /*指定范围, 返回时可能被调整到合适位置*/
		double dCacheLimit = 200.0);  /*内存使用最大上限单位为MB*/

	/** @brief  从一幅指定的遥感影像中切割指定范围的数据
	 *  @return 操作是否成功
	 *  @note   矩形范围必须与原始数据具有相同坐标系统, 如果
	 *          输入的影像没有坐标参数, 则无法完成. 输出的数据类型等
	 *          都与原始影像相同
	 *          对于斜轴影像, 该函数也适用
	 */
	static bool rasterClipByExtent(
		const char* pszFormatTo,    /*目标数据源格式*/
		const char* pszSourceFrom,  /*数据源路径*/
		const char* pszSourceTo,    /*目标数据源路径*/
		GTRawPoint& pt_leftTop, 
		GTRawPoint& pt_rightBottom, /*指定范围, 返回时可能被调整到合适位置*/
		double dCacheLimit = 200.0);/*内存使用最大上限单位为MB*/


	/** @brief  从一幅指定的遥感影像中切割指定像素大小的数据
	 *  @return 操作是否成功
	 *  @note   输出的数据类型等
	 *          都与原始影像相同
	 *          对于斜轴影像, 该函数也适用
	 */
	static bool rasterClipByPixels(
		const char* pszFormatTo,    /*目标数据源格式*/
		const char* pszSourceFrom,  /*数据源路径*/
		const char* pszSourceTo,    /*目标数据源路径*/
		int& xOffset,               /*x偏移, 可能被调整*/
		int& yOffset,               /*y偏移, 可能被调整*/
		int& xSize,                 /*x大小, 可能被调整*/
		int& ySize,                 /*y大小, 可能被调整*/
		double dCacheLimit = 200.0);/*内存使用最大上限单位为MB*/

	/** @brief  按照空间位置合并两幅影像
	 *  @return 操作是否成功
	 *  @note   默认数据类类型和空间坐标为第一幅
	 *          斜轴影像此函数不支持
     */
	static bool rasterMosaicBySpatial(
		const char*    pszSourceFrom1,
        const double*  d_nodata_list1,
		const char*    pszSourceFrom2,
		const double*  d_nodata_list2,
        const char*    pszSourceTo,
		const char*    pszFormatTo,
        const double   xResol      = 0.0,
        const double   yResol      = 0.0,
		enumGTDataType eType       = GTD_Unknown, 
		const char*    pszDstSRS   = NULL,
		double         dCacheLimit = 200.0);

private:
	/*Envelop是否相交, 如果相交, 结果在envr中*/
	static bool twoEnvelopsOverlap(
		const GTEnvelop& env1,
		const GTEnvelop& env2,
		GTEnvelop&       envr);

	/*返回水平线与垂直线的是否相交, 交点在xi,yi*/
	static bool isSegmentsIntersect(
		const double& x1, const double& y1,  /*水平线段左端点*/
		const double& x2, const double& y2,  /*水平线段右端点*/
		const double& x3, const double& y3,  /*垂直线段下端点*/
		const double& x4, const double& y4,  /*垂直线段上端点*/
		double&       xi, double&       yi); /*交点数值*/

    static void replaceNoData(
        void        *pData,
        void        *pDataRef,
        int          i_size,
        double       dNodata,
        GDALDataType dType);
};

/*
 *用于矢量坐标转换的类
 */
class GTS_DLL GTGeometryReprojector: public GTWarp
{
public:
	/*构造初始化, 如果两个坐标系统相同, 则不能构建*/
	GTGeometryReprojector(
		GTSpatialReference* fromSRS, 
		GTSpatialReference* toSRS);

	~GTGeometryReprojector();

	/*静态销毁函数, 对象必须new来分配*/
	static void destroyGeometryReprojector(GTGeometryReprojector *poGeomReprojector);

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

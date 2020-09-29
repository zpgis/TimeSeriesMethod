/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  基础DEM分析处理类 (部分算法参考GRASS和GDAL)
* Author:   范俊甫, fanjf@lreis.ac.cn, 2010年-12月-08日
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* 版权所有   (c) 中国科学院地理科学与资源研究所. 保留所有权利
****************************************************************/
#ifndef GTERRAINANALYSIS_H_INCLUDED
#define GTERRAINANALYSIS_H_INCLUDED

#include "gts_dta_port.h"
#include "gt_solpos.h"
#include "ogr_spatialref.h"
#include "gdalwarper.h"
#include "cpl_string.h"

#define EQUAL_TO_NODATA(a,b) ARE_REAL_EQUAL(a, b)
#define INTERPOL(a,b) ((bSrcHasNoData && (EQUAL_TO_NODATA(a, fSrcNoDataValue) || EQUAL_TO_NODATA(b, fSrcNoDataValue))) ? fSrcNoDataValue : 2 * (a) - (b))

namespace gts{

class GTS_DLL GTTerrainAnalysis
{
public:
	GTTerrainAnalysis(void);
	~GTTerrainAnalysis(void);

	/*输入年、天(1-365)、时间(0-24)、 时区和地点
	 *计算太阳高度角和方位角(0-90, 0-360, 为角度)
	 */
	static bool getSunPosition(
		int       year,   /*年*/
		int       day,    /*日*/
		GTFDTime& sTime,  /*时间与时区*/
		double    sLat,   /*纬度*/
		double    sLon,   /*经度*/
		double&   sAzimA, /*返回: 方位角*/
		double&   sElevA);/*返回: 高度角*/

	/** @brief  由栅格生成等值线图(ESRI ShapeFile)
	 *  @return 操作是否成功
	 *  @note   如dfContourBase=10, dfContourInterval=5,
	 *          将产生10, 15, 20, 25...间隔的等值线
	 *          输出几何类型为LineString
	 *          fieldName指定了属性列的名字
	 *          输出的空间参考将与输入的栅格数据一致
	 */
	static bool generateContour(
		const char* imageSource,      /*输入栅格数据路径, 如有中文字符请转成utf8编码*/
		int         iBand,            /*目标波段*/
		double  	dfContourInterval,/*等值线间隔*/
		double  	dfContourBase,    /*基础等值线值*/
		bool  	    bUseNoData,       /*是否忽略NoData*/
		double  	dfNoDataValue,    /*NoData的数值*/
		const char* targetSource,     /*目标Shape文件路径*/
		const char* layerName,        /*目标层的名字*/
		const char* fieldName         /*存放属性值的列, 类型为64位双精度*/  
		);

	/*计算阴影
	 *在计算中z与scale为比值关系, 因此通常将z设为0
	 *而通过调整scale来获得合适显示比例
	 *scale应该是根据水平单位(与坐标系统有关)和高度单位（通常为m）的比例来决定
	 */
	static bool generateHillShade(
		const char* pszSourceFrom,         /*数据源路径*/
		int         iBand,                 /*波段*/
		const char* pszSourceTo,           /*目标数据源路径*/
		const char* pszFormatTo = "GTiff", /*目标数据源格式*/
		double      az     = 315.0,        /*太阳方位角 degree*/
		double      alt    = 45.0,         /*太阳高度角 degree*/
		double      z      = 1.0,          /*z值的比例因子*/
	    double      scale  = 50000.0,      /*水平与垂直的尺度因子*/
		bool        bEdges = false,        /*是否计算边缘*/
	    bool        bZevenbergenThorne = true /*false为Horn算法(适合粗地形)*/
		);
	
	/*计算Color relief map
	 *输入颜色表, nodata也需要指定颜色值
	 *输入颜色表将被排序(qsort)
	 */
	static bool generateColorRelief(
		const char*             pszSourceFrom,                                    /*数据源路径*/
		int                     iBand,                                            /*波段*/
		const char*             pszSourceTo,                                      /*目标数据源路径*/
		GTDEMColorEntry*        pasColorAssociation,                              /*颜色表*/
		int                     nColorAssociation,                                /*颜色数目*/
		GTDEMColorEntry         noDataColor,                                      /*背景色*/
		const char*             pszFormatTo         = "GTiff",                    /*目标数据源格式*/
		bool                    bAddAlpha           = false,                      /*alpha通道*/
		GTDEMColorSelectionMode eColorSelectionMode = COLOR_SELECTION_INTERPOLATE /*颜色选取方法*/
		);

	static bool generateColorRelief2(
		const char*             pszSourceFrom,                                    /*数据源路径*/
		int                     iBand,                                            /*波段*/
		const char*             pszSourceTo,                                      /*目标数据源路径*/
		double					hsv_s[3],                              /*颜色表*/
		double					hsv_e[3],                                /*颜色数目*/
		int						rgb_bg[3],                                      /*背景色*/
		const char*             pszFormatTo         = "GTiff",                    /*目标数据源格式*/
		bool                    bAddAlpha           = false,                      /*alpha通道*/
		GTDEMColorSelectionMode eColorSelectionMode = COLOR_SELECTION_MIN_MAX /*颜色选取方法*/
		);

	/*计算坡度
	 *scale为水平和垂直的比例因子
	 *应该是根据水平单位(与坐标系统有关)和高度单位（通常为m）的比例(在同一单位下)来决定
	 *否则得不到正确的值.
	 */
	static bool generateSlope(
		const char*   pszSourceFrom,               /*数据源路径*/
		int           iBand,                       /*波段*/
		const char*   pszSourceTo,                 /*目标数据源路径*/
		const char*   pszFormatTo        = "GTiff",/*目标数据源格式*/
		int           iFormat            = 1,      /*1为角度, 0为百分比*/
		double        scale              = 1.0,    /*水平与垂直的尺度因子*/
		bool          bEdges             = false,  /*是否计算边缘*/
		bool          bZevenbergenThorne = true    /*false为Horn算法(适合粗地形)*/
		);

	/*
	 *计算坡向, 局部水平则为NoData背景值
	 */
	static bool generateAspect(
		const char*   pszSourceFrom,               /*数据源路径*/
		int           iBand,                       /*波段*/
		const char*   pszSourceTo,                 /*目标数据源路径*/
		const char*   pszFormatTo        = "GTiff",/*目标数据源格式*/
		int           iFormat            = 1,      /*1正北起算, 0正东起算*/
		bool          bEdges             = false,  /*是否计算边缘*/
		bool          bZevenbergenThorne = true    /*false为Horn算法(适合粗地形)*/
		);

	static bool generateAspectDEM(
		GTFeatureClass& inFeatures, 
		int column, 
		int row, 
		int maxMemorySize,
		const char* pszSourceTo,           /*目标数据源路径*/
		const char* pszFormatTo = "GTiff" /*目标数据源格式*/
		);

	static bool generateSurfaceCurvature(
		const char*			pszSourceFrom,               /*数据源路径*/
		int					iBand,                       /*波段*/
		const char*			pszSourceTo,                 /*目标数据源路径*/
		const char*			pszFormatTo			= "GTiff",/*目标数据源格式*/
		double				scale				= 1.0,    /*水平与垂直的尺度因子*/
		bool				bEdges				= false,  /*是否计算边缘*/
		GTDEMCurvatureType	CurvatureType		= CURVATURE_NORMAL	   /*曲率算法的编号*/
		);

	/*****************************************************************************
	* @brief	计算挖土量（最终结果须乘以单个像元面积才是土方量）
	* @param	[in]
	*			pFile		DEM数据
	*			iBand		波段号
	*			colsMin		区域最小X（列号）
	*			rowsMin		区域最小Y（行号）
	*			colsMax		区域最大X（列号）
	*			rowsMax		区域最大Y（行号）
	*			elevation	目标高程值
	* @param	[out]
	* @return	
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-19
	*******************************************************************************/
	static double dig(
		const char *pFile,int iBand,int colsMin,int rowsMin,int colsMax,int rowsMax,double elevation);
	/*****************************************************************************
	* @brief	计算填土量（最终结果须乘以单个像元面积才是土方量）
	* @param	[in]
	*			pFile		DEM数据
	*			iBand		波段号
	*			colsMin		区域最小X（列号）
	*			rowsMin		区域最小Y（行号）
	*			colsMax		区域最大X（列号）
	*			rowsMax		区域最大Y（行号）
	*			elevation	目标高程值
	* @param	[out]
	* @return	
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-19
	*******************************************************************************/
	static double fill(
		const char *pFile,int iBand,int colsMin,int rowsMin,int colsMax,int rowsMax,double elevation);
private:
	typedef float (*__generic3x3ProcessingAlg) (float* pafWindow, float fDstNoDataValue, void* pData);
	
	static float __computeVal(
		int                       bSrcHasNoData, 
		float                     fSrcNoDataValue,
		float*                    afWin, 
		float                     fDstNoDataValue,
		__generic3x3ProcessingAlg pfnAlg,
		void*                     pData,
		int                       bComputeAtEdges);

	static bool __generic3x3Processing(
		GDALRasterBandH           hSrcBand,
		GDALRasterBandH           hDstBand,
		__generic3x3ProcessingAlg pfnAlg,
		void*                     pData,
		int                       bComputeAtEdges);

	/*计算 shade普通算法*/
	static float __hillshadeAlg(
		float* afWin, /*8邻居窗口*/
		float  fDstNoDataValue, 
		void*  pData);

	/*计算 shadeZevenbergen-Thorne算法*/
	static float __hillshadeZevenbergenThorneAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*shade参数的初始化*/
	static void* __createHillshadeData(
		double* adfGeoTransform,
		double  z,
		double  scale,
		double  alt,
		double  az,
		int     bZevenbergenThorne);

	/*以下为计算color relief的子函数*/
	/*按照高程的比较函数*/
	static int __colorReliefSortColors(
		const void* pA, 
		const void* pB);

	/*由颜色表计算目标颜色值*/
	static int __colorReliefGetRGBA (
		GTDEMColorEntry*        pasColorAssociation,
		int                     nColorAssociation,
		double                  dfVal,
		GTDEMColorSelectionMode eColorSelectionMode,
		int*                    pnR,
		int*                    pnG,
		int*                    pnB,
		int*                    pnA);

	static int hsv2rgb(const double hsv[3], int (&rgb)[3]);

	static int __colorReliefGetRGBA2 (
		GDALRasterBandH         hSrcBand,
		double					hsv_s[3],                         
		double					hsv_e[3],
		double                  dfVal,
		GTDEMColorSelectionMode eColorSelectionMode,
		int*                    pnR,
		int*                    pnG,
		int*                    pnB,
		int*                    pnA);
	
	/*根据颜色表产生Color Relief*/
	static bool __colorRelief (
		GDALRasterBandH         hSrcBand,
		GDALRasterBandH         hDstBand1,
		GDALRasterBandH         hDstBand2,
		GDALRasterBandH         hDstBand3,
		GDALRasterBandH         hDstBand4,
		GTDEMColorSelectionMode eColorSelectionMode,
		GTDEMColorEntry*        pasColorAssociation,
		int                     nColorAssociation,
		GTDEMColorEntry         noDataColor);

	static bool __colorRelief2 (
		GDALRasterBandH         hSrcBand,
		GDALRasterBandH         hDstBand1,
		GDALRasterBandH         hDstBand2,
		GDALRasterBandH         hDstBand3,
		GDALRasterBandH         hDstBand4,
		GTDEMColorSelectionMode eColorSelectionMode,
		double					hsv_s[3],                         
		double					hsv_e[3],
		int						rgb_bg[3]);

	/*坡度的Horn算法*/
	static float __slopeHornAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*坡度的Zevenbergen-Thorne算法*/
	static float __slopeZevenbergenThorneAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*坡度参数的初始化*/
	static void* __createSlopeData(
		double* adfGeoTransform,
		double  scale,
		int     slopeFormat);

	/*坡向的Horn算法*/
	static float __aspectAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*坡向的Zevenbergen-Thorne算法*/
	static float __aspectZevenbergenThorneAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*坡向参数初始化*/
	static void* __createAspectData(
		int bAngleAsAzimuth);

	/*地表粗糙度指数计算的普通算法*/
	static float __TRIAlg (
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*地表粗糙度指数计算的DOCELL算法*/
	static float __TRIDOCELLAlg (
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*产生一般参数的函数*/
	static void* __createGeneralData(
		double* adfGeoTransform,
		double  scale);

	/***************************************
	 * 以下为不同曲率的计算方法, 参考Shary (2002)
	 * 所使用到的共同参数为
	 ***************************************/
	
	/*slope steepness 3*3窗口算法*/
	static float __slopeSteepnessAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*slope gradient 3*3窗口算法*/
	static float __slopeGradientAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*gradient factor3*3窗口算法*/
	static float __gradientFactorAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*mean curvature (单位1/m) 3*3窗口算法*/
	static float __meanCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*difference curvature (单位1/m) 3*3窗口算法*/
	static float __differenceCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*horizontal curvature (单位1/m) 3*3窗口算法*/
	static float __horizontalCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*plan curvature (单位1/m) 3*3窗口算法
	 *与最大坡度方向正交的曲率
	 */
	static float __planCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*profile curvature (单位1/m) 3*3窗口算法
	 *沿着最大坡度方向的曲率
	 */
	static float __profileCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*normal curvature (单位1/m) 3*3窗口算法
	 *=(profile - plan)
	 *=-2.0*(D+E)
	 *D=((e3+e5)/2-e4)/L^2
	 *E=((e1+e7)/2-e4)/L^2
          0 1 2
          3 4 5
          6 7 8
	 */
	static float __normalCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __rotorAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __unsphericityAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __horizontalExcessCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __verticalExcessCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __minimalCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __maximalCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __totalGaussianCurvature(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __totalRingCurvature(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __totalAccumulationAurvature(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __slopeDirection(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

};

}
#endif

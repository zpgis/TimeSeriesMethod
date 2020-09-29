/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  几何对象最小边界
* Author:   范俊甫, fanjf@lreis.ac.cn,2011-09-22
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c)  保留所有权利
****************************************************************/
#ifndef GT_GEOMETRYMINIMUMBOUNDING_H_INCLUDE
#define	GT_GEOMETRYMINIMUMBOUNDING_H_INCLUDE

#include "gts_cg_port.h"

namespace gts{

	class GTS_DLL GTGeometryMinimumBounding
	{
	public:
		GTGeometryMinimumBounding(void);
		~GTGeometryMinimumBounding(void);

		/*****************************************************************************
		* @brief	点集的凸包
		* @param	[in]
		*			pointList			点集
		*			pointNum			点的个数	
		*			[out]
		*			pHullRing			凸包环对象
		* @return	建立凸包对象是否成功
		* @note		返回的凸包环对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsContexHull(
			std::vector<GTRawPoint>&	pointList, 
			int							pointNum, 
			GTRing*						pHullRing);

		/*****************************************************************************
		* @brief	线的凸包
		* @param	[in]
		*			lineStrObj			线对象
		*			[out]
		*			pHullRing			凸包环对象
		* @return	建立凸包对象是否成功
		* @note		返回的凸包环对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringContexHull(
			GTLineString&	lineStrObj,
			GTRing*			pHullRing);

		/*****************************************************************************
		* @brief	多线的凸包
		* @param	[in]
		*			mutiLineStrObj		多线对象
		*			[out]
		*			pHullRing			凸包环对象
		* @return	建立凸包对象是否成功
		* @note		返回的凸包环对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringContexHull(
			GTMultiLineString&	mutiLineStrObj,
			GTRing*				pHullRing);

		/*****************************************************************************
		* @brief	环的凸包
		* @param	[in]
		*			ringObj				环对象
		*			[out]
		*			pHullRing			凸包环对象
		* @return	建立凸包对象是否成功
		* @note		返回的凸包环对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringContexHull(
			GTRing& ringObj,
			GTRing* pHullRing);

		/*****************************************************************************
		* @brief	多边形的凸包
		* @param	[in]
		*			polyObj				多边形对象
		*			[out]
		*			pHullRing			凸包环对象
		* @return	建立凸包对象是否成功
		* @note		返回的凸包环对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonContexHull(
			GTPolygon&	polyObj,
			GTRing*		pHullRing);

		/*****************************************************************************
		* @brief	多多边形的凸包
		* @param	[in]
		*			multiPolyObj		多多边形对象
		*			[out]
		*			pHullRing			凸包环对象
		* @return	建立凸包对象是否成功
		* @note		返回的凸包环对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonContexHull(
			GTMultiPolygon& multiPolyObj,
			GTRing*			pHullRing);

		static bool pointsContexHull(std::vector<GTPoint *>& pointList,
			int pointNum, GTRing* pHullRing);

		static bool multiPointContexHull(GTMultiPoint& multiPointObj,
			GTRing* pHullRing);

		static bool geometryContexHull(GTGeometry* geom, GTRing* pHullRing);

		static bool featureClassContexHull(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);

		/*****************************************************************************
		* @brief	点集的外包矩形
		* @param	[in]
		*			pointList			点集
		*			pointNum			点个数
		*			[out]
		*			pEnvelop			外包矩形对象
		* @return	建立外包矩形对象是否成功
		* @note		返回的外包矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsEnvelop(
			std::vector<GTRawPoint *>&	pointList, 
			int							pointNum, 
			GTEnvelop*					pEnvelop);

		/*****************************************************************************
		* @brief	线的外包矩形
		* @param	[in]
		*			lineStrObj			线对象
		*			[out]
		*			pEnvelop			外包矩形对象
		* @return	建立外包矩形对象是否成功
		* @note		返回的外包矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringEnvelop(
			GTLineString&	lineStrObj,
			GTEnvelop*		pEnvelop);

		/*****************************************************************************
		* @brief	多线的外包矩形
		* @param	[in]
		*			multiLineStrObj		多线对象
		*			[out]
		*			pEnvelop			外包矩形对象
		* @return	建立外包矩形对象是否成功
		* @note		返回的外包矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringEnvelop(
			GTMultiLineString&	multiLineStrObj,
			GTEnvelop*			pEnvelop);

		/*****************************************************************************
		* @brief	环的外包矩形
		* @param	[in]
		*			ringObj				环对象
		*			[out]
		*			pEnvelop			外包矩形对象
		* @return	建立外包矩形对象是否成功
		* @note		返回的外包矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringEnvelop(
			GTRing&		ringObj,
			GTEnvelop*	pEnvelop);

		/*****************************************************************************
		* @brief	多边形的外包矩形
		* @param	[in]
		*			polyObj				多边形对象
		*			[out]
		*			pEnvelop			外包矩形对象
		* @return	建立外包矩形对象是否成功
		* @note		返回的外包矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonEnvelop(
			GTPolygon& polyObj,
			GTEnvelop* pEnvelop);

		/*****************************************************************************
		* @brief	多多边形的外包矩形
		* @param	[in]
		*			polyObj				多边形对象
		*			[out]
		*			pEnvelop			外包矩形对象
		* @return	建立外包矩形对象是否成功
		* @note		返回的外包矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonEnvelop(
			GTMultiPolygon& multiPolyObj,
			GTEnvelop*		pEnvelop);

		static bool pointsEnvelop(std::vector<GTPoint *>& pointList, int pointNum,
			GTEnvelop* pEnvelop);

		static bool multiPointEnvelop(GTMultiPoint& multiPointObj,
			GTEnvelop* pEnvelop);

		static bool geometryEnvelop(GTGeometry* geom, GTEnvelop* pEnvelop);

		static bool featureClassEnvelop(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);

		/*****************************************************************************
		* @brief	点集的快速包围圆
		* @param	[in]
		*			pointList				点集
		*			pointNum				点的数量
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pBoundingBall			生成包围圆的对象
		* @return	建立快速包围圆对象是否成功
		* @note		返回的快速包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsFastBoundingBall(
			std::vector<GTRawPoint *>&	pointList, 
			int							pointNum, 
			GTRawPoint&					center,
			double*						rad,
			GTRing*						pBoundingBall = NULL);

		/*****************************************************************************
		* @brief	线的快速包围圆
		* @param	[in]
		*			lineStrObj				线对象
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pBoundingBall			生成包围圆的对象
		* @return	建立快速包围圆对象是否成功
		* @note		返回的快速包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringFastBoundingBall(
			GTLineString&	lineStrObj,
			GTRawPoint&		center,
			double*			rad,
			GTRing*			pBoundingBall = NULL);

		/*****************************************************************************
		* @brief	多线的快速包围圆
		* @param	[in]
		*			multilineStrObj			多线对象
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pBoundingBall			生成包围圆的对象
		* @return	建立快速包围圆对象是否成功
		* @note		返回的快速包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringFastBoundingBall(
			GTMultiLineString&	multiLineStrObj,
			GTRawPoint&			center,
			double*				rad,
			GTRing*				pBoundingBall = NULL);

		/*****************************************************************************
		* @brief	环快速包围圆
		* @param	[in]
		*			ringObj					环对象
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pBoundingBall			生成包围圆的对象
		* @return	建立快速包围圆对象是否成功
		* @note		返回的快速包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringFastBoundingBall(
			GTRing&		ringObj,
			GTRawPoint& center,
			double*		rad,
			GTRing*		pBoundingBall = NULL);

		/*****************************************************************************
		* @brief	多边形快速包围圆
		* @param	[in]
		*			polyObj					多边形对象
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pBoundingBall			生成包围圆的对象
		* @return	建立快速包围圆对象是否成功
		* @note		返回的快速包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonFastBoundingBall(
			GTPolygon&	polyObj,
			GTRawPoint& center,
			double*		rad,
			GTRing*		pBoundingBall = NULL);

		/*****************************************************************************
		* @brief	多多边形快速包围圆
		* @param	[in]
		*			multiPolyObj			多多边形对象
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pBoundingBall			生成包围圆的对象
		* @return	建立快速包围圆对象是否成功
		* @note		返回的快速包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonFastBoundingBall(
			GTMultiPolygon&	multiPolyObj,
			GTRawPoint&		center,
			double*			rad,
			GTRing*			pBoundingBall = NULL);

		static bool pointsFastBoundingBall(std::vector<GTPoint *>& pointList,
			int pointNum, GTRawPoint& center, double* rad,
			GTRing* pBoundingBall = NULL);

		static bool multiPointFastBoundingBall(GTMultiPoint& multiPointObj,
			GTRawPoint& center, double* rad, GTRing* pBoundingBall = NULL);

		static bool geometryFastBoundingBall(GTGeometry* geom, GTRawPoint& center,
			double* rad, GTRing* pBoundingBall = NULL);

		static bool featureClassFastBoundingBall(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);

		/*****************************************************************************
		* @brief	点集的最小包围圆
		* @param	[in]
		*			pointList				点集
		*			pointNum				点的个数
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pMinCircle				生成包围圆的对象
		* @return	建立最小包围圆对象是否成功
		* @note		返回的最小包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsMinimumCircle(
			std::vector<GTRawPoint *>&	pointList, 
			int							pointNum, 
			GTRawPoint&					center,
			double*						rad,
			GTRing*						pMinCircle = NULL);

		/*****************************************************************************
		* @brief	线的最小包围圆
		* @param	[in]
		*			lineStrObj				线对象
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pMinCircle				生成包围圆的对象
		* @return	建立最小包围圆对象是否成功
		* @note		返回的最小包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringMinimumCircle(
			GTLineString&	lineStrObj,
			GTRawPoint&		center,
			double*			rad,
			GTRing*			pMinCircle = NULL);

		/*****************************************************************************
		* @brief	多线的最小包围圆
		* @param	[in]
		*			multiLineStrObj			多线对象
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pMinCircle				生成包围圆的对象
		* @return	建立最小包围圆对象是否成功
		* @note		返回的最小包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringMinimumCircle(
			GTMultiLineString&	multiLineStrObj,
			GTRawPoint&			center,
			double*				rad,
			GTRing*				pMinCircle = NULL);

		/*****************************************************************************
		* @brief	环的最小包围圆
		* @param	[in]
		*			ringObj					环对象
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pMinCircle				生成包围圆的对象
		* @return	建立最小包围圆对象是否成功
		* @note		返回的最小包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringMinimumCircle(
			GTRing&		ringObj,
			GTRawPoint& center,
			double*		rad,
			GTRing*		pMinCircle = NULL);

		/*****************************************************************************
		* @brief	多边形的最小包围圆
		* @param	[in]
		*			polyObj					多边形对象
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pMinCircle				生成包围圆的对象
		* @return	建立最小包围圆对象是否成功
		* @note		返回的最小包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonMinimumCircle(
			GTPolygon&	polyObj,
			GTRawPoint& center,
			double*		rad,
			GTRing*		pMinCircle = NULL);

		/*****************************************************************************
		* @brief	多多边形的最小包围圆
		* @param	[in]
		*			polyObj					多边形对象
		*			[out]
		*			center					圆心
		*			rad						半径
		*			pMinCircle				生成包围圆的对象
		* @return	建立最小包围圆对象是否成功
		* @note		返回的最小包围圆对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonMinimumCircle(
			GTMultiPolygon&	multiPolyObj,
			GTRawPoint&		center,
			double*			rad,
			GTRing*			pMinCircle = NULL);

		static bool pointsMinimumCircle(std::vector<GTPoint *>& pointList,
			int pointNum, GTRawPoint& center, double* rad,
			GTRing* pMinCircle = NULL);

		static bool multiPointMinimumCircle(GTMultiPoint& multiPointObj,
			GTRawPoint& center, double* rad, GTRing* pMinCircle = NULL);

		static bool geometryMinimumCircle(GTGeometry* geom, GTRawPoint& center,
			double* rad, GTRing* pMinCircle = NULL);

		static bool featureClassMinimumCircle(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);

		/*****************************************************************************
		* @brief	点集的最小面积矩形
		* @param	[in]
		*			pointList				点集
		*			pointNum				点的数量
		*			[out]
		*			pMinAreaRect			最小面积矩形
		* @return	建立最小面积矩形对象是否成功
		* @note		返回的最小面积矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsMinimumAreaRect(
			std::vector<GTRawPoint>&	pointList, 
			int							pointNum, 
			GTRing*						pMinAreaRect);

		/*****************************************************************************
		* @brief	线的最小面积矩形
		* @param	[in]
		*			lineStrObj				线对象
		*			[out]
		*			pMinAreaRect			最小面积矩形
		* @return	建立最小面积矩形对象是否成功
		* @note		返回的最小面积矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringMinimumAreaRect(
			GTLineString&	lineStrObj,
			GTRing*			pMinAreaRect);

		/*****************************************************************************
		* @brief	线的最小面积矩形
		* @param	[in]
		*			lineStrObj				线对象
		*			[out]
		*			pMinAreaRect			最小面积矩形
		* @return	建立最小面积矩形对象是否成功
		* @note		返回的最小面积矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringMinimumAreaRect(
			GTMultiLineString&	multiLineStrObj,
			GTRing*				pMinAreaRect);

		/*****************************************************************************
		* @brief	环的最小面积矩形
		* @param	[in]
		*			ringObj					环对象
		*			[out]
		*			pMinAreaRect			最小面积矩形
		* @return	建立最小面积矩形对象是否成功
		* @note		返回的最小面积矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringMinimumAreaRect(
			GTRing& ringObj,
			GTRing* pMinAreaRect);

		/*****************************************************************************
		* @brief	多边形的最小面积矩形
		* @param	[in]
		*			polyObj					多边形对象
		*			[out]
		*			pMinAreaRect			最小面积矩形
		* @return	建立最小面积矩形对象是否成功
		* @note		返回的最小面积矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonMinimumAreaRect(
			GTPolygon&	polyObj,
			GTRing*		pMinAreaRect);

		/*****************************************************************************
		* @brief	多多边形的最小面积矩形
		* @param	[in]
		*			multiPolyObj			多多边形对象
		*			[out]
		*			pMinAreaRect			最小面积矩形
		* @return	建立最小面积矩形对象是否成功
		* @note		返回的最小面积矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonMinimumAreaRect(
			GTMultiPolygon&	multiPolyObj,
			GTRing*			pMinAreaRect);

		static bool pointsMinimumAreaRect(std::vector<GTPoint *>& pointList,
			int							pointNum, 
			GTRing*						pMinAreaRect);

		static bool multiPointMinimumAreaRect(GTMultiPoint& multiPointObj,
			GTRing*			pMinAreaRect);

		static bool geometryMinimumAreaRect(GTGeometry* geom,
			GTRing*			pMinAreaRect);

		static bool featureClassMinimumAreaRect(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);


		/*****************************************************************************
		* @brief	点集的最小宽度矩形
		* @param	[in]
		*			pointList				点集
		*			pointNum				点的数量
		*			[out]
		*			pMinWidthRect			最小宽度矩形
		* @return	建立最小宽度矩形对象是否成功
		* @note		返回的最小宽度矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsMinimumWidthRect(
			std::vector<GTRawPoint>&	pointList, 
			int							pointNum, 
			GTRing*						pMinWidthRect);

		/*****************************************************************************
		* @brief	线的最小宽度矩形
		* @param	[in]
		*			lineStrObj				线对象
		*			[out]
		*			pMinWidthRect			最小宽度矩形
		* @return	建立最小宽度矩形对象是否成功
		* @note		返回的最小宽度矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringMinimumWidthRect(
			GTLineString&	lineStrObj,
			GTRing*			pMinWidthRect);

		/*****************************************************************************
		* @brief	多线的最小宽度矩形
		* @param	[in]
		*			multiLineStrObj			多线对象
		*			[out]
		*			pMinWidthRect			最小宽度矩形
		* @return	建立最小宽度矩形对象是否成功
		* @note		返回的最小宽度矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringMinimumWidthRect(
			GTMultiLineString&	multiLineStrObj,
			GTRing*				pMinWidthRect);

		/*****************************************************************************
		* @brief	环的最小宽度矩形
		* @param	[in]
		*			ringObj					环对象
		*			[out]
		*			pMinWidthRect			最小宽度矩形
		* @return	建立最小宽度矩形对象是否成功
		* @note		返回的最小宽度矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringMinimumWidthRect(
			GTRing& ringObj,
			GTRing* pMinWidthRect);

		/*****************************************************************************
		* @brief	多边形的最小宽度矩形
		* @param	[in]
		*			polyObj					多边形对象
		*			[out]
		*			pMinWidthRect			最小宽度矩形
		* @return	建立最小宽度矩形对象是否成功
		* @note		返回的最小宽度矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonMinimumWidthRect(
			GTPolygon&	polyObj,
			GTRing*		pMinWidthRect);

		/*****************************************************************************
		* @brief	多多边形的最小宽度矩形
		* @param	[in]
		*			multiPolyObj			多多边形对象
		*			[out]
		*			pMinWidthRect			最小宽度矩形
		* @return	建立最小宽度矩形对象是否成功
		* @note		返回的最小宽度矩形对象需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonMinimumWidthRect(
			GTMultiPolygon&	multiPolyObj,
			GTRing*			pMinWidthRect);

		static bool pointsMinimumWidthRect(std::vector<GTPoint *>& pointList,
			int							pointNum, 
			GTRing*						pMinWidthRect);

		static bool multiPointMinimumWidthRect(GTMultiPoint& multiPointObj,
			GTRing*			pMinWidthRect);

		static bool geometryMinimumWidthRect(GTGeometry* geom,
			GTRing*			pMinWidthRect);

		static bool featureClassMinimumWidthRect(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);
	};

}
#endif

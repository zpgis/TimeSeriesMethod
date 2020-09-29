/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  几何对象的近邻分析（点、线、面、点集、线集、面集）      
* Author:   范俊甫, fanjf@lreis.ac.cn, 2011年-7月-19日
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* 版权所有  (c) 中国科学院地理科学与资源研究所. 保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYNEAREST_H_INCLUDED
#define GTSGEOMETRYNEAREST_H_INCLUDED
#include "gts_cg_port.h"
#include "gt_datadriver.h"
#include "gt_datasource.h"
#include "gt_geometryanalysis.h"
#include <omp.h>
namespace gts{

class GTS_DLL GTGeometryNearest
{
public:
	GTGeometryNearest(void);
	~GTGeometryNearest(void);

	/*****************************************************************************
	* @brief	求两点之间的距离
	* @param	[in]
	*			pointObj1	              点对象
	*			pointObj2			      点对象
	* @return	成功返回两点之间的距离 ，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
static double twoPointsDistance(
	const GTRawPoint* pointObj1,                            
	const GTRawPoint* pointObj2);

   	/*****************************************************************************
	* @brief	点到直线最短最短距离
	* @param	[in]
	*			pointObj	              目标点对象
	*			fromPoint			      直线上的两点
	*			toPoint		               
	*       	[out]
	*			shortestPoint			  最后求得在线上的点
	* @return	成功返回点到直线最短最短距离 ，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToLine(
		const GTRawPoint* pointObj,
		const GTRawPoint* fromPoint,
		const GTRawPoint* toPoint,
		GTRawPoint*       shortestPoint);
	/*****************************************************************************
	* @brief	点到射线最短最短距离
	* @param	[in]
	*			pointObj	              目标点对象
	*			rayFromPoint			  射线的起始端点
	*			rayToPoint		          射线上异于起始端点任意一点
	*       	[out]
	*			shortestPoint			  最后求得在射线上的点
	* @return	成功返回点到射线最短最短距离 ，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToRayDistance(
		const GTRawPoint* pointObj,
		const GTRawPoint* rayFromPoint,
		const GTRawPoint* rayToPoint,
		GTRawPoint*  shortestPoint);

	/*****************************************************************************
	* @brief	点到线段的最短距离
	* @param	[in]
	*			pointObj	              目标点对象
	*			fromPoint			      线段的两个端点
	*			toPoint		              
	*       	[out]
	*			shortestPoint			  最后求得在线段上的点
	* @return	成功返回点到线段的最短距离 ，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToSegment(
		const GTRawPoint* pointObj,
		const GTRawPoint* fromPoint,
		const GTRawPoint* toPoint,
		GTRawPoint*       shortestPoint);
	
	/*****************************************************************************
	* @brief	点到线串的最短距离
	* @param	[in]
	*			pointObj	              目标点对象
	*			lineStringObj			  线串对象
	*       	[out]
	*			shortestPoint			  最后求得在线串上的点
	* @return	成功返回点到线串的最短距离 ，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToLineString(
		const GTRawPoint*   pointObj,
		const GTLineString* lineStringObj,
		GTRawPoint*         shortestPoint);

	/*****************************************************************************
	* @brief	点到多线串最短距离
	* @param	[in]
	*			pointObj	              目标点对象
	*			pMultilineStringObj		  多线串对象
	*           [out]
	*			shortestPoint			  最后求得在多线串上的点
	* @return	成功返回点到线串的最短距离 ，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToMultiLineString(
		    const GTRawPoint*   pointObj,
		    GTMultiLineString* pMultilineStringObj,
		    GTRawPoint*        shortestPoint);

	/*****************************************************************************
	* @brief	点到环最短距离
	* @param	[in]
	*			pointObj	              目标点对象
	*			ringObj          		  环对象
	*       	[out]
	*			shortestPoint			  最后求得在环上的点
	* @return	成功返回点到环最短距离 ，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToRing(
		const GTRawPoint* pointObj,
		GTRing*           ringObj,
		GTRawPoint*       shortestPoint);

	/*****************************************************************************
	* @brief	点到多边形最短距离
	* @param	[in]
	*			pointObj	              目标点对象
	*			polygonObj          	  多边形对象
	*       	[out]
	*			shortestPoint			  最后求得在环上的点
	* @return	成功返回点到多边形最短距离，失败返回-1.0
	* @note		只有两种情况才存在点与多边形的距离：
	*           1是点在多边形内环内 2是点在多边形外环外
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToPolygon(
		const GTRawPoint* pointObj,
		GTPolygon*        polygonObj,
		GTRawPoint*       shortestPoint);

	/*****************************************************************************
	* @brief	点到多边形最短距离
	* @param	[in]
	*			pointObj	              目标点对象
	*			polygonObj          	  多多边形对象
	*       	[out]
	*			shortestPoint			  最后求得在环上的点
	* @return	成功返回点到多边形最短距离，失败返回-1.0
	* @note		只有两种情况才存在点与多边形的距离：
	*           1是点在多边形内环内 2是点在多边形外环外
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToMultiPolygon(
		const GTRawPoint* pointObj,
		GTMultiPolygon*   polygonObj,
		GTRawPoint*       shortestPoint);

	static double shortestMultiPointToMultiPolygon(
		GTMultiPoint* pointObj,
		GTMultiPolygon*		polygonObj,
		GTRawPoint*			shortestPointF,
		GTRawPoint*			shortestPointT);

	static double shortestMultiPointToPolygon(
		GTMultiPoint* pointObj,
		GTPolygon*			polygonObj,
		GTRawPoint*			shortestPointF,
		GTRawPoint*			shortestPointT);
	
	/*****************************************************************************
	* @brief	线段到线段之间最短距离
	* @param	[in]
	*			seg1_fpt，seg1_tpt	      线段1上的始末端点
	*			seg2_fpt，seg2_tpt        线段2上的始末端点
	*       	[out]
	*			startPt，endPt		      最后求得线段的始末端点
	* @return	成功返回线段到线段之间最短距离，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static   double shortestSegmentToSegment(
		const GTRawPoint* seg1_fpt, 
		const GTRawPoint* seg1_tpt,
		const GTRawPoint* seg2_fpt, 
		const GTRawPoint* seg2_tpt,
		GTRawPoint*       startPt,
		GTRawPoint*       endPt );

	/*****************************************************************************
	* @brief	线段到环之间最短距离
	* @param	[in]
	*			seg_fpt，seg_tpt	      线段上的始末端点
	*			ringObj                   环对象
	*       	[out]
	*			startPt，endPt		      最后求得线段的始末端点
	* @return	成功返回线段到环之间最短距离，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestSegmentToRing(
		const GTRawPoint* seg_fpt, 
		const GTRawPoint* seg_tpt,
		GTRing*           ringObj,
		GTRawPoint*       startPt,
		GTRawPoint*       endPt );

	/*****************************************************************************
	* @brief	线段到多边形之间最短距离
	* @param	[in]
	*			seg_fpt，seg_tpt	      线段上的始末端点
	*			polygonObj                多边形对象
	*       	[out]
	*			startPt，endPt		      最后求得最短距离线段的始末端点
	* @return	成功返回线段到多边形之间最短距离，失败返回-1.0
	* @note		只有两种情况才存在线段与多边形的距离：
	*           1是线段在内环内 2是线段在外环外
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static  double shortestSegmentToPolygon(
		const GTRawPoint* seg_fpt, 
		const GTRawPoint* seg_tpt,
		GTPolygon*        polygonObj,
		GTRawPoint*       startPt,
		GTRawPoint*       endPt
		);

	///*****************************************************************************
	//* @brief	线串到线串之间最短距离
	//* @param	[in]
	//*			pLineString				  线串对象
	//*			pLineStringobj            多边形对象
	//*       	[out]
	//*			startPt，endPt		      最后求得最短距离线段的始末端点
	//* @return	成功返回线段到多边形之间最短距离，失败返回-1.0
	//* @note		只有两种情况才存在线段与多边形的距离：
	//*           1是线段在内环内 2是线段在外环外
	//* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	//*******************************************************************************/
	//static  double shortestLineStringToLinestring(
	//	GTLineString*		pLineString,
	//	GTLineString*		pLineStringobj,
	//	GTRawPoint*			startPt,
	//	GTRawPoint*			endPt
	//	);

	/*****************************************************************************
	* @brief	线串到多边形之间最短距离
	* @param	[in]
	*			pLineString				  线串对象
	*			polygonObj                多边形对象
	*       	[out]
	*			startPt，endPt		      最后求得最短距离线段的始末端点
	* @return	成功返回线段到多边形之间最短距离，失败返回-1.0
	* @note		只有两种情况才存在线段与多边形的距离：
	*           1是线段在内环内 2是线段在外环外
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static  double shortestLineStringToPolygon(
		const GTLineString*	pLineString,
		GTPolygon*			polygonObj,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt
		);

	/*****************************************************************************
	* @brief	多线串到多边形之间最短距离
	* @param	[in]
	*			pLineString					多线串对象
	*			polygonObj                多边形对象
	*       	[out]
	*			startPt，endPt		      最后求得最短距离线段的始末端点
	* @return	成功返回线段到多边形之间最短距离，失败返回-1.0
	* @note		只有两种情况才存在线段与多边形的距离：
	*           1是线段在内环内 2是线段在外环外
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static  double shortestMultiLineStringToPolygon(
		const GTMultiLineString*	pLineString,
		GTPolygon*					polygonObj,
		GTRawPoint*					startPt,
		GTRawPoint*					endPt
		);

	/*****************************************************************************
	* @brief	线串到多多边形之间最短距离
	* @param	[in]
	*			pLineString		  多线串对象
	*			polygonObj                多多边形对象
	*       	[out]
	*			startPt，endPt		      最后求得最短距离线段的始末端点
	* @return	成功返回线段到多边形之间最短距离，失败返回-1.0
	* @note		只有两种情况才存在线段与多边形的距离：
	*           1是线段在内环内 2是线段在外环外
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static  double shortestLineStringToMultiPolygon(
		GTLineString*		pLineString,
		GTMultiPolygon*		polygonObj,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt
		);

	/*****************************************************************************
	* @brief	多线串到多多边形之间最短距离
	* @param	[in]
	*			pLineString					多线串对象
	*			polygonObj                多多边形对象
	*       	[out]
	*			startPt，endPt		      最后求得最短距离线段的始末端点
	* @return	成功返回线段到多边形之间最短距离，失败返回-1.0
	* @note		只有两种情况才存在线段与多边形的距离：
	*           1是线段在内环内 2是线段在外环外
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static  double shortestMultiLineStringToMultiPolygon(
		GTMultiLineString*	pLineString,
		GTMultiPolygon*		polygonObj,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt
		);



	/*****************************************************************************
	* @brief	求pFPointsFeatureLayer点集中的每个点到pTPointsFeatureLayer点集中
	*           距离最短的点，再建立一个线图层保存这两点的连线
	* @param	[in]
	*			pFPointsFeatureLayer	             点数据层
	*			pTPointsFeatureLayer			     点数据层
	*       	[out]
	*			pResultLineFeatureLayer              最后生成的最短距离连线图层
	*                                                pResultLineFeatureLayer的enumGTWKBGeometryType为GT_WKBLineString
	* @return	成功返回true ，失败返回false
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool shortestPointsToPoints(
		GTFeatureLayer *pFPointsFeatureLayer,
		GTFeatureLayer *pTPointsFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);

	/*****************************************************************************
	* @brief	求点集中的每个点到线集中的最短的条线,并求出点到线上距离最短的点，
	*			再建立一个线图层保存这两点的连线
	* @param	[in]
	*			pPointsFeatureLayer	                 点数据层
	*			pLinesFeatureLayer			         线数据层
	*       	[out]
	*			pResultLineFeatureLayer              最后生成的最短距离连线图层
	*                                                pResultLineFeatureLayer的enumGTWKBGeometryType为GT_WKBLineString
	* @return	成功返回true ，失败返回false
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool shortestPointsToLines(
		GTFeatureLayer *pPointsFeatureLayer,
		GTFeatureLayer *pLinesFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);

	/*****************************************************************************
	* @brief	求点集中的每个点到环集中的最短的距离,并求出点到环上距离最短的点，
	*			再建立一个线图层保存这两点的连线
	* @param	[in]
	*			pPointsFeatureLayer	                 点数据层
	*			pRingsFeatureLayer			         环数据层
	*       	[out]
	*			pResultLineFeatureLayer              最后生成的最短距离连线图层
	*                                                pResultLineFeatureLayer的enumGTWKBGeometryType为GT_WKBLineString
	* @return	成功返回true ，失败返回false
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool  shortestPointsToRings(
				GTFeatureLayer *pPointsFeatureLayer,
				GTFeatureLayer *pRingsFeatureLayer,
				GTFeatureLayer *pResultLineFeatureLayer);
		    
	/*****************************************************************************
	* @brief	求点集中的每个点到面集中的最短的距离,并求出点到面上距离最短的点，
	*			再建立一个线图层保存这两点的连线
	* @param	[in]
	*			pPointsFeatureLayer	                 点数据层
	*			pPolygonsFeatureLayer			     面数据层
	*       	[out]
	*			pResultLineFeatureLayer              最后生成的最短距离连线图层
	*                                                pResultLineFeatureLayer的enumGTWKBGeometryType为GT_WKBLineString
	* @return	成功返回true ，失败返回false
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool  shortestPointsToPolygons(
		GTFeatureLayer *pPointsFeatureLayer,
		GTFeatureLayer *pPolygonsFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);


	/*****************************************************************************
	* @brief	求线集中的每条线到点集中的最短的距离,并求出线上的点到点集上距离最短的点，
	*			再建立一个线图层保存这两点的连线
	* @param	[in]
	*			pLinesFeatureLayer	                 线数据层
	*			pPointsFeatureLayer			         点数据层
	*       	[out]
	*			pResultLineFeatureLayer              最后生成的最短距离连线图层
	*                                                pResultLineFeatureLayer的enumGTWKBGeometryType为GT_WKBLineString
	* @return	成功返回true ，失败返回false
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool  shortestLinesToPoints(
		GTFeatureLayer *pLinesFeatureLayer,
		GTFeatureLayer *pPointsFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);

	/*****************************************************************************
	* @brief	求F线集中的每条线FL到T线集中的线TL最短的距离,并求出FL线上的点到TL线上距离最短的点，
	*			再建立一个线图层保存这两点的连线
	* @param	[in]
	*			pLinesFeatureLayer	                 线数据层
	*			pTLinesFeatureLayer			         线数据层
	* @param	[out]
	*			pResultLineFeatureLayer              最后生成的最短距离连线图层
	*                                                pResultLineFeatureLayer的enumGTWKBGeometryType为GT_WKBLineString
	* @return	成功返回true ，失败返回false
	* @note		最后建立了一个F线集里的线与T线集里的线的最短距离的线图层
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool shortestLinesToLines(
		GTFeatureLayer *pFLinesFeatureLayer,
		GTFeatureLayer *pTLinesFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);

	/*****************************************************************************
	* @brief	求线集中的每条线到环集中的最短的距离，并求出最短距离线的点到最短距离环上距离最短的点，
	*			再建立一个线图层保存这两点的连线
	* @param	[in]
	*			pLinesFeatureLayer	                 线数据层
	*			pRingsFeatureLayer			         环数据层
	* @param	[out]
	*			pResultLineFeatureLayer              最后生成的最短距离连线图层
	*                                                pResultLineFeatureLayer的enumGTWKBGeometryType为GT_WKBLineString
	* @return	成功返回true ，失败返回false
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool shortestLinesToRings(
		GTFeatureLayer *pLinesFeatureLayer,
		GTFeatureLayer *pRingsFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);
	/*****************************************************************************
	* @brief	求线集中的每条线到面集中的最短的距离，并求出最短距离线的点到最短距离面上距离最短的点，
	*			再建立一个线图层保存这两点的连线
	* @param	[in]
	*			pLinesFeatureLayer	                 线数据层
	*			pPolygonFeatureLayer			     多边形数据层
	* @param	[out]
	*			pResultLineFeatureLayer              最后生成的最短距离连线图层
	*                                                pResultLineFeatureLayer的enumGTWKBGeometryType为GT_WKBLineString
	* @return	成功返回true ，失败返回false
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool  shortestLinesToPolygons(
		GTFeatureLayer *pLinesFeatureLayer,
		GTFeatureLayer *pPolygonFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);



	/*****************************************************************************
	* @brief	点到多点的最短距离
	* @param	[in]
	*			pointObj	              目标点对象
	*			lineStringObj			  线串对象
	*       	[out]
	*			shortestPoint			  最后求得在多点上的点
	* @return	成功返回点到多点的最短距离 ，失败返回-1.0
	* @author	范俊甫，fanjf@lreis.ac.cn,2012-8-22
	*******************************************************************************/
	static double shortestPointToMultiPoint(
		const GTRawPoint*   pointObj,
		const GTMultiPoint* multiPointObj,
		GTRawPoint*         shortestPoint);

	/*****************************************************************************
	* @brief	点到多点的最短距离
	* @param	[in]
	*			multiPointObjA            目标多点对象A
	*			multiPointObjB			  目标多点对象B
	*       	[out]
	*			shortestPointA			  最后求得在多点A上的点
	*			shortestPointB			  最后求得在多点B上的点
	* @return	成功返回点到多点的最短距离 ，失败返回-1.0
	* @author	范俊甫，fanjf@lreis.ac.cn,2012-8-22
	*******************************************************************************/
	static double shortestMultiPointToMultiPoint(
		const GTMultiPoint*   multiPointObjA,
		const GTMultiPoint* multiPointObjB,
		GTRawPoint*         shortestPointA,
		GTRawPoint*         shortestPointB);


	/*****************************************************************************
	* @brief	线段到线串之间最短距离
	* @param	[in]
	*			seg1_fpt，seg1_tpt	      线段1上的始末端点
	*			lineStr				      线串
	*       	[out]
	*			startPt，endPt		      最后求得线段的始末端点
	* @return	成功返回线段到线串之间最短距离，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestSegmentToLineString(
		const GTRawPoint*	seg1_fpt, 
		const GTRawPoint*	seg1_tpt,
		const GTLineString* lineStr,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt );

	static double shortestSegmentToMultiLineString(
		const GTRawPoint*	seg1_fpt, 
		const GTRawPoint*	seg1_tpt,
		const GTMultiLineString* mLineStr,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt );

	static double shortestSegmentToMultiPolygon(
		const GTRawPoint*	seg1_fpt, 
		const GTRawPoint*	seg1_tpt,
		const GTMultiPolygon* mPolygon,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt );

	/*****************************************************************************
	* @brief	线串到线串之间最短距离
	* @param	[in]
	*			lineStrA		线串A
	*			lineStrB		线串B
	*       	[out]
	*			startPt，endPt		      最后求得线段的始末端点
	* @return	成功返回线段到线串之间最短距离，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestLineStringToLineString(
		const GTLineString* lineStrA,
		const GTLineString* lineStrB,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt );

	/*****************************************************************************
	* @brief	线串到多线串之间最短距离
	* @param	[in]
	*			lineStr				线串A
	*			multiLineStr		多线串B
	*       	[out]
	*			startPt，endPt		      最后求得线段的始末端点
	* @return	成功返回2者之间最短距离，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestLineStringToMultiLineString(
		const GTLineString*		 lineStr,
		const GTMultiLineString* multiLineStr,
		GTRawPoint*				 startPt,
		GTRawPoint*				 endPt);

	/*****************************************************************************
	* @brief	多线串到多线串之间最短距离
	* @param	[in]
	*			multiLineStrA		线串A
	*			multiLineStrB		多线串B
	*       	[out]
	*			startPt，endPt		      最后求得线段的始末端点
	* @return	成功返回2者之间最短距离，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestMultiLineStringToMultiLineString(
		const GTMultiLineString* multiLineStrA,
		const GTMultiLineString* multiLineStrB,
		GTRawPoint*				 startPt,
		GTRawPoint*				 endPt);


	/*****************************************************************************
	* @brief	多点到线串的最短距离
	* @param	[in]
	*			multiPointObj             目标多点对象
	*			lineStringObj			  线串对象
	*       	[out]
	*			shortestPointA			  最后求得在多点上的点
	*			shortestPointB			  最后求得在线串上的点
	* @return	成功返回点到线串的最短距离 ，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestMultiPointToLineString(
		const GTMultiPoint* multiPointObj,
		const GTLineString* lineStringObj,
		GTRawPoint*         shortestPointA,
		GTRawPoint*         shortestPointB);

	/*****************************************************************************
	* @brief	多点到多线串的最短距离
	* @param	[in]
	*			mPointObj				目标多点对象
	*			mLineStringObj			多线串对象
	*       	[out]
	*			shortestPointA			  最后求得在多点上的点
	*			shortestPointB			  最后求得在线串上的点
	* @return	成功返回点到线串的最短距离 ，失败返回-1.0
	* @author	陈剑，chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestMultiPointToMultiLineString(
		const GTMultiPoint*		mPointObj,
		const GTMultiLineString* mLineStringObj,
		GTRawPoint*         shortestPointA,
		GTRawPoint*         shortestPointB);





};

}/*namespace gts{*/
#endif

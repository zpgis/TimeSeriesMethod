
/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  线的简化
* 
* Author:   范俊甫, fanjf@lreis.ac.cn 2011-05-11
* Author:   范俊甫, fanjf@lreis.ac.cn, 2011-07-19
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c) 保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYSIMPLICIFY_H_INCLUDED
#define GTSGEOMETRYSIMPLICIFY_H_INCLUDED

#include "gts_cg_port.h"
#include <iostream>

namespace gts{

typedef enum e_SimpMethod
{
	DouglasPeucker = 0,
	Lang = 1,
	Reumann_witkam = 2,
	Vertex_reduction = 3,
	Opheim = 4,
	NThPoint = 5,
	PerpendDist = 6
}SimpMethod;

class GTS_DLL GTGeometrySimplicify
{
public:
	GTGeometrySimplicify(void);
	~GTGeometrySimplicify(void);


	/*****************************************************************************
	* @brief	线的简化：基于要素中的抽象几何类型和多数据量，使用douglasPeucker
	* @param	[in]
	*			geoToSimpfy  	几何对象
	*			dThresh			简化容差
	*			simMethod		线简化方法
	*			[out]
	*			pLinestringObj	简化后线
	* @note     直接改变传入的geoToSimpfy，不是返回新的简化后的几何
	* @return	
	* @author	周玉科，zyk@lreis.ac.cn,2012-05-22
	*******************************************************************************/
	static bool geometrySimplicify(GTGeometry* geoToSimpfy,
								   double	   dThresh);

	/*Gouglas-Peucker简化算法, pLinestringObj中点将被改变, dThresh为阈值*/
	static bool douglasPeucker(
		GTLineString *pLinestringObj, 
		double        dThresh = 1.0);

	/*Gouglas-Peucker简化算法, pLinestringObj中点将被改变, dThresh为阈值*/
	static GTLineString* douglasPeucker_clr(
		GTLineString *pLinestringObj, 
		double        dThresh = 1.0);

	/*****************************************************************************
	* @brief	线的简化(lang)
	* @param	[in]
	*			pLinestringObj	线对象
	*			dThresh			简化容差
	*           look_ahead      搜索范围
	*			[out]
	*			pLinestringObj	简化后线
	* @return	简化后线中点的数量
	* @note		建议look_ahead不要超过20,20只会取得%5多的点
	* @author	周玉科，zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
    static int lang ( 
		GTLineString *pLinestringObj, 
		double dThresh, 
		int look_ahead );

	/*****************************************************************************
	* @brief	线的简化(reumann_witkam)
	* @param	[in]
	*			pLinestringObj	线对象
	*			dThresh			简化容差
	*			[out]
	*			pLinestringObj	简化后线
	* @return	简化后线中点的数量
	* @note		
	* @author	周玉科，zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
	static int reumann_witkam ( 
		GTLineString *pLinestringObj, 
		double dThresh );

	/*****************************************************************************
	* @brief	线的简化(vertex_reduction)
	* @param	[in]
	*			pLinestringObj	线对象
	*			dThresh			简化容差
	*			[out]
	*			pLinestringObj	简化后线
	* @return	简化后线中点的数量
	* @note		
	* @author	周玉科，zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
	static int vertex_reduction( 
		GTLineString *pLinestringObj, 
		double dThresh );


	/*****************************************************************************
	* @brief	线的简化(opheim)
	* @param	[in]
	*			pLinestringObj	线对象
	*			min_tol			最小容差
	*			max_tol			最大容差
	*			outline			简化后线
	*			[out]
	*			outline			简化后线
	* @return	简化后线中点的数量
	* @note		
	* @author	周玉科，zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
	static int opheim( 
		GTLineString *pLinestringObj, 
		double min_tol, 
		double max_tol, 
		GTLineString *outLine);

	/*****************************************************************************
	* @brief	线的简化(NthPoint )
	* @param	[in]
	*			pLinestringObj	线对象
	*			n			    每次保留的第n个点
	*			[out]
	*			pLinestringObj	简化后线
	* @return	简化后线中点的数量
	* @note		
	* @author	周玉科，zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
	static int nThPoint( 
		GTLineString *pLinestringObj, 
		unsigned int n );

	/*****************************************************************************
	* @brief	线的简化(perpendicularDistance )
	* @param	[in]
	*			pLinestringObj	线对象
	*			min_tol		    容差
	*			outLine			简化后线
	*			[out]
	*			outLine			简化后线
	* @return	简化后线中点的数量
	* @note		结果最多简化50%的点
	* @author	周玉科，zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
	static int perpendicularDistance ( 
		GTLineString *pLinestringObj, 
		double min_tol, 
		GTLineString *outLine);

	/*移除线的重复点, 只判断邻近点*/
	static bool removeDuplicate(GTLineString *pLinestringObj);

	/*移除线的重复点, 只判断邻近点*/
	static bool removeDuplicate(GTRing *pRingObj);

	/*****************************************************************************
	* @brief	简单多边形的简化，为多多边形简化服务
	* @param	[in]
	*			pPlygn  	多边形
	*			dThresh		容差
 	*			[out]
 	* @return	简化是否成功,只是标示程序是否正常，几何上true和false不具有实际意义
	*           
 	* @author	周玉科，zyk@lreis.ac.cn,2012-05-23
	*******************************************************************************/
	static bool gtPolygonSimpicify(GTPolygon *pPlygn, double dThresh);

	/*****************************************************************************
	* @brief	简单多边形的简化，为多多边形简化服务
	* @param	[in]
	*			pPlygn  	多边形
	*			dThresh		容差
	*			look_ahead  前驱步长
 	*			[out]
 	* @return	简化是否成功,只是标示程序是否正常，几何上true和false不具有实际意义
	*           
 	* @author	范俊甫，fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_Lang(
		GTPolygon *pPlygn, 
		double dThresh,
		int look_ahead);

	/*****************************************************************************
	* @brief	简单多边形的简化，为多多边形简化服务
	* @param	[in]
	*			pPlygn  	多边形
	*			dThresh		容差
 	*			[out]
 	* @return	简化是否成功,只是标示程序是否正常，几何上true和false不具有实际意义
	*           
 	* @author	范俊甫，fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_ReumannWitkam(
		GTPolygon *pPlygn, 
		double dThresh);

	/*****************************************************************************
	* @brief	简单多边形的简化，为多多边形简化服务
	* @param	[in]
	*			pPlygn  	多边形
	*			dThresh		容差
 	*			[out]
 	* @return	简化是否成功,只是标示程序是否正常，几何上true和false不具有实际意义
	*           
 	* @author	范俊甫，fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_VertexReduction(
		GTPolygon *pPlygn, 
		double dThresh);

	/*****************************************************************************
	* @brief	简单多边形的简化，为多多边形简化服务
	* @param	[in]
	*			pPlygn  	多边形
	*			dMinThresh	最小容差
	*			dMaxThresh	最大容差
 	*			[out]
 	* @return	简化是否成功,只是标示程序是否正常，几何上true和false不具有实际意义
	*           
 	* @author	范俊甫，fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_Opheim(
		GTPolygon *pPlygn, 
		double dMinThresh, 
		double dMaxThresh);

	/*****************************************************************************
	* @brief	简单多边形的简化，为多多边形简化服务
	* @param	[in]
	*			pPlygn  	多边形
	*			iNumber		间隔保留的第n个点
 	*			[out]
 	* @return	简化是否成功,只是标示程序是否正常，几何上true和false不具有实际意义
	*           
 	* @author	范俊甫，fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_NthPoint(GTPolygon *pPlygn, int iNumber);

	
	/*****************************************************************************
	* @brief	简单多边形的简化，为多多边形简化服务
	* @param	[in]
	*			pPlygn  	多边形
	*			dThresh		容差
 	*			[out]
 	* @return	简化是否成功,只是标示程序是否正常，几何上true和false不具有实际意义
	*           
 	* @author	范俊甫，fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_PerpendDistance(
		GTPolygon *pPlygn, 
		double dThresh);

	/*****************************************************************************
	* @brief	要素集合的简化,使用dp算法
	* @param	[in]
	*			pFeatCls  	要素集合
	*			dThresh		容差
 	*			[out]
 	* @return	null
	*           
 	* @author	周玉科，zyk@lreis.ac.cn,2012-05-23
	*******************************************************************************/
	static void simplifyFeatures_dp(GTFeatureClass *pFeatCls,
								    double			 dThresh);

	/*****************************************************************************
	* @brief	要素集合的简化,使用lang算法
	* @param	[in]
	*			pFeatCls  	要素集合
	*			dThresh		容差
 	*			[out]
 	* @return	null
	*           
 	* @author	周玉科，zyk@lreis.ac.cn,2012-05-23
	*******************************************************************************/
	static void simplifyFeatures_lang(GTFeatureClass *pFeatCls,
								      double			 dThresh,
									  int look_ahead);

	/*****************************************************************************
	* @brief	要素集合的简化,使用Reumann_witkam算法
	* @param	[in]
	*			pFeatCls  	要素集合
	*			dThresh		容差
 	*			[out]
 	* @return	null
	*           
 	* @author	周玉科，zyk@lreis.ac.cn,2012-07-20
	*******************************************************************************/
	static void simplifyFeatures_rw(GTFeatureClass *pFeatCls,
								    double			dThresh);

	/*****************************************************************************
	* @brief	要素集合的简化,使用Vertex_reduce算法
	* @param	[in]
	*			pFeatCls  	要素集合
	*			dThresh		容差
 	*			[out]
 	* @return	null
	*           
 	* @author	周玉科，zyk@lreis.ac.cn,2012-07-20
	*******************************************************************************/
	static void simplifyFeatures_vr(GTFeatureClass *pFeatCls,
								    double			dThresh);

	/*****************************************************************************
	* @brief	要素集合的简化,使用op算法
	* @param	[in]
	*			pFeatCls  	要素集合
	*			dThresh		容差
 	*			[out]
 	* @return	null
	*           
 	* @author	周玉科，zyk@lreis.ac.cn,2012-07-20
	*******************************************************************************/
	static void simplifyFeatures_op(GTFeatureClass *pFeatCls,
									double min_tol, 
									double max_tol);

	/*****************************************************************************
	* @brief	要素集合的简化,使用Nth算法
	* @param	[in]
	*			pFeatCls  	要素集合
	*			dThresh		容差
 	*			[out]
 	* @return	null
	*           
 	* @author	周玉科，zyk@lreis.ac.cn,2012-07-20
	*******************************************************************************/
	static void simplifyFeatures_nth(GTFeatureClass *pFeatCls,
									 unsigned int nPt);

	/*****************************************************************************
	* @brief	要素集合的简化,使用per算法
	* @param	[in]
	*			pFeatCls  	要素集合
	*			dThresh		容差
 	*			[out]
 	* @return	null
	*           
 	* @author	周玉科，zyk@lreis.ac.cn,2012-07-20
	*******************************************************************************/
	static void simplifyFeatures_per(GTFeatureClass *pFeatCls,
									 double min_tol);

	/*****************************************************************************
	* @brief	要素集合的简化的多核算法，openmp实现
	* @param	[in]
	*			pFeatCls  	要素集合
	*			dThresh		容差
 	*			[out]
 	* @return	null
	*           
 	* @author	周玉科，zyk@lreis.ac.cn,2012-05-23
	*******************************************************************************/
	static void simplifyFeatures_omp(GTFeatureClass *pFeatCls,
									 double			 dThresh);

};

}
#endif
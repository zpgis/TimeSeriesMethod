/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  线的平滑
* Author:   范俊甫, fanjf@lreis.ac.cn,2011-07-19
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c)  保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYSMOOTH_H_INCLUDED
#define GTSGEOMETRYSMOOTH_H_INCLUDED

#include "gts_cg_port.h"
#include "gt_beziercurve.h"

typedef enum e_SmoothMethod
{
	Bezier		= 0,
	Boyle		= 1,
	Chaiken		= 2,
	Dw			= 3,
	SlideAve	= 4,
}SmoothMethod;

/* chaiken、hermite方法用到下列链表函数 */
typedef struct Pointlist_node
{
	GTRawPoint p;
	Pointlist_node *next;
} POINT_LIST_NODE;
POINT_LIST_NODE *point_list_new(GTRawPoint p);
void			 point_list_add(POINT_LIST_NODE * l, GTRawPoint p);
void			 point_list_free(POINT_LIST_NODE l);
double			 point_angle_between(GTRawPoint a, GTRawPoint b, GTRawPoint c);

namespace gts{

	class GTS_DLL GTGeometrySmooth
	{
	public:
		GTGeometrySmooth(void);
		~GTGeometrySmooth(void);

		/*****************************************************************************
		* @brief	线的平滑(boyle's forward looking algorithm)
		* @param	[in]
		*			pLinestringObj	线对象
		*			look_ahead		前驱步长
		*			[out]
		*			pLinestringObj	平滑后线
		* @return	平滑后线中点的数量
		* @note		
		* @author	周玉科，zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static int boyle(
			GTLineString *pLinestringObj, 
			int look_ahead);

		//clr包装用
		static GTLineString* boyle_clr(
			GTLineString *pLinestringObj, 
			int look_ahead);

		/*****************************************************************************
		* @brief	线的平滑(mcmaster's sliding averaging algorithm)
		* @param	[in]
		*			pLinestringObj	线对象
		*			slide	
		*			lookahead 必须是奇数
		*			[out]
		*			pLinestringObj	平滑后线
		* @return	平滑后线中点的数量
		* @note		Look ahead必须是奇数，返回值与原线点数相等
		* @author	周玉科，zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static int sliding_averaging(
			GTLineString *pLinestringObj,
			double slide, 
			int look_ahead);

		/*****************************************************************************
		* @brief	线的平滑(mcmaster's distance_weighting algorithm)
		* @param	[in]
		*			pLinestringObj	线对象
		*			slide	
		*			lookahead
		*			[out]
		*			pLinestringObj	平滑后线
		* @return	平滑后线中点的数量
		* @note		lookahead 必须为奇数, 返回值与原线点数相等
		* @author	周玉科，zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static int  distance_weighting(
			GTLineString *pLinestringObj, 
			double slide, 
			int look_ahead);

		/*****************************************************************************
		* @brief	线的平滑(chaiken algorithm)
		* @param	[in]
		*			pLinestringObj	线对象
		*			dThresh	
		*			[out]
		*			pLinestringObj	平滑后线
		* @return	平滑后线中点的数量
		* @note		
		* @author	周玉科，zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static int  chaiken(
			GTLineString *pLinestringObj, 
			double dThresh);

		/*****************************************************************************
		* @brief	通过平滑已有线构建bezier曲线(bezier algorithm)
		* @param	[in]
		*			pLinestringObj  线对象(或者有序点数组)
		*           pt_step			插值步长，贝塞尔公式维度为pt_step-1,pt_step应大于2小于33；
		*			point_on_curve	平滑线上点数
		*			[out]
		* @return	GTLineString	平滑后线
		* @note		返回值需要外部释放,pt_step不能超过33,默认为3次贝塞尔曲线;点个数point_count要大于贝塞尔维度
		* @author	周玉科，zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		static GTLineString*  bezier(
			GTLineString *pLinestringObj, 
			const int point_on_curve, 
			const int pt_step = 4);


		//-----------featureclass级别的平滑算法----------//

		/*****************************************************************************
		* @brief	featureclass级别的线平滑(boyle's forward looking algorithm)
		* @param	[in]
		*			pFeatCls	    线要素集合对象
		*			look_ahead		前驱步长
		*			[out]
		*			pLinestringObj	平滑后线
		* @return	null
		* @note		
		* @author	周玉科，zyk@lreis.ac.cn,2012-08-02
		*******************************************************************************/
		static void smoothFeatures_boyle(
			GTFeatureClass *pFeatCls, 
			int look_ahead);

		/*****************************************************************************
		* @brief	featureclass级别的线的平滑(mcmaster's sliding averaging algorithm)
		* @param	[in]
		*			pFeatCls	 线对象的featureclass
		*			slide	
		*			lookahead 必须是奇数
		*			[out]
		*			pFeatCls	 平滑后线featureclass
		* @return	null
		* @note		Look ahead必须是奇数，返回值与原线点数相等
		* @author	周玉科，zyk@lreis.ac.cn,2012-08-02
		*******************************************************************************/
		static void smoothFeatures_slideAve(
			GTFeatureClass *pFeatCls, 
			double slide, 
			int look_ahead);

		/*****************************************************************************
		* @brief	featureclass级别的线的平滑(mcmaster's distance_weighting algorithm)
		* @param	[in]
		*			pFeatCls	 线对象的featureclass
		*			slide	
		*			lookahead
		*			[out]
		*			pFeatCls	 平滑后线featureclass
		* @return	null
		* @note		lookahead 必须为奇数, 返回值与原线点数相等
		* @author	周玉科，zyk@lreis.ac.cn,2012-08-02
		*******************************************************************************/
		static void  smoothFeatures_dw(
			GTFeatureClass *pFeatCls, 
			double slide, 
			int look_ahead);

		/*****************************************************************************
		* @brief	featureclass级别的线的平滑(chaiken algorithm)
		* @param	[in]
		*			pFeatCls	 线对象的featureclass
		*			dThresh	
		*			[out]
		*			pFeatCls     平滑后线
		* @return	平滑后线中点的数量
		* @note		
		* @author	周玉科，zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static void  smoothFeatures_chaiken(
			GTFeatureClass *pFeatCls, 
			double dThresh);

		/*****************************************************************************
		* @brief	featureclass级别的,通过平滑已有线构建bezier曲线(bezier algorithm)
		* @param	[in]
		*			pFeatCls        线对象(或者有序点数组)
		*            pt_step			插值步长，贝塞尔公式维度为pt_step-1,pt_step应大于2小于33；
		*			point_on_curve	平滑线上点数
		*			[out]
		*        	pFeatCls    平滑后线
		* @return	pFeatCls    平滑后线
		* @note		pt_step不能超过33,默认为3次贝塞尔曲线;点个数point_count要大于贝塞尔维度
		* @author	周玉科，zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static void  smoothFeatures_bezier(
			GTFeatureClass *pFeatCls, 
			const int point_on_curve, 
			const int pt_step = 4);

		//private: liuyan modify at 2012-08-20
		/*****************************************************************************
		* @brief	线的平滑(hermite cubic spline algorithm)
		* @param	[in]
		*			pLinestringObj	线对象
		*			step	步长
		*           angle_thresh 角度阈值
		*			[out]
		*			pLinestringObj	平滑后线
		* @return	平滑后线中点的数量
		* @note		暂不公开,参数设置方法不明确,曲折线效果明显；
		*           传入参数要根据输入线的实际情况设置,用于最小化锐弯的点,平滑效果与数据情况有关
		* @author	周玉科，zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static int hermite( 
			GTLineString *pLinestringObj, 
			double step, 
			double angle_thresh, 
			double eps );
		//liuyan add
		/*****************************************************************************
		* @brief	线的平滑(hermite cubic spline algorithm)
		* @param	[in]
		*			pFeatCls	 线对象的featureclass
		*			step	步长
		*           angle_thresh 角度阈值
		*			[out]
		*			pFeatCls	 平滑后线featureclass
		* @return	null
		* @note		暂不公开,参数设置方法不明确,曲折线效果明显；
		*           传入参数要根据输入线的实际情况设置,用于最小化锐弯的点,平滑效果与数据情况有关
		* @author	周玉科，zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static void  smoothFeatures_hermite(
			GTFeatureClass *pFeatCls, 
			double step, 
			double angle_thresh, 
			double eps );

		/*****************************************************************************
		* @brief	线的平滑(bezier algorithm)
		* @param	[in]
		*			pPlygn			待平滑多边形
		*			point_on_curve	插值点个数
		*           pt_step			步长
		*			[out]
		* @return	bool
		* @note		
		* @author	范俊甫，fanjf@lreis.ac.cn,2012-05-13
		*******************************************************************************/
		static bool gtPolygonSmooth_Bezier(GTPolygon *pPlygn, 
			int point_on_curve,int pt_step);

		/*****************************************************************************
		* @brief	线的平滑(Boyle algorithm)
		* @param	[in]
		*			pPlygn			待平滑多边形
		*           iPrevStep		前驱步长
		*			[out]
		* @return	bool
		* @note		
		* @author	范俊甫，fanjf@lreis.ac.cn,2012-05-13
		*******************************************************************************/
		static bool gtPolygonSmooth_Boyle(
			GTPolygon *pPlygn, 
			int iPrevStep);

		/*****************************************************************************
		* @brief	线的平滑(Chaiken algorithm)
		* @param	[in]
		*			pPlygn			待平滑多边形
		*           dTolerance		容差
		*			[out]
		* @return	bool
		* @note		
		* @author	范俊甫，fanjf@lreis.ac.cn,2012-05-13
		*******************************************************************************/
		static bool gtPolygonSmooth_Chaiken(
			GTPolygon *pPlygn, 
			double dTolerance);
		/*****************************************************************************
		* @brief	线的平滑(距离权重 algorithm)
		* @param	[in]
		*			pPlygn			待平滑多边形
		*           slide			slideValue
		*			look_ahead		前驱步长
		*			[out]
		* @return	bool
		* @note		
		* @author	范俊甫，fanjf@lreis.ac.cn,2012-05-13
		*******************************************************************************/
		static bool gtPolygonSmooth_dw(
			GTPolygon *pPlygn, 
			double slide,
			int look_ahead);
		/*****************************************************************************
		* @brief	线的平滑(滑动平均 algorithm)
		* @param	[in]
		*			pPlygn			待平滑多边形
		*           slide			slideValue
		*			look_ahead		前驱步长
		*			[out]
		* @return	bool
		* @note		
		* @author	范俊甫，fanjf@lreis.ac.cn,2012-05-13
		*******************************************************************************/
		static bool gtPolygonSmooth_SlidingMean(
			GTPolygon *pPlygn, 
			double slide,
			int look_ahead);
	};


}
#endif


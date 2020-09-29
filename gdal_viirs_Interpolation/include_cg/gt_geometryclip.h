/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  从一个对象或者图层中剪出兴趣区域
* Author:   范俊甫, fanjf@lreis.ac.cn,2011-8-10
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c)  保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYCLIP_H_INCLUDED
#define GTSGEOMETRYCLIP_H_INCLUDED

#include "gts_cg_port.h"

namespace gts{

	struct GTGBLPoint{
		GTRawPoint   pt;
		unsigned int mark;
	};

	/*****************************************************************************
	*	compareWrapper 用于传入std::sort解除排序时对全局变量的依赖
	*	author	范俊甫, fanjf@lreis.ac.cn,2012-6-5
	*******************************************************************************/
	class compareWrapper
	{
	public:
		compareWrapper(const GTRawPoint& p) : _p(p)
		{}
		bool operator()(const GTRawPoint& p1, 
			const GTRawPoint& p2)
		{
			double d1 = (p1.X - _p.X) * (p1.X - _p.X) +
				(p1.Y - _p.Y) * (p1.Y - _p.Y);
			double d2 = (p2.X - _p.X) * (p2.X - _p.X) +
				(p2.Y - _p.Y) * (p2.Y - _p.Y);  
			return d1 < d2;
		}
	private:
		GTRawPoint _p;
	};

	class GTS_DLL GTGeometryClip
	{
	public:
		GTGeometryClip(void);
		~GTGeometryClip(void);

		/*点被环剪切*/
		/*线被环剪切*/
		/*环被环剪切*/

		/*****************************************************************************
		* @brief	多点被环切割
		* @param	[in]
		*			pMPoint				多点对象
		*			pRing				环对象
		*			bCheck				是否检查退化情况		
		*			[out]
		*			point_list			切割后的多点对象集合
		*			point_mark			切割后的多点对象集合对应的内外部标志
		* @return	切割后的多点对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-10
		*******************************************************************************/
		static int multiPointClipByRing(
			GTMultiPoint					*pMPoint,
			GTRing							*pRing,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false); 

		/*****************************************************************************
		* @brief	多点被多边形切割
		* @param	[in]
		*			pMPoint				多点对象
		*			pPolygon			多边形对象
		*			bCheck				是否检查退化情况		
		*			[out]
		*			point_list			切割后的多点对象集合
		*			point_mark			切割后的多点对象集合对应的内外部标志
		* @return	切割后的多点对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-10
		*******************************************************************************/
		static int multiPointClipByPolygon(
			GTMultiPoint					*pMPoint,
			GTPolygon						*pPolygon,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false);

		/*****************************************************************************
		* @brief	多点被多多边形切割
		* @param	[in]
		*			pMPoint				多点对象
		*			pMPolygon			多多边形对象
		*			bCheck				是否检查退化情况		
		*			[out]
		*			point_list			切割后的多点对象集合
		*			point_mark			切割后的多点对象集合对应的内外部标志
		* @return	切割后的多点对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-10
		*******************************************************************************/
		static int multiPointClipByMultiPolygon(
			GTMultiPoint					*pMPoint,
			GTMultiPolygon					*pMPolygon,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false);

		/*****************************************************************************
		* @brief	线被多线切割，通过标志位来决定是否使用索引
		* @param	[in]
		*			pLineString			线对象
		*			pMLineString		多线对象
		*			bCheck				是否检查退化情况		
		*			bUseRTree			是否使用索引
		*			[out]
		*			line_list			切割后的线对象集合
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-10
		*******************************************************************************/
		static int lineStringClipByMultiLineString(
			GTLineString					*pLineString,
			GTMultiLineString				*pMLineString,
			std::vector<GTLineString *>&	line_list,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	线被多边形切割，通过标志位来决定是否使用索引
		* @param	[in]
		*			pLineString			线对象
		*			pPolygon			多边形对象
		*			bCheck				是否检查退化情况		
		*			bUseRTree			是否使用索引
		*			[out]
		*			line_list			切割后的线对象集合
		*			line_mark			切割后的线对象集合对应的内外部标志
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-25
		*******************************************************************************/
		static int lineStringClipByPolygon(
			GTLineString					*pLineString,
			GTPolygon						*pPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	线被多多边形切割，通过标志位来决定是否使用索引
		* @param	[in]
		*			pLineString			线对象
		*			pMPolygon			多多边形对象
		*			bCheck				是否检查退化情况		
		*			bUseRTree			是否使用索引
		*			[out]
		*			line_list			切割后的线对象集合
		*			line_mark			切割后的线对象集合对应的内外部标志
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int lineStringClipByMultiPolygon(
			GTLineString					*pLineString,
			GTMultiPolygon					*pMPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	多线被线切割，通过标志位来决定是否使用索引
		* @param	[in]
		*			pMLineString		多线对象
		*			pLineString			线对象
		*			bCheck				是否检查退化情况		
		*			bUseRTree			是否使用索引
		*			[out]
		*			line_list			切割后的线对象集合
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-11
		*******************************************************************************/
		static int multiLineStringClipBylineString(
			GTMultiLineString				*pMLineString,
			GTLineString					*pLineString,
			std::vector<GTLineString *>&	line_list,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	多线被多线切割，通过标志位来决定是否使用索引
		* @param	[in]
		*			pMLineString		多线对象
		*			pMLineStringClip	用于切割的多线对象
		*			bCheck				是否检查退化情况		
		*			bUseRTree			是否使用索引
		*			[out]
		*			line_list			切割后的线对象集合
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-11
		*******************************************************************************/
		static int multiLineStringClipByMultiLineString(
			GTMultiLineString				*pMLineString,
			GTMultiLineString				*pMLineStringClip,
			std::vector<GTLineString *>&	line_list,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	多线被多边形切割，通过标志位来决定是否使用索引
		* @param	[in]
		*			pMLineString		多线对象
		*			pPolygon			多边形对象
		*			bCheck				是否检查退化情况		
		*			bUseRTree			是否使用索引
		*			[out]
		*			line_list			切割后的线对象集合
		*			line_mark			切割后的线对象集合对应的内外部标志
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int multiLineStringClipByPolygon(
			GTMultiLineString				*pMLineString,
			GTPolygon						*pPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false);

		/*****************************************************************************
		* @brief	多线被多多边形切割，通过标志位来决定是否使用索引
		* @param	[in]
		*			pLineString			线对象
		*			pMPolygon			多多边形对象
		*			bCheck				是否检查退化情况		
		*			bUseRTree			是否使用索引
		*			[out]
		*			line_list			切割后的线对象集合
		*			line_mark			切割后的线对象集合对应的内外部标志
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int multiLineStringClipByMultiPolygon(
			GTMultiLineString				*pMLineString,
			GTMultiPolygon					*pMPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	多边形被线切割
		* @param	[in]
		*			pPolygon			多边形对象
		*			pLineString			线对象
		*			bCheck				是否检查退化情况		
		*			[out]
		*			ring_list			切割后的环对象集合
		*			ring_mark			切割后的环对象岛的标志
		* @return	切割后的环对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int polygonClipByLineString(
			GTPolygon						*pPolygon,
			GTLineString					*pLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		/*****************************************************************************
		* @brief	多边形被多线切割
		* @param	[in]
		*			pPolygon			多边形对象
		*			pMLineString		多线对象
		*			bCheck				是否检查退化情况		
		*			[out]
		*			ring_list			切割后的环对象集合
		*			ring_mark			切割后的环对象岛的标志
		* @return	切割后的环对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int polygonClipByMultiLineString(
			GTPolygon						*pPolygon,
			GTMultiLineString				*pMLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		/*****************************************************************************
		* @brief	多多边形被线切割
		* @param	[in]
		*			pMPolygon			多多边形对象
		*			pLineString			线对象
		*			bCheck				是否检查退化情况		
		*			[out]
		*			ring_list			切割后的环对象集合
		*			ring_mark			切割后的环对象岛的标志
		* @return	切割后的环对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int multiPolygonClipBylineString(
			GTMultiPolygon					*pMPolygon,
			GTLineString					*pLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		/*****************************************************************************
		* @brief	多多边形被多线切割
		* @param	[in]
		*			pMPolygon			多多边形对象
		*			pLineString			线对象
		*			bCheck				是否检查退化情况		
		*			[out]
		*			ring_list			切割后的环对象集合
		*			ring_mark			切割后的环对象岛的标志
		* @return	切割后的环对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int multiPolygonClipByMultiLineString(
			GTMultiPolygon					*pMPolygon,
			GTMultiLineString				*pMLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		/*****************************************************************************
		* @brief	多边形被线切割,自写算法，采用多个双向链表寻找切割后图形
		* @param	[in]
		*			pPolygon			多边形对象
		*			pLineString			线对象
		*			bCheck				是否检查退化情况		
		*			[out]
		*			ring_list			切割后的环对象集合
		*			ring_mark			切割后的环对象岛的标志
		* @return	切割后的环对象数量
		* @note		返回的指针链表需要外部释放，线自相交的情况暂时无处理，可能会有bug
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-30 完成与8-4
		*******************************************************************************/
		static int polygonClipByLineString1(
			GTPolygon              *pPolygon,
			GTLineString           *pLineString,
			std::vector<GTRing *>&  ring_list, 
			std::vector<int>&		ring_mark,
			bool                    bCheck = false);

		/*2011-10-25 以下从GTGeometryBoolean中移动至此。 刘衍*/
		/*线被环*/
		static int lineStringClipByRing(
			GTLineString                *pLineString,
			GTRing                      *pRing,
			std::vector<GTLineString *>& line_list,       /*结果指针数组, 注意释放*/
			std::vector<unsigned int>  & line_mark,       /*0为外部, 1为内部*/
			bool                         bCheck = false); /*是否检查退化情况*/ 

		/*****************************************************************************
		* @brief	线被环切割，不对环对象建立索引，经测试效率较采用索引的函数高
		* @param	[in]
		*			pLineString			线对象
		*			pRing				环对象
		*			bCheck				是否检查退化情况		
		*			[out]
		*			line_list			切割后的线对象集合
		*			line_mark			切割后的线对象集合对应的内外部标志
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-25
		*******************************************************************************/
		static int lineStringClipByRingWithOutRTree(
			const GTLineString                *pLineString,
			GTRing                      *pRing,
			std::vector<GTLineString *>& line_list,
			std::vector<unsigned int>  & line_mark,
			bool                         bCheck = false);

		/*****************************************************************************
		* @brief	线被环切割，通过标志位来决定是否使用索引
		* @param	[in]
		*			pLineString			线对象
		*			pRing				环对象
		*			bCheck				是否检查退化情况		
		*			bUseRTree			是否使用索引
		*			[out]
		*			line_list			切割后的线对象集合
		*			line_mark			切割后的线对象集合对应的内外部标志
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-25
		*******************************************************************************/
		static int lineStringClipByRing1(
			GTLineString                *pLineString,
			GTRing                      *pRing,
			std::vector<GTLineString *>& line_list,
			std::vector<unsigned int>  & line_mark,
			bool                         bCheck = false,
			bool						 bUseRTree = false); 

		/*线被线*/
		static int lineStringClipByLineString(
			GTLineString                *pLineString,
			GTLineString                *pLineStringClip,
			std::vector<GTLineString *>& line_list,       /*结果指针数组, 注意释放*/
			bool                         bCheck = false); /*是否检查退化情况*/ 

		/*****************************************************************************
		* @brief	线被线切割，不对线对象建立索引，经测试效率较采用索引的函数高
		* @param	[in]
		*			pLineString			线对象
		*			pLineStringClip		用于切割的线对象
		*			bCheck				是否检查退化情况		
		*			[out]
		*			line_list			切割后的线对象集合
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-11
		*******************************************************************************/
		static int lineStringClipByLineStringWithOutRTree(
			GTLineString                *pLineString,
			GTLineString                *pLineStringClip,
			std::vector<GTLineString *>& line_list,       /*结果指针数组, 注意释放*/
			bool                         bCheck = false);

		/*****************************************************************************
		* @brief	线被线切割，通过标志位来决定是否使用索引
		* @param	[in]
		*			pLineString			线对象
		*			pRing				环对象
		*			bCheck				是否检查退化情况		
		*			bUseRTree			是否使用索引
		*			[out]
		*			line_list			切割后的线对象集合
		* @return	切割后的线对象数量
		* @note		返回的指针链表需要外部释放
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-11
		*******************************************************************************/
		static int lineStringClipByLineString1(
			GTLineString                *pLineString,
			GTLineString                *pLineStringClip,
			std::vector<GTLineString *>& line_list,
			bool                         bCheck = false,
			bool						 bUseRTree = false);

		/*环被线切割, 线的两个端点必须在环的外部*/
		static int ringClipByLineString(
			GTRing                 *pRing,
			GTLineString           *pLineString,
			std::vector<GTRing *>&  ring_list,       /*结果指针数组, 注意释放*/
			bool                    bCheck = false); /*是否检查退化情况*/ 
		/*2011-10-25 以上从GTGeometryBoolean中移动至此。 刘衍*/

		/*扩展至多线和多边形*/
		/*图层剪切*/

		static int geometryClipByGeometry(
			GTGeometry					*pGeo,
			GTGeometry					*pGeoClip,
			std::vector<GTGeometry *>&	geolist_list,
			bool						bCheck = false,
			bool						bUseRTree = false);

		static int featureClipByfeature(
			GTFeature					*pFeat,
			GTFeature					*pFeatClip,
			GTFeatureClass				&outClss,
			bool						bCheck = false,
			bool						bUseRTree = false);

		static int featureClassClipByfeatureClass(
			GTFeatureClass				&srcClss,
			GTFeatureClass				&clipCLss,
			GTFeatureClass				&outClss,
			bool						bCheck = false,
			bool						bUseRTree = false);

		/*****************************************************************************
		* @brief	图层被外包矩形剪切
		* @param	[in]
		*			srcLayer					被切割图层
		*			env							用于切割的外包矩形
		*			bUseRTree					是否检查退化情况		
		*			[out]
		*			desLayer					切割后形成的图层
		* @return	切割后的feature对象数量
		* @note		
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-18
		*******************************************************************************/
		static int featureLayerClipByEnvelop(
			const GTFeatureLayer*	srcLayer,
			GTFeatureLayer*			desLayer,
			GTEnvelop&				env);

		static int featureClassClipByEnvelop(
			GTFeatureClass&			inClss,
			GTFeatureClass&			outClss,
			GTEnvelop&				env);

		/*用于图层切割的必须是polygon*/

		/****************************************************
		*以下函数_mp表示单机多核算法*
		****************************************************/

		static int multiPointClipByRing_mp(
			GTMultiPoint					*pMPoint,
			GTRing							*pRing,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false); 

		static int multiPointClipByPolygon_mp(
			GTMultiPoint					*pMPoint,
			GTPolygon						*pPolygon,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false);

		static int multiPointClipByMultiPolygon_mp(
			GTMultiPoint					*pMPoint,
			GTMultiPolygon					*pMPolygon,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false);

		static int multiLineStringClipBylineString_mp(
			GTMultiLineString				*pMLineString,
			GTLineString					*pLineString,
			std::vector<GTLineString *>&	line_list,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		static int multiLineStringClipByMultiLineString_mp(
			GTMultiLineString				*pMLineString,
			GTMultiLineString				*pMLineStringClip,
			std::vector<GTLineString *>&	line_list,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		static int multiLineStringClipByPolygon_mp(
			GTMultiLineString				*pMLineString,
			GTPolygon						*pPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false);

		static int multiLineStringClipByMultiPolygon_mp(
			GTMultiLineString				*pMLineString,
			GTMultiPolygon					*pMPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		static int multiPolygonClipBylineString_mp(
			GTMultiPolygon					*pMPolygon,
			GTLineString					*pLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		static int multiPolygonClipByMultiLineString_mp(
			GTMultiPolygon					*pMPolygon,
			GTMultiLineString				*pMLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		static int geometryClipByGeometry_mp(
			GTGeometry					*pGeo,
			GTGeometry					*pGeoClip,
			std::vector<GTGeometry *>&	geolist_list,
			bool						bCheck = false,
			bool						bUseRTree = false);

		static int featureClipByfeature_mp(
			GTFeature					*pFeat,
			GTFeature					*pFeatClip,
			GTFeatureClass				&outClss,
			bool						bCheck = false,
			bool						bUseRTree = false);

		static int featureClassClipByfeatureClass_mp(
			GTFeatureClass				&srcClss,
			GTFeatureClass				&clipCLss,
			GTFeatureClass				&outClss,
			bool						bCheck = false,
			bool						bUseRTree = false);

		/*2011-10-25 以下从GTGeometryBoolean中移动至此。 刘衍*/
	private:
		static bool compare(const GTRawPoint& p1, const GTRawPoint& p2);
		static int removeDuplicate(std::vector<GTRawPoint> &pt_list, int pt_num);
		/*2011-10-25 以上从GTGeometryBoolean中移动至此。 刘衍*/
	};
};

#endif
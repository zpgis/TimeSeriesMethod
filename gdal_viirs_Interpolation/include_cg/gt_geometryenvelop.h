/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  从一个对象或者图层中剪出兴趣区域
* Author:   范俊甫, fanjf@lreis.ac.cn,2012-12-20
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c)  保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYENVELOP_H_INCLUDED
#define GTSGEOMETRYENVELOP_H_INCLUDED

#include "gts_cg_port.h"

namespace gts{

	class GTS_DLL GTGeometryWithEnvelop
	{
	public:
		GTGeometryWithEnvelop(void);
		~GTGeometryWithEnvelop(void);

		static bool point_in_ring(const GTRawPoint& rp, const GTRing* rg);
		static bool point_in_polygon(const GTRawPoint& rp, GTPolygon* pg);
		static bool segmentIntersected(const GTRawPoint& seg1_fpt,	
			const GTRawPoint& seg1_tpt,
			const GTRawPoint& seg2_fpt,
			const GTRawPoint& seg2_tpt);
		inline static bool isClockwise(const GTRawPoint& pnt1, 
			const GTRawPoint& pnt2, 
			const GTRawPoint& pnt3)
		{
			return ((pnt3.Y - pnt1.Y) * (pnt2.X - pnt1.X)) < ((pnt2.Y - pnt1.Y) * (pnt3.X - pnt1.X));
		}
		inline static bool isSegmentIntersect(const GTRawPoint& seg1_fpt, 
			const GTRawPoint& seg1_tpt, 
			const GTRawPoint& seg2_fpt, 
			const GTRawPoint& seg2_tpt)
		{
			if(MIN(seg1_fpt.X, seg1_tpt.X) > MAX(seg2_fpt.X, seg2_tpt.X) ||
				MAX(seg1_fpt.X, seg1_tpt.X) < MIN(seg2_fpt.X, seg2_tpt.X) ||
				MIN(seg1_fpt.Y, seg1_tpt.Y) > MAX(seg2_fpt.Y, seg2_tpt.Y) ||
				MAX(seg1_fpt.Y, seg1_tpt.Y) < MIN(seg2_fpt.Y, seg2_tpt.Y))
				return false;
			return ((isClockwise(seg1_fpt, seg2_fpt, seg2_tpt) != isClockwise(seg1_tpt, seg2_fpt, seg2_tpt)) &&
				(isClockwise(seg1_fpt, seg1_tpt, seg2_fpt) != isClockwise(seg1_fpt, seg1_tpt, seg2_tpt)));
		}

		static bool isEnvelopWithinRing(const GTEnvelop& pEnvelop, 
										const GTRing& ringObj);

			/*****************************************************************************
			* @brief	线是否与envelope相离
			* @param	[in]
			*			lsObj						线对象
			*			pEnvelop					envelope对象	
			*			[out]
 			* @return	是否相离
			* @note		
			* @author	周玉科，zyk@lreis.ac.cn,2012-12-21
			*******************************************************************************/
		static bool isLineStringDisjoinWithEnvelop(const GTLineString& lsObj, 
			const GTEnvelop& pEnvelop);

			/*****************************************************************************
			* @brief	环是否与envelope相离
			* @param	[in]
			*			ploygonObj					多边形对象
			*			pEnvelop					envelope对象	
			*			[out]
 			* @return	是否相离
			* @note		
			* @author	周玉科，zyk@lreis.ac.cn,2012-12-21
			*******************************************************************************/
		static bool isRingDisjoinWithEnvelop(const GTRing& ringObj, 
			const GTEnvelop& pEnvelop);

			/*****************************************************************************
			* @brief	多线是否与envelope相离
			* @param	[in]
			*			mulinestringObj				多线对象
			*			pEnvelop					envelope对象	
			*			[out]
 			* @return	是否相离
			* @note		
			* @author	周玉科，zyk@lreis.ac.cn,2012-12-21
			*******************************************************************************/
		static bool isMultiLineStringDisjoinWithEnvelop(const GTMultiLineString* mulinestringObj, 
			const GTEnvelop& pEnvelop);


		/*****************************************************************************
		* @brief	多边形是否与envelope相离
		* @param	[in]
		*			ploygonObj					多边形对象
		*			pEnvelop					envelope对象	
		*			[out]
 		* @return	是否相离
		* @note		
		* @author	周玉科，zyk@lreis.ac.cn,2012-12-21
		*******************************************************************************/
		static bool isPolygonDisjoinWithEnvelop(const GTPolygon* ploygonObj,const GTEnvelop& pEnvelop );

		/*****************************************************************************
	    * @brief	多多边形是否与envelope相离
		* @param	[in]
		* mltploygonObj			多多边形对象
		* pEnvelop					envelope对象	
		* [out]
 		* @return	是否相离
		* @note		
		* @author	周玉科，zyk@lreis.ac.cn,2012-12-21
		*******************************************************************************/
		static bool isMultiPolygonDisjoinWithEnvelop(const GTMultiPolygon* mltploygonObj,const GTEnvelop& pEnvelop );

			/*****************************************************************************
			* @brief	线是否在envelope内
			* @param	[in]
			*			lsObj						线对象
			*			pEnvelop					envelope对象	
			*			[out]
 			* @return	是否包含
			* @note		
			* @author	周玉科，zyk@lreis.ac.cn,2012-12-21
			*******************************************************************************/
		static bool isLineStringWithinEnvelop(const GTLineString* lsObj, 
											  const GTEnvelop& pEnvelop);

			/*****************************************************************************
			* @brief	多线是否在envelope内
			* @param	[in]
			*			lsObj						线对象
			*			pEnvelop					envelope对象	
			*			[out]
 			* @return	是否包含
			* @note		
			* @author	周玉科，zyk@lreis.ac.cn,2012-12-21
			*******************************************************************************/
		static bool isMultiLineStringWithinEnvelop(const GTMultiLineString* lsObj, 
												   const GTEnvelop& pEnvelop);
	};
}

#endif
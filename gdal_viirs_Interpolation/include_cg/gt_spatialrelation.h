/****************************************************************
 *
 * Project:  地理信息处理工具集
 * Purpose:  空间关系处理类
 * Author:   范俊甫, fanjf@lreis.ac.cn 2010年-11月-20日
 *
 ****************************************************************
 * Copyright (c) IGSNRR, CAS, China. All rights reserved.
 * 版权所有   (c) 中国科学院地理科学与资源研究所. 保留所有权利
 ****************************************************************/
#ifndef GTSPATIALRELATION_H_INCLUDED
#define GTSPATIALRELATION_H_INCLUDED

#include "gt_geometry.h"
#include "gts_cg_port.h"
#include "gt_geometryanalysis.h"

namespace gts{

/*********************************/
/* 用于几何对象操作的外部接口函数       /
/*********************************/

/*
 *几何关系操作类定义
 *需要考虑空间位置表达的的Tolerance
 */
class GTS_DLL GTSpatialRelation
{
public:
	GTSpatialRelation(void);
	~GTSpatialRelation(void);
	
	/*设置和使用Tolerance*/
	double getTolerance() const;
	void   setTolerance(double dTol);

	/*用于判断几何对象位置关系的函数*/
	/*判断对象使用常量指针传递*/
	/*在不知道具体几何类型下的综合判断*/
	enumGTSpatialRelation getObjectsSR(
		const GTGeometry* geoObject1, 
		const GTGeometry* geoObject2);

	/*以下为已知类型的具体判断函数*/
	
	/*Envelop与Envelop关系*/
	enumGTSpatialRelation getEnvelopAndEnvelop(
		const GTEnvelop* envelopObj1, 
		const GTEnvelop* envelopObj2);
	
	/*两个Envelop是否重叠*/
	 bool  isEnvelopsEqual(const GTEnvelop* envelopObj1, 
		                 const GTEnvelop* envelopObj2);

	/*两个Envelop是否分离*/
	 bool  isEnvelopsDisjoint(const GTEnvelop* envelopObj1, 
		                    const GTEnvelop* envelopObj2);
	
	/*****************************************************************************
	* @brief	点与点相离
	* @param	[in]
	*			pointObj1  线对象
	*           pointObj2  线对象	
	*			[out]
	*			
	* @return	点与点是否相离
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool  isPointsDisjoint(const GTPoint* pointObj1, 
		                  const GTPoint* pointObj2);


	/*****************************************************************************
	* @brief	点与多点相离
	* @param	[in]
	*			pointObj1      点对象
	*           multipointObj  多点对象	
	*			[out]
	*			
	* @return	点与多点是否相离
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool  isPointMultiPointDisjoint(const GTPoint      *pointObj, 
								   const GTMultiPoint *multipointObj);

	/*****************************************************************************
	* @brief	点与线是否相离
	* @param	[in]
	*			pointObj1  线对象
	*           pointObj2  线对象	
	*			[out]
	*			
	* @return	点与线是否相离
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isPointLinestringDisjoint(const GTPoint      *pointObj, 
								   const GTLineString *linestringObj);

	/*****************************************************************************
	* @brief	点与多边形是否相离
	* @param	[in]
	*			pointObj    点对象
	*           polygonObj  多边形对象	
	*			[out]
	*			
	* @return	点与多边形是否相离
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isPointPolygonDisjoint(const GTPoint      *pointObj, 
								const GTPolygon    *polygonObj);

	/*****************************************************************************
	* @brief	多点与多点是否相离
	* @param	[in]
	*			pointObj    点对象
	*           polygonObj  多边形对象	
	*			[out]
	*			
	* @return	多点与多点是否相离
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMultiPointsDisjoint(const GTMultiPoint    *multipointObj1, 
							   const GTMultiPoint    *multipointObj2);

	/*****************************************************************************
	* @brief	多点与线是否相离
	* @param	[in]
	*			multipointObj1  多点对象
	*           linestringObj2  线对象	
	*			[out]
	*			
	* @return	多点与线是否相离
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMultiPointLinestringDisjoint(const GTMultiPoint    *multipointObj1, 
										const GTLineString    *linestringObj2);

	/*****************************************************************************
	* @brief	多点与多边形是否相离
	* @param	[in]
	*			multipointObj1  多点对象
	*           linestringObj2  线对象	
	*			[out]
	*			
	* @return	多点与多边形是否相离
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMultiPointPolygonDisjoint(const GTMultiPoint    *multipointObj, 
							         const GTPolygon       *polygonObj);


	/*****************************************************************************
	* @brief	线与多边形是否相离
	* @param	[in]
	*			linestringObj   线对象
	*           polygonObj      多边形对象	
	*			[out]
	*			
	* @return	线与多边形是否相离
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isLinestringPolygonDisjoint(const GTLineString  *linestringObj, 
							         const GTPolygon     *polygonObj);

	/*****************************************************************************
	* @brief	多边形与多边形是否相离
	* @param	[in]
	*			polygonObj1  多边形对象
	*           polygonObj2  多边形对象	
	*			[out]
	*			
	* @return	多边形与多边形是否相离
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isPolygonsDisjoint(const GTPolygon  *polygonObj1, 
							const GTPolygon  *polygonObj2);

	/*****************************************************************************
	* @brief	点与线是否相接
	* @param	[in]
	*			pointObj       点对象
	*           linestringObj  线对象	
	*			[out]
	*			
	* @return	点与线是否相接
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isPointLinestringTouch(const GTPoint       *pointObj, 
								const GTLineString  *linestringObj);

	/*****************************************************************************
	* @brief	多点与线是否相接
	* @param	[in]
	*			multipointObj  多点对象
	*           linestringObj  线对象	
	*			[out]
	*			
	* @return	多点与线是否相接
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMultiPointLinestringTouch(const GTMultiPoint  *multipointObj, 
									 const GTLineString  *linestringObj);

	/*****************************************************************************
	* @brief	点与多边形是否相接
	* @param	[in]
	*			pointObj  多点对象
	*           polygonObj     多边形对象	
	*			[out]
	*			
	* @return	点与多边形是否相接
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isPointPolygonTouch(const GTPoint    *pointObj, 
							 const GTPolygon  *polygonObj);
	/*****************************************************************************
	* @brief	多点与多边形是否相接
	* @param	[in]
	*			multipointObj  多点对象
	*           polygonObj     多边形对象	
	*			[out]
	*			
	* @return	多点与多边形是否相接
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMultiPointPolygonTouch(const GTMultiPoint  *multipointObj, 
								  const GTPolygon     *polygonObj);

	/*****************************************************************************
	* @brief	线与线是否相接
	* @param	[in]
	*			linestringObj1  线对象
	*           linestringObj2  线对象	
	*			[out]
	*			
	* @return	线与线是否相接
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isLinestringsTouch(const GTLineString  *linestringObj1, 
							const GTLineString  *linestringObj2);


	/*****************************************************************************
	* @brief	线与多点是否相交(cross)
	* @param	[in]
	*			multiPointObj  多点对象
	*           linestringObj  线对象	
	*			[out]
	*			
	* @return	线与多点是否相交(cross)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMulPointLinestringCross(const GTMultiPoint   *multiPointObj, 
								   const GTLineString   *linestringObj);

	/*****************************************************************************
	* @brief	线与线是否相交(cross)
	* @param	[in]
	*			linestringObj1  线对象
	*           linestringObj2  线对象	
	*			[out]
	*			
	* @return	线与线是否相交(cross)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isLinestringsCross(const GTLineString   *linestringObj1, 
							const GTLineString   *linestringObj2);

	/*****************************************************************************
	* @brief	多点与多边形是否相交(cross)
	* @param	[in]
	*			multipointObj1  多点对象
	*           polygonObj      多边形对象	
	*			[out]
	*			
	* @return	多点与多边形是否相交(cross)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-14
	*******************************************************************************/
	 bool isMulPointsPolygonCross(const GTMultiPoint   *multipointObj1, 
								 const GTPolygon      *polygonObj);

	/*****************************************************************************
	* @brief	线与多边形是否相交(cross)
	* @param	[in]
	*			linestringObj  线对象
	*           polygonObj     多边形对象	
	*			[out]
	*			
	* @return	多点与多边形是否跨越(cross)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-14
	*******************************************************************************/
	 bool isLinestringPolygonCross(const GTLineString   *linestringObj, 
								  const GTPolygon      *polygonObj);

	/*****************************************************************************
	* @brief	多点是否包含点(contain)
	* @param	[in]
	*			multipointObj  多点对象
	*           pointObj       点对象	
	*			[out]
	*			
	* @return	多点是否包含点(contain)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isMulPointContainPoint(const GTMultiPoint   *multipointObj, 
								const GTPoint        *pointObj);


	/*****************************************************************************
	* @brief	多点是否包含多点(contain)
	* @param	[in]
	*			multipointObj1  多点对象
	*           multipointObj2  多点对象	
	*			[out]
	*			
	* @return	多点multipointObj1是否包含多点multipointObj2(contain)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isMulPointContainMultiPoint(const GTMultiPoint   *multipointObj1, 
								     const GTMultiPoint   *multipointObj2);

	/*****************************************************************************
	* @brief	线是否包含点(contain)
	* @param	[in]
	*			linestringObj  线对象
	*           pointObj       点对象	
	*			[out]
	*			
	* @return	线是否包含点(contain)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isLinestringContainPoint(const GTLineString  *linestringObj, 
								  const GTPoint       *pointObj);

	/*****************************************************************************
	* @brief	线是否包含多点(contain)
	* @param	[in]
	*			linestringObj  线对象
	*           multipointObj  多点对象	
	*			[out]
	*			
	* @return	线是否包含多点(contain)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isLinestringContainMultiPoint(const GTLineString  *linestringObj, 
								       const GTMultiPoint  *multipointObj);

	/*****************************************************************************
	* @brief	多边形是否包含点(contain)
	* @param	[in]
	*			polygonObj  多边形对象
	*           pointObj    点对象	
	*			[out]
	*			
	* @return	多边形是否包含点(contain)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPolygonContainPoint(const GTPolygon  *polygonObj, 
							   const GTPoint    *pointObj);

	/*****************************************************************************
	* @brief	多边形是否包含多点(contain)
	* @param	[in]
	*			polygonObj     多边形对象
	*           multipointObj  多点对象	
	*			[out]
	*			
	* @return	多边形是否包含多点(contain)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPolygonContainMultiPoint(const GTPolygon    *polygonObj, 
							        const GTMultiPoint *multipointObj);

	/*****************************************************************************
	* @brief	线是否包含线(contain)----未完成
	* @param	[in]
	*			polygonObj     多边形对象
	*           multipointObj  多点对象	
	*			[out]
	*			
	* @return	多边形是否包含多点(contain)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
/*
	 bool isLinestringContainLinestring(const GTLineString *linestringObj1, 
							           const GTLineString *linestringObj2);*/

	/*****************************************************************************
	* @brief	多边形是否包含线(contain)
	* @param	[in]
	*			polygonObj     多边形对象
	*           multipointObj  多点对象	
	*			[out]
	*			
	* @return	多边形是否包含线(contain)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPolygonContainLinestring(const GTPolygon    *polygonObj, 
							        const GTLineString *linestringObj);

	/*****************************************************************************
	* @brief	多边形是否包含多边形(contain)
	* @param	[in]
	*			polygonObj1  多边形对象
	*           polygonObj2  多边形对象	
	*			[out]
	*			
	* @return	多边形是否包含多边形(contain)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPolygonContainPolygon(const GTPolygon  *polygonObj1, 
							     const GTPolygon  *polygonObj2);

	/*****************************************************************************
	* @brief	点与点是否相交(intersect)
	* @param	[in]
	*			pointObj1  点对象
	*           pointObj2  点对象	
	*			[out]
	*			
	* @return	点与点是否相交(intersect)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPointsIntersect(const GTPoint  *pointObj1, 
						   const GTPoint  *pointObj2);

	/*****************************************************************************
	* @brief	点与多点是否相交(intersect)
	* @param	[in]
	*			pointObj  点对象
	*           multipointObj  点对象	
	*			[out]
	*			
	* @return	点与多点是否相交(intersect)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPointMultiPointIntersect(const GTPoint       *pointObj, 
									const GTMultiPoint  *multipointObj);

	/*****************************************************************************
	* @brief	多点与多点是否相交(intersect)
	* @param	[in]
	*			multipointObj2  多点对象
	*           multipointObj1  多点对象	
	*			[out]
	*			
	* @return	点与多点是否相交(intersect)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isMultiPointsIntersect(const GTMultiPoint  *multipointObj1,
								const GTMultiPoint  *multipointObj2);

	/*****************************************************************************
	* @brief	线与点是否相交(intersect)
	* @param	[in]
	*			pointObj       点对象
	*           linestringObj  线对象	
	*			[out]
	*			
	* @return	线与点是否相交(intersect)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPointLinestringIntersect(const GTPoint       *pointObj,
								    const GTLineString  *linestringObj);
	/*****************************************************************************
	* @brief	线与多点是否相交(intersect)
	* @param	[in]
	*			multipointObj       点对象
	*           linestringObj  线对象	
	*			[out]
	*			
	* @return	线与点是否相交(intersect)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isMultiPointLinestringIntersect(const GTMultiPoint  *multipointObj,
												const GTLineString  *linestringObj);
	
	/*****************************************************************************
	* @brief	点与多边形是否相交(intersect)
	* @param	[in]
	*			pointObj    点对象
	*           polygonObj  多边形对象	
	*			[out]
	*			
	* @return	点与多边形是否相交(intersect)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPointPolygonIntersect(const GTPoint    *pointObj,
										const GTPolygon  *polygonObj);
	
	/*****************************************************************************
	* @brief	多点与多边形是否相交(intersect)
	* @param	[in]
	*			multipointObj    多点对象
	*           polygonObj  线对象	
	*			[out]
	*			
	* @return	多点与多边形是否相交(intersect)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isMultiPointPolygonIntersect(const GTMultiPoint  *multipointObj,
								      const GTPolygon     *polygonObj);

	/*****************************************************************************
	* @brief	线与线是否相交(intersect)
	* @param	[in]
	*			linestringObj1  线对象
	*           linestringObj2  线对象	
	*			[out]
	*			
	* @return	线与线是否相交(intersect)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isLinestringsIntersect(const GTLineString  *linestringObj1,
								const GTLineString  *linestringObj2);

	/*****************************************************************************
	* @brief	线与多边形是否相交(intersect)
	* @param	[in]
	*			linestringObj  线对象
	*           polygonObj  线对象	
	*			[out]
	*			
	* @return	线与多边形是否相交(intersect)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isLinestringsPolygonIntersect(const GTLineString  *linestringObj,
									   const GTPolygon     *polygonObj);

	/*****************************************************************************
	* @brief	多边形与多边形是否相交(intersect)
	* @param	[in]
	*			polygonObj1  多边形对象
	*           polygonObj2  多边形对象	
	*			[out]
	*			
	* @return	多边形与多边形是否相交(intersect)
	* @note		 
	* @author	周玉科，zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPolygonsIntersect(const GTPolygon  *polygonObj1,
							 const GTPolygon  *polygonObj2);

	/*RawPoint与Envelop*/
	enumGTSpatialRelation getRawPointAndEnvelopSR(
		const GTRawPoint* rawPointObj, 
		const GTEnvelop*  envelopObj);

	/*单点与Envelop*/
	enumGTSpatialRelation getPointAndEnvelopSR(
		const GTPoint*   pointObj, 
		const GTEnvelop* envelopObj);
	
	/*线段与Envelop*/
	enumGTSpatialRelation getSegmentAndEnvelopSR(
		const GTLine*    lineObj,
		const GTEnvelop* envelopObj);

	/*线与Envelop*/
	enumGTSpatialRelation getLineStringAndEnvelopSR(
		const GTLineString* lineStringObj,
		const GTEnvelop*    envelopObj);

	/*环与Envelop*/
	enumGTSpatialRelation getRingAndEnvelopSR(
		const GTRing*    ringObj,
		const GTEnvelop* envelopObj);

	/*多边形与Envelop*/
	enumGTSpatialRelation getPolygonAndEnvelopSR(
		const GTPolygon* polygonObj,
		const GTEnvelop* envelopObj);
	
	/*单点与单点*/
	enumGTSpatialRelation getPointAndPointSR(
		const GTPoint* pointObj1, 
		const GTPoint* pointObj2);
	
	/*单点与线段*/
	enumGTSpatialRelation getPointAndLineSR(
		const GTPoint* pointObj, 
		const GTLine*  lineObj);
	
	/*单点与线*/
	enumGTSpatialRelation getPointAndLineStringSR(
		const GTPoint*       pointObj, 
		const GTLineString* lineStringObj);

	/*单点与环*/
	enumGTSpatialRelation getPointAndRingSR(
		const GTPoint*      pointObj, 
		const GTLineString* lineStringObj);

	/*单点与多边形*/
	enumGTSpatialRelation getPointAndPolygonSR(
		const GTPoint*   pointObj, 
		const GTPolygon* polygonObj);

protected:
	double dspTolerance;
	bool   useTolerence;

	void   openTolerance();
	void   closeTolerance();
	bool   isUseTolerance() const;
};

}
#endif
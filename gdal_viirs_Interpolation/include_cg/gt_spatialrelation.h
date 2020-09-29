/****************************************************************
 *
 * Project:  ������Ϣ�����߼�
 * Purpose:  �ռ��ϵ������
 * Author:   ������, fanjf@lreis.ac.cn 2010��-11��-20��
 *
 ****************************************************************
 * Copyright (c) IGSNRR, CAS, China. All rights reserved.
 * ��Ȩ����   (c) �й���ѧԺ�����ѧ����Դ�о���. ��������Ȩ��
 ****************************************************************/
#ifndef GTSPATIALRELATION_H_INCLUDED
#define GTSPATIALRELATION_H_INCLUDED

#include "gt_geometry.h"
#include "gts_cg_port.h"
#include "gt_geometryanalysis.h"

namespace gts{

/*********************************/
/* ���ڼ��ζ���������ⲿ�ӿں���       /
/*********************************/

/*
 *���ι�ϵ�����ඨ��
 *��Ҫ���ǿռ�λ�ñ��ĵ�Tolerance
 */
class GTS_DLL GTSpatialRelation
{
public:
	GTSpatialRelation(void);
	~GTSpatialRelation(void);
	
	/*���ú�ʹ��Tolerance*/
	double getTolerance() const;
	void   setTolerance(double dTol);

	/*�����жϼ��ζ���λ�ù�ϵ�ĺ���*/
	/*�ж϶���ʹ�ó���ָ�봫��*/
	/*�ڲ�֪�����弸�������µ��ۺ��ж�*/
	enumGTSpatialRelation getObjectsSR(
		const GTGeometry* geoObject1, 
		const GTGeometry* geoObject2);

	/*����Ϊ��֪���͵ľ����жϺ���*/
	
	/*Envelop��Envelop��ϵ*/
	enumGTSpatialRelation getEnvelopAndEnvelop(
		const GTEnvelop* envelopObj1, 
		const GTEnvelop* envelopObj2);
	
	/*����Envelop�Ƿ��ص�*/
	 bool  isEnvelopsEqual(const GTEnvelop* envelopObj1, 
		                 const GTEnvelop* envelopObj2);

	/*����Envelop�Ƿ����*/
	 bool  isEnvelopsDisjoint(const GTEnvelop* envelopObj1, 
		                    const GTEnvelop* envelopObj2);
	
	/*****************************************************************************
	* @brief	���������
	* @param	[in]
	*			pointObj1  �߶���
	*           pointObj2  �߶���	
	*			[out]
	*			
	* @return	������Ƿ�����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool  isPointsDisjoint(const GTPoint* pointObj1, 
		                  const GTPoint* pointObj2);


	/*****************************************************************************
	* @brief	����������
	* @param	[in]
	*			pointObj1      �����
	*           multipointObj  ������	
	*			[out]
	*			
	* @return	�������Ƿ�����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool  isPointMultiPointDisjoint(const GTPoint      *pointObj, 
								   const GTMultiPoint *multipointObj);

	/*****************************************************************************
	* @brief	�������Ƿ�����
	* @param	[in]
	*			pointObj1  �߶���
	*           pointObj2  �߶���	
	*			[out]
	*			
	* @return	�������Ƿ�����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isPointLinestringDisjoint(const GTPoint      *pointObj, 
								   const GTLineString *linestringObj);

	/*****************************************************************************
	* @brief	���������Ƿ�����
	* @param	[in]
	*			pointObj    �����
	*           polygonObj  ����ζ���	
	*			[out]
	*			
	* @return	���������Ƿ�����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isPointPolygonDisjoint(const GTPoint      *pointObj, 
								const GTPolygon    *polygonObj);

	/*****************************************************************************
	* @brief	��������Ƿ�����
	* @param	[in]
	*			pointObj    �����
	*           polygonObj  ����ζ���	
	*			[out]
	*			
	* @return	��������Ƿ�����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMultiPointsDisjoint(const GTMultiPoint    *multipointObj1, 
							   const GTMultiPoint    *multipointObj2);

	/*****************************************************************************
	* @brief	��������Ƿ�����
	* @param	[in]
	*			multipointObj1  ������
	*           linestringObj2  �߶���	
	*			[out]
	*			
	* @return	��������Ƿ�����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMultiPointLinestringDisjoint(const GTMultiPoint    *multipointObj1, 
										const GTLineString    *linestringObj2);

	/*****************************************************************************
	* @brief	����������Ƿ�����
	* @param	[in]
	*			multipointObj1  ������
	*           linestringObj2  �߶���	
	*			[out]
	*			
	* @return	����������Ƿ�����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMultiPointPolygonDisjoint(const GTMultiPoint    *multipointObj, 
							         const GTPolygon       *polygonObj);


	/*****************************************************************************
	* @brief	���������Ƿ�����
	* @param	[in]
	*			linestringObj   �߶���
	*           polygonObj      ����ζ���	
	*			[out]
	*			
	* @return	���������Ƿ�����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isLinestringPolygonDisjoint(const GTLineString  *linestringObj, 
							         const GTPolygon     *polygonObj);

	/*****************************************************************************
	* @brief	������������Ƿ�����
	* @param	[in]
	*			polygonObj1  ����ζ���
	*           polygonObj2  ����ζ���	
	*			[out]
	*			
	* @return	������������Ƿ�����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isPolygonsDisjoint(const GTPolygon  *polygonObj1, 
							const GTPolygon  *polygonObj2);

	/*****************************************************************************
	* @brief	�������Ƿ����
	* @param	[in]
	*			pointObj       �����
	*           linestringObj  �߶���	
	*			[out]
	*			
	* @return	�������Ƿ����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isPointLinestringTouch(const GTPoint       *pointObj, 
								const GTLineString  *linestringObj);

	/*****************************************************************************
	* @brief	��������Ƿ����
	* @param	[in]
	*			multipointObj  ������
	*           linestringObj  �߶���	
	*			[out]
	*			
	* @return	��������Ƿ����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMultiPointLinestringTouch(const GTMultiPoint  *multipointObj, 
									 const GTLineString  *linestringObj);

	/*****************************************************************************
	* @brief	���������Ƿ����
	* @param	[in]
	*			pointObj  ������
	*           polygonObj     ����ζ���	
	*			[out]
	*			
	* @return	���������Ƿ����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isPointPolygonTouch(const GTPoint    *pointObj, 
							 const GTPolygon  *polygonObj);
	/*****************************************************************************
	* @brief	����������Ƿ����
	* @param	[in]
	*			multipointObj  ������
	*           polygonObj     ����ζ���	
	*			[out]
	*			
	* @return	����������Ƿ����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMultiPointPolygonTouch(const GTMultiPoint  *multipointObj, 
								  const GTPolygon     *polygonObj);

	/*****************************************************************************
	* @brief	�������Ƿ����
	* @param	[in]
	*			linestringObj1  �߶���
	*           linestringObj2  �߶���	
	*			[out]
	*			
	* @return	�������Ƿ����
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isLinestringsTouch(const GTLineString  *linestringObj1, 
							const GTLineString  *linestringObj2);


	/*****************************************************************************
	* @brief	�������Ƿ��ཻ(cross)
	* @param	[in]
	*			multiPointObj  ������
	*           linestringObj  �߶���	
	*			[out]
	*			
	* @return	�������Ƿ��ཻ(cross)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isMulPointLinestringCross(const GTMultiPoint   *multiPointObj, 
								   const GTLineString   *linestringObj);

	/*****************************************************************************
	* @brief	�������Ƿ��ཻ(cross)
	* @param	[in]
	*			linestringObj1  �߶���
	*           linestringObj2  �߶���	
	*			[out]
	*			
	* @return	�������Ƿ��ཻ(cross)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-08
	*******************************************************************************/
	 bool isLinestringsCross(const GTLineString   *linestringObj1, 
							const GTLineString   *linestringObj2);

	/*****************************************************************************
	* @brief	����������Ƿ��ཻ(cross)
	* @param	[in]
	*			multipointObj1  ������
	*           polygonObj      ����ζ���	
	*			[out]
	*			
	* @return	����������Ƿ��ཻ(cross)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-14
	*******************************************************************************/
	 bool isMulPointsPolygonCross(const GTMultiPoint   *multipointObj1, 
								 const GTPolygon      *polygonObj);

	/*****************************************************************************
	* @brief	���������Ƿ��ཻ(cross)
	* @param	[in]
	*			linestringObj  �߶���
	*           polygonObj     ����ζ���	
	*			[out]
	*			
	* @return	����������Ƿ��Խ(cross)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-14
	*******************************************************************************/
	 bool isLinestringPolygonCross(const GTLineString   *linestringObj, 
								  const GTPolygon      *polygonObj);

	/*****************************************************************************
	* @brief	����Ƿ������(contain)
	* @param	[in]
	*			multipointObj  ������
	*           pointObj       �����	
	*			[out]
	*			
	* @return	����Ƿ������(contain)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isMulPointContainPoint(const GTMultiPoint   *multipointObj, 
								const GTPoint        *pointObj);


	/*****************************************************************************
	* @brief	����Ƿ�������(contain)
	* @param	[in]
	*			multipointObj1  ������
	*           multipointObj2  ������	
	*			[out]
	*			
	* @return	���multipointObj1�Ƿ�������multipointObj2(contain)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isMulPointContainMultiPoint(const GTMultiPoint   *multipointObj1, 
								     const GTMultiPoint   *multipointObj2);

	/*****************************************************************************
	* @brief	���Ƿ������(contain)
	* @param	[in]
	*			linestringObj  �߶���
	*           pointObj       �����	
	*			[out]
	*			
	* @return	���Ƿ������(contain)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isLinestringContainPoint(const GTLineString  *linestringObj, 
								  const GTPoint       *pointObj);

	/*****************************************************************************
	* @brief	���Ƿ�������(contain)
	* @param	[in]
	*			linestringObj  �߶���
	*           multipointObj  ������	
	*			[out]
	*			
	* @return	���Ƿ�������(contain)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isLinestringContainMultiPoint(const GTLineString  *linestringObj, 
								       const GTMultiPoint  *multipointObj);

	/*****************************************************************************
	* @brief	������Ƿ������(contain)
	* @param	[in]
	*			polygonObj  ����ζ���
	*           pointObj    �����	
	*			[out]
	*			
	* @return	������Ƿ������(contain)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPolygonContainPoint(const GTPolygon  *polygonObj, 
							   const GTPoint    *pointObj);

	/*****************************************************************************
	* @brief	������Ƿ�������(contain)
	* @param	[in]
	*			polygonObj     ����ζ���
	*           multipointObj  ������	
	*			[out]
	*			
	* @return	������Ƿ�������(contain)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPolygonContainMultiPoint(const GTPolygon    *polygonObj, 
							        const GTMultiPoint *multipointObj);

	/*****************************************************************************
	* @brief	���Ƿ������(contain)----δ���
	* @param	[in]
	*			polygonObj     ����ζ���
	*           multipointObj  ������	
	*			[out]
	*			
	* @return	������Ƿ�������(contain)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
/*
	 bool isLinestringContainLinestring(const GTLineString *linestringObj1, 
							           const GTLineString *linestringObj2);*/

	/*****************************************************************************
	* @brief	������Ƿ������(contain)
	* @param	[in]
	*			polygonObj     ����ζ���
	*           multipointObj  ������	
	*			[out]
	*			
	* @return	������Ƿ������(contain)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPolygonContainLinestring(const GTPolygon    *polygonObj, 
							        const GTLineString *linestringObj);

	/*****************************************************************************
	* @brief	������Ƿ���������(contain)
	* @param	[in]
	*			polygonObj1  ����ζ���
	*           polygonObj2  ����ζ���	
	*			[out]
	*			
	* @return	������Ƿ���������(contain)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPolygonContainPolygon(const GTPolygon  *polygonObj1, 
							     const GTPolygon  *polygonObj2);

	/*****************************************************************************
	* @brief	������Ƿ��ཻ(intersect)
	* @param	[in]
	*			pointObj1  �����
	*           pointObj2  �����	
	*			[out]
	*			
	* @return	������Ƿ��ཻ(intersect)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPointsIntersect(const GTPoint  *pointObj1, 
						   const GTPoint  *pointObj2);

	/*****************************************************************************
	* @brief	�������Ƿ��ཻ(intersect)
	* @param	[in]
	*			pointObj  �����
	*           multipointObj  �����	
	*			[out]
	*			
	* @return	�������Ƿ��ཻ(intersect)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPointMultiPointIntersect(const GTPoint       *pointObj, 
									const GTMultiPoint  *multipointObj);

	/*****************************************************************************
	* @brief	��������Ƿ��ཻ(intersect)
	* @param	[in]
	*			multipointObj2  ������
	*           multipointObj1  ������	
	*			[out]
	*			
	* @return	�������Ƿ��ཻ(intersect)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isMultiPointsIntersect(const GTMultiPoint  *multipointObj1,
								const GTMultiPoint  *multipointObj2);

	/*****************************************************************************
	* @brief	������Ƿ��ཻ(intersect)
	* @param	[in]
	*			pointObj       �����
	*           linestringObj  �߶���	
	*			[out]
	*			
	* @return	������Ƿ��ཻ(intersect)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPointLinestringIntersect(const GTPoint       *pointObj,
								    const GTLineString  *linestringObj);
	/*****************************************************************************
	* @brief	�������Ƿ��ཻ(intersect)
	* @param	[in]
	*			multipointObj       �����
	*           linestringObj  �߶���	
	*			[out]
	*			
	* @return	������Ƿ��ཻ(intersect)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isMultiPointLinestringIntersect(const GTMultiPoint  *multipointObj,
												const GTLineString  *linestringObj);
	
	/*****************************************************************************
	* @brief	���������Ƿ��ཻ(intersect)
	* @param	[in]
	*			pointObj    �����
	*           polygonObj  ����ζ���	
	*			[out]
	*			
	* @return	���������Ƿ��ཻ(intersect)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPointPolygonIntersect(const GTPoint    *pointObj,
										const GTPolygon  *polygonObj);
	
	/*****************************************************************************
	* @brief	����������Ƿ��ཻ(intersect)
	* @param	[in]
	*			multipointObj    ������
	*           polygonObj  �߶���	
	*			[out]
	*			
	* @return	����������Ƿ��ཻ(intersect)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isMultiPointPolygonIntersect(const GTMultiPoint  *multipointObj,
								      const GTPolygon     *polygonObj);

	/*****************************************************************************
	* @brief	�������Ƿ��ཻ(intersect)
	* @param	[in]
	*			linestringObj1  �߶���
	*           linestringObj2  �߶���	
	*			[out]
	*			
	* @return	�������Ƿ��ཻ(intersect)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isLinestringsIntersect(const GTLineString  *linestringObj1,
								const GTLineString  *linestringObj2);

	/*****************************************************************************
	* @brief	���������Ƿ��ཻ(intersect)
	* @param	[in]
	*			linestringObj  �߶���
	*           polygonObj  �߶���	
	*			[out]
	*			
	* @return	���������Ƿ��ཻ(intersect)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isLinestringsPolygonIntersect(const GTLineString  *linestringObj,
									   const GTPolygon     *polygonObj);

	/*****************************************************************************
	* @brief	������������Ƿ��ཻ(intersect)
	* @param	[in]
	*			polygonObj1  ����ζ���
	*           polygonObj2  ����ζ���	
	*			[out]
	*			
	* @return	������������Ƿ��ཻ(intersect)
	* @note		 
	* @author	����ƣ�zyk@lreis.ac.cn,2011-08-16
	*******************************************************************************/
	 bool isPolygonsIntersect(const GTPolygon  *polygonObj1,
							 const GTPolygon  *polygonObj2);

	/*RawPoint��Envelop*/
	enumGTSpatialRelation getRawPointAndEnvelopSR(
		const GTRawPoint* rawPointObj, 
		const GTEnvelop*  envelopObj);

	/*������Envelop*/
	enumGTSpatialRelation getPointAndEnvelopSR(
		const GTPoint*   pointObj, 
		const GTEnvelop* envelopObj);
	
	/*�߶���Envelop*/
	enumGTSpatialRelation getSegmentAndEnvelopSR(
		const GTLine*    lineObj,
		const GTEnvelop* envelopObj);

	/*����Envelop*/
	enumGTSpatialRelation getLineStringAndEnvelopSR(
		const GTLineString* lineStringObj,
		const GTEnvelop*    envelopObj);

	/*����Envelop*/
	enumGTSpatialRelation getRingAndEnvelopSR(
		const GTRing*    ringObj,
		const GTEnvelop* envelopObj);

	/*�������Envelop*/
	enumGTSpatialRelation getPolygonAndEnvelopSR(
		const GTPolygon* polygonObj,
		const GTEnvelop* envelopObj);
	
	/*�����뵥��*/
	enumGTSpatialRelation getPointAndPointSR(
		const GTPoint* pointObj1, 
		const GTPoint* pointObj2);
	
	/*�������߶�*/
	enumGTSpatialRelation getPointAndLineSR(
		const GTPoint* pointObj, 
		const GTLine*  lineObj);
	
	/*��������*/
	enumGTSpatialRelation getPointAndLineStringSR(
		const GTPoint*       pointObj, 
		const GTLineString* lineStringObj);

	/*�����뻷*/
	enumGTSpatialRelation getPointAndRingSR(
		const GTPoint*      pointObj, 
		const GTLineString* lineStringObj);

	/*����������*/
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
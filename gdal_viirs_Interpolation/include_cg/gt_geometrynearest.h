/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ���ζ���Ľ��ڷ������㡢�ߡ��桢�㼯���߼����漯��      
* Author:   ������, fanjf@lreis.ac.cn, 2011��-7��-19��
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* ��Ȩ����  (c) �й���ѧԺ�����ѧ����Դ�о���. ��������Ȩ��
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
	* @brief	������֮��ľ���
	* @param	[in]
	*			pointObj1	              �����
	*			pointObj2			      �����
	* @return	�ɹ���������֮��ľ��� ��ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
static double twoPointsDistance(
	const GTRawPoint* pointObj1,                            
	const GTRawPoint* pointObj2);

   	/*****************************************************************************
	* @brief	�㵽ֱ�������̾���
	* @param	[in]
	*			pointObj	              Ŀ������
	*			fromPoint			      ֱ���ϵ�����
	*			toPoint		               
	*       	[out]
	*			shortestPoint			  �����������ϵĵ�
	* @return	�ɹ����ص㵽ֱ�������̾��� ��ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToLine(
		const GTRawPoint* pointObj,
		const GTRawPoint* fromPoint,
		const GTRawPoint* toPoint,
		GTRawPoint*       shortestPoint);
	/*****************************************************************************
	* @brief	�㵽���������̾���
	* @param	[in]
	*			pointObj	              Ŀ������
	*			rayFromPoint			  ���ߵ���ʼ�˵�
	*			rayToPoint		          ������������ʼ�˵�����һ��
	*       	[out]
	*			shortestPoint			  �������������ϵĵ�
	* @return	�ɹ����ص㵽���������̾��� ��ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToRayDistance(
		const GTRawPoint* pointObj,
		const GTRawPoint* rayFromPoint,
		const GTRawPoint* rayToPoint,
		GTRawPoint*  shortestPoint);

	/*****************************************************************************
	* @brief	�㵽�߶ε���̾���
	* @param	[in]
	*			pointObj	              Ŀ������
	*			fromPoint			      �߶ε������˵�
	*			toPoint		              
	*       	[out]
	*			shortestPoint			  ���������߶��ϵĵ�
	* @return	�ɹ����ص㵽�߶ε���̾��� ��ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToSegment(
		const GTRawPoint* pointObj,
		const GTRawPoint* fromPoint,
		const GTRawPoint* toPoint,
		GTRawPoint*       shortestPoint);
	
	/*****************************************************************************
	* @brief	�㵽�ߴ�����̾���
	* @param	[in]
	*			pointObj	              Ŀ������
	*			lineStringObj			  �ߴ�����
	*       	[out]
	*			shortestPoint			  ���������ߴ��ϵĵ�
	* @return	�ɹ����ص㵽�ߴ�����̾��� ��ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToLineString(
		const GTRawPoint*   pointObj,
		const GTLineString* lineStringObj,
		GTRawPoint*         shortestPoint);

	/*****************************************************************************
	* @brief	�㵽���ߴ���̾���
	* @param	[in]
	*			pointObj	              Ŀ������
	*			pMultilineStringObj		  ���ߴ�����
	*           [out]
	*			shortestPoint			  �������ڶ��ߴ��ϵĵ�
	* @return	�ɹ����ص㵽�ߴ�����̾��� ��ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToMultiLineString(
		    const GTRawPoint*   pointObj,
		    GTMultiLineString* pMultilineStringObj,
		    GTRawPoint*        shortestPoint);

	/*****************************************************************************
	* @brief	�㵽����̾���
	* @param	[in]
	*			pointObj	              Ŀ������
	*			ringObj          		  ������
	*       	[out]
	*			shortestPoint			  �������ڻ��ϵĵ�
	* @return	�ɹ����ص㵽����̾��� ��ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToRing(
		const GTRawPoint* pointObj,
		GTRing*           ringObj,
		GTRawPoint*       shortestPoint);

	/*****************************************************************************
	* @brief	�㵽�������̾���
	* @param	[in]
	*			pointObj	              Ŀ������
	*			polygonObj          	  ����ζ���
	*       	[out]
	*			shortestPoint			  �������ڻ��ϵĵ�
	* @return	�ɹ����ص㵽�������̾��룬ʧ�ܷ���-1.0
	* @note		ֻ����������Ŵ��ڵ������εľ��룺
	*           1�ǵ��ڶ�����ڻ��� 2�ǵ��ڶ�����⻷��
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestPointToPolygon(
		const GTRawPoint* pointObj,
		GTPolygon*        polygonObj,
		GTRawPoint*       shortestPoint);

	/*****************************************************************************
	* @brief	�㵽�������̾���
	* @param	[in]
	*			pointObj	              Ŀ������
	*			polygonObj          	  �����ζ���
	*       	[out]
	*			shortestPoint			  �������ڻ��ϵĵ�
	* @return	�ɹ����ص㵽�������̾��룬ʧ�ܷ���-1.0
	* @note		ֻ����������Ŵ��ڵ������εľ��룺
	*           1�ǵ��ڶ�����ڻ��� 2�ǵ��ڶ�����⻷��
	* @author	�½���chenjian0511204@sina.com,2011-7-19
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
	* @brief	�߶ε��߶�֮����̾���
	* @param	[in]
	*			seg1_fpt��seg1_tpt	      �߶�1�ϵ�ʼĩ�˵�
	*			seg2_fpt��seg2_tpt        �߶�2�ϵ�ʼĩ�˵�
	*       	[out]
	*			startPt��endPt		      �������߶ε�ʼĩ�˵�
	* @return	�ɹ������߶ε��߶�֮����̾��룬ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static   double shortestSegmentToSegment(
		const GTRawPoint* seg1_fpt, 
		const GTRawPoint* seg1_tpt,
		const GTRawPoint* seg2_fpt, 
		const GTRawPoint* seg2_tpt,
		GTRawPoint*       startPt,
		GTRawPoint*       endPt );

	/*****************************************************************************
	* @brief	�߶ε���֮����̾���
	* @param	[in]
	*			seg_fpt��seg_tpt	      �߶��ϵ�ʼĩ�˵�
	*			ringObj                   ������
	*       	[out]
	*			startPt��endPt		      �������߶ε�ʼĩ�˵�
	* @return	�ɹ������߶ε���֮����̾��룬ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestSegmentToRing(
		const GTRawPoint* seg_fpt, 
		const GTRawPoint* seg_tpt,
		GTRing*           ringObj,
		GTRawPoint*       startPt,
		GTRawPoint*       endPt );

	/*****************************************************************************
	* @brief	�߶ε������֮����̾���
	* @param	[in]
	*			seg_fpt��seg_tpt	      �߶��ϵ�ʼĩ�˵�
	*			polygonObj                ����ζ���
	*       	[out]
	*			startPt��endPt		      ��������̾����߶ε�ʼĩ�˵�
	* @return	�ɹ������߶ε������֮����̾��룬ʧ�ܷ���-1.0
	* @note		ֻ����������Ŵ����߶������εľ��룺
	*           1���߶����ڻ��� 2���߶����⻷��
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static  double shortestSegmentToPolygon(
		const GTRawPoint* seg_fpt, 
		const GTRawPoint* seg_tpt,
		GTPolygon*        polygonObj,
		GTRawPoint*       startPt,
		GTRawPoint*       endPt
		);

	///*****************************************************************************
	//* @brief	�ߴ����ߴ�֮����̾���
	//* @param	[in]
	//*			pLineString				  �ߴ�����
	//*			pLineStringobj            ����ζ���
	//*       	[out]
	//*			startPt��endPt		      ��������̾����߶ε�ʼĩ�˵�
	//* @return	�ɹ������߶ε������֮����̾��룬ʧ�ܷ���-1.0
	//* @note		ֻ����������Ŵ����߶������εľ��룺
	//*           1���߶����ڻ��� 2���߶����⻷��
	//* @author	�½���chenjian0511204@sina.com,2011-7-19
	//*******************************************************************************/
	//static  double shortestLineStringToLinestring(
	//	GTLineString*		pLineString,
	//	GTLineString*		pLineStringobj,
	//	GTRawPoint*			startPt,
	//	GTRawPoint*			endPt
	//	);

	/*****************************************************************************
	* @brief	�ߴ��������֮����̾���
	* @param	[in]
	*			pLineString				  �ߴ�����
	*			polygonObj                ����ζ���
	*       	[out]
	*			startPt��endPt		      ��������̾����߶ε�ʼĩ�˵�
	* @return	�ɹ������߶ε������֮����̾��룬ʧ�ܷ���-1.0
	* @note		ֻ����������Ŵ����߶������εľ��룺
	*           1���߶����ڻ��� 2���߶����⻷��
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static  double shortestLineStringToPolygon(
		const GTLineString*	pLineString,
		GTPolygon*			polygonObj,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt
		);

	/*****************************************************************************
	* @brief	���ߴ��������֮����̾���
	* @param	[in]
	*			pLineString					���ߴ�����
	*			polygonObj                ����ζ���
	*       	[out]
	*			startPt��endPt		      ��������̾����߶ε�ʼĩ�˵�
	* @return	�ɹ������߶ε������֮����̾��룬ʧ�ܷ���-1.0
	* @note		ֻ����������Ŵ����߶������εľ��룺
	*           1���߶����ڻ��� 2���߶����⻷��
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static  double shortestMultiLineStringToPolygon(
		const GTMultiLineString*	pLineString,
		GTPolygon*					polygonObj,
		GTRawPoint*					startPt,
		GTRawPoint*					endPt
		);

	/*****************************************************************************
	* @brief	�ߴ���������֮����̾���
	* @param	[in]
	*			pLineString		  ���ߴ�����
	*			polygonObj                �����ζ���
	*       	[out]
	*			startPt��endPt		      ��������̾����߶ε�ʼĩ�˵�
	* @return	�ɹ������߶ε������֮����̾��룬ʧ�ܷ���-1.0
	* @note		ֻ����������Ŵ����߶������εľ��룺
	*           1���߶����ڻ��� 2���߶����⻷��
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static  double shortestLineStringToMultiPolygon(
		GTLineString*		pLineString,
		GTMultiPolygon*		polygonObj,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt
		);

	/*****************************************************************************
	* @brief	���ߴ���������֮����̾���
	* @param	[in]
	*			pLineString					���ߴ�����
	*			polygonObj                �����ζ���
	*       	[out]
	*			startPt��endPt		      ��������̾����߶ε�ʼĩ�˵�
	* @return	�ɹ������߶ε������֮����̾��룬ʧ�ܷ���-1.0
	* @note		ֻ����������Ŵ����߶������εľ��룺
	*           1���߶����ڻ��� 2���߶����⻷��
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static  double shortestMultiLineStringToMultiPolygon(
		GTMultiLineString*	pLineString,
		GTMultiPolygon*		polygonObj,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt
		);



	/*****************************************************************************
	* @brief	��pFPointsFeatureLayer�㼯�е�ÿ���㵽pTPointsFeatureLayer�㼯��
	*           ������̵ĵ㣬�ٽ���һ����ͼ�㱣�������������
	* @param	[in]
	*			pFPointsFeatureLayer	             �����ݲ�
	*			pTPointsFeatureLayer			     �����ݲ�
	*       	[out]
	*			pResultLineFeatureLayer              ������ɵ���̾�������ͼ��
	*                                                pResultLineFeatureLayer��enumGTWKBGeometryTypeΪGT_WKBLineString
	* @return	�ɹ�����true ��ʧ�ܷ���false
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool shortestPointsToPoints(
		GTFeatureLayer *pFPointsFeatureLayer,
		GTFeatureLayer *pTPointsFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);

	/*****************************************************************************
	* @brief	��㼯�е�ÿ���㵽�߼��е���̵�����,������㵽���Ͼ�����̵ĵ㣬
	*			�ٽ���һ����ͼ�㱣�������������
	* @param	[in]
	*			pPointsFeatureLayer	                 �����ݲ�
	*			pLinesFeatureLayer			         �����ݲ�
	*       	[out]
	*			pResultLineFeatureLayer              ������ɵ���̾�������ͼ��
	*                                                pResultLineFeatureLayer��enumGTWKBGeometryTypeΪGT_WKBLineString
	* @return	�ɹ�����true ��ʧ�ܷ���false
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool shortestPointsToLines(
		GTFeatureLayer *pPointsFeatureLayer,
		GTFeatureLayer *pLinesFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);

	/*****************************************************************************
	* @brief	��㼯�е�ÿ���㵽�����е���̵ľ���,������㵽���Ͼ�����̵ĵ㣬
	*			�ٽ���һ����ͼ�㱣�������������
	* @param	[in]
	*			pPointsFeatureLayer	                 �����ݲ�
	*			pRingsFeatureLayer			         �����ݲ�
	*       	[out]
	*			pResultLineFeatureLayer              ������ɵ���̾�������ͼ��
	*                                                pResultLineFeatureLayer��enumGTWKBGeometryTypeΪGT_WKBLineString
	* @return	�ɹ�����true ��ʧ�ܷ���false
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool  shortestPointsToRings(
				GTFeatureLayer *pPointsFeatureLayer,
				GTFeatureLayer *pRingsFeatureLayer,
				GTFeatureLayer *pResultLineFeatureLayer);
		    
	/*****************************************************************************
	* @brief	��㼯�е�ÿ���㵽�漯�е���̵ľ���,������㵽���Ͼ�����̵ĵ㣬
	*			�ٽ���һ����ͼ�㱣�������������
	* @param	[in]
	*			pPointsFeatureLayer	                 �����ݲ�
	*			pPolygonsFeatureLayer			     �����ݲ�
	*       	[out]
	*			pResultLineFeatureLayer              ������ɵ���̾�������ͼ��
	*                                                pResultLineFeatureLayer��enumGTWKBGeometryTypeΪGT_WKBLineString
	* @return	�ɹ�����true ��ʧ�ܷ���false
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool  shortestPointsToPolygons(
		GTFeatureLayer *pPointsFeatureLayer,
		GTFeatureLayer *pPolygonsFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);


	/*****************************************************************************
	* @brief	���߼��е�ÿ���ߵ��㼯�е���̵ľ���,��������ϵĵ㵽�㼯�Ͼ�����̵ĵ㣬
	*			�ٽ���һ����ͼ�㱣�������������
	* @param	[in]
	*			pLinesFeatureLayer	                 �����ݲ�
	*			pPointsFeatureLayer			         �����ݲ�
	*       	[out]
	*			pResultLineFeatureLayer              ������ɵ���̾�������ͼ��
	*                                                pResultLineFeatureLayer��enumGTWKBGeometryTypeΪGT_WKBLineString
	* @return	�ɹ�����true ��ʧ�ܷ���false
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool  shortestLinesToPoints(
		GTFeatureLayer *pLinesFeatureLayer,
		GTFeatureLayer *pPointsFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);

	/*****************************************************************************
	* @brief	��F�߼��е�ÿ����FL��T�߼��е���TL��̵ľ���,�����FL���ϵĵ㵽TL���Ͼ�����̵ĵ㣬
	*			�ٽ���һ����ͼ�㱣�������������
	* @param	[in]
	*			pLinesFeatureLayer	                 �����ݲ�
	*			pTLinesFeatureLayer			         �����ݲ�
	* @param	[out]
	*			pResultLineFeatureLayer              ������ɵ���̾�������ͼ��
	*                                                pResultLineFeatureLayer��enumGTWKBGeometryTypeΪGT_WKBLineString
	* @return	�ɹ�����true ��ʧ�ܷ���false
	* @note		�������һ��F�߼��������T�߼�����ߵ���̾������ͼ��
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool shortestLinesToLines(
		GTFeatureLayer *pFLinesFeatureLayer,
		GTFeatureLayer *pTLinesFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);

	/*****************************************************************************
	* @brief	���߼��е�ÿ���ߵ������е���̵ľ��룬�������̾����ߵĵ㵽��̾��뻷�Ͼ�����̵ĵ㣬
	*			�ٽ���һ����ͼ�㱣�������������
	* @param	[in]
	*			pLinesFeatureLayer	                 �����ݲ�
	*			pRingsFeatureLayer			         �����ݲ�
	* @param	[out]
	*			pResultLineFeatureLayer              ������ɵ���̾�������ͼ��
	*                                                pResultLineFeatureLayer��enumGTWKBGeometryTypeΪGT_WKBLineString
	* @return	�ɹ�����true ��ʧ�ܷ���false
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool shortestLinesToRings(
		GTFeatureLayer *pLinesFeatureLayer,
		GTFeatureLayer *pRingsFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);
	/*****************************************************************************
	* @brief	���߼��е�ÿ���ߵ��漯�е���̵ľ��룬�������̾����ߵĵ㵽��̾������Ͼ�����̵ĵ㣬
	*			�ٽ���һ����ͼ�㱣�������������
	* @param	[in]
	*			pLinesFeatureLayer	                 �����ݲ�
	*			pPolygonFeatureLayer			     ��������ݲ�
	* @param	[out]
	*			pResultLineFeatureLayer              ������ɵ���̾�������ͼ��
	*                                                pResultLineFeatureLayer��enumGTWKBGeometryTypeΪGT_WKBLineString
	* @return	�ɹ�����true ��ʧ�ܷ���false
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static bool  shortestLinesToPolygons(
		GTFeatureLayer *pLinesFeatureLayer,
		GTFeatureLayer *pPolygonFeatureLayer,
		GTFeatureLayer *pResultLineFeatureLayer);



	/*****************************************************************************
	* @brief	�㵽������̾���
	* @param	[in]
	*			pointObj	              Ŀ������
	*			lineStringObj			  �ߴ�����
	*       	[out]
	*			shortestPoint			  �������ڶ���ϵĵ�
	* @return	�ɹ����ص㵽������̾��� ��ʧ�ܷ���-1.0
	* @author	��������fanjf@lreis.ac.cn,2012-8-22
	*******************************************************************************/
	static double shortestPointToMultiPoint(
		const GTRawPoint*   pointObj,
		const GTMultiPoint* multiPointObj,
		GTRawPoint*         shortestPoint);

	/*****************************************************************************
	* @brief	�㵽������̾���
	* @param	[in]
	*			multiPointObjA            Ŀ�������A
	*			multiPointObjB			  Ŀ�������B
	*       	[out]
	*			shortestPointA			  �������ڶ��A�ϵĵ�
	*			shortestPointB			  �������ڶ��B�ϵĵ�
	* @return	�ɹ����ص㵽������̾��� ��ʧ�ܷ���-1.0
	* @author	��������fanjf@lreis.ac.cn,2012-8-22
	*******************************************************************************/
	static double shortestMultiPointToMultiPoint(
		const GTMultiPoint*   multiPointObjA,
		const GTMultiPoint* multiPointObjB,
		GTRawPoint*         shortestPointA,
		GTRawPoint*         shortestPointB);


	/*****************************************************************************
	* @brief	�߶ε��ߴ�֮����̾���
	* @param	[in]
	*			seg1_fpt��seg1_tpt	      �߶�1�ϵ�ʼĩ�˵�
	*			lineStr				      �ߴ�
	*       	[out]
	*			startPt��endPt		      �������߶ε�ʼĩ�˵�
	* @return	�ɹ������߶ε��ߴ�֮����̾��룬ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
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
	* @brief	�ߴ����ߴ�֮����̾���
	* @param	[in]
	*			lineStrA		�ߴ�A
	*			lineStrB		�ߴ�B
	*       	[out]
	*			startPt��endPt		      �������߶ε�ʼĩ�˵�
	* @return	�ɹ������߶ε��ߴ�֮����̾��룬ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestLineStringToLineString(
		const GTLineString* lineStrA,
		const GTLineString* lineStrB,
		GTRawPoint*			startPt,
		GTRawPoint*			endPt );

	/*****************************************************************************
	* @brief	�ߴ������ߴ�֮����̾���
	* @param	[in]
	*			lineStr				�ߴ�A
	*			multiLineStr		���ߴ�B
	*       	[out]
	*			startPt��endPt		      �������߶ε�ʼĩ�˵�
	* @return	�ɹ�����2��֮����̾��룬ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestLineStringToMultiLineString(
		const GTLineString*		 lineStr,
		const GTMultiLineString* multiLineStr,
		GTRawPoint*				 startPt,
		GTRawPoint*				 endPt);

	/*****************************************************************************
	* @brief	���ߴ������ߴ�֮����̾���
	* @param	[in]
	*			multiLineStrA		�ߴ�A
	*			multiLineStrB		���ߴ�B
	*       	[out]
	*			startPt��endPt		      �������߶ε�ʼĩ�˵�
	* @return	�ɹ�����2��֮����̾��룬ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestMultiLineStringToMultiLineString(
		const GTMultiLineString* multiLineStrA,
		const GTMultiLineString* multiLineStrB,
		GTRawPoint*				 startPt,
		GTRawPoint*				 endPt);


	/*****************************************************************************
	* @brief	��㵽�ߴ�����̾���
	* @param	[in]
	*			multiPointObj             Ŀ�������
	*			lineStringObj			  �ߴ�����
	*       	[out]
	*			shortestPointA			  �������ڶ���ϵĵ�
	*			shortestPointB			  ���������ߴ��ϵĵ�
	* @return	�ɹ����ص㵽�ߴ�����̾��� ��ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestMultiPointToLineString(
		const GTMultiPoint* multiPointObj,
		const GTLineString* lineStringObj,
		GTRawPoint*         shortestPointA,
		GTRawPoint*         shortestPointB);

	/*****************************************************************************
	* @brief	��㵽���ߴ�����̾���
	* @param	[in]
	*			mPointObj				Ŀ�������
	*			mLineStringObj			���ߴ�����
	*       	[out]
	*			shortestPointA			  �������ڶ���ϵĵ�
	*			shortestPointB			  ���������ߴ��ϵĵ�
	* @return	�ɹ����ص㵽�ߴ�����̾��� ��ʧ�ܷ���-1.0
	* @author	�½���chenjian0511204@sina.com,2011-7-19
	*******************************************************************************/
	static double shortestMultiPointToMultiLineString(
		const GTMultiPoint*		mPointObj,
		const GTMultiLineString* mLineStringObj,
		GTRawPoint*         shortestPointA,
		GTRawPoint*         shortestPointB);





};

}/*namespace gts{*/
#endif

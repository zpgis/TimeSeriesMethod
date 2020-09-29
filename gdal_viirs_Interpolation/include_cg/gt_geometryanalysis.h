/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ���ζ������������
*           ֻ�ṩ��Ե�������ķ���
* Author:   ������, fanjf@lreis.ac.cn 2010��-12��-08��
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* ��Ȩ����   (c) �й���ѧԺ�����ѧ����Դ�о���. ��������Ȩ��
****************************************************************/
#ifndef GTGEOMETRYANALYSIS_H_INCLUDED
#define GTGEOMETRYANALYSIS_H_INCLUDED

#include "gt_geometry.h"
#include "gpc.h"
#include "gts_cg_port.h"
#include <math.h>

namespace gts{

/*
 *���η���������
 */
class GTS_DLL GTGeometryAnalysis
{
public:
	GTGeometryAnalysis(void);
	virtual ~GTGeometryAnalysis(void);
};

/*
 *�����ļ��㼸����
 */
class GTS_DLL GTCPGeometryBasic: public GTGeometryAnalysis
{
public:
	GTCPGeometryBasic(void);
	~GTCPGeometryBasic(void);

	/************************************************************************/
	/*     �ռ伸�η����Ļ�������                                                  */
	/*     ��̬��������ӿ�                                                        */
	/*     -1������ʶ����Ĳ������                                              */
	/************************************************************************/

	/*@01 ����ƽ�����*/
	static double twoPointsDistance(
		const GTRawPoint* pointObj1,                            
		const GTRawPoint* pointObj2);

	/*@02 �����Բ������(����), ʹ�����Ҷ�����, ����С���������*/
	static double twoPointsGreateCircle1(
		const GTRawPoint* pointObj1,                                 
		const GTRawPoint* pointObj2);

	/*@03 �����Բ������(����), ʹ��Vincenty����, ���Ծ�ȷ����С����*/
	static double twoPointsGreateCircle2(
		const GTRawPoint* pointObj1,                                 
		const GTRawPoint* pointObj2);

	/*@04 �����߶εľ��� (!!!!!!���ĺ���, �������κ��޸�!!!!!!)*/
	static double pointSegmentDistance(
		const GTRawPoint* pointObj,
		const GTRawPoint* segFromPoint,			  
		const GTRawPoint* segToPoint);

	/*@05 �����ߵľ���*/
	static double pointLineDistance(
		const GTRawPoint* pointObj,
		const GTRawPoint* lineFromPoint,		
		const GTRawPoint* lineToPoint);

	/*@06 �������ߵľ���*/
	static double pointRayDistance(
		const GTRawPoint* pointObj,
		const GTRawPoint* rayFromPoint,
		const GTRawPoint* rayToPoint);

	/*@07 ���������, ��ֵ˳ʱ��, ��ֵ��ʱ��, 0����*/
	static double triangleArea(
		const GTRawPoint* pointObj1, 
		const GTRawPoint* pointObj2,
		const GTRawPoint* pointObj3);

	/*@08 ���Ƿ����ߵ����, 0����, 1���, -1�Ҳ�*/
	static int isPointLeftOfLine(
		const GTRawPoint* pointObj,
		const GTRawPoint* lineFromPoint,
		const GTRawPoint* lineToPoint,
		const double&     dTolerance);
	
	/*@09 �����Ƿ��� 1����, 0������*/
	static int threePointsCollinear(
		const GTRawPoint* pointObj1, 
		const GTRawPoint* pointObj2,
		const GTRawPoint* pointObj3,
		const double&     dTolerance);

	/*@10 ���Ƿ������� 1��, 0��*/
	static int isPointOnLine(
		const GTRawPoint* pointObj, 
		const GTRawPoint* lineFromPoint, 
		const GTRawPoint* lineToPoint,
		const double&     dTolerance);
	
	/*@11 ���Ƿ����߶��� 1��, 0��*/
	static int isPointOnSegment(
		const GTRawPoint* pointObj, 
		const GTRawPoint* segFromPoint, 
		const GTRawPoint* segToPoint,
		const double&     dTolerance);
	
	/*@12 ���Ƿ��������� 1��, 0��*/
	static int isPointOnRay(
		const GTRawPoint* pointObj, 
		const GTRawPoint* rayFromPoint, 
		const GTRawPoint* rayToPoint,
		const double&     dTolerance);

	/*@13 ���Ƿ���Envelop�� 1��, 0��*/
	static int isPointWithinEnvelop(
		const GTRawPoint* pointObj, 
		const GTEnvelop*  envelopObj);

	/*@14 ���Ƿ���Linestring�� 1��, 0��*/
	static int isPointOnLineString(
		const GTRawPoint* pointObj, 
		GTLineString*     lineStringObj,
		const double&     dTolerance);

	/*@15 ���Ƿ���Ring�� 1��, 0��*/
	static int isPointOnRing(
		const GTRawPoint* pointObj, 		 
		GTRing*           ringObj, 
		const double&     dTolerance);

	/*@16 ���Ƿ���Ring�� 1��, 0��, crossing number����
	 *(���ĺ���, �������κ��޸�)*/
	static int isPointWithinRingCN(
		const GTRawPoint* pointObj, 
		GTRing*           ringObj);

	/*@17 ���Ƿ���Ring�� 1��, 0��, winding number����*/
	static int isPointWithinRingWN(
		const GTRawPoint* pointObj,
		GTRing*           ringObj);


    /*@18 �������, 0�˻�, ��ֵ˳ʱ��, ��ֵ��ʱ��
	 *(!!!!!!���ĺ���, �������κ��޸�!!!!!!)
	 */
    static double ringArea(GTRing* ringObj);

	/*@19 ����н�[0, pi]*/
	static double threePointsIncludedAngle(
		const GTRawPoint* pointObj1, 
		const GTRawPoint* pointObj2,
		const GTRawPoint* pointObj3);

	/*@20 �߶����߶��ཻ
	 *(!!!!!!���ĺ���, �������κ��޸�!!!!!!)
	 * ����ֵ����:(��������pt_1��pt_2��)
	 *-2:�˻�Ϊ��ͬ��; -1:�˻�Ϊ��ͬ��; 
	 * 0:����;         1:�ཻ; 
	 * 2:�����ص�;      3:ƽ�в�����;
	 * 4:���߲��ཻ;    5:������һ��������
	 */
    static int segmentIntersect(
		const GTRawPoint& seg1_fpt, 
		const GTRawPoint& seg1_tpt,
		const GTRawPoint& seg2_fpt, 
		const GTRawPoint& seg2_tpt,
		GTRawPoint&       pt_1, 
		GTRawPoint&       pt_2);

    /*@21 �����߶���x������֮��ļн�, [0, 2pi)*/
	static double segmentXAxisAngle(
		const GTRawPoint* fromPoint, 
		const GTRawPoint* toPoint);

	/*@22 ���Ƿ��ڱպϵ��ߴ���, 1��, 0��, -1����*/
	static int isPointWithinClosedLineCN(
		const GTRawPoint   *pointObj,
		const GTLineString *lineObj);

	/*@23 ��ȡring�ڵ�һ����*/
	static bool getPointInRing(
		const GTRing *ringObj,
		GTRawPoint   &inPoint);

	/*@24 ��ȡ������ڵ�һ����*/
	static bool getPointInClosedLine(
		const GTLineString *lineObj,
		GTRawPoint         &inPoint);

	/*@25 ���Ƿ����ཻ�����ڲ����ص�*/
	static bool isLineStringWeird(
		const GTLineString *lineObj);

	/*@26 ���Ƿ����ཻ�����ڲ����ص�*/
	static bool isRingWeird(
		const GTRing *ringObj);

	/*@27 ����Centroid*/
	static bool ringCentroid(GTRing *pRing, GTRawPoint& cenPt);

	/*@28 �ߵ�centroid*/
	static bool lineStringCentroid(GTLineString *pLineString, GTRawPoint& cenPt);

	/*@29 �ж�3���Ƿ���˳ʱ��*/
	static bool isClockWise(
		const GTRawPoint &pnt1,
		const GTRawPoint &pnt2,
		const GTRawPoint &pnt3);

	/*@30 �߶��Ƿ��ཻ*/
	static bool isSegmentsIntersect(
		const GTRawPoint& seg1_fpt, 
		const GTRawPoint& seg1_tpt,
		const GTRawPoint& seg2_fpt, 
		const GTRawPoint& seg2_tpt);

	/*@31 �������*/
	//Area of Trapezoid order is: leftdown -->>> rightup (longitude, latitide)
	static double TrapezoidAreaWGS84(const double& lon1, const double& lat1, 
		                             const double& lon2, const double& lat2);
};

/*
 *�����Ŀռ��ϵ������
 */
class GTS_DLL GTSRGeometryBasic: public GTGeometryAnalysis{

public:
	GTSRGeometryBasic(void);
	~GTSRGeometryBasic(void);

	/*�㵽ֱ����̵�, 
	 *���ؾ���, 0.0Ϊ����, 9999Ϊ����, 
	 *�ο�pointSegmentDistance
	 */
	static double shortestPointToLine(
		const GTRawPoint& pointObj,
		const GTRawPoint& fromPoint,
		const GTRawPoint& toPoint,
		GTRawPoint&       shortestPoint);

	/*�㵽�߶���̵�
	 *�ο�pointSegmentDistance
	 */
	static double shortestPointToSegment(
		const GTRawPoint& pointObj,
		const GTRawPoint& fromPoint,
		const GTRawPoint& toPoint,
		GTRawPoint&       shortestPoint);
	
	/*�㵽�ߴ���̵�*/
	static double shortestPointToLineString(
		const GTRawPoint&   pointObj,
		const GTLineString& lineStringObj,
		GTRawPoint&         shortestPoint);
	
	/*�㵽����̵�*/
	static double shortestPointToRing(
		const GTRawPoint& pointObj,
		const GTRing&     ringObj,
		GTRawPoint&       shortestPoint);

	/*��Ҫ��չ����������֮��*/

public:
	/*�ж�һ�������Ƿ���������ڲ�, �����е�������ڲ�, 
     *9999:���ݴ���, 0:����, 1: �ڱ�Ե��\�ڲ�\�ڽ�,
	 *�������ռ����̶ȼ���, Ĭ��Ϊ0.0*/
	
	/*����Envelop�ڲ�, �����ڱ���*/
	static int isPointWithinEnvelop(
		const GTRawPoint& pointObj,                               
		const GTEnvelop&  envelopObj);

	/*�߶���Envelop�ڲ�, �����ڽ�*/
	static int isSegmentWithinEnvelop(
		const GTRawPoint& fromPoint, 
		const GTRawPoint& toPoint, 
		const GTEnvelop&  envelopObj);

	/*Envelop1��Envelop2�ڲ�, ������Ⱥ��ڽ�*/
	static int isEnvelopWithinEnvelop(
		const GTEnvelop& envelopObj1, 
		const GTEnvelop& envelopObj2);

	/*�ߴ���Envelop�ڲ�, �����ڽ�*/
	static int isLineStringWithinEnvelop(
		const GTLineString& lineStringObj, 
		const GTEnvelop&    envelopObj);

	/*����Envelop�ڲ�*/
	static int isRingWithinEnvelop(
		const GTRing&    ringObj, 
		const GTEnvelop& envelopObj);

	/*���ڻ��Ϻ��ڲ�, �����ڱ���
	 *���жϵ��Ƿ��ھ����ڻ���,   __isPointWithinEnvelop
	 *���ж��Ƿ�����,             isPointWithinRingCN
	 *����ж��Ƿ�����,           isPointOnRing
	 */
	static int isPointWithinRing(
		const GTRawPoint& pointObj, 
		const GTRing&     ringObj);
	
	/*�߶��ڻ��ڲ�, �����ڽ�*/
	static int isSegmentWithinRing(
		const GTRawPoint& fromPoint, 
		const GTRawPoint& toPoint, 
		const GTRing&     ringObj);

	/*Envelop�ڻ��ڲ�, �����ڽ�*/
	static int isPointWithinEnvelop(
		const GTEnvelop& envelopObj, 
		const GTRing&    ringObj);

	/*�ߴ��ڻ��ڲ�, �����ڽ�*/
	static int isLineStringWithinRing(
		const GTLineString& lineStringObj, 
		const GTRing&       ringObj);

	/*��1�ڻ�2�ڲ�, �����ڽӺ�ȫ��(���Ͻ��������� by zhou yuke)*/
	static int isRingWithinRing(
		const GTRing* ringObj1, 
		const GTRing* ringObj2);

	/*��Ҫ��չ����������֮���Ƿ����ڲ�*/

	/*��Ҫ��չ����������֮���Ƿ��ཻ*/

	/*��Ҫ��չ����������֮��...*/

};

/*
 *������Boolean������
 */
class GTS_DLL GTBLGeometryBasic: public GTGeometryAnalysis
{
public:
	GTBLGeometryBasic(void);
	~GTBLGeometryBasic(void);

	/*���뻷����
	 *���еĵ��ĵ��˳���Ѿ���֤���, ����-1Ϊʧ��*/
	static int twoRingBoolean(
		const GTRing*     ringObj1, 
		const GTRing*     ringObj2,
		enumGTBooleanType blType,
		GTMultiPolygon*   pPolygon);

	/*������������β���*/
	static int twoMultiPolygonBoolean(
		const GTMultiPolygon* mpolyObj1, 
		const GTMultiPolygon* mpolyObj2,
		enumGTBooleanType     blType,
		GTMultiPolygon*       pPolygon);

	/*ring, polygon, multipolygon֮���Boolean
	 *����GTGeometryָ��, �ⲿ����GTS_DELETE�ͷ�
	 *���islandCheck����Ϊfalse, ���ɷ�����һϵ��ring���ɵ�GTPolygon,
	 *���ı�־��island_mark��, ���ҷ���û�е���
	 *�ⲿ��Ϊ˳ʱ��, �ڲ�Ϊ��ʱ��. ���islandCheck
	 */
 	static GTGeometry* twoSurfaceObjectsBoolean(
		const GTGeometry* pGeometry1,
		const GTGeometry* pGeometry2,
		enumGTBooleanType blType,
		std::vector<int>& island_mark,  /*���ı�־, ���islandCheck=true, ����Ч*/
		bool bEnvelopCheck = false,     /*�Ƿ���Envelop�Ĺ�ϵ, �������Ϊtrue, ���������������, �򷵻�NULL*/
		bool islandCheck   = false);    /*���ڽ���Ƿ���е�����, ���жϵ����ڵĶ����, ������Polygon����*/

	/*std::vector<GTRing*>֮���boolean����, ���ػ��ĸ���*/
	static int twoSurfaceObjectsBoolean(
		std::vector<GTRing *> &ring_list_in_1,  /*���뻷�ļ���1*/
		std::vector<int>      &hole_list_in_1,  /*���ļ���1���ı�־*/
		int                    ring_numb_in_1,
		std::vector<GTRing *> &ring_list_in_2,  /*���뻷�ļ���2*/
		std::vector<int>      &hole_list_in_2,  /*���ļ���2���ı�־*/
		int                    ring_numb_in_2,
		enumGTBooleanType      blType,
		std::vector<GTRing *> &ring_list_out,   /*������ļ���, ע���ⲿָ����ͷ�*/
		std::vector<int>      &hole_list_out);  /*������ļ��ϵ��ı�־*/

private:
	static void         __free_gpcpolyon(gpc_polygon *p);
	static gpc_polygon *__malloc_gpcpolygon(int ringCount, int *pointCountList);
};

/*
 *������FeatureBuffer��
 */
class GTS_DLL GTFEBufferBasic: public GTGeometryAnalysis
{
public:
	GTFEBufferBasic(void);
	~GTFEBufferBasic(void);

	/*���Buffer*/
	static int pointBuffer(
		double dX, 
		double dY, 
	    double dR, 
		GTMultiPolygon* multiPolygon);
	
	/*����ָ��, �ⲿ�ͷ�*/
	static GTRing *pointBuffer(
		const GTPoint *pPointObj,
		double         dR);

	/**************************************************************************
	 *���º�����Ҫ������������㷨*
	 **************************************************************************/
	 /*�ߴ�Buffer, ���ض�������֯�Ľṹ*/
	static int lineStringBuffer(
		const GTLineString* lineStringObj, 
		double              dR, 
		GTMultiPolygon*     multiPolygon);

	/*����ָ������, �ⲿ�ͷ�*/
	static int lineStringBuffer(
		const GTLineString*    lineStringObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*��Buffer(������)*/
	static int ringBufferFace(
		const GTRing*   ringObj, 
		double          dR, 
		GTMultiPolygon* multiPolygon);

	/*��Buffer(������)*/
	static int ringBufferEdge(
		const GTRing*   ringObj, 
		double          dR, 
		GTMultiPolygon* multiPolygon);

	/*�����Buffer(������)
	 *���ڶ���ο����е�����, ��Ҫɾ�����ڲ�����
	 */
	static int polygonBufferFace(
		const GTPolygon* polygonObj, 
		double           dR, 
		GTMultiPolygon*  multiPolygon);
	
	/*���Buffer*/
	static int multiPointBuffer(
		const GTMultiPoint* multiPointObj, 
		double              dR, 
		GTMultiPolygon*     multiPolygon);

	/*����Buffer*/
	static int multiLineStringBuffer(
		const GTMultiLineString* multiLineStringObj,                 
		double                   dR, 
		GTMultiPolygon*          multiPolygon);

	/*������Buffer*/
	static int multiPolygonBufferFace(
		const GTMultiPolygon* multiPolygonObj,                  
		double                dR, 
		GTMultiPolygon*       multiPolygon);

	/*GTGeometryBag��Buffer*/
	static int geometryBagBuffer(
		const GTGeometryBag *geometryBagObj,
		double               dR, 
		GTMultiPolygon*      multiPolygon);

protected:
	/*��Buffer*/
	static int __pointBuffer(
		double  dX, 
		double  dY, 
		double  dR, 
		GTRing* ringObj);

	/*�߶�Buffer(Բ�ζ˵�)*/
	static int __segmentBuffer(
		double  dX1, 
		double  dY1,
		double  dX2, 
		double  dY2, 
		double  dR, 
		GTRing* ringObj);
};

}
#endif
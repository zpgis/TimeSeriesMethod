/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  几何对象分析处理类
*           只提供针对单个对象的分析
* Author:   范俊甫, fanjf@lreis.ac.cn 2010年-12月-08日
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* 版权所有   (c) 中国科学院地理科学与资源研究所. 保留所有权利
****************************************************************/
#ifndef GTGEOMETRYANALYSIS_H_INCLUDED
#define GTGEOMETRYANALYSIS_H_INCLUDED

#include "gt_geometry.h"
#include "gpc.h"
#include "gts_cg_port.h"
#include <math.h>

namespace gts{

/*
 *几何分析抽象类
 */
class GTS_DLL GTGeometryAnalysis
{
public:
	GTGeometryAnalysis(void);
	virtual ~GTGeometryAnalysis(void);
};

/*
 *基础的计算几何类
 */
class GTS_DLL GTCPGeometryBasic: public GTGeometryAnalysis
{
public:
	GTCPGeometryBasic(void);
	~GTCPGeometryBasic(void);

	/************************************************************************/
	/*     空间几何分析的基础函数                                                  */
	/*     静态函数对外接口                                                        */
	/*     -1经常标识错误的操作结果                                              */
	/************************************************************************/

	/*@01 两点平面距离*/
	static double twoPointsDistance(
		const GTRawPoint* pointObj1,                            
		const GTRawPoint* pointObj2);

	/*@02 两点大圆弧距离(公里), 使用余弦定理方程, 计算小距离有误差*/
	static double twoPointsGreateCircle1(
		const GTRawPoint* pointObj1,                                 
		const GTRawPoint* pointObj2);

	/*@03 两点大圆弧距离(公里), 使用Vincenty方程, 可以精确计算小距离*/
	static double twoPointsGreateCircle2(
		const GTRawPoint* pointObj1,                                 
		const GTRawPoint* pointObj2);

	/*@04 点与线段的距离 (!!!!!!核心函数, 不允许任何修改!!!!!!)*/
	static double pointSegmentDistance(
		const GTRawPoint* pointObj,
		const GTRawPoint* segFromPoint,			  
		const GTRawPoint* segToPoint);

	/*@05 点与线的距离*/
	static double pointLineDistance(
		const GTRawPoint* pointObj,
		const GTRawPoint* lineFromPoint,		
		const GTRawPoint* lineToPoint);

	/*@06 点与射线的距离*/
	static double pointRayDistance(
		const GTRawPoint* pointObj,
		const GTRawPoint* rayFromPoint,
		const GTRawPoint* rayToPoint);

	/*@07 三角形面积, 负值顺时针, 正值逆时针, 0共线*/
	static double triangleArea(
		const GTRawPoint* pointObj1, 
		const GTRawPoint* pointObj2,
		const GTRawPoint* pointObj3);

	/*@08 点是否在线的左侧, 0共线, 1左侧, -1右侧*/
	static int isPointLeftOfLine(
		const GTRawPoint* pointObj,
		const GTRawPoint* lineFromPoint,
		const GTRawPoint* lineToPoint,
		const double&     dTolerance);
	
	/*@09 三点是否共线 1共线, 0不共线*/
	static int threePointsCollinear(
		const GTRawPoint* pointObj1, 
		const GTRawPoint* pointObj2,
		const GTRawPoint* pointObj3,
		const double&     dTolerance);

	/*@10 点是否在线上 1是, 0否*/
	static int isPointOnLine(
		const GTRawPoint* pointObj, 
		const GTRawPoint* lineFromPoint, 
		const GTRawPoint* lineToPoint,
		const double&     dTolerance);
	
	/*@11 点是否在线段上 1是, 0否*/
	static int isPointOnSegment(
		const GTRawPoint* pointObj, 
		const GTRawPoint* segFromPoint, 
		const GTRawPoint* segToPoint,
		const double&     dTolerance);
	
	/*@12 点是否在射线上 1是, 0否*/
	static int isPointOnRay(
		const GTRawPoint* pointObj, 
		const GTRawPoint* rayFromPoint, 
		const GTRawPoint* rayToPoint,
		const double&     dTolerance);

	/*@13 点是否在Envelop内 1是, 0否*/
	static int isPointWithinEnvelop(
		const GTRawPoint* pointObj, 
		const GTEnvelop*  envelopObj);

	/*@14 点是否在Linestring上 1是, 0否*/
	static int isPointOnLineString(
		const GTRawPoint* pointObj, 
		GTLineString*     lineStringObj,
		const double&     dTolerance);

	/*@15 点是否在Ring上 1是, 0否*/
	static int isPointOnRing(
		const GTRawPoint* pointObj, 		 
		GTRing*           ringObj, 
		const double&     dTolerance);

	/*@16 点是否在Ring内 1是, 0否, crossing number方法
	 *(核心函数, 不允许任何修改)*/
	static int isPointWithinRingCN(
		const GTRawPoint* pointObj, 
		GTRing*           ringObj);

	/*@17 点是否在Ring内 1是, 0否, winding number方法*/
	static int isPointWithinRingWN(
		const GTRawPoint* pointObj,
		GTRing*           ringObj);


    /*@18 环的面积, 0退化, 负值顺时针, 正值逆时针
	 *(!!!!!!核心函数, 不允许任何修改!!!!!!)
	 */
    static double ringArea(GTRing* ringObj);

	/*@19 三点夹角[0, pi]*/
	static double threePointsIncludedAngle(
		const GTRawPoint* pointObj1, 
		const GTRawPoint* pointObj2,
		const GTRawPoint* pointObj3);

	/*@20 线段与线段相交
	 *(!!!!!!核心函数, 不允许任何修改!!!!!!)
	 * 返回值含义:(结果点存在pt_1和pt_2中)
	 *-2:退化为不同点; -1:退化为相同点; 
	 * 0:分离;         1:相交; 
	 * 2:部分重叠;      3:平行不共线;
	 * 4:共线不相交;    5:共线有一个公共点
	 */
    static int segmentIntersect(
		const GTRawPoint& seg1_fpt, 
		const GTRawPoint& seg1_tpt,
		const GTRawPoint& seg2_fpt, 
		const GTRawPoint& seg2_tpt,
		GTRawPoint&       pt_1, 
		GTRawPoint&       pt_2);

    /*@21 有向线段与x轴正向之间的夹角, [0, 2pi)*/
	static double segmentXAxisAngle(
		const GTRawPoint* fromPoint, 
		const GTRawPoint* toPoint);

	/*@22 点是否在闭合的线串内, 1是, 0否, -1出错*/
	static int isPointWithinClosedLineCN(
		const GTRawPoint   *pointObj,
		const GTLineString *lineObj);

	/*@23 获取ring内的一个点*/
	static bool getPointInRing(
		const GTRing *ringObj,
		GTRawPoint   &inPoint);

	/*@24 获取封闭线内的一个点*/
	static bool getPointInClosedLine(
		const GTLineString *lineObj,
		GTRawPoint         &inPoint);

	/*@25 线是否自相交或相邻部分重叠*/
	static bool isLineStringWeird(
		const GTLineString *lineObj);

	/*@26 环是否自相交或相邻部分重叠*/
	static bool isRingWeird(
		const GTRing *ringObj);

	/*@27 环的Centroid*/
	static bool ringCentroid(GTRing *pRing, GTRawPoint& cenPt);

	/*@28 线的centroid*/
	static bool lineStringCentroid(GTLineString *pLineString, GTRawPoint& cenPt);

	/*@29 判断3点是否是顺时针*/
	static bool isClockWise(
		const GTRawPoint &pnt1,
		const GTRawPoint &pnt2,
		const GTRawPoint &pnt3);

	/*@30 线段是否相交*/
	static bool isSegmentsIntersect(
		const GTRawPoint& seg1_fpt, 
		const GTRawPoint& seg1_tpt,
		const GTRawPoint& seg2_fpt, 
		const GTRawPoint& seg2_tpt);

	/*@31 梯形面积*/
	//Area of Trapezoid order is: leftdown -->>> rightup (longitude, latitide)
	static double TrapezoidAreaWGS84(const double& lon1, const double& lat1, 
		                             const double& lon2, const double& lat2);
};

/*
 *基础的空间关系分析类
 */
class GTS_DLL GTSRGeometryBasic: public GTGeometryAnalysis{

public:
	GTSRGeometryBasic(void);
	~GTSRGeometryBasic(void);

	/*点到直线最短点, 
	 *返回距离, 0.0为本身, 9999为错误, 
	 *参考pointSegmentDistance
	 */
	static double shortestPointToLine(
		const GTRawPoint& pointObj,
		const GTRawPoint& fromPoint,
		const GTRawPoint& toPoint,
		GTRawPoint&       shortestPoint);

	/*点到线段最短点
	 *参考pointSegmentDistance
	 */
	static double shortestPointToSegment(
		const GTRawPoint& pointObj,
		const GTRawPoint& fromPoint,
		const GTRawPoint& toPoint,
		GTRawPoint&       shortestPoint);
	
	/*点到线串最短点*/
	static double shortestPointToLineString(
		const GTRawPoint&   pointObj,
		const GTLineString& lineStringObj,
		GTRawPoint&         shortestPoint);
	
	/*点到环最短点*/
	static double shortestPointToRing(
		const GTRawPoint& pointObj,
		const GTRing&     ringObj,
		GTRawPoint&       shortestPoint);

	/*需要扩展到其它对象之间*/

public:
	/*判断一个对象是否在面对象内部, 即所有点必须在内部, 
     *9999:数据错误, 0:分离, 1: 在边缘上\内部\内接,
	 *不包含空间容忍度计算, 默认为0.0*/
	
	/*点在Envelop内部, 包括在边上*/
	static int isPointWithinEnvelop(
		const GTRawPoint& pointObj,                               
		const GTEnvelop&  envelopObj);

	/*线段在Envelop内部, 包括内接*/
	static int isSegmentWithinEnvelop(
		const GTRawPoint& fromPoint, 
		const GTRawPoint& toPoint, 
		const GTEnvelop&  envelopObj);

	/*Envelop1在Envelop2内部, 包括相等和内接*/
	static int isEnvelopWithinEnvelop(
		const GTEnvelop& envelopObj1, 
		const GTEnvelop& envelopObj2);

	/*线串在Envelop内部, 包括内接*/
	static int isLineStringWithinEnvelop(
		const GTLineString& lineStringObj, 
		const GTEnvelop&    envelopObj);

	/*环在Envelop内部*/
	static int isRingWithinEnvelop(
		const GTRing&    ringObj, 
		const GTEnvelop& envelopObj);

	/*点在环上和内部, 包括在边上
	 *先判断点是否在矩形内或上,   __isPointWithinEnvelop
	 *再判断是否在内,             isPointWithinRingCN
	 *最后判断是否在上,           isPointOnRing
	 */
	static int isPointWithinRing(
		const GTRawPoint& pointObj, 
		const GTRing&     ringObj);
	
	/*线段在环内部, 包括内接*/
	static int isSegmentWithinRing(
		const GTRawPoint& fromPoint, 
		const GTRawPoint& toPoint, 
		const GTRing&     ringObj);

	/*Envelop在环内部, 包括内接*/
	static int isPointWithinEnvelop(
		const GTEnvelop& envelopObj, 
		const GTRing&    ringObj);

	/*线串在环内部, 包括内接*/
	static int isLineStringWithinRing(
		const GTLineString& lineStringObj, 
		const GTRing&       ringObj);

	/*环1在环2内部, 包括内接和全等(不严谨，不可用 by zhou yuke)*/
	static int isRingWithinRing(
		const GTRing* ringObj1, 
		const GTRing* ringObj2);

	/*需要扩展到其它对象之间是否在内部*/

	/*需要扩展到其它对象之间是否相交*/

	/*需要扩展到其它对象之间...*/

};

/*
 *基础的Boolean几何类
 */
class GTS_DLL GTBLGeometryBasic: public GTGeometryAnalysis
{
public:
	GTBLGeometryBasic(void);
	~GTBLGeometryBasic(void);

	/*环与环布尔
	 *所有的岛的点的顺序已经调证完毕, 返回-1为失败*/
	static int twoRingBoolean(
		const GTRing*     ringObj1, 
		const GTRing*     ringObj2,
		enumGTBooleanType blType,
		GTMultiPolygon*   pPolygon);

	/*多多边形与多多边形布尔*/
	static int twoMultiPolygonBoolean(
		const GTMultiPolygon* mpolyObj1, 
		const GTMultiPolygon* mpolyObj2,
		enumGTBooleanType     blType,
		GTMultiPolygon*       pPolygon);

	/*ring, polygon, multipolygon之间的Boolean
	 *返回GTGeometry指针, 外部调用GTS_DELETE释放
	 *如果islandCheck设置为false, 将由返回由一系列ring构成的GTPolygon,
	 *岛的标志在island_mark中, 并且方向没有调整
	 *外部环为顺时针, 内部为逆时针. 如果islandCheck
	 */
 	static GTGeometry* twoSurfaceObjectsBoolean(
		const GTGeometry* pGeometry1,
		const GTGeometry* pGeometry2,
		enumGTBooleanType blType,
		std::vector<int>& island_mark,  /*岛的标志, 如果islandCheck=true, 则无效*/
		bool bEnvelopCheck = false,     /*是否检测Envelop的关系, 如果设置为true, 并且两个对象分离, 则返回NULL*/
		bool islandCheck   = false);    /*对于结果是否进行岛处理, 即判断岛所在的多边形, 并生成Polygon对象*/

	/*std::vector<GTRing*>之间的boolean操作, 返回环的个数*/
	static int twoSurfaceObjectsBoolean(
		std::vector<GTRing *> &ring_list_in_1,  /*输入环的集合1*/
		std::vector<int>      &hole_list_in_1,  /*环的集合1岛的标志*/
		int                    ring_numb_in_1,
		std::vector<GTRing *> &ring_list_in_2,  /*输入环的集合2*/
		std::vector<int>      &hole_list_in_2,  /*环的集合2岛的标志*/
		int                    ring_numb_in_2,
		enumGTBooleanType      blType,
		std::vector<GTRing *> &ring_list_out,   /*输出环的集合, 注意外部指针的释放*/
		std::vector<int>      &hole_list_out);  /*输出环的集合岛的标志*/

private:
	static void         __free_gpcpolyon(gpc_polygon *p);
	static gpc_polygon *__malloc_gpcpolygon(int ringCount, int *pointCountList);
};

/*
 *基础的FeatureBuffer类
 */
class GTS_DLL GTFEBufferBasic: public GTGeometryAnalysis
{
public:
	GTFEBufferBasic(void);
	~GTFEBufferBasic(void);

	/*点的Buffer*/
	static int pointBuffer(
		double dX, 
		double dY, 
	    double dR, 
		GTMultiPolygon* multiPolygon);
	
	/*返回指针, 外部释放*/
	static GTRing *pointBuffer(
		const GTPoint *pPointObj,
		double         dR);

	/**************************************************************************
	 *以下函数需要开发单机多核算法*
	 **************************************************************************/
	 /*线串Buffer, 返回多多边形组织的结构*/
	static int lineStringBuffer(
		const GTLineString* lineStringObj, 
		double              dR, 
		GTMultiPolygon*     multiPolygon);

	/*返回指针链表, 外部释放*/
	static int lineStringBuffer(
		const GTLineString*    lineStringObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*环Buffer(外类型)*/
	static int ringBufferFace(
		const GTRing*   ringObj, 
		double          dR, 
		GTMultiPolygon* multiPolygon);

	/*环Buffer(边类型)*/
	static int ringBufferEdge(
		const GTRing*   ringObj, 
		double          dR, 
		GTMultiPolygon* multiPolygon);

	/*多边形Buffer(外类型)
	 *由于多边形可能有岛存在, 需要删除岛内部区域
	 */
	static int polygonBufferFace(
		const GTPolygon* polygonObj, 
		double           dR, 
		GTMultiPolygon*  multiPolygon);
	
	/*多点Buffer*/
	static int multiPointBuffer(
		const GTMultiPoint* multiPointObj, 
		double              dR, 
		GTMultiPolygon*     multiPolygon);

	/*多线Buffer*/
	static int multiLineStringBuffer(
		const GTMultiLineString* multiLineStringObj,                 
		double                   dR, 
		GTMultiPolygon*          multiPolygon);

	/*多多边形Buffer*/
	static int multiPolygonBufferFace(
		const GTMultiPolygon* multiPolygonObj,                  
		double                dR, 
		GTMultiPolygon*       multiPolygon);

	/*GTGeometryBag的Buffer*/
	static int geometryBagBuffer(
		const GTGeometryBag *geometryBagObj,
		double               dR, 
		GTMultiPolygon*      multiPolygon);

protected:
	/*点Buffer*/
	static int __pointBuffer(
		double  dX, 
		double  dY, 
		double  dR, 
		GTRing* ringObj);

	/*线段Buffer(圆形端点)*/
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
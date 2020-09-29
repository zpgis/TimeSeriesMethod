/****************************************************************
 *
 * Project:  地理信息处理工具集
 * Purpose:  空间几何对象类的定义
 * Author:   范俊甫, fanjf@lreis.ac.cn 2010年-11月-15日
 *
 ****************************************************************
 * Copyright (c) All rights reserved.
 * 版权所有  (c) 保留所有权利
 ****************************************************************/
#pragma once
#ifndef GTGEOMETRY_H_INCLUDED
#define GTGEOMETRY_H_INCLUDED

#include <assert.h>
#include "gts_port.h"
#include "gt_string.h"

/*使用部分OGR函数*/
#include "../include_p/gdal/ogr_p.h"
#include "../include_p/gdal/ogr_feature.h"
#include "../include_p/gdal/ogr_geometry.h"
#include "../include_p/gdal/ogr_core.h"

namespace gts {

/*非抽象类声明*/
class GTString;
class GTRawPoint;
class GTEnvelop;
class GTPoint;
class GTLineString;
class GTRing;
class GTPolygon;
class GTMultiPoint;
class GTMultiLineString;
class GTMultiPolygon;
class GTGeometryBag;
class GTDynamicPoint;

/**
*几何对象的抽象类
*/
class GTS_DLL GTGeometry
{
public:
	GTGeometry() {}
	virtual ~GTGeometry() {}

	/*返回几何对象的维数*/
	virtual unsigned char getDimension() const = 0;

	/*返回几何对象类型*/
	virtual enumGTGeometryType getGeometryType() const = 0;

	/*返回几何对象WKB类型*/
	virtual enumGTWKBGeometryType getGeometryWKBType() const = 0;

	/*返回几何对象数据是否为空*/
	virtual bool isEmpty() const = 0;

	/*返回几何对象的外部矩形（点对象除外）*/
	virtual GTEnvelop getEnvelop() const = 0;

	/*设置几何对象的外部矩形（点对象除外）*/
	virtual void setEnvelop(const GTEnvelop& envelop) = 0;

	/*将所有的几何数据清空（点、线段对象除外）*/
	virtual void setEmpty() = 0;

	/*处理对象落在<-180或者>180或者横跨+180和-180之间的情况*/
	/*该函数只针对大地坐标有效*/
	virtual void geoNormalize() = 0;

	/*坐标移动*/
	virtual void move(double dx, double dy) = 0;

	/*相对给定点的尺度变换*/
	virtual void scale(const GTRawPoint& originPoint, double sx, double sy) = 0;

	/*相对给定点的旋转, 旋转角度为顺时针弧度*/
	virtual void rotate(const GTRawPoint& originPoint, double rtAngle) = 0;

	/*复制一个对象, 调用GTS_DELETE释放*/
	virtual GTGeometry *clone() const = 0;

	/*根据所有对象更新Envelop*/
	virtual void updateEnvelop() = 0;

	/*WKT和WKB数据交换接口*/
	virtual int getWKBSize() const = 0;
	
    virtual enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true) = 0;
	
    /*需要在输入之前为根据getWKBSize()为pDataOut分配空间, 然后外部手动释放*/
	virtual enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const = 0;
	
    virtual enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true) = 0;

    /*函数为pDataOut分配内存, 重要!!!: 外部使用之后调用GTS_FREE释放
	 *示例: char* pDataOut=0; exportToWkt(&pDataOut);
	 *     GTS_FREE(pDataOut);
	 */
	virtual enumGTDataErr exportToWkt(char **pDataOut) const = 0;
};

/**
 *二维空间点对
 */
class GTS_DLL GTRawPoint
{
public:
	GTRawPoint() {}
	GTRawPoint(double x, double y) : X(x), Y(y) {}
	bool operator == (const GTRawPoint& rawPointObj) const {
		return (X == rawPointObj.X) && (Y == rawPointObj.Y);
	}
	bool operator != (const GTRawPoint& rawPointObj) const {
		return (X != rawPointObj.X) || (Y != rawPointObj.Y);
	}

	/*实现与OGR点之间的操作, 以及复杂对象的快速拷贝*/
	GTRawPoint(const OGRRawPoint& ogrRawPointObj) {
		*this = *(GTRawPoint*)&ogrRawPointObj;
	}

	/*按照x-y顺序实现大小符号重载*/
	bool operator > (const GTRawPoint& rawPointObj) const {
		if(X != rawPointObj.X)
			return X > rawPointObj.X;
		if(Y != rawPointObj.Y)
			return Y > rawPointObj.Y;
		return false;
	}
	bool operator < (const GTRawPoint& rawPointObj) const {
		if(X != rawPointObj.X)
			return X < rawPointObj.X;
		if(Y != rawPointObj.Y)
			return Y < rawPointObj.Y;
		return false;
	}

	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_RawPoint; }
	void geoNormalize();
	void move(double dx, double dy) { X += dx, Y += dy; }
	GTRawPoint move_new(double dx, double dy) const { return GTRawPoint(X + dx, Y + dy); }
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	void set(double x, double y) { X = x, Y = y; }

	/*常用的两个静态函数*/

	/*计算三角形面积*/
	static double GetTriangleArea(const GTRawPoint* pointObj1, const GTRawPoint* pointObj2, const GTRawPoint* pointObj3);

	/*点是否在线段上*/
	static bool pointOnSegment(const GTRawPoint* pointObj, const GTRawPoint* segFPoint, const GTRawPoint* segTPoint);

public:
	double X;
	double Y;
};

#ifndef DBL_MIN
#define DBL_MIN		4.94065645841246544e-324
#endif
#ifndef DBL_MAX
#define DBL_MAX		1.79769313486231470e+308
#endif
#ifndef FLT_MIN
#define FLT_MIN		((float)1.40129846432481707e-45)
#endif
#ifndef FLT_MAX
#define FLT_MAX		((float)3.40282346638528860e+38)
#endif

/**
 *二维Box定义
 */
class GTS_DLL GTEnvelop
{
public:
	GTEnvelop() {}
	~GTEnvelop() {}
	GTEnvelop(double ldX, double ldY, double ruX, double ruY) : minX(ldX), minY(ldY), maxX(ruX), maxY(ruY) {}

	bool operator == (const GTEnvelop& envelopObj) const {
		return(
			minX == envelopObj.minX && minY == envelopObj.minY &&
			maxX == envelopObj.maxX && maxY == envelopObj.maxY);
	}
	bool operator != (const GTEnvelop& env) const {
		return !(*this == env);
	}

	bool isValid() const {
		return ((maxX >= minX) &&
			(maxY >= minY));
	}
	void setValue(double ldX, double ldY, double ruX, double ruY) {
		minX = ldX;
		minY = ldY;
		maxX = ruX;
		maxY = ruY;
	}

	bool isContain(const GTEnvelop& envelopObj) const {
		return(
			envelopObj.minX >= minX && envelopObj.maxX <= maxX &&
			envelopObj.minY >= minY && envelopObj.maxY <= maxY);
	}

	bool isDisjoin(const GTEnvelop& envelopObj) const {
		return(
			envelopObj.maxX < minX || envelopObj.minX > maxX ||
			envelopObj.maxY < minY || envelopObj.minY > maxY);
	}

	bool isWithin(const GTEnvelop& envelopObj) const {
		return(
			minX >= envelopObj.minX && maxX <= envelopObj.maxX &&
			minY >= envelopObj.minY && maxY <= envelopObj.maxY);
	}

	void setInvalid() {
		minX = minY = FLT_MAX;
		maxX = maxY = -FLT_MAX;
	}

	void set(double x, double y) {
		this->minX = this->maxX = x;
		this->minY = this->maxY = y;
	}

	void set(double minx, double miny, double maxx, double maxy) {
		this->minX = minx, this->minY = miny, this->maxX = maxx, this->maxY = maxy;
	}

	bool intersected(const GTEnvelop& env) const {
		return this->minX < env.maxX && this->maxX > env.minX && this->minY < env.maxY && this->maxY > env.minY;
	}

	void intersect(const GTEnvelop& env1, const GTEnvelop& env2) {
		this->minX = MAX(env1.minX, env2.minX);
		this->maxX = MIN(env1.maxX, env2.maxX);
		this->minY = MAX(env1.minY, env2.minY);
		this->maxY = MIN(env1.maxY, env2.maxY);
	}

	bool contains(const GTEnvelop& env) const {
		return this->minX <= env.minX && this->maxX >= env.maxX && this->minY <= env.minY && this->maxY >= env.maxY;
	}

	bool contains(const GTRawPoint& rp) const {
		return this->minX <= rp.X && this->maxX >= rp.X && this->minY <= rp.Y && this->maxY >= rp.Y;
	}

	void inflates_to_contain(const GTEnvelop& env) {
		if(this->minX > env.minX)
			this->minX = env.minX;
		if(this->maxX < env.maxX)
			this->maxX = env.maxX;
		if(this->minY > env.minY)
			this->minY = env.minY;
		if(this->maxY < env.maxY)
			this->maxY = env.maxY;
	}

	void inflates_to_contain(const GTRawPoint& rp) {
		if(this->minX > rp.X)
			this->minX = rp.X;
		if(this->maxX < rp.X)
			this->maxX = rp.X;
		if(this->minY > rp.Y)
			this->minY = rp.Y;
		if(this->maxY < rp.Y)
			this->maxY = rp.Y;
	}

	void inflates(double by) {
		inflates(by, by);
	}

	void inflates(double x, double y) {
		this->minX -= x;
		this->maxX += x;
		this->minY -= y;
		this->maxY += y;
	}

	void inflates(double x0, double y0, double x1, double y1) {
		this->minX -= x0;
		this->maxX += x1;
		this->minY -= y0;
		this->maxY += y1;
	}

	GTEnvelop& operator*=(double scale) {
		minX *= scale;
		minY *= scale;
		maxX *= scale;
		maxY *= scale;
		return *this;
	}

	double width() const { return maxX - minX; }
	double height() const { return maxY - minY; }
	double area() const { return (maxX - minX) * (maxY - minY); }
	double x_center() const { return (maxX + minX) * .5; }
	double y_center() const { return (maxY + minY) * .5; }
	GTRawPoint center() const { return GTRawPoint((minX + maxX) * .5, (minY + maxY) * .5); }
	GTRawPoint top_left() const { return GTRawPoint(minX, maxY); }
	GTRawPoint bottom_left() const { return GTRawPoint(minX, minY); }
	GTRawPoint top_right() const { return GTRawPoint(maxX, maxY); }
	GTRawPoint bottom_right() const { return GTRawPoint(maxX, minY); }
	
	void shift(double x, double y) {
		this->minX += x;
		this->maxX += x;
		this->minY += y;
		this->maxY += y;
	}

	void shift(const GTRawPoint& rp) {
		this->minX += rp.X;
		this->maxX += rp.X;
		this->minY += rp.Y;
		this->maxY += rp.Y;
	}

	GTEnvelop shift_new(double x, double y) const {
		return GTEnvelop(this->minX + x, this->minY + y, this->maxX + x, this->maxY + y);
	}

	GTEnvelop shift_new(const GTRawPoint& rp) const {
		return GTEnvelop(this->minX + rp.X, this->minY + rp.Y, this->maxX + rp.X, this->maxY + rp.Y);
	}

	void mirror() {
		set(-maxX, -maxY, -minX, -minY);
	}

	GTEnvelop mirror_new() const {
		return GTEnvelop(-maxX, -maxY, -minX, -minY);
	}
	
	void normalize() {
		if(this->minX > this->maxX) {
			double tmp = this->minX;
			this->minX = this->maxX;
			this->maxX = tmp;
		}
		if(this->minY > this->maxY) {
			double tmp = this->minY;
			this->minY = this->maxY;
			this->maxY = tmp;
		}
	}

public:
	double minX;
	double minY;
	double maxX;
	double maxY;
};

/**
 *点对象的类定义
 */
class GTS_DLL GTPoint: public GTGeometry
{
public:
	friend class GTDynamicPoint;
	GTPoint() : X(0), Y(0), Z(0), M(0) {}
	virtual ~GTPoint() {}
	GTPoint(double x, double y) : X(x), Y(y), Z(0), M(0) {}
	GTPoint(double x, double y, double z, double m) : X(x), Y(y), Z(z), M(m) {}
	bool operator == (const GTPoint& pointObj) const {
		return (X == pointObj.X) && (Y == pointObj.Y) && 
			(Z == pointObj.Z) && (M == pointObj.M);
	}
	bool operator != (const GTPoint& pointObj) const {
		return !(*this == pointObj);
	}
	bool operator > (const GTPoint& pointObj) const {
		if(X != pointObj.X)
			return X > pointObj.X;
		if(Y != pointObj.Y)
			return Y > pointObj.Y;
		if(Z != pointObj.Z)
			return Z > pointObj.Z;
		if(M != pointObj.M)
			return M > pointObj.M;
		return false;
	}
	bool operator < (const GTPoint& pointObj) const {
		if(X != pointObj.X)
			return X < pointObj.X;
		if(Y != pointObj.Y)
			return Y < pointObj.Y;
		if(Z != pointObj.Z)
			return Z < pointObj.Z;
		if(M != pointObj.M)
			return M < pointObj.M;
		return false;
	}
	
	/*实现与OGR点之间的操作, 以及复杂对象的快速拷贝*/
	GTPoint(const OGRPoint& ogrPointObj) {
		X = ogrPointObj.getX();
		Y = ogrPointObj.getY();
		Z = ogrPointObj.getZ();
		M = 0;
	}

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_Point; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBPoint; }
	bool isEmpty() const { return false; }
	GTEnvelop getEnvelop() const { return GTEnvelop(X, Y, X, Y); }
	void setEnvelop(const GTEnvelop& envelop) {}
	void setEmpty() {}
	void geoNormalize() {}
	void move(double dx, double dy) {
		X += dx, Y += dy;
	}
	void scale(const GTRawPoint& originPoint, double sx, double sy) {
		X = (X - originPoint.X) * sx + originPoint.X;
		Y = (Y - originPoint.Y) * sy + originPoint.Y;
	}
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop() {}
	int getWKBSize() const { return 21; }

	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;
	enumGTDataErr importFromOgrObject(const OGRGeometry* ogr_geom, bool updateEnv = true);

	/*自有函数*/
	double getX() const { return X; }
	double getY() const { return Y; }
	double getZ() const { return Z; }
	double getM() const { return M; }
	double& getX() { return X; }
	double& getY() { return Y; }
	double& getZ() { return Z; }
	double& getM() { return M; }
	/*将点转化为GTRawPoint*/
	void queryRawPoint(GTRawPoint& rawPoint) const {
		rawPoint.X = X, rawPoint.Y = Y;
	}
	void setX(double x) { X = x; }
	void setY(double y) { Y = y; }
	void setZ(double z) { Z = z; }
	void setM(double m) { M = m; }
	void setXY(double x, double y) { X = x, Y = y; }
	/*按照x,y,z,m顺序进行比较*/
	int compare(const GTPoint& pointObj) const;

protected:
	double X;
	double Y;
	double Z;
	double M;
};

/**
 *曲线抽象类定义
 */
class GTS_DLL GTCurve: public GTGeometry
{
public:
	GTCurve() {}
	virtual ~GTCurve() {}

	/*获取起始点指针*/
	virtual GTRawPoint* getFromPointPtr() = 0;
	
	/*获取终止点指针*/
	virtual GTRawPoint* getToPointPtr() = 0;

	/*复制起始点*/
	virtual void queryFromPoint(GTRawPoint& fPoint) const = 0;
	
	/*复制终止点*/
	virtual void queryToPoint(GTRawPoint& tPoint) const = 0;

	/*设置起始点*/
	virtual void setFromPoint(const GTRawPoint& fPoint) = 0;

	/*设置终止点*/
	virtual void setToPoint(const GTRawPoint& tPoint) = 0;
	
	/*改变曲线方向,即点的排列顺序*/
	virtual void reverseOrientation() = 0;
	
	/*是否为封闭几何对象*/
	virtual bool isClosed() const = 0;

	/*计算曲线长度*/
	virtual double getLength() const = 0;
};

/**
 *线段抽象类定义
 */
class GTS_DLL GTSegment: public GTCurve
{
public:
	GTSegment() {}
	virtual ~GTSegment() {}
};

/**
 *二维直线（线段）对象类定义
 */
class GTS_DLL GTLine: public GTSegment
{
public:
	GTLine() {}
	virtual ~GTLine() {}
	GTLine(const GTRawPoint& fPoint, const GTRawPoint& tPoint) : fromPoint(fPoint), toPoint(tPoint) {}
	GTLine(double fx, double fy, double tx, double ty) : fromPoint(fx, fy), toPoint(tx, ty) {}
	bool operator == (const GTLine& lineObj) const {
		return(fromPoint == lineObj.fromPoint && toPoint == lineObj.toPoint);
	}

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_Line; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBUnknown; }
	bool isEmpty() const { return false; }
	GTEnvelop getEnvelop() const;
	void setEnvelop(const GTEnvelop& envelop);
	void setEmpty();
	void geoNormalize();
	void move(double dx, double dy) {
		fromPoint.X += dx;
		fromPoint.Y += dy;
		toPoint.X   += dx;
		toPoint.Y   += dy;
	}
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const;
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;

	/*由GTCurve继承*/
	GTRawPoint& getFromPoint() { return fromPoint; }
	const GTRawPoint& getFromPoint() const { return fromPoint; }
	GTRawPoint& getToPoint() { return toPoint; }
	const GTRawPoint& getToPoint() const { return toPoint; }
	GTRawPoint* getFromPointPtr() { return &fromPoint; }
	const GTRawPoint* getFromPointPtr() const { return &fromPoint; }
	GTRawPoint* getToPointPtr() { return &toPoint; }
	const GTRawPoint* getToPointPtr() const { return &toPoint; }
	void queryFromPoint(GTRawPoint& fPoint) const { fPoint = fromPoint; }
	void queryToPoint(GTRawPoint& tPoint) const { tPoint = toPoint; }
	void setFromPoint(const GTRawPoint& fPoint) { fromPoint = fPoint; }
	void setToPoint(const GTRawPoint& tPoint) { toPoint = tPoint; }
	void reverseOrientation() {
		GTRawPoint tmpPoint(fromPoint);
		fromPoint = toPoint;
		toPoint   = tmpPoint;
	}
	bool isClosed() const { return false; }
	double getLength() const;

	GTRawPoint& operator[](int idx) { return idx ? toPoint : fromPoint; }
	const GTRawPoint& operator[](int idx) const { return idx ? toPoint : fromPoint; }
	double getFromPointX() const { return fromPoint.X; }
	double getFromPointY() const { return fromPoint.Y; }
	double getToPointX() const { return toPoint.X; }
	double getToPointY() const { return toPoint.Y; }
	double getSpanX() const { return toPoint.X - fromPoint.X; }
	double getSpanY() const { return toPoint.Y - fromPoint.Y; }

	/*自有成员函数*/
	/*获取线段与x轴正方向的夹角*/
	double getAngle() const;
	
	void putCoords(GTRawPoint& fPoint, GTRawPoint& tPoint) {
		fromPoint = fPoint, toPoint = tPoint;
	}
	void putCoords(double fromX, double fromY, double toX, double toY) {
		fromPoint.X = fromX, fromPoint.Y = fromY;
		toPoint.X = toX, toPoint.Y = toY;
	}
	void queryCoords(GTRawPoint& fPoint, GTRawPoint& tPoint) const {
		fPoint = fromPoint, tPoint = toPoint;
	}
	void putFromPointCoords(double x, double y) {
		fromPoint.X = x, fromPoint.Y = y;
	}
	void putToPointCoords(double x, double y) {
		toPoint.X = x, toPoint.Y = y;
	}

protected:
	GTRawPoint fromPoint;
	GTRawPoint toPoint;
};

/**
 *多曲线抽象类定义，将派生LineString, Ring和Polygon
 */
class GTS_DLL GTPolyCurve: public GTCurve
{
public:
	GTPolyCurve() {}
	virtual ~GTPolyCurve() {}

	/*利用Douglas-Poiker方法对曲线进行采样近似*/
	virtual void generalizeByDouglasPoiker() = 0;

	/*Bezier曲线平滑*/
	virtual void soomthByBeizer() = 0;

	/*曲线的分解, 按照给定点或者是给定的距起始点的长度*/
	virtual void splitAtPoint() = 0;
	virtual void splitAtDistance() = 0;

	/*计算面积*/
	virtual double getArea() const = 0;
};

/**
 *二维椭圆曲线对象类定义
 */
class GTS_DLL GTElliptic: public GTCurve
{
public:
	GTElliptic();
	virtual ~GTElliptic();
	bool operator == (const GTElliptic& ellipticObj);// { return false; }

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_Elliptic; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBUnknown; }
	bool isEmpty() const { return false; }
	GTEnvelop getEnvelop() const;
	void setEnvelop(const GTEnvelop& envelop);
	void setEmpty();
	void geoNormalize();
	void move(double dx, double dy);
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const;
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);

	/*由GTCurve继承*/
	GTRawPoint* getFromPointPtr();
	GTRawPoint* getToPointPtr();
	void queryFromPoint(GTRawPoint& fPoint) const;
	void queryToPoint(GTRawPoint& tPoint) const;
	void setFromPoint(const GTRawPoint& fPoint);
	void setToPoint(const GTRawPoint& tPoint);
	void reverseOrientation();
	bool isClosed() const;
	double getLength() const;

	/*自有成员函数*/

protected:
    GTRawPoint cenPoint;  /*中心点*/
	double d_a;           /*主轴长*/
	double d_b;           /*次轴长*/
	double majorAxisAngle;/*主轴与x轴角度*/
	double fromAngle;     /*起始点角度 [0, 360]*/
	double toAngle;       /*终止点角度 [0, 360]*/

	/*椭圆的三角表达为 t=[0, 360), fi=majorAxisAngle：
	 *x(t) = xc + a*cos(t)*cos(fi) - b*sin(t)*sin(fi)
	 *y(t) = yc + a*cos(t)*sin(fi) - b*sin(t)*cos(fi)
	 */
};

/**
*二维圆曲线对象类定义
*/
class GTS_DLL GTCircular: public GTCurve
{
public:
	GTCircular() : cenPoint(0, 0), d_r(0), fromAngle(0), toAngle(0) {}
	virtual ~GTCircular() {}
	bool operator == (const GTCircular& circularObj) const {
		return (cenPoint  == circularObj.cenPoint  && d_r     == circularObj.d_r &&
			fromAngle == circularObj.fromAngle && toAngle == circularObj.toAngle);
	}

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_Elliptic; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBUnknown; }
	bool isEmpty() const { return false; }
	GTEnvelop getEnvelop() const;
	void setEnvelop(const GTEnvelop& envelop);
	void setEmpty();
	void geoNormalize();
	void move(double dx, double dy) {
		cenPoint.X += dx;
		cenPoint.Y += dy;
	}
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const;
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);

	/*由GTCurve继承*/
	GTRawPoint* getFromPointPtr();
	GTRawPoint* getToPointPtr();
	void queryFromPoint(GTRawPoint& fPoint) const;
	void queryToPoint(GTRawPoint& tPoint) const;
	void setFromPoint(const GTRawPoint& fPoint);
	void setToPoint(const GTRawPoint& tPoint);
	void reverseOrientation();
	bool isClosed() const;
	double getLength() const;

	/*自有成员函数*/
	GTRawPoint getCenterPoint() const { return cenPoint; }
	double getRadius() const { return d_r; }
	double getFromAngle() const { return fromAngle; }
	double getToAngle() const { return toAngle; }

	void setCenterPoint(GTRawPoint& pt) { cenPoint = pt; }
	void setRadius(double radii) { d_r = radii; }
	void setFromAngle(double fAngle) { fromAngle = fAngle; }
	void setToAngle(double tAngle) { toAngle = tAngle; }

	/*将弧度转成线串, 采样按照角度间隔dInterval*/
	GTLineString getLineString(double dInterval) const;
	bool getLineString(GTLineString& ls, double dInterval) const;

protected:
	GTRawPoint cenPoint;  /*中心点*/
	double d_r;           /*半径长*/
	double fromAngle;     /*起始点角度 [0, 360]*/
	double toAngle;       /*终止点角度 [0, 360]*/
};

/**
 *单个线对象类定义
 */
class GTS_DLL GTLineString: public GTPolyCurve
{
public:
	GTLineString() {}
	virtual ~GTLineString() {}
	bool operator == (const GTLineString& lineStringObj) const;

	/*OGRLineString接口*/
	GTLineString(const OGRLineString& ogrLineStringObj, bool updateEnv = true);

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_LineString; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBLineString; }
	bool isEmpty() const { return pointList.empty(); }
	GTEnvelop getEnvelop() const { return envelopObj; }
	void setEnvelop(const GTEnvelop& envelop) { envelopObj = envelop; }
	void setEmpty() { pointList.clear(); envelopObj.setInvalid(); }
	void geoNormalize();
	void move(double dx, double dy);
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const { return (5 + 4 + 8 * (int)pointList.size() * 2); }
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);
	enumGTDataErr importFromOgrObject(const OGRGeometry* ogr_geom, bool updateEnv = true);
	
	/*
	 *需要在输入之前为根据getWKBSize()为pDataOut分配空间
	 *然后外部释放
	 */
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;

	/*由GTCurve继承*/
	GTRawPoint* getFromPointPtr() { return pointList.empty() ? 0 : &pointList.front(); }
	GTRawPoint* getToPointPtr() { return pointList.empty() ? 0 : &pointList.back(); }
	void queryFromPoint(GTRawPoint& fPoint) const {
		assert(!pointList.empty());
		fPoint = pointList.front();
	}
	void queryToPoint(GTRawPoint& tPoint) const {
		assert(!pointList.empty());
		tPoint = pointList.back();
	}
	void setFromPoint(const GTRawPoint& fPoint) {
		assert(!pointList.empty());
		pointList.front() = fPoint;
	}
	void setToPoint(const GTRawPoint& tPoint) {
		assert(!pointList.empty());
		pointList.back() = tPoint;
	}
	void reverseOrientation();
	bool isClosed() const { return false; }
	double getLength() const;

	/*由GTPolyCurve继承*/
	void generalizeByDouglasPoiker();
	void soomthByBeizer();
	void splitAtPoint();
	void splitAtDistance();
	double getArea() const { return 0.; }

	/*自有成员函数*/
	/*在末端追加一个点*/
	void addPoint(const GTRawPoint& pointObj) { pointList.push_back(pointObj); }
	void addPoint(double x, double y) { pointList.push_back(GTRawPoint(x, y)); }

	/*插入和追加点集合*/
	void addPoints(const std::vector<GTRawPoint>& pointArray) {
		addPoints(pointArray.data(), (int)pointArray.size());
	}
	void addPoints(const GTRawPoint* pointArray, int pointsCount);
	
	/*在某个位置插入一个点*/
	void insertPointAt(const GTRawPoint& pointObj, int atLocation) {
		assert(atLocation >= 0 && atLocation < getPointCount());
		pointList.insert(pointList.begin() + atLocation, pointObj);
	}

	/*在某个位置插入点集合*/
	void insertPointsAt(const std::vector<GTRawPoint>& pointArray, int atLocation) {
		insertPointsAt(pointArray.data(), (int)pointArray.size(), atLocation);
	}
	void insertPointsAt(const GTRawPoint* pointArray, int pointsCount, int atLocation);
	
	/*删除某个位置的点*/
	void deletePointAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getPointCount());
		pointList.erase(pointList.begin() + atLocation);
	}
	
	/*清除所有点*/
	void clearAllPoints() { pointList.clear(); envelopObj.setInvalid(); }
	
	/*更新某个位置的点*/
	void updatePointAt(const GTRawPoint& pointObj, int atLocation) {
		assert(atLocation >= 0 && atLocation < getPointCount());
		pointList[atLocation] = pointObj;
	}
	
	/*复制某个位置的点*/
	void queryPointAt(GTRawPoint& pointObj, int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		pointObj = pointList[atLocation];
	}
	
	/*获取某个位置点的指针*/
	GTRawPoint* getPointPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return &pointList[atLocation];
	}
	const GTRawPoint* getPointPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return &pointList[atLocation];
	}
	
	/*获取某个位置点的对象*/
	GTRawPoint getPointAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation];
	}

	/*获取所有点对象的复制, 需要外部释放GTS_DELETES*/
	GTRawPoint* getAllPoints() const;
	
	/*获取点链表的起始指针, 外部不能释放*/
	const GTRawPoint* getPointsPtr() const { return pointList.empty() ? 0 : pointList.data(); }

	/*获取点的个数*/
	int getPointCount() const { return (int)pointList.size(); }
	
	/*获取线段的个数*/
	int getSegmentCount() const { return MAX((int)pointList.size() - 1, 0); }

	/*添加一个线段*/
	void insertSegmentAt(const GTLine& segmentObj, int atLocation);

	/*获取某个线段*/
	GTLine getSegmentAt(int atLocation) const;

	/*获取某点X坐标*/
	double getX(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation].X;
	}

	/*获取某点Y坐标*/
	double getY(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation].Y;
	}

	/*点是否在线上*/
	bool isPointOnLine(const GTRawPoint *pointObj) const { return pointObj ? isPointOnLine(*pointObj) : false; }
	bool isPointOnLine(const GTRawPoint& pointObj) const;
	bool isPointOnLine(double x, double y) const { return isPointOnLine(GTRawPoint(x, y)); }
	bool isPointOnLine(const GTPoint *pointObj) const {
		return pointObj ? isPointOnLine(GTRawPoint(pointObj->getX(), pointObj->getY())) : false;
	}

	/*从OGR点对象数组拷贝导入*/
	bool importFromOGRPoints(const OGRRawPoint *ogrPoints, int ogrPointCount);
	
	/*输出OGR点对象数组, 数组由new分配, 需要外部调用GTS_DELETES释放*/
	OGRRawPoint* exportToOGRPoints() const;
	
	std::vector<GTRawPoint>& getPointList() { return pointList; }
	const std::vector<GTRawPoint>& getPointList() const { return pointList; }

protected:
	/*单个线对象使用点对象容器（需要连续的内存空间），而不是指针容器*/
	std::vector<GTRawPoint> pointList;
	//int pointCount;	// by zjw
	GTEnvelop envelopObj;
};

/**
 *单个环对象类定义
 *在多边形中, 外部环点为顺时针, 内部环点为逆时针
 */
class GTS_DLL GTRing: public GTPolyCurve
{
public:
	friend class GTPolygon;
	GTRing() {}
	virtual ~GTRing() {}
	bool operator == (const GTRing& ringObj) const;
	bool operator != (const GTRing& ringObj) const {
		return ! operator==(ringObj);
	}

	/*OGRLinearRing接口*/
	GTRing(const OGRLinearRing& ogrLinearRingObj, bool updateEnv = true);

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_Ring; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBUnknown; }
	bool isEmpty() const { return pointList.empty(); }
	GTEnvelop getEnvelop() const { return envelopObj; }
	void setEnvelop(const GTEnvelop& envelop) { envelopObj = envelop; }
	void setEmpty() { pointList.clear(); envelopObj.setInvalid(); }
	void geoNormalize();
	void move(double dx, double dy);
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const;
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;
	enumGTDataErr importFromOgrObject(const OGRGeometry* ogr_geom, bool updateEnv = true);

	/*由GTCurve继承*/
	GTRawPoint* getFromPointPtr() { return pointList.empty() ? 0 : &pointList.front(); }
	GTRawPoint* getToPointPtr() { return pointList.empty() ? 0 : &pointList.back(); }
	void queryFromPoint(GTRawPoint& fPoint) const;
	void queryToPoint(GTRawPoint& tPoint) const;
	void setFromPoint(const GTRawPoint& fPoint);
	void setToPoint(const GTRawPoint& tPoint);
	void reverseOrientation();
	bool isClosed() const { return true; }
	double getLength() const;

	/*由GTPolyCurve继承*/
	void generalizeByDouglasPoiker();
	void soomthByBeizer();
	void splitAtPoint();
	void splitAtDistance();
	double getArea() const;

	/*自有成员函数*/
	/*在末端追加一个点*/
	void addPoint(const GTRawPoint& pointObj);
	void addPoint(double x, double y) { addPoint(GTRawPoint(x, y)); }

	/*插入和追加点集合*/
	void addPoints(const std::vector<GTRawPoint>& pointArray) {
		addPoints(pointArray.data(), (int)pointArray.size());
	}
	void addPoints(const GTRawPoint* pointArray, int pointsCount);

	/*在某个位置插入一个点*/
	void insertPointAt(const GTRawPoint& pointObj, int atLocation);

	/*在某个位置插入点集合*/
	void insertPointsAt(const std::vector<GTRawPoint>& pointArray, int atLocation) {
		insertPointsAt(pointArray.data(), (int)pointArray.size(), atLocation);
	}
	void insertPointsAt(const GTRawPoint* pointArray, int pointsCount, int atLocation);

	/*删除某个位置的点*/
	void deletePointAt(int atLocation);

	/*清除所有点*/
	void clearAllPoints() { pointList.clear(); envelopObj.setInvalid(); }

	/*更新某个位置的点*/
	void updatePointAt(const GTRawPoint& pointObj, int atLocation);

	/*复制某个位置的点*/
	void queryPointAt(GTRawPoint& pointObj, int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		pointObj = pointList[atLocation];
	}

	/*获取某个位置点的指针*/
	GTRawPoint* getPointPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return &pointList[atLocation];
	}
	const GTRawPoint* getPointPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return &pointList[atLocation];
	}

	/*获取某个位置点的对象*/
	GTRawPoint getPointAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation];
	}

	/*获取所有点对象的复制, 需要外部释放GTS_DELETS*/
	GTRawPoint* getAllPoints() const;

	/*获取点链表的起始指针, 外部不能释放*/
	const GTRawPoint* getPointsPtr() const { return pointList.empty() ? 0 : pointList.data(); }

	/*获取点的个数*/
	int getPointCount() const { return (int)pointList.size(); }

	/*获取线段的个数*/
	int getSegmentCount() const { return MAX((int)pointList.size() - 1, 0); }
    
	/*获取重心点*/
	GTRawPoint getCentroid() const;
	
	/*添加一个线段*/
	void insertSegmentAt(const GTLine& segmentObj, int atLocation);

	/*获取某个线段*/
	GTLine getSegmentAt(int atLocation) const;

	/*获取某点X坐标*/
	double getX(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation].X;
	}

	/*获取某点Y坐标*/
	double getY(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation].Y;
	}

	/*点是否在环的边上*/
	bool isPointOnRingBoundary(const GTRawPoint *pointObj) const { return pointObj ? isPointOnRingBoundary(*pointObj) : false; }
	bool isPointOnRingBoundary(const GTRawPoint& pointObj) const;
	bool isPointOnRingBoundary(double x, double y) const { return isPointOnRingBoundary(GTRawPoint(x, y)); }
	bool isPointOnRingBoundary(const GTPoint *pointObj) const {
		return pointObj ? isPointOnRingBoundary(GTRawPoint(pointObj->getX(), pointObj->getY())) : false;
	}

	/*点是否在环内, 根据Crossing number进行判断, 不包括点在边上*/
	bool isPointInRing(const GTRawPoint *pointObj) const { return pointObj ? isPointInRing(*pointObj) : false; }
	bool isPointInRing(const GTRawPoint& pointObj) const;
	bool isPointInRing(double x, double y) const { return isPointInRing(GTRawPoint(x, y)); }
	bool isPointInRing(const GTPoint *pointObj) const {
		return pointObj ? isPointInRing(GTRawPoint(pointObj->getX(), pointObj->getY())) : false;
	}

	/*获取点的排列方向*/
	enumGTRingOrientation getOrientation() const;

	/*从OGR点对象数组拷贝导入*/
	bool importFromOGRPoints(const OGRRawPoint *ogrPoints, int ogrPointCount);

	/*输出OGR点对象数组, 数组由new分配, 需要外部调用GTS_DELETES释放*/
	OGRRawPoint* exportToOGRPoints() const;
	
	std::vector<GTRawPoint>& getPointList() { return pointList; }
	const std::vector<GTRawPoint>& getPointList() const { return pointList; }

	GTRawPoint& operator[](int idx) { return pointList[idx]; }
	const GTRawPoint& operator[](int idx) const { return pointList[idx]; }

protected:
	/*单个线对象使用点对象容器（需要连续的内存空间），而不是指针容器*/
	/*首尾点必须相同，在插入删除操作中要加入相应的处理*/
	std::vector<GTRawPoint> pointList;
// 	int pointCount;	// by zjw
	GTEnvelop envelopObj;

private:
	enumGTDataErr _importFromWkb(enumGTWKBByteOrder byteOrder, const unsigned char *pDataIn);
	enumGTDataErr _exportToWkb  (enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	int           _getWKBSize() const;
};

/**
 *单个多边形对象类定义, 只有一层岛和一个外部多边形, 
 *多岛和多个外部多边形要使用GTMultiPolygon描述
 */
class GTS_DLL GTPolygon: public GTPolyCurve
{
public:
	GTPolygon() {}
	virtual ~GTPolygon() {}
	bool operator == (const GTPolygon& polygonObj) const;

	/*OGRPolygon接口*/
	GTPolygon(const OGRPolygon& ogrPolygonObj, bool updateEnv = true);

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_Polygon; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBPolygon; }
	bool isEmpty() const { return ringList.empty(); }
	GTEnvelop getEnvelop() const { return envelopObj; }
	void setEnvelop(const GTEnvelop& envelop) { envelopObj = envelop; }
	void setEmpty() { ringList.clear(); envelopObj.setInvalid(); }
	void geoNormalize();
	void move(double dx, double dy);
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const;
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;
	enumGTDataErr importFromOgrObject(const OGRGeometry* ogr_geom, bool updateEnv = true);

	/*由GTCurve继承*/
	GTRawPoint* getFromPointPtr();
	GTRawPoint* getToPointPtr();
	void queryFromPoint(GTRawPoint& fPoint) const;
	void queryToPoint(GTRawPoint& tPoint) const;
	void setFromPoint(const GTRawPoint& fPoint);
	void setToPoint(const GTRawPoint& tPoint);
	void reverseOrientation();
	bool isClosed() const { return true; }
	double getLength() const;

	/*由GTPolyCurve继承*/
	void generalizeByDouglasPoiker();
	void soomthByBeizer();
	void splitAtPoint();
	void splitAtDistance();
	double getArea() const;
	
	/*自有成员函数*/
	/*在末端追加一个环*/
	void addRing(const GTRing& ringObj) { ringList.push_back(ringObj); }

	/*在末端追加一系列环*/
	void addRings(const std::vector<GTRing>& ringArray) {
		ringList.insert(ringList.end(), ringArray.begin(), ringArray.end());
	}

	/*在某个位置插入一个环*/
	/*atLocation = 0表示在开始插入
	 *atLocation = ringCount表示在最后插入
	 *需要严格控制下表越界
	 */
	void insertRingAt(const GTRing& ringObj, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getRingCount());
		ringList.insert(ringList.begin() + atLocation, ringObj);
	}

	/*在某个位置插入环集合*/
	/*atLocation = 0表示在开始插入
	 *atLocation = ringCount表示在最后插入
	 *需要严格控制下表越界
	 */
	void insertRingsAt(const std::vector<GTRing>& ringArray, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getRingCount());
		ringList.insert(ringList.begin() + atLocation, ringArray.begin(), ringArray.end());
	}

	/*删除某个位置的环*/
	void deleteRingAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getRingCount());
		ringList.erase(ringList.begin() + atLocation);
	}

	/*清除所有环*/
	void clearAllRings() { ringList.clear(); }

	/*更新某个位置的环*/
	void updateRingAt(const GTRing& ringObj, int atLocation) {
		assert(atLocation >= 0 && atLocation < getRingCount());
		ringList[atLocation] = ringObj;
	}

	/*复制某个位置的环*/
	void queryRingAt(GTRing& ringObj, int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount());
		ringObj = ringList[atLocation];
	}

	/*获取某个位置环的指针*/
	GTRing* getRingPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getRingCount());
		return &ringList[atLocation];
	}
	const GTRing* getRingPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount());
		return &ringList[atLocation];
	}

	/*获取某个位置环的对象*/
	GTRing getRingAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount());
		return ringList[atLocation];
	}

	/*获取环的个数*/
	int getRingCount() const { return (int)ringList.size(); }

	/*某个环反向*/
	void reverseOrientationAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getRingCount());
		ringList[atLocation].reverseOrientation();
	}

	/*某个环的方向*/
	enumGTRingOrientation getOrientationAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount());
		return ringList[atLocation].getOrientation();
	}

	/*某个环的面积*/
	double getAreaAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount());
		return ringList[atLocation].getArea();
	}

	/*获取最大环的面积*/
	int getMaxAreaOfRing(double& dArea) const;

	/*获取外部环指针*/
	GTRing* getExteriorRingPtr() { return ringList.empty() ? 0 : &ringList.front(); }
	const GTRing* getExteriorRingPtr() const { return ringList.empty() ? 0 : &ringList.front(); }

	/*获取外部环的复制*/
	GTRing getExteriorRing() const { return ringList.empty() ? GTRing() : ringList.front(); }

	/*获取内部环指针,atLocation从0开始*/
	GTRing* getInteriorRingPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getRingCount() - 1);
		return &ringList[atLocation+1];
	}
	
	/*获取内部环的复制, atLocation从0开始*/
	GTRing getInteriorRingAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount() - 1);
		return ringList[atLocation+1];
	}

	std::vector<GTRing>& getRingList() { return ringList; }
	const std::vector<GTRing>& getRingList() const { return ringList; }

protected:
	/*单个多边形对象使用环对象容器（需要连续的内存空间），而不是指针容器*/
	std::vector<GTRing> ringList;
	//int ringCount;
	GTEnvelop envelopObj;
};

/**
 *多几何对象的抽象类
 *主要的实现的类包括多线\多线\多面
 *与对应的单个几何对象可能处于一个Layer
 */
class GTS_DLL GTGeometryCollection: public GTGeometry
{
public:
	GTGeometryCollection() {}
	virtual ~GTGeometryCollection() {}

	/*获取几何对象的个数*/
	virtual int getGeometryCount() const = 0;

	/*追加一个组成的几何对象*/
	virtual void addGeometry(const GTGeometry* geometryObj) = 0;

	/*插入单个几何对象*/
	virtual void insertGeometryAt(const GTGeometry* geometryObj, int atLocation) = 0;

	/*删除单个几何对象*/
	virtual void deleteGeometryAt(int atLocation) = 0;

	/*更新单个几何对象*/
	virtual void updateGeometryAt(const GTGeometry* geometryObj, int atLocation) = 0;

	/*获取某个位置几何对象的指针*/
	virtual GTGeometry* getGeometryPtrAt(int atLocation) = 0;
};

/**
 *单个多点对象类定义(类似class含义)
 */
class GTS_DLL GTMultiPoint: public GTGeometryCollection
{
public:
	GTMultiPoint() {}
	virtual ~GTMultiPoint() {}
	bool operator == (const GTMultiPoint& multiPointObj) const;

	/*OGRMultiPoint接口*/
	GTMultiPoint(const OGRMultiPoint& ogrMultiPointObj, bool updateEnv = true);

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_MultiPoint; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBMultiPoint; }
	bool isEmpty() const { return pointList.empty(); }
	GTEnvelop getEnvelop() const { return envelopObj; }
	void setEnvelop(const GTEnvelop& envelop) { envelopObj = envelop; }
	void setEmpty() { pointList.clear(); envelopObj.setInvalid(); }
	void geoNormalize();
	void move(double dx, double dy);
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const;
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;
	enumGTDataErr importFromOgrObject(const OGRGeometry* ogr_geom, bool updateEnv = true);

	/*由GTGeometryCollection继承*/
	int getGeometryCount() const { return (int)pointList.size(); }
	void addGeometry(const GTGeometry* geometryObj);
	void insertGeometryAt(const GTGeometry* geometryObj, int atLocation);
	void deleteGeometryAt(int atLocation) { deletePointAt(atLocation); }
	void updateGeometryAt(const GTGeometry* geometryObj, int atLocation);
	GTGeometry* getGeometryPtrAt(int atLocation) { return getPointPtrAt(atLocation); }

	/*在末端追加一个点*/
	void addPoint(const GTPoint& pointObj) { pointList.push_back(pointObj); }
	void addPoint(double x, double y) { pointList.push_back(GTPoint(x, y)); }

	/*插入和追加点集合*/
	void addPoints(const std::vector<GTPoint>& pointArray);
	
	/*在某个位置插入一个点*/
	void insertPointAt(const GTPoint& pointObj, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		pointList.insert(pointList.begin() + atLocation, pointObj);
	}

	/*在某个位置插入点集合*/
	void insertPointsAt(const std::vector<GTPoint>& pointArray, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		pointList.insert(pointList.begin() + atLocation, pointArray.begin(), pointArray.end());
	}

	/*删除某个位置的点*/
	void deletePointAt(int atLocation) {
		assert(atLocation >= 0 || atLocation < getGeometryCount());
		pointList.erase(pointList.begin() + atLocation);
	}

	/*清除所有点*/
	void clearAllPoints() { pointList.clear(); }

	/*更新某个位置的点*/
	void updatePointAt(const GTPoint& pointObj, int atLocation) {
		assert(atLocation >= 0 || atLocation < getGeometryCount());
		pointList[atLocation] = pointObj;
	}

	/*复制某个位置的点*/
	void queryPointAt(GTPoint& pointObj, int atLocation) const {
		assert(atLocation >= 0 || atLocation < getGeometryCount());
		pointObj = pointList[atLocation];
	}

	/*获取某个位置点的指针*/
	GTPoint* getPointPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &pointList[atLocation];
	}
	const GTPoint* getPointPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &pointList[atLocation];
	}

	/*获取某个位置点的对象*/
	GTPoint getPointAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return pointList[atLocation];
	}

	/*点对象(或坐标)是否在集合内*/
	bool isContained(double x, double y) const { return isContained(GTPoint(x, y)); }
	bool isContained(const GTPoint& pointObj) const;

	std::vector<GTPoint>& getPointList() { return pointList; }
	const std::vector<GTPoint>& getPointList() const { return pointList; }

protected:
	/*单个多点对象使用点对象容器（需要连续的内存空间），而不是指针容器*/
	std::vector<GTPoint> pointList;
	//int pointCount;	// by zjw
	GTEnvelop envelopObj;
};

/**
 *单个多线对象类定义(类似class含义)
 */
class GTS_DLL GTMultiLineString: public GTGeometryCollection
{
public:
	GTMultiLineString() {}
	virtual ~GTMultiLineString() {}
	bool operator == (const GTMultiLineString& mlineStringObj) const;

	/*OGRMultiLineString接口*/
	GTMultiLineString(const OGRMultiLineString& ogrMultiLineStringObj, bool updateEnv = true);

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_MultiLineString; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBMultiLineString; }
	bool isEmpty() const { return lineStringList.empty(); }
	GTEnvelop getEnvelop() const { return envelopObj; }
	void setEnvelop(const GTEnvelop& envelop) { envelopObj = envelop; }
	void setEmpty() { lineStringList.clear(); envelopObj.setInvalid(); }
	void geoNormalize();
	void move(double dx, double dy);
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const;
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;
	enumGTDataErr importFromOgrObject(const OGRGeometry* ogr_geom, bool updateEnv = true);

	/*由GTGeometryCollection继承*/
	int getGeometryCount() const { return (int)lineStringList.size(); }
	void addGeometry(const GTGeometry* geometryObj);
	void insertGeometryAt(const GTGeometry* geometryObj, int atLocation);
	void deleteGeometryAt(int atLocation) { deleteLineStringAt(atLocation); }
	void updateGeometryAt(const GTGeometry* geometryObj, int atLocation);
	GTGeometry* getGeometryPtrAt(int atLocation) { return getLineStringPtrAt(atLocation); }

	/*自有成员函数*/
	/*在末端追加一个线*/
	void addLineString(const GTLineString& lineStringObj) { lineStringList.push_back(lineStringObj); }

	/*在末端追加一系列线*/
	void addLineStrings(const std::vector<GTLineString>& lineStringArray) {
		lineStringList.insert(lineStringList.end(), lineStringArray.begin(), lineStringArray.end());
	}

	/*在某个位置插入一个线*/
	void insertLineStringAt(const GTLineString& lineStringObj, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		lineStringList.insert(lineStringList.begin() + atLocation, lineStringObj);
	}

	/*在某个位置插入线集合*/
	void insertLineStringsAt(const std::vector<GTLineString>& lineStringArray, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		lineStringList.insert(lineStringList.begin() + atLocation, lineStringArray.begin(), lineStringArray.end());
	}

	/*删除某个位置的线*/
	void deleteLineStringAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		lineStringList.erase(lineStringList.begin() + atLocation);
	}

	/*清除所有线*/
	void clearAllLineStrings() { lineStringList.clear(); }

	/*更新某个位置的线*/
	void updateLineStringAt(const GTLineString& lineStringObj, int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		lineStringList[atLocation] = lineStringObj;
	}

	/*复制某个位置的线*/
	void queryLineStringAt(GTLineString& lineStringObj, int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		lineStringObj = lineStringList[atLocation];
	}

	/*获取某个位置线的指针*/
	GTLineString* getLineStringPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &lineStringList[atLocation];
	}
	const GTLineString* getLineStringPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &lineStringList[atLocation];
	}

	/*获取某个位置线的对象*/
	GTLineString getLineStringAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return lineStringList[atLocation];
	}

	/*线对象是否在集合内*/
	bool isContained(const GTLineString& lineStringObj) const;

protected:
	/*单个多线对象使用线对象容器（需要连续的内存空间），而不是指针容器*/
	std::vector<GTLineString> lineStringList;
	//int lineStringCount;	// by zjw
	GTEnvelop envelopObj;
};

/**
 *单个多多边形对象类定义(类似class含义)
 */
class GTS_DLL GTMultiPolygon: public GTGeometryCollection
{
public:
	GTMultiPolygon() {}
	virtual ~GTMultiPolygon() {}
	bool operator == (const GTMultiPolygon& mpolygonObj) const;

	/*OGRMultiPolygon接口*/
	GTMultiPolygon(const OGRMultiPolygon& ogrMultiPolygonObj, bool updateEnv = true);

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_MultiPolygon; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBMultiPolygon; }
	bool isEmpty() const { return polygonList.empty(); }
	GTEnvelop getEnvelop() const { return envelopObj; }
	void setEnvelop(const GTEnvelop& envelop) { envelopObj = envelop; }
	void setEmpty() { polygonList.clear(); envelopObj.setInvalid(); }
	void geoNormalize();
	void move(double dx, double dy);
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const;
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;
	enumGTDataErr importFromOgrObject(const OGRGeometry* ogr_geom, bool updateEnv = true);

	/*由GTGeometryCollection继承*/
	int getGeometryCount() const { return (int)polygonList.size(); }
	void addGeometry(const GTGeometry* geometryObj);
	void insertGeometryAt(const GTGeometry* geometryObj, int atLocation);
	void deleteGeometryAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		deletePolygonAt(atLocation);
	}
	void updateGeometryAt(const GTGeometry* geometryObj, int atLocation);
	GTGeometry* getGeometryPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return getPolygonPtrAt(atLocation);
	}

	/*自有成员函数*/
	/*在末端追加一个线*/
	void addPolygon(const GTPolygon& polygonObj) { polygonList.push_back(polygonObj); }

	/*在末端追加一系列线*/
	void addPolygons(const std::vector<GTPolygon>& polygonArray) {
		polygonList.insert(polygonList.end(), polygonArray.begin(), polygonArray.end());
	}

	/*在某个位置插入一个线*/
	void insertPolygonAt(const GTPolygon& polygonObj, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		polygonList.insert(polygonList.begin() + atLocation, polygonObj);
	}

	/*在某个位置插入线集合*/
	void insertPolygonsAt(const std::vector<GTPolygon>& polygonArray, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		polygonList.insert(polygonList.begin() + atLocation, polygonArray.begin(), polygonArray.end());
	}

	/*删除某个位置的线*/
	void deletePolygonAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		polygonList.erase(polygonList.begin() + atLocation);
	}

	/*清除所有线*/
	void clearAllPolygons() { polygonList.clear(); }

	/*更新某个位置的线*/
	void updatePolygonAt(const GTPolygon& polygonObj, int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		polygonList[atLocation] = polygonObj;
	}

	/*复制某个位置的线*/
	void queryPolygonAt(GTPolygon& polygonObj, int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		polygonObj = polygonList[atLocation];
	}

	/*获取某个位置线的指针*/
	GTPolygon* getPolygonPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &polygonList[atLocation];
	}
	const GTPolygon* getPolygonPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &polygonList[atLocation];
	}

	/*获取某个位置线的对象*/
	GTPolygon getPolygonAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return polygonList[atLocation];
	}

	/*多边形对象是否在集合内*/
	bool isContained(const GTPolygon& polygonObj) const;

	/*多边形个数*/
	int getPolygonCount() const { return (int)polygonList.size(); }

	/*总的环个数*/
	int getRingCount() const;

protected:
	/*单个多多边形对象使用多边形对象容器（需要连续的内存空间），而不是指针容器*/
	std::vector<GTPolygon> polygonList;
	//int polygonCount;	// by zjw
	GTEnvelop envelopObj;
};

/**
 *单个混合几何对象类定义(类似class含义)
 */
class GTS_DLL GTGeometryBag : public GTGeometryCollection
{
public:
	GTGeometryBag() {}
	virtual ~GTGeometryBag();
	GTGeometryBag(const GTGeometryBag& geometryBagObj) { *this = geometryBagObj; }
	GTGeometryBag& operator = (const GTGeometryBag& geometryBagObj);
	bool operator == (const GTGeometryBag& geometryBagObj) const;

	/*OGRGeometryCollection接口*/
	GTGeometryBag(const OGRGeometryCollection& ogrGeometryCollectionObj);

	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_GeometryBag; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBGeometryCollection; }
	bool isEmpty() const { return geometryList.empty(); }
	GTEnvelop getEnvelop() const { return envelopObj; }
	void setEnvelop(const GTEnvelop& envelop) { envelopObj = envelop; }
	void setEmpty() { clearAllGeometry(); }
	void geoNormalize();
	void move(double dx, double dy);
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const;
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;

	/*由GTGeometryCollection继承, 将加入外部几何对象的Clone*/
	int getGeometryCount() const { return (int)geometryList.size(); }
	void addGeometry(const GTGeometry* geometryObj);
	void insertGeometryAt(const GTGeometry* geometryObj, int atLocation);
	void deleteGeometryAt(int atLocation);
	void updateGeometryAt(const GTGeometry* geometryObj, int atLocation);
	GTGeometry* getGeometryPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return geometryList[atLocation];
	}
	const GTGeometry* getGeometryPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return geometryList[atLocation];
	}

	/*自有成员函数*/
	void clearAllGeometry();

protected:
	/*使用抽象类GTGeometry的指针容器*/
	std::vector<GTGeometry*> geometryList;
	//int geometryCount;	// by zjw
	GTEnvelop envelopObj;
};

/*
 *时态点对象的类
 */
class GTS_DLL GTDynamicPoint: public GTGeometry
{
public:
	GTDynamicPoint() {}
	virtual ~GTDynamicPoint() {}
	
	/*由GTGeometry继承*/
	unsigned char getDimension() const { return 2; }
	enumGTGeometryType getGeometryType() const { return GT_DynamicPoint; }
	enumGTWKBGeometryType getGeometryWKBType() const { return GT_WKBUnknown; }
	bool isEmpty() const { return dynPointList.empty(); }
	GTEnvelop getEnvelop() const { return envelopObj; }
	void setEnvelop(const GTEnvelop& envelop) { envelopObj = envelop; }
	void setEmpty();
	void geoNormalize();
	void move(double dx, double dy);
	void scale(const GTRawPoint& originPoint, double sx, double sy);
	void rotate(const GTRawPoint& originPoint, double rtAngle);
	GTGeometry *clone() const;
	void updateEnvelop();
	int getWKBSize() const;
	enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;

	/*自有函数*/
	void addPointAndTime(const GTPoint& pointObj, const tm& timeObj);
	void insertPointAndTimeAt(const GTPoint& pointObj, const tm& timeObj, int atLocation);
	void deletePointAndTimeAt(int atLocation);
	void updatePointAndTimeAt(const GTPoint& pointObj, const tm& timeObj, int atLocation);
	void updatePointAt(const GTPoint& pointObj, int atLocation);
	void updateTimeAt(const tm& timeObj, int atLocation);
	GTPoint getPointAt(int atLocation) const;
	tm getTimeAt(int atLocation) const;
	void queryPointAndTimeAt(int atLocation, GTPoint& pointObj, tm& timeObj) const;
	void clearAll();
	int getPointCount() const { return (int)dynPointList.size(); }

protected:
	std::vector<GTPoint> dynPointList;
	std::vector<tm>      dynTimeList;
	//int dynPointCount;	// by zjw
	GTEnvelop envelopObj;
};

/*
 *GTGeometryFactory类
 */
class GTS_DLL GTGeometryFactory
{
public:
// 	GTGeometryFactory();
// 	~GTGeometryFactory();

	/*OGC WKB and WKT supports*/
	/*外部调用 GTS_DELETE释放*/
	static GTGeometry *createGeometryFromWkb(unsigned char *pDataIn);
	static GTGeometry *createGeometryFromWkt(const char *pDataIn);

	/*GML转换支持
	 *返回为NULL或者char*, 外部调用GTS_FREE释放
	 *psSRS为空间参考的WKT串, 非EPSG标准的将不被支持
     *如果没有指明x轴的方向, 在大地坐标下, 坐标顺序为lat/long
	 *uFormat=1为GML3.0, =0为GML2.1.2, 其他为GML3.0
	 *uCurve针对GML3.0, uCurve=1,使用gml:Curve标签, 否则使用gml:LineString
	 *uLongSRS针对GML3.0, 是否使用urn:ogc:def:crs:EPSG::, 或者EPSG:
	 */
	static char* exportToGML(
		const GTGeometry  *pGeom,
		const char        *psSRS,
		unsigned int       uFormat  = 1,
		unsigned int       uCurve   = 1,
		unsigned int       uLongSRS = 1);

	/*KML转换支持
	 *返回为NULL或者char*, 外部调用GTS_FREE释放
	 */
	static char* exportToKML(const GTGeometry* pGeom);

	/*JSON转换支持
	 *返回为NULL或者char*, 外部调用GTS_FREE释放
	 */
	static char* exportToJSON(const GTGeometry* pGeom);

};

}
#endif

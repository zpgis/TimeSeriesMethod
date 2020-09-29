/****************************************************************
 *
 * Project:  ������Ϣ�����߼�
 * Purpose:  �ռ伸�ζ�����Ķ���
 * Author:   ������, fanjf@lreis.ac.cn 2010��-11��-15��
 *
 ****************************************************************
 * Copyright (c) All rights reserved.
 * ��Ȩ����  (c) ��������Ȩ��
 ****************************************************************/
#pragma once
#ifndef GTGEOMETRY_H_INCLUDED
#define GTGEOMETRY_H_INCLUDED

#include <assert.h>
#include "gts_port.h"
#include "gt_string.h"

/*ʹ�ò���OGR����*/
#include "../include_p/gdal/ogr_p.h"
#include "../include_p/gdal/ogr_feature.h"
#include "../include_p/gdal/ogr_geometry.h"
#include "../include_p/gdal/ogr_core.h"

namespace gts {

/*�ǳ���������*/
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
*���ζ���ĳ�����
*/
class GTS_DLL GTGeometry
{
public:
	GTGeometry() {}
	virtual ~GTGeometry() {}

	/*���ؼ��ζ����ά��*/
	virtual unsigned char getDimension() const = 0;

	/*���ؼ��ζ�������*/
	virtual enumGTGeometryType getGeometryType() const = 0;

	/*���ؼ��ζ���WKB����*/
	virtual enumGTWKBGeometryType getGeometryWKBType() const = 0;

	/*���ؼ��ζ��������Ƿ�Ϊ��*/
	virtual bool isEmpty() const = 0;

	/*���ؼ��ζ�����ⲿ���Σ��������⣩*/
	virtual GTEnvelop getEnvelop() const = 0;

	/*���ü��ζ�����ⲿ���Σ��������⣩*/
	virtual void setEnvelop(const GTEnvelop& envelop) = 0;

	/*�����еļ���������գ��㡢�߶ζ�����⣩*/
	virtual void setEmpty() = 0;

	/*�����������<-180����>180���ߺ��+180��-180֮������*/
	/*�ú���ֻ��Դ��������Ч*/
	virtual void geoNormalize() = 0;

	/*�����ƶ�*/
	virtual void move(double dx, double dy) = 0;

	/*��Ը�����ĳ߶ȱ任*/
	virtual void scale(const GTRawPoint& originPoint, double sx, double sy) = 0;

	/*��Ը��������ת, ��ת�Ƕ�Ϊ˳ʱ�뻡��*/
	virtual void rotate(const GTRawPoint& originPoint, double rtAngle) = 0;

	/*����һ������, ����GTS_DELETE�ͷ�*/
	virtual GTGeometry *clone() const = 0;

	/*�������ж������Envelop*/
	virtual void updateEnvelop() = 0;

	/*WKT��WKB���ݽ����ӿ�*/
	virtual int getWKBSize() const = 0;
	
    virtual enumGTDataErr importFromWkb(unsigned char *pDataIn, bool updateEnv = true) = 0;
	
    /*��Ҫ������֮ǰΪ����getWKBSize()ΪpDataOut����ռ�, Ȼ���ⲿ�ֶ��ͷ�*/
	virtual enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const = 0;
	
    virtual enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true) = 0;

    /*����ΪpDataOut�����ڴ�, ��Ҫ!!!: �ⲿʹ��֮�����GTS_FREE�ͷ�
	 *ʾ��: char* pDataOut=0; exportToWkt(&pDataOut);
	 *     GTS_FREE(pDataOut);
	 */
	virtual enumGTDataErr exportToWkt(char **pDataOut) const = 0;
};

/**
 *��ά�ռ���
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

	/*ʵ����OGR��֮��Ĳ���, �Լ����Ӷ���Ŀ��ٿ���*/
	GTRawPoint(const OGRRawPoint& ogrRawPointObj) {
		*this = *(GTRawPoint*)&ogrRawPointObj;
	}

	/*����x-y˳��ʵ�ִ�С��������*/
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

	/*���õ�������̬����*/

	/*�������������*/
	static double GetTriangleArea(const GTRawPoint* pointObj1, const GTRawPoint* pointObj2, const GTRawPoint* pointObj3);

	/*���Ƿ����߶���*/
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
 *��άBox����
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
 *�������ඨ��
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
	
	/*ʵ����OGR��֮��Ĳ���, �Լ����Ӷ���Ŀ��ٿ���*/
	GTPoint(const OGRPoint& ogrPointObj) {
		X = ogrPointObj.getX();
		Y = ogrPointObj.getY();
		Z = ogrPointObj.getZ();
		M = 0;
	}

	/*��GTGeometry�̳�*/
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

	/*���к���*/
	double getX() const { return X; }
	double getY() const { return Y; }
	double getZ() const { return Z; }
	double getM() const { return M; }
	double& getX() { return X; }
	double& getY() { return Y; }
	double& getZ() { return Z; }
	double& getM() { return M; }
	/*����ת��ΪGTRawPoint*/
	void queryRawPoint(GTRawPoint& rawPoint) const {
		rawPoint.X = X, rawPoint.Y = Y;
	}
	void setX(double x) { X = x; }
	void setY(double y) { Y = y; }
	void setZ(double z) { Z = z; }
	void setM(double m) { M = m; }
	void setXY(double x, double y) { X = x, Y = y; }
	/*����x,y,z,m˳����бȽ�*/
	int compare(const GTPoint& pointObj) const;

protected:
	double X;
	double Y;
	double Z;
	double M;
};

/**
 *���߳����ඨ��
 */
class GTS_DLL GTCurve: public GTGeometry
{
public:
	GTCurve() {}
	virtual ~GTCurve() {}

	/*��ȡ��ʼ��ָ��*/
	virtual GTRawPoint* getFromPointPtr() = 0;
	
	/*��ȡ��ֹ��ָ��*/
	virtual GTRawPoint* getToPointPtr() = 0;

	/*������ʼ��*/
	virtual void queryFromPoint(GTRawPoint& fPoint) const = 0;
	
	/*������ֹ��*/
	virtual void queryToPoint(GTRawPoint& tPoint) const = 0;

	/*������ʼ��*/
	virtual void setFromPoint(const GTRawPoint& fPoint) = 0;

	/*������ֹ��*/
	virtual void setToPoint(const GTRawPoint& tPoint) = 0;
	
	/*�ı����߷���,���������˳��*/
	virtual void reverseOrientation() = 0;
	
	/*�Ƿ�Ϊ��ռ��ζ���*/
	virtual bool isClosed() const = 0;

	/*�������߳���*/
	virtual double getLength() const = 0;
};

/**
 *�߶γ����ඨ��
 */
class GTS_DLL GTSegment: public GTCurve
{
public:
	GTSegment() {}
	virtual ~GTSegment() {}
};

/**
 *��άֱ�ߣ��߶Σ������ඨ��
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

	/*��GTGeometry�̳�*/
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

	/*��GTCurve�̳�*/
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

	/*���г�Ա����*/
	/*��ȡ�߶���x��������ļн�*/
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
 *�����߳����ඨ�壬������LineString, Ring��Polygon
 */
class GTS_DLL GTPolyCurve: public GTCurve
{
public:
	GTPolyCurve() {}
	virtual ~GTPolyCurve() {}

	/*����Douglas-Poiker���������߽��в�������*/
	virtual void generalizeByDouglasPoiker() = 0;

	/*Bezier����ƽ��*/
	virtual void soomthByBeizer() = 0;

	/*���ߵķֽ�, ���ո���������Ǹ����ľ���ʼ��ĳ���*/
	virtual void splitAtPoint() = 0;
	virtual void splitAtDistance() = 0;

	/*�������*/
	virtual double getArea() const = 0;
};

/**
 *��ά��Բ���߶����ඨ��
 */
class GTS_DLL GTElliptic: public GTCurve
{
public:
	GTElliptic();
	virtual ~GTElliptic();
	bool operator == (const GTElliptic& ellipticObj);// { return false; }

	/*��GTGeometry�̳�*/
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

	/*��GTCurve�̳�*/
	GTRawPoint* getFromPointPtr();
	GTRawPoint* getToPointPtr();
	void queryFromPoint(GTRawPoint& fPoint) const;
	void queryToPoint(GTRawPoint& tPoint) const;
	void setFromPoint(const GTRawPoint& fPoint);
	void setToPoint(const GTRawPoint& tPoint);
	void reverseOrientation();
	bool isClosed() const;
	double getLength() const;

	/*���г�Ա����*/

protected:
    GTRawPoint cenPoint;  /*���ĵ�*/
	double d_a;           /*���᳤*/
	double d_b;           /*���᳤*/
	double majorAxisAngle;/*������x��Ƕ�*/
	double fromAngle;     /*��ʼ��Ƕ� [0, 360]*/
	double toAngle;       /*��ֹ��Ƕ� [0, 360]*/

	/*��Բ�����Ǳ��Ϊ t=[0, 360), fi=majorAxisAngle��
	 *x(t) = xc + a*cos(t)*cos(fi) - b*sin(t)*sin(fi)
	 *y(t) = yc + a*cos(t)*sin(fi) - b*sin(t)*cos(fi)
	 */
};

/**
*��άԲ���߶����ඨ��
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

	/*��GTGeometry�̳�*/
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

	/*��GTCurve�̳�*/
	GTRawPoint* getFromPointPtr();
	GTRawPoint* getToPointPtr();
	void queryFromPoint(GTRawPoint& fPoint) const;
	void queryToPoint(GTRawPoint& tPoint) const;
	void setFromPoint(const GTRawPoint& fPoint);
	void setToPoint(const GTRawPoint& tPoint);
	void reverseOrientation();
	bool isClosed() const;
	double getLength() const;

	/*���г�Ա����*/
	GTRawPoint getCenterPoint() const { return cenPoint; }
	double getRadius() const { return d_r; }
	double getFromAngle() const { return fromAngle; }
	double getToAngle() const { return toAngle; }

	void setCenterPoint(GTRawPoint& pt) { cenPoint = pt; }
	void setRadius(double radii) { d_r = radii; }
	void setFromAngle(double fAngle) { fromAngle = fAngle; }
	void setToAngle(double tAngle) { toAngle = tAngle; }

	/*������ת���ߴ�, �������սǶȼ��dInterval*/
	GTLineString getLineString(double dInterval) const;
	bool getLineString(GTLineString& ls, double dInterval) const;

protected:
	GTRawPoint cenPoint;  /*���ĵ�*/
	double d_r;           /*�뾶��*/
	double fromAngle;     /*��ʼ��Ƕ� [0, 360]*/
	double toAngle;       /*��ֹ��Ƕ� [0, 360]*/
};

/**
 *�����߶����ඨ��
 */
class GTS_DLL GTLineString: public GTPolyCurve
{
public:
	GTLineString() {}
	virtual ~GTLineString() {}
	bool operator == (const GTLineString& lineStringObj) const;

	/*OGRLineString�ӿ�*/
	GTLineString(const OGRLineString& ogrLineStringObj, bool updateEnv = true);

	/*��GTGeometry�̳�*/
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
	 *��Ҫ������֮ǰΪ����getWKBSize()ΪpDataOut����ռ�
	 *Ȼ���ⲿ�ͷ�
	 */
	enumGTDataErr exportToWkb(enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	enumGTDataErr importFromWkt(const char *pDataIn, bool updateEnv = true);
	enumGTDataErr exportToWkt(char **pDataOut) const;

	/*��GTCurve�̳�*/
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

	/*��GTPolyCurve�̳�*/
	void generalizeByDouglasPoiker();
	void soomthByBeizer();
	void splitAtPoint();
	void splitAtDistance();
	double getArea() const { return 0.; }

	/*���г�Ա����*/
	/*��ĩ��׷��һ����*/
	void addPoint(const GTRawPoint& pointObj) { pointList.push_back(pointObj); }
	void addPoint(double x, double y) { pointList.push_back(GTRawPoint(x, y)); }

	/*�����׷�ӵ㼯��*/
	void addPoints(const std::vector<GTRawPoint>& pointArray) {
		addPoints(pointArray.data(), (int)pointArray.size());
	}
	void addPoints(const GTRawPoint* pointArray, int pointsCount);
	
	/*��ĳ��λ�ò���һ����*/
	void insertPointAt(const GTRawPoint& pointObj, int atLocation) {
		assert(atLocation >= 0 && atLocation < getPointCount());
		pointList.insert(pointList.begin() + atLocation, pointObj);
	}

	/*��ĳ��λ�ò���㼯��*/
	void insertPointsAt(const std::vector<GTRawPoint>& pointArray, int atLocation) {
		insertPointsAt(pointArray.data(), (int)pointArray.size(), atLocation);
	}
	void insertPointsAt(const GTRawPoint* pointArray, int pointsCount, int atLocation);
	
	/*ɾ��ĳ��λ�õĵ�*/
	void deletePointAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getPointCount());
		pointList.erase(pointList.begin() + atLocation);
	}
	
	/*������е�*/
	void clearAllPoints() { pointList.clear(); envelopObj.setInvalid(); }
	
	/*����ĳ��λ�õĵ�*/
	void updatePointAt(const GTRawPoint& pointObj, int atLocation) {
		assert(atLocation >= 0 && atLocation < getPointCount());
		pointList[atLocation] = pointObj;
	}
	
	/*����ĳ��λ�õĵ�*/
	void queryPointAt(GTRawPoint& pointObj, int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		pointObj = pointList[atLocation];
	}
	
	/*��ȡĳ��λ�õ��ָ��*/
	GTRawPoint* getPointPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return &pointList[atLocation];
	}
	const GTRawPoint* getPointPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return &pointList[atLocation];
	}
	
	/*��ȡĳ��λ�õ�Ķ���*/
	GTRawPoint getPointAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation];
	}

	/*��ȡ���е����ĸ���, ��Ҫ�ⲿ�ͷ�GTS_DELETES*/
	GTRawPoint* getAllPoints() const;
	
	/*��ȡ���������ʼָ��, �ⲿ�����ͷ�*/
	const GTRawPoint* getPointsPtr() const { return pointList.empty() ? 0 : pointList.data(); }

	/*��ȡ��ĸ���*/
	int getPointCount() const { return (int)pointList.size(); }
	
	/*��ȡ�߶εĸ���*/
	int getSegmentCount() const { return MAX((int)pointList.size() - 1, 0); }

	/*���һ���߶�*/
	void insertSegmentAt(const GTLine& segmentObj, int atLocation);

	/*��ȡĳ���߶�*/
	GTLine getSegmentAt(int atLocation) const;

	/*��ȡĳ��X����*/
	double getX(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation].X;
	}

	/*��ȡĳ��Y����*/
	double getY(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation].Y;
	}

	/*���Ƿ�������*/
	bool isPointOnLine(const GTRawPoint *pointObj) const { return pointObj ? isPointOnLine(*pointObj) : false; }
	bool isPointOnLine(const GTRawPoint& pointObj) const;
	bool isPointOnLine(double x, double y) const { return isPointOnLine(GTRawPoint(x, y)); }
	bool isPointOnLine(const GTPoint *pointObj) const {
		return pointObj ? isPointOnLine(GTRawPoint(pointObj->getX(), pointObj->getY())) : false;
	}

	/*��OGR��������鿽������*/
	bool importFromOGRPoints(const OGRRawPoint *ogrPoints, int ogrPointCount);
	
	/*���OGR���������, ������new����, ��Ҫ�ⲿ����GTS_DELETES�ͷ�*/
	OGRRawPoint* exportToOGRPoints() const;
	
	std::vector<GTRawPoint>& getPointList() { return pointList; }
	const std::vector<GTRawPoint>& getPointList() const { return pointList; }

protected:
	/*�����߶���ʹ�õ������������Ҫ�������ڴ�ռ䣩��������ָ������*/
	std::vector<GTRawPoint> pointList;
	//int pointCount;	// by zjw
	GTEnvelop envelopObj;
};

/**
 *�����������ඨ��
 *�ڶ������, �ⲿ����Ϊ˳ʱ��, �ڲ�����Ϊ��ʱ��
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

	/*OGRLinearRing�ӿ�*/
	GTRing(const OGRLinearRing& ogrLinearRingObj, bool updateEnv = true);

	/*��GTGeometry�̳�*/
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

	/*��GTCurve�̳�*/
	GTRawPoint* getFromPointPtr() { return pointList.empty() ? 0 : &pointList.front(); }
	GTRawPoint* getToPointPtr() { return pointList.empty() ? 0 : &pointList.back(); }
	void queryFromPoint(GTRawPoint& fPoint) const;
	void queryToPoint(GTRawPoint& tPoint) const;
	void setFromPoint(const GTRawPoint& fPoint);
	void setToPoint(const GTRawPoint& tPoint);
	void reverseOrientation();
	bool isClosed() const { return true; }
	double getLength() const;

	/*��GTPolyCurve�̳�*/
	void generalizeByDouglasPoiker();
	void soomthByBeizer();
	void splitAtPoint();
	void splitAtDistance();
	double getArea() const;

	/*���г�Ա����*/
	/*��ĩ��׷��һ����*/
	void addPoint(const GTRawPoint& pointObj);
	void addPoint(double x, double y) { addPoint(GTRawPoint(x, y)); }

	/*�����׷�ӵ㼯��*/
	void addPoints(const std::vector<GTRawPoint>& pointArray) {
		addPoints(pointArray.data(), (int)pointArray.size());
	}
	void addPoints(const GTRawPoint* pointArray, int pointsCount);

	/*��ĳ��λ�ò���һ����*/
	void insertPointAt(const GTRawPoint& pointObj, int atLocation);

	/*��ĳ��λ�ò���㼯��*/
	void insertPointsAt(const std::vector<GTRawPoint>& pointArray, int atLocation) {
		insertPointsAt(pointArray.data(), (int)pointArray.size(), atLocation);
	}
	void insertPointsAt(const GTRawPoint* pointArray, int pointsCount, int atLocation);

	/*ɾ��ĳ��λ�õĵ�*/
	void deletePointAt(int atLocation);

	/*������е�*/
	void clearAllPoints() { pointList.clear(); envelopObj.setInvalid(); }

	/*����ĳ��λ�õĵ�*/
	void updatePointAt(const GTRawPoint& pointObj, int atLocation);

	/*����ĳ��λ�õĵ�*/
	void queryPointAt(GTRawPoint& pointObj, int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		pointObj = pointList[atLocation];
	}

	/*��ȡĳ��λ�õ��ָ��*/
	GTRawPoint* getPointPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return &pointList[atLocation];
	}
	const GTRawPoint* getPointPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return &pointList[atLocation];
	}

	/*��ȡĳ��λ�õ�Ķ���*/
	GTRawPoint getPointAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation];
	}

	/*��ȡ���е����ĸ���, ��Ҫ�ⲿ�ͷ�GTS_DELETS*/
	GTRawPoint* getAllPoints() const;

	/*��ȡ���������ʼָ��, �ⲿ�����ͷ�*/
	const GTRawPoint* getPointsPtr() const { return pointList.empty() ? 0 : pointList.data(); }

	/*��ȡ��ĸ���*/
	int getPointCount() const { return (int)pointList.size(); }

	/*��ȡ�߶εĸ���*/
	int getSegmentCount() const { return MAX((int)pointList.size() - 1, 0); }
    
	/*��ȡ���ĵ�*/
	GTRawPoint getCentroid() const;
	
	/*���һ���߶�*/
	void insertSegmentAt(const GTLine& segmentObj, int atLocation);

	/*��ȡĳ���߶�*/
	GTLine getSegmentAt(int atLocation) const;

	/*��ȡĳ��X����*/
	double getX(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation].X;
	}

	/*��ȡĳ��Y����*/
	double getY(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getPointCount());
		return pointList[atLocation].Y;
	}

	/*���Ƿ��ڻ��ı���*/
	bool isPointOnRingBoundary(const GTRawPoint *pointObj) const { return pointObj ? isPointOnRingBoundary(*pointObj) : false; }
	bool isPointOnRingBoundary(const GTRawPoint& pointObj) const;
	bool isPointOnRingBoundary(double x, double y) const { return isPointOnRingBoundary(GTRawPoint(x, y)); }
	bool isPointOnRingBoundary(const GTPoint *pointObj) const {
		return pointObj ? isPointOnRingBoundary(GTRawPoint(pointObj->getX(), pointObj->getY())) : false;
	}

	/*���Ƿ��ڻ���, ����Crossing number�����ж�, ���������ڱ���*/
	bool isPointInRing(const GTRawPoint *pointObj) const { return pointObj ? isPointInRing(*pointObj) : false; }
	bool isPointInRing(const GTRawPoint& pointObj) const;
	bool isPointInRing(double x, double y) const { return isPointInRing(GTRawPoint(x, y)); }
	bool isPointInRing(const GTPoint *pointObj) const {
		return pointObj ? isPointInRing(GTRawPoint(pointObj->getX(), pointObj->getY())) : false;
	}

	/*��ȡ������з���*/
	enumGTRingOrientation getOrientation() const;

	/*��OGR��������鿽������*/
	bool importFromOGRPoints(const OGRRawPoint *ogrPoints, int ogrPointCount);

	/*���OGR���������, ������new����, ��Ҫ�ⲿ����GTS_DELETES�ͷ�*/
	OGRRawPoint* exportToOGRPoints() const;
	
	std::vector<GTRawPoint>& getPointList() { return pointList; }
	const std::vector<GTRawPoint>& getPointList() const { return pointList; }

	GTRawPoint& operator[](int idx) { return pointList[idx]; }
	const GTRawPoint& operator[](int idx) const { return pointList[idx]; }

protected:
	/*�����߶���ʹ�õ������������Ҫ�������ڴ�ռ䣩��������ָ������*/
	/*��β�������ͬ���ڲ���ɾ��������Ҫ������Ӧ�Ĵ���*/
	std::vector<GTRawPoint> pointList;
// 	int pointCount;	// by zjw
	GTEnvelop envelopObj;

private:
	enumGTDataErr _importFromWkb(enumGTWKBByteOrder byteOrder, const unsigned char *pDataIn);
	enumGTDataErr _exportToWkb  (enumGTWKBByteOrder byteOrder, unsigned char *pDataOut) const;
	int           _getWKBSize() const;
};

/**
 *��������ζ����ඨ��, ֻ��һ�㵺��һ���ⲿ�����, 
 *�ൺ�Ͷ���ⲿ�����Ҫʹ��GTMultiPolygon����
 */
class GTS_DLL GTPolygon: public GTPolyCurve
{
public:
	GTPolygon() {}
	virtual ~GTPolygon() {}
	bool operator == (const GTPolygon& polygonObj) const;

	/*OGRPolygon�ӿ�*/
	GTPolygon(const OGRPolygon& ogrPolygonObj, bool updateEnv = true);

	/*��GTGeometry�̳�*/
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

	/*��GTCurve�̳�*/
	GTRawPoint* getFromPointPtr();
	GTRawPoint* getToPointPtr();
	void queryFromPoint(GTRawPoint& fPoint) const;
	void queryToPoint(GTRawPoint& tPoint) const;
	void setFromPoint(const GTRawPoint& fPoint);
	void setToPoint(const GTRawPoint& tPoint);
	void reverseOrientation();
	bool isClosed() const { return true; }
	double getLength() const;

	/*��GTPolyCurve�̳�*/
	void generalizeByDouglasPoiker();
	void soomthByBeizer();
	void splitAtPoint();
	void splitAtDistance();
	double getArea() const;
	
	/*���г�Ա����*/
	/*��ĩ��׷��һ����*/
	void addRing(const GTRing& ringObj) { ringList.push_back(ringObj); }

	/*��ĩ��׷��һϵ�л�*/
	void addRings(const std::vector<GTRing>& ringArray) {
		ringList.insert(ringList.end(), ringArray.begin(), ringArray.end());
	}

	/*��ĳ��λ�ò���һ����*/
	/*atLocation = 0��ʾ�ڿ�ʼ����
	 *atLocation = ringCount��ʾ��������
	 *��Ҫ�ϸ�����±�Խ��
	 */
	void insertRingAt(const GTRing& ringObj, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getRingCount());
		ringList.insert(ringList.begin() + atLocation, ringObj);
	}

	/*��ĳ��λ�ò��뻷����*/
	/*atLocation = 0��ʾ�ڿ�ʼ����
	 *atLocation = ringCount��ʾ��������
	 *��Ҫ�ϸ�����±�Խ��
	 */
	void insertRingsAt(const std::vector<GTRing>& ringArray, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getRingCount());
		ringList.insert(ringList.begin() + atLocation, ringArray.begin(), ringArray.end());
	}

	/*ɾ��ĳ��λ�õĻ�*/
	void deleteRingAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getRingCount());
		ringList.erase(ringList.begin() + atLocation);
	}

	/*������л�*/
	void clearAllRings() { ringList.clear(); }

	/*����ĳ��λ�õĻ�*/
	void updateRingAt(const GTRing& ringObj, int atLocation) {
		assert(atLocation >= 0 && atLocation < getRingCount());
		ringList[atLocation] = ringObj;
	}

	/*����ĳ��λ�õĻ�*/
	void queryRingAt(GTRing& ringObj, int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount());
		ringObj = ringList[atLocation];
	}

	/*��ȡĳ��λ�û���ָ��*/
	GTRing* getRingPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getRingCount());
		return &ringList[atLocation];
	}
	const GTRing* getRingPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount());
		return &ringList[atLocation];
	}

	/*��ȡĳ��λ�û��Ķ���*/
	GTRing getRingAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount());
		return ringList[atLocation];
	}

	/*��ȡ���ĸ���*/
	int getRingCount() const { return (int)ringList.size(); }

	/*ĳ��������*/
	void reverseOrientationAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getRingCount());
		ringList[atLocation].reverseOrientation();
	}

	/*ĳ�����ķ���*/
	enumGTRingOrientation getOrientationAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount());
		return ringList[atLocation].getOrientation();
	}

	/*ĳ���������*/
	double getAreaAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount());
		return ringList[atLocation].getArea();
	}

	/*��ȡ��󻷵����*/
	int getMaxAreaOfRing(double& dArea) const;

	/*��ȡ�ⲿ��ָ��*/
	GTRing* getExteriorRingPtr() { return ringList.empty() ? 0 : &ringList.front(); }
	const GTRing* getExteriorRingPtr() const { return ringList.empty() ? 0 : &ringList.front(); }

	/*��ȡ�ⲿ���ĸ���*/
	GTRing getExteriorRing() const { return ringList.empty() ? GTRing() : ringList.front(); }

	/*��ȡ�ڲ���ָ��,atLocation��0��ʼ*/
	GTRing* getInteriorRingPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getRingCount() - 1);
		return &ringList[atLocation+1];
	}
	
	/*��ȡ�ڲ����ĸ���, atLocation��0��ʼ*/
	GTRing getInteriorRingAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getRingCount() - 1);
		return ringList[atLocation+1];
	}

	std::vector<GTRing>& getRingList() { return ringList; }
	const std::vector<GTRing>& getRingList() const { return ringList; }

protected:
	/*��������ζ���ʹ�û�������������Ҫ�������ڴ�ռ䣩��������ָ������*/
	std::vector<GTRing> ringList;
	//int ringCount;
	GTEnvelop envelopObj;
};

/**
 *�༸�ζ���ĳ�����
 *��Ҫ��ʵ�ֵ����������\����\����
 *���Ӧ�ĵ������ζ�����ܴ���һ��Layer
 */
class GTS_DLL GTGeometryCollection: public GTGeometry
{
public:
	GTGeometryCollection() {}
	virtual ~GTGeometryCollection() {}

	/*��ȡ���ζ���ĸ���*/
	virtual int getGeometryCount() const = 0;

	/*׷��һ����ɵļ��ζ���*/
	virtual void addGeometry(const GTGeometry* geometryObj) = 0;

	/*���뵥�����ζ���*/
	virtual void insertGeometryAt(const GTGeometry* geometryObj, int atLocation) = 0;

	/*ɾ���������ζ���*/
	virtual void deleteGeometryAt(int atLocation) = 0;

	/*���µ������ζ���*/
	virtual void updateGeometryAt(const GTGeometry* geometryObj, int atLocation) = 0;

	/*��ȡĳ��λ�ü��ζ����ָ��*/
	virtual GTGeometry* getGeometryPtrAt(int atLocation) = 0;
};

/**
 *�����������ඨ��(����class����)
 */
class GTS_DLL GTMultiPoint: public GTGeometryCollection
{
public:
	GTMultiPoint() {}
	virtual ~GTMultiPoint() {}
	bool operator == (const GTMultiPoint& multiPointObj) const;

	/*OGRMultiPoint�ӿ�*/
	GTMultiPoint(const OGRMultiPoint& ogrMultiPointObj, bool updateEnv = true);

	/*��GTGeometry�̳�*/
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

	/*��GTGeometryCollection�̳�*/
	int getGeometryCount() const { return (int)pointList.size(); }
	void addGeometry(const GTGeometry* geometryObj);
	void insertGeometryAt(const GTGeometry* geometryObj, int atLocation);
	void deleteGeometryAt(int atLocation) { deletePointAt(atLocation); }
	void updateGeometryAt(const GTGeometry* geometryObj, int atLocation);
	GTGeometry* getGeometryPtrAt(int atLocation) { return getPointPtrAt(atLocation); }

	/*��ĩ��׷��һ����*/
	void addPoint(const GTPoint& pointObj) { pointList.push_back(pointObj); }
	void addPoint(double x, double y) { pointList.push_back(GTPoint(x, y)); }

	/*�����׷�ӵ㼯��*/
	void addPoints(const std::vector<GTPoint>& pointArray);
	
	/*��ĳ��λ�ò���һ����*/
	void insertPointAt(const GTPoint& pointObj, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		pointList.insert(pointList.begin() + atLocation, pointObj);
	}

	/*��ĳ��λ�ò���㼯��*/
	void insertPointsAt(const std::vector<GTPoint>& pointArray, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		pointList.insert(pointList.begin() + atLocation, pointArray.begin(), pointArray.end());
	}

	/*ɾ��ĳ��λ�õĵ�*/
	void deletePointAt(int atLocation) {
		assert(atLocation >= 0 || atLocation < getGeometryCount());
		pointList.erase(pointList.begin() + atLocation);
	}

	/*������е�*/
	void clearAllPoints() { pointList.clear(); }

	/*����ĳ��λ�õĵ�*/
	void updatePointAt(const GTPoint& pointObj, int atLocation) {
		assert(atLocation >= 0 || atLocation < getGeometryCount());
		pointList[atLocation] = pointObj;
	}

	/*����ĳ��λ�õĵ�*/
	void queryPointAt(GTPoint& pointObj, int atLocation) const {
		assert(atLocation >= 0 || atLocation < getGeometryCount());
		pointObj = pointList[atLocation];
	}

	/*��ȡĳ��λ�õ��ָ��*/
	GTPoint* getPointPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &pointList[atLocation];
	}
	const GTPoint* getPointPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &pointList[atLocation];
	}

	/*��ȡĳ��λ�õ�Ķ���*/
	GTPoint getPointAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return pointList[atLocation];
	}

	/*�����(������)�Ƿ��ڼ�����*/
	bool isContained(double x, double y) const { return isContained(GTPoint(x, y)); }
	bool isContained(const GTPoint& pointObj) const;

	std::vector<GTPoint>& getPointList() { return pointList; }
	const std::vector<GTPoint>& getPointList() const { return pointList; }

protected:
	/*����������ʹ�õ������������Ҫ�������ڴ�ռ䣩��������ָ������*/
	std::vector<GTPoint> pointList;
	//int pointCount;	// by zjw
	GTEnvelop envelopObj;
};

/**
 *�������߶����ඨ��(����class����)
 */
class GTS_DLL GTMultiLineString: public GTGeometryCollection
{
public:
	GTMultiLineString() {}
	virtual ~GTMultiLineString() {}
	bool operator == (const GTMultiLineString& mlineStringObj) const;

	/*OGRMultiLineString�ӿ�*/
	GTMultiLineString(const OGRMultiLineString& ogrMultiLineStringObj, bool updateEnv = true);

	/*��GTGeometry�̳�*/
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

	/*��GTGeometryCollection�̳�*/
	int getGeometryCount() const { return (int)lineStringList.size(); }
	void addGeometry(const GTGeometry* geometryObj);
	void insertGeometryAt(const GTGeometry* geometryObj, int atLocation);
	void deleteGeometryAt(int atLocation) { deleteLineStringAt(atLocation); }
	void updateGeometryAt(const GTGeometry* geometryObj, int atLocation);
	GTGeometry* getGeometryPtrAt(int atLocation) { return getLineStringPtrAt(atLocation); }

	/*���г�Ա����*/
	/*��ĩ��׷��һ����*/
	void addLineString(const GTLineString& lineStringObj) { lineStringList.push_back(lineStringObj); }

	/*��ĩ��׷��һϵ����*/
	void addLineStrings(const std::vector<GTLineString>& lineStringArray) {
		lineStringList.insert(lineStringList.end(), lineStringArray.begin(), lineStringArray.end());
	}

	/*��ĳ��λ�ò���һ����*/
	void insertLineStringAt(const GTLineString& lineStringObj, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		lineStringList.insert(lineStringList.begin() + atLocation, lineStringObj);
	}

	/*��ĳ��λ�ò����߼���*/
	void insertLineStringsAt(const std::vector<GTLineString>& lineStringArray, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		lineStringList.insert(lineStringList.begin() + atLocation, lineStringArray.begin(), lineStringArray.end());
	}

	/*ɾ��ĳ��λ�õ���*/
	void deleteLineStringAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		lineStringList.erase(lineStringList.begin() + atLocation);
	}

	/*���������*/
	void clearAllLineStrings() { lineStringList.clear(); }

	/*����ĳ��λ�õ���*/
	void updateLineStringAt(const GTLineString& lineStringObj, int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		lineStringList[atLocation] = lineStringObj;
	}

	/*����ĳ��λ�õ���*/
	void queryLineStringAt(GTLineString& lineStringObj, int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		lineStringObj = lineStringList[atLocation];
	}

	/*��ȡĳ��λ���ߵ�ָ��*/
	GTLineString* getLineStringPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &lineStringList[atLocation];
	}
	const GTLineString* getLineStringPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &lineStringList[atLocation];
	}

	/*��ȡĳ��λ���ߵĶ���*/
	GTLineString getLineStringAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return lineStringList[atLocation];
	}

	/*�߶����Ƿ��ڼ�����*/
	bool isContained(const GTLineString& lineStringObj) const;

protected:
	/*�������߶���ʹ���߶�����������Ҫ�������ڴ�ռ䣩��������ָ������*/
	std::vector<GTLineString> lineStringList;
	//int lineStringCount;	// by zjw
	GTEnvelop envelopObj;
};

/**
 *���������ζ����ඨ��(����class����)
 */
class GTS_DLL GTMultiPolygon: public GTGeometryCollection
{
public:
	GTMultiPolygon() {}
	virtual ~GTMultiPolygon() {}
	bool operator == (const GTMultiPolygon& mpolygonObj) const;

	/*OGRMultiPolygon�ӿ�*/
	GTMultiPolygon(const OGRMultiPolygon& ogrMultiPolygonObj, bool updateEnv = true);

	/*��GTGeometry�̳�*/
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

	/*��GTGeometryCollection�̳�*/
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

	/*���г�Ա����*/
	/*��ĩ��׷��һ����*/
	void addPolygon(const GTPolygon& polygonObj) { polygonList.push_back(polygonObj); }

	/*��ĩ��׷��һϵ����*/
	void addPolygons(const std::vector<GTPolygon>& polygonArray) {
		polygonList.insert(polygonList.end(), polygonArray.begin(), polygonArray.end());
	}

	/*��ĳ��λ�ò���һ����*/
	void insertPolygonAt(const GTPolygon& polygonObj, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		polygonList.insert(polygonList.begin() + atLocation, polygonObj);
	}

	/*��ĳ��λ�ò����߼���*/
	void insertPolygonsAt(const std::vector<GTPolygon>& polygonArray, int atLocation) {
		assert(atLocation >= 0 && atLocation <= getGeometryCount());
		polygonList.insert(polygonList.begin() + atLocation, polygonArray.begin(), polygonArray.end());
	}

	/*ɾ��ĳ��λ�õ���*/
	void deletePolygonAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		polygonList.erase(polygonList.begin() + atLocation);
	}

	/*���������*/
	void clearAllPolygons() { polygonList.clear(); }

	/*����ĳ��λ�õ���*/
	void updatePolygonAt(const GTPolygon& polygonObj, int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		polygonList[atLocation] = polygonObj;
	}

	/*����ĳ��λ�õ���*/
	void queryPolygonAt(GTPolygon& polygonObj, int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		polygonObj = polygonList[atLocation];
	}

	/*��ȡĳ��λ���ߵ�ָ��*/
	GTPolygon* getPolygonPtrAt(int atLocation) {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &polygonList[atLocation];
	}
	const GTPolygon* getPolygonPtrAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return &polygonList[atLocation];
	}

	/*��ȡĳ��λ���ߵĶ���*/
	GTPolygon getPolygonAt(int atLocation) const {
		assert(atLocation >= 0 && atLocation < getGeometryCount());
		return polygonList[atLocation];
	}

	/*����ζ����Ƿ��ڼ�����*/
	bool isContained(const GTPolygon& polygonObj) const;

	/*����θ���*/
	int getPolygonCount() const { return (int)polygonList.size(); }

	/*�ܵĻ�����*/
	int getRingCount() const;

protected:
	/*���������ζ���ʹ�ö���ζ�����������Ҫ�������ڴ�ռ䣩��������ָ������*/
	std::vector<GTPolygon> polygonList;
	//int polygonCount;	// by zjw
	GTEnvelop envelopObj;
};

/**
 *������ϼ��ζ����ඨ��(����class����)
 */
class GTS_DLL GTGeometryBag : public GTGeometryCollection
{
public:
	GTGeometryBag() {}
	virtual ~GTGeometryBag();
	GTGeometryBag(const GTGeometryBag& geometryBagObj) { *this = geometryBagObj; }
	GTGeometryBag& operator = (const GTGeometryBag& geometryBagObj);
	bool operator == (const GTGeometryBag& geometryBagObj) const;

	/*OGRGeometryCollection�ӿ�*/
	GTGeometryBag(const OGRGeometryCollection& ogrGeometryCollectionObj);

	/*��GTGeometry�̳�*/
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

	/*��GTGeometryCollection�̳�, �������ⲿ���ζ����Clone*/
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

	/*���г�Ա����*/
	void clearAllGeometry();

protected:
	/*ʹ�ó�����GTGeometry��ָ������*/
	std::vector<GTGeometry*> geometryList;
	//int geometryCount;	// by zjw
	GTEnvelop envelopObj;
};

/*
 *ʱ̬��������
 */
class GTS_DLL GTDynamicPoint: public GTGeometry
{
public:
	GTDynamicPoint() {}
	virtual ~GTDynamicPoint() {}
	
	/*��GTGeometry�̳�*/
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

	/*���к���*/
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
 *GTGeometryFactory��
 */
class GTS_DLL GTGeometryFactory
{
public:
// 	GTGeometryFactory();
// 	~GTGeometryFactory();

	/*OGC WKB and WKT supports*/
	/*�ⲿ���� GTS_DELETE�ͷ�*/
	static GTGeometry *createGeometryFromWkb(unsigned char *pDataIn);
	static GTGeometry *createGeometryFromWkt(const char *pDataIn);

	/*GMLת��֧��
	 *����ΪNULL����char*, �ⲿ����GTS_FREE�ͷ�
	 *psSRSΪ�ռ�ο���WKT��, ��EPSG��׼�Ľ�����֧��
     *���û��ָ��x��ķ���, �ڴ��������, ����˳��Ϊlat/long
	 *uFormat=1ΪGML3.0, =0ΪGML2.1.2, ����ΪGML3.0
	 *uCurve���GML3.0, uCurve=1,ʹ��gml:Curve��ǩ, ����ʹ��gml:LineString
	 *uLongSRS���GML3.0, �Ƿ�ʹ��urn:ogc:def:crs:EPSG::, ����EPSG:
	 */
	static char* exportToGML(
		const GTGeometry  *pGeom,
		const char        *psSRS,
		unsigned int       uFormat  = 1,
		unsigned int       uCurve   = 1,
		unsigned int       uLongSRS = 1);

	/*KMLת��֧��
	 *����ΪNULL����char*, �ⲿ����GTS_FREE�ͷ�
	 */
	static char* exportToKML(const GTGeometry* pGeom);

	/*JSONת��֧��
	 *����ΪNULL����char*, �ⲿ����GTS_FREE�ͷ�
	 */
	static char* exportToJSON(const GTGeometry* pGeom);

};

}
#endif

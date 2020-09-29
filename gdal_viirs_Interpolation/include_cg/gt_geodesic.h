/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  Geodesic相关计算
*           只提供针对单个对象的分析
* Author:   范俊甫, fanjf@lreis.ac.cn 2012年-07月-21日
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* 版权所有   (c) 中国科学院地理科学与资源研究所. 保留所有权利
****************************************************************/
#ifndef GTGEODESIC_H_INCLUDED
#define GTGEODESIC_H_INCLUDED

#include "gt_geometry.h"
#include "gts_cg_port.h"
#include <math.h>
#include <cmath>

namespace gts{

class GTS_DLL GTGeodesic
{
public:
	GTGeodesic(void);
	~GTGeodesic(void);

public:
	/*@度分秒转十进制*/
	static double dmsToDecimal(int dDeg, int dMin, double dSec);

	/*@度分秒转十进制*/
	static void decimalToDms(double dDeci, int& dDeg, int& dMin, double& dSec);

	/*@球面两点大圆弧距离(meter), 使用sin定理*/
	static double twoPointsGreateCircle1_sphere(
		const GTRawPoint* pointObj1,                                 
		const GTRawPoint* pointObj2,
		double dR = 6371000.0);

	/*@球面两点大圆弧距离(meter), 使用Vincenty方程, 可以精确计算小距离*/
	static double twoPointsGreateCircle2_sphere(
		const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2,
		double dR = 6371000.0);

	/*@椭球面两点测地线距离(meter), 使用Vincenty方程*/
	static double twoPointsGeodestic_ellipsoid(
		const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2,
		double *ini_bearing = NULL,   //返回初始航向
		double *fin_beraing = NULL,   //返回终止航向
		double da   = 6378137.0,      //WGS84
		double db   = 6356752.314245, //WGS84
		double df_i = 298.257223563); //WGS84

	/*@球面两点大圆弧方向的中点@*/
	static GTRawPoint midPointGeodestic_sphere(
		const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2);

	/*@球面两点loxodrome (or rhumb line)距离与航向@*/
	static double twoPointsLoxodrome_sphere(
		const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2,
		double *bearing = NULL,
		double dR = 6371000.0);

	/*@球面两点loxodrome的中点@*/
	static GTRawPoint midPointLoxodrome_sphere(
		const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2);

    /*@球面两点大圆弧线(利用中点插值)@*/
    static void geodesticLineString(
        const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2,
        unsigned int iOrder,         //插值次数, 1-1, 2-3, 3-7... = sum(2^(iOder-1))
        GTLineString* lineString);

    /*@球面两点loxodrome线(利用中点插值)@*/
    static void loxodromeLineString(
        const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2,
        unsigned int iOrder,         //插值次数, 1-1, 2-3, 3-7... = sum(2^(iOder-1))
        GTLineString* lineString);
};

}
#endif
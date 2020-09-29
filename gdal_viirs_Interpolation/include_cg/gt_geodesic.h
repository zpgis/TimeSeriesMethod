/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  Geodesic��ؼ���
*           ֻ�ṩ��Ե�������ķ���
* Author:   ������, fanjf@lreis.ac.cn 2012��-07��-21��
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* ��Ȩ����   (c) �й���ѧԺ�����ѧ����Դ�о���. ��������Ȩ��
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
	/*@�ȷ���תʮ����*/
	static double dmsToDecimal(int dDeg, int dMin, double dSec);

	/*@�ȷ���תʮ����*/
	static void decimalToDms(double dDeci, int& dDeg, int& dMin, double& dSec);

	/*@���������Բ������(meter), ʹ��sin����*/
	static double twoPointsGreateCircle1_sphere(
		const GTRawPoint* pointObj1,                                 
		const GTRawPoint* pointObj2,
		double dR = 6371000.0);

	/*@���������Բ������(meter), ʹ��Vincenty����, ���Ծ�ȷ����С����*/
	static double twoPointsGreateCircle2_sphere(
		const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2,
		double dR = 6371000.0);

	/*@�������������߾���(meter), ʹ��Vincenty����*/
	static double twoPointsGeodestic_ellipsoid(
		const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2,
		double *ini_bearing = NULL,   //���س�ʼ����
		double *fin_beraing = NULL,   //������ֹ����
		double da   = 6378137.0,      //WGS84
		double db   = 6356752.314245, //WGS84
		double df_i = 298.257223563); //WGS84

	/*@���������Բ��������е�@*/
	static GTRawPoint midPointGeodestic_sphere(
		const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2);

	/*@��������loxodrome (or rhumb line)�����뺽��@*/
	static double twoPointsLoxodrome_sphere(
		const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2,
		double *bearing = NULL,
		double dR = 6371000.0);

	/*@��������loxodrome���е�@*/
	static GTRawPoint midPointLoxodrome_sphere(
		const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2);

    /*@���������Բ����(�����е��ֵ)@*/
    static void geodesticLineString(
        const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2,
        unsigned int iOrder,         //��ֵ����, 1-1, 2-3, 3-7... = sum(2^(iOder-1))
        GTLineString* lineString);

    /*@��������loxodrome��(�����е��ֵ)@*/
    static void loxodromeLineString(
        const GTRawPoint* pointObj1,
		const GTRawPoint* pointObj2,
        unsigned int iOrder,         //��ֵ����, 1-1, 2-3, 3-7... = sum(2^(iOder-1))
        GTLineString* lineString);
};

}
#endif
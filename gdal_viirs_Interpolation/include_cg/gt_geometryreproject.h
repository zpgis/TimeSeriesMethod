/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ���ζ����ͶӰת��
* 
* Author:   ������, fanjf@lreis.ac.cn 2011��-05��-11��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c) ��������Ȩ��
****************************************************************/
#ifndef GTSGEOMETRYREPROJECT_H_INCLUDED
#define GTSGEOMETRYREPROJECT_H_INCLUDED

#include "gts_cg_port.h"
#include "ogr_spatialref.h"
#include "gdalwarper.h"
#include "cpl_string.h"

namespace gts{

class GTS_DLL GTGeometryReproject
{
public:
	/*�����ʼ��, �����������ϵͳ��ͬ, ���ܹ���*/
	GTGeometryReproject(
		GTSpatialReference* fromSRS, 
		GTSpatialReference* toSRS);

	~GTGeometryReproject();

	/*��̬���ٺ���, �������new������*/
	static void destroyGeometryTransform(GTGeometryReproject *poGeomReproject);

	/*�Ƿ���Ч*/
	bool getIsValid();

	/*ԭʼ�����WKT��*/
	const char* getFromSRSWKT();

	/*Ŀ�������WKT��*/
	const char* getToSRSWKT();

	/*ת����������, ����ļ������ݵ����������Դ����ϵͳ��ͬ*/
	/*�����µļ��ζ������ⲿGTS_DELETE�ͷ�*/
	GTGeometry *geometryCoordinateTransform1(const GTGeometry* pGeometry);

	/*ת����������, ����ļ������ݵ����������Դ����ϵͳ��ͬ*/
	/*ԭʼ���ζ�������ݽ����滻*/
	bool geometryCoordinateTransform2(GTGeometry* pGeometry);

	/*��ȡһ��Envelop������Ŀ������ϵͳ�µķ�Χ*/
	bool getApprEnvelop(const GTEnvelop& srcEnv, GTEnvelop& dstEnv);

	/*��ȡһ��Envelop�ķ��任����ֵ*/
	bool getApprInverseEnvelop(const GTEnvelop& srcEnv, GTEnvelop& dstEnv);

protected:
	bool isValid;
	OGRCoordinateTransformation *poCT;
	char *pszSrcWKT;
	char *pszDstWKT;
};

}
#endif

/****************************************************************
 *
 * Project:  ������Ϣ�����߼�
 * Purpose:  �ռ�ο�������Ķ���, ��װGDAL/OGR
 * Author:   ������, fanjf@lreis.ac.cn 2010��-11��-23��
 *
 ****************************************************************
 * Copyright (c) All rights reserved.
 * ��Ȩ����  (c) ��������Ȩ��
 ****************************************************************/
#pragma once
#ifndef GTSPATIALREFERENCE_H_INCLUDED
#define GTSPATIALREFERENCE_H_INCLUDED

#include "gts_port.h"
#include "gt_geometry.h"
#include "../include_p/gdal/cpl_conv.h"
#include "../include_p/gdal/ogr_spatialref.h"

namespace gts{

/*
 *�ռ�ͶӰ������, �ڲ�ʹ��WKT��ʽ������ͶӰ��Ϣ
 */
class GTS_DLL GTSpatialReference
{
public:
	GTSpatialReference();
	~GTSpatialReference();
	GTSpatialReference(const GTSpatialReference& spRefObj);
	GTSpatialReference& operator = (const GTSpatialReference& spRefObj);
	bool operator == (const GTSpatialReference& spRefObj) const {
		return isSameSpatialReference(spRefObj);
	}

	/*�Ƚ������ռ����ϵͳ�Ƿ���ͬ*/
	bool isSameSpatialReference(const GTSpatialReference& spRefObj) const;

	/*������֪��EPSG(SRID)��Ž���ͶӰ����ĳ�ʼ�� 4326=WGS84
	  EPSG��OGC��SRID��һ�µ�, Ҳ�������ݿ��Լ���SRID���в��*/
	enumGTDataErr importFromEPSG(int epsgNo);
	
	/*��Porj4ͶӰ�����ַ�������, ����: "+proj=utm +zone=11 +datum=WGS84"*/
	enumGTDataErr importFromProj4(const char* proj4Str);

	/*��WKTͶӰ�����ַ�������*/
	enumGTDataErr importFromWkt(const char* wktStr);
    
	/*�����Ѷ���ͶӰ��WKT���, ����ʹ��ָ���ַ���ݲ��ܷ��ظ�Ӧ�ó���
	 ��Ҫ�ⲿ����CPLFree�ͷ�srsInfo, ��Ҫ�����ĸ�������������Ǹ����ͷ�
	 �Ƽ��÷���
	   char *sInfo = 0; 
	   exportToWkt(&sInfo);
	   CPLFree(sInfo)*/
	enumGTDataErr exportToWkt(char** srsInfo) const;

	/*����XMLͶӰ���, �÷������溯������, ����Ҫ����XML�ṹ, ��������Ч�ʵ�*/
	enumGTDataErr exportToXML(char** srsInfo) const;

	/*��ȡOGRSpatialReferenceָ���ַ*/
	OGRSpatialReference *getOGRSpatialRefPtr() { return &ogrSpatialRef; }
	const OGRSpatialReference *getOGRSpatialRefPtr() const { return &ogrSpatialRef; }

	/*��ȡEPSG��Ȩ��*/
	int getAuthorityCode() const;

	/*�Ƿ�Ϊ�������ϵ*/
	int isGeographic() const { return ogrSpatialRef.IsGeographic(); }

	/*�Ƿ�ΪͶӰ����ϵ*/
	int isProjected() const { return ogrSpatialRef.IsProjected(); }

	/*��ȡ�������ĵ�λ, ������"radian"
	 *�����ǽǶ�"degree"(0.0174532925199433)
	 *�÷�������char**��������*/
	enumGTUnitType getAngularUnits(char** unitInfo) const;

	/*��ȡͶӰ����ĵ�λ, ��:
	 *Meter, Foot, Foot_US, Nautical Mile, Link
	 *Chain, Rod
	 *�÷�������char**��������*/
	enumGTUnitType getLinearUnits(char** unitInfo) const;

	/*���ζ����ͶӰת��*/
	/*�����µļ��ζ���, ��Ҫ�ⲿGTS_DELETE�ͷ�*/
	static GTGeometry *geometryCoordinateTransform(const GTGeometry* pGeometry, 
		                                           const GTSpatialReference* fromSRS, 
											       const GTSpatialReference* toSRS);

protected:
	OGRSpatialReference ogrSpatialRef;
};

}
#endif

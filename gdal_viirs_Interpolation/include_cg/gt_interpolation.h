/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  
* Author:   
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c)  ��������Ȩ��
****************************************************************/
#ifndef GTSINTERPOLATION_H_INCLUDED
#define GTSINTERPOLATION_H_INCLUDED

#include "gts_cg_port.h"
#include "ogr_spatialref.h"
#include "gdalwarper.h"
#include "cpl_string.h"

namespace gts{

class GTS_DLL GTInterpolation
{
public:
	GTInterpolation(void);
	~GTInterpolation(void);

	/*��ɢ���ֵ��դ��, ʹ�� GDALGridCreate*/
	static bool interpolatePointsToGrid(
		GDALGridAlgorithm eAlgorithm,
		const int  	      nPoints,
		const double *    padfX, /*X����*/
		const double *    padfY, /*Y����*/
		const double *    padfZ, /*Zֵ*/
		const GTEnvelop&  env,   /*�ռ䷶Χ*/
		const int  	      nXSize,
		const int  	      nYSize,
		enumGTDataType    eType,
		void *            pData);

	static bool interpolatePointsToGridSource(
		GTFeatureLayer   *pLayer,       /*������*/  
		unsigned int      iField,       /*ֵ������*/
		const char       *pszFormatTo,  /*Ŀ������Դ��ʽ*/
		const char       *pszSourceTo,  /*Ŀ������Դ·��*/
		GDALGridAlgorithm eAlgorithm);  /*�㷨*/

	/*���ζ�������դ������, ʹ��GDALRasterizeGeometries*/
	static bool interpolateGeometryToGrid(
		GTRasterDataset          *poDataset,      /*�����Ը��·�ʽ��*/
		std::vector<GTGeometry*> &geomList,       /*���ζ����б�*/
		int                       geomCount,      /*���ζ������*/
		std::vector<double>      &dBurnValueList);/*ת�������ֵ*/

	static bool interpolateGeometryToGridSource(
		GTFeatureLayer *pLayer,       /*������*/  
		unsigned int    iField,       /*ֵ������*/
		const char     *pszFormatTo,  /*Ŀ������Դ��ʽ*/
		const char     *pszSourceTo,  /*Ŀ������Դ·��*/
		const double    dResol);      /*�ռ�ֱ���*/

};

}
#endif

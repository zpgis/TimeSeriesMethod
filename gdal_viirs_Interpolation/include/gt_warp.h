/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  �ռ���������ת���Ķ�����
* Author:   ������, fanjf@lreis.ac.cn 2011��-03��-21��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����  (c) ��������Ȩ��
****************************************************************/
#pragma once
#ifndef GTWARP_H_INCLUDED
#define GTWARP_H_INCLUDED

#include "gt_datasource.h"
#include "gt_string.h"
#include "../include_p/gdal/ogr_spatialref.h"
#include "../include_p/gdal/gdalwarper.h"
#include "../include_p/gdal/cpl_string.h"

namespace gts{

/*
 *GTWarpϵ����Ļ���
 */
class GTS_DLL GTWarp
{
public:
	GTWarp(void);
	virtual ~GTWarp(void);
};

/*
 *GTRasterWarp��
 */
class GTS_DLL GTRasterWarp: public GTWarp
{
public:
	GTRasterWarp(void);
	~GTRasterWarp(void);

	/*
	 *���ڼ�ͶӰ�任�ľ�̬����
	 */
	static bool simpleRasterWarp(
		const char* pszFormatTo,            /*Ŀ������Դ��ʽ*/
		const char* pszSourceFrom,          /*����Դ·��*/
		const char* pszSourceTo,            /*Ŀ������Դ·��*/
		const char* pszSRSWKT,              /*Ŀ������Դ�ռ�ο�*/
		GDALResampleAlg eResampleAlg = GRA_NearestNeighbour, /*�ز�������*/
		double cacheLimit            = 0.0, /*�ڴ滺��ʹ�ô�С����, ��λ��MB*/
		enumGTDataType eOutDataType  = GTD_Unknown,  /*�����������*/
		const double*  padfSrcNoData = 0,   /*Դ��Nodata�б�, Ҫ��Band��Ŀ��Ӧ, �������ز���*/
		int iSrcNoDataCount          = 0,   /*Դ��Nodata�б������*/
		const double*  padfDstNoData = 0,   /*Ŀ���Nodata�б�, Ҫ��Band��Ŀ��Ӧ*/
		int iDstNoDataCount          = 0);  /*Ŀ���Nodata�б������*/

	/*
	 *����ͼ����ز���
	 */
	static bool rasterResampling(
		const char* pszFormatTo,            /*Ŀ������Դ��ʽ*/
		const char* pszSourceFrom,          /*����Դ·��*/
		const char* pszSourceTo,            /*Ŀ������Դ·��*/
		double xResl,                       /*Ŀ�����ݵ�x�ֱ���*/
		double yResl,                       /*Ŀ�����ݵ�y�ֱ���*/
		GDALResampleAlg eResampleAlg = GRA_NearestNeighbour, /*�ز�������*/
		double cacheLimit            = 0.0); /*�ڴ滺��ʹ�ô�С����, ��λ��MB*/

	/*
	 *������GCPs����ͼ���ͶӰ�任
	 */
	static bool rasterWarpByGCP(
		const char* pszFormatTo,            /*Ŀ������Դ��ʽ*/
		const char* pszSourceFrom,          /*����Դ·��*/
		const char* pszSourceTo,            /*Ŀ������Դ·��*/
		const char* pszSRSWKT,              /*Ŀ������Դ�ռ�ο�*/
		int  	    nGCPCount,              /*GCP����*/
		GDAL_GCP*  	pasGCPList,             /*GCP�б�*/
		int  	    nReqOrder,              /*����ʽ����1-3*/
		GDALResampleAlg eResampleAlg = GRA_NearestNeighbour, /*�ز�������*/
		double cacheLimit            = 0.0); /*�ڴ滺��ʹ�ô�С����, ��λ��MB*/
};

/*
 *ʸ��/դ��ת������
 */
class GTS_DLL GTRasterizeAndPolygonize: public GTWarp
{
public:
	GTRasterizeAndPolygonize(void);
	~GTRasterizeAndPolygonize(void);

	/** @brief  ��һϵ�м��ζ���դ��
	 *  @return �����Ƿ�ɹ�
	 *  @note   ����ļ��ζ��������դ���������ͬ��������ϵ
	 *          ���ζ���ķ�Χ���ܳ�������Ӱ��ķ�Χ
	 *          Ŀǰֻ֧�ֵ����εĲ���
	 *          ����Ӱ������Ը��´�
	 */
	static bool executeRasterize(
		GTRasterDataset* poDataset,        /*�����Ը��·�ʽ��*/
		std::vector<GTGeometry*>& geomList,/*���ζ����б�*/
		int geomCount,                     /*���ζ������*/
		double dBurnValue );               /*ת�������ֵ*/

	/** @brief  ��դ��Ӱ��ת�ɶ��������
	 *  @return �����Ƿ�ɹ�
	 *  @note   
	 */
	static bool executePolygonize(
		const char* pszSourceFrom,  /*����Դ·��*/
		int iBandNum,               /*����Դ�Ĳ���*/
		const char* pszSourceTo,    /*Ŀ��Shape�ļ�·��*/
		const char* pszLayerName,   /*�������*/
		bool bFourConnected = true, /*��ͨ�㷨����, Ĭ��4����*/
		bool bUsedMaskBand = false);/*�Ƿ�ʹ��mask����*/
	
	/** @brief  ��ɢ���ֵΪդ������
	 *  @return �����Ƿ�ɹ�
	 *  @note   pData��ҪԤ�ȷ����
	 */
	static bool executeGridCreate(
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

	/** @brief  ����ֵ������в�ֵ���
	 *  @return �����Ƿ�ɹ�
	 *  @note   ����Ӱ�������mask����, 0ΪNodata��־
	 */
	static bool executeInterpolateNodata(
		GTRasterDataset* poDataset,
		int              iBand, 
		double           dfMaxSearchDist,          /*���������������*/
		int              nSmoothingIterations = 1);/*3*3ƽ�����ڵ�������(>=0)*/

	
	/** @brief  �Ƴ�С��ߵ�������, ����ϲ����ڽ���������
	 *  @return �����Ƿ�ɹ�
	 *  @note   ����Ӱ�������mask����, 0ΪNodata��־
	 */
	static bool executeSieveFilter(
		GTRasterDataset* poDataset,
		int              iSrcBand,
		int              iDstBand,
		int  	         nSizeThreshold, /*���ش�С��ֵ*/
		int  	         nConnectedness);/*4��8�ھ�*/

	/** @brief  DEM���ɵȸ���
	 *  @return �����Ƿ�ɹ�
	 *  @note   
	 */
	static bool executeContourGenerate(
		GTRasterDataset* poDataset,
		int              iBand, 
		double  	     dfContourInterval,/*�̼߳��*/
		double  	     dfContourBase,    /*�����߳�ֵ*/
		bool  	         bUseNoData,
		double  	     dfNoDataValue,
		const char*      pszSourceTo,      /*Ŀ��Shape�ļ�·��*/
		const char*      pszLayerName);    /*�������*/
};

/*
 *դ���и���ƴ����
 */
class GTS_DLL GTRasterClipAndMosaic: public GTWarp
{
public:
	GTRasterClipAndMosaic(void);
	~GTRasterClipAndMosaic(void);

	/** @brief  ��һ��ָ����ң��Ӱ�����и�ָ�����ε�����
	 *  @return �����Ƿ�ɹ�
	 *  @note   ���η�Χ������ԭʼ���ݾ�����ͬ����ϵͳ, ���
	 *          �����Ӱ��û���������, ���޷����. ������������͵�
	 *          ����ԭʼӰ����ͬ
	 *          ����б��Ӱ��, �ú���������
	 */
	static bool rasterClipByEnvelop(
		const char* pszFormatTo,    /*Ŀ������Դ��ʽ*/
		const char* pszSourceFrom,  /*����Դ·��*/
		const char* pszSourceTo,    /*Ŀ������Դ·��*/
		GTEnvelop& clipEnv,         /*ָ����Χ, ����ʱ���ܱ�����������λ��*/
		double dCacheLimit = 200.0);  /*�ڴ�ʹ��������޵�λΪMB*/

	/** @brief  ��һ��ָ����ң��Ӱ�����и�ָ����Χ������
	 *  @return �����Ƿ�ɹ�
	 *  @note   ���η�Χ������ԭʼ���ݾ�����ͬ����ϵͳ, ���
	 *          �����Ӱ��û���������, ���޷����. ������������͵�
	 *          ����ԭʼӰ����ͬ
	 *          ����б��Ӱ��, �ú���Ҳ����
	 */
	static bool rasterClipByExtent(
		const char* pszFormatTo,    /*Ŀ������Դ��ʽ*/
		const char* pszSourceFrom,  /*����Դ·��*/
		const char* pszSourceTo,    /*Ŀ������Դ·��*/
		GTRawPoint& pt_leftTop, 
		GTRawPoint& pt_rightBottom, /*ָ����Χ, ����ʱ���ܱ�����������λ��*/
		double dCacheLimit = 200.0);/*�ڴ�ʹ��������޵�λΪMB*/


	/** @brief  ��һ��ָ����ң��Ӱ�����и�ָ�����ش�С������
	 *  @return �����Ƿ�ɹ�
	 *  @note   ������������͵�
	 *          ����ԭʼӰ����ͬ
	 *          ����б��Ӱ��, �ú���Ҳ����
	 */
	static bool rasterClipByPixels(
		const char* pszFormatTo,    /*Ŀ������Դ��ʽ*/
		const char* pszSourceFrom,  /*����Դ·��*/
		const char* pszSourceTo,    /*Ŀ������Դ·��*/
		int& xOffset,               /*xƫ��, ���ܱ�����*/
		int& yOffset,               /*yƫ��, ���ܱ�����*/
		int& xSize,                 /*x��С, ���ܱ�����*/
		int& ySize,                 /*y��С, ���ܱ�����*/
		double dCacheLimit = 200.0);/*�ڴ�ʹ��������޵�λΪMB*/

	/** @brief  ���տռ�λ�úϲ�����Ӱ��
	 *  @return �����Ƿ�ɹ�
	 *  @note   Ĭ�����������ͺͿռ�����Ϊ��һ��
	 *          б��Ӱ��˺�����֧��
     */
	static bool rasterMosaicBySpatial(
		const char*    pszSourceFrom1,
        const double*  d_nodata_list1,
		const char*    pszSourceFrom2,
		const double*  d_nodata_list2,
        const char*    pszSourceTo,
		const char*    pszFormatTo,
        const double   xResol      = 0.0,
        const double   yResol      = 0.0,
		enumGTDataType eType       = GTD_Unknown, 
		const char*    pszDstSRS   = NULL,
		double         dCacheLimit = 200.0);

private:
	/*Envelop�Ƿ��ཻ, ����ཻ, �����envr��*/
	static bool twoEnvelopsOverlap(
		const GTEnvelop& env1,
		const GTEnvelop& env2,
		GTEnvelop&       envr);

	/*����ˮƽ���봹ֱ�ߵ��Ƿ��ཻ, ������xi,yi*/
	static bool isSegmentsIntersect(
		const double& x1, const double& y1,  /*ˮƽ�߶���˵�*/
		const double& x2, const double& y2,  /*ˮƽ�߶��Ҷ˵�*/
		const double& x3, const double& y3,  /*��ֱ�߶��¶˵�*/
		const double& x4, const double& y4,  /*��ֱ�߶��϶˵�*/
		double&       xi, double&       yi); /*������ֵ*/

    static void replaceNoData(
        void        *pData,
        void        *pDataRef,
        int          i_size,
        double       dNodata,
        GDALDataType dType);
};

/*
 *����ʸ������ת������
 */
class GTS_DLL GTGeometryReprojector: public GTWarp
{
public:
	/*�����ʼ��, �����������ϵͳ��ͬ, ���ܹ���*/
	GTGeometryReprojector(
		GTSpatialReference* fromSRS, 
		GTSpatialReference* toSRS);

	~GTGeometryReprojector();

	/*��̬���ٺ���, �������new������*/
	static void destroyGeometryReprojector(GTGeometryReprojector *poGeomReprojector);

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

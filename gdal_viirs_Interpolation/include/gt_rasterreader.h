/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  դ������ת���Ķ�����
* Author:   ������, fanjf@lreis.ac.cn 2011��-03��-09��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����  (c) ��������Ȩ��
****************************************************************/
#pragma once
#ifndef GTRASTERWARP_H_INCLUDED
#define GTRASTERWARP_H_INCLUDED

#include "gt_layer.h"
#include "../include_p/gdal/gdalwarper.h"

namespace gts{

/*
 *RasterWarpϵ����Ļ���
 */
class GTS_DLL GTRasterReader
{
public:
	GTRasterReader(void);
	virtual ~GTRasterReader(void);
};

/*դ�����ݵ�ֱ�Ӷ�ȡ*/
class GTS_DLL GTRasterSimpleReader: public GTRasterReader
{
public:
	GTRasterSimpleReader(GTRasterDataset *pDataset);
	~GTRasterSimpleReader(void);

	static void destroyRasterSimpleReader(GTRasterSimpleReader *poSimpleReader);

	/*��ȡӰ���е�����
	 *nXOff 	�����Ͻǵ�Xƫ����
	 *nYOff 	�����Ͻǵ�Yƫ����
	 *nXSize 	��ȡ/д��Ŀ��
	 *nYSize 	��ȡ/д��ĸ߶�
	 *pData 	��ȡ/д���buffer��Ӧ�����ٰ���nBufXSize * nBufYSize * nBandCount �� eBufType ���͵��ֽ�. 
	 *nBufXSize buffer Ӱ��Ŀ��
	 *nBufYSize buffer Ӱ��ĸ߶�
	 *eBufType 	pData ��������
	 *nBandCount	��/д�Ĳ�������Ĭ��Ϊ����
	 *panBandMap 	�����б���1��ʼ����� NULL ��ӵ�һ��nBandCount��ʼ
	 *nPixelSpace   Ĭ��Ϊ0������Ķ�/д���
	 *nLineSpace 	Ĭ��Ϊ0������Ķ�/д���
	 *nBandSpace 	Ĭ��Ϊ0�����εĶ�/д��� */
	bool readDataByPixels(
		int   nXOff,
		int   nYOff,
		int   nXSize,
		int   nYSize,
		void *pData,
		int   nBufXSize,
		int   nBufYSize,
		enumGTDataType eBufType,
		int   nBandCount  = 0,
		int  *panBandMap  = NULL,
		int   nPixelSpace = 0,
		int   nLineSpace  = 0,
		int   nBandSpace  = 0);

	bool readDataByPixels(
		int   nXOff,
		int   nYOff,
		int   nXSize,
		int   nYSize,
		int   nBand,
		void *pData,
		int   nBufXSize,
		int   nBufYSize,
		enumGTDataType eBufType);

    /* ���ո����Ŀռ䷶Χ�Ĵ�С��ȡһ������
     * envΪʵ�ʵĿռ䷶Χ
	 * nBandΪ�û����ȡ�Ĳ��Σ���1��ʼ
	 * dataTypeΪ�û�ָ������������
     * *pData���ⲿ��ʼ�����ڴ�ռ�,
     * nXSize, nYSizeΪĿ����������������
	 * bReadOverviewΪ�Ƿ�ӽ�������ȡ����
	 * ע��: ����non-north_upӰ��, �÷���������
     */
    bool readDataByEnvelop(
		GTEnvelop&     env, 
		int            nBand, 
		enumGTDataType dataType, 
		void*          pData, 
		int            nXSize, 
		int            nYSize,
		bool           bReadOverview = true);

	/* ���ո����Ŀռ䷶Χ�Ĵ�С��ȡһ������
     * envΪʵ�ʵĿռ䷶Χ
	 * nBandΪ�û����ȡ�Ĳ��Σ���1��ʼ
	 * dataTypeΪ�û�ָ������������
	 * dResolX, dResolYΪָ��������ռ�ֱ���
     * *pData�ɸú��������ڴ�ռ�,�ⲿ����GTS_FREE�ͷ�
	 * *pDataӦ�����ⲿ��ʼ��Ϊ0
     * nXSize, nYSize���ص�Ϊ��ȡ��Ŀ����������������
	 * bReadOverviewΪ�Ƿ�ӽ�������ȡ����
	 * ע��: ����non-north_upӰ��, �÷���������
     */
	bool readDataByEnvelop(
		GTEnvelop&     env, 
		int            nBand, 
		enumGTDataType dataType,
		double         dResolX,
		double         dResolY,
		void**         pData, 
		int&           nXSize, 
		int&           nYSize,
		bool           bReadOverview = true); 

	/* ���ո����Ŀռ䷶Χ�Ĵ�С��ȡһ������
     * ������ΧΪ�����Ӱ��Ŀռ䷶Χ, �������ǵ�ȷ��
	 * nBandΪ�û����ȡ�Ĳ��Σ���1��ʼ
	 * dataTypeΪ�û�ָ������������
	 * dResolX, dResolYΪָ��������ռ�ֱ���
     * *pData�ɸú��������ڴ�ռ�,�ⲿ����GTS_FREE�ͷ�
	 * *pDataӦ�����ⲿ��ʼ��Ϊ0
     * nXSize, nYSize���ص�Ϊ��ȡ��Ŀ����������������
	 * bReadOverviewΪ�Ƿ�ӽ�������ȡ����
	 * ��Է�����Ӱ��, ȡ�������������ʾ��ʱ����Ҫ��������ת
     */
	bool readDataByExtent(
		const GTRawPoint&    pt_leftTop, 
		const GTRawPoint&    pt_rightBottom, 
		int            nBand, 
		enumGTDataType dataType,
		double         dResolX,
		double         dResolY,
		void**         pData, 
		int&           nXSize, 
		int&           nYSize,
		bool           bReadOverview = true); 

	/* ���ո����Ŀռ䷶Χ�Ĵ�С��ȡһ������
     * ������ΧΪ�����Ӱ��Ŀռ䷶Χ, �������ǵ�ȷ��
	 * nBandΪ�û����ȡ�Ĳ��Σ���1��ʼ
	 * dataTypeΪ�û�ָ������������
	 * dResolX, dResolYΪָ��������ռ�ֱ���
     * pData���ⲿ�����ڴ�ռ�
     * nXSize, nYSizepΪData����������
	 * bReadOverviewΪ�Ƿ�ӽ�������ȡ����
	 * ��Է�����Ӱ��, ȡ�������������ʾ��ʱ����Ҫ��������ת
     */
	bool readDataByExtent(
		const GTRawPoint&    pt_leftTop, 
		const GTRawPoint&    pt_rightBottom, 
		int            nBand, 
		enumGTDataType dataType,
		double         dResolX,
		double         dResolY,
		void*          pData, 
		int            nXSize, 
		int            nYSize,
		bool           bReadOverview = true); 

private:
	GTRasterDataset *poDataset; /*gdal���ݼ�ָ�� */
};

/*
 *������ͨ������任ת������ϵͳ֮���ȡ��������
 */
class GTS_DLL GTRasterWarpReader: public GTRasterReader
{
public:
	/*����ʱ��Ҫ����GTRasterDataset����ָ��, 
	 *Ŀ����������, �ز�������, Ŀ����������
	 *ת���Ĳ�����Ŀ���б�(��1��ʼ)
	 */
	GTRasterWarpReader(
		GTRasterDataset *pDataset, 
		const char*      srsDstWKT,          /*Ŀ����������*/
		GDALResampleAlg  resamAlg,           /*�ز�������*/
		enumGTDataType   dstDataType,        /*�����������*/
		int              iBandCount,         /*Ŀ�겨����Ŀ*/
		int             *iBandList,          /*Ŀ�겨���б�*/
		double           dCacheLimit = 0.0,  /*�ڴ�ʹ������, ��λΪMB*/
		double          *pdNodata    = 0,    /*Ŀ�����ݿ�ֵ��־, ����ǿ�,���ȱ������iBandCount*/
		GDALProgressFunc progressFunc = 0,
		void            *progressArg = 0);

	~GTRasterWarpReader(void);

	/*�ͷ�GTRasterWarpReader����ָ��ľ�̬����, pDataset������new��ʾ����*/
	static void destroyRasterWarpReader(GTRasterWarpReader *pRasterWarp);

	/*��ȡ��ȫת��֮��Ŀ��ͼ������ؿ��*/
	int getDestApprWidth() const { return destApprWidth; }

	/*��ȡ��ȫת��֮��Ŀ��ͼ������ظ߶�*/
	int getDestApprHeight() const { return destApprHeight; }

	/*��ȡת��֮��Ŀ��ͼ��ķ���任����*/
	const double* getDestAffineParams() const { return pdfDstGeoTransform; }

	/*�Ƿ���Ч*/
	bool isValid() const { return bValid; }

	/*��ȡת��֮��Ŀ��ͼ���x����ռ�ֱ���*/
	double getXApprResolution();

	/*��ȡת��֮��Ŀ��ͼ���y����ռ�ֱ���*/
	double getYApprResolution();
	
	/*��ȡת��֮��Ŀ��ͼ��Ŀռ䷶Χ*/
	bool getApprSpatialExtent(GTEnvelop &env);

	/*��ȡת����Ŀ����������*/
	enumGTDataType getDstDataType();

    /*���ո����Ŀռ䷶Χ�Ĵ�С��ȡһ������
     *envΪʵ�ʵĿռ䷶Χ
     *����*pData����Ϊ��, ����ʱ����˳��Ϊ������,���ϵ���
     *(*pData)���ڲ����䣬�ⲿ����GTS_FREE�ͷ�
     *nXSize, nYSizeΪĿ����������������, 
     *��getX(Y)ApprResolution��env��С���Խ��м������1:1�Ĵ�Сֵ
     */
    bool readWarpedData(
		const GTEnvelop& env, 
		void**           pData, 
		int              nXSize, 
		int              nYSize);

	void setProgressData(GDALProgressFunc progressFunc, void* progressArg);

private:
	GDALDataset     *poDataset;     /*gdal���ݼ�ָ�� */
	const char      *pszSrcWKT;     /*ԭʼ�����궨��  */
	const char      *pszDstWKT;     /*Ŀ�����궨��    */
	GDALWarpOptions *psWarpOptions; /*Warp�����ṹָ��*/

	int    destApprWidth;        /*Ŀ�����ݿ�Ľ��ƿ��*/
	int    destApprHeight;       /*Ŀ�����ݿ�Ľ��Ƹ߶�*/
	double pdfDstGeoTransform[6];/*Ŀ��ͶӰ֮����任����*/
	bool   bValid;
};

}
#endif

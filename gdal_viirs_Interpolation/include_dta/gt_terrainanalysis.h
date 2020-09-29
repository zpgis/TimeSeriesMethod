/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ����DEM���������� (�����㷨�ο�GRASS��GDAL)
* Author:   ������, fanjf@lreis.ac.cn, 2010��-12��-08��
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* ��Ȩ����   (c) �й���ѧԺ�����ѧ����Դ�о���. ��������Ȩ��
****************************************************************/
#ifndef GTERRAINANALYSIS_H_INCLUDED
#define GTERRAINANALYSIS_H_INCLUDED

#include "gts_dta_port.h"
#include "gt_solpos.h"
#include "ogr_spatialref.h"
#include "gdalwarper.h"
#include "cpl_string.h"

#define EQUAL_TO_NODATA(a,b) ARE_REAL_EQUAL(a, b)
#define INTERPOL(a,b) ((bSrcHasNoData && (EQUAL_TO_NODATA(a, fSrcNoDataValue) || EQUAL_TO_NODATA(b, fSrcNoDataValue))) ? fSrcNoDataValue : 2 * (a) - (b))

namespace gts{

class GTS_DLL GTTerrainAnalysis
{
public:
	GTTerrainAnalysis(void);
	~GTTerrainAnalysis(void);

	/*�����ꡢ��(1-365)��ʱ��(0-24)�� ʱ���͵ص�
	 *����̫���߶ȽǺͷ�λ��(0-90, 0-360, Ϊ�Ƕ�)
	 */
	static bool getSunPosition(
		int       year,   /*��*/
		int       day,    /*��*/
		GTFDTime& sTime,  /*ʱ����ʱ��*/
		double    sLat,   /*γ��*/
		double    sLon,   /*����*/
		double&   sAzimA, /*����: ��λ��*/
		double&   sElevA);/*����: �߶Ƚ�*/

	/** @brief  ��դ�����ɵ�ֵ��ͼ(ESRI ShapeFile)
	 *  @return �����Ƿ�ɹ�
	 *  @note   ��dfContourBase=10, dfContourInterval=5,
	 *          ������10, 15, 20, 25...����ĵ�ֵ��
	 *          �����������ΪLineString
	 *          fieldNameָ���������е�����
	 *          ����Ŀռ�ο����������դ������һ��
	 */
	static bool generateContour(
		const char* imageSource,      /*����դ������·��, ���������ַ���ת��utf8����*/
		int         iBand,            /*Ŀ�겨��*/
		double  	dfContourInterval,/*��ֵ�߼��*/
		double  	dfContourBase,    /*������ֵ��ֵ*/
		bool  	    bUseNoData,       /*�Ƿ����NoData*/
		double  	dfNoDataValue,    /*NoData����ֵ*/
		const char* targetSource,     /*Ŀ��Shape�ļ�·��*/
		const char* layerName,        /*Ŀ��������*/
		const char* fieldName         /*�������ֵ����, ����Ϊ64λ˫����*/  
		);

	/*������Ӱ
	 *�ڼ�����z��scaleΪ��ֵ��ϵ, ���ͨ����z��Ϊ0
	 *��ͨ������scale����ú�����ʾ����
	 *scaleӦ���Ǹ���ˮƽ��λ(������ϵͳ�й�)�͸߶ȵ�λ��ͨ��Ϊm���ı���������
	 */
	static bool generateHillShade(
		const char* pszSourceFrom,         /*����Դ·��*/
		int         iBand,                 /*����*/
		const char* pszSourceTo,           /*Ŀ������Դ·��*/
		const char* pszFormatTo = "GTiff", /*Ŀ������Դ��ʽ*/
		double      az     = 315.0,        /*̫����λ�� degree*/
		double      alt    = 45.0,         /*̫���߶Ƚ� degree*/
		double      z      = 1.0,          /*zֵ�ı�������*/
	    double      scale  = 50000.0,      /*ˮƽ�봹ֱ�ĳ߶�����*/
		bool        bEdges = false,        /*�Ƿ�����Ե*/
	    bool        bZevenbergenThorne = true /*falseΪHorn�㷨(�ʺϴֵ���)*/
		);
	
	/*����Color relief map
	 *������ɫ��, nodataҲ��Ҫָ����ɫֵ
	 *������ɫ��������(qsort)
	 */
	static bool generateColorRelief(
		const char*             pszSourceFrom,                                    /*����Դ·��*/
		int                     iBand,                                            /*����*/
		const char*             pszSourceTo,                                      /*Ŀ������Դ·��*/
		GTDEMColorEntry*        pasColorAssociation,                              /*��ɫ��*/
		int                     nColorAssociation,                                /*��ɫ��Ŀ*/
		GTDEMColorEntry         noDataColor,                                      /*����ɫ*/
		const char*             pszFormatTo         = "GTiff",                    /*Ŀ������Դ��ʽ*/
		bool                    bAddAlpha           = false,                      /*alphaͨ��*/
		GTDEMColorSelectionMode eColorSelectionMode = COLOR_SELECTION_INTERPOLATE /*��ɫѡȡ����*/
		);

	static bool generateColorRelief2(
		const char*             pszSourceFrom,                                    /*����Դ·��*/
		int                     iBand,                                            /*����*/
		const char*             pszSourceTo,                                      /*Ŀ������Դ·��*/
		double					hsv_s[3],                              /*��ɫ��*/
		double					hsv_e[3],                                /*��ɫ��Ŀ*/
		int						rgb_bg[3],                                      /*����ɫ*/
		const char*             pszFormatTo         = "GTiff",                    /*Ŀ������Դ��ʽ*/
		bool                    bAddAlpha           = false,                      /*alphaͨ��*/
		GTDEMColorSelectionMode eColorSelectionMode = COLOR_SELECTION_MIN_MAX /*��ɫѡȡ����*/
		);

	/*�����¶�
	 *scaleΪˮƽ�ʹ�ֱ�ı�������
	 *Ӧ���Ǹ���ˮƽ��λ(������ϵͳ�й�)�͸߶ȵ�λ��ͨ��Ϊm���ı���(��ͬһ��λ��)������
	 *����ò�����ȷ��ֵ.
	 */
	static bool generateSlope(
		const char*   pszSourceFrom,               /*����Դ·��*/
		int           iBand,                       /*����*/
		const char*   pszSourceTo,                 /*Ŀ������Դ·��*/
		const char*   pszFormatTo        = "GTiff",/*Ŀ������Դ��ʽ*/
		int           iFormat            = 1,      /*1Ϊ�Ƕ�, 0Ϊ�ٷֱ�*/
		double        scale              = 1.0,    /*ˮƽ�봹ֱ�ĳ߶�����*/
		bool          bEdges             = false,  /*�Ƿ�����Ե*/
		bool          bZevenbergenThorne = true    /*falseΪHorn�㷨(�ʺϴֵ���)*/
		);

	/*
	 *��������, �ֲ�ˮƽ��ΪNoData����ֵ
	 */
	static bool generateAspect(
		const char*   pszSourceFrom,               /*����Դ·��*/
		int           iBand,                       /*����*/
		const char*   pszSourceTo,                 /*Ŀ������Դ·��*/
		const char*   pszFormatTo        = "GTiff",/*Ŀ������Դ��ʽ*/
		int           iFormat            = 1,      /*1��������, 0��������*/
		bool          bEdges             = false,  /*�Ƿ�����Ե*/
		bool          bZevenbergenThorne = true    /*falseΪHorn�㷨(�ʺϴֵ���)*/
		);

	static bool generateAspectDEM(
		GTFeatureClass& inFeatures, 
		int column, 
		int row, 
		int maxMemorySize,
		const char* pszSourceTo,           /*Ŀ������Դ·��*/
		const char* pszFormatTo = "GTiff" /*Ŀ������Դ��ʽ*/
		);

	static bool generateSurfaceCurvature(
		const char*			pszSourceFrom,               /*����Դ·��*/
		int					iBand,                       /*����*/
		const char*			pszSourceTo,                 /*Ŀ������Դ·��*/
		const char*			pszFormatTo			= "GTiff",/*Ŀ������Դ��ʽ*/
		double				scale				= 1.0,    /*ˮƽ�봹ֱ�ĳ߶�����*/
		bool				bEdges				= false,  /*�Ƿ�����Ե*/
		GTDEMCurvatureType	CurvatureType		= CURVATURE_NORMAL	   /*�����㷨�ı��*/
		);

	/*****************************************************************************
	* @brief	���������������ս������Ե�����Ԫ���������������
	* @param	[in]
	*			pFile		DEM����
	*			iBand		���κ�
	*			colsMin		������СX���кţ�
	*			rowsMin		������СY���кţ�
	*			colsMax		�������X���кţ�
	*			rowsMax		�������Y���кţ�
	*			elevation	Ŀ��߳�ֵ
	* @param	[out]
	* @return	
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-19
	*******************************************************************************/
	static double dig(
		const char *pFile,int iBand,int colsMin,int rowsMin,int colsMax,int rowsMax,double elevation);
	/*****************************************************************************
	* @brief	���������������ս������Ե�����Ԫ���������������
	* @param	[in]
	*			pFile		DEM����
	*			iBand		���κ�
	*			colsMin		������СX���кţ�
	*			rowsMin		������СY���кţ�
	*			colsMax		�������X���кţ�
	*			rowsMax		�������Y���кţ�
	*			elevation	Ŀ��߳�ֵ
	* @param	[out]
	* @return	
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-19
	*******************************************************************************/
	static double fill(
		const char *pFile,int iBand,int colsMin,int rowsMin,int colsMax,int rowsMax,double elevation);
private:
	typedef float (*__generic3x3ProcessingAlg) (float* pafWindow, float fDstNoDataValue, void* pData);
	
	static float __computeVal(
		int                       bSrcHasNoData, 
		float                     fSrcNoDataValue,
		float*                    afWin, 
		float                     fDstNoDataValue,
		__generic3x3ProcessingAlg pfnAlg,
		void*                     pData,
		int                       bComputeAtEdges);

	static bool __generic3x3Processing(
		GDALRasterBandH           hSrcBand,
		GDALRasterBandH           hDstBand,
		__generic3x3ProcessingAlg pfnAlg,
		void*                     pData,
		int                       bComputeAtEdges);

	/*���� shade��ͨ�㷨*/
	static float __hillshadeAlg(
		float* afWin, /*8�ھӴ���*/
		float  fDstNoDataValue, 
		void*  pData);

	/*���� shadeZevenbergen-Thorne�㷨*/
	static float __hillshadeZevenbergenThorneAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*shade�����ĳ�ʼ��*/
	static void* __createHillshadeData(
		double* adfGeoTransform,
		double  z,
		double  scale,
		double  alt,
		double  az,
		int     bZevenbergenThorne);

	/*����Ϊ����color relief���Ӻ���*/
	/*���ո̵߳ıȽϺ���*/
	static int __colorReliefSortColors(
		const void* pA, 
		const void* pB);

	/*����ɫ�����Ŀ����ɫֵ*/
	static int __colorReliefGetRGBA (
		GTDEMColorEntry*        pasColorAssociation,
		int                     nColorAssociation,
		double                  dfVal,
		GTDEMColorSelectionMode eColorSelectionMode,
		int*                    pnR,
		int*                    pnG,
		int*                    pnB,
		int*                    pnA);

	static int hsv2rgb(const double hsv[3], int (&rgb)[3]);

	static int __colorReliefGetRGBA2 (
		GDALRasterBandH         hSrcBand,
		double					hsv_s[3],                         
		double					hsv_e[3],
		double                  dfVal,
		GTDEMColorSelectionMode eColorSelectionMode,
		int*                    pnR,
		int*                    pnG,
		int*                    pnB,
		int*                    pnA);
	
	/*������ɫ�����Color Relief*/
	static bool __colorRelief (
		GDALRasterBandH         hSrcBand,
		GDALRasterBandH         hDstBand1,
		GDALRasterBandH         hDstBand2,
		GDALRasterBandH         hDstBand3,
		GDALRasterBandH         hDstBand4,
		GTDEMColorSelectionMode eColorSelectionMode,
		GTDEMColorEntry*        pasColorAssociation,
		int                     nColorAssociation,
		GTDEMColorEntry         noDataColor);

	static bool __colorRelief2 (
		GDALRasterBandH         hSrcBand,
		GDALRasterBandH         hDstBand1,
		GDALRasterBandH         hDstBand2,
		GDALRasterBandH         hDstBand3,
		GDALRasterBandH         hDstBand4,
		GTDEMColorSelectionMode eColorSelectionMode,
		double					hsv_s[3],                         
		double					hsv_e[3],
		int						rgb_bg[3]);

	/*�¶ȵ�Horn�㷨*/
	static float __slopeHornAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*�¶ȵ�Zevenbergen-Thorne�㷨*/
	static float __slopeZevenbergenThorneAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*�¶Ȳ����ĳ�ʼ��*/
	static void* __createSlopeData(
		double* adfGeoTransform,
		double  scale,
		int     slopeFormat);

	/*�����Horn�㷨*/
	static float __aspectAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*�����Zevenbergen-Thorne�㷨*/
	static float __aspectZevenbergenThorneAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*���������ʼ��*/
	static void* __createAspectData(
		int bAngleAsAzimuth);

	/*�ر�ֲڶ�ָ���������ͨ�㷨*/
	static float __TRIAlg (
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*�ر�ֲڶ�ָ�������DOCELL�㷨*/
	static float __TRIDOCELLAlg (
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*����һ������ĺ���*/
	static void* __createGeneralData(
		double* adfGeoTransform,
		double  scale);

	/***************************************
	 * ����Ϊ��ͬ���ʵļ��㷽��, �ο�Shary (2002)
	 * ��ʹ�õ��Ĺ�ͬ����Ϊ
	 ***************************************/
	
	/*slope steepness 3*3�����㷨*/
	static float __slopeSteepnessAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*slope gradient 3*3�����㷨*/
	static float __slopeGradientAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*gradient factor3*3�����㷨*/
	static float __gradientFactorAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*mean curvature (��λ1/m) 3*3�����㷨*/
	static float __meanCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*difference curvature (��λ1/m) 3*3�����㷨*/
	static float __differenceCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*horizontal curvature (��λ1/m) 3*3�����㷨*/
	static float __horizontalCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*plan curvature (��λ1/m) 3*3�����㷨
	 *������¶ȷ�������������
	 */
	static float __planCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*profile curvature (��λ1/m) 3*3�����㷨
	 *��������¶ȷ��������
	 */
	static float __profileCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	/*normal curvature (��λ1/m) 3*3�����㷨
	 *=(profile - plan)
	 *=-2.0*(D+E)
	 *D=((e3+e5)/2-e4)/L^2
	 *E=((e1+e7)/2-e4)/L^2
          0 1 2
          3 4 5
          6 7 8
	 */
	static float __normalCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __rotorAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __unsphericityAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __horizontalExcessCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __verticalExcessCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __minimalCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __maximalCurvatureAlg(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __totalGaussianCurvature(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __totalRingCurvature(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __totalAccumulationAurvature(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

	static float __slopeDirection(
		float* afWin, 
		float  fDstNoDataValue, 
		void*  pData);

};

}
#endif

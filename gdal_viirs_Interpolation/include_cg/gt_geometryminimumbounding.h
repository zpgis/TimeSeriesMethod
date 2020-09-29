/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ���ζ�����С�߽�
* Author:   ������, fanjf@lreis.ac.cn,2011-09-22
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c)  ��������Ȩ��
****************************************************************/
#ifndef GT_GEOMETRYMINIMUMBOUNDING_H_INCLUDE
#define	GT_GEOMETRYMINIMUMBOUNDING_H_INCLUDE

#include "gts_cg_port.h"

namespace gts{

	class GTS_DLL GTGeometryMinimumBounding
	{
	public:
		GTGeometryMinimumBounding(void);
		~GTGeometryMinimumBounding(void);

		/*****************************************************************************
		* @brief	�㼯��͹��
		* @param	[in]
		*			pointList			�㼯
		*			pointNum			��ĸ���	
		*			[out]
		*			pHullRing			͹��������
		* @return	����͹�������Ƿ�ɹ�
		* @note		���ص�͹����������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsContexHull(
			std::vector<GTRawPoint>&	pointList, 
			int							pointNum, 
			GTRing*						pHullRing);

		/*****************************************************************************
		* @brief	�ߵ�͹��
		* @param	[in]
		*			lineStrObj			�߶���
		*			[out]
		*			pHullRing			͹��������
		* @return	����͹�������Ƿ�ɹ�
		* @note		���ص�͹����������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringContexHull(
			GTLineString&	lineStrObj,
			GTRing*			pHullRing);

		/*****************************************************************************
		* @brief	���ߵ�͹��
		* @param	[in]
		*			mutiLineStrObj		���߶���
		*			[out]
		*			pHullRing			͹��������
		* @return	����͹�������Ƿ�ɹ�
		* @note		���ص�͹����������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringContexHull(
			GTMultiLineString&	mutiLineStrObj,
			GTRing*				pHullRing);

		/*****************************************************************************
		* @brief	����͹��
		* @param	[in]
		*			ringObj				������
		*			[out]
		*			pHullRing			͹��������
		* @return	����͹�������Ƿ�ɹ�
		* @note		���ص�͹����������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringContexHull(
			GTRing& ringObj,
			GTRing* pHullRing);

		/*****************************************************************************
		* @brief	����ε�͹��
		* @param	[in]
		*			polyObj				����ζ���
		*			[out]
		*			pHullRing			͹��������
		* @return	����͹�������Ƿ�ɹ�
		* @note		���ص�͹����������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonContexHull(
			GTPolygon&	polyObj,
			GTRing*		pHullRing);

		/*****************************************************************************
		* @brief	�����ε�͹��
		* @param	[in]
		*			multiPolyObj		�����ζ���
		*			[out]
		*			pHullRing			͹��������
		* @return	����͹�������Ƿ�ɹ�
		* @note		���ص�͹����������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonContexHull(
			GTMultiPolygon& multiPolyObj,
			GTRing*			pHullRing);

		static bool pointsContexHull(std::vector<GTPoint *>& pointList,
			int pointNum, GTRing* pHullRing);

		static bool multiPointContexHull(GTMultiPoint& multiPointObj,
			GTRing* pHullRing);

		static bool geometryContexHull(GTGeometry* geom, GTRing* pHullRing);

		static bool featureClassContexHull(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);

		/*****************************************************************************
		* @brief	�㼯���������
		* @param	[in]
		*			pointList			�㼯
		*			pointNum			�����
		*			[out]
		*			pEnvelop			������ζ���
		* @return	����������ζ����Ƿ�ɹ�
		* @note		���ص�������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsEnvelop(
			std::vector<GTRawPoint *>&	pointList, 
			int							pointNum, 
			GTEnvelop*					pEnvelop);

		/*****************************************************************************
		* @brief	�ߵ��������
		* @param	[in]
		*			lineStrObj			�߶���
		*			[out]
		*			pEnvelop			������ζ���
		* @return	����������ζ����Ƿ�ɹ�
		* @note		���ص�������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringEnvelop(
			GTLineString&	lineStrObj,
			GTEnvelop*		pEnvelop);

		/*****************************************************************************
		* @brief	���ߵ��������
		* @param	[in]
		*			multiLineStrObj		���߶���
		*			[out]
		*			pEnvelop			������ζ���
		* @return	����������ζ����Ƿ�ɹ�
		* @note		���ص�������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringEnvelop(
			GTMultiLineString&	multiLineStrObj,
			GTEnvelop*			pEnvelop);

		/*****************************************************************************
		* @brief	�����������
		* @param	[in]
		*			ringObj				������
		*			[out]
		*			pEnvelop			������ζ���
		* @return	����������ζ����Ƿ�ɹ�
		* @note		���ص�������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringEnvelop(
			GTRing&		ringObj,
			GTEnvelop*	pEnvelop);

		/*****************************************************************************
		* @brief	����ε��������
		* @param	[in]
		*			polyObj				����ζ���
		*			[out]
		*			pEnvelop			������ζ���
		* @return	����������ζ����Ƿ�ɹ�
		* @note		���ص�������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonEnvelop(
			GTPolygon& polyObj,
			GTEnvelop* pEnvelop);

		/*****************************************************************************
		* @brief	�����ε��������
		* @param	[in]
		*			polyObj				����ζ���
		*			[out]
		*			pEnvelop			������ζ���
		* @return	����������ζ����Ƿ�ɹ�
		* @note		���ص�������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonEnvelop(
			GTMultiPolygon& multiPolyObj,
			GTEnvelop*		pEnvelop);

		static bool pointsEnvelop(std::vector<GTPoint *>& pointList, int pointNum,
			GTEnvelop* pEnvelop);

		static bool multiPointEnvelop(GTMultiPoint& multiPointObj,
			GTEnvelop* pEnvelop);

		static bool geometryEnvelop(GTGeometry* geom, GTEnvelop* pEnvelop);

		static bool featureClassEnvelop(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);

		/*****************************************************************************
		* @brief	�㼯�Ŀ��ٰ�ΧԲ
		* @param	[in]
		*			pointList				�㼯
		*			pointNum				�������
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pBoundingBall			���ɰ�ΧԲ�Ķ���
		* @return	�������ٰ�ΧԲ�����Ƿ�ɹ�
		* @note		���صĿ��ٰ�ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsFastBoundingBall(
			std::vector<GTRawPoint *>&	pointList, 
			int							pointNum, 
			GTRawPoint&					center,
			double*						rad,
			GTRing*						pBoundingBall = NULL);

		/*****************************************************************************
		* @brief	�ߵĿ��ٰ�ΧԲ
		* @param	[in]
		*			lineStrObj				�߶���
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pBoundingBall			���ɰ�ΧԲ�Ķ���
		* @return	�������ٰ�ΧԲ�����Ƿ�ɹ�
		* @note		���صĿ��ٰ�ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringFastBoundingBall(
			GTLineString&	lineStrObj,
			GTRawPoint&		center,
			double*			rad,
			GTRing*			pBoundingBall = NULL);

		/*****************************************************************************
		* @brief	���ߵĿ��ٰ�ΧԲ
		* @param	[in]
		*			multilineStrObj			���߶���
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pBoundingBall			���ɰ�ΧԲ�Ķ���
		* @return	�������ٰ�ΧԲ�����Ƿ�ɹ�
		* @note		���صĿ��ٰ�ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringFastBoundingBall(
			GTMultiLineString&	multiLineStrObj,
			GTRawPoint&			center,
			double*				rad,
			GTRing*				pBoundingBall = NULL);

		/*****************************************************************************
		* @brief	�����ٰ�ΧԲ
		* @param	[in]
		*			ringObj					������
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pBoundingBall			���ɰ�ΧԲ�Ķ���
		* @return	�������ٰ�ΧԲ�����Ƿ�ɹ�
		* @note		���صĿ��ٰ�ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringFastBoundingBall(
			GTRing&		ringObj,
			GTRawPoint& center,
			double*		rad,
			GTRing*		pBoundingBall = NULL);

		/*****************************************************************************
		* @brief	����ο��ٰ�ΧԲ
		* @param	[in]
		*			polyObj					����ζ���
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pBoundingBall			���ɰ�ΧԲ�Ķ���
		* @return	�������ٰ�ΧԲ�����Ƿ�ɹ�
		* @note		���صĿ��ٰ�ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonFastBoundingBall(
			GTPolygon&	polyObj,
			GTRawPoint& center,
			double*		rad,
			GTRing*		pBoundingBall = NULL);

		/*****************************************************************************
		* @brief	�����ο��ٰ�ΧԲ
		* @param	[in]
		*			multiPolyObj			�����ζ���
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pBoundingBall			���ɰ�ΧԲ�Ķ���
		* @return	�������ٰ�ΧԲ�����Ƿ�ɹ�
		* @note		���صĿ��ٰ�ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonFastBoundingBall(
			GTMultiPolygon&	multiPolyObj,
			GTRawPoint&		center,
			double*			rad,
			GTRing*			pBoundingBall = NULL);

		static bool pointsFastBoundingBall(std::vector<GTPoint *>& pointList,
			int pointNum, GTRawPoint& center, double* rad,
			GTRing* pBoundingBall = NULL);

		static bool multiPointFastBoundingBall(GTMultiPoint& multiPointObj,
			GTRawPoint& center, double* rad, GTRing* pBoundingBall = NULL);

		static bool geometryFastBoundingBall(GTGeometry* geom, GTRawPoint& center,
			double* rad, GTRing* pBoundingBall = NULL);

		static bool featureClassFastBoundingBall(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);

		/*****************************************************************************
		* @brief	�㼯����С��ΧԲ
		* @param	[in]
		*			pointList				�㼯
		*			pointNum				��ĸ���
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pMinCircle				���ɰ�ΧԲ�Ķ���
		* @return	������С��ΧԲ�����Ƿ�ɹ�
		* @note		���ص���С��ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsMinimumCircle(
			std::vector<GTRawPoint *>&	pointList, 
			int							pointNum, 
			GTRawPoint&					center,
			double*						rad,
			GTRing*						pMinCircle = NULL);

		/*****************************************************************************
		* @brief	�ߵ���С��ΧԲ
		* @param	[in]
		*			lineStrObj				�߶���
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pMinCircle				���ɰ�ΧԲ�Ķ���
		* @return	������С��ΧԲ�����Ƿ�ɹ�
		* @note		���ص���С��ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringMinimumCircle(
			GTLineString&	lineStrObj,
			GTRawPoint&		center,
			double*			rad,
			GTRing*			pMinCircle = NULL);

		/*****************************************************************************
		* @brief	���ߵ���С��ΧԲ
		* @param	[in]
		*			multiLineStrObj			���߶���
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pMinCircle				���ɰ�ΧԲ�Ķ���
		* @return	������С��ΧԲ�����Ƿ�ɹ�
		* @note		���ص���С��ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringMinimumCircle(
			GTMultiLineString&	multiLineStrObj,
			GTRawPoint&			center,
			double*				rad,
			GTRing*				pMinCircle = NULL);

		/*****************************************************************************
		* @brief	������С��ΧԲ
		* @param	[in]
		*			ringObj					������
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pMinCircle				���ɰ�ΧԲ�Ķ���
		* @return	������С��ΧԲ�����Ƿ�ɹ�
		* @note		���ص���С��ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringMinimumCircle(
			GTRing&		ringObj,
			GTRawPoint& center,
			double*		rad,
			GTRing*		pMinCircle = NULL);

		/*****************************************************************************
		* @brief	����ε���С��ΧԲ
		* @param	[in]
		*			polyObj					����ζ���
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pMinCircle				���ɰ�ΧԲ�Ķ���
		* @return	������С��ΧԲ�����Ƿ�ɹ�
		* @note		���ص���С��ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonMinimumCircle(
			GTPolygon&	polyObj,
			GTRawPoint& center,
			double*		rad,
			GTRing*		pMinCircle = NULL);

		/*****************************************************************************
		* @brief	�����ε���С��ΧԲ
		* @param	[in]
		*			polyObj					����ζ���
		*			[out]
		*			center					Բ��
		*			rad						�뾶
		*			pMinCircle				���ɰ�ΧԲ�Ķ���
		* @return	������С��ΧԲ�����Ƿ�ɹ�
		* @note		���ص���С��ΧԲ������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonMinimumCircle(
			GTMultiPolygon&	multiPolyObj,
			GTRawPoint&		center,
			double*			rad,
			GTRing*			pMinCircle = NULL);

		static bool pointsMinimumCircle(std::vector<GTPoint *>& pointList,
			int pointNum, GTRawPoint& center, double* rad,
			GTRing* pMinCircle = NULL);

		static bool multiPointMinimumCircle(GTMultiPoint& multiPointObj,
			GTRawPoint& center, double* rad, GTRing* pMinCircle = NULL);

		static bool geometryMinimumCircle(GTGeometry* geom, GTRawPoint& center,
			double* rad, GTRing* pMinCircle = NULL);

		static bool featureClassMinimumCircle(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);

		/*****************************************************************************
		* @brief	�㼯����С�������
		* @param	[in]
		*			pointList				�㼯
		*			pointNum				�������
		*			[out]
		*			pMinAreaRect			��С�������
		* @return	������С������ζ����Ƿ�ɹ�
		* @note		���ص���С������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsMinimumAreaRect(
			std::vector<GTRawPoint>&	pointList, 
			int							pointNum, 
			GTRing*						pMinAreaRect);

		/*****************************************************************************
		* @brief	�ߵ���С�������
		* @param	[in]
		*			lineStrObj				�߶���
		*			[out]
		*			pMinAreaRect			��С�������
		* @return	������С������ζ����Ƿ�ɹ�
		* @note		���ص���С������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringMinimumAreaRect(
			GTLineString&	lineStrObj,
			GTRing*			pMinAreaRect);

		/*****************************************************************************
		* @brief	�ߵ���С�������
		* @param	[in]
		*			lineStrObj				�߶���
		*			[out]
		*			pMinAreaRect			��С�������
		* @return	������С������ζ����Ƿ�ɹ�
		* @note		���ص���С������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringMinimumAreaRect(
			GTMultiLineString&	multiLineStrObj,
			GTRing*				pMinAreaRect);

		/*****************************************************************************
		* @brief	������С�������
		* @param	[in]
		*			ringObj					������
		*			[out]
		*			pMinAreaRect			��С�������
		* @return	������С������ζ����Ƿ�ɹ�
		* @note		���ص���С������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringMinimumAreaRect(
			GTRing& ringObj,
			GTRing* pMinAreaRect);

		/*****************************************************************************
		* @brief	����ε���С�������
		* @param	[in]
		*			polyObj					����ζ���
		*			[out]
		*			pMinAreaRect			��С�������
		* @return	������С������ζ����Ƿ�ɹ�
		* @note		���ص���С������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonMinimumAreaRect(
			GTPolygon&	polyObj,
			GTRing*		pMinAreaRect);

		/*****************************************************************************
		* @brief	�����ε���С�������
		* @param	[in]
		*			multiPolyObj			�����ζ���
		*			[out]
		*			pMinAreaRect			��С�������
		* @return	������С������ζ����Ƿ�ɹ�
		* @note		���ص���С������ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonMinimumAreaRect(
			GTMultiPolygon&	multiPolyObj,
			GTRing*			pMinAreaRect);

		static bool pointsMinimumAreaRect(std::vector<GTPoint *>& pointList,
			int							pointNum, 
			GTRing*						pMinAreaRect);

		static bool multiPointMinimumAreaRect(GTMultiPoint& multiPointObj,
			GTRing*			pMinAreaRect);

		static bool geometryMinimumAreaRect(GTGeometry* geom,
			GTRing*			pMinAreaRect);

		static bool featureClassMinimumAreaRect(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);


		/*****************************************************************************
		* @brief	�㼯����С��Ⱦ���
		* @param	[in]
		*			pointList				�㼯
		*			pointNum				�������
		*			[out]
		*			pMinWidthRect			��С��Ⱦ���
		* @return	������С��Ⱦ��ζ����Ƿ�ɹ�
		* @note		���ص���С��Ⱦ��ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool pointsMinimumWidthRect(
			std::vector<GTRawPoint>&	pointList, 
			int							pointNum, 
			GTRing*						pMinWidthRect);

		/*****************************************************************************
		* @brief	�ߵ���С��Ⱦ���
		* @param	[in]
		*			lineStrObj				�߶���
		*			[out]
		*			pMinWidthRect			��С��Ⱦ���
		* @return	������С��Ⱦ��ζ����Ƿ�ɹ�
		* @note		���ص���С��Ⱦ��ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool lineStringMinimumWidthRect(
			GTLineString&	lineStrObj,
			GTRing*			pMinWidthRect);

		/*****************************************************************************
		* @brief	���ߵ���С��Ⱦ���
		* @param	[in]
		*			multiLineStrObj			���߶���
		*			[out]
		*			pMinWidthRect			��С��Ⱦ���
		* @return	������С��Ⱦ��ζ����Ƿ�ɹ�
		* @note		���ص���С��Ⱦ��ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiLineStringMinimumWidthRect(
			GTMultiLineString&	multiLineStrObj,
			GTRing*				pMinWidthRect);

		/*****************************************************************************
		* @brief	������С��Ⱦ���
		* @param	[in]
		*			ringObj					������
		*			[out]
		*			pMinWidthRect			��С��Ⱦ���
		* @return	������С��Ⱦ��ζ����Ƿ�ɹ�
		* @note		���ص���С��Ⱦ��ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool ringMinimumWidthRect(
			GTRing& ringObj,
			GTRing* pMinWidthRect);

		/*****************************************************************************
		* @brief	����ε���С��Ⱦ���
		* @param	[in]
		*			polyObj					����ζ���
		*			[out]
		*			pMinWidthRect			��С��Ⱦ���
		* @return	������С��Ⱦ��ζ����Ƿ�ɹ�
		* @note		���ص���С��Ⱦ��ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool polygonMinimumWidthRect(
			GTPolygon&	polyObj,
			GTRing*		pMinWidthRect);

		/*****************************************************************************
		* @brief	�����ε���С��Ⱦ���
		* @param	[in]
		*			multiPolyObj			�����ζ���
		*			[out]
		*			pMinWidthRect			��С��Ⱦ���
		* @return	������С��Ⱦ��ζ����Ƿ�ɹ�
		* @note		���ص���С��Ⱦ��ζ�����Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-09-22
		*******************************************************************************/
		static bool multiPolygonMinimumWidthRect(
			GTMultiPolygon&	multiPolyObj,
			GTRing*			pMinWidthRect);

		static bool pointsMinimumWidthRect(std::vector<GTPoint *>& pointList,
			int							pointNum, 
			GTRing*						pMinWidthRect);

		static bool multiPointMinimumWidthRect(GTMultiPoint& multiPointObj,
			GTRing*			pMinWidthRect);

		static bool geometryMinimumWidthRect(GTGeometry* geom,
			GTRing*			pMinWidthRect);

		static bool featureClassMinimumWidthRect(GTFeatureClass& srcClass,
			GTFeatureClass& desClass);
	};

}
#endif

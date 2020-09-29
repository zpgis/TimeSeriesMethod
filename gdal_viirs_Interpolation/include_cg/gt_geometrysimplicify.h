
/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  �ߵļ�
* 
* Author:   ������, fanjf@lreis.ac.cn 2011-05-11
* Author:   ������, fanjf@lreis.ac.cn, 2011-07-19
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c) ��������Ȩ��
****************************************************************/
#ifndef GTSGEOMETRYSIMPLICIFY_H_INCLUDED
#define GTSGEOMETRYSIMPLICIFY_H_INCLUDED

#include "gts_cg_port.h"
#include <iostream>

namespace gts{

typedef enum e_SimpMethod
{
	DouglasPeucker = 0,
	Lang = 1,
	Reumann_witkam = 2,
	Vertex_reduction = 3,
	Opheim = 4,
	NThPoint = 5,
	PerpendDist = 6
}SimpMethod;

class GTS_DLL GTGeometrySimplicify
{
public:
	GTGeometrySimplicify(void);
	~GTGeometrySimplicify(void);


	/*****************************************************************************
	* @brief	�ߵļ򻯣�����Ҫ���еĳ��󼸺����ͺͶ���������ʹ��douglasPeucker
	* @param	[in]
	*			geoToSimpfy  	���ζ���
	*			dThresh			���ݲ�
	*			simMethod		�߼򻯷���
	*			[out]
	*			pLinestringObj	�򻯺���
	* @note     ֱ�Ӹı䴫���geoToSimpfy�����Ƿ����µļ򻯺�ļ���
	* @return	
	* @author	����ƣ�zyk@lreis.ac.cn,2012-05-22
	*******************************************************************************/
	static bool geometrySimplicify(GTGeometry* geoToSimpfy,
								   double	   dThresh);

	/*Gouglas-Peucker���㷨, pLinestringObj�е㽫���ı�, dThreshΪ��ֵ*/
	static bool douglasPeucker(
		GTLineString *pLinestringObj, 
		double        dThresh = 1.0);

	/*Gouglas-Peucker���㷨, pLinestringObj�е㽫���ı�, dThreshΪ��ֵ*/
	static GTLineString* douglasPeucker_clr(
		GTLineString *pLinestringObj, 
		double        dThresh = 1.0);

	/*****************************************************************************
	* @brief	�ߵļ�(lang)
	* @param	[in]
	*			pLinestringObj	�߶���
	*			dThresh			���ݲ�
	*           look_ahead      ������Χ
	*			[out]
	*			pLinestringObj	�򻯺���
	* @return	�򻯺����е������
	* @note		����look_ahead��Ҫ����20,20ֻ��ȡ��%5��ĵ�
	* @author	����ƣ�zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
    static int lang ( 
		GTLineString *pLinestringObj, 
		double dThresh, 
		int look_ahead );

	/*****************************************************************************
	* @brief	�ߵļ�(reumann_witkam)
	* @param	[in]
	*			pLinestringObj	�߶���
	*			dThresh			���ݲ�
	*			[out]
	*			pLinestringObj	�򻯺���
	* @return	�򻯺����е������
	* @note		
	* @author	����ƣ�zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
	static int reumann_witkam ( 
		GTLineString *pLinestringObj, 
		double dThresh );

	/*****************************************************************************
	* @brief	�ߵļ�(vertex_reduction)
	* @param	[in]
	*			pLinestringObj	�߶���
	*			dThresh			���ݲ�
	*			[out]
	*			pLinestringObj	�򻯺���
	* @return	�򻯺����е������
	* @note		
	* @author	����ƣ�zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
	static int vertex_reduction( 
		GTLineString *pLinestringObj, 
		double dThresh );


	/*****************************************************************************
	* @brief	�ߵļ�(opheim)
	* @param	[in]
	*			pLinestringObj	�߶���
	*			min_tol			��С�ݲ�
	*			max_tol			����ݲ�
	*			outline			�򻯺���
	*			[out]
	*			outline			�򻯺���
	* @return	�򻯺����е������
	* @note		
	* @author	����ƣ�zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
	static int opheim( 
		GTLineString *pLinestringObj, 
		double min_tol, 
		double max_tol, 
		GTLineString *outLine);

	/*****************************************************************************
	* @brief	�ߵļ�(NthPoint )
	* @param	[in]
	*			pLinestringObj	�߶���
	*			n			    ÿ�α����ĵ�n����
	*			[out]
	*			pLinestringObj	�򻯺���
	* @return	�򻯺����е������
	* @note		
	* @author	����ƣ�zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
	static int nThPoint( 
		GTLineString *pLinestringObj, 
		unsigned int n );

	/*****************************************************************************
	* @brief	�ߵļ�(perpendicularDistance )
	* @param	[in]
	*			pLinestringObj	�߶���
	*			min_tol		    �ݲ�
	*			outLine			�򻯺���
	*			[out]
	*			outLine			�򻯺���
	* @return	�򻯺����е������
	* @note		�������50%�ĵ�
	* @author	����ƣ�zyk@lreis.ac.cn,2011-07-19
	*******************************************************************************/
	static int perpendicularDistance ( 
		GTLineString *pLinestringObj, 
		double min_tol, 
		GTLineString *outLine);

	/*�Ƴ��ߵ��ظ���, ֻ�ж��ڽ���*/
	static bool removeDuplicate(GTLineString *pLinestringObj);

	/*�Ƴ��ߵ��ظ���, ֻ�ж��ڽ���*/
	static bool removeDuplicate(GTRing *pRingObj);

	/*****************************************************************************
	* @brief	�򵥶���εļ򻯣�Ϊ�����μ򻯷���
	* @param	[in]
	*			pPlygn  	�����
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	���Ƿ�ɹ�,ֻ�Ǳ�ʾ�����Ƿ�������������true��false������ʵ������
	*           
 	* @author	����ƣ�zyk@lreis.ac.cn,2012-05-23
	*******************************************************************************/
	static bool gtPolygonSimpicify(GTPolygon *pPlygn, double dThresh);

	/*****************************************************************************
	* @brief	�򵥶���εļ򻯣�Ϊ�����μ򻯷���
	* @param	[in]
	*			pPlygn  	�����
	*			dThresh		�ݲ�
	*			look_ahead  ǰ������
 	*			[out]
 	* @return	���Ƿ�ɹ�,ֻ�Ǳ�ʾ�����Ƿ�������������true��false������ʵ������
	*           
 	* @author	��������fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_Lang(
		GTPolygon *pPlygn, 
		double dThresh,
		int look_ahead);

	/*****************************************************************************
	* @brief	�򵥶���εļ򻯣�Ϊ�����μ򻯷���
	* @param	[in]
	*			pPlygn  	�����
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	���Ƿ�ɹ�,ֻ�Ǳ�ʾ�����Ƿ�������������true��false������ʵ������
	*           
 	* @author	��������fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_ReumannWitkam(
		GTPolygon *pPlygn, 
		double dThresh);

	/*****************************************************************************
	* @brief	�򵥶���εļ򻯣�Ϊ�����μ򻯷���
	* @param	[in]
	*			pPlygn  	�����
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	���Ƿ�ɹ�,ֻ�Ǳ�ʾ�����Ƿ�������������true��false������ʵ������
	*           
 	* @author	��������fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_VertexReduction(
		GTPolygon *pPlygn, 
		double dThresh);

	/*****************************************************************************
	* @brief	�򵥶���εļ򻯣�Ϊ�����μ򻯷���
	* @param	[in]
	*			pPlygn  	�����
	*			dMinThresh	��С�ݲ�
	*			dMaxThresh	����ݲ�
 	*			[out]
 	* @return	���Ƿ�ɹ�,ֻ�Ǳ�ʾ�����Ƿ�������������true��false������ʵ������
	*           
 	* @author	��������fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_Opheim(
		GTPolygon *pPlygn, 
		double dMinThresh, 
		double dMaxThresh);

	/*****************************************************************************
	* @brief	�򵥶���εļ򻯣�Ϊ�����μ򻯷���
	* @param	[in]
	*			pPlygn  	�����
	*			iNumber		��������ĵ�n����
 	*			[out]
 	* @return	���Ƿ�ɹ�,ֻ�Ǳ�ʾ�����Ƿ�������������true��false������ʵ������
	*           
 	* @author	��������fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_NthPoint(GTPolygon *pPlygn, int iNumber);

	
	/*****************************************************************************
	* @brief	�򵥶���εļ򻯣�Ϊ�����μ򻯷���
	* @param	[in]
	*			pPlygn  	�����
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	���Ƿ�ɹ�,ֻ�Ǳ�ʾ�����Ƿ�������������true��false������ʵ������
	*           
 	* @author	��������fanjf@lreis.ac.cn,2013-05-13
	*******************************************************************************/
	static bool gtPolygonSimpicify_PerpendDistance(
		GTPolygon *pPlygn, 
		double dThresh);

	/*****************************************************************************
	* @brief	Ҫ�ؼ��ϵļ�,ʹ��dp�㷨
	* @param	[in]
	*			pFeatCls  	Ҫ�ؼ���
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	null
	*           
 	* @author	����ƣ�zyk@lreis.ac.cn,2012-05-23
	*******************************************************************************/
	static void simplifyFeatures_dp(GTFeatureClass *pFeatCls,
								    double			 dThresh);

	/*****************************************************************************
	* @brief	Ҫ�ؼ��ϵļ�,ʹ��lang�㷨
	* @param	[in]
	*			pFeatCls  	Ҫ�ؼ���
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	null
	*           
 	* @author	����ƣ�zyk@lreis.ac.cn,2012-05-23
	*******************************************************************************/
	static void simplifyFeatures_lang(GTFeatureClass *pFeatCls,
								      double			 dThresh,
									  int look_ahead);

	/*****************************************************************************
	* @brief	Ҫ�ؼ��ϵļ�,ʹ��Reumann_witkam�㷨
	* @param	[in]
	*			pFeatCls  	Ҫ�ؼ���
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	null
	*           
 	* @author	����ƣ�zyk@lreis.ac.cn,2012-07-20
	*******************************************************************************/
	static void simplifyFeatures_rw(GTFeatureClass *pFeatCls,
								    double			dThresh);

	/*****************************************************************************
	* @brief	Ҫ�ؼ��ϵļ�,ʹ��Vertex_reduce�㷨
	* @param	[in]
	*			pFeatCls  	Ҫ�ؼ���
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	null
	*           
 	* @author	����ƣ�zyk@lreis.ac.cn,2012-07-20
	*******************************************************************************/
	static void simplifyFeatures_vr(GTFeatureClass *pFeatCls,
								    double			dThresh);

	/*****************************************************************************
	* @brief	Ҫ�ؼ��ϵļ�,ʹ��op�㷨
	* @param	[in]
	*			pFeatCls  	Ҫ�ؼ���
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	null
	*           
 	* @author	����ƣ�zyk@lreis.ac.cn,2012-07-20
	*******************************************************************************/
	static void simplifyFeatures_op(GTFeatureClass *pFeatCls,
									double min_tol, 
									double max_tol);

	/*****************************************************************************
	* @brief	Ҫ�ؼ��ϵļ�,ʹ��Nth�㷨
	* @param	[in]
	*			pFeatCls  	Ҫ�ؼ���
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	null
	*           
 	* @author	����ƣ�zyk@lreis.ac.cn,2012-07-20
	*******************************************************************************/
	static void simplifyFeatures_nth(GTFeatureClass *pFeatCls,
									 unsigned int nPt);

	/*****************************************************************************
	* @brief	Ҫ�ؼ��ϵļ�,ʹ��per�㷨
	* @param	[in]
	*			pFeatCls  	Ҫ�ؼ���
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	null
	*           
 	* @author	����ƣ�zyk@lreis.ac.cn,2012-07-20
	*******************************************************************************/
	static void simplifyFeatures_per(GTFeatureClass *pFeatCls,
									 double min_tol);

	/*****************************************************************************
	* @brief	Ҫ�ؼ��ϵļ򻯵Ķ���㷨��openmpʵ��
	* @param	[in]
	*			pFeatCls  	Ҫ�ؼ���
	*			dThresh		�ݲ�
 	*			[out]
 	* @return	null
	*           
 	* @author	����ƣ�zyk@lreis.ac.cn,2012-05-23
	*******************************************************************************/
	static void simplifyFeatures_omp(GTFeatureClass *pFeatCls,
									 double			 dThresh);

};

}
#endif
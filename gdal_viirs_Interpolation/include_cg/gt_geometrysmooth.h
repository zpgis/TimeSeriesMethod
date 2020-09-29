/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  �ߵ�ƽ��
* Author:   ������, fanjf@lreis.ac.cn,2011-07-19
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c)  ��������Ȩ��
****************************************************************/
#ifndef GTSGEOMETRYSMOOTH_H_INCLUDED
#define GTSGEOMETRYSMOOTH_H_INCLUDED

#include "gts_cg_port.h"
#include "gt_beziercurve.h"

typedef enum e_SmoothMethod
{
	Bezier		= 0,
	Boyle		= 1,
	Chaiken		= 2,
	Dw			= 3,
	SlideAve	= 4,
}SmoothMethod;

/* chaiken��hermite�����õ����������� */
typedef struct Pointlist_node
{
	GTRawPoint p;
	Pointlist_node *next;
} POINT_LIST_NODE;
POINT_LIST_NODE *point_list_new(GTRawPoint p);
void			 point_list_add(POINT_LIST_NODE * l, GTRawPoint p);
void			 point_list_free(POINT_LIST_NODE l);
double			 point_angle_between(GTRawPoint a, GTRawPoint b, GTRawPoint c);

namespace gts{

	class GTS_DLL GTGeometrySmooth
	{
	public:
		GTGeometrySmooth(void);
		~GTGeometrySmooth(void);

		/*****************************************************************************
		* @brief	�ߵ�ƽ��(boyle's forward looking algorithm)
		* @param	[in]
		*			pLinestringObj	�߶���
		*			look_ahead		ǰ������
		*			[out]
		*			pLinestringObj	ƽ������
		* @return	ƽ�������е������
		* @note		
		* @author	����ƣ�zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static int boyle(
			GTLineString *pLinestringObj, 
			int look_ahead);

		//clr��װ��
		static GTLineString* boyle_clr(
			GTLineString *pLinestringObj, 
			int look_ahead);

		/*****************************************************************************
		* @brief	�ߵ�ƽ��(mcmaster's sliding averaging algorithm)
		* @param	[in]
		*			pLinestringObj	�߶���
		*			slide	
		*			lookahead ����������
		*			[out]
		*			pLinestringObj	ƽ������
		* @return	ƽ�������е������
		* @note		Look ahead����������������ֵ��ԭ�ߵ������
		* @author	����ƣ�zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static int sliding_averaging(
			GTLineString *pLinestringObj,
			double slide, 
			int look_ahead);

		/*****************************************************************************
		* @brief	�ߵ�ƽ��(mcmaster's distance_weighting algorithm)
		* @param	[in]
		*			pLinestringObj	�߶���
		*			slide	
		*			lookahead
		*			[out]
		*			pLinestringObj	ƽ������
		* @return	ƽ�������е������
		* @note		lookahead ����Ϊ����, ����ֵ��ԭ�ߵ������
		* @author	����ƣ�zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static int  distance_weighting(
			GTLineString *pLinestringObj, 
			double slide, 
			int look_ahead);

		/*****************************************************************************
		* @brief	�ߵ�ƽ��(chaiken algorithm)
		* @param	[in]
		*			pLinestringObj	�߶���
		*			dThresh	
		*			[out]
		*			pLinestringObj	ƽ������
		* @return	ƽ�������е������
		* @note		
		* @author	����ƣ�zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static int  chaiken(
			GTLineString *pLinestringObj, 
			double dThresh);

		/*****************************************************************************
		* @brief	ͨ��ƽ�������߹���bezier����(bezier algorithm)
		* @param	[in]
		*			pLinestringObj  �߶���(�������������)
		*           pt_step			��ֵ��������������ʽά��Ϊpt_step-1,pt_stepӦ����2С��33��
		*			point_on_curve	ƽ�����ϵ���
		*			[out]
		* @return	GTLineString	ƽ������
		* @note		����ֵ��Ҫ�ⲿ�ͷ�,pt_step���ܳ���33,Ĭ��Ϊ3�α���������;�����point_countҪ���ڱ�����ά��
		* @author	����ƣ�zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		static GTLineString*  bezier(
			GTLineString *pLinestringObj, 
			const int point_on_curve, 
			const int pt_step = 4);


		//-----------featureclass�����ƽ���㷨----------//

		/*****************************************************************************
		* @brief	featureclass�������ƽ��(boyle's forward looking algorithm)
		* @param	[in]
		*			pFeatCls	    ��Ҫ�ؼ��϶���
		*			look_ahead		ǰ������
		*			[out]
		*			pLinestringObj	ƽ������
		* @return	null
		* @note		
		* @author	����ƣ�zyk@lreis.ac.cn,2012-08-02
		*******************************************************************************/
		static void smoothFeatures_boyle(
			GTFeatureClass *pFeatCls, 
			int look_ahead);

		/*****************************************************************************
		* @brief	featureclass������ߵ�ƽ��(mcmaster's sliding averaging algorithm)
		* @param	[in]
		*			pFeatCls	 �߶����featureclass
		*			slide	
		*			lookahead ����������
		*			[out]
		*			pFeatCls	 ƽ������featureclass
		* @return	null
		* @note		Look ahead����������������ֵ��ԭ�ߵ������
		* @author	����ƣ�zyk@lreis.ac.cn,2012-08-02
		*******************************************************************************/
		static void smoothFeatures_slideAve(
			GTFeatureClass *pFeatCls, 
			double slide, 
			int look_ahead);

		/*****************************************************************************
		* @brief	featureclass������ߵ�ƽ��(mcmaster's distance_weighting algorithm)
		* @param	[in]
		*			pFeatCls	 �߶����featureclass
		*			slide	
		*			lookahead
		*			[out]
		*			pFeatCls	 ƽ������featureclass
		* @return	null
		* @note		lookahead ����Ϊ����, ����ֵ��ԭ�ߵ������
		* @author	����ƣ�zyk@lreis.ac.cn,2012-08-02
		*******************************************************************************/
		static void  smoothFeatures_dw(
			GTFeatureClass *pFeatCls, 
			double slide, 
			int look_ahead);

		/*****************************************************************************
		* @brief	featureclass������ߵ�ƽ��(chaiken algorithm)
		* @param	[in]
		*			pFeatCls	 �߶����featureclass
		*			dThresh	
		*			[out]
		*			pFeatCls     ƽ������
		* @return	ƽ�������е������
		* @note		
		* @author	����ƣ�zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static void  smoothFeatures_chaiken(
			GTFeatureClass *pFeatCls, 
			double dThresh);

		/*****************************************************************************
		* @brief	featureclass�����,ͨ��ƽ�������߹���bezier����(bezier algorithm)
		* @param	[in]
		*			pFeatCls        �߶���(�������������)
		*            pt_step			��ֵ��������������ʽά��Ϊpt_step-1,pt_stepӦ����2С��33��
		*			point_on_curve	ƽ�����ϵ���
		*			[out]
		*        	pFeatCls    ƽ������
		* @return	pFeatCls    ƽ������
		* @note		pt_step���ܳ���33,Ĭ��Ϊ3�α���������;�����point_countҪ���ڱ�����ά��
		* @author	����ƣ�zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static void  smoothFeatures_bezier(
			GTFeatureClass *pFeatCls, 
			const int point_on_curve, 
			const int pt_step = 4);

		//private: liuyan modify at 2012-08-20
		/*****************************************************************************
		* @brief	�ߵ�ƽ��(hermite cubic spline algorithm)
		* @param	[in]
		*			pLinestringObj	�߶���
		*			step	����
		*           angle_thresh �Ƕ���ֵ
		*			[out]
		*			pLinestringObj	ƽ������
		* @return	ƽ�������е������
		* @note		�ݲ�����,�������÷�������ȷ,������Ч�����ԣ�
		*           �������Ҫ���������ߵ�ʵ���������,������С������ĵ�,ƽ��Ч������������й�
		* @author	����ƣ�zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static int hermite( 
			GTLineString *pLinestringObj, 
			double step, 
			double angle_thresh, 
			double eps );
		//liuyan add
		/*****************************************************************************
		* @brief	�ߵ�ƽ��(hermite cubic spline algorithm)
		* @param	[in]
		*			pFeatCls	 �߶����featureclass
		*			step	����
		*           angle_thresh �Ƕ���ֵ
		*			[out]
		*			pFeatCls	 ƽ������featureclass
		* @return	null
		* @note		�ݲ�����,�������÷�������ȷ,������Ч�����ԣ�
		*           �������Ҫ���������ߵ�ʵ���������,������С������ĵ�,ƽ��Ч������������й�
		* @author	����ƣ�zyk@lreis.ac.cn,2010-07-19
		*******************************************************************************/
		static void  smoothFeatures_hermite(
			GTFeatureClass *pFeatCls, 
			double step, 
			double angle_thresh, 
			double eps );

		/*****************************************************************************
		* @brief	�ߵ�ƽ��(bezier algorithm)
		* @param	[in]
		*			pPlygn			��ƽ�������
		*			point_on_curve	��ֵ�����
		*           pt_step			����
		*			[out]
		* @return	bool
		* @note		
		* @author	��������fanjf@lreis.ac.cn,2012-05-13
		*******************************************************************************/
		static bool gtPolygonSmooth_Bezier(GTPolygon *pPlygn, 
			int point_on_curve,int pt_step);

		/*****************************************************************************
		* @brief	�ߵ�ƽ��(Boyle algorithm)
		* @param	[in]
		*			pPlygn			��ƽ�������
		*           iPrevStep		ǰ������
		*			[out]
		* @return	bool
		* @note		
		* @author	��������fanjf@lreis.ac.cn,2012-05-13
		*******************************************************************************/
		static bool gtPolygonSmooth_Boyle(
			GTPolygon *pPlygn, 
			int iPrevStep);

		/*****************************************************************************
		* @brief	�ߵ�ƽ��(Chaiken algorithm)
		* @param	[in]
		*			pPlygn			��ƽ�������
		*           dTolerance		�ݲ�
		*			[out]
		* @return	bool
		* @note		
		* @author	��������fanjf@lreis.ac.cn,2012-05-13
		*******************************************************************************/
		static bool gtPolygonSmooth_Chaiken(
			GTPolygon *pPlygn, 
			double dTolerance);
		/*****************************************************************************
		* @brief	�ߵ�ƽ��(����Ȩ�� algorithm)
		* @param	[in]
		*			pPlygn			��ƽ�������
		*           slide			slideValue
		*			look_ahead		ǰ������
		*			[out]
		* @return	bool
		* @note		
		* @author	��������fanjf@lreis.ac.cn,2012-05-13
		*******************************************************************************/
		static bool gtPolygonSmooth_dw(
			GTPolygon *pPlygn, 
			double slide,
			int look_ahead);
		/*****************************************************************************
		* @brief	�ߵ�ƽ��(����ƽ�� algorithm)
		* @param	[in]
		*			pPlygn			��ƽ�������
		*           slide			slideValue
		*			look_ahead		ǰ������
		*			[out]
		* @return	bool
		* @note		
		* @author	��������fanjf@lreis.ac.cn,2012-05-13
		*******************************************************************************/
		static bool gtPolygonSmooth_SlidingMean(
			GTPolygon *pPlygn, 
			double slide,
			int look_ahead);
	};


}
#endif


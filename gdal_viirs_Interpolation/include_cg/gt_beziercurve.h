/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  bezier���߼�ƽ��
* 
* Author:   ������, fanjf@lreis.ac.cn,2011-07-26
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c) ��������Ȩ��
****************************************************************/

#ifndef GT_BEZIERCURVE_H_INCLUDED
#define GT_BEZIERCURVE_H_INCLUDED

#include "gt_geometry.h"

namespace gts
{
	class GTS_DLL GTBezierCurve
	{
	public:
		GTBezierCurve ();
		~GTBezierCurve();


		/*****************************************************************************
		* @brief	ͨ��ƽ�������߹���bezier����(bezier algorithm)
		* @param	[in]
		*			pLinestringObj  �߶���(�������������)
		*           pt_step			��ֵ��������������ʽά��Ϊpt_step-1,pt_stepӦ����2С��33��
		*			point_on_curve	ƽ�����ϵ���
		*			[out]
		* @return	GTLineString	ƽ������
		* @note		����ֵ��Ҫ�ⲿ�ͷ�,pt_step���ܳ���33,Ĭ��Ϊ3�α���������;�����point_count������ڱ�����ά��
		* @author	����ƣ�zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		GTLineString*  makeBezierCurve(
			GTLineString *pLinestringObj, 
			const int     point_on_curve, 
			const int     pt_step = 4);
 
		/*****************************************************************************
		* @brief	ͨ����������鹹��bezier����(bezier algorithm)
		* @param	[in]
		*			point_list      ���������
		*           pt_step			��ֵ��������������ʽά��Ϊpt_step-1,pt_stepӦ����2С��33��
		*			point_on_curve	ƽ�����ϵ���
		*			[out]
		* @return	GTLineString	ƽ������
		* @note		����ֵ��Ҫ�ⲿ�ͷ�;pt_step���ܳ���33,Ĭ��Ϊ3�α���������;�����point_count������ڱ�����ά��
		* @author	����ƣ�zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		GTLineString*  makeBezierCurve(
			GTRawPoint *point_list, 
			const int  point_count, 
			const int  point_on_curve, 
			const int  pt_step);
		
	private:

		/*****************************************************************************
		* @brief	���ص�n������
		* @param	[in]
		*			n	�߶���
		*			[out]
		*			
		* @return	���ص�n������
		* @note		n�������0,С��33(��Ϊ�������鳤������Ϊ33)
		* @author	����ƣ�zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		double    factorial(int n);

		/*****************************************************************************
		* @brief	����Ni
		* @param	[in]
		*			factorialLookup	����������ʽϵ���б�
		* @note		������createFactorialTable��ʼ����ϵ���б�,ע���ͷ�
		* @author	����ƣ�zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		double*   factorialLookup;
		/* ��ʼ��ϵ���б� */
		void	  createFactorialTable();

		/*****************************************************************************
		* @brief	����Ni
		* @param	[in]
		*			n	
		*           i
		*			[out]
		*			
		* @return	��������
		* @note		n��������i
		* @author	����ƣ�zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		double    Ni(int n, int i);

		/*****************************************************************************
		* @brief	����˹̹�׺���
		* @param	[in]
		*			n	factor��������
		*           i	
		*			[out]
		*
		* @return	���غ���ֵ
		* @note		n��������i
		* @author	����ƣ�zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		double    bernstein(int n, int i, double t);
	};
}
#endif
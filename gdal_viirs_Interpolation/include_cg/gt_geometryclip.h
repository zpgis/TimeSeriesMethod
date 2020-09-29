/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ��һ���������ͼ���м�����Ȥ����
* Author:   ������, fanjf@lreis.ac.cn,2011-8-10
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c)  ��������Ȩ��
****************************************************************/
#ifndef GTSGEOMETRYCLIP_H_INCLUDED
#define GTSGEOMETRYCLIP_H_INCLUDED

#include "gts_cg_port.h"

namespace gts{

	struct GTGBLPoint{
		GTRawPoint   pt;
		unsigned int mark;
	};

	/*****************************************************************************
	*	compareWrapper ���ڴ���std::sort�������ʱ��ȫ�ֱ���������
	*	author	������, fanjf@lreis.ac.cn,2012-6-5
	*******************************************************************************/
	class compareWrapper
	{
	public:
		compareWrapper(const GTRawPoint& p) : _p(p)
		{}
		bool operator()(const GTRawPoint& p1, 
			const GTRawPoint& p2)
		{
			double d1 = (p1.X - _p.X) * (p1.X - _p.X) +
				(p1.Y - _p.Y) * (p1.Y - _p.Y);
			double d2 = (p2.X - _p.X) * (p2.X - _p.X) +
				(p2.Y - _p.Y) * (p2.Y - _p.Y);  
			return d1 < d2;
		}
	private:
		GTRawPoint _p;
	};

	class GTS_DLL GTGeometryClip
	{
	public:
		GTGeometryClip(void);
		~GTGeometryClip(void);

		/*�㱻������*/
		/*�߱�������*/
		/*����������*/

		/*****************************************************************************
		* @brief	��㱻���и�
		* @param	[in]
		*			pMPoint				������
		*			pRing				������
		*			bCheck				�Ƿ����˻����		
		*			[out]
		*			point_list			�и��Ķ����󼯺�
		*			point_mark			�и��Ķ����󼯺϶�Ӧ�����ⲿ��־
		* @return	�и��Ķ���������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-8-10
		*******************************************************************************/
		static int multiPointClipByRing(
			GTMultiPoint					*pMPoint,
			GTRing							*pRing,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false); 

		/*****************************************************************************
		* @brief	��㱻������и�
		* @param	[in]
		*			pMPoint				������
		*			pPolygon			����ζ���
		*			bCheck				�Ƿ����˻����		
		*			[out]
		*			point_list			�и��Ķ����󼯺�
		*			point_mark			�и��Ķ����󼯺϶�Ӧ�����ⲿ��־
		* @return	�и��Ķ���������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-8-10
		*******************************************************************************/
		static int multiPointClipByPolygon(
			GTMultiPoint					*pMPoint,
			GTPolygon						*pPolygon,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false);

		/*****************************************************************************
		* @brief	��㱻�������и�
		* @param	[in]
		*			pMPoint				������
		*			pMPolygon			�����ζ���
		*			bCheck				�Ƿ����˻����		
		*			[out]
		*			point_list			�и��Ķ����󼯺�
		*			point_mark			�и��Ķ����󼯺϶�Ӧ�����ⲿ��־
		* @return	�и��Ķ���������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-8-10
		*******************************************************************************/
		static int multiPointClipByMultiPolygon(
			GTMultiPoint					*pMPoint,
			GTMultiPolygon					*pMPolygon,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false);

		/*****************************************************************************
		* @brief	�߱������иͨ����־λ�������Ƿ�ʹ������
		* @param	[in]
		*			pLineString			�߶���
		*			pMLineString		���߶���
		*			bCheck				�Ƿ����˻����		
		*			bUseRTree			�Ƿ�ʹ������
		*			[out]
		*			line_list			�и����߶��󼯺�
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-8-10
		*******************************************************************************/
		static int lineStringClipByMultiLineString(
			GTLineString					*pLineString,
			GTMultiLineString				*pMLineString,
			std::vector<GTLineString *>&	line_list,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	�߱�������иͨ����־λ�������Ƿ�ʹ������
		* @param	[in]
		*			pLineString			�߶���
		*			pPolygon			����ζ���
		*			bCheck				�Ƿ����˻����		
		*			bUseRTree			�Ƿ�ʹ������
		*			[out]
		*			line_list			�и����߶��󼯺�
		*			line_mark			�и����߶��󼯺϶�Ӧ�����ⲿ��־
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-7-25
		*******************************************************************************/
		static int lineStringClipByPolygon(
			GTLineString					*pLineString,
			GTPolygon						*pPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	�߱��������иͨ����־λ�������Ƿ�ʹ������
		* @param	[in]
		*			pLineString			�߶���
		*			pMPolygon			�����ζ���
		*			bCheck				�Ƿ����˻����		
		*			bUseRTree			�Ƿ�ʹ������
		*			[out]
		*			line_list			�и����߶��󼯺�
		*			line_mark			�и����߶��󼯺϶�Ӧ�����ⲿ��־
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int lineStringClipByMultiPolygon(
			GTLineString					*pLineString,
			GTMultiPolygon					*pMPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	���߱����иͨ����־λ�������Ƿ�ʹ������
		* @param	[in]
		*			pMLineString		���߶���
		*			pLineString			�߶���
		*			bCheck				�Ƿ����˻����		
		*			bUseRTree			�Ƿ�ʹ������
		*			[out]
		*			line_list			�и����߶��󼯺�
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-8-11
		*******************************************************************************/
		static int multiLineStringClipBylineString(
			GTMultiLineString				*pMLineString,
			GTLineString					*pLineString,
			std::vector<GTLineString *>&	line_list,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	���߱������иͨ����־λ�������Ƿ�ʹ������
		* @param	[in]
		*			pMLineString		���߶���
		*			pMLineStringClip	�����и�Ķ��߶���
		*			bCheck				�Ƿ����˻����		
		*			bUseRTree			�Ƿ�ʹ������
		*			[out]
		*			line_list			�и����߶��󼯺�
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-8-11
		*******************************************************************************/
		static int multiLineStringClipByMultiLineString(
			GTMultiLineString				*pMLineString,
			GTMultiLineString				*pMLineStringClip,
			std::vector<GTLineString *>&	line_list,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	���߱�������иͨ����־λ�������Ƿ�ʹ������
		* @param	[in]
		*			pMLineString		���߶���
		*			pPolygon			����ζ���
		*			bCheck				�Ƿ����˻����		
		*			bUseRTree			�Ƿ�ʹ������
		*			[out]
		*			line_list			�и����߶��󼯺�
		*			line_mark			�и����߶��󼯺϶�Ӧ�����ⲿ��־
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int multiLineStringClipByPolygon(
			GTMultiLineString				*pMLineString,
			GTPolygon						*pPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false);

		/*****************************************************************************
		* @brief	���߱��������иͨ����־λ�������Ƿ�ʹ������
		* @param	[in]
		*			pLineString			�߶���
		*			pMPolygon			�����ζ���
		*			bCheck				�Ƿ����˻����		
		*			bUseRTree			�Ƿ�ʹ������
		*			[out]
		*			line_list			�и����߶��󼯺�
		*			line_mark			�и����߶��󼯺϶�Ӧ�����ⲿ��־
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int multiLineStringClipByMultiPolygon(
			GTMultiLineString				*pMLineString,
			GTMultiPolygon					*pMPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		/*****************************************************************************
		* @brief	����α����и�
		* @param	[in]
		*			pPolygon			����ζ���
		*			pLineString			�߶���
		*			bCheck				�Ƿ����˻����		
		*			[out]
		*			ring_list			�и��Ļ����󼯺�
		*			ring_mark			�и��Ļ����󵺵ı�־
		* @return	�и��Ļ���������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int polygonClipByLineString(
			GTPolygon						*pPolygon,
			GTLineString					*pLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		/*****************************************************************************
		* @brief	����α������и�
		* @param	[in]
		*			pPolygon			����ζ���
		*			pMLineString		���߶���
		*			bCheck				�Ƿ����˻����		
		*			[out]
		*			ring_list			�и��Ļ����󼯺�
		*			ring_mark			�и��Ļ����󵺵ı�־
		* @return	�и��Ļ���������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int polygonClipByMultiLineString(
			GTPolygon						*pPolygon,
			GTMultiLineString				*pMLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		/*****************************************************************************
		* @brief	�����α����и�
		* @param	[in]
		*			pMPolygon			�����ζ���
		*			pLineString			�߶���
		*			bCheck				�Ƿ����˻����		
		*			[out]
		*			ring_list			�и��Ļ����󼯺�
		*			ring_mark			�и��Ļ����󵺵ı�־
		* @return	�и��Ļ���������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int multiPolygonClipBylineString(
			GTMultiPolygon					*pMPolygon,
			GTLineString					*pLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		/*****************************************************************************
		* @brief	�����α������и�
		* @param	[in]
		*			pMPolygon			�����ζ���
		*			pLineString			�߶���
		*			bCheck				�Ƿ����˻����		
		*			[out]
		*			ring_list			�и��Ļ����󼯺�
		*			ring_mark			�и��Ļ����󵺵ı�־
		* @return	�и��Ļ���������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-7-26
		*******************************************************************************/
		static int multiPolygonClipByMultiLineString(
			GTMultiPolygon					*pMPolygon,
			GTMultiLineString				*pMLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		/*****************************************************************************
		* @brief	����α����и�,��д�㷨�����ö��˫������Ѱ���и��ͼ��
		* @param	[in]
		*			pPolygon			����ζ���
		*			pLineString			�߶���
		*			bCheck				�Ƿ����˻����		
		*			[out]
		*			ring_list			�и��Ļ����󼯺�
		*			ring_mark			�и��Ļ����󵺵ı�־
		* @return	�и��Ļ���������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷţ������ཻ�������ʱ�޴������ܻ���bug
		* @author	������, fanjf@lreis.ac.cn,2011-7-30 �����8-4
		*******************************************************************************/
		static int polygonClipByLineString1(
			GTPolygon              *pPolygon,
			GTLineString           *pLineString,
			std::vector<GTRing *>&  ring_list, 
			std::vector<int>&		ring_mark,
			bool                    bCheck = false);

		/*2011-10-25 ���´�GTGeometryBoolean���ƶ����ˡ� ����*/
		/*�߱���*/
		static int lineStringClipByRing(
			GTLineString                *pLineString,
			GTRing                      *pRing,
			std::vector<GTLineString *>& line_list,       /*���ָ������, ע���ͷ�*/
			std::vector<unsigned int>  & line_mark,       /*0Ϊ�ⲿ, 1Ϊ�ڲ�*/
			bool                         bCheck = false); /*�Ƿ����˻����*/ 

		/*****************************************************************************
		* @brief	�߱����и���Ի�������������������Ч�ʽϲ��������ĺ�����
		* @param	[in]
		*			pLineString			�߶���
		*			pRing				������
		*			bCheck				�Ƿ����˻����		
		*			[out]
		*			line_list			�и����߶��󼯺�
		*			line_mark			�и����߶��󼯺϶�Ӧ�����ⲿ��־
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-7-25
		*******************************************************************************/
		static int lineStringClipByRingWithOutRTree(
			const GTLineString                *pLineString,
			GTRing                      *pRing,
			std::vector<GTLineString *>& line_list,
			std::vector<unsigned int>  & line_mark,
			bool                         bCheck = false);

		/*****************************************************************************
		* @brief	�߱����иͨ����־λ�������Ƿ�ʹ������
		* @param	[in]
		*			pLineString			�߶���
		*			pRing				������
		*			bCheck				�Ƿ����˻����		
		*			bUseRTree			�Ƿ�ʹ������
		*			[out]
		*			line_list			�и����߶��󼯺�
		*			line_mark			�и����߶��󼯺϶�Ӧ�����ⲿ��־
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-7-25
		*******************************************************************************/
		static int lineStringClipByRing1(
			GTLineString                *pLineString,
			GTRing                      *pRing,
			std::vector<GTLineString *>& line_list,
			std::vector<unsigned int>  & line_mark,
			bool                         bCheck = false,
			bool						 bUseRTree = false); 

		/*�߱���*/
		static int lineStringClipByLineString(
			GTLineString                *pLineString,
			GTLineString                *pLineStringClip,
			std::vector<GTLineString *>& line_list,       /*���ָ������, ע���ͷ�*/
			bool                         bCheck = false); /*�Ƿ����˻����*/ 

		/*****************************************************************************
		* @brief	�߱����и�����߶�����������������Ч�ʽϲ��������ĺ�����
		* @param	[in]
		*			pLineString			�߶���
		*			pLineStringClip		�����и���߶���
		*			bCheck				�Ƿ����˻����		
		*			[out]
		*			line_list			�и����߶��󼯺�
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-8-11
		*******************************************************************************/
		static int lineStringClipByLineStringWithOutRTree(
			GTLineString                *pLineString,
			GTLineString                *pLineStringClip,
			std::vector<GTLineString *>& line_list,       /*���ָ������, ע���ͷ�*/
			bool                         bCheck = false);

		/*****************************************************************************
		* @brief	�߱����иͨ����־λ�������Ƿ�ʹ������
		* @param	[in]
		*			pLineString			�߶���
		*			pRing				������
		*			bCheck				�Ƿ����˻����		
		*			bUseRTree			�Ƿ�ʹ������
		*			[out]
		*			line_list			�и����߶��󼯺�
		* @return	�и����߶�������
		* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
		* @author	������, fanjf@lreis.ac.cn,2011-8-11
		*******************************************************************************/
		static int lineStringClipByLineString1(
			GTLineString                *pLineString,
			GTLineString                *pLineStringClip,
			std::vector<GTLineString *>& line_list,
			bool                         bCheck = false,
			bool						 bUseRTree = false);

		/*�������и�, �ߵ������˵�����ڻ����ⲿ*/
		static int ringClipByLineString(
			GTRing                 *pRing,
			GTLineString           *pLineString,
			std::vector<GTRing *>&  ring_list,       /*���ָ������, ע���ͷ�*/
			bool                    bCheck = false); /*�Ƿ����˻����*/ 
		/*2011-10-25 ���ϴ�GTGeometryBoolean���ƶ����ˡ� ����*/

		/*��չ�����ߺͶ����*/
		/*ͼ�����*/

		static int geometryClipByGeometry(
			GTGeometry					*pGeo,
			GTGeometry					*pGeoClip,
			std::vector<GTGeometry *>&	geolist_list,
			bool						bCheck = false,
			bool						bUseRTree = false);

		static int featureClipByfeature(
			GTFeature					*pFeat,
			GTFeature					*pFeatClip,
			GTFeatureClass				&outClss,
			bool						bCheck = false,
			bool						bUseRTree = false);

		static int featureClassClipByfeatureClass(
			GTFeatureClass				&srcClss,
			GTFeatureClass				&clipCLss,
			GTFeatureClass				&outClss,
			bool						bCheck = false,
			bool						bUseRTree = false);

		/*****************************************************************************
		* @brief	ͼ�㱻������μ���
		* @param	[in]
		*			srcLayer					���и�ͼ��
		*			env							�����и���������
		*			bUseRTree					�Ƿ����˻����		
		*			[out]
		*			desLayer					�и���γɵ�ͼ��
		* @return	�и���feature��������
		* @note		
		* @author	������, fanjf@lreis.ac.cn,2011-8-18
		*******************************************************************************/
		static int featureLayerClipByEnvelop(
			const GTFeatureLayer*	srcLayer,
			GTFeatureLayer*			desLayer,
			GTEnvelop&				env);

		static int featureClassClipByEnvelop(
			GTFeatureClass&			inClss,
			GTFeatureClass&			outClss,
			GTEnvelop&				env);

		/*����ͼ���и�ı�����polygon*/

		/****************************************************
		*���º���_mp��ʾ��������㷨*
		****************************************************/

		static int multiPointClipByRing_mp(
			GTMultiPoint					*pMPoint,
			GTRing							*pRing,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false); 

		static int multiPointClipByPolygon_mp(
			GTMultiPoint					*pMPoint,
			GTPolygon						*pPolygon,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false);

		static int multiPointClipByMultiPolygon_mp(
			GTMultiPoint					*pMPoint,
			GTMultiPolygon					*pMPolygon,
			std::vector<GTMultiPoint *>&	point_list,
			std::vector<unsigned int>&		point_mark,
			bool							bCheck = false);

		static int multiLineStringClipBylineString_mp(
			GTMultiLineString				*pMLineString,
			GTLineString					*pLineString,
			std::vector<GTLineString *>&	line_list,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		static int multiLineStringClipByMultiLineString_mp(
			GTMultiLineString				*pMLineString,
			GTMultiLineString				*pMLineStringClip,
			std::vector<GTLineString *>&	line_list,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		static int multiLineStringClipByPolygon_mp(
			GTMultiLineString				*pMLineString,
			GTPolygon						*pPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false);

		static int multiLineStringClipByMultiPolygon_mp(
			GTMultiLineString				*pMLineString,
			GTMultiPolygon					*pMPolygon,
			std::vector<GTLineString *>&	line_list,
			std::vector<unsigned int>  &	line_mark,
			bool							bCheck = false,
			bool							bUseRTree = false); 

		static int multiPolygonClipBylineString_mp(
			GTMultiPolygon					*pMPolygon,
			GTLineString					*pLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		static int multiPolygonClipByMultiLineString_mp(
			GTMultiPolygon					*pMPolygon,
			GTMultiLineString				*pMLineString,
			std::vector<GTRing *>&			ring_list,
			std::vector<int>&				ring_mark,
			bool							bCheck = false); 

		static int geometryClipByGeometry_mp(
			GTGeometry					*pGeo,
			GTGeometry					*pGeoClip,
			std::vector<GTGeometry *>&	geolist_list,
			bool						bCheck = false,
			bool						bUseRTree = false);

		static int featureClipByfeature_mp(
			GTFeature					*pFeat,
			GTFeature					*pFeatClip,
			GTFeatureClass				&outClss,
			bool						bCheck = false,
			bool						bUseRTree = false);

		static int featureClassClipByfeatureClass_mp(
			GTFeatureClass				&srcClss,
			GTFeatureClass				&clipCLss,
			GTFeatureClass				&outClss,
			bool						bCheck = false,
			bool						bUseRTree = false);

		/*2011-10-25 ���´�GTGeometryBoolean���ƶ����ˡ� ����*/
	private:
		static bool compare(const GTRawPoint& p1, const GTRawPoint& p2);
		static int removeDuplicate(std::vector<GTRawPoint> &pt_list, int pt_num);
		/*2011-10-25 ���ϴ�GTGeometryBoolean���ƶ����ˡ� ����*/
	};
};

#endif
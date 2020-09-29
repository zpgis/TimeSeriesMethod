/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  Buffer������(�����߶�Buffer�ϲ�)
* Author:   ������, fanjf@lreis.ac.cn 05-11-2011
*
****************************************************************
* Copyright (c)  All rights reserved.
* ��Ȩ����    (c)  ��������Ȩ��
****************************************************************/
#ifndef GTSGEOMETRYBUFFER_H_INCLUDED
#define GTSGEOMETRYBUFFER_H_INCLUDED

#include "gts_cg_port.h"

namespace gts{

class GTS_DLL GTGeometryBuffer
{
public:
	GTGeometryBuffer(void);
	~GTGeometryBuffer(void);

	/*���¾����������Ringָ������������, ��Ҫ�ⲿ�ͷ�, ����Ring�ĸ���
	 *ͬʱҲ���ص��ı�־, ����ring�е��˳��û�е���
	 */
	
	/*���Buffer*/
	static int pointBuffer(
		const GTPoint*         pointObj, 
		double                 dR, 
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	����Buffer, ����ָ������, �ⲿ�ͷţ��ϲ�����Ϊ����segmentbuffer
	*			��ÿ����2bufferΪһ�����κϲ�
	* @param	[in]
	*			ringObj			������
	*			dR				buffer�뾶
	*			bufferType		��buffer���ͣ�GT_BUFFER_OUTER��GT_BUFFER_EDGE��
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	��buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-18
	*******************************************************************************/
	static int ringBuffer(
		const GTRing*          ringObj,
		double                 dR,
		enumGTBufferType	   bufferType,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*�ߵ�Buffer, ����ָ������, �ⲿ�ͷ�*/
	static int lineStringBuffer(
		const GTLineString*    lineStringObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	�ߵ�Buffer, ����ָ������, �ⲿ�ͷ�:�����޸���lineStringBuffer������
	*			���ĺϲ���segmentbuffer����Ϊ��ÿ����2bufferΪһ�飬���κϲ�
	* @param	[in]
	*			lineStringObj	�߶���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	��buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-18
	*******************************************************************************/
	static int lineStringBuffer1(
		const GTLineString*    lineStringObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*����ε�Buffer���ձ߽���, ����ָ������, �ⲿ�ͷ�*/
	static int polygonBuffer(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	����ε�Buffer, ����ָ������, �ⲿ�ͷ�
	* @param	[in]
	*			polygonObj		����ζ���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	�����buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-19
	*******************************************************************************/
	static int polygonBuffer1(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*����ε�Buffer���������, ����ָ������, �ⲿ�ͷ�*/
	static int polygonBufferFace(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	����ε�Buffe���������, ����ָ������, �ⲿ�ͷ�
	* @param	[in]
	*			polygonObj		����ζ���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	�����buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int polygonBufferFace1(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*����Buffer, ����ָ������, �ⲿ�ͷ�*/
	static int multiPointBuffer(
		const GTMultiPoint*    mpointObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	����Buffe, ����ָ������, �ⲿ�ͷ�
	* @param	[in]
	*			mpointObj		������
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	���buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int multiPointBuffer1(
		const GTMultiPoint*    mpointObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	����Buffe, ����ָ������, �ⲿ�ͷ�,���Ӹ������buffer��
	*			���ཻ���жϣ��ཻ��ϲ������ཻ�򲻵��úϲ��������ú�����
	*			multiPointBuffer1Ч�ʸ�
	* @param	[in]
	*			mpointObj		������
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	���buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-21
	*******************************************************************************/
	static int multiPointBuffer2(
		const GTMultiPoint*    mpointObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*���ߵ�Buffer, ����ָ������, �ⲿ�ͷ�*/
	static int multiLineStringBuffer(
		const GTMultiLineString* mlineStringObj, 
		double                   dR,
		std::vector<GTRing *>&   ring_list_out,
		std::vector<int>&        hole_list_out);

	/*****************************************************************************
	* @brief	���ߵ�Buffer, ����ָ������, �ⲿ�ͷţ����ĺϲ���lineStringbuffer����Ϊ
	*			��ÿ����2bufferΪһ�飬���κϲ�
	* @param	[in]
	*			mlineStringObj	���߶���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	����buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-18
	*******************************************************************************/
	static int multiLineStringBuffer1(
		const GTMultiLineString* mlineStringObj, 
		double                   dR,
		std::vector<GTRing *>&   ring_list_out,
		std::vector<int>&        hole_list_out);

	static int multiLineStringBuffer2(
		const GTMultiLineString* mlineStringObj, 
		double                   dR,
		std::vector<GTRing *>&   ring_list_out,
		std::vector<int>&        hole_list_out);

	/*���ߵ�Buffer���ձ߽���, ����ָ������, �ⲿ�ͷ�*/
	static int multiPolygonBuffer(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	�����ε�Buffer, ����ָ������, �ⲿ�ͷ�
	* @param	[in]
	*			mpolygonObj		�����ζ���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	������buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-19
	*******************************************************************************/
	static int multiPolygonBuffer1(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*���ߵ�Buffer���������, ����ָ������, �ⲿ�ͷ�*/
	static int multiPolygonBufferFace(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	�����ε�Buffer���������, ����ָ������, �ⲿ�ͷ�
	* @param	[in]
	*			mpolygonObj		�����ζ���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	������buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int multiPolygonBufferFace1(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	���ζ����Buffer, ����ָ��, �ⲿ�ͷ�
	* @param	[in]
	*			pGeoIn			����ļ��ζ���
	*			dR				buffer�뾶
	*			polyBufferType	���϶���Ϊ����λ������ʱ������buffer������
	*			[out]
	*			pGeoOut			����ļ��ζ���
	* @return	����
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-8-4
	*******************************************************************************/
	static int geometryBuffer(
		const GTGeometry*		pGeoIn, 
		double					dR,
		GTGeometry**			pGeoOut,
		enumGTBufferType		polyBufferType = GT_BUFFER_OUTER);


	static int featureBuffer(const GTFeature* feature, 
								GTFeature** outFeature,
								double dr, 
								const GTField* field = NULL, 
								enumGTBufferType bufferType = GT_BUFFER_OUTER
								);

	static int featureClassBuffer(GTFeatureClass& inFeature, 
								GTFeatureClass& outFeature,
								double dr, 
								const GTField* field = NULL, 
								enumGTBufferType bufferType = GT_BUFFER_OUTER, 
								enumGTBufferDissolveType dissolveType = GT_BUFFER_DISSOLVE_NONE,
								GTFields* fields = NULL,
								checkCondition checkFunc = NULL
								);
	/****************************************************
	 *���º���_mp��ʾ��������㷨*
	 ****************************************************/

	/*****************************************************************************
	* @brief	����Buffer, ����ָ������, �ⲿ�ͷţ��ϲ�����Ϊ����segmentbuffer��ÿ
	*			����2bufferΪһ�飬���κϲ�,�����ö�˲��м���
	* @param	[in]
	*			ringObj			������
	*			dR				buffer�뾶
	*			bufferType		��buffer���ͣ�GT_BUFFER_OUTER��GT_BUFFER_EDGE
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	��buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-18
	*******************************************************************************/
	static int ringBuffer_mp(
		const GTRing*          ringObj,
		double                 dR,
		enumGTBufferType bufferType,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);


	/*�ߵ�Buffer, ����ָ������, �ⲿ�ͷ�*/
	static int lineStringBuffer_mp(
		const GTLineString*    lineStringObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);


	/*****************************************************************************
	* @brief	�ߵ�Buffer, ����ָ������, �ⲿ�ͷ�:�����޸���lineStringBuffer������
	*			���ĺϲ���segmentbuffer����Ϊ��ÿ����2bufferΪһ�飬���κϲ�,������
	*			��˲��м���
	* @param	[in]
	*			lineStringObj	�߶���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	��buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-18
	*******************************************************************************/
	static int lineStringBuffer_mp1(
		const GTLineString*    lineStringObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);


	/*����ε�Buffer���ձ߽���, ����ָ������, �ⲿ�ͷ�*/
	static int polygonBuffer_mp(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	����ε�Buffer, ����ָ������, �ⲿ�ͷţ���˲��м���
	* @param	[in]
	*			polygonObj		����ζ���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	�����buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-19
	*******************************************************************************/
	static int polygonBuffer_mp1(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*����ε�Buffer���������, ����ָ������, �ⲿ�ͷ�*/
	static int polygonBufferFace_mp(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	����ε�Buffe���������, ����ָ������, �ⲿ�ͷ�,��˲��м���
	* @param	[in]
	*			polygonObj		����ζ���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	�����buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int polygonBufferFace_mp1(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);
	
	/*����Buffer, ����ָ������, �ⲿ�ͷ�*/
	static int multiPointBuffer_mp(
		const GTMultiPoint*    mpointObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	����Buffe, ����ָ������, �ⲿ�ͷţ���˲�������
	* @param	[in]
	*			mpointObj		������
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	��buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-21
	*******************************************************************************/
	static int multiPointBuffer_mp1(
		const GTMultiPoint*    mpointObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);
	
	/*���ߵ�Buffer, ����ָ������, �ⲿ�ͷ�*/
	static int multiLineStringBuffer_mp(
		const GTMultiLineString* mlineStringObj, 
		double                   dR,
		std::vector<GTRing *>&   ring_list_out,
		std::vector<int>&        hole_list_out);

	/*****************************************************************************
	* @brief	���ߵ�Buffer, ����ָ������, �ⲿ�ͷţ����ĺϲ���lineStringbuffer����Ϊ
	*			��ÿ����2bufferΪһ�飬���κϲ�,���ö�˲��м���
	* @param	[in]
	*			mlineStringObj	���߶���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	����buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-19
	*******************************************************************************/
	static int multiLineStringBuffer_mp1(
		const GTMultiLineString* mlineStringObj, 
		double                   dR,
		std::vector<GTRing *>&   ring_list_out,
		std::vector<int>&        hole_list_out);

	/*���ߵ�Buffer�����߽���, ����ָ������, �ⲿ�ͷ�*/
	static int multiPolygonBuffer_mp(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	�����ε�Buffer, ����ָ������, �ⲿ�ͷ�,���ö�˲��м���
	* @param	[in]
	*			GTMultiPolygon	�����ζ���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	������buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-19
	*******************************************************************************/
	static int multiPolygonBuffer_mp1(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*���ߵ�Buffer���������, ����ָ������, �ⲿ�ͷ�*/
	static int multiPolygonBufferFace_mp(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	�����ε�Buffer���������, ����ָ������, �ⲿ�ͷ�,���ö�˲��м���
	* @param	[in]
	*			GTMultiPolygon	�����ζ���
	*			dR				buffer�뾶
	*			[out]
	*			ring_list_out	buffer��ring���󼯺�
	*			hole_list_out	��Ӧring���󵺱�־�ļ��ϣ�1:����0:�ǵ�
	* @return	������buffer��ring������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int multiPolygonBufferFace_mp1(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	���ζ����Buffer, ����ָ��, �ⲿ�ͷ�,���ö�˲��м���
	* @param	[in]
	*			pGeoIn			����ļ��ζ���
	*			dR				buffer�뾶
	*			[out]
	*			pGeoOut			����ļ��ζ���
	* @return	����
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-8-4
	*******************************************************************************/
	static int geometryBuffer_mp(
		const GTGeometry*		pGeoIn, 
		double					dR,
		GTGeometry**			pGeoOut,
		enumGTBufferType		polyBufferType = GT_BUFFER_OUTER);

	static int featureBuffer_mp(const GTFeature* feature, 
								GTFeature** outFeature,
								double dr, 
								const GTField* field, 
								enumGTBufferType bufferType
								);

	static int featureClassBuffer_mp(GTFeatureClass& inFeature, 
										GTFeatureClass& outFeature,
										double dr, 
										const GTField* field, 
										enumGTBufferType bufferType = GT_BUFFER_OUTER, 
										enumGTBufferDissolveType dissolveType = GT_BUFFER_DISSOLVE_NONE,
										GTFields* fields = NULL,
										checkCondition checkFunc = NULL
										);

	static int featureClassBuffer_mp1(GTFeatureClass& inFeature, 
										GTFeatureClass& outFeature,
										double dr, 
										const GTField* field, 
										enumGTBufferType bufferType = GT_BUFFER_OUTER, 
										enumGTBufferDissolveType dissolveType = GT_BUFFER_DISSOLVE_NONE,
										GTFields* fields = NULL,
										checkCondition checkFunc = NULL
										);

	static int bufferUnion3(GTFeatureClass* p_tmpClass, GTFeatureClass& outFeature,
								GTFields* fields,
								enumGTBufferDissolveType dissolveType = GT_BUFFER_DISSOLVE_ALL,
								checkCondition checkFunc = NULL,
								bool useRTree = true);

	static int bufferUnion3_mp(GTFeatureClass* p_tmpClass, GTFeatureClass& outFeature,
								GTFields* fields,
								enumGTBufferDissolveType dissolveType = GT_BUFFER_DISSOLVE_ALL,
								checkCondition checkFunc = NULL,
								bool useRTree = true);

	static int bufferUnion2(GTFeatureClass* p_tmpClass, GTFeatureClass& outFeature,
		GTFields* fields,
		enumGTBufferDissolveType dissolveType = GT_BUFFER_DISSOLVE_ALL,
		checkCondition checkFunc = NULL);

	static int reduceUnionRing(std::vector<std::vector<GTRing *> > *uion_origin_ring_list, std::vector<std::vector<int> > *uion_hole_list);

	static int reduceUnionRing2(std::vector<std::vector<GTRing *> > &all_origin_ring_list, 
								std::vector<std::vector<int> > &all_hole_list, 
								std::vector<GTRing *>& ring_list_out,
								std::vector<int>&      hole_list_out);

	static int reduceUnionRing_mp(std::vector<std::vector<GTRing *> > *uion_origin_ring_list, std::vector<std::vector<int> > *uion_hole_list);

	static int polygonizeRing_mp(std::vector<GTRing *> &ring_list_out,
								std::vector<int> &hole_list_out,
								GTGeometry** pGeoOut);

	static int polygonizeRing(std::vector<GTRing *> &ring_list_out,
								std::vector<int> &hole_list_out,
								GTGeometry** pGeoOut);

private:
	/*��Buffer*/
	static int __pointBuffer(
		double  dX, 
		double  dY, 
		double  dR, 
		GTRing* ringObj);

	/*�߶�Buffer(Բ�ζ˵�)*/
	static int __segmentBuffer(
		double  dX1, 
		double  dY1,
		double  dX2, 
		double  dY2, 
		double  dR, 
		GTRing* ringObj);

	/*������Buffer, ���ձ߽���*/
	static int __ringBuffer(
		const GTRing*          ringObj,
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*������Buffer, ���������*/
	static int __ringBufferface(
		const GTRing*          ringObj,
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);


	static bool checkDissolveIntersectWithEnvelop(GTFeature* f1, GTFeature* f2);

	static bool checkDissolveAll(GTFeature* f1, GTFeature* f2);

	static bool checkDissolveList(GTFeature* f1, GTFeature* f2, GTFields* fields);
};

}
#endif

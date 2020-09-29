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
#ifndef GTSGEOMETRYDISSOLVE_H_INCLUDED
#define GTSGEOMETRYDISSOLVE_H_INCLUDED

#include "gts_cg_port.h"

namespace gts{

class GTS_DLL GTGeometryDissolve
{
public:
	GTGeometryDissolve(void);
	~GTGeometryDissolve(void);

	/*���ཻ�ֽ�*/

	/*****************************************************************************
	* @brief	�������ཻ�ֽ�
	* @param	[in]
	*			ring			������
	*			[out]
	*			ringlist		�ֽ��Ļ����󼯺�
	* @return	�ֽ��Ļ�����
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-09-30
	*******************************************************************************/
	static int ringDissolve(const GTRing *ring, std::vector<GTRing *>& ringlist);


	/*****************************************************************************
	* @brief	�ߵ����ཻ�ֽ�
	* @param	[in]
	*			lineStr			�߶���
	*			[out]
	*			lineStrlist		�ֽ����߶��󼯺�
	* @return	�ֽ���������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-10-09
	*******************************************************************************/
	static int lineStringDissolve(const GTLineString *lineStr, std::vector<GTLineString *>& lineStrlist);

	/*��������ཻ�ֽ�*/
	/*�����ཻ�ֽ�*/


	/*���ڰ߿�ϲ�*/

	/*****************************************************************************
	* @brief	ָ���о�����ͬ����ֵ�����ڶ���κϲ���
	* @param	[in]
	*			inputFeatures				����feature����
	*			inputFeatureCount			����feature������
	*			resultFields				���feature�ֶζ���
	*			resultFieldIndex			����ֶ���ԭʼfeature�ֶζ����е�����
	*			resultFieldCount			����ֶθ���
	*			resultFeatures				���feature����
	*			bCreateMultiPartFeature		�Ƿ����ɶಿ�ֶ���
	*			dTolerance					�ݲ�ֵ,���С�ڴ�ֵ��GTRing�ڹ��������ζ���ʱ������.
	*			[out]
	*			resultFeatures				������ϲ���Ľ��feature����
	* @return	���feature����
	* @note		�����feature�Ǿ�������ɸѡ֮���feanture���ϣ�������������кϲ������ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	��������fanjf@lreis.ac.cn,2011-10-11
	*******************************************************************************/
	static int dissolvePolygonsWithSameAttributes(
		std::vector<GTFeature*>	inputFeatures,
		const int				inputFeatureCount,
		GTFields				&resultFields,
		std::vector<int>		resultFieldIndex,
		const int				resultFieldCount,
		std::vector<GTFeature*> &resultFeatures,
		const bool				bCreateMultiPartFeature = true,
		const double			dTolerance = 1e-10);


	/*****************************************************************************
	* @brief	ͼ����ָ���о�����ͬ����ֵ�����ڶ���κϲ���
	* @param	[in]
	*			inputLayer					����ͼ��
	*			resultLayer					���ͼ��
	*			resultFieldIndex			����ֶ���ԭʼfeature�ֶζ����е�����
	*			resultFieldCount			����ֶθ���
	*			bCreateMultiPartFeature		�Ƿ����ɶಿ�ֶ���
	*			dTolerance					�ݲ�ֵ,���С�ڴ�ֵ��GTRing�ڹ��������ζ���ʱ������.
	*			[out]
	* @return	���ؽ������feature��.����ֵΪ-1˵�����ִ���,����ֵΪ-2˵������ͼ��Ŀռ�ο���һ��,����-3˵�����ͼ���ʼ״̬�ֶθ�����Ϊ0.
	* @note		
	* @author	��������fanjf@lreis.ac.cn,2011-10-11
	*******************************************************************************/
	static int dissolvePolygonLayer_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		std::vector<int>	resultFieldIndex,
		const int			resultFieldCount,
		const bool			bCreateMultiPartFeature = true,
		const double		dTolerance = 1e-10);

	
	/*****************************************************************************
	* @brief	ͼ����ָ���о�����ͬ����ֵ�����ڶ���κϲ���
	* @param	[in]
	*			inputLayer					����ͼ��
	*			resultLayer					���ͼ��
	*			resultFieldIndex			����ֶ���ԭʼfeature�ֶζ����е�����
	*			resultFieldCount			����ֶθ���
	*			bCreateMultiPartFeature		�Ƿ����ɶಿ�ֶ���
	*			dTolerance					�ݲ�ֵ,���С�ڴ�ֵ��GTRing�ڹ��������ζ���ʱ������.
	*			[out]
	* @return	���ؽ������feature��.����ֵΪ-1˵�����ִ���,����ֵΪ-2˵������ͼ��Ŀռ�ο���һ��,����-3˵�����ͼ���ʼ״̬�ֶθ�����Ϊ0.
	* @note		��Ӻ������ݴ�������.
	* @author	��������fanjf@lreis.ac.cn,2012-06-15
	*******************************************************************************/
	static int dissolvePolygonLayer(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		std::vector<int>	resultFieldIndex,
		const int			resultFieldCount,
		const bool			bCreateMultiPartFeature = true,
		const double		dTolerance = 1e-10,
		const int           blockSize = 200000);

	static int dissolvePolygonLayer_mp(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		std::vector<int>	resultFieldIndex,
		const int			resultFieldCount,
		const bool			bCreateMultiPartFeature = true,
		const double		dTolerance = 1e-10,
		const int           blockSize = 200000);

	/*���صļ��ζ����ɵ������ͷ����ڴ�*/
	static GTGeometry* convertSingleToMulti_Point(
		const GTGeometry*	inputPointA,
		const GTGeometry*	inputPointB);

	static GTGeometry* convertSingleToMulti_LineString(
		const GTGeometry*	inputLineA,
		const GTGeometry*	inputLineB);

	static GTGeometry* convertSingleToMulti_Polygon(
		const GTGeometry*	inputPolygonA,
		const GTGeometry*	inputPolygonB);


	/*****************************************************************************
	* @brief	ͼ����ָ���о�����ͬ����ֵ��feature�ۺ�Ϊһ��multi part feature��
	* @param	[in]
	*			inputLayer					����ͼ��,ΪGT_Pont����GT_MultiPont����
	*			resultLayer					���ͼ��,����ΪGT_MultiPont����
	*			resultFieldIndex			����ֶ���ԭʼfeature�ֶζ����е�����
	*			resultFieldCount			����ֶθ���
	*			[out]
	* @return	���ؽ������feature��.����ֵΪ-1˵�����ִ���;
	*			����ֵΪ-2˵������ͼ��Ŀռ�ο���һ��;
	*			����-3˵�����ͼ���ʼ״̬�ֶθ�����Ϊ0.
	* @note		��Ӻ������ݴ�������.
	* @author	��������fanjf@lreis.ac.cn,2012-06-15/07-09 edited
	*******************************************************************************/
	static int dissolvePointLayerByAttributes(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		std::vector<int>	resultFieldIndex,
		const int			resultFieldCount,
		const int           blockSize = 200000);


	/*****************************************************************************
	* @brief	ͼ����ָ���о�����ͬ����ֵ��feature�ۺ�Ϊһ��multi part feature��
	* @param	[in]
	*			inputLayer					����ͼ��,ΪGT_Pont����GT_MultiPont����
	*			resultLayer					���ͼ��,����ΪGT_MultiPont����
	*			resultFieldIndex			����ֶ���ԭʼfeature�ֶζ����е�����
	*			resultFieldCount			����ֶθ���
	*			[out]
	* @return	���ؽ������feature��.����ֵΪ-1˵�����ִ���;
	*			����ֵΪ-2˵������ͼ��Ŀռ�ο���һ��;
	*			����-3˵�����ͼ���ʼ״̬�ֶθ�����Ϊ0.
	* @note		��Ӻ������ݴ�������.
	* @author	��������fanjf@lreis.ac.cn,2012-06-15/07-09 edited
	*******************************************************************************/
	static int dissolveLineLayerByAttributes(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		std::vector<int>	resultFieldIndex,
		const int			resultFieldCount,
		const int           blockSize = 200000);

	/*ָ����������ͬ�ϲ�*/

};

}
#endif


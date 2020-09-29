/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ���ζ���֮�������ת��
* 
* Author:   ������, fanjf@lreis.ac.cn 2011��-05��-11��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c) ��������Ȩ��
****************************************************************/
#ifndef GTSGEOMETRYCONVERT_H_INCLUDED
#define GTSGEOMETRYCONVERT_H_INCLUDED

#include "gts_cg_port.h"
#include "gt_geometrysimplicify.h"
#include "gt_geometrynearest.h"

namespace gts{

struct GTGCNPoint{
	GTRawPoint                point;
	std::vector<unsigned int> edge_list;
	unsigned int              edge_count;
	unsigned int              mark;
};

struct GTGCNLineString{
	GTLineString line;
	unsigned int start_node;
	unsigned int end_node;
	unsigned int visited;
};

class GTS_DLL GTGeometryConvert
{
public:
	GTGeometryConvert(void);
	~GTGeometryConvert(void);

	/*����������
	 *����߶�֮�����ص�, ���������Ƴ��ص�����
	 *�����߶β��ܱպ�
	 */
	static int convertLineToPolygon(
		std::vector<GTLineString *>& lineString_list,
		const int                    lineString_count,
        std::vector<GTRing *>&       ring_list_out,
		std::vector<GTLineString>&   line_list_out);
	
	/*****************************************************************************
	* @brief	����������,��д�㷨��ͨ���ֱ������ߴ����뽻������������������
	*			���°棬�����Ƿ���bug
	* @param	[in]
	*			lineString_list				�ߴ�����
	*			lineString_count			�ߴ�����	
	*			[out]
	*			ring_list_out				���ɵĻ�����
	* @return	���ɺ�Ļ���������
	* @note		���ص�ָ��������Ҫ�ⲿ�ͷ�
	* @author	������, fanjf@lreis.ac.cn,2011-8-26
	*******************************************************************************/
	static int convertLineToPolygon3(
		std::vector<GTLineString *>& lineString_list,
		const int                    lineString_count,
        std::vector<GTRing *>&       ring_list_out,
		std::vector<GTLineString>&   line_list_out);

	/*��������*/
	
	/*�����ɵ�*/

	/*****************************************************************************
	* @brief	��lineString����multiLineStringת�ɵ㣨centroid��
	* @param	[in]
	*			inputLine			lineString����multiLineString���͵ļ��ζ���
	* @param	[out]
	* @return	����lineString����multiLineString��centroid.
	* @note		����ֵΪGTRawPoint����.
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *lineCentroid(
		const GTGeometry	*inputLine);

	/*****************************************************************************
	* @brief	��lineString����multiLineStringת���ɵ㣨inside,ȡ�ߴ��Ͼ����ߵ�envelop��������ĵ㣩
	* @param	[in]
	*			inputLine			lineString����multiLineString���͵ļ��ζ���
	* @param	[out]
	* @return	����ȡlineString����multiLineString�Ͼ��뼸�ζ����Envelop��������ĵ�.
	* @note		����ֵΪGTRawPoint����.
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *lineToPointInside(
		const GTGeometry	*inputLine);

	/*****************************************************************************
	* @brief	��lineString�ĳ����е㣨����������һ������1/2�ܳ��ȵ�vertex�����õȱȶ������е����꣩
	* @param	[in]
	*			input_line			�ߴ����ζ���.
	*			dTolerance			�ݲ�ֵ,��2�߶γ���֮��С�ڴ�ֵʱ��Ϊ���߳������.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�ߴ��ĳ����е�,�õ㲻һ�����ߴ���vertex֮һ,����һ�����ߴ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *lineStringMiddleLengthPoint(
		GTLineString *input_line,
		const double  dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	��lineString����multiLineString��ת���ɵ��
	* @param	[in]
	*			inputLineLayer			lineString����multiLineStringͼ��.
	*			resultLayer				���ͼ��,����Ϊ���,�Һ���������ʱ�Ѿ���ʼ��;
	*									�ɸ���;�ֶθ���Ϊ0;�ռ�ο���������߲�һ��.
	*			isInside				true:ȡ���Ͼ����ߵ����������������ĵ�; false:ȡ�ߵ�centroid.
	* @param	[out]
	* @return	���ͼ�����վ���������ͼ����ͬ�����Խṹ.
	* @note		ע���ͷŽ��ͼ���Լ����ͼ��������ֶζ���;multiLineString��centroidΪÿ��lineString��centroid��x/y��ֵ.
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static int convertLineLayerToPointLayer_old(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false);


	/*****************************************************************************
	* @brief	��lineString����multiLineString��ת���ɵ��,��Ӻ������ݴ�������,���ڴ�й©.
	* @param	[in]
	*			inputLineLayer			lineString����multiLineStringͼ��.
	*			resultLayer				���ͼ��,����Ϊ���,�Һ���������ʱ�Ѿ���ʼ��;
	*									�ɸ���;�ֶθ���Ϊ0;�ռ�ο���������߲�һ��.
	*			isInside				true:ȡ���Ͼ����ߵ����������������ĵ�; false:ȡ�ߵ�centroid.
	* @param	[out]
	* @return	���ͼ�����վ���������ͼ����ͬ�����Խṹ.
	* @note		ע���ͷŽ��ͼ���Լ����ͼ��������ֶζ���;multiLineString��centroidΪÿ��lineString��centroid��x/y��ֵ.
	* @author	������,fanjf@lreis.ac.cn,2012-06-13
	*******************************************************************************/
	static int convertLineLayerToPointLayer(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false,
		const int            blockSize = 200000);

	/*****************************************************************************
	* @brief	��lineString����multiLineString��ת���ɵ��,��Ӻ������ݴ�������,���ڴ�й©.
	*			������˰汾.
	* @param	[in]
	*			inputLineLayer			lineString����multiLineStringͼ��.
	*			resultLayer				���ͼ��,����Ϊ���,�Һ���������ʱ�Ѿ���ʼ��;
	*									�ɸ���;�ֶθ���Ϊ0;�ռ�ο���������߲�һ��.
	*			isInside				true:ȡ���Ͼ����ߵ����������������ĵ�; false:ȡ�ߵ�centroid.
	* @param	[out]
	* @return	���ͼ�����վ���������ͼ����ͬ�����Խṹ.
	* @note		ע���ͷŽ��ͼ���Լ����ͼ��������ֶζ���;multiLineString��centroidΪÿ��lineString��centroid��x/y��ֵ.
	* @author	������,fanjf@lreis.ac.cn,2012-06-13
	*******************************************************************************/
	static int convertLineLayerToPointLayer_mp(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false,
		const int            blockSize = 50000);




	/*���ת����*/
	/*****************************************************************************
	* @brief	multiPoint��centroidΪ�������x��yƽ��ֵ
	* @param	[in]
	*			inputPoints			multiPoint���͵ļ��ζ���
	* @param	[out]
	* @return	����multiPoint��centroid.
	* @note		����ֵΪGTRawPoint����.
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *multiPointsCentroid(
		const GTGeometry	*inputPoints);

	/*****************************************************************************
	* @brief	multiPoint�����е��о���centroid����ĵ�
	* @param	[in]
	*			inputPoints			multiPoint���͵ļ��ζ���
	* @param	[out]
	* @return	����multiPoint���е��о���centroid����ĵ�.
	* @note		����ֵΪGTRawPoint����.
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *multiPointsToPointInside(
		const GTGeometry	*inputPoints);

	/*****************************************************************************
	* @brief	��multiPoint��ת���ɵ��
	* @param	[in]
	*			inputMultiPointLayer	multiPointͼ��.
	*			resultLayer				���ͼ��,����Ϊ���,�Һ���������ʱ�Ѿ���ʼ��;
	*									�ɸ���;�ֶθ���Ϊ0;�ռ�ο��������multiPoint��һ��.
	*			isInside				true:ȡmultiPoint���е��о�����centroid����ĵ�; false:ȡmultiPoint��centroid.
	* @param	[out]
	* @return	���ͼ�����վ���������ͼ����ͬ�����Խṹ.
	* @note		ע���ͷŽ��ͼ���Լ����ͼ��������ֶζ���;multiPoint��centroidΪÿ����x/y��ֵ.
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static int convertMultiPointLayerToPointLayer_old(
		GTFeatureLayer		*inputMultiPointLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false);

	/*****************************************************************************
	* @brief	��multiPoint��ת���ɵ��
	* @param	[in]
	*			inputMultiPointLayer	multiPointͼ��.
	*			resultLayer				���ͼ��,����Ϊ���,�Һ���������ʱ�Ѿ���ʼ��;
	*									�ɸ���;�ֶθ���Ϊ0;�ռ�ο��������multiPoint��һ��.
	*			isInside				true:ȡmultiPoint���е��о�����centroid����ĵ�; false:ȡmultiPoint��centroid.
	* @param	[out]
	* @return	���ͼ�����վ���������ͼ����ͬ�����Խṹ.
	* @note		ע���ͷŽ��ͼ���Լ����ͼ��������ֶζ���;multiPoint��centroidΪÿ����x/y��ֵ.
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static int convertMultiPointLayerToPointLayer(
		GTFeatureLayer		*inputMultiPointLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false,
		const int			 blockSize = 200000);

	/*������˰汾*/
	static int convertMultiPointLayerToPointLayer_mp(
		GTFeatureLayer		*inputMultiPointLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false,
		const int			 blockSize = 50000);

		

		

	/*�ߵ�vertex���ɵ�*/
	/*****************************************************************************
	* @brief	��lineString����multiLineString��verticesת�ɵ㣨�۵�ת�㣩
	* @param	[in]
	*			inputLine			lineString����multiLineString���͵ļ��ζ���
	*			point_list_out		����㼯��
	*			point_location		0:ALL,ȡ����vertex; 1:MID,ȡÿ���ߵ��е�; 2:START,ȡÿ���ߵ����;
	*								3:END,ȡÿ���ߵ��յ�; 4:BOTH_ENDS,ȡÿ���ߵ������յ�.
	*			dTolerance			�ݲ�ֵ,��2�߶γ���֮��С�ڴ�ֵʱ��Ϊ���߳������.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	����lineString����multiLineString����������vertex���ϼ������.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static int lineVerticesToPoints(
		const GTGeometry			*inputLine,
		std::vector<GTRawPoint*>	&point_list_out,
		const int					 point_location = 0,
		const double				 dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	��lineString����multiLineString��ת���ɵ��(����ת��)
	* @param	[in]
	*			inputLineLayer			lineString����multiLineStringͼ��.
	*			resultLayer				���ͼ��,����Ϊ���,�Һ���������ʱ�Ѿ���ʼ��;
	*									�ɸ���;�ֶθ���Ϊ0;�ռ�ο���������߲�һ��.
	*			pointLocation			0:ALL,ȡ����vertex; 1:MID,ȡÿ���ߵ��е�; 2:START,ȡÿ���ߵ����;
	*									3:END,ȡÿ���ߵ��յ�; 4:BOTH_ENDS,ȡÿ���ߵ������յ�.
	*			dTolerance				�ݲ�ֵ,��2�߶γ���֮��С�ڴ�ֵʱ��Ϊ���߳������.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	���ͼ�����վ���������ͼ����ͬ�����Խṹ.
	* @note		ע���ͷŽ��ͼ���Լ����ͼ��������ֶζ���;ֻ�����ߴ������е�ʱ��ʹ���ݲ�ֵ.
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static int convertLineLayerToPointLayerByVertices_old(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const int			 pointLocation = 0,
		const double		 dTolerance = 1e-10);


	/*****************************************************************************
	* @brief	��lineString����multiLineString��ת���ɵ��(����ת��)
	*			��Ӻ������ݴ�������,�Ѳ�,���ڴ�й©.
	* @param	[in]
	*			inputLineLayer			lineString����multiLineStringͼ��.
	*			resultLayer				���ͼ��,����Ϊ���,�Һ���������ʱ�Ѿ���ʼ��;
	*									�ɸ���;�ֶθ���Ϊ0;�ռ�ο���������߲�һ��.
	*			pointLocation			0:ALL,ȡ����vertex; 1:MID,ȡÿ���ߵ��е�; 2:START,ȡÿ���ߵ����;
	*									3:END,ȡÿ���ߵ��յ�; 4:BOTH_ENDS,ȡÿ���ߵ������յ�.
	*			dTolerance				�ݲ�ֵ,��2�߶γ���֮��С�ڴ�ֵʱ��Ϊ���߳������.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	���ͼ�����վ���������ͼ����ͬ�����Խṹ.
	* @note		ע���ͷŽ��ͼ���Լ����ͼ��������ֶζ���;ֻ�����ߴ������е�ʱ��ʹ���ݲ�ֵ.
	* @author	������,fanjf@lreis.ac.cn,2011-6-13
	*******************************************************************************/
	static int convertLineLayerToPointLayerByVertices(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const int			 pointLocation = 0,
		const double		 dTolerance = 1e-10,
		const int            blockSize = 200000);

	/*������˰汾,�Ѳ�,���ڴ�й©.2012-6-13*/
	static int convertLineLayerToPointLayerByVertices_mp(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const int			 pointLocation = 0,
		const double		 dTolerance = 1e-10,
		const int            blockSize = 200000);


	/*��������*/

	/*****************************************************************************
	* @brief	̽������A�Ͷ����B�Ƿ���ڹ����ߣ�A\B�Ĺ����߿��ܴ��ڲ�ͬ�ĵ㣩
	* @param	[in]
	*			polygonA				�����A.
	*			polygonB				�����B.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	true/false.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-10-10
	*******************************************************************************/
	static bool isHaveCommonEdges(
		const GTGeometry			*polygonA,
		const GTGeometry			*polygonB,
		const double				 dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	̽������A�Ͷ����B�Ĺ����ߣ�A\B�Ĺ����߿��ܴ��ڲ�ͬ�ĵ㣩
	* @param	[in]
	*			polygonA				�����A.
	*			polygonB				�����B.
	*			lineString_out			�����A\B�Ĺ�����,ע���ͷ��ڴ�.
	*			lineString_marker		�����ߵı�־. 0:��ӦlineString���ڶ����A;  1:��ӦlineString���ڶ����B.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�����ߵĸ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int detectCommonEdges(
		const GTGeometry			*polygonA,
		const GTGeometry			*polygonB,
		std::vector<GTLineString*>	&lineString_out,
		std::vector<int>			&lineString_marker,
		const double				 dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	̽����������ͼ�������ڶ���εĹ�����.
	* @param	[in]
	*			inputLayer				����Ķ����ͼ��.
	*			resultLayer				���ͼ��,��������ǰ�����½�,���ֶ���Ϊ0,����Ϊ�ɸ���.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�����ߵĸ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int getCommonEdges_old(
		GTFeatureLayer *inputLayer,
		GTFeatureLayer *resultLayer,
		const double	dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	̽����������ͼ�������ڶ���εĹ�����.
	* @param	[in]
	*			inputLayer				����Ķ����ͼ��.
	*			resultLayer				���ͼ��,��������ǰ�����½�,���ֶ���Ϊ0,����Ϊ�ɸ���.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�����ߵĸ���.
	* @note		��Ӻ������ݴ�������,�Ѳ�,���ڴ�й©.
	* @author	������,fanjf@lreis.ac.cn,2012-6-14
	*******************************************************************************/
	static int getCommonEdges(
		GTFeatureLayer *inputLayer,
		GTFeatureLayer *resultLayer,
		const double	dTolerance = 1e-10,
		const int       blockSize = 200000);

	/*****************************************************************************
	* @brief	�������ÿ���������(�յ�)�����lineString.
	* @param	[in]
	*			polygonA				����Ķ����.
	*			lineString_out			�����A��,ʹ�����ע���ͷ��ڴ�.
	* @param	[out]
	* @return	�ߵĸ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static  int convertPolygonToLines(
		const GTGeometry			*polygonA,
		std::vector<GTLineString*>  &lineString_out);

	/*****************************************************************************
	* @brief	����A��B�Ĺ�����(������ԭ���ڶ����A�Ĺ����ߣ�����������ring��id�Լ�
	*			��������ring�ϵ���ʼ�����ֹ��ID��������֤�õ��Ĺ������ϵĵ㶼�Ƕ����A�Ļ��ϵĵ�)
	* @param	[in]
	*			polygonA				����Ķ����A.
	*			polygonB				����Ķ����B.
	*			lineString_out			�����AB�Ĺ�����,ʹ�����ע���ͷ��ڴ�.
	*			ringId_A				����������ring��id,��AΪmultiPolygon,��ringIDΪ�ӵ�һ������ε����һ��������ۼӵ�ring����
	*			fromAndToPointID_out	�����ߵ���ʼ�����ֹ����ring�ϵ�ID.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�ߵĸ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static  int detectCommonEdgesOnlyOnPolygonAWithMarkers(
		const GTGeometry				*polygonA,
		const GTGeometry				*polygonB,
		std::vector<GTLineString*>		&lineStringOut,
		std::vector<int>				&ringId_A,
		std::vector<std::vector<int> >	&fromAndToPointID_out,
		const double					 dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	����A��B�Ĺ�����(������ԭ���ڶ����A�Ĺ����ߣ�����������ring��id�Լ���
	*			Ӧ�����B�ϻ���id��������֤�õ��Ĺ������ϵĵ㶼�Ƕ����A�Ļ��ϵĵ�)
	* @param	[in]
	*			polygonA				����Ķ����A.
	*			polygonB				����Ķ����B.
	*			lineString_out			�����AB�Ĺ�����,ʹ�����ע���ͷ��ڴ�.
	*			ringId_A				����������ring��id,��AΪmultiPolygon,��ringIDΪ�ӵ�һ������ε����һ��������ۼӵ�ring����
	*			ringId_B				����������ring��id,��BΪmultiPolygon,��ringIDΪ�ӵ�һ������ε����һ��������ۼӵ�ring����
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�ߵĸ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int detectCommonEdgesOnlyOnPolygonAWithMarkers(
		const GTGeometry			*polygonA,
		const GTGeometry			*polygonB,
		std::vector<GTLineString*>	&lineStringOut,
		std::vector<int>			&ringId_A,
		std::vector<int>			&ringId_B,
		const double				 dTolerance = 1e-10);


	/*****************************************************************************
	* @brief	�������ͼ��תΪ�߲�(�������Ժ��߶���������ID)
	* @param	[in]
	*			inputLayer				����Ķ����ͼ��.
	*			resultLayer				���ͼ��.��������ǰ�����½�,���ֶ���Ϊ0,����Ϊ�ɸ���.
	*			bKeepNonCommonEdges		���������Ƿ����ǹ�����.
	*			bSplitOnIntersectPoint	��Զ����ͼ���ڲ�����ѹ�ǵ����,�Ƿ��ڱߵĽ��㴦���.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�ߵĸ���.
	* @note		�ú���δ����������union��ֱ�Ӵ�ϣ����ڹ�������״һ�����㲻һ�����´�Ϻ����ʣ���ߴ������빫���������غϵ�����
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int convertPolygonLayerToLineLayerWithAttributes_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	�������ͼ��תΪ�߲�(�������Ժ��߶���������ID),��Ӻ������ݴ�������.
	* @param	[in]
	*			inputLayer				����Ķ����ͼ��.
	*			resultLayer				���ͼ��.��������ǰ�����½�,���ֶ���Ϊ0,����Ϊ�ɸ���.
	*			bKeepNonCommonEdges		���������Ƿ����ǹ�����.
	*			bSplitOnIntersectPoint	��Զ����ͼ���ڲ�����ѹ�ǵ����,�Ƿ��ڱߵĽ��㴦���.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�ߵĸ���.
	* @note		�ú���δ����������union��ֱ�Ӵ�ϣ����ڹ�������״һ�����㲻һ�����´�Ϻ����ʣ���ߴ������빫���������غϵ�����
	* @author	������,fanjf@lreis.ac.cn,2012-6-14
	*******************************************************************************/
	static int convertPolygonLayerToLineLayerWithAttributes(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10,
		const int           blockSize = 200000);
	/*������˰汾*/
	static int convertPolygonLayerToLineLayerWithAttributes_mp(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10,
		const int           blockSize = 200000);


	/*****************************************************************************
	* @brief	��A\B�Ĺ�����������A�Ĳ���.
	* @param	[in]
	*			polygonA				����Ķ����A.
	*			polygonB				����Ķ����B.
	*			lineStringOut			����Ĺ������б�.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�ߵĸ���.
	* @note		���lineString�ϵĵ㲻һ������B,�˺�������Ϊ����detectCommonEdgesOnlyOnPolygonAWithMarkers�Ĳ�����ṩ.
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int detectCommonEdgesOnlyOnPolygonA(
		const GTGeometry			*polygonA,
		const GTGeometry			*polygonB,
		std::vector<GTLineString*>	&lineStringOut,
		const double				dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	������������ͬ��״���ܾ��в�ͬ��ļ��ζ���ϲ�.Ҫ�ϲ����߱�������غϵ���
	*			״��������״�ϵĵ���Բ�ͬ��ȡ���в�ͬ�ĵ����һ��ͬԭʼ��״һ�����߷���.
	* @param	[in]
	*			line_str_a				����ĵ�1��lineString.
	*			line_str_b				����ĵ�2��lineString.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�ϲ����.
	* @note		��������Ϊɾ���ظ��ĵ�,������ͬ��,��˸ú�������������������lineString�ϲ�.
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static GTLineString* unionLineStringsWithSameShapeToOne(
		GTLineString	*line_str_a,
		GTLineString	*line_str_b,
		const double	dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	�󹫹��ߣ����ع�������A\B 2�������������в�ͬ��ļ��ϣ���������ԭ����A����ԭ����B����ͬ���ֽ�����һ�ݣ�
	* @param	[in]
	*			polygonA				����Ķ����A.
	*			polygonB				����Ķ����B.
	*			lineStringOut			����Ĺ������б�.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�����߸���.
	* @note		ʵ��˼·Ϊ:�ȷֱ���A/B������,�ٽ��غ��ߺϲ�,ʹ��unionLineStringsWithSameShapeToOne����.
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int detectUnionedCommonEdges(
		const GTGeometry			*polygonA,
		const GTGeometry			*polygonB,
		std::vector<GTLineString*>	&lineStringOut,
		const double				dTolerance = 1e-10);


	/*****************************************************************************
	* @brief	�������ͼ��תΪ�߲�(���������ԣ������߽�����һ��)
	* @param	[in]
	*			inputLayer				����Ķ����ͼ��.
	*			resultLayer				���ͼ��.��������ǰ�����½�,���ֶ���Ϊ0,����Ϊ�ɸ���.
	*			bKeepNonCommonEdges		���������Ƿ����ǹ�����.
	*			bSplitOnIntersectPoint	��Զ����ͼ���ڲ�����ѹ�ǵ����,�Ƿ��ڱߵĽ��㴦���.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�ߵĸ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int convertPolygonLayerToLineLayer_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	�������ͼ��תΪ�߲�(���������ԣ������߽�����һ��).��Ӻ������ݴ�������.
				�Ѳ�,���ڴ�й©.2012-6-13
	* @param	[in]
	*			inputLayer				����Ķ����ͼ��.
	*			resultLayer				���ͼ��.��������ǰ�����½�,���ֶ���Ϊ0,����Ϊ�ɸ���.
	*			bKeepNonCommonEdges		���������Ƿ����ǹ�����.
	*			bSplitOnIntersectPoint	��Զ����ͼ���ڲ�����ѹ�ǵ����,�Ƿ��ڱߵĽ��㴦���.
	*			dTolerance				�ݲ�ֵ,�����жϵ��Ƿ��غ�,���Ƿ�������,���Ƿ��ڻ��ڵ�.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�ߵĸ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2012-06-13
	*******************************************************************************/
	static int convertPolygonLayerToLineLayer(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10,
		const int           blockSize = 200000);
	/*������˰汾*/
	static int convertPolygonLayerToLineLayer_mp(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10,
		const int           blockSize = 200000);

		

	/*****************************************************************************
	* @brief	�������ͼ��תΪ�߲�(�򵥴��ÿ����������ԭʼ����)
	* @param	[in]
	*			inputLayer				����Ķ����ͼ��.
	*			resultLayer				���ͼ��.��������ǰ�����½�,���ֶ���Ϊ0,����Ϊ�ɸ���.
	*			isOneToOne				true:������Ķ���κͶ�����תΪ����; false:ÿ����תΪһ����(һ��feature).
	* @param	[out]
	* @return	�ߵĸ���.
	* @note		�������յ㴦�򵥴��ÿ����ΪlineString.
	* @author	������,fanjf@lreis.ac.cn,2011-9-20
	*******************************************************************************/
	static int convertPolygonLayerToLineLayer_Simple_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			isOneToOne = false);

	/*****************************************************************************
	* @brief	�������ͼ��תΪ�߲�(�򵥴��ÿ����������ԭʼ����)
	*			��Ӻ������ݴ�������.�Ѳ�,���ڴ�й©.
	* @param	[in]
	*			inputLayer				����Ķ����ͼ��.
	*			resultLayer				���ͼ��.��������ǰ�����½�,���ֶ���Ϊ0,����Ϊ�ɸ���.
	*			isOneToOne				true:������Ķ���κͶ�����תΪ����; false:ÿ����תΪһ����(һ��feature).
	* @param	[out]
	* @return	�ߵĸ���.
	* @note		�������յ㴦�򵥴��ÿ����ΪlineString.
	* @author	������,fanjf@lreis.ac.cn,2012-06-14
	*******************************************************************************/
	static int convertPolygonLayerToLineLayer_Simple(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			isOneToOne = false,
		const int           blockSize = 200000);
	/*������˰汾*/
	static int convertPolygonLayerToLineLayer_Simple_mp(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			isOneToOne = false,
		const int           blockSize = 200000);

	/*�����ɵ�(��ʱû��ʵ��Very Important Point����ȡ)*/
	/*****************************************************************************
	* @brief	��������۵�ת��
	* @param	[in]
	*			polygonA				����Ķ����.
	*			resultPoints			����㼯��.
	* @param	[out]
	* @return	��ĸ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-29
	*******************************************************************************/
	static int polygonVerticesToPoints(
		const GTGeometry		  *polygonA,
		std::vector<GTRawPoint*>  &resultPoints
		);

	/*****************************************************************************
	* @brief	��������۵�ת��/���
	* @param	[in]
	*			inputLayer			����Ķ����ͼ��.
	*			resultLayer			�����ͼ��.
	*			bMultiToSingle		�Ƿ񽫶���ɢΪ����.
	* @param	[out]
	* @return	��ĸ���.
	* @note		bMultiToSingleΪtrueʱ,���ͼ�㼸������ΪGT_WKBPoint;bMultiToSingleΪfalseʱ,���ͼ�㼸������ΪGT_WKBMultiPoint.
	* @author	������,fanjf@lreis.ac.cn,2011-9-29
	*******************************************************************************/
	static int convertPolygonLayerToPointLayerByVertices_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool           bMultiToSingle = false);

	/*****************************************************************************
	* @brief	��������۵�ת��/���.��Ӻ������ݴ�������.�Ѳ�,���ڴ�й©.
	* @param	[in]
	*			inputLayer			����Ķ����ͼ��.
	*			resultLayer			�����ͼ��.
	*			bMultiToSingle		�Ƿ񽫶���ɢΪ����.
	* @param	[out]
	* @return	��ĸ���.
	* @note		bMultiToSingleΪtrueʱ,���ͼ�㼸������ΪGT_WKBPoint;bMultiToSingleΪfalseʱ,���ͼ�㼸������ΪGT_WKBMultiPoint.
	* @author	������,fanjf@lreis.ac.cn,2012-6-14
	*******************************************************************************/
	static int convertPolygonLayerToPointLayerByVertices(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool           bMultiToSingle = false,
		const int            blockSize = 200000);
	/*������˰汾*/
	static int convertPolygonLayerToPointLayerByVertices_mp(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool           bMultiToSingle = false,
		const int            blockSize = 200000);

	//static int convertPolygonLayerToPointLayerByVertices_mp2(
	//	GTFeatureLayer		*inputLayer,
	//	GTFeatureLayer		*resultLayer,
	//	const bool           bMultiToSingle = false,
	//	const int            blockSize = 200000);

	/*****************************************************************************
	* @brief	������/�����ε�Centroid(������ÿ���ֵ�Centroid).
	* @param	[in]
	*			polygon				����λ�����ζ���.
	*			centroids			���������
	*			isMultiToOne		true:multiPolygon�õ��Ķ���㱻�����һ��,false:multiPolygon���ض����
	*			dTolerance			�ݲ�ֵ,Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	���Centroid����.
	* @note		����GTCPGeometryBasic::ringCentroid()����ʵ��,ͬһ�򵥶�������⻷������ڻ���������ȼ�������(�⻷��ȥ�ڻ�).
	*			ͬһ�����εĸ����򵥲��ֶ����֮�䰴�������Ȩ����ƽ�����ꡣ
	* @author	������,fanjf@lreis.ac.cn,2011-10-20
	*******************************************************************************/
	static int polygonCentroid(
		const GTGeometry		 *polygon,
		std::vector<GTRawPoint*> &centroids,
		const bool				 isMultiToOne = true);

	/*****************************************************************************
	* @brief	�������Centroidת��/���
	* @param	[in]
	*			inputLayer			����Ķ����ͼ��.
	*			resultLayer			�����ͼ��.
	*			bMultiToSingle		�Ƿ񽫶��ۺ�Ϊ��һ��.
	* @param	[out]
	* @return	��ĸ���.
	* @note		���ͼ�㼸������ΪGT_Point.
	* @author	������,fanjf@lreis.ac.cn,2011-10-20
	*******************************************************************************/
	static int convertPolygonLayerToPointLayerByCentroid_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool           bMultiToSingle = true);

	/*****************************************************************************
	* @brief	�������Centroidת��/���.��Ӻ������ݴ�������.�Ѳ�,���ڴ�й©.
	* @param	[in]
	*			inputLayer			����Ķ����ͼ��.
	*			resultLayer			�����ͼ��.
	*			bMultiToSingle		�Ƿ񽫶��ۺ�Ϊ��һ��.
	* @param	[out]
	* @return	��ĸ���.
	* @note		���ͼ�㼸������ΪGT_Point.
	* @author	������,fanjf@lreis.ac.cn,2012-6-13
	*******************************************************************************/
	static int convertPolygonLayerToPointLayerByCentroid(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool           bMultiToSingle = true,
		const int            blockSize = 200000);

	/*������˰汾*/
	static int convertPolygonLayerToPointLayerByCentroid_mp(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool           bMultiToSingle = true,
		const int            blockSize = 200000);


private:
	static bool compare(const GTRawPoint& p1, const GTRawPoint& p2);
	static int  segmentLineStringIntersect(
		GTRawPoint&              p1, 
		GTRawPoint&              p2, 
		GTLineString&            lineString,
		std::vector<GTRawPoint>& point_list_out);

	/*****************************************************************************
	* @brief	��lineString�ĳ����е㣨����������һ������1/2�ܳ��ȵ�vertex�����õȱȶ������е����꣩
	* @param	[in]
	*			input_line			�ߴ����ζ���.
	*			dTolerance			�ݲ�ֵ,��2�߶γ���֮��С�ڴ�ֵʱ��Ϊ���߳������.Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	�ߴ��ĳ����е�,�õ㲻һ�����ߴ���vertex֮һ,����һ�����ߴ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *getLineStringMiddleLengthPoint(
		GTLineString *input_line,
		const double  dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	�жϵ��Ƿ��ڶ�����ڲ�.
	* @param	[in]
	*			_ptn				�����.
	*			_polygon			����ζ���
	*			dTolerance			�ݲ�ֵ,Ĭ��Ϊ1e-10.
	* @param	[out]
	* @return	true:���ڶ�����ڲ�(ĳ���⻷��������һ���ڻ�����).
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static bool isPointWithInPolygon(
		const GTRawPoint *_ptn,
		const GTGeometry *_polygon,
		const double      dTolerance = 1e-10); 

	/*****************************************************************************
	* @brief	�ڲ�ͬ������Դ�ж�ȡfeature, ���feature class.�ú������Զ��ж�
	*			feature layer��DataDriverType,��ΪOGR����Դʱ��where�Ӿ佫��������,
	*			��Ϊ��OGR����Դʱ,ʹ��where�Ӿ��ѯ.
	* @param	[in]
	*			fLayer				feature layer.
	*			cWhere				where�Ӿ�,��"fid>=100 and fid <=200".
	*			iStartFID			��bWhere=false,��ȡfeature����ʼfid(���feature class��������feature).
	*			iEndFID				��bWhere=false,��ȡfeature����ֹfid(���feature class��������feature).
	*			fClass				���ս����feature class.
	* @param	[out]
	* @return	���feature class��feature����.
	* @note		cWhere�Ӿ���fid��1��ʼ,iStartFID��iEndFID���0��ʼ����.
	* @author	������,fanjf@lreis.ac.cn,2012-06-18
	*******************************************************************************/
	static int getFeaturesByFIDorWHERE(
		GTFeatureLayer*			fLayer,
		const char*				cWhere,
		const int				iStartFID,
		const int				iEndFID,
		GTFeatureClass& 		fClass);

};

}
#endif
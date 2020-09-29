/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  Geometry Overlay
* Author:   ������,fanjf@lreis.ac.cn,2011-10-9
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c)  ��������Ȩ��
****************************************************************/
#ifndef GTSGEOMETRYOVERLAY_H_INCLUDED
#define GTSGEOMETRYOVERLAY_H_INCLUDED

#include "gts_cg_port.h"
#include "gt_geometryanalysis.h"
#include "gt_geometryconvert.h"

namespace gts{

class GTS_DLL GTGeometryOverlay
{
private:
	/*****************************************************************************
	* @brief	2��������ͬ���Խṹ��feature�������Ը�ֵ
	* @param	[in]
	*			oringnal_feature		����������Դfezature
	*			target_feature			����ֵ�ļ��ζ���
	* @param	[out]
	* @return	�����ֶ�ֻҪ��һ�������˸�ֵ����������true.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-9-19
	*******************************************************************************/
	static bool feature_attributes_clone(
		GTFeature *oringnal_feature,
		GTFeature &target_feature);
	
public:
	GTGeometryOverlay(void);
	~GTGeometryOverlay(void);
	/*Erase, ��һ�������ͼ��ɾ��ĳ����*/
	/*���뻷*/
	/*���뻷*/
	/*���뻷*/
	/*****************************************************************************
	* @brief	polygon��multipolygon����֮��Ĳ�������,�����µ�polygon��multipolygon
	* @param	[in]
	*			inputGeometry			�������ļ��ζ���
	*			eraseGeometry			�������ζ���
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	����erase������ζ���,����ΪNULL����Ҫ�ⲿ�������ͷ���ָ��.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static GTGeometry* polygonErasePolygon(
		const GTGeometry *inputGeometry,
		const GTGeometry *eraseGeometry,
		const double      dTolerance=1E-10);

	/*****************************************************************************
	* @brief	polygon��multipolygonͼ���������,�����µ�multipolygonͼ��.
	*			ֱ�ӵ���polygonErasePolygon�������м���.
	* @param	[in]
	*			inputFeatureLayer		�������Ķ����ͼ��.
	*			eraseFeatureLayer		Ҫ��������ͼ��Ķ����ͼ��.
	*			resultLayer				���ͼ��,��������ǰ�����½�,���ֶ���Ϊ0,����Ϊ�ɸ���.
	*			chkEraseTopology        �Ƿ��鲢����erase layer�����˴���Խ����ɵ�Ӱ��,���ܽ���,Ĭ��ֵΪfalse.
	*			dTolerance				�ݲ�ֵ,���С�ڴ�ֵ��GTRing�ڹ��������ζ���ʱ������.
	*			[out]
	* @return	���ؽ������feature��.����ֵΪ-1˵�����ִ���,����ֵΪ-2˵������ͼ��Ŀռ�ο���һ��,����-3˵�����ͼ���ʼ״̬�ֶθ�����Ϊ0.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int erasePolygonLayer(
		GTFeatureLayer		*inputFeatureLayer,
		GTFeatureLayer		*eraseFeatureLayer,
		GTFeatureLayer      *resultLayer,
		const bool			 chkEraseTopology = false,
		const double         dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon��multipolygonͼ���������,�����µ�multipolygonͼ��.
	*			ֱ�ӵ���polygonErasePolygon�������м���.(������˰汾)
	* @param	[in]
	*			inputFeatureLayer		�������Ķ����ͼ��.
	*			eraseFeatureLayer		Ҫ��������ͼ��Ķ����ͼ��.
	*			resultLayer				���ͼ��,��������ǰ�����½�,���ֶθ���Ϊ0,����Ϊ�ɸ���.
	*			chkEraseTopology        �Ƿ��鲢����erase layer�����˴���Խ����ɵ�Ӱ��,���ܽ���,Ĭ��ֵΪfalse.
	*			dTolerance				�ݲ�ֵ,���С�ڴ�ֵ��GTRing�ڹ��������ζ���ʱ������.
	*			[out]
	* @return	���ؽ������feature��.����ֵΪ-1˵�����ִ���,����ֵΪ-2˵������ͼ��Ŀռ�ο���һ��,����-3˵�����ͼ���ʼ״̬�ֶθ�����Ϊ0.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int erasePolygonLayer_mp(
		GTFeatureLayer		*inputFeatureLayer,
		GTFeatureLayer		*eraseFeatureLayer,
		GTFeatureLayer      *resultLayer,
		const bool			 chkEraseTopology = false,
		const double         dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon��multipolygon featureclass��������,�����µ�featureclass.
	*			ֱ�ӵ���polygonErasePolygon�������м���.
	* @param	[in]
	*			inputFeatureClass		��������feature����
	*			eraseFeatureClass		��������feature����
	*			chkEraseTopology        �Ƿ��鲢����erase layer�����˴���Խ����ɵ�Ӱ��,���ܽ���,Ĭ��ֵΪfalse.
	*			[out]
	* @return	���ؽ��featureclass.����ֵΪNULL˵�����ִ����inputFeatureClass����ΪNULL.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static GTFeatureClass* polygonsErasePolygons(
		GTFeatureClass        *inputFeatureClass,
		GTFeatureClass        *eraseFeatureClass,
		const bool			   chkEraseTopology = false,
		const double           dTolerance = 1E-10);


	/*Identity, ��Identity�����ȡ���������Ĳ���*/
	/*���뻷*/
	/*���뻷*/
	/*���뻷*/
	/*****************************************************************************
	* @brief	2�������Identity����,����0��2���µļ��ζ���洢��vector��.
	*			Ч��ͬarcgis 2������ε�Identity����.
	* @param	[in]
	*			inputGeometry			���뼸�ζ���
	*			symmdiffGeometry		Identity���ζ���
	*			result_geom_vec			������ζ�������
	*			result_mark_vec			������ζ���������,0:����ͼ�ν�����ǰ��. 1:����˫��.
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	*			[out]
	* @return	���ؽ�����ζ������.����ֵΪ-1˵�����ִ���,1˵��2ͼ������.
	* @note		�����������ⲿ�ͷŽ�����ζ��������ж����ڴ�.
	* @author	������,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static int polygonIdentityPolygon(
		const GTGeometry			*inputGeometry,
		const GTGeometry			*identityGeometry,
		std::vector<GTGeometry*>	&result_geom_vec,
		std::vector<int>			&result_mark_vec,
		const double				 dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	2�������ͼ��Identity����,Ч��ͬarcgis 2�������ͼ���Identity����.
	* @param	[in]
	*			inputFeatureLayer			��������ͼ��.
	*			identityFeatureLayer		Identity����ͼ��.
	*			resultLayer					���ͼ��,��������ǰ�����½�,���ֶθ���Ϊ0,����Ϊ�ɸ���.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; ����:ERROR,����-1	
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	*			[out]
	* @return	���ؽ�����ζ������.����ֵΪ-1˵�����ִ���,����-2˵��2������ͼ��ռ�ο���һ��,����-3˵����ʼ״̬���ͼ���ֶβ�Ϊ0.
	* @note		�㷨����:�ȶ�ÿ��ͼ����Union,����ͼ������,����ͼ�����,�õ����.
	* @author	������,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static int identityPolygonLayers(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *identityFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int              joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	2�������Featureclass��Identity����,Ч��ͬarcgis��Identity����.2��featureclass�ڲ�feature�����һ�µ����Խṹ
	* @param	[in]
	*			inputFeatureClassName		����featureclass����,һ���Ǹ�featureclass��feature����ͼ������
	*			inputFeatureClass			����featureclass
	*			identityFeatureClassName	identity featureclass����,һ���Ǹ�featureclass��feature����ͼ������
	*			identityFeatureClass		identity featureclass
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; ����:ERROR,����-1
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	*			[out]
	* @return	���ؽ��featureclass.����ֵΪNULL˵�����ִ���.
	* @note		�㷨����:�ȶ�ÿ��featureclass�ڲ���Union,����featureclass�����,�����,�õ����.
	* @author	������,fanjf@lreis.ac.cn,2011-8-11
	*******************************************************************************/
	static GTFeatureClass* polygonsIdentityPolygons(
		const char			  *inputFeatureClassName,
		GTFeatureClass        *inputFeatureClass,
		const char			  *identityFeatureClassName,
		GTFeatureClass        *identityFeatureClass,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);


	/*Intersect, ��*/
	/*�����*/
	/*������*/
	/*���뻷*/
	/*****************************************************************************
	* @brief	polygon��multipolygon����֮����󽻲���,�����µ�polygon��multipolygon
	* @param	[in]
	*			inputGeometry			�������ļ��ζ���
	*			intersectGeometry		�󽻼��ζ���
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @return	����Intersect������ζ���,����ΪNULL����Ҫ�ⲿ�������ͷ���ָ��.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static GTGeometry* polygonIntersectPolygon(
		const GTGeometry	*inputGeometry,
		const GTGeometry	*intersectGeometry,
		const double		 dTolerance=1E-10);

	/*****************************************************************************
	* @brief	polygon��multipolygonͼ�����󽻲���,�����µ�multipolygonͼ��.
	*			ֱ�ӵ���polygonIntersectPolygon�������м���.���ֻ�����ϲ���Ĺ�������.
	*			���ÿ��������Ķ���κϲ����̲��ö�̬����.
	* @param	[in]
	*			inputFeatureLayer		Ҫ�����󽻲����Ķ����ͼ��
	*			resultLayer				���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	*			[out]
	* @return	���ؽ������feature��.����-1��ʾ���ִ���,����-3˵�����ͼ���ֶθ�����Ϊ0.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-2
	*******************************************************************************/
	static int intersectPolygonsInSameLayer(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon��multipolygonͼ�����󽻲���,�����µ�multipolygonͼ��.
	*			ֱ�ӵ���polygonIntersectPolygon�������м���.���ֻ�����ϲ���Ĺ�������.
	*			���ÿ��������Ķ���κϲ����̲��ö�̬����.(mp��ʾ������˰汾,Ŀǰ��������
	*			�󽻲���Ӧ�ö�˼���,�����󽻺�ĺϲ�����û��Ӧ�ж�˼���)
	* @param	[in]
	*			inputFeatureLayer		Ҫ�����󽻲����Ķ����ͼ��
	*			resultLayer				���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	*			[out]
	* @return	���ؽ������feature��.����-1��ʾ���ִ���,����-3˵�����ͼ���ֶθ�����Ϊ0.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-2
	*******************************************************************************/
	static int intersectPolygonsInSameLayer_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon��multipolygonͼ�����󽻲���,�����µ�multipolygonͼ��.
	*			ֱ�ӵ���polygonIntersectPolygon�������м���.�����������θ������ཻ�����
	*			������ͬ.�������ΰ������Ա������Էֱ���и��Ը�����ε�����.(�����ArcGIS
	*			Overlap Intersect������ͬ)
	* @param	[in]
	*			inputFeatureLayer			Ҫ�����󽻲����Ķ����ͼ��
	*			resultLayer					���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; ����:ERROR,����-1
	*			dTolerance					��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10.
	*			[out]
	* @return	���ؽ������feature��.����-1��ʾ���ִ���,����-3˵�����ͼ���ں������ó�ʼ״̬�ֶ�����Ϊ0.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-2
	*******************************************************************************/
	static int intersectPolygonsInSameLayer2(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon��multipolygonͼ���󽻲���,�����µ�multipolygonͼ��.
	*			ֱ�ӵ���polygonIntersectPolygon�������м���.Ч��ͬarcgis2��ͼ����.
	* @param	[in]
	*			inputFeatureLayer			Ҫ�����󽻲����Ķ����ͼ��A
	*			intersectFeatureLayer		Ҫ�����󽻲����Ķ����ͼ��B
	*			resultLayer					���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; ����:����-1;
	*			dTolerance					��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10.
	*			[out]
	* @return	���ؽ������feature��.����ֵΪ-1˵�����ִ���,����ֵΪ-2˵������ͼ��Ŀռ�ο���һ��.
	*			����-3˵�����ͼ���ں������ó�ʼ״̬�ֶ�����Ϊ0.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-3
	*******************************************************************************/
	static int intersectPolygonLayers(
		GTFeatureLayer	      *inputFeatureLayer,
		GTFeatureLayer        *intersectFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	static int intersectPolygons(
		const char 	          *inputLayerName,
		GTFeatureClass	      *inputFeatureClass,
		const char 	          *intsectLayerName,
		GTFeatureClass	      *intsectFeatureClass,
		GTFeatureClass        *resultFeatureClass,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon��multipolygonͼ���󽻲���,�����µ�multipolygonͼ��.
	*			ֱ�ӵ���polygonIntersectPolygon�������м���. mp��ʾ������˰汾.
	*			Ч��ͬarcgis2��ͼ����.
	* @param	[in]
	*			inputFeatureLayer			Ҫ�����󽻲����Ķ����ͼ��A
	*			intersectFeatureLayer		Ҫ�����󽻲����Ķ����ͼ��B
	*			resultLayer					���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; ����:����-1;
	*			dTolerance					��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10.
	*			[out]
	* @return	���ؽ������feature��.����ֵΪ-1˵�����ִ���,����ֵΪ-2˵������ͼ��Ŀռ�ο���һ��.
	*			����-3˵�����ͼ���ں������ó�ʼ״̬�ֶ�����Ϊ0.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-3
	*******************************************************************************/
	static int intersectPolygonLayers_mp(
		GTFeatureLayer	      *inputFeatureLayer,
		GTFeatureLayer        *intersectFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	���в�ͬ���Խṹ��2�������featureclass��.2��featureclass��������2��featurelayer.
	*			2��char*������Ϊ�˺ϲ��ֶ����Զ������ֶ�������,������ʹ��ԭ��featurelayer����.
	*			ֱ�ӵ���polygonIntersectPolygon�������м���.Ч��ͬarcgis2��ͼ����.
	* @param	[in]
	*			inputFeatureClassName		����featureclass����
	*			inputFeatureClass			����featureclass
	*			intersectFeatureClassName	intersect����featureclass����
	*			intersectFeatureClass		intersect����featureclass
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; ����:����-1;
	*			[out]
	* @return	���ؽ��featureclass.����ֵΪNULL˵�����ִ������2��featureclass��feature�ռ�ο���һ��.
	* @note		����������ע�����ٷ���featureclass��fieldsDefPtr.
	* @author	������,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static GTFeatureClass* polygonsIntersectPolygons(
		const char			  *inputFeatureClassName,
		GTFeatureClass        *inputFeatureClass,
		const char			  *intersectFeatureClassName,
		GTFeatureClass        *intersectFeatureClass,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);



	/*Symmetrical Difference, ���*/
	/*�����*/
	/*������*/
	/*���뻷*/
	/*****************************************************************************
	* @brief	2�������������,����0��2���µļ��ζ���洢��vector��.
	*			Ч��ͬarcgis 2������������.
	* @param	[in]
	*			inputGeometry			���뼸�ζ���A
	*			symmdiffGeometry		���뼸�ζ���B
	*			result_geom_vec			������ζ�������
	*			result_mark_vec			������ζ���������
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	*			[out]
	* @return	���ؽ�����ζ������.����ֵΪ-1˵�����ִ���,0˵��2��ͼ����ȫ�ص�,1˵��ĳһͼ����ȫ������.
	* @note		�����������ⲿ�ͷŽ�����ζ��������ж����ڴ�.
	* @author	������,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static int polygonSymmetricalDifferencePolygon(
		const GTGeometry			*inputGeometry,
		const GTGeometry			*symmdiffGeometry,
		std::vector<GTGeometry*>	&result_geom_vec,
		std::vector<int>			&result_mark_vec,
		const double				 dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon��multipolygonͼ��������,�����µ�multipolygonͼ��.
	*			Ч��ͬarcgis2��ͼ�������.
	* @param	[in]
	*			inputFeatureLayer		Ҫ�����󽻲����Ķ����ͼ��A����
	*			symmdiffFeatureLayer	Ҫ�����󽻲����Ķ����ͼ��B����
	*			resultLayer				���ͼ������
	*			joinAttributes			0:NO_FID; 1:ONLY_FID; 2:ALL; ����:����-1;
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	*			[out]
	* @return	���ؽ������feature��.����ֵΪ-1˵�����ִ���,����ֵΪ-2˵������ͼ��Ŀռ�ο���һ��.
	*			����-3˵�����ͼ���ں������ó�ʼ״̬�ֶ�����Ϊ0.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static int symmetricalDifferencePolygonLayers(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *symmdiffFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int              joinAttributes = 2,
		const double           dTolerance = 1E-10);



	/*Union, ���*/
	/*�����*/
	/*������*/
	/*���뻷*/
	/*****************************************************************************
	* @brief	2������κϲ�Ϊһ�������
	* @param	[in]
	*			inputGeometry			�ϲ��ļ��ζ���A
	*			unionGeometry			�ϲ��ļ��ζ���B
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	����Union������ζ���,����ΪNULL����Ҫ�ⲿ�������ͷ���ָ��.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-7-26
	*******************************************************************************/
	static GTGeometry* polygonUnionPolygonToOne(
		const GTGeometry *inputGeometry,
		const GTGeometry *unionGeometry,
		const double      dTolerance=1E-10);

	/*****************************************************************************
	* @brief	�������κϲ���һ�������(�����ϲ�).
	* @param	[in]
	*			inputFeatureClass		Ҫ�ϲ���Feature����.������������ֵ,��Ҫ�����߹������ͷ�.
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	��������Union���Feature,����ΪNULL����Ҫ�ⲿ�������ͷ���ָ��.
	* @note		���feature�������κ�ԭʼfeature������.
	* @author	������,fanjf@lreis.ac.cn,2011-8-3
	*******************************************************************************/
	static GTFeature* unionPolygonsToOne(
		GTFeatureClass  *inputFeatureClass,
		const double     dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	�������κϲ���һ�������(�����ϲ�).
	* @param	[in]
	*			inputGeometries		Ҫ�ϲ��ļ��ζ��󼯺�.������������ֵ,��Ҫ�����߹������ͷ�.
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	��������Union������ζ���,����ΪNULL����Ҫ�ⲿ�������ͷ���ָ��.
	* @note		�����Ҫ�ⲿ�ͷ�
	* @author	������,fanjf@lreis.ac.cn,2011-8-12
	*******************************************************************************/
	static GTGeometry* unionPolygonsToOne(
		const std::vector<GTGeometry*>  inputGeometries,
		const double                    dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	�������κϲ���һ�������(�����ϲ�).
	* @param	[in]
	*			inputGeometries		    Ҫ�ϲ��ļ��ζ��󼯺�.������������ֵ,��Ҫ�����߹������ͷ�.
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	��������Union������ζ���,����ΪNULL����Ҫ�ⲿ�������ͷ���ָ��.
	* @note		�����Ҫ�ⲿ�ͷ�
	* @author	������,fanjf@lreis.ac.cn,2011-8-12
	*******************************************************************************/
	static GTGeometry* unionPolygonsToOne(
		const std::vector<GTFeature*>   inputFeatures,
		const double                    dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	ͬһͼ���ڶ������(��ͼ������״�����ߺϲ�).
	* @param	[in]
	*			inputFeatureLayer		��������ͼ��
	*			resultLayer				���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	���ؽ��ͼ����Feature����,��Ϊ-1˵����������.����-3˵�����ͼ���ֶ�������0.
	* @note		���feature�������κ�ԭʼfeature������.
	* @author	������,fanjf@lreis.ac.cn,2011-8-3
	*******************************************************************************/
	static int unionPolygonsInSameLayer(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	ͬһͼ���ڶ������(���ϲ�,ͬarcgis union����).
	* @param	[in]
	*			inputFeatureLayer		��������ͼ��
	*			resultLayer				���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			joinAttributes			0:NO_FID; 1:ONLY_FID; 2:ALL; ����:ERROR,����-1
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	���ؽ��ͼ����Feature����,��Ϊ-1˵����������.����-3˵�����ͼ���ֶ�������0.
	* @note		���feature�������Խ�ϲ��Ա���ԭʼ���������.
	* @author	������,fanjf@lreis.ac.cn,2011-8-4
	*******************************************************************************/
	static int unionPolygonsInSameLayer2(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	�������(ͬһͼ���ڻ���2��ͼ����ཻͼ�α��ϲ�).
	* @param	[in]
	*			inputFeatureLayer		Ҫ����ͼ��union�Ķ����ͼ��A
	*			unionFeatureLayer		Ҫ����ͼ��union�Ķ����ͼ��B
	*			resultLayer				���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	���ؽ��ͼ����Feature����,��Ϊ-1˵����������.����-3˵�����ͼ���ֶ�������0.
	* @note		���feature������ԭʼ������κ�����.
	* @author	������,fanjf@lreis.ac.cn,2011-8-4
	*******************************************************************************/
	static int unionPolygonLayers(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer   	  *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	�������(���ϲ�,ͬarcgis ��ͼ�� union����).
	* @param	[in]
	*			inputFeatureLayer		Ҫ����ͼ��union�Ķ����ͼ��A
	*			unionFeatureLayer		Ҫ����ͼ��union�Ķ����ͼ��B
	*			resultLayer				���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			joinAttributes			0:NO_FID; 1:ONLY_FID; 2:ALL; ����:ERROR,����-1
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	���ؽ��ͼ����Feature����,��Ϊ-1˵����������.����-2˵��2��ͼ��Ŀռ�ο���һ��.����-3˵�����ͼ���ֶ�������0.
	* @note		���feature�������Խ�ϲ��Ա���ԭʼ���������.
	* @author	������,fanjf@lreis.ac.cn,2011-8-4
	*******************************************************************************/
	static int unionPolygonLayers2(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer   	  *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	һ��������(���ϲ�,ͬarcgis union����).���feature�������Խ�ϲ��Ա���ԭʼ���������.
	* @param	[in]
	*			fcName						feature class����(����ͼ������)
	*			inputFeatureClass			�����󲢲�����һ��feature,�ⲿ������ָ��
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; ����:ERROR,����-1
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	�����󲢽������NULL.
	* @note		���ص�featureclass����Ϊ��,��Ҫ�ⲿ�ͷ�feature���ֶζ���(fieldsDefPtr).
	* @author	������,fanjf@lreis.ac.cn,2011-8-9
	*******************************************************************************/
	static GTFeatureClass* unionPolygons(
		const char            *fcName,
		GTFeatureClass        *inputFeatureClass,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	�������(��ͼ���й������ߺϲ�).
	* @param	[in]
	*			inputFeatures			��������
	*			resultGeometries		������ζ��󼯺�
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	���ؽ������θ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-3
	*******************************************************************************/
	static int unionPolygons(
		std::vector<GTFeature*>	  inputFeatures,
		std::vector<GTGeometry*>  &resultGeometries,
		const double			  dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	�������(��ͼ���й������ߺϲ�).
	* @param	[in]
	*			inputFeatures			��������
	*			resultGeometries		������ζ��󼯺�
	*			dTolerance				��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	���ؽ������θ���.
	* @note		
	* @author	������,fanjf@lreis.ac.cn,2011-8-3
	*******************************************************************************/
	static int unionPolygonsWhoINSCT(
		std::vector<GTFeature*>	  inputFeatures,
		std::vector<GTGeometry*>  &resultGeometries,
		const double			  dTolerance = 1e-10);


	/*Difference, ���*/
	/*�����*/
	/*������*/
	/*���뻷*/


	/*Update, ����*/
	/*�����뻷*/
	/*****************************************************************************
	* @brief	����θ��¶����(ͬarcgis update����).
	* @param	[in]
	*			inputGeometry			�������εļ��ζ���
	*			updateGeometry			���¶���εļ��ζ���
	*			result_geom_vec			������ζ�������
	*			result_mark_vec			������ζ���������,0:����ǰ��  1:���Ժ���   2:����ǰ�ߺͺ��ߵĺϲ�
	*			bKeepBorders			�Ƿ���ཻ�Ľ�����ζ�����кϲ�����
	*			dTolerance				�ݲ�,���С�ڴ�ֵ��ring������.
	* @param	[out]
	* @return	���ؽ�����ζ���ĸ���
	* @note		���롢���¼�������ζ�����躯�����������ⲿ�ͷ���ָ��.
	* @author	������,fanjf@lreis.ac.cn,2011-8-12
	*******************************************************************************/
	static int polygonUpdatePolygon(
		const GTGeometry			*inputGeometry,
		const GTGeometry			*updateGeometry,
		std::vector<GTGeometry*>	&result_geom_vec,
		std::vector<int>			&result_mark_vec,
		const bool					 bKeepBorders = true,
		const double				 dTolerance = 1E-10);


	/*****************************************************************************
	* @brief	�����ͼ�����(update)�����ͼ��(��arcgis����).
	* @param	[in]
	*			inputFeatureLayer			������(update)�Ķ����ͼ��
	*			updateFeatureLayer			Ҫ����(update)���߶����ͼ��
	*			resultLayer					���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			bKeepBorders				�Ƿ���updateͼ�ε���߽�(true:Ĭ��,update����ͼ
	*										���ͼ�β����ϲ�;false:update���߲��ڵ�ͼ�α��ϲ�)
	*			dTolerance					��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	���ؽ��ͼ����Feature����,��Ϊ-1˵����������.����-2˵��2��ͼ��Ŀռ�ο���һ��.��Ϊ-3˵�����ͼ���ʼ״̬�ֶ�����Ϊ0.
	* @note		��ָ���������߽�,arcgis������updateͼ����ͼ�εĽ�������,��������updateͼ���������ཻ�ߺϲ�.
	* @author	������,fanjf@lreis.ac.cn,2011-8-12
	*******************************************************************************/
	static int updatePolygonLayer(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *updateFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   bKeepBorders = true,
		const double           dTolerance = 1E-10);




	/*Spatial Join, ���ڿռ��ϵ����������*/
	/*���뻷*/
	/*****************************************************************************
	* @brief	���ڿռ��ϵ�Ķ����ͼ����������(ͬarcgis spatial join����).
	* @param	[in]
	*			inputFeatureLayer			Ŀ������ͼ��
	*			sjoinFeatureLayer			spatial join���߶����ͼ��
	*			resultLayer					���ͼ��,Ҫ��ǿ�,�ɸ���,�������ֶθ���Ϊ0.
	*			bJoinOneToOne				���������ݲ��ж�����빤�߲��ж�����������join����ʱ�ı�������.true:������һ������; false:�����������.
	*			bKeepAllInputFeatures		��Ŀ������ͼ���ж���β��빤�߲����κ�һ�����������join����ʱ��ͼ�α�������.true:ȫ�����; false:����
	*										������������.
	*			iMatchOption				����spatial join����������. 0:�ཻ(INTSECTS,Ŀ��ͼ���빤��ͼ���ཻ); 
	*										1:����(CONTIANS,Ŀ��ͼ����ȫ��������ͼ��); 2:������(WITH_IN,Ŀ��ͼ�ΰ����ڹ���ͼ��); 3:�����ƥ��(CLOSEST,
	*										�ҵ������ͼ�ν���ƥ��,�ݲ�֧��)
	*			dTolerance					��������ring����С���,С�ڴ�ֵ��ring������.Ĭ��Ϊ1E-10
	* @param	[out]
	* @return	���ؽ��ͼ����Feature����,��Ϊ-1˵����������.����-2˵��2��ͼ��Ŀռ�ο���һ��,����-3˵�����ͼ���ʼ״̬�ֶθ�����Ϊ0,����-4˵��spatial join
	*			�ռ��ϵƥ��ģʽ����֧��.
	* @note		Ŀǰ�в�֧��fields map.
	* @author	������,fanjf@lreis.ac.cn,2011-8-13
	*******************************************************************************/
	static int spatialJoinPolygonLayers(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *sjoinFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   bJoinOneToOne = true,
		const bool             bKeepAllInputFeatures = true,
		const int              iMatchOption = 0,
		const double           dTolerance = 1E-10);




};

}
#endif

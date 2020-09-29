/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  几何变换
* Author:   范俊甫, fanjf@lreis.ac.cn,2011-8-18
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c)  保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYTRANSFORM_H_INCLUDED
#define GTSGEOMETRYTRANSFORM_H_INCLUDED

#include "gts_cg_port.h"

namespace gts{

	class GTS_DLL GTGeometryTransform
	{
	public:
		GTGeometryTransform(void);
		~GTGeometryTransform(void);

		/*点围绕某点的旋转, dAngle为degree*/
		static bool pointRotate(
			GTRawPoint* pt, 
			GTRawPoint* ptOrg, 
			double      dAngle);

		/*点的scale*/
		static bool pointScale(
			GTRawPoint* pt,
			double      dScaleX,
			double      dScaleY);

		/*点的move*/
		/*****************************************************************************
		* @brief	点的move
		* @param	[in]
		*			pt					输入要移动的点
		*			dMoveX				向x轴移动的距离
		*			dMoveY				向y轴移动的距离		
		*			[out]
		*			pt					移动后的点
		* @return	是否成功
		* @note		
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-18
		*******************************************************************************/
		static bool pointMove(
			GTRawPoint* pt,
			double      dMoveX = 0.0,
			double      dMoveY = 0.0);

		/*****************************************************************************
		* @brief	点围绕某点的旋转, dAngle为degree
		* @param	[in]
		*			pt					输入要旋转的点
		*			ptOrg				旋转围绕的点
		*			dAngle				旋转的角度	
		*			[out]
		*			pt					旋转后的点
		* @return	是否成功
		* @note		
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-18
		*******************************************************************************/
		static bool pointRotate1(
			GTRawPoint* pt, 
			GTRawPoint* ptOrg, 
			double      dAngle = 0.0);

		/*****************************************************************************
		* @brief	点按照某点的缩放
		* @param	[in]
		*			pt					输入要缩放的点
		*			ptOrg				缩放参照的点
		*			dScaleX				x坐标放大倍数
		*			dScaleY				y坐标放大倍数
		*			[out]
		*			pt					缩放后的点
		* @return	是否成功
		* @note		
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-18
		*******************************************************************************/
		static bool pointScale1(
			GTRawPoint* pt, 
			GTRawPoint* ptOrg, 
			double      dScaleX = 1.0,
			double      dScaleY = 1.0);

		/*****************************************************************************
		* @brief	点按照某点的反射
		* @param	[in]
		*			pt					输入要反射的点
		*			ptOrg				反射参照的点
		*			[out]
		*			pt					反射后的点
		* @return	是否成功
		* @note		
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-18
		*******************************************************************************/
		static bool pointReflect(
			GTRawPoint* pt, 
			GTRawPoint* ptOrg);

		/*****************************************************************************
		* @brief	点按照某点的Shear
		* @param	[in]
		*			pt					输入要Shear的点
		*			ptOrg				Shear参照的点
		*			dM					Shear的参数
		*			bHori				水平或者垂直Shear
		*			[out]
		*			pt					Shear后的点
		* @return	是否成功
		* @note		
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-18
		*******************************************************************************/
		static bool pointShear(
			GTRawPoint* pt, 
			double		dM = 1.0,
			bool		bHori = true);


		/*点的仿射变换*/
		/*static bool pointAffineTransfrom(
		GTRawPoint* pt,
		double    * dAffineMatrix);*/

		static bool pointAffineTransform(
			GTRawPoint*		pPoint, 
			const			GTGCP* gcp_list,
			const int		gcp_count);

		static bool pointAffineTransfromByMatrix(
			GTRawPoint* pt,
			double    * dAffineMatrix);

		/*6参数的仿射变换矩阵的逆矩阵*/
		static bool inverseTransformMatrix(
			double *dOrgMatrix, 
			double *dInvMatrix);
		/*****************************************************************************
		* @brief	根据所选点集以及对应点集，计算仿射变换矩阵
		* @param	[in]
		*			gcp_list					输入的点集
		*			gcp_count					点集中点的数量
		*			[out]
		*			dAffineMatrix				仿射变换矩阵
		* @return	是否成功
		* @note		
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-18
		*******************************************************************************/
		static bool affineTransformFromGCP(
			const		 GTGCP* gcp_list,
			const int    gcp_count,
			double*      dAffineMatrix);

		static bool multiPointAffineTransform(
			GTMultiPoint*	pMPoint, 
			const			GTGCP* gcp_list,
			const int		gcp_count);

		static bool multiPointAffineTransformByMatrix(
			GTMultiPoint*	pMPoint, 
			double*			dAffineMatrix);

		static bool multiPointMove(
			GTMultiPoint*	pMPoint, 
			double			dMoveX = 0.0,
			double			dMoveY = 0.0);

		static bool multiPointRotate(
			GTMultiPoint*	pMPoint, 
			GTRawPoint*		ptOrg, 
			double			dAngle = 0.0);

		static bool multiPointScale(
			GTMultiPoint*	pMPoint, 
			GTRawPoint*		ptOrg, 
			double			dScaleX = 1.0,
			double			dScaleY = 1.0);

		static bool multiPointReflect(
			GTMultiPoint*	pMPoint, 
			GTRawPoint*		ptOrg);

		static bool multiPointShear(
			GTMultiPoint*	pMPoint, 
			double			dM = 1.0,
			bool			bHori = true);

		static bool ringAffineTransform(
			GTRing*		pRing, 
			const		GTGCP* gcp_list,
			const int	gcp_count);

		static bool ringAffineTransformByMatrix(
			GTRing*		pRing, 
			double*		dAffineMatrix);

		static bool ringMove(
			GTRing*			pRing, 
			double			dMoveX = 0.0,
			double			dMoveY = 0.0);

		static bool ringRotate(
			GTRing*			pRing, 
			GTRawPoint*		ptOrg, 
			double			dAngle = 0.0);

		static bool ringScale(
			GTRing*			pRing,  
			GTRawPoint*		ptOrg, 
			double			dScaleX = 1.0,
			double			dScaleY = 1.0);

		static bool ringReflect(
			GTRing*			pRing, 
			GTRawPoint*		ptOrg);

		static bool ringShear(
			GTRing*			pRing, 
			double			dM = 1.0,
			bool			bHori = true);

		static bool lineStringAffineTransform(
			GTLineString*	pLineStr, 
			const			GTGCP* gcp_list,
			const int		gcp_count);

		static bool lineStringAffineTransformByMatrix(
			GTLineString*	pLineStr, 
			double*			dAffineMatrix);

		static bool lineStringMove(
			GTLineString*	pLineStr, 
			double			dMoveX = 0.0,
			double			dMoveY = 0.0);

		static bool lineStringRotate(
			GTLineString*	pLineStr, 
			GTRawPoint*		ptOrg, 
			double			dAngle = 0.0);

		static bool lineStringScale(
			GTLineString*	pLineStr, 
			GTRawPoint*		ptOrg, 
			double			dScaleX = 1.0,
			double			dScaleY = 1.0);

		static bool lineStringReflect(
			GTLineString*	pLineStr, 
			GTRawPoint*		ptOrg);

		static bool lineStringShear(
			GTLineString*	pLineStr, 
			double			dM = 1.0,
			bool			bHori = true);

		static bool multiLineStringAffineTransform(
			GTMultiLineString*	pMLineStr, 
			const				GTGCP* gcp_list,
			const int			gcp_count);

		static bool multiLineStringAffineTransformByMatrix(
			GTMultiLineString*	pMLineStr, 
			double*				dAffineMatrix);

		static bool multiLineStringMove(
			GTMultiLineString*	pMLineStr, 
			double				dMoveX = 0.0,
			double				dMoveY = 0.0);

		static bool multiLineStringRotate(
			GTMultiLineString*	pMLineStr, 
			GTRawPoint*			ptOrg, 
			double				dAngle = 0.0);

		static bool multiLineStringScale(
			GTMultiLineString*	pMLineStr, 
			GTRawPoint*			ptOrg, 
			double				dScaleX = 1.0,
			double				dScaleY = 1.0);

		static bool multiLineStringReflect(
			GTMultiLineString*	pMLineStr, 
			GTRawPoint*			ptOrg);

		static bool multiLineStringShear(
			GTMultiLineString*	pMLineStr, 
			double				dM = 1.0,
			bool				bHori = true);

		static bool polygonAffineTransform(
			GTPolygon*		pPolygon, 
			const			GTGCP* gcp_list,
			const int		gcp_count);

		static bool polygonAffineTransformByMatrix(
			GTPolygon*		pPolygon, 
			double*			dAffineMatrix);

		static bool polygonMove(
			GTPolygon*		pPolygon, 
			double			dMoveX = 0.0,
			double			dMoveY = 0.0);

		static bool polygonRotate(
			GTPolygon*		pPolygon, 
			GTRawPoint*		ptOrg, 
			double			dAngle = 0.0);

		static bool polygonScale(
			GTPolygon*		pPolygon, 
			GTRawPoint*		ptOrg, 
			double			dScaleX = 1.0,
			double			dScaleY = 1.0);

		static bool polygonReflect(
			GTPolygon*		pPolygon, 
			GTRawPoint*		ptOrg);

		static bool polygonShear(
			GTPolygon*		pPolygon, 
			double			dM = 1.0,
			bool			bHori = true);

		static bool multiPolygonAffineTransform(
			GTMultiPolygon*		pMPolygon, 
			const				GTGCP* gcp_list,
			const int			gcp_count);

		static bool multiPolygonAffineTransformByMatrix(
			GTMultiPolygon*		pMPolygon, 
			double*				dAffineMatrix);

		static bool multiPolygonMove(
			GTMultiPolygon*		pMPolygon, 
			double				dMoveX = 0.0,
			double				dMoveY = 0.0);

		static bool multiPolygonRotate(
			GTMultiPolygon*		pMPolygon, 
			GTRawPoint*			ptOrg, 
			double				dAngle = 0.0);

		static bool multiPolygonScale(
			GTMultiPolygon*		pMPolygon, 
			GTRawPoint*			ptOrg, 
			double				dScaleX = 1.0,
			double				dScaleY = 1.0);

		static bool multiPolygonReflect(
			GTMultiPolygon*		pMPolygon, 
			GTRawPoint*			ptOrg);

		static bool multiPolygonShear(
			GTMultiPolygon*		pMPolygon, 
			double				dM = 1.0,
			bool				bHori = true);

		static bool geometryAffineTransform(
			GTGeometry*		pGeo, 
			const			GTGCP* gcp_list,
			const int		gcp_count);

		static bool geometryAffineTransformByMatrix(
			GTGeometry*		pGeo, 
			double*			dAffineMatrix);

		static bool geometryRotate(
			GTGeometry*		pGeo, 
			GTRawPoint*		ptOrg, 
			double			dAngle = 0.0);

		static bool geometryMove(
			GTGeometry*		pGeo, 
			double			dMoveX = 0.0,
			double			dMoveY = 0.0);

		static bool geometryScale(
			GTGeometry*		pGeo, 
			GTRawPoint*		ptOrg, 
			double			dScaleX = 1.0,
			double			dScaleY = 1.0);

		static bool geometryReflect(
			GTGeometry*		pGeo, 
			GTRawPoint*		ptOrg);

		static bool geometryShear(
			GTGeometry*		pGeo, 
			double			dM = 1.0,
			bool			bHori = true);

		static bool featureClassAffineTransform(
			GTFeatureClass&			srcClss,
			GTFeatureClass&			desClss,
			const					GTGCP* gcp_list,
			const int				gcp_count);

		static bool featureClassAffineTransformByMatrix(
			GTFeatureClass&			srcClss,
			GTFeatureClass&			desClss,
			double*					dAffineMatrix);

		static bool featureClassRotate(
			GTFeatureClass&			srcClss,
			GTFeatureClass&			desClss,
			GTRawPoint*				ptOrg, 
			double					dAngle = 0.0);

		static bool featureClassMove(
			GTFeatureClass&			srcClss,
			GTFeatureClass&			desClss,
			double					dMoveX = 0.0,
			double					dMoveY = 0.0);

		static bool featureClassScale(
			GTFeatureClass&			srcClss,
			GTFeatureClass&			desClss,
			GTRawPoint*				ptOrg, 
			double					dScaleX = 1.0,
			double					dScaleY = 1.0);

		static bool featureClassReflect(
			GTFeatureClass&			srcClss,
			GTFeatureClass&			desClss,
			GTRawPoint*				ptOrg);

		static bool featureClassShear(
			GTFeatureClass&			srcClss,
			GTFeatureClass&			desClss,
			double					dM = 1.0,
			bool					bHori = true);
	};

}
#endif
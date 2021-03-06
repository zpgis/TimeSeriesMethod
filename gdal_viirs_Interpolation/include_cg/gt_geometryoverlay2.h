/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  Geometry Overlay
* Author:   范俊甫,fanjf@lreis.ac.cn,2011-10-9
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c)  保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYOVERLAY2_H_INCLUDED
#define GTSGEOMETRYOVERLAY2_H_INCLUDED

#include "gts_cg_port.h"
#include "gt_geometryanalysis.h"
#include "gt_geometryconvert.h"

namespace gts{

class GTS_DLL GTGeometryOverlay2
{
private:
	/*****************************************************************************
	* @brief	2个具有相同属性结构的feature进行属性赋值
	* @param	[in]
	*			oringnal_feature		属性数据来源fezature
	*			target_feature			被赋值的几何对象
	* @param	[out]
	* @return	所有字段只要有一个进行了赋值操作即返回true.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-19
	*******************************************************************************/
	static bool feature_attributes_clone(
		GTFeature *oringnal_feature,
		GTFeature &target_feature);
	
	/*****************************************************************************
	* @brief	Get the exterior ring count of the geometry.
	* @param	[in]
	*			polygon		polygon or multi-polygon geometry.
	* @param	[out]
	* @return	exterior ring count of the geometry.
	* @note		
	* @author	Fan Junfu,fanjf@lreis.ac.cn,2012-5-28
	*******************************************************************************/
	static int getExteriorRingCount(const GTGeometry* polygon);


	/*****************************************************************************
	* @brief	在不同的数据源中读取feature, 获得feature class.该函数将自动判断
	*			feature layer的DataDriverType,当为OGR数据源时，where子句将不起作用,
	*			当为非OGR数据源时,使用where子句查询.
	* @param	[in]
	*			fLayer				feature layer.
	*			cWhere				where子句,如"fid>=100 and fid <=200".
	*			iStartFID			若bWhere=false,读取feature的起始fid(结果feature class将包含该feature).
	*			iEndFID				若bWhere=false,读取feature的终止fid(结果feature class将包含该feature).
	*			fClass				接收结果的feature class.
	* @param	[out]
	* @return	结果feature class中feature个数.
	* @note		cWhere子句中fid从1开始,iStartFID和iEndFID则从0开始计数.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-06-18
	*******************************************************************************/
	static int getFeaturesByFIDorWHERE(
		GTFeatureLayer*			fLayer,
		const char*				cWhere,
		const int				iStartFID,
		const int				iEndFID,
		GTFeatureClass& 		fClass);

	/*****************************************************************************
	* @brief	统计几何对象包含的点数.
	* @param	[in]
	*			geom				几何对象.
	* @param	[out]
	* @return	点数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-12-31
	*******************************************************************************/
	static int getGeometryPointCount(const GTGeometry* geom);


public:
	GTGeometryOverlay2(void);
	~GTGeometryOverlay2(void);
	/*Erase, 从一个对象或图层删除某部分*/
	/*点与环*/
	/*线与环*/
	/*环与环*/
	/*****************************************************************************
	* @brief	polygon或multipolygon对象之间的擦除操作,生成新的polygon或multipolygon
	* @param	[in]
	*			inputGeometry			被擦除的几何对象
	*			eraseGeometry			擦除几何对象
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回erase结果几何对象,若不为NULL则需要外部调用者释放其指针.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static GTGeometry* polygonErasePolygon(
		const GTGeometry *inputGeometry,
		const GTGeometry *eraseGeometry,
		const double      dTolerance=1E-10);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层擦除操作,生成新的multipolygon图层.
	*			直接调用polygonErasePolygon函数进行计算.
	* @param	[in]
	*			inputFeatureLayer		被擦除的多边形图层.
	*			eraseFeatureLayer		要擦除其他图层的多边形图层.
	*			resultLayer				结果图层,函数调用前必须新建,且字段数为0,必须为可更新.
	*			chkEraseTopology        是否检查并消除erase layer的拓扑错误对结果造成的影响,可能较慢,默认值为false.
	*			dTolerance				容差值,面积小于此值的GTRing在构造结果几何对象时被舍弃.
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误,返回值为-2说明操作图层的空间参考不一致,返回-3说明结果图层初始状态字段个数不为0.
	* @note		已测, 无内存泄漏.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-11
	*******************************************************************************/
	static int erasePolygonLayer(
		GTFeatureLayer		*inputFeatureLayer,
		GTFeatureLayer		*eraseFeatureLayer,
		GTFeatureLayer      *resultLayer,
		const bool			 chkEraseTopology = false,
		const double         dTolerance = 1E-10,
		const int			blockSize = 200000);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层擦除操作,生成新的multipolygon图层.
	*			直接调用polygonErasePolygon函数进行计算.(单机多核版本)
	/*先分块再开OpenMP，瞬时内存占用为(blockSize*featureSize)
	* @param	[in]
	*			inputFeatureLayer		被擦除的多边形图层.
	*			eraseFeatureLayer		要擦除其他图层的多边形图层.
	*			resultLayer				结果图层,函数调用前必须新建,且字段个数为0,必须为可更新.
	*			chkEraseTopology        是否检查并消除erase layer的拓扑错误对结果造成的影响,可能较慢,默认值为false.
	*			dTolerance				容差值,面积小于此值的GTRing在构造结果几何对象时被舍弃.
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误,返回值为-2说明操作图层的空间参考不一致,返回-3说明结果图层初始状态字段个数不为0.
	* @note		已测, 无内存泄漏.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-11
	*******************************************************************************/
	static int erasePolygonLayer_mp(
		GTFeatureLayer		*inputFeatureLayer,
		GTFeatureLayer		*eraseFeatureLayer,
		GTFeatureLayer      *resultLayer,
		const bool			 chkEraseTopology = false,
		const double         dTolerance = 1E-10,
		const int			blockSize = 200000);

	/*先开OpenMP再分块，导致瞬时内存占用为(blockSize*featureSize*核数)
	已测, 无内存泄漏.2012-6-11*/
	static int erasePolygonLayer_mp_old(
		GTFeatureLayer		*inputFeatureLayer,
		GTFeatureLayer		*eraseFeatureLayer,
		GTFeatureLayer      *resultLayer,
		const bool			 chkEraseTopology = false,
		const double         dTolerance = 1E-10,
		const int			blockSize = 100000);

	/*****************************************************************************
	* @brief	polygon或multipolygon featureclass擦除操作,生成新的featureclass.
	*			直接调用polygonErasePolygon函数进行计算.
	* @param	[in]
	*			inputFeatureClass		被擦除的feature集合
	*			eraseFeatureClass		擦除工具feature集合
	*			chkEraseTopology        是否检查并消除erase layer的拓扑错误对结果造成的影响,可能较慢,默认值为false.
	*			[out]
	* @return	返回结果featureclass.返回值为NULL说明出现错误或inputFeatureClass参数为NULL.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static GTFeatureClass* polygonsErasePolygons(
		GTFeatureClass        *inputFeatureClass,
		GTFeatureClass        *eraseFeatureClass,
		const bool			   chkEraseTopology = false,
		const double           dTolerance = 1E-10);

	/*多边形擦除多边形，OpenMP方法*/
	static GTFeatureClass* polygonsErasePolygons_mp(
		GTFeatureClass        *inputFeatureClass,
		GTFeatureClass        *eraseFeatureClass,
		const bool			   chkEraseTopology = false,
		const double           dTolerance = 1E-10);


	/*Identity, 将Identity对象截取在输入对象的部分*/
	/*点与环*/
	/*线与环*/
	/*环与环*/
	/*****************************************************************************
	* @brief	2个多边形Identity操作,生成0～2个新的几何对象存储在vector中.
	*			效果同arcgis 2个多边形的Identity操作.
	* @param	[in]
	*			inputGeometry			输入几何对象
	*			symmdiffGeometry		Identity几何对象
	*			result_geom_vec			结果几何对象容器
	*			result_mark_vec			结果几何对象标记容器,0:几何图形仅来自前者. 1:来自双方.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果几何对象个数.返回值为-1说明出现错误,1说明2图形相离.
	* @note		函数调用者外部释放结果几何对象容器中对象内存.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static int polygonIdentityPolygon(
		const GTGeometry			*inputGeometry,
		const GTGeometry			*identityGeometry,
		std::vector<GTGeometry*>	&result_geom_vec,
		std::vector<int>			&result_mark_vec,
		const double				 dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	2个多边形图层Identity操作,效果同arcgis 2个多边形图层的Identity操作.
	* @param	[in]
	*			inputFeatureLayer			输入数据图层.
	*			identityFeatureLayer		Identity工具图层.
	*			resultLayer					结果图层,函数调用前必须新建,且字段个数为0,必须为可更新.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1	
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果几何对象个数.返回值为-1说明出现错误,返回-2说明2个操作图层空间参考不一致,返回-3说明初始状态结果图层字段不为0.
	* @note		算法流程:先对每个图层做Union,再做图层间擦除,最后对图层间求交,得到结果.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static int identityPolygonLayers_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *identityFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int              joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	2个多边形图层Identity操作,效果同arcgis 2个多边形图层的Identity操作.
	*			添加海量数据处理能力.
	* @param	[in]
	*			inputFeatureLayer			输入数据图层.
	*			identityFeatureLayer		Identity工具图层.
	*			resultLayer					结果图层,函数调用前必须新建,且字段个数为0,必须为可更新.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1	
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果几何对象个数.返回值为-1说明出现错误,返回-2说明2个操作图层空间参考不一致,返回-3说明初始状态结果图层字段不为0.
	* @note		算法流程:先对每个图层做Union,再做图层间擦除,最后对图层间求交,得到结果.
	*			已测,无内存泄漏.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012.6.11
	*******************************************************************************/
	static int identityPolygonLayers(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *identityFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool             ignorIntersectInSameLayer = true,
		const int              joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int			   blockSize = 200000);

	static int identityPolygonLayers_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *identityFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool             ignorIntersectInSameLayer = true,
		const int              joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int			   blockSize = 200000);

	/*****************************************************************************
	* @brief	2个多边形Featureclass的Identity操作,效果同arcgis的Identity操作.2个featureclass内部feature须具有一致的属性结构
	* @param	[in]
	*			inputFeatureClassName		输入featureclass名称,一般是该featureclass内feature所在图层名称
	*			inputFeatureClass			输入featureclass
	*			identityFeatureClassName	identity featureclass名称,一般是该featureclass内feature所在图层名称
	*			identityFeatureClass		identity featureclass
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果featureclass.返回值为NULL说明出现错误.
	* @note		算法流程:先对每个featureclass内部做Union,再做featureclass间擦除,最后求交,得到结果.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-11
	*******************************************************************************/
	static GTFeatureClass* polygonsIdentityPolygons(
		const char			  *inputFeatureClassName,
		GTFeatureClass        *inputFeatureClass,
		const char			  *identityFeatureClassName,
		GTFeatureClass        *identityFeatureClass,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);


	/*Intersect, 求交*/
	/*点与点*/
	/*线与线*/
	/*环与环*/
	/*****************************************************************************
	* @brief	polygon或multipolygon对象之间的求交操作,生成新的polygon或multipolygon
	* @param	[in]
	*			inputGeometry			被擦除的几何对象
	*			intersectGeometry		求交几何对象
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @return	返回Intersect结果几何对象,若不为NULL则需要外部调用者释放其指针.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static GTGeometry* polygonIntersectPolygon(
		const GTGeometry	*inputGeometry,
		const GTGeometry	*intersectGeometry,
		const double		 dTolerance=1E-10);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层内求交操作,生成新的multipolygon图层.
	*			直接调用polygonIntersectPolygon函数进行计算.结果只保留合并后的公共部分.
	*			求出每个交集后的多边形合并过程采用动态索引.
	* @param	[in]
	*			inputFeatureLayer		要进行求交操作的多边形图层
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果层中feature数.返回-1表示出现错误,返回-3说明结果图层字段个数不为0.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-2
	*******************************************************************************/
	static int intersectPolygonsInSameLayer_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层内求交操作,生成新的multipolygon图层.
	*			直接调用polygonIntersectPolygon函数进行计算.结果只保留合并后的公共部分.
	*			求出每个交集后的多边形合并过程采用动态索引.
	* @param	[in]
	*			inputFeatureLayer		要进行求交操作的多边形图层
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果层中feature数.返回-1表示出现错误,返回-3说明结果图层字段个数不为0.
	* @note		已测,无内存泄漏.2012-6-12,无内存泄漏
	* @author	范俊甫,fanjf@lreis.ac.cn
	*******************************************************************************/
	static int intersectPolygonsInSameLayer(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);
	/*intersectPolygonsInSameLayer的单机多核版本
	核心计算依旧是串行,仅建立索引部分是并行,无内存泄漏.2012-6-12*/
	static int intersectPolygonsInSameLayer_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层内求交操作,生成新的multipolygon图层.
	*			直接调用polygonIntersectPolygon函数进行计算.结果只保留合并后的公共部分.
	*			求出每个交集后的多边形合并过程采用动态索引.(mp表示单机多核版本,目前仅对两两
	*			求交部分应用多核计算,两两求交后的合并过程没有应有多核计算)
	* @param	[in]
	*			inputFeatureLayer		要进行求交操作的多边形图层
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果层中feature数.返回-1表示出现错误,返回-3说明结果图层字段个数不为0.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-2
	*******************************************************************************/
	static int intersectPolygonsInSameLayer_old_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层内求交操作,生成新的multipolygon图层.
	*			直接调用polygonIntersectPolygon函数进行计算.结果保留多边形个数与相交多边形
	*			个数相同.结果多边形按照属性保留策略分别具有各自父多边形的属性.(结果与ArcGIS
	*			Overlap Intersect工具相同)
	* @param	[in]
	*			inputFeatureLayer			要进行求交操作的多边形图层
	*			resultLayer					结果图层,要求非空,可更新,且属性字段个数为0.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1
	*			dTolerance					结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10.
	*			[out]
	* @return	返回结果层中feature数.返回-1表示出现错误,返回-3说明结果图层在函数调用初始状态字段数不为0.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-2
	*******************************************************************************/
	static int intersectPolygonsInSameLayer2_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层内求交操作,生成新的multipolygon图层.
	*			直接调用polygonIntersectPolygon函数进行计算.结果保留多边形个数与相交多边形
	*			个数相同.结果多边形按照属性保留策略分别具有各自父多边形的属性.(结果与ArcGIS
	*			Overlap Intersect工具相同)
	* @param	[in]
	*			inputFeatureLayer			要进行求交操作的多边形图层
	*			resultLayer					结果图层,要求非空,可更新,且属性字段个数为0.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1
	*			dTolerance					结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10.
	*			[out]
	* @return	返回结果层中feature数.返回-1表示出现错误,返回-3说明结果图层在函数调用初始状态字段数不为0.
	* @note		已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn
	*******************************************************************************/
	static int intersectPolygonsInSameLayer2(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	/*单机多核版本Debug下有动态创建R树索引引起的断言错误（split_q.c文件的第69行），release正常*/
	static int intersectPolygonsInSameLayer2_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes =2,
		const double		   dTolerance = 1e-10,
		const int              blockSize = 200000);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层求交操作,生成新的multipolygon图层.
	*			直接调用polygonIntersectPolygon函数进行计算.效果同arcgis2个图层求交.
	* @param	[in]
	*			inputFeatureLayer			要进行求交操作的多边形图层A
	*			intersectFeatureLayer		要进行求交操作的多边形图层B
	*			resultLayer					结果图层,要求非空,可更新,且属性字段个数为0.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:返回-1;
	*			dTolerance					结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10.
	*			blockSize					单次从数据库中取出feature个数，简单控制内存占用。
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误,返回值为-2说明操作图层的空间参考不一致.
	*			返回-3说明结果图层在函数调用初始状态字段数不为0.
	* @note		已测,无内存泄漏.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-11
	*******************************************************************************/
	static int intersectPolygonLayers(
		GTFeatureLayer	      *inputFeatureLayer,
		GTFeatureLayer        *intersectFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	static int intersectPolygons(
		const char 	          *inputLayerName,
		GTFeatureClass	      *inputFeatureClass,
		const char 	          *intsectLayerName,
		GTFeatureClass	      *intsectFeatureClass,
		GTFeatureClass        *resultFeatureClass,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层求交操作,生成新的multipolygon图层.
	*			直接调用polygonIntersectPolygon函数进行计算. mp表示单机多核版本.
	*			效果同arcgis2个图层求交.添加分块处理海量数据功能.
	* @param	[in]
	*			inputFeatureLayer			要进行求交操作的多边形图层A
	*			intersectFeatureLayer		要进行求交操作的多边形图层B
	*			resultLayer					结果图层,要求非空,可更新,且属性字段个数为0.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:返回-1;
	*			dTolerance					结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10.
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误,返回值为-2说明操作图层的空间参考不一致.
	*			返回-3说明结果图层在函数调用初始状态字段数不为0.
	* @note		已测,无内存泄漏.2012-6-11
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-3, edited in 20120525 by fanjf
	*******************************************************************************/
	static int intersectPolygonLayers_mp(
		GTFeatureLayer	      *inputFeatureLayer,
		GTFeatureLayer        *intersectFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	
	/*****************************************************************************
	* @brief	polygon或multipolygon图层求交操作,生成新的multipolygon图层.
	*			直接调用polygonIntersectPolygon函数进行计算. mp表示单机多核版本.
	*			效果同arcgis2个图层求交.添加分块处理海量数据功能.
	* @param	[in]
	*			inputFeatureLayer			要进行求交操作的多边形图层A
	*			intersectFeatureLayer		要进行求交操作的多边形图层B
	*			resultLayer					结果图层,要求非空,可更新,且属性字段个数为0.
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:返回-1;
	*			dTolerance					结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10.
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误,返回值为-2说明操作图层的空间参考不一致.
	*			返回-3说明结果图层在函数调用初始状态字段数不为0.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static int intersectPolygonLayers_old_mp(
		GTFeatureLayer	      *inputFeatureLayer,
		GTFeatureLayer        *intersectFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	具有不同属性结构的2个多边形featureclass求交.2个featureclass必须来自2个featurelayer.
	*			2个char*参数是为了合并字段是自动生成字段名而用,因此最好使用原来featurelayer名称.
	*			直接调用polygonIntersectPolygon函数进行计算.效果同arcgis2个图层求交.
	* @param	[in]
	*			inputFeatureClassName		输入featureclass名称
	*			inputFeatureClass			输入featureclass
	*			intersectFeatureClassName	intersect工具featureclass名称
	*			intersectFeatureClass		intersect工具featureclass
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:返回-1;
	*			[out]
	* @return	返回结果featureclass.返回值为NULL说明出现错误或者2个featureclass中feature空间参考不一致.
	* @note		函数调用者注意销毁返回featureclass的fieldsDefPtr.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static GTFeatureClass* polygonsIntersectPolygons(
		const char			  *inputFeatureClassName,
		GTFeatureClass        *inputFeatureClass,
		const char			  *intersectFeatureClassName,
		GTFeatureClass        *intersectFeatureClass,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);
	/*20120525 fanjf@lreis.ac.cn 添加polygonsIntersectPolygons函数的OpenMP版本*/
	static GTFeatureClass* polygonsIntersectPolygons_mp(
		const char			  *inputFeatureClassName,
		GTFeatureClass        *inputFeatureClass,
		const char			  *intersectFeatureClassName,
		GTFeatureClass        *intersectFeatureClass,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);



	/*Symmetrical Difference, 异或*/
	/*点与点*/
	/*线与线*/
	/*环与环*/
	/*****************************************************************************
	* @brief	2个多边形异或操作,生成0～2个新的几何对象存储在vector中.
	*			效果同arcgis 2个多边形求异或.
	* @param	[in]
	*			inputGeometry			输入几何对象A
	*			symmdiffGeometry		输入几何对象B
	*			result_geom_vec			结果几何对象容器
	*			result_mark_vec			结果几何对象标记容器
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果几何对象个数.返回值为-1说明出现错误,0说明2个图形完全重叠,1说明某一图形完全被包含.
	* @note		函数调用者外部释放结果几何对象容器中对象内存.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static int polygonSymmetricalDifferencePolygon(
		const GTGeometry			*inputGeometry,
		const GTGeometry			*symmdiffGeometry,
		std::vector<GTGeometry*>	&result_geom_vec,
		std::vector<int>			&result_mark_vec,
		const double				 dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层异或操作,生成新的multipolygon图层.
	*			效果同arcgis2个图层求异或.
	* @param	[in]
	*			inputFeatureLayer		要进行求交操作的多边形图层A名称
	*			symmdiffFeatureLayer	要进行求交操作的多边形图层B名称
	*			resultLayer				结果图层名称
	*			joinAttributes			0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:返回-1;
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误,返回值为-2说明操作图层的空间参考不一致.
	*			返回-3说明结果图层在函数调用初始状态字段数不为0.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-10
	*******************************************************************************/
	static int symmetricalDifferencePolygonLayers_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *symmdiffFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int              joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层异或操作,生成新的multipolygon图层.
	*			效果同arcgis2个图层求异或.添加海量数据处理支持.
	* @param	[in]
	*			inputFeatureLayer		要进行求交操作的多边形图层A名称
	*			symmdiffFeatureLayer	要进行求交操作的多边形图层B名称
	*			resultLayer				结果图层名称
	*			joinAttributes			0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:返回-1;
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误,返回值为-2说明操作图层的空间参考不一致.
	*			返回-3说明结果图层在函数调用初始状态字段数不为0.
	* @note		已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-06-06
	*******************************************************************************/
	static int symmetricalDifferencePolygonLayers(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *symmdiffFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool             ignorIntersectInSameLayer = true,
		const int              joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	static int symmetricalDifferencePolygonLayers_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *symmdiffFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool             ignorIntersectInSameLayer = true,
		const int              joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	/*****************************************************************************
	* @brief	采用GT_BOOL_XOR操作符，舍弃所有属性标记。2个多边形异或操作,生成0-1个新的几何对象存储在vector中.
	* @param	[in]
	*			inputFeatureLayer		要进行求交操作的多边形图层A名称
	*			symmdiffFeatureLayer	要进行求交操作的多边形图层B名称
	*			result_geom_vec			结果几何对象，分属于输入和操作几何对象两者的部分被合并为一个对象，因此结果仅包含
	*									1个元素。
	*			result_mark_vec			该变量暂时无用。
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果几何对象个数.返回值为-1说明出现错误,0说明2个图形完全重叠,1说明相交和相离.
	* @note		函数调用者外部释放结果几何对象容器中对象内存.
	* @author	范俊甫,fanjf@lreis.ac.cn,2013-4-7
	*******************************************************************************/
	static int polygonSymmetricalDifferencePolygon_new(
		const GTGeometry			*inputGeometry,
		const GTGeometry			*symmdiffGeometry,
		std::vector<GTGeometry*>	&result_geom_vec,
		std::vector<int>			&result_mark_vec,
		const double				 dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	采用GT_BOOL_XOR操作符，舍弃所有属性标记。2组多边形异或操作.
	* @param	[in]
	*			inputFeatureClass		要进行求交操作的多边形集合。
	*			symmDiffFeatureClass	要进行求交操作的多边形集合。
	*			resultFeatureClass		结果要素集合。
	*			splitGeometries			多多边形是否分裂为单多边形。
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			[out]
	* @return	返回结果要素个数.
	* @note		函数调用者外部释放结果要素集合中对象的内存空间.
	* @author	范俊甫,fanjf@lreis.ac.cn,2013-4-7
	*******************************************************************************/
	static int symmetricalDifferencePolygons(
		GTFeatureClass        *inputFeatureClass,
		GTFeatureClass        *symmDiffFeatureClass,
		GTFeatureClass		  &resultFeatureClass,
		const	bool		   splitGeometries = false,
		const double		   dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	polygon或multipolygon图层异或操作,生成新的multipolygon图层.
	*			采用GT_BOOL_XOR操作符，舍弃所有属性标记。
	* @param	[in]
	*			inputFeatureLayer		要进行求交操作的多边形图层A名称
	*			symmdiffFeatureLayer	要进行求交操作的多边形图层B名称
	*			resultLayer				结果图层名称
	*			splitGeometries			多几何对象是否分裂为单几何对象
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	*			blockSize				单次操作对象数量限制
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误,返回值为-2说明操作图层的空间参考不一致.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2013-04-07
	*******************************************************************************/
	static int symmetricalDifferencePolygonLayers_new(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *symmdiffFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   splitGeometries = false,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);




	/*Union, 求和*/
	/*点与点*/
	/*线与线*/
	/*环与环*/
	/*****************************************************************************
	* @brief	2个多边形合并为一个多边形
	* @param	[in]
	*			inputGeometry			合并的几何对象A
	*			unionGeometry			合并的几何对象B
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回Union结果几何对象,若不为NULL则需要外部调用者释放其指针.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-7-26
	*******************************************************************************/
	static GTGeometry* polygonUnionPolygonToOne(
		const GTGeometry *inputGeometry,
		const GTGeometry *unionGeometry,
		const double      dTolerance=1E-10);

	/*****************************************************************************
	* @brief	多个多边形合并成一个多边形(两两合并).
	* @param	[in]
	*			inputFeatureClass		要合并的Feature集合.函数不更改其值,需要调用者构建和释放.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回最终Union结果Feature,若不为NULL则需要外部调用者释放其指针.
	* @note		结果feature不带有任何原始feature的属性.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-3
	*******************************************************************************/
	static GTFeature* unionPolygonsToOne(
		GTFeatureClass  *inputFeatureClass,
		const double     dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	多个多边形合并成一个多边形(两两合并).
	* @param	[in]
	*			inputGeometries		要合并的几何对象集合.函数不更改其值,需要调用者构建和释放.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回最终Union结果几何对象,若不为NULL则需要外部调用者释放其指针.
	* @note		结果需要外部释放
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-12
	*******************************************************************************/
	static GTGeometry* unionPolygonsToOne(
		const std::vector<GTGeometry*>  inputGeometries,
		const double                    dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	多个多边形合并成一个多边形(两两合并).
	* @param	[in]
	*			inputGeometries		    要合并的几何对象集合.函数不更改其值,需要调用者构建和释放.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回最终Union结果几何对象,若不为NULL则需要外部调用者释放其指针.
	* @note		结果需要外部释放
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-12
	*******************************************************************************/
	static GTGeometry* unionPolygonsToOne(
		const std::vector<GTFeature*>   inputFeatures,
		const double                    dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	同一图层内多边形求并(仅图形有面状交集者合并).
	*			添加海量数据分块处理支持20120529
	* @param	[in]
	*			inputFeatureLayer		输入多边形图层
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-3说明结果图层字段数大于0.
	* @note		结果feature不带有任何原始feature的属性.已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-05-29
	*******************************************************************************/
	static int unionPolygonsInSameLayer(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	/*****************************************************************************
	* @brief	同一图层内多边形求并(仅图形有面状交集者合并).
	*			添加海量数据分块处理支持.20121231.两两合并.
	* @param	[in]
	*			inputFeatureLayer		输入多边形图层
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-3说明结果图层字段数大于0.
	* @note		结果feature不带有任何原始feature的属性.已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-12-31
	*******************************************************************************/
	static int unionPolygonsInSameLayer_new(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	static int unionPolygonsInSameLayer_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	同一图层内多边形求并(不合并,同arcgis union功能).
	*			注意：本函数仅添加了有限的海量数据分块处理支持,级联查找可能相交的
	*			feature进行计算仍旧有可能超过内存限制.
	* @param	[in]
	*			inputFeatureLayer		输入多边形图层
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			joinAttributes			0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-3说明结果图层字段数大于0.
	* @note		结果feature按照属性结合策略保留原始多边形属性.已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-5-29
	*******************************************************************************/
	static int unionPolygonsInSameLayer2(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	/*****************************************************************************
	* @brief	同一图层内多边形求并(不合并,同arcgis union功能).
	*			注意：本函数仅添加了有限的海量数据分块处理支持,级联查找可能相交的
	*			feature进行计算仍旧有可能超过内存限制.OpenMP版本.
	* @param	[in]
	*			inputFeatureLayer		输入多边形图层
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			joinAttributes			0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-3说明结果图层字段数大于0.
	* @note		结果feature按照属性结合策略保留原始多边形属性.已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-5-29
	*******************************************************************************/
	static int unionPolygonsInSameLayer2_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double		   dTolerance=1E-10,
		const int              blockSize=200000);

	static int unionPolygonsInSameLayer2_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);



	/*****************************************************************************
	* @brief	多边形求并(同一图层内或者2个图层间相交图形被合并).
	* @param	[in]
	*			inputFeatureLayer		要进行图层union的多边形图层A
	*			unionFeatureLayer		要进行图层union的多边形图层B
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-3说明结果图层字段数大于0.
	* @note		结果feature不保留原始多边形任何属性.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-4
	*******************************************************************************/
	static int unionPolygonLayers_old_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer   	  *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	多边形求并(同一图层内或者2个图层间相交图形被合并).加入有限的大数据量处理支持.
	* @param	[in]
	*			inputFeatureLayer		要进行图层union的多边形图层A
	*			unionFeatureLayer		要进行图层union的多边形图层B
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-3说明结果图层字段数大于0.
	* @note		结果feature不保留原始多边形任何属性.已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-4
	*******************************************************************************/
	static int unionPolygonLayers_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer   	  *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	/*****************************************************************************
	* @brief	多边形求并(同一图层内或者2个图层间相交图形被合并).加入有限的大数据量处理支持.
	*			多边形搜索策略修改为种子扩展搜索,同unionPolygonsInSameLayer2/_mp函数.
	* @param	[in]
	*			inputFeatureLayer		要进行图层union的多边形图层A
	*			unionFeatureLayer		要进行图层union的多边形图层B
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-3说明结果图层字段数大于0.
	* @note		结果feature不保留原始多边形任何属性.已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-12
	*******************************************************************************/
	static int unionPolygonLayers(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer   	  *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool             ignorIntersectInSameLayer = true,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	/*****************************************************************************
	* @brief	多边形求并(同一图层内或者2个图层间相交图形被合并).加入有限的大数据量处理支持.
	*			多边形搜索策略修改为种子扩展搜索,每一组内为两两合并.
	* @param	[in]
	*			inputFeatureLayer		要进行图层union的多边形图层A
	*			unionFeatureLayer		要进行图层union的多边形图层B
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-3说明结果图层字段数大于0.
	* @note		结果feature不保留原始多边形任何属性.已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-12-31
	*******************************************************************************/
	static int unionPolygonLayers_new(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		//const bool			   ignorIntersectInSameLayer,
		const double		   dTolerance = 1e-10,
		const int              blockSize = 200000);

	/*已测,无内存泄漏.2012-6-12*/
	static int unionPolygonLayers_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer   	  *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool             ignorIntersectInSameLayer = true,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	/*****************************************************************************
	* @brief	多边形求并(不合并,同arcgis 多图层 union功能).
	* @param	[in]
	*			inputFeatureLayer		要进行图层union的多边形图层A
	*			unionFeatureLayer		要进行图层union的多边形图层B
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			joinAttributes			0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-2说明2个图层的空间参考不一致.返回-3说明结果图层字段数大于0.
	* @note		结果feature按照属性结合策略保留原始多边形属性.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-4
	*******************************************************************************/
	static int unionPolygonLayers2_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer   	  *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);


	/*****************************************************************************
	* @brief	多边形求并(不合并,同arcgis 多图层 union功能).添加海量数据处理功能.
	* @param	[in]
	*			inputFeatureLayer		要进行图层union的多边形图层A
	*			unionFeatureLayer		要进行图层union的多边形图层B
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			joinAttributes			0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-2说明2个图层的空间参考不一致.返回-3说明结果图层字段数大于0.
	* @note		结果feature按照属性结合策略保留原始多边形属性.已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-5
	*******************************************************************************/
	static int unionPolygonLayers2(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer   	  *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool              ignorIntersectInSameLayer = true,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	/*****************************************************************************
	* @brief	多边形求并(不合并,同arcgis 多图层 union功能).添加海量数据处理功能.单机多核版本.
	* @param	[in]
	*			inputFeatureLayer		要进行图层union的多边形图层A
	*			unionFeatureLayer		要进行图层union的多边形图层B
	*			resultLayer				结果图层,要求非空,可更新,且属性字段个数为0.
	*			joinAttributes			0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-2说明2个图层的空间参考不一致.返回-3说明结果图层字段数大于0.
	* @note		结果feature按照属性结合策略保留原始多边形属性.已测,无内存泄漏.2012-6-12
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-5
	*******************************************************************************/
	static int unionPolygonLayers2_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer   	  *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool             ignorIntersectInSameLayer = true,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);




	/*****************************************************************************
	* @brief	一组多边形求并(不合并,同arcgis union功能).结果feature按照属性结合策略保留原始多边形属性.
	* @param	[in]
	*			fcName						feature class名称(或者图层名称)
	*			inputFeatureClass			进行求并操作的一组feature,外部管理其指针
	*			joinAttributes				0:NO_FID; 1:ONLY_FID; 2:ALL; 其他:ERROR,返回-1
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回求并结果或者NULL.
	* @note		返回的featureclass若不为空,需要外部释放feature的字段定义(fieldsDefPtr).
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-9
	*******************************************************************************/
	static GTFeatureClass* unionPolygons(
		const char            *fcName,
		GTFeatureClass        *inputFeatureClass,
		const int			   joinAttributes = 2,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	多边形求并(仅图形有公共边者合并).
	* @param	[in]
	*			inputFeatures			输入多边形
	*			resultGeometries		结果几何对象集合
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果多边形个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-3
	*******************************************************************************/
	static int unionPolygons(
		std::vector<GTFeature*>	  inputFeatures,
		std::vector<GTGeometry*>  &resultGeometries,
		const double			  dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	多边形求并(仅图形有公共边者合并).
	* @param	[in]
	*			inputFeatures			输入多边形
	*			resultGeometries		结果几何对象集合
	*			dTolerance				结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果多边形个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-3
	*******************************************************************************/
	static int unionPolygonsWhoINSCT(
		std::vector<GTFeature*>	  inputFeatures,
		std::vector<GTGeometry*>  &resultGeometries,
		const double			  dTolerance = 1e-10);


	/*Difference, 求差*/
	/*点与点*/
	/*线与线*/
	/*环与环*/


	/*Update, 更新*/
	/*仅环与环*/
	/*****************************************************************************
	* @brief	多边形更新多边形(同arcgis update功能).
	* @param	[in]
	*			inputGeometry			输入多边形的几何对象
	*			updateGeometry			更新多边形的几何对象
	*			result_geom_vec			结果几何对象容器
	*			result_mark_vec			结果几何对象标记容器,0:来自前者  1:来自后者   2:来自前者和后者的合并
	*			bKeepBorders			是否对相交的结果几何对象进行合并操作
	*			dTolerance				容差,面积小于此值的ring被舍弃.
	* @param	[out]
	* @return	返回结果几何对象的个数
	* @note		输入、更新及结果几何对象均需函数调用者在外部释放其指针.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-12
	*******************************************************************************/
	static int polygonUpdatePolygon(
		const GTGeometry			*inputGeometry,
		const GTGeometry			*updateGeometry,
		std::vector<GTGeometry*>	&result_geom_vec,
		std::vector<int>			&result_mark_vec,
		const bool					 bKeepBorders = true,
		const double				 dTolerance = 1E-10);


	/*****************************************************************************
	* @brief	多边形图层更新(update)多边形图层(与arcgis类似).
	* @param	[in]
	*			inputFeatureLayer			被更新(update)的多边形图层
	*			updateFeatureLayer			要更新(update)工具多边形图层
	*			resultLayer					结果图层,要求非空,可更新,且属性字段个数为0.
	*			bKeepBorders				是否保留update图形的外边界(true:默认,update工具图
	*										层的图形不被合并;false:update工具层内的图形被合并)
	*			dTolerance					结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-2说明2个图层的空间参考不一致.若为-3说明结果图层初始状态字段数不为0.
	* @note		若指定不保留边界,arcgis保留了update图层中图形的交集部分,本函数将update图层中所有相交者合并.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-12
	*******************************************************************************/
	static int updatePolygonLayer_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *updateFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   bKeepBorders = true,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	多边形图层更新(update)多边形图层(与arcgis类似).添加海量数据处理支持.
	* @param	[in]
	*			inputFeatureLayer			被更新(update)的多边形图层
	*			updateFeatureLayer			要更新(update)工具多边形图层
	*			resultLayer					结果图层,要求非空,可更新,且属性字段个数为0.
	*			bKeepBorders				是否保留update图形的外边界(true:默认,update工具图
	*										层的图形不被合并;false:update工具层内的图形被合并)
	*			dTolerance					结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-2说明2个图层的空间参考不一致.若为-3说明结果图层初始状态字段数不为0.
	* @note		若指定不保留边界,arcgis保留了update图层中图形的交集部分,本函数将update图层中所有相交者合并.
	* @author	范俊甫,fanjf@lreis.ac.cn,已测,无内存泄漏.2012-6-11
	*******************************************************************************/
	static int updatePolygonLayer(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *updateFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   bKeepBorders = true,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000,
		const bool			   useSeedCrossSearch = true);

	static int updatePolygonLayer_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *updateFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   bKeepBorders = true,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000,
		const bool			   useSeedCrossSearch = true);





	/*Spatial Join, 基于空间关系的属性连接*/
	/*环与环*/
	/*****************************************************************************
	* @brief	基于空间关系的多边形图层属性连接(同arcgis spatial join功能).
	* @param	[in]
	*			inputFeatureLayer			目标多边形图层
	*			sjoinFeatureLayer			spatial join工具多边形图层
	*			resultLayer					结果图层,要求非空,可更新,且属性字段个数为0.
	*			bJoinOneToOne				当输入数据层中多边形与工具层中多个多边形满足join条件时的保留策略.true:仅保留一个副本; false:保留多个副本.
	*			bKeepAllInputFeatures		当目标多边形图层中多边形不与工具层中任何一个多边形满足join条件时的图形保留策略.true:全部输出; false:舍弃
	*										不满足条件者.
	*			iMatchOption				满足spatial join的条件设置. 0:相交(INTSECTS,目标图形与工具图形相交); 
	*										1:包含(CONTIANS,目标图形完全包含工具图形); 2:被包含(WITH_IN,目标图形包含于工具图形); 3:最近者匹配(CLOSEST,
	*										找到最近的图形进行匹配,暂不支持)
	*			dTolerance					结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-2说明2个图层的空间参考不一致,返回-3说明结果图层初始状态字段个数不为0,返回-4说明spatial join
	*			空间关系匹配模式不被支持.
	* @note		目前尚不支持fields map.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-13
	*******************************************************************************/
	static int spatialJoinPolygonLayers_old(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *sjoinFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   bJoinOneToOne = true,
		const bool             bKeepAllInputFeatures = true,
		const int              iMatchOption = 0,
		const double           dTolerance = 1E-10);

	/*****************************************************************************
	* @brief	基于空间关系的多边形图层属性连接(同arcgis spatial join功能).
	* @param	[in]
	*			inputFeatureLayer			目标多边形图层
	*			sjoinFeatureLayer			spatial join工具多边形图层
	*			resultLayer					结果图层,要求非空,可更新,且属性字段个数为0.
	*			bJoinOneToOne				当输入数据层中多边形与工具层中多个多边形满足join条件时的保留策略.true:仅保留一个副本; false:保留多个副本.
	*			bKeepAllInputFeatures		当目标多边形图层中多边形不与工具层中任何一个多边形满足join条件时的图形保留策略.true:全部输出; false:舍弃
	*										不满足条件者.
	*			iMatchOption				满足spatial join的条件设置. 0:相交(INTSECTS,目标图形与工具图形相交); 
	*										1:包含(CONTIANS,目标图形完全包含工具图形); 2:被包含(WITH_IN,目标图形包含于工具图形); 3:最近者匹配(CLOSEST,
	*										找到最近的图形进行匹配,暂不支持)
	*			dTolerance					结果多边形ring的最小面积,小于此值的ring被舍弃.默认为1E-10
	* @param	[out]
	* @return	返回结果图层中Feature个数,若为-1说明发生错误.返回-2说明2个图层的空间参考不一致,返回-3说明结果图层初始状态字段个数不为0,返回-4说明spatial join
	*			空间关系匹配模式不被支持.
	* @note		目前尚不支持fields map.已测,无内存泄漏.2012-6-11
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-11
	*******************************************************************************/
	static int spatialJoinPolygonLayers(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *sjoinFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   bJoinOneToOne = true,
		const bool             bKeepAllInputFeatures = true,
		const int              iMatchOption = 0,
		const double           dTolerance = 1E-10,
		const int			   blockSize = 200000);

	static int spatialJoinPolygonLayers_mp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *sjoinFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   bJoinOneToOne = true,
		const bool             bKeepAllInputFeatures = true,
		const int              iMatchOption = 0,
		const double           dTolerance = 1E-10,
		const int			   blockSize = 200000);







	/*该函数针对较小数据量(2个图层feature总数不超过设定值，如1000000)的图层Overlay并行操作*/
	static int erasePolygonLayer_lmp(
		GTFeatureLayer			*inputFeatureLayer,
		GTFeatureLayer			*eraseFeatureLayer,
		GTFeatureLayer			*resultLayer,
		const bool				 chkEraseTopology = false,
		const double			 dTolerance = 1e-10,
		const int                blockSize  = 200000);

	/*该函数针对较小数据量(2个图层feature总数不超过设定值，如1000000)的图层Overlay并行操作*/
	static int intersectPolygonsInSameLayer_lmp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const double		   dTolerance = 1e-10);

	/*该函数针对较小数据量(2个图层feature总数不超过设定值，如1000000)的图层Overlay并行操作*/
	static int intersectPolygonLayers_lmp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *intersectFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes = 2,
		const double		   dTolerance = 1e-10,
		const int              blockSize = 200000);

	/*该函数针对较小数据量(2个图层feature总数不超过设定值，如1000000)的图层Overlay并行操作*/
	static int intersectPolygonsInSameLayer2_lmp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes =2,
		const double		   dTolerance = 1e-10,
		const int              blockSize = 200000);

	/*该函数针对较小数据量(2个图层feature总数不超过设定值，如1000000)的图层Overlay并行操作*/
	static int unionPolygonsInSameLayer2_lmp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const int			   joinAttributes =2,
		const double		   dTolerance = 1e-10,
		const int              blockSize = 200000);

	/*该函数针对较小数据量(2个图层feature总数不超过设定值，如1000000)的图层Overlay并行操作*/
	static int unionPolygonLayers_lmp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool              ignorIntersectInSameLayer = true,
		const double		   dTolerance = 1e-10,
		const int              blockSize = 200000);

	/*该函数针对较小数据量(2个图层feature总数不超过设定值，如1000000)的图层Overlay并行操作*/
	static int unionPolygonLayers2_lmp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer   	  *unionFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool              ignorIntersectInSameLayer = true,
		const int              joinAttributes = 2,
		const double		   dTolerance = 1e-10,
		const int              blockSize=200000);

	/*该函数针对较小数据量(2个图层feature总数不超过设定值，如1000000)的图层Overlay并行操作*/
	static int updatePolygonLayer_lmp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *updateFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   bKeepBorders = true,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000,
		const bool			   useSeedCrossSearch = true);

	/*该函数针对较小数据量(2个图层feature总数不超过设定值，如1000000)的图层Overlay并行操作*/
	static int identityPolygonLayers_lmp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *identityFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool             ignorIntersectInSameLayer = true,
		const int              joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int			   blockSize = 200000);

	/*该函数针对较小数据量(2个图层feature总数不超过设定值，如1000000)的图层Overlay并行操作*/
	static int symmetricalDifferencePolygonLayers_lmp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *symmdiffFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool             ignorIntersectInSameLayer = true,
		const int              joinAttributes = 2,
		const double           dTolerance = 1E-10,
		const int              blockSize = 200000);

	static int spatialJoinPolygonLayers_lmp(
		GTFeatureLayer        *inputFeatureLayer,
		GTFeatureLayer        *sjoinFeatureLayer,
		GTFeatureLayer        *resultLayer,
		const bool			   bJoinOneToOne = true,
		const bool             bKeepAllInputFeatures = true,
		const int              iMatchOption = 0,
		const double           dTolerance = 1E-10,
		const int			   blockSize = 200000);

};

}
#endif

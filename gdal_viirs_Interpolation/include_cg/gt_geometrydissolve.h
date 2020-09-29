/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  
* Author:   
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c)  保留所有权利
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

	/*自相交分解*/

	/*****************************************************************************
	* @brief	环的自相交分解
	* @param	[in]
	*			ring			环对象
	*			[out]
	*			ringlist		分解后的环对象集合
	* @return	分解后的环数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-30
	*******************************************************************************/
	static int ringDissolve(const GTRing *ring, std::vector<GTRing *>& ringlist);


	/*****************************************************************************
	* @brief	线的自相交分解
	* @param	[in]
	*			lineStr			线对象
	*			[out]
	*			lineStrlist		分解后的线对象集合
	* @return	分解后的线数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-10-09
	*******************************************************************************/
	static int lineStringDissolve(const GTLineString *lineStr, std::vector<GTLineString *>& lineStrlist);

	/*多边形自相交分解*/
	/*线自相交分解*/


	/*相邻斑块合并*/

	/*****************************************************************************
	* @brief	指定列具有相同属性值的相邻多边形合并。
	* @param	[in]
	*			inputFeatures				输入feature对象
	*			inputFeatureCount			输入feature对象数
	*			resultFields				结果feature字段定义
	*			resultFieldIndex			结果字段在原始feature字段定义中的索引
	*			resultFieldCount			结果字段个数
	*			resultFeatures				结果feature对象
	*			bCreateMultiPartFeature		是否生成多部分对象
	*			dTolerance					容差值,面积小于此值的GTRing在构造结果几何对象时被舍弃.
	*			[out]
	*			resultFeatures				按规则合并后的结果feature对象
	* @return	结果feature数量
	* @note		输入的feature是经过属性筛选之后的feanture集合，都将按规则进行合并，返回的指针链表需要外部释放
	* @author	范俊甫，fanjf@lreis.ac.cn,2011-10-11
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
	* @brief	图层内指定列具有相同属性值的相邻多边形合并。
	* @param	[in]
	*			inputLayer					输入图层
	*			resultLayer					结果图层
	*			resultFieldIndex			结果字段在原始feature字段定义中的索引
	*			resultFieldCount			结果字段个数
	*			bCreateMultiPartFeature		是否生成多部分对象
	*			dTolerance					容差值,面积小于此值的GTRing在构造结果几何对象时被舍弃.
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误,返回值为-2说明操作图层的空间参考不一致,返回-3说明结果图层初始状态字段个数不为0.
	* @note		
	* @author	范俊甫，fanjf@lreis.ac.cn,2011-10-11
	*******************************************************************************/
	static int dissolvePolygonLayer_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		std::vector<int>	resultFieldIndex,
		const int			resultFieldCount,
		const bool			bCreateMultiPartFeature = true,
		const double		dTolerance = 1e-10);

	
	/*****************************************************************************
	* @brief	图层内指定列具有相同属性值的相邻多边形合并。
	* @param	[in]
	*			inputLayer					输入图层
	*			resultLayer					结果图层
	*			resultFieldIndex			结果字段在原始feature字段定义中的索引
	*			resultFieldCount			结果字段个数
	*			bCreateMultiPartFeature		是否生成多部分对象
	*			dTolerance					容差值,面积小于此值的GTRing在构造结果几何对象时被舍弃.
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误,返回值为-2说明操作图层的空间参考不一致,返回-3说明结果图层初始状态字段个数不为0.
	* @note		添加海量数据处理能力.
	* @author	范俊甫，fanjf@lreis.ac.cn,2012-06-15
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

	/*返回的几何对象由调用者释放其内存*/
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
	* @brief	图层内指定列具有相同属性值的feature聚合为一个multi part feature。
	* @param	[in]
	*			inputLayer					输入图层,为GT_Pont或者GT_MultiPont类型
	*			resultLayer					结果图层,必须为GT_MultiPont类型
	*			resultFieldIndex			结果字段在原始feature字段定义中的索引
	*			resultFieldCount			结果字段个数
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误;
	*			返回值为-2说明操作图层的空间参考不一致;
	*			返回-3说明结果图层初始状态字段个数不为0.
	* @note		添加海量数据处理能力.
	* @author	范俊甫，fanjf@lreis.ac.cn,2012-06-15/07-09 edited
	*******************************************************************************/
	static int dissolvePointLayerByAttributes(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		std::vector<int>	resultFieldIndex,
		const int			resultFieldCount,
		const int           blockSize = 200000);


	/*****************************************************************************
	* @brief	图层内指定列具有相同属性值的feature聚合为一个multi part feature。
	* @param	[in]
	*			inputLayer					输入图层,为GT_Pont或者GT_MultiPont类型
	*			resultLayer					结果图层,必须为GT_MultiPont类型
	*			resultFieldIndex			结果字段在原始feature字段定义中的索引
	*			resultFieldCount			结果字段个数
	*			[out]
	* @return	返回结果层中feature数.返回值为-1说明出现错误;
	*			返回值为-2说明操作图层的空间参考不一致;
	*			返回-3说明结果图层初始状态字段个数不为0.
	* @note		添加海量数据处理能力.
	* @author	范俊甫，fanjf@lreis.ac.cn,2012-06-15/07-09 edited
	*******************************************************************************/
	static int dissolveLineLayerByAttributes(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		std::vector<int>	resultFieldIndex,
		const int			resultFieldCount,
		const int           blockSize = 200000);

	/*指定属性列相同合并*/

};

}
#endif


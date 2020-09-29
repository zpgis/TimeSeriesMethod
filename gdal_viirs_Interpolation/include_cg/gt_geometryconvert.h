/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  几何对象之间的类型转换
* 
* Author:   范俊甫, fanjf@lreis.ac.cn 2011年-05月-11日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c) 保留所有权利
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

	/*由线生成面
	 *如果线段之间有重叠, 必须事先移除重叠部分
	 *并且线段不能闭合
	 */
	static int convertLineToPolygon(
		std::vector<GTLineString *>& lineString_list,
		const int                    lineString_count,
        std::vector<GTRing *>&       ring_list_out,
		std::vector<GTLineString>&   line_list_out);
	
	/*****************************************************************************
	* @brief	由线生成面,自写算法，通过分别建立各线串点与交点链表，依次搜索各点
	*			最新版，待测是否有bug
	* @param	[in]
	*			lineString_list				线串集合
	*			lineString_count			线串数量	
	*			[out]
	*			ring_list_out				生成的环集合
	* @return	生成后的环对象数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-26
	*******************************************************************************/
	static int convertLineToPolygon3(
		std::vector<GTLineString *>& lineString_list,
		const int                    lineString_count,
        std::vector<GTRing *>&       ring_list_out,
		std::vector<GTLineString>&   line_list_out);

	/*点生成线*/
	
	/*线生成点*/

	/*****************************************************************************
	* @brief	将lineString或者multiLineString转成点（centroid）
	* @param	[in]
	*			inputLine			lineString或者multiLineString类型的几何对象
	* @param	[out]
	* @return	返回lineString或者multiLineString的centroid.
	* @note		返回值为GTRawPoint类型.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *lineCentroid(
		const GTGeometry	*inputLine);

	/*****************************************************************************
	* @brief	将lineString或者multiLineString转换成点（inside,取线串上距离线的envelop中心最近的点）
	* @param	[in]
	*			inputLine			lineString或者multiLineString类型的几何对象
	* @param	[out]
	* @return	返回取lineString或者multiLineString上距离几何对象的Envelop中心最近的点.
	* @note		返回值为GTRawPoint类型.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *lineToPointInside(
		const GTGeometry	*inputLine);

	/*****************************************************************************
	* @brief	求lineString的长度中点（逐段相加至第一个大于1/2总长度的vertex，再用等比定理求中点坐标）
	* @param	[in]
	*			input_line			线串几何对象.
	*			dTolerance			容差值,当2线段长度之差小于此值时认为二者长度相等.默认为1e-10.
	* @param	[out]
	* @return	线串的长度中点,该点不一定是线串的vertex之一,但是一定在线串上.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *lineStringMiddleLengthPoint(
		GTLineString *input_line,
		const double  dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	将lineString或者multiLineString层转换成点层
	* @param	[in]
	*			inputLineLayer			lineString或者multiLineString图层.
	*			resultLayer				结果图层,必须为点层,且函数被调用时已经初始化;
	*									可更新;字段个数为0;空间参考与输入的线层一致.
	*			isInside				true:取线上距离线的外包矩形中心最近的点; false:取线的centroid.
	* @param	[out]
	* @return	结果图层最终具有与输入图层相同的属性结构.
	* @note		注意释放结果图层以及结果图层的属性字段定义;multiLineString的centroid为每个lineString的centroid的x/y均值.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static int convertLineLayerToPointLayer_old(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false);


	/*****************************************************************************
	* @brief	将lineString或者multiLineString层转换成点层,添加海量数据处理能力,无内存泄漏.
	* @param	[in]
	*			inputLineLayer			lineString或者multiLineString图层.
	*			resultLayer				结果图层,必须为点层,且函数被调用时已经初始化;
	*									可更新;字段个数为0;空间参考与输入的线层一致.
	*			isInside				true:取线上距离线的外包矩形中心最近的点; false:取线的centroid.
	* @param	[out]
	* @return	结果图层最终具有与输入图层相同的属性结构.
	* @note		注意释放结果图层以及结果图层的属性字段定义;multiLineString的centroid为每个lineString的centroid的x/y均值.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-06-13
	*******************************************************************************/
	static int convertLineLayerToPointLayer(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false,
		const int            blockSize = 200000);

	/*****************************************************************************
	* @brief	将lineString或者multiLineString层转换成点层,添加海量数据处理能力,无内存泄漏.
	*			单机多核版本.
	* @param	[in]
	*			inputLineLayer			lineString或者multiLineString图层.
	*			resultLayer				结果图层,必须为点层,且函数被调用时已经初始化;
	*									可更新;字段个数为0;空间参考与输入的线层一致.
	*			isInside				true:取线上距离线的外包矩形中心最近的点; false:取线的centroid.
	* @param	[out]
	* @return	结果图层最终具有与输入图层相同的属性结构.
	* @note		注意释放结果图层以及结果图层的属性字段定义;multiLineString的centroid为每个lineString的centroid的x/y均值.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-06-13
	*******************************************************************************/
	static int convertLineLayerToPointLayer_mp(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false,
		const int            blockSize = 50000);




	/*多点转单点*/
	/*****************************************************************************
	* @brief	multiPoint的centroid为各个点的x、y平均值
	* @param	[in]
	*			inputPoints			multiPoint类型的几何对象
	* @param	[out]
	* @return	返回multiPoint的centroid.
	* @note		返回值为GTRawPoint类型.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *multiPointsCentroid(
		const GTGeometry	*inputPoints);

	/*****************************************************************************
	* @brief	multiPoint的所有点中距离centroid最近的点
	* @param	[in]
	*			inputPoints			multiPoint类型的几何对象
	* @param	[out]
	* @return	返回multiPoint所有点中距离centroid最近的点.
	* @note		返回值为GTRawPoint类型.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *multiPointsToPointInside(
		const GTGeometry	*inputPoints);

	/*****************************************************************************
	* @brief	将multiPoint层转换成点层
	* @param	[in]
	*			inputMultiPointLayer	multiPoint图层.
	*			resultLayer				结果图层,必须为点层,且函数被调用时已经初始化;
	*									可更新;字段个数为0;空间参考与输入的multiPoint层一致.
	*			isInside				true:取multiPoint所有点中距离其centroid最近的点; false:取multiPoint的centroid.
	* @param	[out]
	* @return	结果图层最终具有与输入图层相同的属性结构.
	* @note		注意释放结果图层以及结果图层的属性字段定义;multiPoint的centroid为每个点x/y均值.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static int convertMultiPointLayerToPointLayer_old(
		GTFeatureLayer		*inputMultiPointLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false);

	/*****************************************************************************
	* @brief	将multiPoint层转换成点层
	* @param	[in]
	*			inputMultiPointLayer	multiPoint图层.
	*			resultLayer				结果图层,必须为点层,且函数被调用时已经初始化;
	*									可更新;字段个数为0;空间参考与输入的multiPoint层一致.
	*			isInside				true:取multiPoint所有点中距离其centroid最近的点; false:取multiPoint的centroid.
	* @param	[out]
	* @return	结果图层最终具有与输入图层相同的属性结构.
	* @note		注意释放结果图层以及结果图层的属性字段定义;multiPoint的centroid为每个点x/y均值.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static int convertMultiPointLayerToPointLayer(
		GTFeatureLayer		*inputMultiPointLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false,
		const int			 blockSize = 200000);

	/*单机多核版本*/
	static int convertMultiPointLayerToPointLayer_mp(
		GTFeatureLayer		*inputMultiPointLayer,
		GTFeatureLayer		*resultLayer,
		const bool			 isInside = false,
		const int			 blockSize = 50000);

		

		

	/*线的vertex生成点*/
	/*****************************************************************************
	* @brief	将lineString或者multiLineString的vertices转成点（折点转点）
	* @param	[in]
	*			inputLine			lineString或者multiLineString类型的几何对象
	*			point_list_out		结果点集合
	*			point_location		0:ALL,取所有vertex; 1:MID,取每条线的中点; 2:START,取每条线的起点;
	*								3:END,取每条线的终点; 4:BOTH_ENDS,取每条线的起点和终点.
	*			dTolerance			容差值,当2线段长度之差小于此值时认为二者长度相等.默认为1e-10.
	* @param	[out]
	* @return	返回lineString或者multiLineString符合条件的vertex集合及其个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static int lineVerticesToPoints(
		const GTGeometry			*inputLine,
		std::vector<GTRawPoint*>	&point_list_out,
		const int					 point_location = 0,
		const double				 dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	将lineString或者multiLineString层转换成点层(折线转点)
	* @param	[in]
	*			inputLineLayer			lineString或者multiLineString图层.
	*			resultLayer				结果图层,必须为点层,且函数被调用时已经初始化;
	*									可更新;字段个数为0;空间参考与输入的线层一致.
	*			pointLocation			0:ALL,取所有vertex; 1:MID,取每条线的中点; 2:START,取每条线的起点;
	*									3:END,取每条线的终点; 4:BOTH_ENDS,取每条线的起点和终点.
	*			dTolerance				容差值,当2线段长度之差小于此值时认为二者长度相等.默认为1e-10.
	* @param	[out]
	* @return	结果图层最终具有与输入图层相同的属性结构.
	* @note		注意释放结果图层以及结果图层的属性字段定义;只有求线串长度中点时才使用容差值.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static int convertLineLayerToPointLayerByVertices_old(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const int			 pointLocation = 0,
		const double		 dTolerance = 1e-10);


	/*****************************************************************************
	* @brief	将lineString或者multiLineString层转换成点层(折线转点)
	*			添加海量数据处理能力,已测,无内存泄漏.
	* @param	[in]
	*			inputLineLayer			lineString或者multiLineString图层.
	*			resultLayer				结果图层,必须为点层,且函数被调用时已经初始化;
	*									可更新;字段个数为0;空间参考与输入的线层一致.
	*			pointLocation			0:ALL,取所有vertex; 1:MID,取每条线的中点; 2:START,取每条线的起点;
	*									3:END,取每条线的终点; 4:BOTH_ENDS,取每条线的起点和终点.
	*			dTolerance				容差值,当2线段长度之差小于此值时认为二者长度相等.默认为1e-10.
	* @param	[out]
	* @return	结果图层最终具有与输入图层相同的属性结构.
	* @note		注意释放结果图层以及结果图层的属性字段定义;只有求线串长度中点时才使用容差值.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-6-13
	*******************************************************************************/
	static int convertLineLayerToPointLayerByVertices(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const int			 pointLocation = 0,
		const double		 dTolerance = 1e-10,
		const int            blockSize = 200000);

	/*单机多核版本,已测,无内存泄漏.2012-6-13*/
	static int convertLineLayerToPointLayerByVertices_mp(
		GTFeatureLayer		*inputLineLayer,
		GTFeatureLayer		*resultLayer,
		const int			 pointLocation = 0,
		const double		 dTolerance = 1e-10,
		const int            blockSize = 200000);


	/*面生成线*/

	/*****************************************************************************
	* @brief	探测多边形A和多边形B是否存在公共边（A\B的公共边可能存在不同的点）
	* @param	[in]
	*			polygonA				多边形A.
	*			polygonB				多边形B.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	true/false.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-10-10
	*******************************************************************************/
	static bool isHaveCommonEdges(
		const GTGeometry			*polygonA,
		const GTGeometry			*polygonB,
		const double				 dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	探测多边形A和多边形B的公共边（A\B的公共边可能存在不同的点）
	* @param	[in]
	*			polygonA				多边形A.
	*			polygonB				多边形B.
	*			lineString_out			多边形A\B的公共边,注意释放内存.
	*			lineString_marker		公共边的标志. 0:对应lineString属于多边形A;  1:对应lineString属于多边形B.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	公共边的个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int detectCommonEdges(
		const GTGeometry			*polygonA,
		const GTGeometry			*polygonB,
		std::vector<GTLineString*>	&lineString_out,
		std::vector<int>			&lineString_marker,
		const double				 dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	探测输入多边形图层内相邻多边形的公共边.
	* @param	[in]
	*			inputLayer				输入的多边形图层.
	*			resultLayer				结果图层,函数调用前必须新建,且字段数为0,必须为可更新.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	公共边的个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int getCommonEdges_old(
		GTFeatureLayer *inputLayer,
		GTFeatureLayer *resultLayer,
		const double	dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	探测输入多边形图层内相邻多边形的公共边.
	* @param	[in]
	*			inputLayer				输入的多边形图层.
	*			resultLayer				结果图层,函数调用前必须新建,且字段数为0,必须为可更新.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	公共边的个数.
	* @note		添加海量数据处理能力,已测,无内存泄漏.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-14
	*******************************************************************************/
	static int getCommonEdges(
		GTFeatureLayer *inputLayer,
		GTFeatureLayer *resultLayer,
		const double	dTolerance = 1e-10,
		const int       blockSize = 200000);

	/*****************************************************************************
	* @brief	将多边形每个环从起点(终点)处拆成lineString.
	* @param	[in]
	*			polygonA				输入的多边形.
	*			lineString_out			多边形A边,使用完毕注意释放内存.
	* @param	[out]
	* @return	边的个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static  int convertPolygonToLines(
		const GTGeometry			*polygonA,
		std::vector<GTLineString*>  &lineString_out);

	/*****************************************************************************
	* @brief	返回A与B的公共边(仅返回原属于多边形A的公共边，公共边所在ring的id以及
	*			公共边在ring上的起始点和终止点ID，这样保证得到的公共边上的点都是多边形A的环上的点)
	* @param	[in]
	*			polygonA				输入的多边形A.
	*			polygonB				输入的多边形B.
	*			lineString_out			多边形AB的公共边,使用完毕注意释放内存.
	*			ringId_A				公共边所在ring的id,若A为multiPolygon,则ringID为从第一个多边形到最后一个多边形累加的ring个数
	*			fromAndToPointID_out	公共边的起始点和终止点在ring上的ID.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	边的个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static  int detectCommonEdgesOnlyOnPolygonAWithMarkers(
		const GTGeometry				*polygonA,
		const GTGeometry				*polygonB,
		std::vector<GTLineString*>		&lineStringOut,
		std::vector<int>				&ringId_A,
		std::vector<std::vector<int> >	&fromAndToPointID_out,
		const double					 dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	返回A与B的公共边(仅返回原属于多边形A的公共边，公共边所在ring的id以及对
	*			应多边形B上环的id，这样保证得到的公共边上的点都是多边形A的环上的点)
	* @param	[in]
	*			polygonA				输入的多边形A.
	*			polygonB				输入的多边形B.
	*			lineString_out			多边形AB的公共边,使用完毕注意释放内存.
	*			ringId_A				公共边所在ring的id,若A为multiPolygon,则ringID为从第一个多边形到最后一个多边形累加的ring个数
	*			ringId_B				公共边所在ring的id,若B为multiPolygon,则ringID为从第一个多边形到最后一个多边形累加的ring个数
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	边的个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int detectCommonEdgesOnlyOnPolygonAWithMarkers(
		const GTGeometry			*polygonA,
		const GTGeometry			*polygonB,
		std::vector<GTLineString*>	&lineStringOut,
		std::vector<int>			&ringId_A,
		std::vector<int>			&ringId_B,
		const double				 dTolerance = 1e-10);


	/*****************************************************************************
	* @brief	将多边形图层转为线层(保留属性和线段左右邻面ID)
	* @param	[in]
	*			inputLayer				输入的多边形图层.
	*			resultLayer				结果图层.函数调用前必须新建,且字段数为0,必须为可更新.
	*			bKeepNonCommonEdges		输出结果中是否保留非公共边.
	*			bSplitOnIntersectPoint	针对多边形图层内部存在压盖的情况,是否在边的交点处打断.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	边的个数.
	* @note		该函数未将公共边做union，直接打断，存在公共边形状一样而点不一样导致打断后存在剩余线串部分与公共边依旧重合的问题
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int convertPolygonLayerToLineLayerWithAttributes_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	将多边形图层转为线层(保留属性和线段左右邻面ID),添加海量数据处理能力.
	* @param	[in]
	*			inputLayer				输入的多边形图层.
	*			resultLayer				结果图层.函数调用前必须新建,且字段数为0,必须为可更新.
	*			bKeepNonCommonEdges		输出结果中是否保留非公共边.
	*			bSplitOnIntersectPoint	针对多边形图层内部存在压盖的情况,是否在边的交点处打断.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	边的个数.
	* @note		该函数未将公共边做union，直接打断，存在公共边形状一样而点不一样导致打断后存在剩余线串部分与公共边依旧重合的问题
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-14
	*******************************************************************************/
	static int convertPolygonLayerToLineLayerWithAttributes(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10,
		const int           blockSize = 200000);
	/*单机多核版本*/
	static int convertPolygonLayerToLineLayerWithAttributes_mp(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10,
		const int           blockSize = 200000);


	/*****************************************************************************
	* @brief	求A\B的公共边中属于A的部分.
	* @param	[in]
	*			polygonA				输入的多边形A.
	*			polygonB				输入的多边形B.
	*			lineStringOut			输出的公共边列表.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	边的个数.
	* @note		结果lineString上的点不一定属于B,此函数仅作为函数detectCommonEdgesOnlyOnPolygonAWithMarkers的补充而提供.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int detectCommonEdgesOnlyOnPolygonA(
		const GTGeometry			*polygonA,
		const GTGeometry			*polygonB,
		std::vector<GTLineString*>	&lineStringOut,
		const double				dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	将两个具有相同形状可能具有不同点的几何对象合并.要合并的线必须具有重合的形
	*			状，但是形状上的点可以不同，取所有不同的点组成一个同原始形状一样的线返回.
	* @param	[in]
	*			line_str_a				输入的第1个lineString.
	*			line_str_b				输入的第2个lineString.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	合并结果.
	* @note		具体做法为删除重复的点,保留不同点,因此该函数不适用于任意两个lineString合并.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static GTLineString* unionLineStringsWithSameShapeToOne(
		GTLineString	*line_str_a,
		GTLineString	*line_str_b,
		const double	dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	求公共边，返回公共边是A\B 2个公共边上所有不同点的集合（不再区分原属于A或者原属于B，相同部分仅保留一份）
	* @param	[in]
	*			polygonA				输入的多边形A.
	*			polygonB				输入的多边形B.
	*			lineStringOut			输出的公共边列表.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	公共边个数.
	* @note		实现思路为:先分别求A/B公共边,再将重合者合并,使用unionLineStringsWithSameShapeToOne函数.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int detectUnionedCommonEdges(
		const GTGeometry			*polygonA,
		const GTGeometry			*polygonB,
		std::vector<GTLineString*>	&lineStringOut,
		const double				dTolerance = 1e-10);


	/*****************************************************************************
	* @brief	将多边形图层转为线层(不保留属性，公共边仅保留一份)
	* @param	[in]
	*			inputLayer				输入的多边形图层.
	*			resultLayer				结果图层.函数调用前必须新建,且字段数为0,必须为可更新.
	*			bKeepNonCommonEdges		输出结果中是否保留非公共边.
	*			bSplitOnIntersectPoint	针对多边形图层内部存在压盖的情况,是否在边的交点处打断.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	边的个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static int convertPolygonLayerToLineLayer_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	将多边形图层转为线层(不保留属性，公共边仅保留一份).添加海量数据处理能力.
				已测,无内存泄漏.2012-6-13
	* @param	[in]
	*			inputLayer				输入的多边形图层.
	*			resultLayer				结果图层.函数调用前必须新建,且字段数为0,必须为可更新.
	*			bKeepNonCommonEdges		输出结果中是否保留非公共边.
	*			bSplitOnIntersectPoint	针对多边形图层内部存在压盖的情况,是否在边的交点处打断.
	*			dTolerance				容差值,用于判断点是否重合,点是否在线上,点是否在环内等.默认为1e-10.
	* @param	[out]
	* @return	边的个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-06-13
	*******************************************************************************/
	static int convertPolygonLayerToLineLayer(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10,
		const int           blockSize = 200000);
	/*单机多核版本*/
	static int convertPolygonLayerToLineLayer_mp(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			bKeepNonCommonEdges = true,
		const bool			bSplitOnIntersectPoint = false,
		const double		dTolerance = 1e-10,
		const int           blockSize = 200000);

		

	/*****************************************************************************
	* @brief	将多边形图层转为线层(简单打断每个环，保留原始属性)
	* @param	[in]
	*			inputLayer				输入的多边形图层.
	*			resultLayer				结果图层.函数调用前必须新建,且字段数为0,必须为可更新.
	*			isOneToOne				true:多个环的多边形和多多边形转为多线; false:每个环转为一条线(一个feature).
	* @param	[out]
	* @return	边的个数.
	* @note		从起点和终点处简单打断每个环为lineString.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-20
	*******************************************************************************/
	static int convertPolygonLayerToLineLayer_Simple_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			isOneToOne = false);

	/*****************************************************************************
	* @brief	将多边形图层转为线层(简单打断每个环，保留原始属性)
	*			添加海量数据处理能力.已测,无内存泄漏.
	* @param	[in]
	*			inputLayer				输入的多边形图层.
	*			resultLayer				结果图层.函数调用前必须新建,且字段数为0,必须为可更新.
	*			isOneToOne				true:多个环的多边形和多多边形转为多线; false:每个环转为一条线(一个feature).
	* @param	[out]
	* @return	边的个数.
	* @note		从起点和终点处简单打断每个环为lineString.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-06-14
	*******************************************************************************/
	static int convertPolygonLayerToLineLayer_Simple(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			isOneToOne = false,
		const int           blockSize = 200000);
	/*单机多核版本*/
	static int convertPolygonLayerToLineLayer_Simple_mp(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool			isOneToOne = false,
		const int           blockSize = 200000);

	/*面生成点(暂时没有实现Very Important Point的求取)*/
	/*****************************************************************************
	* @brief	将多边形折点转点
	* @param	[in]
	*			polygonA				输入的多边形.
	*			resultPoints			结果点集合.
	* @param	[out]
	* @return	点的个数.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-29
	*******************************************************************************/
	static int polygonVerticesToPoints(
		const GTGeometry		  *polygonA,
		std::vector<GTRawPoint*>  &resultPoints
		);

	/*****************************************************************************
	* @brief	将多边形折点转点/多点
	* @param	[in]
	*			inputLayer			输入的多边形图层.
	*			resultLayer			结果点图层.
	*			bMultiToSingle		是否将多点打散为单点.
	* @param	[out]
	* @return	点的个数.
	* @note		bMultiToSingle为true时,结果图层几何类型为GT_WKBPoint;bMultiToSingle为false时,结果图层几何类型为GT_WKBMultiPoint.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-29
	*******************************************************************************/
	static int convertPolygonLayerToPointLayerByVertices_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool           bMultiToSingle = false);

	/*****************************************************************************
	* @brief	将多边形折点转点/多点.添加海量数据处理能力.已测,无内存泄漏.
	* @param	[in]
	*			inputLayer			输入的多边形图层.
	*			resultLayer			结果点图层.
	*			bMultiToSingle		是否将多点打散为单点.
	* @param	[out]
	* @return	点的个数.
	* @note		bMultiToSingle为true时,结果图层几何类型为GT_WKBPoint;bMultiToSingle为false时,结果图层几何类型为GT_WKBMultiPoint.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-14
	*******************************************************************************/
	static int convertPolygonLayerToPointLayerByVertices(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool           bMultiToSingle = false,
		const int            blockSize = 200000);
	/*单机多核版本*/
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
	* @brief	求多边形/多多边形的Centroid(或者是每部分的Centroid).
	* @param	[in]
	*			polygon				多边形或多多边形对象.
	*			centroids			结果点容器
	*			isMultiToOne		true:multiPolygon得到的多个点被计算成一个,false:multiPolygon返回多个点
	*			dTolerance			容差值,默认为1e-10.
	* @param	[out]
	* @return	结果Centroid个数.
	* @note		调用GTCPGeometryBasic::ringCentroid()函数实现,同一简单多边形内外环与各个内环按照面积比计算坐标(外环减去内环).
	*			同一多多边形的各个简单部分多边形之间按照面积加权计算平均坐标。
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-10-20
	*******************************************************************************/
	static int polygonCentroid(
		const GTGeometry		 *polygon,
		std::vector<GTRawPoint*> &centroids,
		const bool				 isMultiToOne = true);

	/*****************************************************************************
	* @brief	将多边形Centroid转点/多点
	* @param	[in]
	*			inputLayer			输入的多边形图层.
	*			resultLayer			结果点图层.
	*			bMultiToSingle		是否将多点聚合为单一点.
	* @param	[out]
	* @return	点的个数.
	* @note		结果图层几何类型为GT_Point.
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-10-20
	*******************************************************************************/
	static int convertPolygonLayerToPointLayerByCentroid_old(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool           bMultiToSingle = true);

	/*****************************************************************************
	* @brief	将多边形Centroid转点/多点.添加海量数据处理能力.已测,无内存泄漏.
	* @param	[in]
	*			inputLayer			输入的多边形图层.
	*			resultLayer			结果点图层.
	*			bMultiToSingle		是否将多点聚合为单一点.
	* @param	[out]
	* @return	点的个数.
	* @note		结果图层几何类型为GT_Point.
	* @author	范俊甫,fanjf@lreis.ac.cn,2012-6-13
	*******************************************************************************/
	static int convertPolygonLayerToPointLayerByCentroid(
		GTFeatureLayer		*inputLayer,
		GTFeatureLayer		*resultLayer,
		const bool           bMultiToSingle = true,
		const int            blockSize = 200000);

	/*单机多核版本*/
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
	* @brief	求lineString的长度中点（逐段相加至第一个大于1/2总长度的vertex，再用等比定理求中点坐标）
	* @param	[in]
	*			input_line			线串几何对象.
	*			dTolerance			容差值,当2线段长度之差小于此值时认为二者长度相等.默认为1e-10.
	* @param	[out]
	* @return	线串的长度中点,该点不一定是线串的vertex之一,但是一定在线串上.
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-8-23
	*******************************************************************************/
	static GTRawPoint *getLineStringMiddleLengthPoint(
		GTLineString *input_line,
		const double  dTolerance = 1e-10);

	/*****************************************************************************
	* @brief	判断点是否在多边形内部.
	* @param	[in]
	*			_ptn				点对象.
	*			_polygon			多边形对象
	*			dTolerance			容差值,默认为1e-10.
	* @param	[out]
	* @return	true:点在多边形内部(某个外环内且任意一个内环以外).
	* @note		
	* @author	范俊甫,fanjf@lreis.ac.cn,2011-9-3
	*******************************************************************************/
	static bool isPointWithInPolygon(
		const GTRawPoint *_ptn,
		const GTGeometry *_polygon,
		const double      dTolerance = 1e-10); 

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

};

}
#endif
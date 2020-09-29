/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  Buffer处理类(基于线段Buffer合并)
* Author:   范俊甫, fanjf@lreis.ac.cn 05-11-2011
*
****************************************************************
* Copyright (c)  All rights reserved.
* 版权所有    (c)  保留所有权利
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

	/*以下均将结果存在Ring指针对象或数组中, 需要外部释放, 返回Ring的个数
	 *同时也返回岛的标志, 但是ring中点的顺序没有调整
	 */
	
	/*点的Buffer*/
	static int pointBuffer(
		const GTPoint*         pointObj, 
		double                 dR, 
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	环的Buffer, 返回指针链表, 外部释放，合并策略为：各segmentbuffer
	*			以每相邻2buffer为一组依次合并
	* @param	[in]
	*			ringObj			环对象
	*			dR				buffer半径
	*			bufferType		环buffer类型：GT_BUFFER_OUTER；GT_BUFFER_EDGE；
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	环buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-18
	*******************************************************************************/
	static int ringBuffer(
		const GTRing*          ringObj,
		double                 dR,
		enumGTBufferType	   bufferType,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*线的Buffer, 返回指针链表, 外部释放*/
	static int lineStringBuffer(
		const GTLineString*    lineStringObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	线的Buffer, 返回指针链表, 外部释放:刘衍修改自lineStringBuffer函数，
	*			更改合并各segmentbuffer策略为以每相邻2buffer为一组，依次合并
	* @param	[in]
	*			lineStringObj	线对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	线buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-18
	*******************************************************************************/
	static int lineStringBuffer1(
		const GTLineString*    lineStringObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*多边形的Buffer按照边进行, 返回指针链表, 外部释放*/
	static int polygonBuffer(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多边形的Buffer, 返回指针链表, 外部释放
	* @param	[in]
	*			polygonObj		多边形对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多边形buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-19
	*******************************************************************************/
	static int polygonBuffer1(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*多边形的Buffer按照面进行, 返回指针链表, 外部释放*/
	static int polygonBufferFace(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多边形的Buffe按照面进行, 返回指针链表, 外部释放
	* @param	[in]
	*			polygonObj		多边形对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多边形buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int polygonBufferFace1(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*多点的Buffer, 返回指针链表, 外部释放*/
	static int multiPointBuffer(
		const GTMultiPoint*    mpointObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多点的Buffe, 返回指针链表, 外部释放
	* @param	[in]
	*			mpointObj		多点对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多点buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int multiPointBuffer1(
		const GTMultiPoint*    mpointObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多点的Buffe, 返回指针链表, 外部释放,增加各单点间buffer是
	*			否相交的判断，相交则合并，不相交则不调用合并函数，该函数比
	*			multiPointBuffer1效率高
	* @param	[in]
	*			mpointObj		多点对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多点buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-21
	*******************************************************************************/
	static int multiPointBuffer2(
		const GTMultiPoint*    mpointObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*多线的Buffer, 返回指针链表, 外部释放*/
	static int multiLineStringBuffer(
		const GTMultiLineString* mlineStringObj, 
		double                   dR,
		std::vector<GTRing *>&   ring_list_out,
		std::vector<int>&        hole_list_out);

	/*****************************************************************************
	* @brief	多线的Buffer, 返回指针链表, 外部释放，更改合并各lineStringbuffer策略为
	*			以每相邻2buffer为一组，依次合并
	* @param	[in]
	*			mlineStringObj	多线对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多线buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-18
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

	/*多线的Buffer按照边进行, 返回指针链表, 外部释放*/
	static int multiPolygonBuffer(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多多边形的Buffer, 返回指针链表, 外部释放
	* @param	[in]
	*			mpolygonObj		多多边形对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多多边形buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-19
	*******************************************************************************/
	static int multiPolygonBuffer1(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*多线的Buffer按照面进行, 返回指针链表, 外部释放*/
	static int multiPolygonBufferFace(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多多边形的Buffer按照面进行, 返回指针链表, 外部释放
	* @param	[in]
	*			mpolygonObj		多多边形对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多多边形buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int multiPolygonBufferFace1(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	几何对象的Buffer, 返回指针, 外部释放
	* @param	[in]
	*			pGeoIn			输入的几何对象
	*			dR				buffer半径
	*			polyBufferType	集合对象为多边形或多多边形时，生成buffer的类型
	*			[out]
	*			pGeoOut			输出的几何对象
	* @return	数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-4
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
	 *以下函数_mp表示单机多核算法*
	 ****************************************************/

	/*****************************************************************************
	* @brief	环的Buffer, 返回指针链表, 外部释放，合并策略为：各segmentbuffer以每
	*			相邻2buffer为一组，依次合并,并采用多核并行计算
	* @param	[in]
	*			ringObj			环对象
	*			dR				buffer半径
	*			bufferType		环buffer类型：GT_BUFFER_OUTER；GT_BUFFER_EDGE
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	环buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-18
	*******************************************************************************/
	static int ringBuffer_mp(
		const GTRing*          ringObj,
		double                 dR,
		enumGTBufferType bufferType,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);


	/*线的Buffer, 返回指针链表, 外部释放*/
	static int lineStringBuffer_mp(
		const GTLineString*    lineStringObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);


	/*****************************************************************************
	* @brief	线的Buffer, 返回指针链表, 外部释放:刘衍修改自lineStringBuffer函数，
	*			更改合并各segmentbuffer策略为以每相邻2buffer为一组，依次合并,并采用
	*			多核并行计算
	* @param	[in]
	*			lineStringObj	线对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	线buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-18
	*******************************************************************************/
	static int lineStringBuffer_mp1(
		const GTLineString*    lineStringObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);


	/*多边形的Buffer按照边进行, 返回指针链表, 外部释放*/
	static int polygonBuffer_mp(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多边形的Buffer, 返回指针链表, 外部释放，多核并行计算
	* @param	[in]
	*			polygonObj		多边形对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多边形buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-19
	*******************************************************************************/
	static int polygonBuffer_mp1(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*多边形的Buffer按照面进行, 返回指针链表, 外部释放*/
	static int polygonBufferFace_mp(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多边形的Buffe按照面进行, 返回指针链表, 外部释放,多核并行计算
	* @param	[in]
	*			polygonObj		多边形对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多边形buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int polygonBufferFace_mp1(
		const GTPolygon*       polygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);
	
	/*多点的Buffer, 返回指针链表, 外部释放*/
	static int multiPointBuffer_mp(
		const GTMultiPoint*    mpointObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多点的Buffe, 返回指针链表, 外部释放，多核并行运算
	* @param	[in]
	*			mpointObj		多点对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	点buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-21
	*******************************************************************************/
	static int multiPointBuffer_mp1(
		const GTMultiPoint*    mpointObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);
	
	/*多线的Buffer, 返回指针链表, 外部释放*/
	static int multiLineStringBuffer_mp(
		const GTMultiLineString* mlineStringObj, 
		double                   dR,
		std::vector<GTRing *>&   ring_list_out,
		std::vector<int>&        hole_list_out);

	/*****************************************************************************
	* @brief	多线的Buffer, 返回指针链表, 外部释放，更改合并各lineStringbuffer策略为
	*			以每相邻2buffer为一组，依次合并,采用多核并行计算
	* @param	[in]
	*			mlineStringObj	多线对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多线buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-19
	*******************************************************************************/
	static int multiLineStringBuffer_mp1(
		const GTMultiLineString* mlineStringObj, 
		double                   dR,
		std::vector<GTRing *>&   ring_list_out,
		std::vector<int>&        hole_list_out);

	/*多线的Buffer按照线进行, 返回指针链表, 外部释放*/
	static int multiPolygonBuffer_mp(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多多边形的Buffer, 返回指针链表, 外部释放,采用多核并行计算
	* @param	[in]
	*			GTMultiPolygon	多多边形对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多多边形buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-19
	*******************************************************************************/
	static int multiPolygonBuffer_mp1(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*多线的Buffer按照面进行, 返回指针链表, 外部释放*/
	static int multiPolygonBufferFace_mp(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	多多边形的Buffer按照面进行, 返回指针链表, 外部释放,采用多核并行计算
	* @param	[in]
	*			GTMultiPolygon	多多边形对象
	*			dR				buffer半径
	*			[out]
	*			ring_list_out	buffer中ring对象集合
	*			hole_list_out	对应ring对象岛标志的集合：1:岛；0:非岛
	* @return	多多边形buffer中ring的数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-7-20
	*******************************************************************************/
	static int multiPolygonBufferFace_mp1(
		const GTMultiPolygon*  mpolygonObj, 
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*****************************************************************************
	* @brief	几何对象的Buffer, 返回指针, 外部释放,采用多核并行计算
	* @param	[in]
	*			pGeoIn			输入的几何对象
	*			dR				buffer半径
	*			[out]
	*			pGeoOut			输出的几何对象
	* @return	数量
	* @note		返回的指针链表需要外部释放
	* @author	范俊甫, fanjf@lreis.ac.cn,2011-8-4
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
	/*点Buffer*/
	static int __pointBuffer(
		double  dX, 
		double  dY, 
		double  dR, 
		GTRing* ringObj);

	/*线段Buffer(圆形端点)*/
	static int __segmentBuffer(
		double  dX1, 
		double  dY1,
		double  dX2, 
		double  dY2, 
		double  dR, 
		GTRing* ringObj);

	/*单环的Buffer, 按照边进行*/
	static int __ringBuffer(
		const GTRing*          ringObj,
		double                 dR,
		std::vector<GTRing *>& ring_list_out,
		std::vector<int>&      hole_list_out);

	/*单环的Buffer, 按照面进行*/
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

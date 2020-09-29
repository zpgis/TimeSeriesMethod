/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  二维R树对象类的定义, 封装Guttman和GRASS源码
* Author:   范俊甫, fanjf@lreis.ac.cn 2010年-11月-23日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有  (c) 保留所有权利
****************************************************************/
#pragma once
#ifndef GTSPATIALINDEX_H_INCLUDED
#define GTSPATIALINDEX_H_INCLUDED

#include "gts_port.h"
#include "gt_geometry.h"
#include "../include_p/rt/rtree.h"
#include "assert.h" 

namespace gts{

/*
 * R树索引
 * (1)树中id必须 > 0
 * (2)所有id如果相同, 可以创建, 但搜索结果不正确
 * (3)id可以不连续, 但必须不相同
 * (4)如果id不连续, 有可能会影响搜索效率
 * (5)可以将输入的id设为0或者null, 由程序自身维护自动增长的id
 */
class GTS_DLL GTSpatialIndex
{
public:
	GTSpatialIndex(void);
	~GTSpatialIndex(void);

	/*删除RTree, 同时释放内存, 需要重新初始化才能使用*/
	void deleteRTreeIndex();

	/*索引结构是否为空*/
	bool isEmpty() const;

	/*获取矩形对象个数*/
	int getRectCount() const;

	/*初始化RTree索引*/
	bool initRTreeIndex();

	/*
	 *插入Envelop对象和id, 若果id<=0, 将使用自动增长
     *插入之前需要确保Envelop有效, 程序不进行判断
	 */
	 bool addRect(
		const GTEnvelop& envelopObj, 
		void            *objID);

	/*
	 *插入Envelop数组和id数组
	 *此时envelopList和idList为长度rectsCount的数组
	 *如果idList为空, 将使用自动增长 
     *插入之前需要确保Envelop有效, 程序不进行判断
	 */
	bool addRects(
		const GTEnvelop *envelopList, 
		const void     **idList, 
		int              rectsCount);

	/*动态删除Envelop对象, id与envelopObj必须对应*/
	bool deleteRect(
		const GTEnvelop &envelopObj, 
		void            *objID);

	/*以矩形空间覆盖为基础, 搜索所有满足条件的对象
	 *-1表明数据和内存有问题, 其余为对象个数
	 *如果**idList=0, 则不返回id
	 *输入idList必须为0, 返回后调用GTS_FREE释放
	 *推荐用法: 
	 *int *ilist = 0;
	 *spatialSearch(, &ilist);
	 *GTS_FREE(ilist);
	 */
	int spatialSearch(
		const GTEnvelop *envelopObj, 
		void          ***idList);

	/*单点搜索, 返回标识号*/
	void* spatialSearch(
		double x, 
		double y);

private:
	struct Node *root;
	int          rectCount;
	long         rectIdx;

	int spatial_search_1(const GTEnvelop *enObj, std::vector<void*>& obj_list);
	int spatial_search_2(struct Node *pNode, struct Rect *pRect, std::vector<void*>& obj_list);
	int spatial_search_3(struct Node *pNode, double x, double y, void **fid);

	bool point_in_rect        (double& x, double& y, const Rect* rectObj);
	bool rect_rect_disjoin    (const Rect* rectObj1, const Rect* rectObj2);
	bool circle_rect_intersect(double& cen_x, double& cen_y, double& radii, const Rect* rectObj);
	double twoPointsDistance  (const double& x1, const double& y1, const double& x2, const double& y2);

	int __RTreeOverlap(struct Rect *R, struct Rect *S);
};

}
#endif

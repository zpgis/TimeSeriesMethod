/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  二维Quad树对象类的定义, 参考OGR CPLQuadTree
* Author:   范俊甫, fanjf@lreis.ac.cn 2011年-06月-10日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有  (c) 保留所有权利
****************************************************************/
#pragma once
#ifndef GTQUADTREEINDEX_H_INCLUDED
#define GTQUADTREEINDEX_H_INCLUDED

#include "gts_port.h"
#include "gt_geometry.h"

#define GTS_QUADTREE_MAX_DEFAULT_TREE_DEPTH 12
#define GTS_QUADTREE_MAX_SUBNODES 4
#define GTS_QUADTREE_DEFAULT_SPLIT_RATIO  0.5

namespace gts{

class GTS_DLL GTQuadTreeIndex;

struct GTQuadTreeNode{
	GTEnvelop       rect;       /*叶子节点范围*/
	int             nFeatures;  /*对象个数*/
	GTEnvelop      *pahFeatures;/*对象实体范围*/
	int            *pahId;      /*对象id*/
	void           *pData;      /*对象实体数据*/
	int             nNumSubNodes;
	GTQuadTreeNode *apSubNode[GTS_QUADTREE_MAX_SUBNODES];
    int             nDepth;     /*节点的深度, 根节点为0*/
};

struct GTQuadTree
{
	GTQuadTreeNode  *psRoot;
	int              nFeatures;
	int              nMaxDepth;
	int              nBucketCapacity;
	double           dfSplitRatio;
    int              nCurrentDepth;
};

/*区域四叉树定义类, 次序为LD->RD->LU->RU*/
class GTS_DLL GTQuadTreeIndex
{
public:
	GTQuadTreeIndex(void);
	~GTQuadTreeIndex(void);

    /*初始化四叉树, 制定整个区域的范围*/
	bool initQuadTree(const GTEnvelop* pGlobalBounds);
    
    /*四叉树是否有效*/
    bool isValid() const;

    /*清除四叉树*/
	void destroyQuadTree();
	
    /*设置分割的最大深度*/
	void quadTreeSetMaxDepth(int nMaxDepth);
    
    /*插入一个区域, 可以用于矢量或者是栅格对象*/
	void quadTreeInsert(GTEnvelop* hBound, int objID);
    
    /*获取最大允许深度*/
    int getMaxDepth();

    /*获取当前深度*/
    int getCurrentDepth();
    
    /*获取空间范围*/
    const GTEnvelop *getSpatialExtend();

    /*按照矩形范围搜索四叉树, 返回对象个数和id列表
     *nSearchMode表明不同的搜索模式
     *0-深度和广度混合搜索模式, 即搜有所有与目标区域相交的对象(通常用于矢量)
     *1-指定深度的搜索模式, 此时nSearchDepth不能为空(通常用于栅格)
     */
    int quadTreeSearch(
		const GTEnvelop* pAoi,
		std::vector<int>& idList,
        int  nSearchMode        = 0,
        const int* nSearchDepth = NULL);

private:
	GTQuadTreeNode *quadTreeNodeCreate(const GTEnvelop* pRect);

	bool quadTreeCreate(const GTEnvelop* pGlobalBounds);

	void quadTreeNodeDestroy(GTQuadTreeNode *psNode);

	void quadTreeDestroy(GTQuadTree *pQuadtree);

	int quadTreeGetAdvisedMaxDepth(int nExpectedFeatures);

	void quadTreeAddFeatureInternal(
		const GTEnvelop *pRect, int objID);

	void quadTreeNodeAddFeatureAlg1(
		GTQuadTreeNode *psNode,
		const GTEnvelop* pRect, int objID);

	void quadTreeNodeAddFeatureAlg2(
		GTQuadTreeNode *psNode,
		const GTEnvelop* pRect, 
		int objID,
		int nMaxDepth);

	void quadTreeSplitBounds( 
		double dfSplitRatio,
		const GTEnvelop *inEnv,
		GTEnvelop *outEnv1,
		GTEnvelop *outEnv2);

    void quadTreeCollectObjects1(
		const GTQuadTreeNode *psNode,
        const GTEnvelop* pAoi,
        std::vector<int>& idList);

	void quadTreeCollectObjects2(
		const GTQuadTreeNode *psNode,
        const GTEnvelop* pAoi,
        std::vector<int>& idList,
        const int* nSearchDepth);

	inline int _rectContained(const GTEnvelop *a, const GTEnvelop *b)
	{
		if(a->minX >= b->minX && a->maxX <= b->maxX)
		{
			if(a->minY >= b->minY && a->maxY <= b->maxY)
			{
				return true;
			}
		}
		return false;  
	}

	inline int _rectOverlap(const GTEnvelop *a, const GTEnvelop *b)
	{
		if(a->minX > b->maxX) return false;
		if(a->maxX < b->minX) return false;
		if(a->minY > b->maxY) return false;
		if(a->maxY < b->minY) return false;
		return true;
	}

private:
	GTQuadTree *hQuadTree;
};

}
#endif

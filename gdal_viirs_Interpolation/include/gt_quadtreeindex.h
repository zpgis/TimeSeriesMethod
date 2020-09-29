/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ��άQuad��������Ķ���, �ο�OGR CPLQuadTree
* Author:   ������, fanjf@lreis.ac.cn 2011��-06��-10��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����  (c) ��������Ȩ��
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
	GTEnvelop       rect;       /*Ҷ�ӽڵ㷶Χ*/
	int             nFeatures;  /*�������*/
	GTEnvelop      *pahFeatures;/*����ʵ�巶Χ*/
	int            *pahId;      /*����id*/
	void           *pData;      /*����ʵ������*/
	int             nNumSubNodes;
	GTQuadTreeNode *apSubNode[GTS_QUADTREE_MAX_SUBNODES];
    int             nDepth;     /*�ڵ�����, ���ڵ�Ϊ0*/
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

/*�����Ĳ���������, ����ΪLD->RD->LU->RU*/
class GTS_DLL GTQuadTreeIndex
{
public:
	GTQuadTreeIndex(void);
	~GTQuadTreeIndex(void);

    /*��ʼ���Ĳ���, �ƶ���������ķ�Χ*/
	bool initQuadTree(const GTEnvelop* pGlobalBounds);
    
    /*�Ĳ����Ƿ���Ч*/
    bool isValid() const;

    /*����Ĳ���*/
	void destroyQuadTree();
	
    /*���÷ָ��������*/
	void quadTreeSetMaxDepth(int nMaxDepth);
    
    /*����һ������, ��������ʸ��������դ�����*/
	void quadTreeInsert(GTEnvelop* hBound, int objID);
    
    /*��ȡ����������*/
    int getMaxDepth();

    /*��ȡ��ǰ���*/
    int getCurrentDepth();
    
    /*��ȡ�ռ䷶Χ*/
    const GTEnvelop *getSpatialExtend();

    /*���վ��η�Χ�����Ĳ���, ���ض��������id�б�
     *nSearchMode������ͬ������ģʽ
     *0-��Ⱥ͹�Ȼ������ģʽ, ������������Ŀ�������ཻ�Ķ���(ͨ������ʸ��)
     *1-ָ����ȵ�����ģʽ, ��ʱnSearchDepth����Ϊ��(ͨ������դ��)
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

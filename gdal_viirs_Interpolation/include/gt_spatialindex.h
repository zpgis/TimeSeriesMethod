/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ��άR��������Ķ���, ��װGuttman��GRASSԴ��
* Author:   ������, fanjf@lreis.ac.cn 2010��-11��-23��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����  (c) ��������Ȩ��
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
 * R������
 * (1)����id���� > 0
 * (2)����id�����ͬ, ���Դ���, �������������ȷ
 * (3)id���Բ�����, �����벻��ͬ
 * (4)���id������, �п��ܻ�Ӱ������Ч��
 * (5)���Խ������id��Ϊ0����null, �ɳ�������ά���Զ�������id
 */
class GTS_DLL GTSpatialIndex
{
public:
	GTSpatialIndex(void);
	~GTSpatialIndex(void);

	/*ɾ��RTree, ͬʱ�ͷ��ڴ�, ��Ҫ���³�ʼ������ʹ��*/
	void deleteRTreeIndex();

	/*�����ṹ�Ƿ�Ϊ��*/
	bool isEmpty() const;

	/*��ȡ���ζ������*/
	int getRectCount() const;

	/*��ʼ��RTree����*/
	bool initRTreeIndex();

	/*
	 *����Envelop�����id, ����id<=0, ��ʹ���Զ�����
     *����֮ǰ��Ҫȷ��Envelop��Ч, ���򲻽����ж�
	 */
	 bool addRect(
		const GTEnvelop& envelopObj, 
		void            *objID);

	/*
	 *����Envelop�����id����
	 *��ʱenvelopList��idListΪ����rectsCount������
	 *���idListΪ��, ��ʹ���Զ����� 
     *����֮ǰ��Ҫȷ��Envelop��Ч, ���򲻽����ж�
	 */
	bool addRects(
		const GTEnvelop *envelopList, 
		const void     **idList, 
		int              rectsCount);

	/*��̬ɾ��Envelop����, id��envelopObj�����Ӧ*/
	bool deleteRect(
		const GTEnvelop &envelopObj, 
		void            *objID);

	/*�Ծ��οռ串��Ϊ����, �����������������Ķ���
	 *-1�������ݺ��ڴ�������, ����Ϊ�������
	 *���**idList=0, �򲻷���id
	 *����idList����Ϊ0, ���غ����GTS_FREE�ͷ�
	 *�Ƽ��÷�: 
	 *int *ilist = 0;
	 *spatialSearch(, &ilist);
	 *GTS_FREE(ilist);
	 */
	int spatialSearch(
		const GTEnvelop *envelopObj, 
		void          ***idList);

	/*��������, ���ر�ʶ��*/
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

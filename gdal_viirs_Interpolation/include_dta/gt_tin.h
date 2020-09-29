/****************************************************************
* Project:  ������Ϣ�����߼�
* Purpose:  TIN����������
* Author:   ������, fanjf@lreis.ac.cn, 2011��-05��-07��
*
****************************************************************
* Copyright (c)
* ��Ȩ����   (c)
****************************************************************/
#ifndef GTTIN_H_INCLUDED
#define GTTIN_H_INCLUDED

#include "gts_dta_port.h"
#include "gt_datasource.h"

namespace gts{

struct GTTinEdge;
struct GTTinVertex
{
	double x, y, z;
	struct GTTinEdge *entry_pt;
};

struct GTTinEdge
{
	struct GTTinVertex *org;
	struct GTTinVertex *dest;
	struct GTTinEdge   *onext;
	struct GTTinEdge   *oprev;
	struct GTTinEdge   *dnext;
	struct GTTinEdge   *dprev;
};

class GTS_DLL GTTin
{
public:
	GTTin(void);
	~GTTin(void);

	/** @brief  TIN����DEM
	 *  @return �����Ƿ�ɹ�
	 *  @note   
	 */
	static bool executeDEMGenerate(
		const char* pszFormatTo,            /*Ŀ������Դ��ʽ*/
		const char* pszSourceTo,            /*Ŀ������Դ·��*/
		GTTinLayer* pTinLayer,              /*����TIN���ָ��*/
		double      dResolution);           /*�ռ�ֱ���*/
};

class GTS_DLL GTTinGenerator
{
public:
	GTTinGenerator();
	~GTTinGenerator(void);
	void clearAll();
	static void destroyTinGenerator(GTTinGenerator *pTinGenerator);

	bool buildTinFromPoints(
		std::vector<GTPoint *>& nodeList,
		const int               nodeNum);
	
	/*�����ݵ�����һ��TING-polygon��*/
	bool createTinLayer(GTTinLayer *pLayer);

	bool getTinFeatureClass(GTTinLayer *pLayer, GTFeatureClass& clss);

	/*�ȸ�������TIN*/
	bool buildTinFromLineStrings(
		std::vector<GTLineString *>& lineList,
		const int					 lineNum);

	bool buildTinFromContours(
		std::vector<GTLineString *>& counterList,
		std::vector<double>&		 counterHighlist,
		const int					 counterNum,
		std::vector<GTLineString *>& breakLine,
		std::vector<double>&		 breakLineHighlist,
		GTPolygon *					 border = NULL);

	bool getAllTin(
		std::vector<GTRing>& ring_list,
		std::vector<std::vector<double> >& high_list);

	bool createTinLayer1(GTTinLayer *pLayer);

private:
	/*�������ݺ���*/
	int read_sites(
		std::vector<GTPoint *>& nodeList,
		const int               nodeNum);

	/*���������ڴ洦����*/
	bool alloc_memory(unsigned int n);
	void free_memory();
	struct GTTinEdge *get_edge();
	void free_edge    (struct GTTinEdge *e);
	bool alloc_sites  (unsigned int n);
	bool realloc_sites(unsigned int n);
	bool alloc_edges  (unsigned int n);

	/*�ߵĴ�����*/
	struct GTTinEdge *join(
		struct GTTinEdge   *e1, 
		struct GTTinVertex *v1, 
		struct GTTinEdge   *e2, 
		struct GTTinVertex *v2, 
		int                 side);
	
	void delete_edge(struct GTTinEdge *e);

	void splice(
		struct GTTinEdge   *a, 
		struct GTTinEdge   *b, 
		struct GTTinVertex *v);

	struct GTTinEdge *create_edge(
		struct GTTinVertex *v1, 
		struct GTTinVertex *v2);

	void find_lowest_cross_edge(
		struct GTTinEdge    *r_cw_l, 
		struct GTTinVertex  *s,
		struct GTTinEdge    *l_ccw_r, 
		struct GTTinVertex  *u,
		struct GTTinEdge   **l_lower,
		struct GTTinVertex **org_l_lower,
		struct GTTinEdge   **r_lower,
		struct GTTinVertex **org_r_lower);

	void merge(
		struct GTTinEdge   *r_cw_l, 
		struct GTTinVertex *s,
		struct GTTinEdge   *l_ccw_r, 
		struct GTTinVertex *u,
		struct GTTinEdge  **l_tangent);

	void remove_duplicates(unsigned int *size);
	static int cmp(const void *a, const void *b);

	void divide(
		unsigned int l, 
		unsigned int r,
		struct GTTinEdge **l_ccw, 
		struct GTTinEdge **r_cw);

private:
	struct GTTinVertex *sites;
	struct GTTinEdge   *edges; 
	struct GTTinEdge  **free_list_e;
	unsigned int        n_free_e;
	unsigned int        n_site_num;

	bool b_success;

	std::vector<GTRing>					tin_ring_list;
	std::vector<std::vector<double> >	tin_high_list;
	/*Euler's formula
	 *Vertex - Edge + Face = 2
	 *Egde <= 3*Vertex - 6
	 */
};

}
#endif

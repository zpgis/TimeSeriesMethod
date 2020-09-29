/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ���ཻ�ֽ�
* Author:   ������, fanjf@lreis.ac.cn 11-01-2006
* modify:	������, fanjf@lreis.ac.cn 8-29-2011
*
****************************************************************
* Copyright (c)  All rights reserved.
* ��Ȩ����    (c)  ��������Ȩ��
****************************************************************/
#ifndef GT_GEOMETRYDECOMP_H_INCLUDE
#define GT_GEOMETRYDECOMP_H_INCLUDE

#include "gts_cg_port.h"
#include "gt_atdList.h"

namespace gts{

	class GTGeometryDecomp{
		typedef struct PD_Point{
			double x;
			double y;
			int    id;    //* ����id
			int    id1;
			int    id2;
		};
		typedef struct PD_Tuple{
			double x;
			double y;
		};
		typedef struct PD_Segment{
			PD_Tuple sta_pt;
			PD_Tuple end_pt;
		};
		typedef struct PD_Polygon{
			int              v_num;
			std::vector<PD_Point> v_list;
		};

		/*���ڼ�¼�����,��Ϊ˫������ڵ������*/
		class PD_Vertex {
			public:
				PD_Vertex(void)
					:pro_num(0){}
					~PD_Vertex(void){}
				PD_Vertex(const double& _x, const double& _y)
					:x(_x),y(_y),pro_num(0){}
				PD_Vertex(const PD_Vertex& _vertex)
				{
					this->x        = _vertex.x;
					this->y        = _vertex.y;
					this->type     = _vertex.type;
					this->pro_num  = _vertex.pro_num;
					this->id       = _vertex.id;
					this->id1      = _vertex.id1;
					this->id2      = _vertex.id2;
				}
				PD_Vertex& operator=(const PD_Vertex& _vertex)
				{
					this->x        = _vertex.x;
					this->y        = _vertex.y;
					this->type     = _vertex.type;
					this->pro_num  = _vertex.pro_num;
					this->id       = _vertex.id;
					this->id1      = _vertex.id1;
					this->id2      = _vertex.id2;
					return *this;
				}
				bool operator == (const PD_Vertex& _vertex)
				{
					//return (this->x == _vertex.x)&&(this->y == _vertex.y);
					//����id�ж��Ƿ����
					return (this->id == _vertex.id);
				}
			public:
				double x, y;     // *����ֵ
				bool   type;     // *trueΪ����, falseΪ����
				int    pro_num;  // *����Ĵ��� 
				int    id;       // *�����id
				int    id1;      // *����id
				int    id2;      // *����id
		};


	public:
		GTGeometryDecomp(void):origi_v_num(0), inter_v_num(0){}
		~GTGeometryDecomp(void){v_list.Clear();v_set.clear();}
		void ClearAll();
		//���е��ηֽ�
		int  PolygonDecompose(std::vector<GTRing *>& _poly_list);

		void InitVertexList(const std::vector<GTRawPoint>& _vlist, const int& _count);

		int  Decompose(std::vector<GTRing *>& _poly_list);
	private:
		//�жϵ��Ƿ���ԭʼ����ε��ڲ�
		int  PointInsidePolygon(const double& _x, const double& _y)const;
		//�������ཻ����ж�, �������ཻ��ĸ���
		int  DetectSelfIntersections();
		//���������߶εĽ���
		int  SegmentsIntersecting(const GTLine& _seg1,const GTLine& _seg2, GTRawPoint& _pt0, GTRawPoint& _pt1);
		//�ж������߶��Ƿ����
		bool IsAdjanceEdges(const int& _id1,const int& _id2)const;
		//�������ε�����
		GTRawPoint TriangleCentroid(const GTRawPoint& _p0,const GTRawPoint& _p1,const GTRawPoint& _p2)const;
		//Ѱ��ƽ����
		GTRawPoint FindTrivialPoint(const GTRawPoint& _p0,const GTRawPoint& _p1,const GTRawPoint& _p2)const;
		//�жϵ��Ƿ��ڸ����������ε��ڲ�
		int  PointInsideTriangle(const GTRawPoint& _p0,const GTRawPoint& _p1,const GTRawPoint& _p2, const GTRawPoint& _p) const;


		GTATDList<PD_Vertex>    v_list;//���ڷֽ��˫��ѭ������
		std::vector<PD_Point>   v_set; //���ԭʼ�ڵ�����򼯺�
		int origi_v_num;
		int inter_v_num;
	};
}
#endif
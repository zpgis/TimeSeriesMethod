/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  自相交分解
* Author:   范俊甫, fanjf@lreis.ac.cn 11-01-2006
* modify:	范俊甫, fanjf@lreis.ac.cn 8-29-2011
*
****************************************************************
* Copyright (c)  All rights reserved.
* 版权所有    (c)  保留所有权利
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
			int    id;    //* 自身id
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

		/*用于记录点的类,作为双向链表节点的数据*/
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
					//根据id判断是否相等
					return (this->id == _vertex.id);
				}
			public:
				double x, y;     // *坐标值
				bool   type;     // *true为顶点, false为交点
				int    pro_num;  // *处理的次数 
				int    id;       // *本身的id
				int    id1;      // *交点id
				int    id2;      // *交点id
		};


	public:
		GTGeometryDecomp(void):origi_v_num(0), inter_v_num(0){}
		~GTGeometryDecomp(void){v_list.Clear();v_set.clear();}
		void ClearAll();
		//进行单次分解
		int  PolygonDecompose(std::vector<GTRing *>& _poly_list);

		void InitVertexList(const std::vector<GTRawPoint>& _vlist, const int& _count);

		int  Decompose(std::vector<GTRing *>& _poly_list);
	private:
		//判断点是否在原始多边形的内部
		int  PointInsidePolygon(const double& _x, const double& _y)const;
		//进行自相交点的判断, 返回自相交点的个数
		int  DetectSelfIntersections();
		//计算两条线段的交点
		int  SegmentsIntersecting(const GTLine& _seg1,const GTLine& _seg2, GTRawPoint& _pt0, GTRawPoint& _pt1);
		//判断两个线段是否相接
		bool IsAdjanceEdges(const int& _id1,const int& _id2)const;
		//求三角形的重心
		GTRawPoint TriangleCentroid(const GTRawPoint& _p0,const GTRawPoint& _p1,const GTRawPoint& _p2)const;
		//寻找平凡点
		GTRawPoint FindTrivialPoint(const GTRawPoint& _p0,const GTRawPoint& _p1,const GTRawPoint& _p2)const;
		//判断点是否在给定三角形形的内部
		int  PointInsideTriangle(const GTRawPoint& _p0,const GTRawPoint& _p1,const GTRawPoint& _p2, const GTRawPoint& _p) const;


		GTATDList<PD_Vertex>    v_list;//用于分解的双向循环链表
		std::vector<PD_Point>   v_set; //存放原始节点的有序集合
		int origi_v_num;
		int inter_v_num;
	};
}
#endif
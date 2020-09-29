#ifndef GTKDTREE_H_INCLUDED
#define GTKDTREE_H_INCLUDED

#include <vector>
#include <algorithm>
#include <limits>
#include "gt_spatialindex.h"
#include "gt_geometry.h"

struct KDPoint{
	KDPoint(){}
	KDPoint(const KDPoint& point)
	{
		this->X = point.X;
		this->Y = point.Y;
		this->Id = point.Id;
	}
	KDPoint(double x,double y,long id)
	{
		this->Id = id;
		this->X = x;
		this->Y = y;
	}
	bool operator == (const KDPoint& rawPointObj) const {
		return (X == rawPointObj.X) && (Y == rawPointObj.Y);
	}

	double X,Y;
	long Id;
};

struct GTKdNodeP{
	KDPoint _point;
	GTKdNodeP *_left, *_right;
	bool _is_leaf;
	GTKdNodeP(const KDPoint& point)
	{
		this->_point   = point;
		this->_left    = 0;
		this->_right   = 0;
		this->_is_leaf = false;
	}
	GTKdNodeP(){}
};

//All data is restored in leaf nodes. 
//!!!Overlaied (same) points are not acceptable!!!
namespace gts
{
	class GTS_DLL GTKdTreeP{//: public ATSpatialIndex{
		struct PointSortX{
			bool operator () (const KDPoint& p1, const KDPoint &p2)
			{
				if (p1.X != p2.X) 
					return (p1.X < p2.X);
				else 
					return (p1.Y < p2.Y);
				return true;
			}
		};
		struct PointSortY{
			bool operator () (const KDPoint& p1, const KDPoint &p2)
			{
				if (p1.Y != p2.Y) 
					return (p1.Y < p2.Y);
				else 
					return (p1.X < p2.X);
				return true;
			}
		};
		struct Range{
			double x0, y0, x1, y1;
			Range(const double& x0, const double& y0, const double& x1, const double& y1)
			{
				this->x0 = x0; this->y0 = y0;
				this->x1 = x1; this->y1 = y1;
			}
			Range(void){}
		};

	public:
		GTKdTreeP();
		~GTKdTreeP();
		bool BuildKdTree(const std::vector<KDPoint>& point_list, const int& point_num);
		int  GetNodeNumber() const;
		int  GetLeafNumber() const;
		bool IsValidKdTree() const;
		void DestroyKdTree();
		std::vector<KDPoint> SearchByRect(const double& x0, const double& y0, const double& x1, const double& y1);
		int SearchByRect(const double& x0, const double& y0, const double& x1, const double& y1, std::vector<bool>& sel_list);

	protected:
		GTKdNodeP* _root;
		int _node_num;
		int _leaf_num;
		bool _b_equal;
		void DeleteKdTree(GTKdNodeP* node);
		GTKdNodeP* BuildKdTree1(const std::vector<KDPoint>& point_list, const int& point_num);
		GTKdNodeP* BuildKdTree2(std::vector<KDPoint> &xsorted, std::vector<KDPoint> &ysorted, int depth);
		inline bool lessthanx(const KDPoint& p1, const KDPoint& p2)
		{
			if(p1.X == p2.X && p1.Y == p2.Y)
			{
				_b_equal = !_b_equal;
				return _b_equal;
			}
			if(p1.X != p2.X) return (p1.X < p2.X);
			else return (p1.Y< p2.Y);
		}    
		inline bool lessthany(KDPoint& p1, KDPoint& p2)
		{
			if(p1.X == p2.X && p1.Y == p2.Y)
			{
				_b_equal = !_b_equal;
				return _b_equal;
			}
			if(p1.Y != p2.Y) return (p1.Y < p2.Y);
			else return (p1.X < p2.X);
		}
		inline bool pointinregion(const KDPoint& p, const Range& r)
		{
			if(p.X >= r.x0 && p.X <= r.x1 && 
				p.Y >= r.y0 && p.Y <= r.y1) return true;
			else return false;
		}
		inline bool isfullycontained(const Range& small1, const Range& big1)
		{
			if(small1.x0 >= big1.x0 && small1.y0 >= big1.y0 && 
				small1.x1 <= big1.x1 && small1.y1 <= big1.y1) return true;
			else return false;
		}
		inline bool intersects(const Range& r0, const Range& r1)
		{
			if (r0.x0 > r1.x1)      return false;
			else if (r1.x0 > r0.x1) return false;
			else if (r0.y0 > r1.y1) return false;
			else if (r1.y0 > r0.y1) return false;
			else return true;
		}
		void searchkdtree (GTKdNodeP *v, Range R, std::vector<KDPoint>& point_list);
		void searchtree   (GTKdNodeP *v, const Range &R, int depth, const Range &regionv, std::vector<KDPoint>& point_list);
		void reportsubtree(GTKdNodeP *v, std::vector<KDPoint>& point_list);
	};

}

#endif
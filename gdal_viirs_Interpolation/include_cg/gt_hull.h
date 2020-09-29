/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  2D离散点的凸包生成
* Author:   范俊甫, fanjf@lreis.ac.cn 05-11-2011
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c)  保留所有权利
****************************************************************/
#ifndef GTSHULL_H_INCLUDED
#define GTSHULL_H_INCLUDED

#include "gts_cg_port.h"
#include "gt_geometry.h"
#include <algorithm>
#include <math.h>
#include <vector>

namespace gts{

class GTS_DLL GTHull
{
public:
	GTHull(void);
	~GTHull(void);
	/* 2D离散点的凸包
	 * 结果在pHullRing, pHullRing为用户分配
	 */
	static bool create2DHull(
		std::vector<GTRawPoint *>& pointList, 
		int     pointNum, 
		GTRing* pHullRing);

	/* 2D离散点的凸包
	 * 结果在pHullRing, pHullRing为用户分配
	 */
	static bool create2DHull(
		std::vector<GTPoint *>& pointList, 
		int     pointNum, 
		GTRing* pHullRing);

	/* 2D离散点的凸包, Andrew's monotone chain 2D convex hull algorithm
	 * 结果在pHullRing, pHullRing为用户分配
	 */
	static bool create2DHull_AM(
		std::vector<GTRawPoint>& pointList, 
		int     pointNum, 
		GTRing* pHullRing);

private:
	inline static int cmpPoints(const GTRawPoint &v1, const GTRawPoint &v2)
	{
		if      (v1.X > v2.X)  return 0;
		else if (v1.X < v2.X)  return 1;
		else if (v1.Y > v2.Y)  return 0;
		else if(v1.X == v2.X && v1.Y == v2.Y) return 0;
		return 1;
	}
	static int cmpPoints1(const GTRawPoint *v1, const GTRawPoint *v2);
	static int cmpPoints2(const GTPoint *v1, const GTPoint *v2);
	static int rightTurn1(std::vector<GTRawPoint *>& pointList, int i, int j, int k);
	static int rightTurn2(std::vector<GTPoint *>& pointList, int i, int j, int k);

	inline static double
	isLeft(GTRawPoint& P0, GTRawPoint& P1, GTRawPoint& P2)
	{
		return (P1.X - P0.X)*(P2.Y - P0.Y) - (P2.X - P0.X)*(P1.Y - P0.Y);
	}
};

}
#endif
/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  生成平行线
* Author:   范俊甫, fanjf@lreis.ac.cn;范俊甫, fanjf@lreis.ac.cn;2011-09-06
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c) 保留所有权利
****************************************************************/
#ifndef GT_GEOMETRYPARALLEL_H_INCLUDED
#define GT_GEOMETRYPARALLEL_H_INCLUDED

#include "gts_cg_port.h"

namespace gts{

class GTS_DLL GTGeometryParallel
{
	public:
		GTGeometryParallel(void);
		~GTGeometryParallel(void);
		/*****************************************************************************
		* @brief	生成平行线
		* @param	[in]
		*			Points			线对象
		*			da				长半轴半径
		*			db				短半轴半径
		*			dalpha			
		*			side			>=0：左侧；<0：右侧
		*			round			1：圆角末端；0：尖末端
		*			caps			拐角处是否平滑
		*			looped			是否将生成的平行线首尾相连
		*			tol				
		*			[out]
		*			nPoints			输出的平行线对象
		* @return
		* @note		
		* @author	范俊甫, fanjf@lreis.ac.cn,2011-09-06
		*******************************************************************************/
		static void parallel_line(GTLineString *Points, double da, double db,
					double dalpha, int side, int round, int caps, int looped,
					double tol, GTLineString *nPoints);

	private:

	 static void rotate_vector(double x, double y, 
						double cosa, double sina,
						double *nx, double *ny);

	 static void norm_vector(double x1, double y1, 
					  double x2, double y2, 
					  double *x, double *y);

	 static void elliptic_tangent(double x, double y, double da, double db,
			     double dalpha, double *px, double *py);

	 static double angular_tolerance(double tol, double da, double db);

	 static void line_coefficients(double x1, double y1, double x2, double y2,
							double *a, double *b, double *c);

	 static void elliptic_transform(double x, double y, double da, double db,
		 double dalpha, double *nx, double *ny);

	 static int line_intersection(double a1, double b1, double c1, double a2,
		 double b2, double c2, double *x, double *y);

	 static int Vect_line_prune(GTLineString *lineStringObj);

	};
}
#endif

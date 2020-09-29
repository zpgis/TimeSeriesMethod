/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ����ƽ����
* Author:   ������, fanjf@lreis.ac.cn;������, fanjf@lreis.ac.cn;2011-09-06
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c) ��������Ȩ��
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
		* @brief	����ƽ����
		* @param	[in]
		*			Points			�߶���
		*			da				������뾶
		*			db				�̰���뾶
		*			dalpha			
		*			side			>=0����ࣻ<0���Ҳ�
		*			round			1��Բ��ĩ�ˣ�0����ĩ��
		*			caps			�սǴ��Ƿ�ƽ��
		*			looped			�Ƿ����ɵ�ƽ������β����
		*			tol				
		*			[out]
		*			nPoints			�����ƽ���߶���
		* @return
		* @note		
		* @author	������, fanjf@lreis.ac.cn,2011-09-06
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

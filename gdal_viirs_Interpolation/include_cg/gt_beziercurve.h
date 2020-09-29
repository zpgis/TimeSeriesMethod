/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  bezier曲线及平滑
* 
* Author:   范俊甫, fanjf@lreis.ac.cn,2011-07-26
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c) 保留所有权利
****************************************************************/

#ifndef GT_BEZIERCURVE_H_INCLUDED
#define GT_BEZIERCURVE_H_INCLUDED

#include "gt_geometry.h"

namespace gts
{
	class GTS_DLL GTBezierCurve
	{
	public:
		GTBezierCurve ();
		~GTBezierCurve();


		/*****************************************************************************
		* @brief	通过平滑已有线构建bezier曲线(bezier algorithm)
		* @param	[in]
		*			pLinestringObj  线对象(或者有序点数组)
		*           pt_step			插值步长，贝塞尔公式维度为pt_step-1,pt_step应大于2小于33；
		*			point_on_curve	平滑线上点数
		*			[out]
		* @return	GTLineString	平滑后线
		* @note		返回值需要外部释放,pt_step不能超过33,默认为3次贝塞尔曲线;点个数point_count必须大于贝塞尔维度
		* @author	周玉科，zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		GTLineString*  makeBezierCurve(
			GTLineString *pLinestringObj, 
			const int     point_on_curve, 
			const int     pt_step = 4);
 
		/*****************************************************************************
		* @brief	通过有序点数组构建bezier曲线(bezier algorithm)
		* @param	[in]
		*			point_list      有序点数组
		*           pt_step			插值步长，贝塞尔公式维度为pt_step-1,pt_step应大于2小于33；
		*			point_on_curve	平滑线上点数
		*			[out]
		* @return	GTLineString	平滑后线
		* @note		返回值需要外部释放;pt_step不能超过33,默认为3次贝塞尔曲线;点个数point_count必须大于贝塞尔维度
		* @author	周玉科，zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		GTLineString*  makeBezierCurve(
			GTRawPoint *point_list, 
			const int  point_count, 
			const int  point_on_curve, 
			const int  pt_step);
		
	private:

		/*****************************************************************************
		* @brief	返回第n个因数
		* @param	[in]
		*			n	线对象
		*			[out]
		*			
		* @return	返回第n个因数
		* @note		n必须大于0,小于33(因为因数数组长度设置为33)
		* @author	周玉科，zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		double    factorial(int n);

		/*****************************************************************************
		* @brief	返回Ni
		* @param	[in]
		*			factorialLookup	贝塞尔多项式系数列表
		* @note		必须用createFactorialTable初始化该系数列表,注意释放
		* @author	周玉科，zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		double*   factorialLookup;
		/* 初始化系数列表 */
		void	  createFactorialTable();

		/*****************************************************************************
		* @brief	返回Ni
		* @param	[in]
		*			n	
		*           i
		*			[out]
		*			
		* @return	返回因数
		* @note		n必须大等于i
		* @author	周玉科，zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		double    Ni(int n, int i);

		/*****************************************************************************
		* @brief	伯恩斯坦底函数
		* @param	[in]
		*			n	factor因数索引
		*           i	
		*			[out]
		*
		* @return	返回函数值
		* @note		n必须大等于i
		* @author	周玉科，zyk@lreis.ac.cn,2011-07-26
		*******************************************************************************/
		double    bernstein(int n, int i, double t);
	};
}
#endif
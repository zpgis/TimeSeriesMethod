#pragma once
#ifndef SADATAEXEC_H
#define SADATAEXEC_H
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "DifferenceHistogram.h"
namespace  gdal_viirs_Interpolation
{
	class SADataExec {
	public:
		SADataExec() = default;
		~SADataExec() = default;
		/* @brief 查看该图像的直方图分布  */
		static std::vector<DifferenceHistogram>* viirs_histogram(const char* prev01_path, std::vector<double>& order, int* validPixelCount);
		/* @brief 查看该图像的散点图分布   第一个为真实数据 第二个为插补数据  */


		/** @brief Suomi-NPP VIIRS数据去噪（中值滤波5*5窗口）*/
		static int viirs_Denoising_MedianFlitering_5P5(const char* src_path, const char* dst_path, float maxDNFilter = -1.0);

		/** @brief Suomi-NPP VIIRS VCM月度数据插补（最小二乘线性拟合）least squares method */
		static int viirs_Interpolation_LSM(
			const char* prev01_path,
			const char* prev02_path,
			const char* prev03_path,
			const char* prev04_path,
			const char* prev05_path,
			const char* prev06_path,
			const char* current_path,
			const char* next01_path,
			const char* next02_path,
			const char* next03_path,
			const char* next04_path,
			const char* next05_path,
			const char* next06_path,
			const char* dst_path,
			float r2 = 0.85);

		/** @brief Suomi-NPP VIIRS VCM月度数据插补（最小二乘曲线拟合  二次方程）least squares method */
		static int viirs_Interpolation_LSM_Curve(
			const char* prev01_path,
			const char* prev02_path,
			const char* prev03_path,
			const char* prev04_path,
			const char* prev05_path,
			const char* prev06_path,
			const char* current_path,
			const char* next01_path,
			const char* next02_path,
			const char* next03_path,
			const char* next04_path,
			const char* next05_path,
			const char* next06_path,
			const char* dst_path,
			float r2 = 0.85);

		/** @brief Suomi-NPP VIIRS VCM月度数据插补（最小二乘曲线拟合  三次方程）least squares method */
		static int viirs_Interpolation_LSM_Curve_Three(
			const char* prev01_path,
			const char* prev02_path,
			const char* prev03_path,
			const char* prev04_path,
			const char* prev05_path,
			const char* prev06_path,
			const char* current_path,
			const char* next01_path,
			const char* next02_path,
			const char* next03_path,
			const char* next04_path,
			const char* next05_path,
			const char* next06_path,
			const char* dst_path,
			float r2 = 0.85);

		/** @brief Suomi-NPP VIIRS VCM月度数据插补（三次样条插值 ）cubic spline interpolation */
		static int viirs_Interpolation_Spline3(
			const char* prev01_path,
			const char* prev02_path,
			const char* prev03_path,
			const char* prev04_path,
			const char* prev05_path,
			const char* prev06_path,
			const char* current_path,
			const char* next01_path,
			const char* next02_path,
			const char* next03_path,
			const char* next04_path,
			const char* next05_path,
			const char* next06_path,
			const char* dst_path,
			float r2 = 0.85);

		/** @brief Suomi-NPP VIIRS VCM月度数据插补（三阶贝塞尔曲线插值 ）Interpolation with cubic Bezier Curves */
		static int viirs_Interpolation_BezierCurve(
			const char* prev01_path,
			const char* prev02_path,
			const char* prev03_path,
			const char* prev04_path,
			const char* prev05_path,
			const char* prev06_path,
			const char* current_path,
			const char* next01_path,
			const char* next02_path,
			const char* next03_path,
			const char* next04_path,
			const char* next05_path,
			const char* next06_path,
			const char* dst_path,
			float r2 = 0.85);

		/** @brief Suomi-NPP VIIRS VCM月度数据插补（三阶埃尔米特 Hermite 插值 ） */
		static int viirs_Interpolation_Hermite(
			const char* prev01_path,
			const char* prev02_path,
			const char* prev03_path,
			const char* prev04_path,
			const char* prev05_path,
			const char* prev06_path,
			const char* current_path,
			const char* next01_path,
			const char* next02_path,
			const char* next03_path,
			const char* next04_path,
			const char* next05_path,
			const char* next06_path,
			const char* dst_path,
			float r2 = 0.85);

		/** @brief Suomi-NPP VIIRS VCM月度数据插补（三阶指数平滑法  ） */
		static int viirs_Interpolation_CES(
			const char* prev01_path,
			const char* prev02_path,
			const char* prev03_path,
			const char* prev04_path,
			const char* prev05_path,
			const char* prev06_path,
			const char* current_path,
			const char* next01_path,
			const char* next02_path,
			const char* next03_path,
			const char* next04_path,
			const char* next05_path,
			const char* next06_path,
			const char* dst_path,
			float r2 = 0.85);

		/** @brief Suomi-NPP VIIRS VCM月度数据插补（灰色预测法  GrayForecastModel ） */
		static int viirs_Interpolation_GFM(
			const char* prev01_path,
			const char* prev02_path,
			const char* prev03_path,
			const char* prev04_path,
			const char* prev05_path,
			const char* prev06_path,
			const char* current_path,
			const char* next01_path,
			const char* next02_path,
			const char* next03_path,
			const char* next04_path,
			const char* next05_path,
			const char* next06_path,
			const char* dst_path,
			float r2 = 0.85);
	};
	
	/*
	 * @brief 用于处理裁剪之后的局部的城市区域
	 */
	class RegionDataHanle {
	public:
		/* 记录小于0 且大于285的异常值，并将小于0的赋值为0，大于285的赋值为285 
		 * threshold  最大阈值
		*/
		static int RecordAndHandleException(const char* src_dem_path, const char* dst_dem_path, int* lowExceptionCount,
			int* highExceptionCount, double* lowExceptionValue, double* highExceptionValue,/* std::vector<double>* lowValues, 
			std::vector<double>* highValues ,*/ double threshold);
		// 统计有效值的像元个数
		static int totalCount(const char* src_dem_path);
		// 统计图像的像元值总和
		static double totalDNValue(const char* src_dem_path, double* minValue, double* maxValue);
		// 两个图像相减，得到一个差异绝对值图像
		static void differenceImage(const char* src_path1, const char* src_path2, const char* dst_dem_path);
		static std::vector<Real_Simulate>* viirs_scatter(const char* prev01_path, const char* prev02_path);
	};


}
#endif // SADATAEXEC_H
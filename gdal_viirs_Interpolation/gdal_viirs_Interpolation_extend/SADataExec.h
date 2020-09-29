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
		/* @brief �鿴��ͼ���ֱ��ͼ�ֲ�  */
		static std::vector<DifferenceHistogram>* viirs_histogram(const char* prev01_path, std::vector<double>& order, int* validPixelCount);
		/* @brief �鿴��ͼ���ɢ��ͼ�ֲ�   ��һ��Ϊ��ʵ���� �ڶ���Ϊ�岹����  */


		/** @brief Suomi-NPP VIIRS����ȥ�루��ֵ�˲�5*5���ڣ�*/
		static int viirs_Denoising_MedianFlitering_5P5(const char* src_path, const char* dst_path, float maxDNFilter = -1.0);

		/** @brief Suomi-NPP VIIRS VCM�¶����ݲ岹����С����������ϣ�least squares method */
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

		/** @brief Suomi-NPP VIIRS VCM�¶����ݲ岹����С�����������  ���η��̣�least squares method */
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

		/** @brief Suomi-NPP VIIRS VCM�¶����ݲ岹����С�����������  ���η��̣�least squares method */
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

		/** @brief Suomi-NPP VIIRS VCM�¶����ݲ岹������������ֵ ��cubic spline interpolation */
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

		/** @brief Suomi-NPP VIIRS VCM�¶����ݲ岹�����ױ��������߲�ֵ ��Interpolation with cubic Bezier Curves */
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

		/** @brief Suomi-NPP VIIRS VCM�¶����ݲ岹�����װ������� Hermite ��ֵ �� */
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

		/** @brief Suomi-NPP VIIRS VCM�¶����ݲ岹������ָ��ƽ����  �� */
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

		/** @brief Suomi-NPP VIIRS VCM�¶����ݲ岹����ɫԤ�ⷨ  GrayForecastModel �� */
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
	 * @brief ���ڴ���ü�֮��ľֲ��ĳ�������
	 */
	class RegionDataHanle {
	public:
		/* ��¼С��0 �Ҵ���285���쳣ֵ������С��0�ĸ�ֵΪ0������285�ĸ�ֵΪ285 
		 * threshold  �����ֵ
		*/
		static int RecordAndHandleException(const char* src_dem_path, const char* dst_dem_path, int* lowExceptionCount,
			int* highExceptionCount, double* lowExceptionValue, double* highExceptionValue,/* std::vector<double>* lowValues, 
			std::vector<double>* highValues ,*/ double threshold);
		// ͳ����Чֵ����Ԫ����
		static int totalCount(const char* src_dem_path);
		// ͳ��ͼ�����Ԫֵ�ܺ�
		static double totalDNValue(const char* src_dem_path, double* minValue, double* maxValue);
		// ����ͼ��������õ�һ���������ֵͼ��
		static void differenceImage(const char* src_path1, const char* src_path2, const char* dst_dem_path);
		static std::vector<Real_Simulate>* viirs_scatter(const char* prev01_path, const char* prev02_path);
	};


}
#endif // SADATAEXEC_H
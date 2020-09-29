#include "SADataExec.h"
#include "gdal_priv.h"
#include "Cramer.h"
#include "Spline.h"
#include "BezierCurve.h"

#include "hermiteInterpolation.h"
#include <omp.h>
#include "CubicExponentialSmoothing.h"
#include "GrayForecastModel.h"
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iomanip>
#include "DifferenceHistogram.h"
using namespace CubicExponentialSmoothingSpace;
using namespace BezierCurveSpace;
using namespace SplineSpace;
using namespace gdal_viirs_Interpolation;
using namespace GFMpace;
using namespace std;

std::vector<Real_Simulate>* RegionDataHanle::viirs_scatter(const char* prev01_path, const char* prev02_path) {
	
	//std::multimap<double, double>* map = new std::multimap<double, double>();
	vector<Real_Simulate>* vec = new vector<Real_Simulate>();
	
	//注册GDAL函数和功能
	GDALAllRegister();
	CPLSetConfigOption("GDAL_DATA", "D:\\Program Files\\gdal\\data\\");
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;
	GDALDatasetH hSrcDS02 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;
	GDALRasterBandH hSrcBand02 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };



	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		//return false;
	}


	//打开所有文件
	hSrcDS01 = GDALOpen(prev01_path, GA_ReadOnly);
	hSrcDS02 = GDALOpen(prev02_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);
	hSrcBand02 = GDALGetRasterBand(hSrcDS02, 1);


	if (hSrcBand01 == NULL) flags[0] = 0;
	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS01);
	ysize = GDALGetRasterYSize(hSrcDS01);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;
	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand01);
	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData02 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;

	int count = 0;
	int index = 1;
	for (int i = 0;i < ysize;i++)
	{
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData02, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		if (err == CE_None)
		{
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
				GDALRasterIO(hSrcBand02, GF_Read, 0, i, xsize, 1, pData02, xsize, 1, gType, 0, 0);
			}

			for (int j = 0;j < xsize;j++)
			{
				
				if (pData01[j] >= 0) {
					count++;
					
					double vcm = pData01[j];
					double simulate = pData02[j];
					//cout << "i = " << i << " index = " << index++ << " vcm = " << vcm << " simulate = " << simulate << endl;
					Real_Simulate real;
					if (vcm > 400) {
						continue;
					}
					if (simulate > 400) {
						continue;
					}
					real.real = vcm;
					real.simulate = simulate;
					vec->push_back(real);
					//map->insert(pair<double, double>(vcm, simulate) );
				}
				


			}
		}


		//写出一行结果
#pragma region
//if (maxDNFilter > 0)
//{
//	for (int s = 0; s < xsize; s++)
//	{
//		if (rData[s] > maxDNFilter)
//		{
//			rData[s] = 0.0;
//		}
//		if (rData[s] < 0.0)
//		{
//			rData[s] = 0.0;
//		}
//	}
//}

#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);


	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);


	GDALDestroyDriverManager();

	
	return vec;
}

vector<DifferenceHistogram>* SADataExec::viirs_histogram(const char* prev01_path, vector<double>&  order, int * validPixelCount) {
	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		//return false;
	}

	//打开所有文件
	hSrcDS01 = GDALOpen(prev01_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);

	if (hSrcBand01 == NULL) flags[0] = 0;
	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS01);
	ysize = GDALGetRasterYSize(hSrcDS01);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;
	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand01);
	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;
	std::map<int, int> map;
	// 构造一个前闭后开区间
	for (size_t i = 0; i < order.size(); i++)
	{
		map.insert(std::pair<int, int>(order[i], 0));
	}
	vector<DifferenceHistogram> *hists = new vector<DifferenceHistogram>;
	// 构造一个前闭后开区间
	for (size_t i = 0; i < order.size(); i++)
	{
		DifferenceHistogram hist(order[i]);
		hists->push_back(std::move(hist));
	}

	int index = 0;
	for (int i = 0;i < ysize;i++)
	{
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		if (err == CE_None)
		{
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}
		
			for (int j = 0;j < xsize;j++)
			{
				
				if (pData01[j] < 0) {
					continue;
				}
				index++;
				// 向前取整
				for (auto iter = hists->rbegin(); iter != hists->rend(); ++iter)
				{
					if (pData01[j] > iter->startValue) {
						iter->pixelCount++;
						iter->totalValue += pData01[j];
						break;
					}
				}
				// 四舍五入
				//for (auto iter = hists->rbegin(); iter != hists->rend(); ++iter)
				//{

				//	if (pData01[j] > (iter->startValue - 0.5) && pData01[j] <= (iter->startValue + 0.5)) {
				//		iter->pixelCount++;
				//		iter->totalValue += pData01[j];
				//		break;
				//	}
				//}
				
			}
		}


		//写出一行结果
#pragma region
//if (maxDNFilter > 0)
//{
//	for (int s = 0; s < xsize; s++)
//	{
//		if (rData[s] > maxDNFilter)
//		{
//			rData[s] = 0.0;
//		}
//		if (rData[s] < 0.0)
//		{
//			rData[s] = 0.0;
//		}
//	}
//}

#pragma endregion

	}
	*validPixelCount = index;
	cout << "index = " << index << endl;
	GDALSetGeoTransform(hDstDS, adfGeoTransform);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);


	GDALDestroyDriverManager();
	//std::cout << rslt << " lines is copied!" << std::endl;
	//if (rslt == ysize)
	//{
	//	std::cout << "SUCCESS!" << std::endl;
	//}
	//else
	//{
	//	std::cout << "FINISH with ERRORS!" << std::endl;
	//	std::cout << ysize - rslt << " lines was not copied!" << std::endl;
	//}
	return hists;
}

int SADataExec::viirs_Denoising_MedianFlitering_5P5(const char* src_path, const char* dst_path, float maxDNFilter)
{
	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS = NULL;
	GDALRasterBandH hSrcBand = NULL;
	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];

	/*检测目标数据信息*/
	hDstDS = GDALOpen(dst_path, GA_Update);
	if (hDstDS != NULL)
	{
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return -1;
	}

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		return false;
	}

	/*检测数据源信息*/
	hSrcDS = GDALOpen(src_path, GA_ReadOnly);
	if (hSrcDS == NULL)
	{
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入数据的第一个波段
	hSrcBand = GDALGetRasterBand(hSrcDS, 1);
	if (hSrcBand == 0)
	{
		GDALClose(hSrcDS);
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS);
	ysize = GDALGetRasterYSize(hSrcDS);
	cout << "Lines: " << ysize << "; Columns: " << xsize << endl;
	if (xsize <= 1 || ysize <= 1)
	{
		GDALClose(hSrcDS);
		GDALDestroyDriverManager();
		return false;
	}

	//获取图像的仿射变换矩阵
	if (GDALGetGeoTransform(hSrcDS, adfGeoTransform) != CE_None)
	{
		GDALClose(hSrcDS);
		GDALDestroyDriverManager();
		return false;
	}

	//获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;
	dfDstNoDataValue = GDALGetRasterNoDataValue(hSrcBand, &bDstHasNoData);

	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand);

	//创建拷贝数据集
	hDstDS = GDALCreate(
		hDriver,
		dst_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	//判断是否创建成功
	if (hDstDS == NULL)
	{
		GDALClose(hSrcDS);
		GDALDestroyDriverManager();
		return false;
	}
	hDstBand = GDALGetRasterBand(hDstDS, 1);

	//逐行读取并拷贝数据,pData用于存储每一行的数据
	//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData = (float*)malloc(GDALGetDataTypeSize(gType) * xsize * 5 / 8);
	float* rData = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;

	int rslt = 0;

	for (int i = 0;i < ysize;i++)
	{
		cout << "Processing line " << i + 1 << "!" << endl;
		//每次复制前清空上次读取数据占用的内存
		memset(pData, 0x00, GDALGetDataTypeSize(gType) * xsize * 5 / 8);
		memset(rData, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);

		//处理一行
		if (i == 0)
		{
# pragma region
			err = GDALRasterIO(hSrcBand,
				GF_Read,
				0,
				i,
				xsize,
				3,
				pData,
				xsize,
				3,
				gType,
				0,
				0);
			if (err == CE_None)
			{
				for (int j = 0;j < xsize;j++)
				{
					float dns[25] = { 0.0 };
					if (j == 0)
					{
						dns[0] = (float)pData[j];
						dns[1] = (float)pData[j + 1];
						dns[2] = (float)pData[j + 2];
						dns[3] = (float)pData[xsize + j];
						dns[4] = (float)pData[xsize + j + 1];
						dns[5] = (float)pData[xsize + j + 2];
						dns[6] = (float)pData[xsize * 2 + j];
						dns[7] = (float)pData[xsize * 2 + j + 1];
						dns[8] = (float)pData[xsize * 2 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 9);
						rData[j] = dns_vec[4];
					}
					else if (j == 1)
					{
						dns[0] = (float)pData[j - 1];
						dns[1] = (float)pData[j];
						dns[2] = (float)pData[j + 1];
						dns[3] = (float)pData[j + 2];
						dns[4] = (float)pData[xsize + j - 1];
						dns[5] = (float)pData[xsize + j];
						dns[6] = (float)pData[xsize + j + 1];
						dns[7] = (float)pData[xsize + j + 2];
						dns[8] = (float)pData[xsize * 2 + j - 1];
						dns[9] = (float)pData[xsize * 2 + j];
						dns[10] = (float)pData[xsize * 2 + j + 1];
						dns[11] = (float)pData[xsize * 2 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 12);
						rData[j] = dns_vec[5];
					}
					else if (j == xsize - 2)
					{
						dns[0] = (float)pData[j - 2];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j];
						dns[3] = (float)pData[j + 1];
						dns[4] = (float)pData[xsize + j - 2];
						dns[5] = (float)pData[xsize + j - 1];
						dns[6] = (float)pData[xsize + j];
						dns[7] = (float)pData[xsize + j + 1];
						dns[8] = (float)pData[xsize * 2 + j - 2];
						dns[9] = (float)pData[xsize * 2 + j - 1];
						dns[10] = (float)pData[xsize * 2 + j];
						dns[11] = (float)pData[xsize * 2 + j + 1];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 12);
						rData[j] = dns_vec[5];
					}
					else if (j == xsize - 1)
					{
						dns[0] = (float)pData[j];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j - 2];
						dns[3] = (float)pData[xsize + j];
						dns[4] = (float)pData[xsize + j - 1];
						dns[5] = (float)pData[xsize + j - 2];
						dns[6] = (float)pData[xsize * 2 + j];
						dns[7] = (float)pData[xsize * 2 + j - 1];
						dns[8] = (float)pData[xsize * 2 + j - 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 9);
						rData[j] = dns_vec[4];
					}
					else
					{
						dns[0] = (float)pData[j - 2];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j];
						dns[3] = (float)pData[j + 1];
						dns[4] = (float)pData[j + 2];
						dns[5] = (float)pData[xsize + j - 2];
						dns[6] = (float)pData[xsize + j - 1];
						dns[7] = (float)pData[xsize + j];
						dns[8] = (float)pData[xsize + j + 1];
						dns[9] = (float)pData[xsize + j + 2];
						dns[10] = (float)pData[xsize * 2 + j - 2];
						dns[11] = (float)pData[xsize * 2 + j - 1];
						dns[12] = (float)pData[xsize * 2 + j];
						dns[13] = (float)pData[xsize * 2 + j + 1];
						dns[14] = (float)pData[xsize * 2 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 15);
						rData[j] = dns_vec[7];
					}
				}
			}
# pragma endregion
		}
		else if (i == 1)
		{
# pragma region
			err = GDALRasterIO(hSrcBand,
				GF_Read,
				0,
				i - 1,
				xsize,
				4,
				pData,
				xsize,
				4,
				gType,
				0,
				0);
			if (err == CE_None)
			{
				for (int j = 0;j < xsize;j++)
				{
					float dns[25] = { 0.0 };
					if (j == 0)
					{
						dns[0] = (float)pData[j];
						dns[1] = (float)pData[j + 1];
						dns[2] = (float)pData[j + 2];
						dns[3] = (float)pData[xsize + j];
						dns[4] = (float)pData[xsize + j + 1];
						dns[5] = (float)pData[xsize + j + 2];
						dns[6] = (float)pData[xsize * 2 + j];
						dns[7] = (float)pData[xsize * 2 + j + 1];
						dns[8] = (float)pData[xsize * 2 + j + 2];
						dns[9] = (float)pData[xsize * 3 + j];
						dns[10] = (float)pData[xsize * 3 + j + 1];
						dns[11] = (float)pData[xsize * 3 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 12);
						rData[j] = dns_vec[5];
					}
					else if (j == 1)
					{
						dns[0] = (float)pData[j - 1];
						dns[1] = (float)pData[j];
						dns[2] = (float)pData[j + 1];
						dns[3] = (float)pData[j + 2];
						dns[4] = (float)pData[xsize + j - 1];
						dns[5] = (float)pData[xsize + j];
						dns[6] = (float)pData[xsize + j + 1];
						dns[7] = (float)pData[xsize + j + 2];
						dns[8] = (float)pData[xsize * 2 + j - 1];
						dns[9] = (float)pData[xsize * 2 + j];
						dns[10] = (float)pData[xsize * 2 + j + 1];
						dns[11] = (float)pData[xsize * 2 + j + 2];
						dns[12] = (float)pData[xsize * 3 + j - 1];
						dns[13] = (float)pData[xsize * 3 + j];
						dns[14] = (float)pData[xsize * 3 + j + 1];
						dns[15] = (float)pData[xsize * 3 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 16);
						rData[j] = dns_vec[7];
					}
					else if (j == xsize - 2)
					{
						dns[0] = (float)pData[j - 2];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j];
						dns[3] = (float)pData[j + 1];
						dns[4] = (float)pData[xsize + j - 2];
						dns[5] = (float)pData[xsize + j - 1];
						dns[6] = (float)pData[xsize + j];
						dns[7] = (float)pData[xsize + j + 1];
						dns[8] = (float)pData[xsize * 2 + j - 2];
						dns[9] = (float)pData[xsize * 2 + j - 1];
						dns[10] = (float)pData[xsize * 2 + j];
						dns[11] = (float)pData[xsize * 2 + j + 1];
						dns[12] = (float)pData[xsize * 3 + j - 2];
						dns[13] = (float)pData[xsize * 3 + j - 1];
						dns[14] = (float)pData[xsize * 3 + j];
						dns[15] = (float)pData[xsize * 3 + j + 1];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 16);
						rData[j] = dns_vec[7];
					}
					else if (j == xsize - 1)
					{
						dns[0] = (float)pData[j];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j - 2];
						dns[3] = (float)pData[xsize + j];
						dns[4] = (float)pData[xsize + j - 1];
						dns[5] = (float)pData[xsize + j - 2];
						dns[6] = (float)pData[xsize * 2 + j];
						dns[7] = (float)pData[xsize * 2 + j - 1];
						dns[8] = (float)pData[xsize * 2 + j - 2];
						dns[9] = (float)pData[xsize * 3 + j];
						dns[10] = (float)pData[xsize * 3 + j - 1];
						dns[11] = (float)pData[xsize * 3 + j - 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 12);
						rData[j] = dns_vec[5];
					}
					else
					{
						dns[0] = (float)pData[j - 2];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j];
						dns[3] = (float)pData[j + 1];
						dns[4] = (float)pData[j + 2];
						dns[5] = (float)pData[xsize + j - 2];
						dns[6] = (float)pData[xsize + j - 1];
						dns[7] = (float)pData[xsize + j];
						dns[8] = (float)pData[xsize + j + 1];
						dns[9] = (float)pData[xsize + j + 2];
						dns[10] = (float)pData[xsize * 2 + j - 2];
						dns[11] = (float)pData[xsize * 2 + j - 1];
						dns[12] = (float)pData[xsize * 2 + j];
						dns[13] = (float)pData[xsize * 2 + j + 1];
						dns[14] = (float)pData[xsize * 2 + j + 2];
						dns[15] = (float)pData[xsize * 3 + j - 2];
						dns[16] = (float)pData[xsize * 3 + j - 1];
						dns[17] = (float)pData[xsize * 3 + j];
						dns[18] = (float)pData[xsize * 3 + j + 1];
						dns[19] = (float)pData[xsize * 3 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 20);
						rData[j] = dns_vec[9];
					}
				}
			}
# pragma endregion
		}
		else if (i == ysize - 2)
		{
# pragma region
			err = GDALRasterIO(hSrcBand,
				GF_Read,
				0,
				i - 2,
				xsize,
				4,
				pData,
				xsize,
				4,
				gType,
				0,
				0);
			if (err == CE_None)
			{
				for (int j = 0;j < xsize;j++)
				{
					float dns[25] = { 0.0 };
					if (j == 0)
					{
						dns[0] = (float)pData[j];
						dns[1] = (float)pData[j + 1];
						dns[2] = (float)pData[j + 2];
						dns[3] = (float)pData[xsize + j];
						dns[4] = (float)pData[xsize + j + 1];
						dns[5] = (float)pData[xsize + j + 2];
						dns[6] = (float)pData[xsize * 2 + j];
						dns[7] = (float)pData[xsize * 2 + j + 1];
						dns[8] = (float)pData[xsize * 2 + j + 2];
						dns[9] = (float)pData[xsize * 3 + j];
						dns[10] = (float)pData[xsize * 3 + j + 1];
						dns[11] = (float)pData[xsize * 3 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 12);
						rData[j] = dns_vec[5];
					}
					else if (j == 1)
					{
						dns[0] = (float)pData[j - 1];
						dns[1] = (float)pData[j];
						dns[2] = (float)pData[j + 1];
						dns[3] = (float)pData[j + 2];
						dns[4] = (float)pData[xsize + j - 1];
						dns[5] = (float)pData[xsize + j];
						dns[6] = (float)pData[xsize + j + 1];
						dns[7] = (float)pData[xsize + j + 2];
						dns[8] = (float)pData[xsize * 2 + j - 1];
						dns[9] = (float)pData[xsize * 2 + j];
						dns[10] = (float)pData[xsize * 2 + j + 1];
						dns[11] = (float)pData[xsize * 2 + j + 2];
						dns[12] = (float)pData[xsize * 3 + j - 1];
						dns[13] = (float)pData[xsize * 3 + j];
						dns[14] = (float)pData[xsize * 3 + j + 1];
						dns[15] = (float)pData[xsize * 3 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 16);
						rData[j] = dns_vec[7];
					}
					else if (j == xsize - 2)
					{
						dns[0] = (float)pData[j - 2];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j];
						dns[3] = (float)pData[j + 1];
						dns[4] = (float)pData[xsize + j - 2];
						dns[5] = (float)pData[xsize + j - 1];
						dns[6] = (float)pData[xsize + j];
						dns[7] = (float)pData[xsize + j + 1];
						dns[8] = (float)pData[xsize * 2 + j - 2];
						dns[9] = (float)pData[xsize * 2 + j - 1];
						dns[10] = (float)pData[xsize * 2 + j];
						dns[11] = (float)pData[xsize * 2 + j + 1];
						dns[12] = (float)pData[xsize * 3 + j - 2];
						dns[13] = (float)pData[xsize * 3 + j - 1];
						dns[14] = (float)pData[xsize * 3 + j];
						dns[15] = (float)pData[xsize * 3 + j + 1];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 16);
						rData[j] = dns_vec[7];
					}
					else if (j == xsize - 1)
					{
						dns[0] = (float)pData[j];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j - 2];
						dns[3] = (float)pData[xsize + j];
						dns[4] = (float)pData[xsize + j - 1];
						dns[5] = (float)pData[xsize + j - 2];
						dns[6] = (float)pData[xsize * 2 + j];
						dns[7] = (float)pData[xsize * 2 + j - 1];
						dns[8] = (float)pData[xsize * 2 + j - 2];
						dns[9] = (float)pData[xsize * 3 + j];
						dns[10] = (float)pData[xsize * 3 + j - 1];
						dns[11] = (float)pData[xsize * 3 + j - 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 12);
						rData[j] = dns_vec[5];
					}
					else
					{
						dns[0] = (float)pData[j - 2];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j];
						dns[3] = (float)pData[j + 1];
						dns[4] = (float)pData[j + 2];
						dns[5] = (float)pData[xsize + j - 2];
						dns[6] = (float)pData[xsize + j - 1];
						dns[7] = (float)pData[xsize + j];
						dns[8] = (float)pData[xsize + j + 1];
						dns[9] = (float)pData[xsize + j + 2];
						dns[10] = (float)pData[xsize * 2 + j - 2];
						dns[11] = (float)pData[xsize * 2 + j - 1];
						dns[12] = (float)pData[xsize * 2 + j];
						dns[13] = (float)pData[xsize * 2 + j + 1];
						dns[14] = (float)pData[xsize * 2 + j + 2];
						dns[15] = (float)pData[xsize * 3 + j - 2];
						dns[16] = (float)pData[xsize * 3 + j - 1];
						dns[17] = (float)pData[xsize * 3 + j];
						dns[18] = (float)pData[xsize * 3 + j + 1];
						dns[19] = (float)pData[xsize * 3 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 20);
						rData[j] = dns_vec[9];
					}
				}
			}
# pragma endregion
		}
		else if (i == ysize - 1)
		{
# pragma region
			err = GDALRasterIO(hSrcBand,
				GF_Read,
				0,
				i - 2,
				xsize,
				3,
				pData,
				xsize,
				3,
				gType,
				0,
				0);
			if (err == CE_None)
			{
				for (int j = 0;j < xsize;j++)
				{
					float dns[25] = { 0.0 };
					if (j == 0)
					{
						dns[0] = (float)pData[j];
						dns[1] = (float)pData[j + 1];
						dns[2] = (float)pData[j + 2];
						dns[3] = (float)pData[xsize + j];
						dns[4] = (float)pData[xsize + j + 1];
						dns[5] = (float)pData[xsize + j + 2];
						dns[6] = (float)pData[xsize * 2 + j];
						dns[7] = (float)pData[xsize * 2 + j + 1];
						dns[8] = (float)pData[xsize * 2 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 9);
						rData[j] = dns_vec[4];
					}
					else if (j == 1)
					{
						dns[0] = (float)pData[j - 1];
						dns[1] = (float)pData[j];
						dns[2] = (float)pData[j + 1];
						dns[3] = (float)pData[j + 2];
						dns[4] = (float)pData[xsize + j - 1];
						dns[5] = (float)pData[xsize + j];
						dns[6] = (float)pData[xsize + j + 1];
						dns[7] = (float)pData[xsize + j + 2];
						dns[8] = (float)pData[xsize * 2 + j - 1];
						dns[9] = (float)pData[xsize * 2 + j];
						dns[10] = (float)pData[xsize * 2 + j + 1];
						dns[11] = (float)pData[xsize * 2 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 12);
						rData[j] = dns_vec[5];
					}
					else if (j == xsize - 2)
					{
						dns[0] = (float)pData[j - 2];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j];
						dns[3] = (float)pData[j + 1];
						dns[4] = (float)pData[xsize + j - 2];
						dns[5] = (float)pData[xsize + j - 1];
						dns[6] = (float)pData[xsize + j];
						dns[7] = (float)pData[xsize + j + 1];
						dns[8] = (float)pData[xsize * 2 + j - 2];
						dns[9] = (float)pData[xsize * 2 + j - 1];
						dns[10] = (float)pData[xsize * 2 + j];
						dns[11] = (float)pData[xsize * 2 + j + 1];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 12);
						rData[j] = dns_vec[5];
					}
					else if (j == xsize - 1)
					{
						dns[0] = (float)pData[j];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j - 2];
						dns[3] = (float)pData[xsize + j];
						dns[4] = (float)pData[xsize + j - 1];
						dns[5] = (float)pData[xsize + j - 2];
						dns[6] = (float)pData[xsize * 2 + j];
						dns[7] = (float)pData[xsize * 2 + j - 1];
						dns[8] = (float)pData[xsize * 2 + j - 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 9);
						rData[j] = dns_vec[4];
					}
					else
					{
						dns[0] = (float)pData[j - 2];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j];
						dns[3] = (float)pData[j + 1];
						dns[4] = (float)pData[j + 2];
						dns[5] = (float)pData[xsize + j - 2];
						dns[6] = (float)pData[xsize + j - 1];
						dns[7] = (float)pData[xsize + j];
						dns[8] = (float)pData[xsize + j + 1];
						dns[9] = (float)pData[xsize + j + 2];
						dns[10] = (float)pData[xsize * 2 + j - 2];
						dns[11] = (float)pData[xsize * 2 + j - 1];
						dns[12] = (float)pData[xsize * 2 + j];
						dns[13] = (float)pData[xsize * 2 + j + 1];
						dns[14] = (float)pData[xsize * 2 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 15);
						rData[j] = dns_vec[7];
					}
				}
			}
# pragma endregion
		}
		else
		{
# pragma region
			err = GDALRasterIO(hSrcBand,
				GF_Read,
				0,
				i - 2,
				xsize,
				5,
				pData,
				xsize,
				5,
				gType,
				0,
				0);
			if (err == CE_None)
			{
				for (int j = 0;j < xsize;j++)
				{
					float dns[25] = { 0.0 };
					if (j == 0)
					{
						dns[0] = (float)pData[j];
						dns[1] = (float)pData[j + 1];
						dns[2] = (float)pData[j + 2];
						dns[3] = (float)pData[xsize + j];
						dns[4] = (float)pData[xsize + j + 1];
						dns[5] = (float)pData[xsize + j + 2];
						dns[6] = (float)pData[xsize * 2 + j];
						dns[7] = (float)pData[xsize * 2 + j + 1];
						dns[8] = (float)pData[xsize * 2 + j + 2];
						dns[9] = (float)pData[xsize * 3 + j];
						dns[10] = (float)pData[xsize * 3 + j + 1];
						dns[11] = (float)pData[xsize * 3 + j + 2];
						dns[12] = (float)pData[xsize * 4 + j];
						dns[13] = (float)pData[xsize * 4 + j + 1];
						dns[14] = (float)pData[xsize * 4 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 15);
						rData[j] = dns_vec[7];
					}
					else if (j == 1)
					{
						dns[0] = (float)pData[j - 1];
						dns[1] = (float)pData[j];
						dns[2] = (float)pData[j + 1];
						dns[3] = (float)pData[j + 2];
						dns[4] = (float)pData[xsize + j - 1];
						dns[5] = (float)pData[xsize + j];
						dns[6] = (float)pData[xsize + j + 1];
						dns[7] = (float)pData[xsize + j + 2];
						dns[8] = (float)pData[xsize * 2 + j - 1];
						dns[9] = (float)pData[xsize * 2 + j];
						dns[10] = (float)pData[xsize * 2 + j + 1];
						dns[11] = (float)pData[xsize * 2 + j + 2];
						dns[12] = (float)pData[xsize * 3 + j - 1];
						dns[13] = (float)pData[xsize * 3 + j];
						dns[14] = (float)pData[xsize * 3 + j + 1];
						dns[15] = (float)pData[xsize * 3 + j + 2];
						dns[16] = (float)pData[xsize * 4 + j - 1];
						dns[17] = (float)pData[xsize * 4 + j];
						dns[18] = (float)pData[xsize * 4 + j + 1];
						dns[19] = (float)pData[xsize * 4 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 20);
						rData[j] = dns_vec[9];
					}
					else if (j == xsize - 2)
					{
						dns[0] = (float)pData[j - 2];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j];
						dns[3] = (float)pData[j + 1];
						dns[4] = (float)pData[xsize + j - 2];
						dns[5] = (float)pData[xsize + j - 1];
						dns[6] = (float)pData[xsize + j];
						dns[7] = (float)pData[xsize + j + 1];
						dns[8] = (float)pData[xsize * 2 + j - 2];
						dns[9] = (float)pData[xsize * 2 + j - 1];
						dns[10] = (float)pData[xsize * 2 + j];
						dns[11] = (float)pData[xsize * 2 + j + 1];
						dns[12] = (float)pData[xsize * 3 + j - 2];
						dns[13] = (float)pData[xsize * 3 + j - 1];
						dns[14] = (float)pData[xsize * 3 + j];
						dns[15] = (float)pData[xsize * 3 + j + 1];
						dns[16] = (float)pData[xsize * 4 + j - 2];
						dns[17] = (float)pData[xsize * 4 + j - 1];
						dns[18] = (float)pData[xsize * 4 + j];
						dns[19] = (float)pData[xsize * 4 + j + 1];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 20);
						rData[j] = dns_vec[9];
					}
					else if (j == xsize - 1)
					{
						dns[0] = (float)pData[j];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j - 2];
						dns[3] = (float)pData[xsize + j];
						dns[4] = (float)pData[xsize + j - 1];
						dns[5] = (float)pData[xsize + j - 2];
						dns[6] = (float)pData[xsize * 2 + j];
						dns[7] = (float)pData[xsize * 2 + j - 1];
						dns[8] = (float)pData[xsize * 2 + j - 2];
						dns[9] = (float)pData[xsize * 3 + j];
						dns[10] = (float)pData[xsize * 3 + j - 1];
						dns[11] = (float)pData[xsize * 3 + j - 2];
						dns[12] = (float)pData[xsize * 4 + j];
						dns[13] = (float)pData[xsize * 4 + j - 1];
						dns[14] = (float)pData[xsize * 4 + j - 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 15);
						rData[j] = dns_vec[7];
					}
					else
					{
						dns[0] = (float)pData[j - 2];
						dns[1] = (float)pData[j - 1];
						dns[2] = (float)pData[j];
						dns[3] = (float)pData[j + 1];
						dns[4] = (float)pData[j + 2];
						dns[5] = (float)pData[xsize + j - 2];
						dns[6] = (float)pData[xsize + j - 1];
						dns[7] = (float)pData[xsize + j];
						dns[8] = (float)pData[xsize + j + 1];
						dns[9] = (float)pData[xsize + j + 2];
						dns[10] = (float)pData[xsize * 2 + j - 2];
						dns[11] = (float)pData[xsize * 2 + j - 1];
						dns[12] = (float)pData[xsize * 2 + j];
						dns[13] = (float)pData[xsize * 2 + j + 1];
						dns[14] = (float)pData[xsize * 2 + j + 2];
						dns[15] = (float)pData[xsize * 3 + j - 2];
						dns[16] = (float)pData[xsize * 3 + j - 1];
						dns[17] = (float)pData[xsize * 3 + j];
						dns[18] = (float)pData[xsize * 3 + j + 1];
						dns[19] = (float)pData[xsize * 3 + j + 2];
						dns[20] = (float)pData[xsize * 4 + j - 2];
						dns[21] = (float)pData[xsize * 4 + j - 1];
						dns[22] = (float)pData[xsize * 4 + j];
						dns[23] = (float)pData[xsize * 4 + j + 1];
						dns[24] = (float)pData[xsize * 4 + j + 2];

						std::vector<float> dns_vec(dns, dns + 25);
						std::sort(dns_vec.begin(), dns_vec.begin() + 25);
						rData[j] = dns_vec[12];
					}
				}
			}
# pragma endregion
		}

		//写出一行结果，写出前进行高值噪声过滤(大于高值噪声过滤值的像元值设置为0.0）
#pragma region
		if (maxDNFilter > 0)
		{
			for (int s = 0; s < xsize; s++)
			{
				if (rData[s] > maxDNFilter)
				{
					rData[s] = 0.0;
				}
				if (rData[s] < 0.0)
				{
					rData[s] = 0.0;
				}
			}
		}
		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			i,
			xsize,
			1,
			rData,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			//cout<<"line "<<i<<" is copied!"<<endl;
			rslt++;
		}
		else
		{
			cout << "ERROR: line " << i << " was failed!" << endl;
		}
#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);
	GDALSetProjection(hDstDS, GDALGetProjectionRef(hSrcDS));
	if (bDstHasNoData)
		GDALSetRasterNoDataValue(hDstBand, dfDstNoDataValue);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData);
	GDALClose(hDstDS);
	GDALClose(hSrcDS);
	GDALDestroyDriverManager();
	cout << rslt << " lines is copied!" << endl;
	if (rslt == ysize)
	{
		cout << "SUCCESS!" << endl;
	}
	else
	{
		cout << "FINISH with ERRORS!" << endl;
		cout << ysize - rslt << " lines was not copied!" << endl;
	}
	return rslt;
}


int SADataExec::viirs_Interpolation_LSM(
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
	float r2)
{
	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;
	GDALDatasetH hSrcDS02 = NULL;
	GDALDatasetH hSrcDS03 = NULL;
	GDALDatasetH hSrcDS04 = NULL;
	GDALDatasetH hSrcDS05 = NULL;
	GDALDatasetH hSrcDS06 = NULL;
	GDALDatasetH hSrcDS07 = NULL;
	GDALDatasetH hSrcDS08 = NULL;
	GDALDatasetH hSrcDS09 = NULL;
	GDALDatasetH hSrcDS10 = NULL;
	GDALDatasetH hSrcDS11 = NULL;
	GDALDatasetH hSrcDS12 = NULL;
	GDALDatasetH hSrcDS13 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;
	GDALRasterBandH hSrcBand02 = NULL;
	GDALRasterBandH hSrcBand03 = NULL;
	GDALRasterBandH hSrcBand04 = NULL;
	GDALRasterBandH hSrcBand05 = NULL;
	GDALRasterBandH hSrcBand06 = NULL;
	GDALRasterBandH hSrcBand07 = NULL;
	GDALRasterBandH hSrcBand08 = NULL;
	GDALRasterBandH hSrcBand09 = NULL;
	GDALRasterBandH hSrcBand10 = NULL;
	GDALRasterBandH hSrcBand11 = NULL;
	GDALRasterBandH hSrcBand12 = NULL;
	GDALRasterBandH hSrcBand13 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };

	/*检测目标数据信息*/
	hDstDS = GDALOpen(dst_path, GA_Update);
	if (hDstDS != NULL)
	{
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return -1;
	}

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		return false;
	}

	/*检测数据源信息*/
	hSrcDS07 = GDALOpen(current_path, GA_ReadOnly);
	if (hSrcDS07 == NULL)
	{
		//待插补的数据无法打开
		GDALDestroyDriverManager();
		return false;
	}
	else
	{
		flags[6] = 1;
	}

	hSrcBand07 = GDALGetRasterBand(hSrcDS07, 1);
	if (hSrcBand07 == NULL)
	{
		//待插补的数据无法打开
		GDALClose(hSrcBand07);
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS07);
	ysize = GDALGetRasterYSize(hSrcDS07);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;

	GDALGetGeoTransform(hSrcDS07, adfGeoTransform);

	//获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;
	dfDstNoDataValue = GDALGetRasterNoDataValue(hSrcBand07, &bDstHasNoData);

	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand07);

	//创建拷贝数据集
	hDstDS = GDALCreate(
		hDriver,
		dst_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	//判断是否创建成功
	if (hDstDS == NULL)
	{
		GDALClose(hSrcDS07);
		GDALDestroyDriverManager();
		return false;
	}
	hDstBand = GDALGetRasterBand(hDstDS, 1);
	if (hDstBand == NULL)
	{
		GDALClose(hSrcDS07);
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return false;
	}

	//打开所有文件
	hSrcDS01 = GDALOpen(prev01_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;
	hSrcDS02 = GDALOpen(prev02_path, GA_ReadOnly);
	if (hSrcDS02) flags[1] = 1;
	hSrcDS03 = GDALOpen(prev03_path, GA_ReadOnly);
	if (hSrcDS03) flags[2] = 1;
	hSrcDS04 = GDALOpen(prev04_path, GA_ReadOnly);
	if (hSrcDS04) flags[3] = 1;
	hSrcDS05 = GDALOpen(prev05_path, GA_ReadOnly);
	if (hSrcDS05) flags[4] = 1;
	hSrcDS06 = GDALOpen(prev06_path, GA_ReadOnly);
	if (hSrcDS06) flags[5] = 1;

	hSrcDS08 = GDALOpen(next01_path, GA_ReadOnly);
	if (hSrcDS08) flags[7] = 1;
	hSrcDS09 = GDALOpen(next02_path, GA_ReadOnly);
	if (hSrcDS09) flags[8] = 1;
	hSrcDS10 = GDALOpen(next03_path, GA_ReadOnly);
	if (hSrcDS10) flags[9] = 1;
	hSrcDS11 = GDALOpen(next04_path, GA_ReadOnly);
	if (hSrcDS11) flags[10] = 1;
	hSrcDS12 = GDALOpen(next05_path, GA_ReadOnly);
	if (hSrcDS12) flags[11] = 1;
	hSrcDS13 = GDALOpen(next06_path, GA_ReadOnly);
	if (hSrcDS13) flags[12] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);
	hSrcBand02 = GDALGetRasterBand(hSrcDS02, 1);
	hSrcBand03 = GDALGetRasterBand(hSrcDS03, 1);
	hSrcBand04 = GDALGetRasterBand(hSrcDS04, 1);
	hSrcBand05 = GDALGetRasterBand(hSrcDS05, 1);
	hSrcBand06 = GDALGetRasterBand(hSrcDS06, 1);

	hSrcBand08 = GDALGetRasterBand(hSrcDS08, 1);
	hSrcBand09 = GDALGetRasterBand(hSrcDS09, 1);
	hSrcBand10 = GDALGetRasterBand(hSrcDS10, 1);
	hSrcBand11 = GDALGetRasterBand(hSrcDS11, 1);
	hSrcBand12 = GDALGetRasterBand(hSrcDS12, 1);
	hSrcBand13 = GDALGetRasterBand(hSrcDS13, 1);

	if (hSrcBand01 == NULL) flags[0] = 0;
	if (hSrcBand02 == NULL) flags[1] = 0;
	if (hSrcBand03 == NULL) flags[2] = 0;
	if (hSrcBand04 == NULL) flags[3] = 0;
	if (hSrcBand05 == NULL) flags[4] = 0;
	if (hSrcBand06 == NULL) flags[5] = 0;
	if (hSrcBand07 == NULL) flags[6] = 0;
	if (hSrcBand08 == NULL) flags[7] = 0;
	if (hSrcBand09 == NULL) flags[8] = 0;
	if (hSrcBand10 == NULL) flags[9] = 0;
	if (hSrcBand11 == NULL) flags[10] = 0;
	if (hSrcBand12 == NULL) flags[11] = 0;
	if (hSrcBand13 == NULL) flags[12] = 0;

	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData02 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData03 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData04 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData05 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData06 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData07 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData08 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData09 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData10 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData11 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData12 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData13 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;

	int rslt = 0;

	for (int i = 0;i < ysize;i++)
	{
		//cout<<"Processing line "<<i+1<<"!"<<endl;
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData02, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData03, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData04, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData05, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData06, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData07, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData08, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData09, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData10, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData11, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData12, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData13, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);

		//memset(rData, 0x00, GDALGetDataTypeSize(gType)*xsize/8);

		//处理一行
# pragma region
		err = GDALRasterIO(hSrcBand07, GF_Read, 0, i, xsize, 1, pData07, xsize, 1, gType, 0, 0);
		if (err == CE_None)
		{
#pragma region
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}

			if (flags[1] == 1)
			{
				GDALRasterIO(hSrcBand02, GF_Read, 0, i, xsize, 1, pData02, xsize, 1, gType, 0, 0);
			}

			if (flags[2] == 1)
			{
				GDALRasterIO(hSrcBand03, GF_Read, 0, i, xsize, 1, pData03, xsize, 1, gType, 0, 0);
			}

			if (flags[3] == 1)
			{
				GDALRasterIO(hSrcBand04, GF_Read, 0, i, xsize, 1, pData04, xsize, 1, gType, 0, 0);
			}

			if (flags[4] == 1)
			{
				GDALRasterIO(hSrcBand05, GF_Read, 0, i, xsize, 1, pData05, xsize, 1, gType, 0, 0);
			}

			if (flags[5] == 1)
			{
				GDALRasterIO(hSrcBand06, GF_Read, 0, i, xsize, 1, pData06, xsize, 1, gType, 0, 0);
			}

			if (flags[7] == 1)
			{
				GDALRasterIO(hSrcBand08, GF_Read, 0, i, xsize, 1, pData08, xsize, 1, gType, 0, 0);
			}

			if (flags[8] == 1)
			{
				GDALRasterIO(hSrcBand09, GF_Read, 0, i, xsize, 1, pData09, xsize, 1, gType, 0, 0);
			}
			if (flags[9] == 1)
			{
				GDALRasterIO(hSrcBand10, GF_Read, 0, i, xsize, 1, pData10, xsize, 1, gType, 0, 0);
			}

			if (flags[10] == 1)
			{
				GDALRasterIO(hSrcBand11, GF_Read, 0, i, xsize, 1, pData11, xsize, 1, gType, 0, 0);
			}

			if (flags[11] == 1)
			{
				GDALRasterIO(hSrcBand12, GF_Read, 0, i, xsize, 1, pData12, xsize, 1, gType, 0, 0);
			}

			if (flags[12] == 1)
			{
				GDALRasterIO(hSrcBand13, GF_Read, 0, i, xsize, 1, pData13, xsize, 1, gType, 0, 0);
			}
#pragma endregion

			for (int j = 0;j < xsize;j++)
			{
				//最小二乘线性拟合，参考：
				//https://www.cnblogs.com/softlin/p/5815531.html
				//https://zh.wikipedia.org/zh/最小二乘法
				double dns[13] = { 0.0 };
				double idx[13] = { 0.0 };
				double xy[13] = { 0.0 };
				double x2[13] = { 0.0 };
				double idx_avg = 0.0, idx_sum = 0.0;
				double dns_avg = 0.0, dns_sum = 0.0;
				double xy_avg = 0.0, xy_sum = 0.0;
				double x2_avg = 0.0, x2_sum = 0.0;
				int val_dn_number = 0;//可用数据点的个数

				dns[0] = pData01[j];
				dns[1] = pData02[j];
				dns[2] = pData03[j];
				dns[3] = pData04[j];
				dns[4] = pData05[j];
				dns[5] = pData06[j];
				dns[6] = pData07[j];
				dns[7] = pData08[j];
				dns[8] = pData09[j];
				dns[9] = pData10[j];
				dns[10] = pData11[j];
				dns[11] = pData12[j];
				dns[12] = pData13[j];

				for (int m = 0;m < 13;m++)
				{
					if (m != 6 && flags[m] == 1)
					{
						idx[m] = m;
						xy[m] = idx[m] * dns[m];
						x2[m] = idx[m] * idx[m];
						idx_sum += idx[m];
						dns_sum += dns[m];
						xy_sum += xy[m];
						x2_sum += x2[m];
						val_dn_number++;
					}
				}
				idx_avg = idx_sum / val_dn_number;
				dns_avg = dns_sum / val_dn_number;
				xy_avg = xy_sum / val_dn_number;
				x2_avg = x2_sum / val_dn_number;
				double k = (xy_avg - dns_avg * idx_avg) / (x2_avg - idx_avg * idx_avg);
				double b = dns_avg - k * idx_avg;
				pData07[j] = (float)(k * 6 + b);
			}
		}
# pragma endregion


		//写出一行结果
#pragma region
//if (maxDNFilter > 0)
//{
//	for (int s = 0; s < xsize; s++)
//	{
//		if (rData[s] > maxDNFilter)
//		{
//			rData[s] = 0.0;
//		}
//		if (rData[s] < 0.0)
//		{
//			rData[s] = 0.0;
//		}
//	}
//}
		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			i,
			xsize,
			1,
			pData07,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			std::cout << "line " << i << " is processed!" << std::endl;
			rslt++;
		}
		else
		{
			std::cout << "ERROR: line " << i << " was failed!" << std::endl;
		}
#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);
	GDALSetProjection(hDstDS, GDALGetProjectionRef(hSrcDS07));
	if (bDstHasNoData)
		GDALSetRasterNoDataValue(hDstBand, dfDstNoDataValue);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);
	free(pData02);
	free(pData03);
	free(pData04);
	free(pData05);
	free(pData06);
	free(pData07);
	free(pData08);
	free(pData09);
	free(pData10);
	free(pData11);
	free(pData12);
	free(pData13);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);
	GDALClose(hSrcDS02);
	GDALClose(hSrcDS03);
	GDALClose(hSrcDS04);
	GDALClose(hSrcDS05);
	GDALClose(hSrcDS06);
	GDALClose(hSrcDS07);
	GDALClose(hSrcDS08);
	GDALClose(hSrcDS09);
	GDALClose(hSrcDS10);
	GDALClose(hSrcDS11);
	GDALClose(hSrcDS12);
	GDALClose(hSrcDS13);

	GDALDestroyDriverManager();
	std::cout << rslt << " lines is copied!" << std::endl;
	if (rslt == ysize)
	{
		std::cout << "SUCCESS!" << std::endl;
	}
	else
	{
		std::cout << "FINISH with ERRORS!" << std::endl;
		std::cout << ysize - rslt << " lines was not copied!" << std::endl;
	}
	return rslt;
}

int SADataExec::viirs_Interpolation_LSM_Curve(
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
	float r2)
{
	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;
	GDALDatasetH hSrcDS02 = NULL;
	GDALDatasetH hSrcDS03 = NULL;
	GDALDatasetH hSrcDS04 = NULL;
	GDALDatasetH hSrcDS05 = NULL;
	GDALDatasetH hSrcDS06 = NULL;
	GDALDatasetH hSrcDS07 = NULL;
	GDALDatasetH hSrcDS08 = NULL;
	GDALDatasetH hSrcDS09 = NULL;
	GDALDatasetH hSrcDS10 = NULL;
	GDALDatasetH hSrcDS11 = NULL;
	GDALDatasetH hSrcDS12 = NULL;
	GDALDatasetH hSrcDS13 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;
	GDALRasterBandH hSrcBand02 = NULL;
	GDALRasterBandH hSrcBand03 = NULL;
	GDALRasterBandH hSrcBand04 = NULL;
	GDALRasterBandH hSrcBand05 = NULL;
	GDALRasterBandH hSrcBand06 = NULL;
	GDALRasterBandH hSrcBand07 = NULL;
	GDALRasterBandH hSrcBand08 = NULL;
	GDALRasterBandH hSrcBand09 = NULL;
	GDALRasterBandH hSrcBand10 = NULL;
	GDALRasterBandH hSrcBand11 = NULL;
	GDALRasterBandH hSrcBand12 = NULL;
	GDALRasterBandH hSrcBand13 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };

	/*检测目标数据信息*/
	hDstDS = GDALOpen(dst_path, GA_Update);
	if (hDstDS != NULL)
	{
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return -1;
	}

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		return false;
	}

	/*检测数据源信息*/
	hSrcDS07 = GDALOpen(current_path, GA_ReadOnly);
	if (hSrcDS07 == NULL)
	{
		//待插补的数据无法打开
		GDALDestroyDriverManager();
		return false;
	}
	else
	{
		flags[6] = 1;
	}

	hSrcBand07 = GDALGetRasterBand(hSrcDS07, 1);
	if (hSrcBand07 == NULL)
	{
		//待插补的数据无法打开
		GDALClose(hSrcBand07);
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS07);
	ysize = GDALGetRasterYSize(hSrcDS07);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;

	GDALGetGeoTransform(hSrcDS07, adfGeoTransform);

	//获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;
	dfDstNoDataValue = GDALGetRasterNoDataValue(hSrcBand07, &bDstHasNoData);

	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand07);

	//创建拷贝数据集
	hDstDS = GDALCreate(
		hDriver,
		dst_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	//判断是否创建成功
	if (hDstDS == NULL)
	{
		GDALClose(hSrcDS07);
		GDALDestroyDriverManager();
		return false;
	}
	hDstBand = GDALGetRasterBand(hDstDS, 1);
	if (hDstBand == NULL)
	{
		GDALClose(hSrcDS07);
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return false;
	}

	//打开所有文件
	hSrcDS01 = GDALOpen(prev01_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;
	hSrcDS02 = GDALOpen(prev02_path, GA_ReadOnly);
	if (hSrcDS02) flags[1] = 1;
	hSrcDS03 = GDALOpen(prev03_path, GA_ReadOnly);
	if (hSrcDS03) flags[2] = 1;
	hSrcDS04 = GDALOpen(prev04_path, GA_ReadOnly);
	if (hSrcDS04) flags[3] = 1;
	hSrcDS05 = GDALOpen(prev05_path, GA_ReadOnly);
	if (hSrcDS05) flags[4] = 1;
	hSrcDS06 = GDALOpen(prev06_path, GA_ReadOnly);
	if (hSrcDS06) flags[5] = 1;

	hSrcDS08 = GDALOpen(next01_path, GA_ReadOnly);
	if (hSrcDS08) flags[7] = 1;
	hSrcDS09 = GDALOpen(next02_path, GA_ReadOnly);
	if (hSrcDS09) flags[8] = 1;
	hSrcDS10 = GDALOpen(next03_path, GA_ReadOnly);
	if (hSrcDS10) flags[9] = 1;
	hSrcDS11 = GDALOpen(next04_path, GA_ReadOnly);
	if (hSrcDS11) flags[10] = 1;
	hSrcDS12 = GDALOpen(next05_path, GA_ReadOnly);
	if (hSrcDS12) flags[11] = 1;
	hSrcDS13 = GDALOpen(next06_path, GA_ReadOnly);
	if (hSrcDS13) flags[12] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);
	hSrcBand02 = GDALGetRasterBand(hSrcDS02, 1);
	hSrcBand03 = GDALGetRasterBand(hSrcDS03, 1);
	hSrcBand04 = GDALGetRasterBand(hSrcDS04, 1);
	hSrcBand05 = GDALGetRasterBand(hSrcDS05, 1);
	hSrcBand06 = GDALGetRasterBand(hSrcDS06, 1);

	hSrcBand08 = GDALGetRasterBand(hSrcDS08, 1);
	hSrcBand09 = GDALGetRasterBand(hSrcDS09, 1);
	hSrcBand10 = GDALGetRasterBand(hSrcDS10, 1);
	hSrcBand11 = GDALGetRasterBand(hSrcDS11, 1);
	hSrcBand12 = GDALGetRasterBand(hSrcDS12, 1);
	hSrcBand13 = GDALGetRasterBand(hSrcDS13, 1);

	if (hSrcBand01 == NULL) flags[0] = 0;
	if (hSrcBand02 == NULL) flags[1] = 0;
	if (hSrcBand03 == NULL) flags[2] = 0;
	if (hSrcBand04 == NULL) flags[3] = 0;
	if (hSrcBand05 == NULL) flags[4] = 0;
	if (hSrcBand06 == NULL) flags[5] = 0;
	if (hSrcBand07 == NULL) flags[6] = 0;
	if (hSrcBand08 == NULL) flags[7] = 0;
	if (hSrcBand09 == NULL) flags[8] = 0;
	if (hSrcBand10 == NULL) flags[9] = 0;
	if (hSrcBand11 == NULL) flags[10] = 0;
	if (hSrcBand12 == NULL) flags[11] = 0;
	if (hSrcBand13 == NULL) flags[12] = 0;

	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData02 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData03 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData04 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData05 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData06 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData07 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData08 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData09 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData10 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData11 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData12 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData13 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;

	int rslt = 0;

	for (int i = 0;i < ysize;i++)
	//for (int i = 5000;i < 5001;i++)
	{
		//cout<<"Processing line "<<i+1<<"!"<<endl;
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData02, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData03, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData04, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData05, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData06, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData07, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData08, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData09, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData10, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData11, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData12, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData13, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);

		//memset(rData, 0x00, GDALGetDataTypeSize(gType)*xsize/8);

		//处理一行
# pragma region
		err = GDALRasterIO(hSrcBand07, GF_Read, 0, i, xsize, 1, pData07, xsize, 1, gType, 0, 0);
		if (err == CE_None)
		{
#pragma region
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}

			if (flags[1] == 1)
			{
				GDALRasterIO(hSrcBand02, GF_Read, 0, i, xsize, 1, pData02, xsize, 1, gType, 0, 0);
			}

			if (flags[2] == 1)
			{
				GDALRasterIO(hSrcBand03, GF_Read, 0, i, xsize, 1, pData03, xsize, 1, gType, 0, 0);
			}

			if (flags[3] == 1)
			{
				GDALRasterIO(hSrcBand04, GF_Read, 0, i, xsize, 1, pData04, xsize, 1, gType, 0, 0);
			}

			if (flags[4] == 1)
			{
				GDALRasterIO(hSrcBand05, GF_Read, 0, i, xsize, 1, pData05, xsize, 1, gType, 0, 0);
			}

			if (flags[5] == 1)
			{
				GDALRasterIO(hSrcBand06, GF_Read, 0, i, xsize, 1, pData06, xsize, 1, gType, 0, 0);
			}

			if (flags[7] == 1)
			{
				GDALRasterIO(hSrcBand08, GF_Read, 0, i, xsize, 1, pData08, xsize, 1, gType, 0, 0);
			}

			if (flags[8] == 1)
			{
				GDALRasterIO(hSrcBand09, GF_Read, 0, i, xsize, 1, pData09, xsize, 1, gType, 0, 0);
			}
			if (flags[9] == 1)
			{
				GDALRasterIO(hSrcBand10, GF_Read, 0, i, xsize, 1, pData10, xsize, 1, gType, 0, 0);
			}

			if (flags[10] == 1)
			{
				GDALRasterIO(hSrcBand11, GF_Read, 0, i, xsize, 1, pData11, xsize, 1, gType, 0, 0);
			}

			if (flags[11] == 1)
			{
				GDALRasterIO(hSrcBand12, GF_Read, 0, i, xsize, 1, pData12, xsize, 1, gType, 0, 0);
			}

			if (flags[12] == 1)
			{
				GDALRasterIO(hSrcBand13, GF_Read, 0, i, xsize, 1, pData13, xsize, 1, gType, 0, 0);
			}
#pragma endregion

			for (int j = 0;j < xsize;j++)
			//for (int j = 5000;j < 6000;j++)
			{
				//最小二乘二次曲线拟合
				double dns[13] = { 0.0 };
				double x_sum = 0.0, x2_sum = 0.0, x3_sum = 0.0, x4_sum = 0.0;
				double y_sum = 0.0, xy_sum = 0.0, x2y_sum = 0.0;
				double val_dn_number = 0;//可用数据点的个数

				dns[0] = pData01[j];
				dns[1] = pData02[j];
				dns[2] = pData03[j];
				dns[3] = pData04[j];
				dns[4] = pData05[j];
				dns[5] = pData06[j];
				dns[6] = pData07[j];
				dns[7] = pData08[j];
				dns[8] = pData09[j];
				dns[9] = pData10[j];
				dns[10] = pData11[j];
				dns[11] = pData12[j];
				dns[12] = pData13[j];

				for (int m = 0;m < 13;m++)
				{
					if (m != 6 && flags[m] == 1)
					{
						x_sum += m;
						x2_sum += pow(m, 2);
						x3_sum += pow(m, 3);
						x4_sum += pow(m, 4);

						y_sum += dns[m];
						xy_sum += m * dns[m];
						x2y_sum += pow(m, 2) * dns[m];

						val_dn_number++;
					}
				}
				std::vector<std::vector<double>> equations = {
					{ x4_sum, x3_sum,  x2_sum,  x2y_sum},
					{ x3_sum,  x2_sum,  x_sum, xy_sum},
					{ x2_sum,  x_sum, val_dn_number, y_sum}
				};

				auto solution = solveCramer(equations);
				double a = solution[0];
				double b = solution[1];
				double c = solution[2];
				pData07[j] = (float)(a * 6 * 6  + b * 6 + c);
				//std::cout << "pData07[" << j << "] = " << pData07[j] << std::endl;
			}
		}
# pragma endregion


		//写出一行结果
#pragma region
//if (maxDNFilter > 0)
//{
//	for (int s = 0; s < xsize; s++)
//	{
//		if (rData[s] > maxDNFilter)
//		{
//			rData[s] = 0.0;
//		}
//		if (rData[s] < 0.0)
//		{
//			rData[s] = 0.0;
//		}
//	}
//}
		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			i,
			xsize,
			1,
			pData07,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			std::cout << "line " << i << " is processed!" << std::endl;
			rslt++;
		}
		else
		{
			std::cout << "ERROR: line " << i << " was failed!" << std::endl;
		}
#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);
	GDALSetProjection(hDstDS, GDALGetProjectionRef(hSrcDS07));
	if (bDstHasNoData)
		GDALSetRasterNoDataValue(hDstBand, dfDstNoDataValue);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);
	free(pData02);
	free(pData03);
	free(pData04);
	free(pData05);
	free(pData06);
	free(pData07);
	free(pData08);
	free(pData09);
	free(pData10);
	free(pData11);
	free(pData12);
	free(pData13);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);
	GDALClose(hSrcDS02);
	GDALClose(hSrcDS03);
	GDALClose(hSrcDS04);
	GDALClose(hSrcDS05);
	GDALClose(hSrcDS06);
	GDALClose(hSrcDS07);
	GDALClose(hSrcDS08);
	GDALClose(hSrcDS09);
	GDALClose(hSrcDS10);
	GDALClose(hSrcDS11);
	GDALClose(hSrcDS12);
	GDALClose(hSrcDS13);

	GDALDestroyDriverManager();
	std::cout << rslt << " lines is copied!" << std::endl;
	if (rslt == ysize)
	{
		std::cout << "SUCCESS!" << std::endl;
	}
	else
	{
		std::cout << "FINISH with ERRORS!" << std::endl;
		std::cout << ysize - rslt << " lines was not copied!" << std::endl;
	}
	return rslt;
}

int SADataExec::viirs_Interpolation_LSM_Curve_Three(
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
	float r2)
{
	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;
	GDALDatasetH hSrcDS02 = NULL;
	GDALDatasetH hSrcDS03 = NULL;
	GDALDatasetH hSrcDS04 = NULL;
	GDALDatasetH hSrcDS05 = NULL;
	GDALDatasetH hSrcDS06 = NULL;
	GDALDatasetH hSrcDS07 = NULL;
	GDALDatasetH hSrcDS08 = NULL;
	GDALDatasetH hSrcDS09 = NULL;
	GDALDatasetH hSrcDS10 = NULL;
	GDALDatasetH hSrcDS11 = NULL;
	GDALDatasetH hSrcDS12 = NULL;
	GDALDatasetH hSrcDS13 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;
	GDALRasterBandH hSrcBand02 = NULL;
	GDALRasterBandH hSrcBand03 = NULL;
	GDALRasterBandH hSrcBand04 = NULL;
	GDALRasterBandH hSrcBand05 = NULL;
	GDALRasterBandH hSrcBand06 = NULL;
	GDALRasterBandH hSrcBand07 = NULL;
	GDALRasterBandH hSrcBand08 = NULL;
	GDALRasterBandH hSrcBand09 = NULL;
	GDALRasterBandH hSrcBand10 = NULL;
	GDALRasterBandH hSrcBand11 = NULL;
	GDALRasterBandH hSrcBand12 = NULL;
	GDALRasterBandH hSrcBand13 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };

	/*检测目标数据信息*/
	hDstDS = GDALOpen(dst_path, GA_Update);
	if (hDstDS != NULL)
	{
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return -1;
	}

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		return false;
	}

	/*检测数据源信息*/
	hSrcDS07 = GDALOpen(current_path, GA_ReadOnly);
	if (hSrcDS07 == NULL)
	{
		//待插补的数据无法打开
		GDALDestroyDriverManager();
		return false;
	}
	else
	{
		flags[6] = 1;
	}

	hSrcBand07 = GDALGetRasterBand(hSrcDS07, 1);
	if (hSrcBand07 == NULL)
	{
		//待插补的数据无法打开
		GDALClose(hSrcBand07);
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS07);
	ysize = GDALGetRasterYSize(hSrcDS07);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;

	GDALGetGeoTransform(hSrcDS07, adfGeoTransform);

	//获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;
	dfDstNoDataValue = GDALGetRasterNoDataValue(hSrcBand07, &bDstHasNoData);

	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand07);

	//创建拷贝数据集
	hDstDS = GDALCreate(
		hDriver,
		dst_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	//判断是否创建成功
	if (hDstDS == NULL)
	{
		GDALClose(hSrcDS07);
		GDALDestroyDriverManager();
		return false;
	}
	hDstBand = GDALGetRasterBand(hDstDS, 1);
	if (hDstBand == NULL)
	{
		GDALClose(hSrcDS07);
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return false;
	}

	//打开所有文件
	hSrcDS01 = GDALOpen(prev01_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;
	hSrcDS02 = GDALOpen(prev02_path, GA_ReadOnly);
	if (hSrcDS02) flags[1] = 1;
	hSrcDS03 = GDALOpen(prev03_path, GA_ReadOnly);
	if (hSrcDS03) flags[2] = 1;
	hSrcDS04 = GDALOpen(prev04_path, GA_ReadOnly);
	if (hSrcDS04) flags[3] = 1;
	hSrcDS05 = GDALOpen(prev05_path, GA_ReadOnly);
	if (hSrcDS05) flags[4] = 1;
	hSrcDS06 = GDALOpen(prev06_path, GA_ReadOnly);
	if (hSrcDS06) flags[5] = 1;

	hSrcDS08 = GDALOpen(next01_path, GA_ReadOnly);
	if (hSrcDS08) flags[7] = 1;
	hSrcDS09 = GDALOpen(next02_path, GA_ReadOnly);
	if (hSrcDS09) flags[8] = 1;
	hSrcDS10 = GDALOpen(next03_path, GA_ReadOnly);
	if (hSrcDS10) flags[9] = 1;
	hSrcDS11 = GDALOpen(next04_path, GA_ReadOnly);
	if (hSrcDS11) flags[10] = 1;
	hSrcDS12 = GDALOpen(next05_path, GA_ReadOnly);
	if (hSrcDS12) flags[11] = 1;
	hSrcDS13 = GDALOpen(next06_path, GA_ReadOnly);
	if (hSrcDS13) flags[12] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);
	hSrcBand02 = GDALGetRasterBand(hSrcDS02, 1);
	hSrcBand03 = GDALGetRasterBand(hSrcDS03, 1);
	hSrcBand04 = GDALGetRasterBand(hSrcDS04, 1);
	hSrcBand05 = GDALGetRasterBand(hSrcDS05, 1);
	hSrcBand06 = GDALGetRasterBand(hSrcDS06, 1);

	hSrcBand08 = GDALGetRasterBand(hSrcDS08, 1);
	hSrcBand09 = GDALGetRasterBand(hSrcDS09, 1);
	hSrcBand10 = GDALGetRasterBand(hSrcDS10, 1);
	hSrcBand11 = GDALGetRasterBand(hSrcDS11, 1);
	hSrcBand12 = GDALGetRasterBand(hSrcDS12, 1);
	hSrcBand13 = GDALGetRasterBand(hSrcDS13, 1);

	if (hSrcBand01 == NULL) flags[0] = 0;
	if (hSrcBand02 == NULL) flags[1] = 0;
	if (hSrcBand03 == NULL) flags[2] = 0;
	if (hSrcBand04 == NULL) flags[3] = 0;
	if (hSrcBand05 == NULL) flags[4] = 0;
	if (hSrcBand06 == NULL) flags[5] = 0;
	if (hSrcBand07 == NULL) flags[6] = 0;
	if (hSrcBand08 == NULL) flags[7] = 0;
	if (hSrcBand09 == NULL) flags[8] = 0;
	if (hSrcBand10 == NULL) flags[9] = 0;
	if (hSrcBand11 == NULL) flags[10] = 0;
	if (hSrcBand12 == NULL) flags[11] = 0;
	if (hSrcBand13 == NULL) flags[12] = 0;

	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData02 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData03 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData04 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData05 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData06 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData07 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData08 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData09 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData10 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData11 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData12 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData13 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;

	int rslt = 0;

	for (int i = 0;i < ysize;i++)
	//for (int i = 5000;i < 5001;i++)
	{
		//cout<<"Processing line "<<i+1<<"!"<<endl;
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData02, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData03, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData04, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData05, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData06, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData07, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData08, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData09, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData10, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData11, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData12, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData13, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);

		//memset(rData, 0x00, GDALGetDataTypeSize(gType)*xsize/8);

		//处理一行
# pragma region
		err = GDALRasterIO(hSrcBand07, GF_Read, 0, i, xsize, 1, pData07, xsize, 1, gType, 0, 0);
		if (err == CE_None)
		{
#pragma region
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}

			if (flags[1] == 1)
			{
				GDALRasterIO(hSrcBand02, GF_Read, 0, i, xsize, 1, pData02, xsize, 1, gType, 0, 0);
			}

			if (flags[2] == 1)
			{
				GDALRasterIO(hSrcBand03, GF_Read, 0, i, xsize, 1, pData03, xsize, 1, gType, 0, 0);
			}

			if (flags[3] == 1)
			{
				GDALRasterIO(hSrcBand04, GF_Read, 0, i, xsize, 1, pData04, xsize, 1, gType, 0, 0);
			}

			if (flags[4] == 1)
			{
				GDALRasterIO(hSrcBand05, GF_Read, 0, i, xsize, 1, pData05, xsize, 1, gType, 0, 0);
			}

			if (flags[5] == 1)
			{
				GDALRasterIO(hSrcBand06, GF_Read, 0, i, xsize, 1, pData06, xsize, 1, gType, 0, 0);
			}

			if (flags[7] == 1)
			{
				GDALRasterIO(hSrcBand08, GF_Read, 0, i, xsize, 1, pData08, xsize, 1, gType, 0, 0);
			}

			if (flags[8] == 1)
			{
				GDALRasterIO(hSrcBand09, GF_Read, 0, i, xsize, 1, pData09, xsize, 1, gType, 0, 0);
			}
			if (flags[9] == 1)
			{
				GDALRasterIO(hSrcBand10, GF_Read, 0, i, xsize, 1, pData10, xsize, 1, gType, 0, 0);
			}

			if (flags[10] == 1)
			{
				GDALRasterIO(hSrcBand11, GF_Read, 0, i, xsize, 1, pData11, xsize, 1, gType, 0, 0);
			}

			if (flags[11] == 1)
			{
				GDALRasterIO(hSrcBand12, GF_Read, 0, i, xsize, 1, pData12, xsize, 1, gType, 0, 0);
			}

			if (flags[12] == 1)
			{
				GDALRasterIO(hSrcBand13, GF_Read, 0, i, xsize, 1, pData13, xsize, 1, gType, 0, 0);
			}
#pragma endregion

			for (int j = 0;j < xsize;j++)
				//for (int j = 5000;j < 6000;j++)
			{
				//最小二乘三次曲线拟合
				double dns[13] = { 0.0 };
				double x_sum = 0.0, x2_sum = 0.0, x3_sum = 0.0, x4_sum = 0.0, x5_sum = 0.0, x6_sum = 0.0;
				double y_sum = 0.0, xy_sum = 0.0, x2y_sum = 0.0, x3y_sum = 0.0;
				double val_dn_number = 0;//可用数据点的个数

				dns[0] = pData01[j];
				dns[1] = pData02[j];
				dns[2] = pData03[j];
				dns[3] = pData04[j];
				dns[4] = pData05[j];
				dns[5] = pData06[j];
				dns[6] = pData07[j];
				dns[7] = pData08[j];
				dns[8] = pData09[j];
				dns[9] = pData10[j];
				dns[10] = pData11[j];
				dns[11] = pData12[j];
				dns[12] = pData13[j];

				for (int m = 0;m < 13;m++)
				{
					if (m != 6 && flags[m] == 1)
					{
						x_sum += m;
						x2_sum += pow(m, 2);
						x3_sum += pow(m, 3);
						x4_sum += pow(m, 4);
						x5_sum += pow(m, 5);
						x6_sum += pow(m, 6);

						y_sum += dns[m];
						xy_sum += m * dns[m];
						x2y_sum += pow(m, 2) * dns[m];
						x3y_sum += pow(m, 3) * dns[m];

						val_dn_number++;
					}
				}
				std::vector<std::vector<double>> equations = {
					{ val_dn_number, x_sum,  x2_sum,x3_sum,  y_sum},
					{ x_sum,  x2_sum,  x3_sum, x4_sum, xy_sum},
					{ x2_sum,  x3_sum, x4_sum,x5_sum, x2y_sum},
					{ x3_sum,  x4_sum, x5_sum,x6_sum, x3y_sum}
				};

				auto solution = solveCramer(equations);
				double w0 = solution[0];
				double w1 = solution[1];
				double w2 = solution[2];
				double w3 = solution[3];
				pData07[j] = (float)(w0 + w1 * 6 + w2 * 6 * 6+ w3 *6*6*6);
				//std::cout << "pData07[" << j << "] = " << pData07[j] << std::endl;
			}
		}
# pragma endregion


		//写出一行结果
#pragma region
//if (maxDNFilter > 0)
//{
//	for (int s = 0; s < xsize; s++)
//	{
//		if (rData[s] > maxDNFilter)
//		{
//			rData[s] = 0.0;
//		}
//		if (rData[s] < 0.0)
//		{
//			rData[s] = 0.0;
//		}
//	}
//}
		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			i,
			xsize,
			1,
			pData07,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			std::cout << "line " << i << " is processed!" << std::endl;
			rslt++;
		}
		else
		{
			std::cout << "ERROR: line " << i << " was failed!" << std::endl;
		}
#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);
	GDALSetProjection(hDstDS, GDALGetProjectionRef(hSrcDS07));
	if (bDstHasNoData)
		GDALSetRasterNoDataValue(hDstBand, dfDstNoDataValue);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);
	free(pData02);
	free(pData03);
	free(pData04);
	free(pData05);
	free(pData06);
	free(pData07);
	free(pData08);
	free(pData09);
	free(pData10);
	free(pData11);
	free(pData12);
	free(pData13);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);
	GDALClose(hSrcDS02);
	GDALClose(hSrcDS03);
	GDALClose(hSrcDS04);
	GDALClose(hSrcDS05);
	GDALClose(hSrcDS06);
	GDALClose(hSrcDS07);
	GDALClose(hSrcDS08);
	GDALClose(hSrcDS09);
	GDALClose(hSrcDS10);
	GDALClose(hSrcDS11);
	GDALClose(hSrcDS12);
	GDALClose(hSrcDS13);

	GDALDestroyDriverManager();
	std::cout << rslt << " lines is copied!" << std::endl;
	if (rslt == ysize)
	{
		std::cout << "SUCCESS!" << std::endl;
	}
	else
	{
		std::cout << "FINISH with ERRORS!" << std::endl;
		std::cout << ysize - rslt << " lines was not copied!" << std::endl;
	}
	return rslt;
}

int SADataExec::viirs_Interpolation_Spline3(
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
	float r2)
{
	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;
	GDALDatasetH hSrcDS02 = NULL;
	GDALDatasetH hSrcDS03 = NULL;
	GDALDatasetH hSrcDS04 = NULL;
	GDALDatasetH hSrcDS05 = NULL;
	GDALDatasetH hSrcDS06 = NULL;
	GDALDatasetH hSrcDS07 = NULL;
	GDALDatasetH hSrcDS08 = NULL;
	GDALDatasetH hSrcDS09 = NULL;
	GDALDatasetH hSrcDS10 = NULL;
	GDALDatasetH hSrcDS11 = NULL;
	GDALDatasetH hSrcDS12 = NULL;
	GDALDatasetH hSrcDS13 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;
	GDALRasterBandH hSrcBand02 = NULL;
	GDALRasterBandH hSrcBand03 = NULL;
	GDALRasterBandH hSrcBand04 = NULL;
	GDALRasterBandH hSrcBand05 = NULL;
	GDALRasterBandH hSrcBand06 = NULL;
	GDALRasterBandH hSrcBand07 = NULL;
	GDALRasterBandH hSrcBand08 = NULL;
	GDALRasterBandH hSrcBand09 = NULL;
	GDALRasterBandH hSrcBand10 = NULL;
	GDALRasterBandH hSrcBand11 = NULL;
	GDALRasterBandH hSrcBand12 = NULL;
	GDALRasterBandH hSrcBand13 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };

	/*检测目标数据信息*/
	hDstDS = GDALOpen(dst_path, GA_Update);
	if (hDstDS != NULL)
	{
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return -1;
	}

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		return false;
	}

	/*检测数据源信息*/
	hSrcDS07 = GDALOpen(current_path, GA_ReadOnly);
	if (hSrcDS07 == NULL)
	{
		//待插补的数据无法打开
		GDALDestroyDriverManager();
		return false;
	}
	else
	{
		flags[6] = 1;
	}

	hSrcBand07 = GDALGetRasterBand(hSrcDS07, 1);
	if (hSrcBand07 == NULL)
	{
		//待插补的数据无法打开
		GDALClose(hSrcBand07);
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS07);
	ysize = GDALGetRasterYSize(hSrcDS07);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;

	GDALGetGeoTransform(hSrcDS07, adfGeoTransform);

	//获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;
	dfDstNoDataValue = GDALGetRasterNoDataValue(hSrcBand07, &bDstHasNoData);

	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand07);

	//创建拷贝数据集
	hDstDS = GDALCreate(
		hDriver,
		dst_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	//判断是否创建成功
	if (hDstDS == NULL)
	{
		GDALClose(hSrcDS07);
		GDALDestroyDriverManager();
		return false;
	}
	hDstBand = GDALGetRasterBand(hDstDS, 1);
	if (hDstBand == NULL)
	{
		GDALClose(hSrcDS07);
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return false;
	}

	//打开所有文件
	hSrcDS01 = GDALOpen(prev01_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;
	hSrcDS02 = GDALOpen(prev02_path, GA_ReadOnly);
	if (hSrcDS02) flags[1] = 1;
	hSrcDS03 = GDALOpen(prev03_path, GA_ReadOnly);
	if (hSrcDS03) flags[2] = 1;
	hSrcDS04 = GDALOpen(prev04_path, GA_ReadOnly);
	if (hSrcDS04) flags[3] = 1;
	hSrcDS05 = GDALOpen(prev05_path, GA_ReadOnly);
	if (hSrcDS05) flags[4] = 1;
	hSrcDS06 = GDALOpen(prev06_path, GA_ReadOnly);
	if (hSrcDS06) flags[5] = 1;

	hSrcDS08 = GDALOpen(next01_path, GA_ReadOnly);
	if (hSrcDS08) flags[7] = 1;
	hSrcDS09 = GDALOpen(next02_path, GA_ReadOnly);
	if (hSrcDS09) flags[8] = 1;
	hSrcDS10 = GDALOpen(next03_path, GA_ReadOnly);
	if (hSrcDS10) flags[9] = 1;
	hSrcDS11 = GDALOpen(next04_path, GA_ReadOnly);
	if (hSrcDS11) flags[10] = 1;
	hSrcDS12 = GDALOpen(next05_path, GA_ReadOnly);
	if (hSrcDS12) flags[11] = 1;
	hSrcDS13 = GDALOpen(next06_path, GA_ReadOnly);
	if (hSrcDS13) flags[12] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);
	hSrcBand02 = GDALGetRasterBand(hSrcDS02, 1);
	hSrcBand03 = GDALGetRasterBand(hSrcDS03, 1);
	hSrcBand04 = GDALGetRasterBand(hSrcDS04, 1);
	hSrcBand05 = GDALGetRasterBand(hSrcDS05, 1);
	hSrcBand06 = GDALGetRasterBand(hSrcDS06, 1);

	hSrcBand08 = GDALGetRasterBand(hSrcDS08, 1);
	hSrcBand09 = GDALGetRasterBand(hSrcDS09, 1);
	hSrcBand10 = GDALGetRasterBand(hSrcDS10, 1);
	hSrcBand11 = GDALGetRasterBand(hSrcDS11, 1);
	hSrcBand12 = GDALGetRasterBand(hSrcDS12, 1);
	hSrcBand13 = GDALGetRasterBand(hSrcDS13, 1);

	if (hSrcBand01 == NULL) flags[0] = 0;
	if (hSrcBand02 == NULL) flags[1] = 0;
	if (hSrcBand03 == NULL) flags[2] = 0;
	if (hSrcBand04 == NULL) flags[3] = 0;
	if (hSrcBand05 == NULL) flags[4] = 0;
	if (hSrcBand06 == NULL) flags[5] = 0;
	if (hSrcBand07 == NULL) flags[6] = 0;
	if (hSrcBand08 == NULL) flags[7] = 0;
	if (hSrcBand09 == NULL) flags[8] = 0;
	if (hSrcBand10 == NULL) flags[9] = 0;
	if (hSrcBand11 == NULL) flags[10] = 0;
	if (hSrcBand12 == NULL) flags[11] = 0;
	if (hSrcBand13 == NULL) flags[12] = 0;

	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData02 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData03 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData04 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData05 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData06 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData07 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData08 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData09 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData10 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData11 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData12 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData13 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;

	int rslt = 0;

	for (int i = 0;i < ysize;i++)
	{
		//cout<<"Processing line "<<i+1<<"!"<<endl;
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData02, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData03, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData04, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData05, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData06, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData07, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData08, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData09, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData10, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData11, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData12, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData13, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);

		//memset(rData, 0x00, GDALGetDataTypeSize(gType)*xsize/8);

		//处理一行
# pragma region
		err = GDALRasterIO(hSrcBand07, GF_Read, 0, i, xsize, 1, pData07, xsize, 1, gType, 0, 0);
		if (err == CE_None)
		{
#pragma region
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}

			if (flags[1] == 1)
			{
				GDALRasterIO(hSrcBand02, GF_Read, 0, i, xsize, 1, pData02, xsize, 1, gType, 0, 0);
			}

			if (flags[2] == 1)
			{
				GDALRasterIO(hSrcBand03, GF_Read, 0, i, xsize, 1, pData03, xsize, 1, gType, 0, 0);
			}

			if (flags[3] == 1)
			{
				GDALRasterIO(hSrcBand04, GF_Read, 0, i, xsize, 1, pData04, xsize, 1, gType, 0, 0);
			}

			if (flags[4] == 1)
			{
				GDALRasterIO(hSrcBand05, GF_Read, 0, i, xsize, 1, pData05, xsize, 1, gType, 0, 0);
			}

			if (flags[5] == 1)
			{
				GDALRasterIO(hSrcBand06, GF_Read, 0, i, xsize, 1, pData06, xsize, 1, gType, 0, 0);
			}

			if (flags[7] == 1)
			{
				GDALRasterIO(hSrcBand08, GF_Read, 0, i, xsize, 1, pData08, xsize, 1, gType, 0, 0);
			}

			if (flags[8] == 1)
			{
				GDALRasterIO(hSrcBand09, GF_Read, 0, i, xsize, 1, pData09, xsize, 1, gType, 0, 0);
			}
			if (flags[9] == 1)
			{
				GDALRasterIO(hSrcBand10, GF_Read, 0, i, xsize, 1, pData10, xsize, 1, gType, 0, 0);
			}

			if (flags[10] == 1)
			{
				GDALRasterIO(hSrcBand11, GF_Read, 0, i, xsize, 1, pData11, xsize, 1, gType, 0, 0);
			}

			if (flags[11] == 1)
			{
				GDALRasterIO(hSrcBand12, GF_Read, 0, i, xsize, 1, pData12, xsize, 1, gType, 0, 0);
			}

			if (flags[12] == 1)
			{
				GDALRasterIO(hSrcBand13, GF_Read, 0, i, xsize, 1, pData13, xsize, 1, gType, 0, 0);
			}
#pragma endregion

			for (int j = 0;j < xsize;j++)
			{
				// 三次样条插值
				double dns[13] = { 0.0 };
				double idx[13] = { 0.0 };

				dns[0] = pData01[j];
				dns[1] = pData02[j];
				dns[2] = pData03[j];
				dns[3] = pData04[j];
				dns[4] = pData05[j];
				dns[5] = pData06[j];
				dns[6] = pData07[j];
				dns[7] = pData08[j];
				dns[8] = pData09[j];
				dns[9] = pData10[j];
				dns[10] = pData11[j];
				dns[11] = pData12[j];
				dns[12] = pData13[j];

				std::vector<double> x;
				std::vector<double> y;
				for (int m = 0;m < 13;m++)
				{
					if (m != 6 && flags[m] == 1)
					{
						x.push_back(m);
						y.push_back(dns[m]);
					}
				}

				int len = x.size();
				double *x0 = new double[len];		//已知的数据点
				double *y0 = new double[len];
				std::copy(x.begin(), x.end(), x0);
				std::copy(y.begin(), y.end(), y0);
				try
				{
					//Spline sp(x0,y0,5,GivenSecondOrder,0,0);
					SplineInterface* sp = new Spline(x0, y0, len);	//使用接口，且使用默认边界条件
					double x = 6;
					double y;
					sp->SinglePointInterp(x, y);	//求x的插值结果y

					pData07[j] = y;

					delete sp;
					//std::cout << "x=" << x << "时的插值结果为:" << y << std::endl;
				}
				catch (SplineFailure sf)
				{
					//std::cout << sf.GetMessage() << std::endl;
				}
				delete[] x0;
				delete[] y0;
				//pData07[j] = (float)(k * 6 + b);
			}
		}
# pragma endregion


		//写出一行结果
#pragma region
//if (maxDNFilter > 0)
//{
//	for (int s = 0; s < xsize; s++)
//	{
//		if (rData[s] > maxDNFilter)
//		{
//			rData[s] = 0.0;
//		}
//		if (rData[s] < 0.0)
//		{
//			rData[s] = 0.0;
//		}
//	}
//}
		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			i,
			xsize,
			1,
			pData07,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			std::cout << "line " << i << " is processed!" << std::endl;
			rslt++;
		}
		else
		{
			std::cout << "ERROR: line " << i << " was failed!" << std::endl;
		}
#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);
	GDALSetProjection(hDstDS, GDALGetProjectionRef(hSrcDS07));
	if (bDstHasNoData)
		GDALSetRasterNoDataValue(hDstBand, dfDstNoDataValue);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);
	free(pData02);
	free(pData03);
	free(pData04);
	free(pData05);
	free(pData06);
	free(pData07);
	free(pData08);
	free(pData09);
	free(pData10);
	free(pData11);
	free(pData12);
	free(pData13);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);
	GDALClose(hSrcDS02);
	GDALClose(hSrcDS03);
	GDALClose(hSrcDS04);
	GDALClose(hSrcDS05);
	GDALClose(hSrcDS06);
	GDALClose(hSrcDS07);
	GDALClose(hSrcDS08);
	GDALClose(hSrcDS09);
	GDALClose(hSrcDS10);
	GDALClose(hSrcDS11);
	GDALClose(hSrcDS12);
	GDALClose(hSrcDS13);

	GDALDestroyDriverManager();
	std::cout << rslt << " lines is copied!" << std::endl;
	if (rslt == ysize)
	{
		std::cout << "SUCCESS!" << std::endl;
	}
	else
	{
		std::cout << "FINISH with ERRORS!" << std::endl;
		std::cout << ysize - rslt << " lines was not copied!" << std::endl;
	}
	return rslt;
}

int SADataExec::viirs_Interpolation_BezierCurve(
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
	float r2)
{
	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;
	GDALDatasetH hSrcDS02 = NULL;
	GDALDatasetH hSrcDS03 = NULL;
	GDALDatasetH hSrcDS04 = NULL;
	GDALDatasetH hSrcDS05 = NULL;
	GDALDatasetH hSrcDS06 = NULL;
	GDALDatasetH hSrcDS07 = NULL;
	GDALDatasetH hSrcDS08 = NULL;
	GDALDatasetH hSrcDS09 = NULL;
	GDALDatasetH hSrcDS10 = NULL;
	GDALDatasetH hSrcDS11 = NULL;
	GDALDatasetH hSrcDS12 = NULL;
	GDALDatasetH hSrcDS13 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;
	GDALRasterBandH hSrcBand02 = NULL;
	GDALRasterBandH hSrcBand03 = NULL;
	GDALRasterBandH hSrcBand04 = NULL;
	GDALRasterBandH hSrcBand05 = NULL;
	GDALRasterBandH hSrcBand06 = NULL;
	GDALRasterBandH hSrcBand07 = NULL;
	GDALRasterBandH hSrcBand08 = NULL;
	GDALRasterBandH hSrcBand09 = NULL;
	GDALRasterBandH hSrcBand10 = NULL;
	GDALRasterBandH hSrcBand11 = NULL;
	GDALRasterBandH hSrcBand12 = NULL;
	GDALRasterBandH hSrcBand13 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };

	/*检测目标数据信息*/
	hDstDS = GDALOpen(dst_path, GA_Update);
	if (hDstDS != NULL)
	{
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return -1;
	}

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		return false;
	}

	/*检测数据源信息*/
	hSrcDS07 = GDALOpen(current_path, GA_ReadOnly);
	if (hSrcDS07 == NULL)
	{
		//待插补的数据无法打开
		GDALDestroyDriverManager();
		return false;
	}
	else
	{
		flags[6] = 1;
	}

	hSrcBand07 = GDALGetRasterBand(hSrcDS07, 1);
	if (hSrcBand07 == NULL)
	{
		//待插补的数据无法打开
		GDALClose(hSrcBand07);
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS07);
	ysize = GDALGetRasterYSize(hSrcDS07);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;

	GDALGetGeoTransform(hSrcDS07, adfGeoTransform);

	//获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;
	dfDstNoDataValue = GDALGetRasterNoDataValue(hSrcBand07, &bDstHasNoData);

	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand07);

	//创建拷贝数据集
	hDstDS = GDALCreate(
		hDriver,
		dst_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	//判断是否创建成功
	if (hDstDS == NULL)
	{
		GDALClose(hSrcDS07);
		GDALDestroyDriverManager();
		return false;
	}
	hDstBand = GDALGetRasterBand(hDstDS, 1);
	if (hDstBand == NULL)
	{
		GDALClose(hSrcDS07);
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return false;
	}

	//打开所有文件
	hSrcDS01 = GDALOpen(prev01_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;
	hSrcDS02 = GDALOpen(prev02_path, GA_ReadOnly);
	if (hSrcDS02) flags[1] = 1;
	hSrcDS03 = GDALOpen(prev03_path, GA_ReadOnly);
	if (hSrcDS03) flags[2] = 1;
	hSrcDS04 = GDALOpen(prev04_path, GA_ReadOnly);
	if (hSrcDS04) flags[3] = 1;
	hSrcDS05 = GDALOpen(prev05_path, GA_ReadOnly);
	if (hSrcDS05) flags[4] = 1;
	hSrcDS06 = GDALOpen(prev06_path, GA_ReadOnly);
	if (hSrcDS06) flags[5] = 1;

	hSrcDS08 = GDALOpen(next01_path, GA_ReadOnly);
	if (hSrcDS08) flags[7] = 1;
	hSrcDS09 = GDALOpen(next02_path, GA_ReadOnly);
	if (hSrcDS09) flags[8] = 1;
	hSrcDS10 = GDALOpen(next03_path, GA_ReadOnly);
	if (hSrcDS10) flags[9] = 1;
	hSrcDS11 = GDALOpen(next04_path, GA_ReadOnly);
	if (hSrcDS11) flags[10] = 1;
	hSrcDS12 = GDALOpen(next05_path, GA_ReadOnly);
	if (hSrcDS12) flags[11] = 1;
	hSrcDS13 = GDALOpen(next06_path, GA_ReadOnly);
	if (hSrcDS13) flags[12] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);
	hSrcBand02 = GDALGetRasterBand(hSrcDS02, 1);
	hSrcBand03 = GDALGetRasterBand(hSrcDS03, 1);
	hSrcBand04 = GDALGetRasterBand(hSrcDS04, 1);
	hSrcBand05 = GDALGetRasterBand(hSrcDS05, 1);
	hSrcBand06 = GDALGetRasterBand(hSrcDS06, 1);

	hSrcBand08 = GDALGetRasterBand(hSrcDS08, 1);
	hSrcBand09 = GDALGetRasterBand(hSrcDS09, 1);
	hSrcBand10 = GDALGetRasterBand(hSrcDS10, 1);
	hSrcBand11 = GDALGetRasterBand(hSrcDS11, 1);
	hSrcBand12 = GDALGetRasterBand(hSrcDS12, 1);
	hSrcBand13 = GDALGetRasterBand(hSrcDS13, 1);

	if (hSrcBand01 == NULL) flags[0] = 0;
	if (hSrcBand02 == NULL) flags[1] = 0;
	if (hSrcBand03 == NULL) flags[2] = 0;
	if (hSrcBand04 == NULL) flags[3] = 0;
	if (hSrcBand05 == NULL) flags[4] = 0;
	if (hSrcBand06 == NULL) flags[5] = 0;
	if (hSrcBand07 == NULL) flags[6] = 0;
	if (hSrcBand08 == NULL) flags[7] = 0;
	if (hSrcBand09 == NULL) flags[8] = 0;
	if (hSrcBand10 == NULL) flags[9] = 0;
	if (hSrcBand11 == NULL) flags[10] = 0;
	if (hSrcBand12 == NULL) flags[11] = 0;
	if (hSrcBand13 == NULL) flags[12] = 0;

	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData02 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData03 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData04 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData05 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData06 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData07 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData08 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData09 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData10 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData11 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData12 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData13 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;

	int rslt = 0;

	for (int i = 0;i < ysize;i++)
	{
		//cout<<"Processing line "<<i+1<<"!"<<endl;
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData02, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData03, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData04, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData05, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData06, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData07, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData08, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData09, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData10, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData11, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData12, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData13, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);

		//memset(rData, 0x00, GDALGetDataTypeSize(gType)*xsize/8);

		//处理一行
# pragma region
		err = GDALRasterIO(hSrcBand07, GF_Read, 0, i, xsize, 1, pData07, xsize, 1, gType, 0, 0);
		if (err == CE_None)
		{
#pragma region
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}

			if (flags[1] == 1)
			{
				GDALRasterIO(hSrcBand02, GF_Read, 0, i, xsize, 1, pData02, xsize, 1, gType, 0, 0);
			}

			if (flags[2] == 1)
			{
				GDALRasterIO(hSrcBand03, GF_Read, 0, i, xsize, 1, pData03, xsize, 1, gType, 0, 0);
			}

			if (flags[3] == 1)
			{
				GDALRasterIO(hSrcBand04, GF_Read, 0, i, xsize, 1, pData04, xsize, 1, gType, 0, 0);
			}

			if (flags[4] == 1)
			{
				GDALRasterIO(hSrcBand05, GF_Read, 0, i, xsize, 1, pData05, xsize, 1, gType, 0, 0);
			}

			if (flags[5] == 1)
			{
				GDALRasterIO(hSrcBand06, GF_Read, 0, i, xsize, 1, pData06, xsize, 1, gType, 0, 0);
			}

			if (flags[7] == 1)
			{
				GDALRasterIO(hSrcBand08, GF_Read, 0, i, xsize, 1, pData08, xsize, 1, gType, 0, 0);
			}

			if (flags[8] == 1)
			{
				GDALRasterIO(hSrcBand09, GF_Read, 0, i, xsize, 1, pData09, xsize, 1, gType, 0, 0);
			}
			if (flags[9] == 1)
			{
				GDALRasterIO(hSrcBand10, GF_Read, 0, i, xsize, 1, pData10, xsize, 1, gType, 0, 0);
			}

			if (flags[10] == 1)
			{
				GDALRasterIO(hSrcBand11, GF_Read, 0, i, xsize, 1, pData11, xsize, 1, gType, 0, 0);
			}

			if (flags[11] == 1)
			{
				GDALRasterIO(hSrcBand12, GF_Read, 0, i, xsize, 1, pData12, xsize, 1, gType, 0, 0);
			}

			if (flags[12] == 1)
			{
				GDALRasterIO(hSrcBand13, GF_Read, 0, i, xsize, 1, pData13, xsize, 1, gType, 0, 0);
			}
#pragma endregion

			for (int j = 0;j < xsize;j++)
			{
				// 贝塞尔曲线插值
				double dns[13] = { 0.0 };
				double idx[13] = { 0.0 };

				dns[0] = pData01[j];
				dns[1] = pData02[j];
				dns[2] = pData03[j];
				dns[3] = pData04[j];
				dns[4] = pData05[j];
				dns[5] = pData06[j];
				dns[6] = pData07[j];
				dns[7] = pData08[j];
				dns[8] = pData09[j];
				dns[9] = pData10[j];
				dns[10] = pData11[j];
				dns[11] = pData12[j];
				dns[12] = pData13[j];

				std::vector<double> x;
				std::vector<double> y;

				std::vector<BezierPoint> inputPoints;
				for (int m = 0;m < 13;m++)
				{
					if (m != 6 && flags[m] == 1)
					{
						BezierPoint po(m, dns[m]);
						inputPoints.push_back(po);
					}
				}

				int len = x.size();
				double* x0 = new double[len];		//已知的数据点
				double* y0 = new double[len];
				std::copy(x.begin(), x.end(), x0);
				std::copy(y.begin(), y.end(), y0);
				try
				{
					BezierCurve* bc = new BezierCurve(inputPoints);

					double x = 6;
					double y;
					pData07[j] = bc->SinglePointInterp(x);	//求x的插值结果y

					delete bc;
					//std::cout << "x=" << x << "时的插值结果为:" << y << std::endl;
				}
				catch (BezierExecption be)
				{
					//std::cout <<be.GetMessage() << std::endl;
				}
				delete[] x0;
				delete[] y0;
			}
		}
# pragma endregion


		//写出一行结果
#pragma region
//if (maxDNFilter > 0)
//{
//	for (int s = 0; s < xsize; s++)
//	{
//		if (rData[s] > maxDNFilter)
//		{
//			rData[s] = 0.0;
//		}
//		if (rData[s] < 0.0)
//		{
//			rData[s] = 0.0;
//		}
//	}
//}
		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			i,
			xsize,
			1,
			pData07,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			std::cout << "line " << i << " is processed!" << std::endl;
			rslt++;
		}
		else
		{
			std::cout << "ERROR: line " << i << " was failed!" << std::endl;
		}
#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);
	GDALSetProjection(hDstDS, GDALGetProjectionRef(hSrcDS07));
	if (bDstHasNoData)
		GDALSetRasterNoDataValue(hDstBand, dfDstNoDataValue);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);
	free(pData02);
	free(pData03);
	free(pData04);
	free(pData05);
	free(pData06);
	free(pData07);
	free(pData08);
	free(pData09);
	free(pData10);
	free(pData11);
	free(pData12);
	free(pData13);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);
	GDALClose(hSrcDS02);
	GDALClose(hSrcDS03);
	GDALClose(hSrcDS04);
	GDALClose(hSrcDS05);
	GDALClose(hSrcDS06);
	GDALClose(hSrcDS07);
	GDALClose(hSrcDS08);
	GDALClose(hSrcDS09);
	GDALClose(hSrcDS10);
	GDALClose(hSrcDS11);
	GDALClose(hSrcDS12);
	GDALClose(hSrcDS13);

	GDALDestroyDriverManager();
	std::cout << rslt << " lines is copied!" << std::endl;
	if (rslt == ysize)
	{
		std::cout << "SUCCESS!" << std::endl;
	}
	else
	{
		std::cout << "FINISH with ERRORS!" << std::endl;
		std::cout << ysize - rslt << " lines was not copied!" << std::endl;
	}
	return rslt;
}

int SADataExec::viirs_Interpolation_Hermite(
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
	float r2)
{
	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;
	GDALDatasetH hSrcDS02 = NULL;
	GDALDatasetH hSrcDS03 = NULL;
	GDALDatasetH hSrcDS04 = NULL;
	GDALDatasetH hSrcDS05 = NULL;
	GDALDatasetH hSrcDS06 = NULL;
	GDALDatasetH hSrcDS07 = NULL;
	GDALDatasetH hSrcDS08 = NULL;
	GDALDatasetH hSrcDS09 = NULL;
	GDALDatasetH hSrcDS10 = NULL;
	GDALDatasetH hSrcDS11 = NULL;
	GDALDatasetH hSrcDS12 = NULL;
	GDALDatasetH hSrcDS13 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;
	GDALRasterBandH hSrcBand02 = NULL;
	GDALRasterBandH hSrcBand03 = NULL;
	GDALRasterBandH hSrcBand04 = NULL;
	GDALRasterBandH hSrcBand05 = NULL;
	GDALRasterBandH hSrcBand06 = NULL;
	GDALRasterBandH hSrcBand07 = NULL;
	GDALRasterBandH hSrcBand08 = NULL;
	GDALRasterBandH hSrcBand09 = NULL;
	GDALRasterBandH hSrcBand10 = NULL;
	GDALRasterBandH hSrcBand11 = NULL;
	GDALRasterBandH hSrcBand12 = NULL;
	GDALRasterBandH hSrcBand13 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };

	/*检测目标数据信息*/
	hDstDS = GDALOpen(dst_path, GA_Update);
	if (hDstDS != NULL)
	{
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return -1;
	}

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		return false;
	}

	/*检测数据源信息*/
	hSrcDS07 = GDALOpen(current_path, GA_ReadOnly);
	if (hSrcDS07 == NULL)
	{
		//待插补的数据无法打开
		GDALDestroyDriverManager();
		return false;
	}
	else
	{
		flags[6] = 1;
	}

	hSrcBand07 = GDALGetRasterBand(hSrcDS07, 1);
	if (hSrcBand07 == NULL)
	{
		//待插补的数据无法打开
		GDALClose(hSrcBand07);
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS07);
	ysize = GDALGetRasterYSize(hSrcDS07);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;

	GDALGetGeoTransform(hSrcDS07, adfGeoTransform);

	//获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;
	dfDstNoDataValue = GDALGetRasterNoDataValue(hSrcBand07, &bDstHasNoData);

	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand07);

	//创建拷贝数据集
	hDstDS = GDALCreate(
		hDriver,
		dst_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	//判断是否创建成功
	if (hDstDS == NULL)
	{
		GDALClose(hSrcDS07);
		GDALDestroyDriverManager();
		return false;
	}
	hDstBand = GDALGetRasterBand(hDstDS, 1);
	if (hDstBand == NULL)
	{
		GDALClose(hSrcDS07);
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return false;
	}

	//打开所有文件
	hSrcDS01 = GDALOpen(prev01_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;
	hSrcDS02 = GDALOpen(prev02_path, GA_ReadOnly);
	if (hSrcDS02) flags[1] = 1;
	hSrcDS03 = GDALOpen(prev03_path, GA_ReadOnly);
	if (hSrcDS03) flags[2] = 1;
	hSrcDS04 = GDALOpen(prev04_path, GA_ReadOnly);
	if (hSrcDS04) flags[3] = 1;
	hSrcDS05 = GDALOpen(prev05_path, GA_ReadOnly);
	if (hSrcDS05) flags[4] = 1;
	hSrcDS06 = GDALOpen(prev06_path, GA_ReadOnly);
	if (hSrcDS06) flags[5] = 1;

	hSrcDS08 = GDALOpen(next01_path, GA_ReadOnly);
	if (hSrcDS08) flags[7] = 1;
	hSrcDS09 = GDALOpen(next02_path, GA_ReadOnly);
	if (hSrcDS09) flags[8] = 1;
	hSrcDS10 = GDALOpen(next03_path, GA_ReadOnly);
	if (hSrcDS10) flags[9] = 1;
	hSrcDS11 = GDALOpen(next04_path, GA_ReadOnly);
	if (hSrcDS11) flags[10] = 1;
	hSrcDS12 = GDALOpen(next05_path, GA_ReadOnly);
	if (hSrcDS12) flags[11] = 1;
	hSrcDS13 = GDALOpen(next06_path, GA_ReadOnly);
	if (hSrcDS13) flags[12] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);
	hSrcBand02 = GDALGetRasterBand(hSrcDS02, 1);
	hSrcBand03 = GDALGetRasterBand(hSrcDS03, 1);
	hSrcBand04 = GDALGetRasterBand(hSrcDS04, 1);
	hSrcBand05 = GDALGetRasterBand(hSrcDS05, 1);
	hSrcBand06 = GDALGetRasterBand(hSrcDS06, 1);

	hSrcBand08 = GDALGetRasterBand(hSrcDS08, 1);
	hSrcBand09 = GDALGetRasterBand(hSrcDS09, 1);
	hSrcBand10 = GDALGetRasterBand(hSrcDS10, 1);
	hSrcBand11 = GDALGetRasterBand(hSrcDS11, 1);
	hSrcBand12 = GDALGetRasterBand(hSrcDS12, 1);
	hSrcBand13 = GDALGetRasterBand(hSrcDS13, 1);

	if (hSrcBand01 == NULL) flags[0] = 0;
	if (hSrcBand02 == NULL) flags[1] = 0;
	if (hSrcBand03 == NULL) flags[2] = 0;
	if (hSrcBand04 == NULL) flags[3] = 0;
	if (hSrcBand05 == NULL) flags[4] = 0;
	if (hSrcBand06 == NULL) flags[5] = 0;
	if (hSrcBand07 == NULL) flags[6] = 0;
	if (hSrcBand08 == NULL) flags[7] = 0;
	if (hSrcBand09 == NULL) flags[8] = 0;
	if (hSrcBand10 == NULL) flags[9] = 0;
	if (hSrcBand11 == NULL) flags[10] = 0;
	if (hSrcBand12 == NULL) flags[11] = 0;
	if (hSrcBand13 == NULL) flags[12] = 0;

	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData02 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData03 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData04 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData05 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData06 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData07 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData08 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData09 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData10 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData11 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData12 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData13 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;

	int rslt = 0;

	// 对调用的matlab函数进行初始化操作
	if (!mclInitializeApplication(NULL, 0)) {
		std::cout << "Could not initialize the application.\n";
		return -1;
	}
	bool isOk = 0;
	std::cout << "isOk = " << isOk << std::endl;
	isOk = hermiteInterpolationInitialize();
	std::cout << "hermiteInterpolationInitialize  isOk = " << isOk << std::endl;

	//omp_set_num_threads(1);

	for (int i = 7000;i < 11000;i++)
	{
		//cout<<"Processing line "<<i+1<<"!"<<endl;
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData02, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData03, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData04, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData05, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData06, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData07, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData08, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData09, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData10, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData11, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData12, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData13, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);

		//memset(rData, 0x00, GDALGetDataTypeSize(gType)*xsize/8);

		//处理一行
# pragma region
		err = GDALRasterIO(hSrcBand07, GF_Read, 0, i, xsize, 1, pData07, xsize, 1, gType, 0, 0);
		if (err == CE_None)
		{
#pragma region
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}

			if (flags[1] == 1)
			{
				GDALRasterIO(hSrcBand02, GF_Read, 0, i, xsize, 1, pData02, xsize, 1, gType, 0, 0);
			}

			if (flags[2] == 1)
			{
				GDALRasterIO(hSrcBand03, GF_Read, 0, i, xsize, 1, pData03, xsize, 1, gType, 0, 0);
			}

			if (flags[3] == 1)
			{
				GDALRasterIO(hSrcBand04, GF_Read, 0, i, xsize, 1, pData04, xsize, 1, gType, 0, 0);
			}

			if (flags[4] == 1)
			{
				GDALRasterIO(hSrcBand05, GF_Read, 0, i, xsize, 1, pData05, xsize, 1, gType, 0, 0);
			}

			if (flags[5] == 1)
			{
				GDALRasterIO(hSrcBand06, GF_Read, 0, i, xsize, 1, pData06, xsize, 1, gType, 0, 0);
			}

			if (flags[7] == 1)
			{
				GDALRasterIO(hSrcBand08, GF_Read, 0, i, xsize, 1, pData08, xsize, 1, gType, 0, 0);
			}

			if (flags[8] == 1)
			{
				GDALRasterIO(hSrcBand09, GF_Read, 0, i, xsize, 1, pData09, xsize, 1, gType, 0, 0);
			}
			if (flags[9] == 1)
			{
				GDALRasterIO(hSrcBand10, GF_Read, 0, i, xsize, 1, pData10, xsize, 1, gType, 0, 0);
			}

			if (flags[10] == 1)
			{
				GDALRasterIO(hSrcBand11, GF_Read, 0, i, xsize, 1, pData11, xsize, 1, gType, 0, 0);
			}

			if (flags[11] == 1)
			{
				GDALRasterIO(hSrcBand12, GF_Read, 0, i, xsize, 1, pData12, xsize, 1, gType, 0, 0);
			}

			if (flags[12] == 1)
			{
				GDALRasterIO(hSrcBand13, GF_Read, 0, i, xsize, 1, pData13, xsize, 1, gType, 0, 0);
			}
#pragma endregion
		

			std::vector<double> xx;
			std::vector<double> yy;

			for (int j = 0;j < xsize;j++)
			{
				double dns[13] = { 0.0 };
				double idx[13] = { 0.0 };

				dns[0] = pData01[j];
				dns[1] = pData02[j];
				dns[2] = pData03[j];
				dns[3] = pData04[j];
				dns[4] = pData05[j];
				dns[5] = pData06[j];
				dns[6] = pData07[j];
				dns[7] = pData08[j];
				dns[8] = pData09[j];
				dns[9] = pData10[j];
				dns[10] = pData11[j];
				dns[11] = pData12[j];
				dns[12] = pData13[j];

				std::vector<double> x;
				std::vector<double> y;
				int flag = 0;
				for (int m = 0;m < 13;m++)
				{
					if (m != 6 && flags[m] == 1)
					{
						xx.push_back(m);
						yy.push_back(dns[m]);
						flag++;
					}
				}
				if (flag < 12) {
					std::cout << "i = " << i << " j = " << j << std::endl;
				}
	
			}
			//double end = omp_get_wtime();
			//std::cout << "Multi-thread Time is: " << end - start << std::endl;

			int yLen = 12;
			int xLen = 28800;
			int totalCount = yLen * xLen;
			mwArray a(1, totalCount, mxDOUBLE_CLASS);
			mwArray b(1, totalCount, mxDOUBLE_CLASS);
			mwArray x_interpolation(1, 1, mxDOUBLE_CLASS);
			mwArray y_interpolation(1, xLen, mxDOUBLE_CLASS); // 插值的结果

			double* xxArr = new double[28800 *12];
			double* yyArr = new double[28800 * 12];

			std::copy(xx.begin(), xx.end(), xxArr);
			std::copy(yy.begin(), yy.end(), yyArr);

			a.SetData(xxArr, totalCount);
			b.SetData(yyArr, totalCount);
			x_interpolation(1, 1) = 6.0;

			Hermite(1, y_interpolation, a, b, x_interpolation);

			//int count = y_interpolation.NumberOfElements();
			//int dims = y_interpolation.NumberOfDimensions();
			//int d = y_interpolation.GetDimensions();
			double output_y[28800];
			y_interpolation.Real().GetData(output_y, 28800);

			for (size_t i = 0; i < 28800; i++)
			{
				pData07[i] = output_y[i];
			}

			delete[] xxArr;
			delete[] yyArr;
		}


		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			i,
			xsize,
			1,
			pData07,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			std::cout << "line " << i << " is processed!" << std::endl;
			rslt++;
		}
		else
		{
			std::cout << "ERROR: line " << i << " was failed!" << std::endl;
		}
#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);
	GDALSetProjection(hDstDS, GDALGetProjectionRef(hSrcDS07));
	if (bDstHasNoData)
		GDALSetRasterNoDataValue(hDstBand, dfDstNoDataValue);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);
	free(pData02);
	free(pData03);
	free(pData04);
	free(pData05);
	free(pData06);
	free(pData07);
	free(pData08);
	free(pData09);
	free(pData10);
	free(pData11);
	free(pData12);
	free(pData13);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);
	GDALClose(hSrcDS02);
	GDALClose(hSrcDS03);
	GDALClose(hSrcDS04);
	GDALClose(hSrcDS05);
	GDALClose(hSrcDS06);
	GDALClose(hSrcDS07);
	GDALClose(hSrcDS08);
	GDALClose(hSrcDS09);
	GDALClose(hSrcDS10);
	GDALClose(hSrcDS11);
	GDALClose(hSrcDS12);
	GDALClose(hSrcDS13);

	GDALDestroyDriverManager();
	std::cout << rslt << " lines is copied!" << std::endl;
	if (rslt == ysize)
	{
		std::cout << "SUCCESS!" << std::endl;
	}
	else
	{
		std::cout << "FINISH with ERRORS!" << std::endl;
		std::cout << ysize - rslt << " lines was not copied!" << std::endl;
	}
	return rslt;
}

int SADataExec::viirs_Interpolation_CES(
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
	float r2)
{
	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;
	GDALDatasetH hSrcDS02 = NULL;
	GDALDatasetH hSrcDS03 = NULL;
	GDALDatasetH hSrcDS04 = NULL;
	GDALDatasetH hSrcDS05 = NULL;
	GDALDatasetH hSrcDS06 = NULL;
	GDALDatasetH hSrcDS07 = NULL;
	GDALDatasetH hSrcDS08 = NULL;
	GDALDatasetH hSrcDS09 = NULL;
	GDALDatasetH hSrcDS10 = NULL;
	GDALDatasetH hSrcDS11 = NULL;
	GDALDatasetH hSrcDS12 = NULL;
	GDALDatasetH hSrcDS13 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;
	GDALRasterBandH hSrcBand02 = NULL;
	GDALRasterBandH hSrcBand03 = NULL;
	GDALRasterBandH hSrcBand04 = NULL;
	GDALRasterBandH hSrcBand05 = NULL;
	GDALRasterBandH hSrcBand06 = NULL;
	GDALRasterBandH hSrcBand07 = NULL;
	GDALRasterBandH hSrcBand08 = NULL;
	GDALRasterBandH hSrcBand09 = NULL;
	GDALRasterBandH hSrcBand10 = NULL;
	GDALRasterBandH hSrcBand11 = NULL;
	GDALRasterBandH hSrcBand12 = NULL;
	GDALRasterBandH hSrcBand13 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };

	/*检测目标数据信息*/
	hDstDS = GDALOpen(dst_path, GA_Update);
	if (hDstDS != NULL)
	{
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return -1;
	}

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		return false;
	}

	/*检测数据源信息*/
	hSrcDS07 = GDALOpen(current_path, GA_ReadOnly);
	if (hSrcDS07 == NULL)
	{
		//待插补的数据无法打开
		GDALDestroyDriverManager();
		return false;
	}
	else
	{
		flags[6] = 1;
	}

	hSrcBand07 = GDALGetRasterBand(hSrcDS07, 1);
	if (hSrcBand07 == NULL)
	{
		//待插补的数据无法打开
		GDALClose(hSrcBand07);
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS07);
	ysize = GDALGetRasterYSize(hSrcDS07);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;

	GDALGetGeoTransform(hSrcDS07, adfGeoTransform);

	//获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;
	dfDstNoDataValue = GDALGetRasterNoDataValue(hSrcBand07, &bDstHasNoData);

	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand07);

	//创建拷贝数据集
	hDstDS = GDALCreate(
		hDriver,
		dst_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	//判断是否创建成功
	if (hDstDS == NULL)
	{
		GDALClose(hSrcDS07);
		GDALDestroyDriverManager();
		return false;
	}
	hDstBand = GDALGetRasterBand(hDstDS, 1);
	if (hDstBand == NULL)
	{
		GDALClose(hSrcDS07);
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return false;
	}

	//打开所有文件
	hSrcDS01 = GDALOpen(prev01_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;
	hSrcDS02 = GDALOpen(prev02_path, GA_ReadOnly);
	if (hSrcDS02) flags[1] = 1;
	hSrcDS03 = GDALOpen(prev03_path, GA_ReadOnly);
	if (hSrcDS03) flags[2] = 1;
	hSrcDS04 = GDALOpen(prev04_path, GA_ReadOnly);
	if (hSrcDS04) flags[3] = 1;
	hSrcDS05 = GDALOpen(prev05_path, GA_ReadOnly);
	if (hSrcDS05) flags[4] = 1;
	hSrcDS06 = GDALOpen(prev06_path, GA_ReadOnly);
	if (hSrcDS06) flags[5] = 1;

	hSrcDS08 = GDALOpen(next01_path, GA_ReadOnly);
	if (hSrcDS08) flags[7] = 1;
	hSrcDS09 = GDALOpen(next02_path, GA_ReadOnly);
	if (hSrcDS09) flags[8] = 1;
	hSrcDS10 = GDALOpen(next03_path, GA_ReadOnly);
	if (hSrcDS10) flags[9] = 1;
	hSrcDS11 = GDALOpen(next04_path, GA_ReadOnly);
	if (hSrcDS11) flags[10] = 1;
	hSrcDS12 = GDALOpen(next05_path, GA_ReadOnly);
	if (hSrcDS12) flags[11] = 1;
	hSrcDS13 = GDALOpen(next06_path, GA_ReadOnly);
	if (hSrcDS13) flags[12] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);
	hSrcBand02 = GDALGetRasterBand(hSrcDS02, 1);
	hSrcBand03 = GDALGetRasterBand(hSrcDS03, 1);
	hSrcBand04 = GDALGetRasterBand(hSrcDS04, 1);
	hSrcBand05 = GDALGetRasterBand(hSrcDS05, 1);
	hSrcBand06 = GDALGetRasterBand(hSrcDS06, 1);

	hSrcBand08 = GDALGetRasterBand(hSrcDS08, 1);
	hSrcBand09 = GDALGetRasterBand(hSrcDS09, 1);
	hSrcBand10 = GDALGetRasterBand(hSrcDS10, 1);
	hSrcBand11 = GDALGetRasterBand(hSrcDS11, 1);
	hSrcBand12 = GDALGetRasterBand(hSrcDS12, 1);
	hSrcBand13 = GDALGetRasterBand(hSrcDS13, 1);

	if (hSrcBand01 == NULL) flags[0] = 0;
	if (hSrcBand02 == NULL) flags[1] = 0;
	if (hSrcBand03 == NULL) flags[2] = 0;
	if (hSrcBand04 == NULL) flags[3] = 0;
	if (hSrcBand05 == NULL) flags[4] = 0;
	if (hSrcBand06 == NULL) flags[5] = 0;
	if (hSrcBand07 == NULL) flags[6] = 0;
	if (hSrcBand08 == NULL) flags[7] = 0;
	if (hSrcBand09 == NULL) flags[8] = 0;
	if (hSrcBand10 == NULL) flags[9] = 0;
	if (hSrcBand11 == NULL) flags[10] = 0;
	if (hSrcBand12 == NULL) flags[11] = 0;
	if (hSrcBand13 == NULL) flags[12] = 0;

	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData02 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData03 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData04 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData05 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData06 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData07 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData08 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData09 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData10 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData11 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData12 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData13 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;

	int rslt = 0;

	for (int i = 0;i < ysize;i++)
	{
		//cout<<"Processing line "<<i+1<<"!"<<endl;
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData02, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData03, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData04, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData05, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData06, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData07, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData08, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData09, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData10, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData11, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData12, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData13, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);

		//memset(rData, 0x00, GDALGetDataTypeSize(gType)*xsize/8);

		//处理一行
# pragma region
		err = GDALRasterIO(hSrcBand07, GF_Read, 0, i, xsize, 1, pData07, xsize, 1, gType, 0, 0);
		if (err == CE_None)
		{
#pragma region
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}

			if (flags[1] == 1)
			{
				GDALRasterIO(hSrcBand02, GF_Read, 0, i, xsize, 1, pData02, xsize, 1, gType, 0, 0);
			}

			if (flags[2] == 1)
			{
				GDALRasterIO(hSrcBand03, GF_Read, 0, i, xsize, 1, pData03, xsize, 1, gType, 0, 0);
			}

			if (flags[3] == 1)
			{
				GDALRasterIO(hSrcBand04, GF_Read, 0, i, xsize, 1, pData04, xsize, 1, gType, 0, 0);
			}

			if (flags[4] == 1)
			{
				GDALRasterIO(hSrcBand05, GF_Read, 0, i, xsize, 1, pData05, xsize, 1, gType, 0, 0);
			}

			if (flags[5] == 1)
			{
				GDALRasterIO(hSrcBand06, GF_Read, 0, i, xsize, 1, pData06, xsize, 1, gType, 0, 0);
			}

			if (flags[7] == 1)
			{
				GDALRasterIO(hSrcBand08, GF_Read, 0, i, xsize, 1, pData08, xsize, 1, gType, 0, 0);
			}

			if (flags[8] == 1)
			{
				GDALRasterIO(hSrcBand09, GF_Read, 0, i, xsize, 1, pData09, xsize, 1, gType, 0, 0);
			}
			if (flags[9] == 1)
			{
				GDALRasterIO(hSrcBand10, GF_Read, 0, i, xsize, 1, pData10, xsize, 1, gType, 0, 0);
			}

			if (flags[10] == 1)
			{
				GDALRasterIO(hSrcBand11, GF_Read, 0, i, xsize, 1, pData11, xsize, 1, gType, 0, 0);
			}

			if (flags[11] == 1)
			{
				GDALRasterIO(hSrcBand12, GF_Read, 0, i, xsize, 1, pData12, xsize, 1, gType, 0, 0);
			}

			if (flags[12] == 1)
			{
				GDALRasterIO(hSrcBand13, GF_Read, 0, i, xsize, 1, pData13, xsize, 1, gType, 0, 0);
			}
#pragma endregion
//#pragma omp parallel for num_threads(16)
			for (int j = 0;j < xsize;j++)
			{
				double dns[13] = { 0.0 };
				double idx[13] = { 0.0 };

				dns[0] = pData01[j];
				dns[1] = pData02[j];
				dns[2] = pData03[j];
				dns[3] = pData04[j];
				dns[4] = pData05[j];
				dns[5] = pData06[j];
				dns[6] = pData07[j];
				dns[7] = pData08[j];
				dns[8] = pData09[j];
				dns[9] = pData10[j];
				dns[10] = pData11[j];
				dns[11] = pData12[j];
				dns[12] = pData13[j];

				std::vector<double> x;
				std::vector<double> y;
				for (int m = 0;m < 13;m++)
				{
					if (m != 6 && flags[m] == 1)
					{
						x.push_back(m);
						y.push_back(dns[m]);
					}
				}

				int len = x.size();
				double* x0 = new double[len];		//已知的数据点
				double* y0 = new double[len];
				std::copy(x.begin(), x.end(), x0);
				std::copy(y.begin(), y.end(), y0);

				pData07[j] = cubicExponentialSmoothingValue(x, y);
				delete[] x0;
				delete[] y0;
			}
		}
# pragma endregion


		//写出一行结果
#pragma region
//if (maxDNFilter > 0)
//{
//	for (int s = 0; s < xsize; s++)
//	{
//		if (rData[s] > maxDNFilter)
//		{
//			rData[s] = 0.0;
//		}
//		if (rData[s] < 0.0)
//		{
//			rData[s] = 0.0;
//		}
//	}
//}
		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			i,
			xsize,
			1,
			pData07,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			std::cout << "line " << i << " is processed!" << std::endl;
			rslt++;
		}
		else
		{
			std::cout << "ERROR: line " << i << " was failed!" << std::endl;
		}
#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);
	GDALSetProjection(hDstDS, GDALGetProjectionRef(hSrcDS07));
	if (bDstHasNoData)
		GDALSetRasterNoDataValue(hDstBand, dfDstNoDataValue);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);
	free(pData02);
	free(pData03);
	free(pData04);
	free(pData05);
	free(pData06);
	free(pData07);
	free(pData08);
	free(pData09);
	free(pData10);
	free(pData11);
	free(pData12);
	free(pData13);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);
	GDALClose(hSrcDS02);
	GDALClose(hSrcDS03);
	GDALClose(hSrcDS04);
	GDALClose(hSrcDS05);
	GDALClose(hSrcDS06);
	GDALClose(hSrcDS07);
	GDALClose(hSrcDS08);
	GDALClose(hSrcDS09);
	GDALClose(hSrcDS10);
	GDALClose(hSrcDS11);
	GDALClose(hSrcDS12);
	GDALClose(hSrcDS13);

	GDALDestroyDriverManager();
	std::cout << rslt << " lines is copied!" << std::endl;
	if (rslt == ysize)
	{
		std::cout << "SUCCESS!" << std::endl;
	}
	else
	{
		std::cout << "FINISH with ERRORS!" << std::endl;
		std::cout << ysize - rslt << " lines was not copied!" << std::endl;
	}
	return rslt;
}

int SADataExec::viirs_Interpolation_GFM(
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
	float r2)
{
	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;
	GDALDatasetH hSrcDS02 = NULL;
	GDALDatasetH hSrcDS03 = NULL;
	GDALDatasetH hSrcDS04 = NULL;
	GDALDatasetH hSrcDS05 = NULL;
	GDALDatasetH hSrcDS06 = NULL;
	GDALDatasetH hSrcDS07 = NULL;
	GDALDatasetH hSrcDS08 = NULL;
	GDALDatasetH hSrcDS09 = NULL;
	GDALDatasetH hSrcDS10 = NULL;
	GDALDatasetH hSrcDS11 = NULL;
	GDALDatasetH hSrcDS12 = NULL;
	GDALDatasetH hSrcDS13 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;
	GDALRasterBandH hSrcBand02 = NULL;
	GDALRasterBandH hSrcBand03 = NULL;
	GDALRasterBandH hSrcBand04 = NULL;
	GDALRasterBandH hSrcBand05 = NULL;
	GDALRasterBandH hSrcBand06 = NULL;
	GDALRasterBandH hSrcBand07 = NULL;
	GDALRasterBandH hSrcBand08 = NULL;
	GDALRasterBandH hSrcBand09 = NULL;
	GDALRasterBandH hSrcBand10 = NULL;
	GDALRasterBandH hSrcBand11 = NULL;
	GDALRasterBandH hSrcBand12 = NULL;
	GDALRasterBandH hSrcBand13 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };

	/*检测目标数据信息*/
	hDstDS = GDALOpen(dst_path, GA_Update);
	if (hDstDS != NULL)
	{
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return -1;
	}

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		return false;
	}

	/*检测数据源信息*/
	hSrcDS07 = GDALOpen(current_path, GA_ReadOnly);
	if (hSrcDS07 == NULL)
	{
		//待插补的数据无法打开
		GDALDestroyDriverManager();
		return false;
	}
	else
	{
		flags[6] = 1;
	}

	hSrcBand07 = GDALGetRasterBand(hSrcDS07, 1);
	if (hSrcBand07 == NULL)
	{
		//待插补的数据无法打开
		GDALClose(hSrcBand07);
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS07);
	ysize = GDALGetRasterYSize(hSrcDS07);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;

	GDALGetGeoTransform(hSrcDS07, adfGeoTransform);

	//获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;
	dfDstNoDataValue = GDALGetRasterNoDataValue(hSrcBand07, &bDstHasNoData);

	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand07);

	//创建拷贝数据集
	hDstDS = GDALCreate(
		hDriver,
		dst_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	//判断是否创建成功
	if (hDstDS == NULL)
	{
		GDALClose(hSrcDS07);
		GDALDestroyDriverManager();
		return false;
	}
	hDstBand = GDALGetRasterBand(hDstDS, 1);
	if (hDstBand == NULL)
	{
		GDALClose(hSrcDS07);
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return false;
	}

	//打开所有文件
	hSrcDS01 = GDALOpen(prev01_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;
	hSrcDS02 = GDALOpen(prev02_path, GA_ReadOnly);
	if (hSrcDS02) flags[1] = 1;
	hSrcDS03 = GDALOpen(prev03_path, GA_ReadOnly);
	if (hSrcDS03) flags[2] = 1;
	hSrcDS04 = GDALOpen(prev04_path, GA_ReadOnly);
	if (hSrcDS04) flags[3] = 1;
	hSrcDS05 = GDALOpen(prev05_path, GA_ReadOnly);
	if (hSrcDS05) flags[4] = 1;
	hSrcDS06 = GDALOpen(prev06_path, GA_ReadOnly);
	if (hSrcDS06) flags[5] = 1;

	hSrcDS08 = GDALOpen(next01_path, GA_ReadOnly);
	if (hSrcDS08) flags[7] = 1;
	hSrcDS09 = GDALOpen(next02_path, GA_ReadOnly);
	if (hSrcDS09) flags[8] = 1;
	hSrcDS10 = GDALOpen(next03_path, GA_ReadOnly);
	if (hSrcDS10) flags[9] = 1;
	hSrcDS11 = GDALOpen(next04_path, GA_ReadOnly);
	if (hSrcDS11) flags[10] = 1;
	hSrcDS12 = GDALOpen(next05_path, GA_ReadOnly);
	if (hSrcDS12) flags[11] = 1;
	hSrcDS13 = GDALOpen(next06_path, GA_ReadOnly);
	if (hSrcDS13) flags[12] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);
	hSrcBand02 = GDALGetRasterBand(hSrcDS02, 1);
	hSrcBand03 = GDALGetRasterBand(hSrcDS03, 1);
	hSrcBand04 = GDALGetRasterBand(hSrcDS04, 1);
	hSrcBand05 = GDALGetRasterBand(hSrcDS05, 1);
	hSrcBand06 = GDALGetRasterBand(hSrcDS06, 1);

	hSrcBand08 = GDALGetRasterBand(hSrcDS08, 1);
	hSrcBand09 = GDALGetRasterBand(hSrcDS09, 1);
	hSrcBand10 = GDALGetRasterBand(hSrcDS10, 1);
	hSrcBand11 = GDALGetRasterBand(hSrcDS11, 1);
	hSrcBand12 = GDALGetRasterBand(hSrcDS12, 1);
	hSrcBand13 = GDALGetRasterBand(hSrcDS13, 1);

	if (hSrcBand01 == NULL) flags[0] = 0;
	if (hSrcBand02 == NULL) flags[1] = 0;
	if (hSrcBand03 == NULL) flags[2] = 0;
	if (hSrcBand04 == NULL) flags[3] = 0;
	if (hSrcBand05 == NULL) flags[4] = 0;
	if (hSrcBand06 == NULL) flags[5] = 0;
	if (hSrcBand07 == NULL) flags[6] = 0;
	if (hSrcBand08 == NULL) flags[7] = 0;
	if (hSrcBand09 == NULL) flags[8] = 0;
	if (hSrcBand10 == NULL) flags[9] = 0;
	if (hSrcBand11 == NULL) flags[10] = 0;
	if (hSrcBand12 == NULL) flags[11] = 0;
	if (hSrcBand13 == NULL) flags[12] = 0;

	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData02 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData03 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData04 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData05 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData06 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData07 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData08 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData09 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData10 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData11 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData12 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData13 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;

	int rslt = 0;

	for (int i = 0;i < ysize;i++)
	{
		//cout<<"Processing line "<<i+1<<"!"<<endl;
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData02, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData03, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData04, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData05, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData06, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData07, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData08, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData09, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData10, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData11, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData12, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData13, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);

		//memset(rData, 0x00, GDALGetDataTypeSize(gType)*xsize/8);

		//处理一行
# pragma region
		err = GDALRasterIO(hSrcBand07, GF_Read, 0, i, xsize, 1, pData07, xsize, 1, gType, 0, 0);
		if (err == CE_None)
		{
#pragma region
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}

			if (flags[1] == 1)
			{
				GDALRasterIO(hSrcBand02, GF_Read, 0, i, xsize, 1, pData02, xsize, 1, gType, 0, 0);
			}

			if (flags[2] == 1)
			{
				GDALRasterIO(hSrcBand03, GF_Read, 0, i, xsize, 1, pData03, xsize, 1, gType, 0, 0);
			}

			if (flags[3] == 1)
			{
				GDALRasterIO(hSrcBand04, GF_Read, 0, i, xsize, 1, pData04, xsize, 1, gType, 0, 0);
			}

			if (flags[4] == 1)
			{
				GDALRasterIO(hSrcBand05, GF_Read, 0, i, xsize, 1, pData05, xsize, 1, gType, 0, 0);
			}

			if (flags[5] == 1)
			{
				GDALRasterIO(hSrcBand06, GF_Read, 0, i, xsize, 1, pData06, xsize, 1, gType, 0, 0);
			}

			if (flags[7] == 1)
			{
				GDALRasterIO(hSrcBand08, GF_Read, 0, i, xsize, 1, pData08, xsize, 1, gType, 0, 0);
			}

			if (flags[8] == 1)
			{
				GDALRasterIO(hSrcBand09, GF_Read, 0, i, xsize, 1, pData09, xsize, 1, gType, 0, 0);
			}
			if (flags[9] == 1)
			{
				GDALRasterIO(hSrcBand10, GF_Read, 0, i, xsize, 1, pData10, xsize, 1, gType, 0, 0);
			}

			if (flags[10] == 1)
			{
				GDALRasterIO(hSrcBand11, GF_Read, 0, i, xsize, 1, pData11, xsize, 1, gType, 0, 0);
			}

			if (flags[11] == 1)
			{
				GDALRasterIO(hSrcBand12, GF_Read, 0, i, xsize, 1, pData12, xsize, 1, gType, 0, 0);
			}

			if (flags[12] == 1)
			{
				GDALRasterIO(hSrcBand13, GF_Read, 0, i, xsize, 1, pData13, xsize, 1, gType, 0, 0);
			}
#pragma endregion
			//#pragma omp parallel for num_threads(16)
			for (int j = 0;j < xsize;j++)
			{
				double dns[13] = { 0.0 };
				double idx[13] = { 0.0 };

				dns[0] = pData01[j];
				dns[1] = pData02[j];
				dns[2] = pData03[j];
				dns[3] = pData04[j];
				dns[4] = pData05[j];
				dns[5] = pData06[j];
				dns[6] = pData07[j];
				dns[7] = pData08[j];
				dns[8] = pData09[j];
				dns[9] = pData10[j];
				dns[10] = pData11[j];
				dns[11] = pData12[j];
				dns[12] = pData13[j];

				std::vector<double> x;
				std::vector<double> y;
				for (int m = 0;m < 13;m++)
				{
					if (m != 6 && flags[m] == 1)
					{
						x.push_back(m);
						y.push_back(dns[m]);
					}
				}

				int len = x.size();
				double* x0 = new double[len];		//已知的数据点
				double* y0 = new double[len];
				std::copy(x.begin(), x.end(), x0);
				std::copy(y.begin(), y.end(), y0);

				pData07[j] = getGrayForecastModel(x, y);
				delete[] x0;
				delete[] y0;
			}
		}
# pragma endregion


		//写出一行结果
#pragma region
//if (maxDNFilter > 0)
//{
//	for (int s = 0; s < xsize; s++)
//	{
//		if (rData[s] > maxDNFilter)
//		{
//			rData[s] = 0.0;
//		}
//		if (rData[s] < 0.0)
//		{
//			rData[s] = 0.0;
//		}
//	}
//}
		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			i,
			xsize,
			1,
			pData07,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			std::cout << "line " << i << " is processed!" << std::endl;
			rslt++;
		}
		else
		{
			std::cout << "ERROR: line " << i << " was failed!" << std::endl;
		}
#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);
	GDALSetProjection(hDstDS, GDALGetProjectionRef(hSrcDS07));
	if (bDstHasNoData)
		GDALSetRasterNoDataValue(hDstBand, dfDstNoDataValue);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);
	free(pData02);
	free(pData03);
	free(pData04);
	free(pData05);
	free(pData06);
	free(pData07);
	free(pData08);
	free(pData09);
	free(pData10);
	free(pData11);
	free(pData12);
	free(pData13);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);
	GDALClose(hSrcDS02);
	GDALClose(hSrcDS03);
	GDALClose(hSrcDS04);
	GDALClose(hSrcDS05);
	GDALClose(hSrcDS06);
	GDALClose(hSrcDS07);
	GDALClose(hSrcDS08);
	GDALClose(hSrcDS09);
	GDALClose(hSrcDS10);
	GDALClose(hSrcDS11);
	GDALClose(hSrcDS12);
	GDALClose(hSrcDS13);

	GDALDestroyDriverManager();
	std::cout << rslt << " lines is copied!" << std::endl;
	if (rslt == ysize)
	{
		std::cout << "SUCCESS!" << std::endl;
	}
	else
	{
		std::cout << "FINISH with ERRORS!" << std::endl;
		std::cout << ysize - rslt << " lines was not copied!" << std::endl;
	}
	return rslt;
}


int RegionDataHanle::RecordAndHandleException(const char* src_dem_path, const char* dst_dem_path, int * lowExceptionCount, int * highExceptionCount,
	double* lowExceptionValue, double* highExceptionValue, /*std::vector<double>* lowValues, std::vector<double>* highValues,*/ double threshold) {

	double c0 = 0.0;
	double c1 = 1.0;

	//注册GDAL函数和功能
	GDALAllRegister();

	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS = NULL;
	GDALRasterBandH hSrcBand = NULL;
	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];

	/*检测目标数据信息*/
	hDstDS = GDALOpen(dst_dem_path, GA_Update);
	if (hDstDS != NULL)
	{
		GDALClose(hDstDS);
		GDALDestroyDriverManager();
		return -1;
	}

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		return false;
	}

	/*检测数据源信息*/
	hSrcDS = GDALOpen(src_dem_path, GA_ReadOnly);
	if (hSrcDS == NULL)
	{
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入数据的第一个波段
	hSrcBand = GDALGetRasterBand(hSrcDS, 1);
	if (hSrcBand == 0)
	{
		GDALClose(hSrcDS);
		GDALDestroyDriverManager();
		return false;
	}

	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS);
	ysize = GDALGetRasterYSize(hSrcDS);
	cout << "Lines: " << ysize << "; Columns: " << xsize << endl;
	if (xsize <= 1 || ysize <= 1)
	{
		GDALClose(hSrcDS);
		GDALDestroyDriverManager();
		return false;
	}

	//获取图像的仿射变换矩阵
	if (GDALGetGeoTransform(hSrcDS, adfGeoTransform) != CE_None)
	{
		GDALClose(hSrcDS);
		GDALDestroyDriverManager();
		return false;
	}

	//获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;
	dfDstNoDataValue = GDALGetRasterNoDataValue(hSrcBand, &bDstHasNoData);

	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand);

	//创建拷贝数据集
	hDstDS = GDALCreate(
		hDriver,
		dst_dem_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	//判断是否创建成功
	if (hDstDS == NULL)
	{
		GDALClose(hSrcDS);
		GDALDestroyDriverManager();
		return false;
	}
	hDstBand = GDALGetRasterBand(hDstDS, 1);

	//逐行读取并拷贝数据,pData用于存储每一行的数据
	//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8 * 3);

	CPLErr err = CE_None;

	int rslt = 0;
	{
		float* firstData = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

		err = GDALRasterIO(hSrcBand,
			GF_Read,
			0,
			0,
			xsize,
			1,
			firstData,
			xsize,
			1,
			gType,
			0,
			0);

		//读取成功，写入一行
		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			0,
			xsize,
			1,
			firstData,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			//cout<<"line "<<i<<" is copied!"<<endl;
			rslt++;
		}
		else
		{
			cout << "ERROR: line " << 0 << " was not copied!" << endl;
		}
	}
	for (int i = 1;i < ysize - 1;i++)
	{
		//每次复制前清空上次读取数据占用的内存
		memset(pData, 0x00, GDALGetDataTypeSize(gType) * xsize / 8 * 3);
		//读取一行
		err = GDALRasterIO(hSrcBand,
			GF_Read,
			0,
			i - 1,
			xsize,
			3,
			pData,
			xsize,
			3,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			float* dstData = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
			for (int j = 0;j < xsize;j++)
			{
				if (pData[xsize + j] > threshold) {
					(*highExceptionValue) += pData[xsize + j];
					//highValues->push_back(pData[xsize + j]);
					// 修复大于397.18的异常值， 原理为：如果某个像元的值大于本研究最大阈值
					// 观察其周围8个像元值的大小，利用这8个中的最大值代替；如果周围的8个像元值
					// 均大于最大阈值，先进行下一个栅格像元的处理，直到每个像元值均小于最大阈值为止

					vector<float> comp{pData[xsize + j - 1],pData[xsize + j + 1], pData[j - 1],pData[j],pData[j + 1],
				pData[2 * xsize + j - 1], pData[2 * xsize + j], pData[2 * xsize + j + 1] };

					if (j == 0) {
						comp.clear();
						vector<float> temp{ pData[xsize + j + 1],pData[j],pData[j + 1],
					 pData[2 * xsize + j], pData[2 * xsize + j + 1] };
						comp = temp;
					}else 
					if (j == xsize - 1) {
						comp.clear();
						vector<float> temp{ pData[xsize + j - 1], pData[j - 1],pData[j],pData[2 * xsize + j - 1], pData[2 * xsize + j]};
						comp = temp;
					}

					while (comp.size() > 0) {
						auto maxIter = std::max_element(comp.begin(), comp.end());
						if (*maxIter > threshold) {
							comp.erase(maxIter);
						}
						else {
							pData[xsize + j] = *maxIter;
							break;
						}
					}

					(*highExceptionCount)++;
				}
				if (pData[xsize + j] < 0 && pData[xsize + j] > -10000) {
					(*lowExceptionValue) += pData[xsize + j];
					//lowValues->push_back(pData[xsize + j]);
					pData[xsize + j] = 0;
					(*lowExceptionCount)++;
				}
				dstData[j] = pData[xsize + j];

			}

			//读取成功，写入一行
			err = GDALRasterIO(hDstBand,
				GF_Write,
				0,
				i,
				xsize,
				1,
				dstData,
				xsize,
				1,
				gType,
				0,
				0);
			if (err == CE_None)
			{
				cout<<"line "<<i<<" is copied!"<<endl;
				rslt++;
			}
			else
			{
				cout << "ERROR: line " << i << " was not copied!" << endl;
			}
		}
	}

	{
		float* lastData = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

		err = GDALRasterIO(hSrcBand,
			GF_Read,
			0,
			ysize - 1,
			xsize,
			1,
			lastData,
			xsize,
			1,
			gType,
			0,
			0);

		//读取成功，写入一行
		err = GDALRasterIO(hDstBand,
			GF_Write,
			0,
			ysize - 1,
			xsize,
			1,
			lastData,
			xsize,
			1,
			gType,
			0,
			0);
		if (err == CE_None)
		{
			cout << "line " << ysize - 1 << " is copied!" << endl;
			rslt++;
		}
		else
		{
			cout << "ERROR: line " << ysize - 1<< " was not copied!" << endl;
		}
	}

	GDALSetGeoTransform(hDstDS, adfGeoTransform);
	GDALSetProjection(hDstDS, GDALGetProjectionRef(hSrcDS));
	if (bDstHasNoData)
		GDALSetRasterNoDataValue(hDstBand, dfDstNoDataValue);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData);
	GDALClose(hDstDS);
	GDALClose(hSrcDS);
	GDALDestroyDriverManager();
	cout << rslt << " lines is copied!" << endl;
	if (rslt == ysize)
	{
		cout << "SUCCESS!" << endl;
	}
	else
	{
		cout << "FINISH with ERRORS!" << endl;
		cout << ysize - rslt << " lines was not copied!" << endl;
	}
	return rslt;
}

int RegionDataHanle::totalCount(const char* src_dem_path) {
	//注册GDAL函数和功能
	GDALAllRegister();
	CPLSetConfigOption("GDAL_DATA", "D:\\Program Files\\gdal\\data\\");
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;


	GDALRasterBandH hSrcBand01 = NULL;


	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };



	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		//return false;
	}


	//打开所有文件
	hSrcDS01 = GDALOpen(src_dem_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);



	if (hSrcBand01 == NULL) flags[0] = 0;
	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS01);
	ysize = GDALGetRasterYSize(hSrcDS01);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;
	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand01);
	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);


	CPLErr err = CE_None;
	std::map<std::string, int> map;
	map.insert(std::pair<std::string, int>("小于0", 0));
	map.insert(std::pair<std::string, int>("等于0", 0));
	map.insert(std::pair<std::string, int>("大于0小于10", 0));
	map.insert(std::pair<std::string, int>("大于等于10小于100", 0));
	map.insert(std::pair<std::string, int>("大于等于100小于500", 0));
	map.insert(std::pair<std::string, int>("大于等于500小于1000", 0));
	map.insert(std::pair<std::string, int>("大于1000", 0));
	int count = 0;
	int index = 1;
	for (int i = 0;i < ysize;i++)
	{
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		if (err == CE_None)
		{
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}

			for (int j = 0;j < xsize;j++)
			{
				//cout << "i = " << i << " index = " << index++ << " vcm = " << pData01[j] << endl;
				if (pData01[j] >= 0) {
					count++;
				}
				
				

			}
		}


		//写出一行结果
#pragma region
//if (maxDNFilter > 0)
//{
//	for (int s = 0; s < xsize; s++)
//	{
//		if (rData[s] > maxDNFilter)
//		{
//			rData[s] = 0.0;
//		}
//		if (rData[s] < 0.0)
//		{
//			rData[s] = 0.0;
//		}
//	}
//}

#pragma endregion

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);


	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);


	GDALDestroyDriverManager();
	//std::cout << rslt << " lines is copied!" << std::endl;
	//if (rslt == ysize)
	//{
	//	std::cout << "SUCCESS!" << std::endl;
	//}
	//else
	//{
	//	std::cout << "FINISH with ERRORS!" << std::endl;
	//	std::cout << ysize - rslt << " lines was not copied!" << std::endl;
	//}
	return count;
}

double RegionDataHanle::totalDNValue(const char* src_dem_path, double* minValue, double* maxValue) {
	//注册GDAL函数和功能
	GDALAllRegister();
	CPLSetConfigOption("GDAL_DATA", "D:\\Program Files\\gdal\\data\\");
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	//数据驱动、数据集、波段等变量声明
	GDALDriverH  hDriver = NULL;
	GDALDatasetH hDstDS = NULL;
	GDALDatasetH hSrcDS01 = NULL;

	GDALRasterBandH hSrcBand01 = NULL;

	GDALRasterBandH hDstBand = NULL;
	int xsize, ysize;
	double  adfGeoTransform[6];
	int flags[13] = { 0 };

	hDriver = GDALGetDriverByName("GTiff");
	if (hDriver == NULL || GDALGetMetadataItem(hDriver, GDAL_DCAP_CREATE, NULL) == NULL)
	{
		/*不支持创建制定格式*/
		GDALDestroyDriverManager();
		//return false;
	}

	//打开所有文件
	hSrcDS01 = GDALOpen(src_dem_path, GA_ReadOnly);
	if (hSrcDS01) flags[0] = 1;

	//获取输入数据的第一个波段
	hSrcBand01 = GDALGetRasterBand(hSrcDS01, 1);

	if (hSrcBand01 == NULL) flags[0] = 0;
	//获取输入图像大小（列数、行数）
	xsize = GDALGetRasterXSize(hSrcDS01);
	ysize = GDALGetRasterYSize(hSrcDS01);
	std::cout << "Lines: " << ysize << "; Columns: " << xsize << std::endl;
	//获取输入数据集的像元类型
	GDALDataType gType = GDALGetRasterDataType(hSrcBand01);
	//逐行读取并拷贝数据,pData用于存储每一行的数据
//void* pData = malloc(GDALGetDataTypeSize(gType)*xsize/8);
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);

	CPLErr err = CE_None;
	std::map<std::string, int> map;
	map.insert(std::pair<std::string, int>("小于0", 0));
	map.insert(std::pair<std::string, int>("等于0", 0));
	map.insert(std::pair<std::string, int>("大于0小于10", 0));
	map.insert(std::pair<std::string, int>("大于等于10小于100", 0));
	map.insert(std::pair<std::string, int>("大于等于100小于500", 0));
	map.insert(std::pair<std::string, int>("大于等于500小于1000", 0));
	map.insert(std::pair<std::string, int>("大于1000", 0));
	double tnl = 0; // 总的像元值
	for (int i = 0;i < ysize;i++)
	{
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		if (err == CE_None)
		{
			if (flags[0] == 1)
			{
				GDALRasterIO(hSrcBand01, GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
			}

			for (int j = 0;j < xsize;j++)
			{
				if (pData01[j] >= 0) {
					tnl += pData01[j];
				}	
				// 最小值
				if (*minValue > pData01[j] && pData01[j] > -10000) {
					*minValue = pData01[j];
				}
				// 最大值
				if (*maxValue < pData01[j]) {
					*maxValue = pData01[j];
				}
			}
		}

	}
	GDALSetGeoTransform(hDstDS, adfGeoTransform);

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = GDALGetRasterStatistics(hDstBand, 0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	GDALSetRasterStatistics(hDstBand, dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);

	GDALClose(hDstDS);
	GDALClose(hSrcDS01);

	GDALDestroyDriverManager();
	return tnl;
}

void RegionDataHanle::differenceImage(const char* src_path1, const char* src_path2, const char* dst_dem_path) {
	// 注册GDAL函数与功能
	GDALAllRegister();

	// 数据驱动、数据集、波段等变量声明
	GDALDriver* poDriver = nullptr;
	GDALDataset* poSrcDS1 = nullptr;
	GDALDataset* poSrcDS2 = nullptr;
	GDALDataset* poDstDS = nullptr;

	GDALRasterBand* poSrcBand01 = nullptr;
	GDALRasterBand* poSrcBand02 = nullptr;
	GDALRasterBand* poDstBand = nullptr;

	int xsize, ysize;
	double adfTransform[6];

	// 检测目标数据信息
	poDstDS = (GDALDataset*)GDALOpen(dst_dem_path, GA_ReadOnly);
	if (poDstDS != nullptr) {

		GDALClose((GDALDatasetH)poDstDS);
		return;
	}

	poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	if (poDriver == nullptr) {
		exit(1);
		return;
	}

	// 检测数据源信息
	poSrcDS1 = (GDALDataset*)GDALOpen(src_path1, GA_ReadOnly);
	poSrcDS2 = (GDALDataset*)GDALOpen(src_path2, GA_ReadOnly);

	if (poSrcDS1 == nullptr || poSrcDS2 == nullptr) {
		//源数据无法打开
		return;
	}

	poSrcBand01 = poSrcDS1->GetRasterBand(1);
	poSrcBand02 = poSrcDS2->GetRasterBand(1);

	if (poSrcBand01 == nullptr || poSrcBand02 == nullptr) {
		return;
	}

	// 获取输入图像大小（列数、行数）
	xsize = poSrcBand01->GetXSize();
	ysize = poSrcBand01->GetYSize();
	int nBlockXSize, nBlockYSize;
	poSrcBand01->GetBlockSize(&nBlockXSize, &nBlockYSize);

	cout << "Lines: " << ysize << "; Columns: " << xsize << endl;
	cout << "nBlockYSize: " << nBlockYSize << "; nBlockXSize: " << nBlockXSize << endl;

	if (poSrcDS1->GetProjectionRef() != nullptr) {
		printf("Projection is '%s'\n", poSrcDS1->GetProjectionRef());
	}
	if (poSrcDS1->GetGeoTransform(adfTransform) == CE_None) {
		printf("Origin = (%.6f,%.6f)\n",
			adfTransform[0], adfTransform[3]);
		printf("Pixel Size = (%.6f,%.6f)\n",
			adfTransform[1], adfTransform[5]);
	}

	// 获取Nodata值
	double dfDstNoDataValue = 0;
	int bDstHasNoData = TRUE;

	dfDstNoDataValue = poSrcBand01->GetNoDataValue(&bDstHasNoData);
	// 获取输入数据集的像元类型
	GDALDataType gType = poSrcBand01->GetRasterDataType();

	// 创建拷贝数据集
	poDstDS = poDriver->Create(dst_dem_path,
		xsize,
		ysize,
		1,
		gType,
		0);

	// 判断是否创建成功
	if (poDstDS == nullptr) {

		GDALClose((GDALDatasetH)poSrcDS1);
		GDALClose((GDALDatasetH)poSrcDS2);
		return;
	}

	poDstBand = poDstDS->GetRasterBand(1);
	if (poDstBand == nullptr) {
		GDALClose((GDALDatasetH)poSrcDS1);
		GDALClose((GDALDatasetH)poSrcDS2);
		return;
	}

	//逐行读取并拷贝数据,pData用于存储每一行的数据
	float* pData01 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* pData02 = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	float* dstData = (float*)malloc(GDALGetDataTypeSize(gType) * xsize / 8);
	
	CPLErr err = CE_None;

	int rslt = 0;
	for (int i = 0;i < ysize;i++)
	{
		//每次计算前清空上次读取数据占用的内存
		memset(pData01, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(pData02, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);
		memset(dstData, 0x00, GDALGetDataTypeSize(gType) * xsize / 8);

		poSrcBand01->RasterIO( GF_Read, 0, i, xsize, 1, pData01, xsize, 1, gType, 0, 0);
		poSrcBand02->RasterIO(GF_Read, 0, i, xsize, 1, pData02, xsize, 1, gType, 0, 0);

		for (int j = 0;j < xsize;j++)
		{
			if (pData01[j] < -10000 && pData02[j] < -10000) {
				dstData[j] = pData01[j];
			}
			else if(pData01[j] < -10000) {
				dstData[j] = pData02[j];
			}
			else if (pData02[j] < -10000) {
				dstData[j] = pData01[j];
			}
			else {
				dstData[j] = abs(pData01[j] - pData02[j]);
			}

		}

		err = poDstBand->RasterIO(GF_Write, 0, i, xsize, 1, dstData, xsize, 1, gType, 0, 0);
		if (err == CE_None)
		{
			cout << "line " << i << " is processed!" << endl;
			rslt++;
		}
		else
		{
			cout << "ERROR: line " << i << " was failed!" << endl;
		}
	}

	poDstDS->SetGeoTransform(adfTransform);
	poDstDS->SetProjection(poSrcDS1->GetProjectionRef());
	if (bDstHasNoData) {
		poDstBand->SetNoDataValue(dfDstNoDataValue);
	}

	int bDstMax = TRUE, bDstMin = TRUE;
	double dfDstMaxValue = 0.0;
	double dfDstMinValue = 0.0;
	double dfDstMean = 0.0;
	double dfDstStdDev = 0.0;
	err = poSrcBand01->GetStatistics(0, 1, &dfDstMinValue, &dfDstMaxValue, &dfDstMean, &dfDstStdDev);
	poDstBand->SetStatistics(dfDstMinValue, dfDstMaxValue, dfDstMean, dfDstStdDev);

	free(pData01);
	free(pData02);
	free(dstData);

	GDALClose((GDALDatasetH)poDstDS);
	GDALClose((GDALDatasetH)poSrcDS1);
	GDALClose((GDALDatasetH)poSrcDS2);

	cout << rslt << " lines is copied!" << endl;
	if (rslt == ysize)
	{
		cout << "SUCCESS!" << endl;
	}
	else
	{
		cout << "FINISH with ERRORS!" << endl;
		cout << ysize - rslt << " lines was not copied!" << endl;
	}
}
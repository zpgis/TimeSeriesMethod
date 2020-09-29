/****************************************************************
* Project:  地理信息栅格处理工具集
* Purpose:  栅格输入输出管理类
* Author:   范俊甫, fanjf@lreis.ac.cn, 2013年-01月-06日
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved
* 版权所有   (c) 中国科学院地理科学与资源研究所
****************************************************************/
#pragma once

//#include "gt_raster2d.h"
//#include "gt_datasource.h"

#include <ogr_spatialref.h>
#include <gdal_priv.h>
//#pragma comment(lib,"gdal_i.lib")

#include <fstream>
#include <iomanip>
#include "gt_datasource.h"
#include "gt_raster2d.h"
using namespace std;


//栅格单元数据类型
enum GTRasterDataType{
	RDT_Int,	// 整型
	RDT_Float,	// 浮点型
	RDT_Double  // 双进度型
};

/*计算内存限制，如果超出改值，则无法进行计算（以MB为单位）*/
#ifndef MEMORY_LIMIT
#	define MEMORY_LIMIT 2048 /*MB*/
#endif

//class __declspec(dllexport) GTRasterIO
class GTS_DLL GTRasterIO
{
public:		
	// 读取栅格数据
	static  GTRaster2D* Read(const char* fileName);	/*@fileName 读取栅格数据路径*/
	// 写栅格数据为ASCII格式
	static void WriteToASCII(const char* fileName,/*@fileName 输出栅格数据路径*/
		                   GTRaster2D* source,/*@source 栅格数据信息类*/
						   GTRasterDataType rdt);/*@rdt 栅格数据输出类型，暂支持整型与浮点型*/
	// 写栅格数据为Tiff格式
	static void WriteToTiff(const char* fileName,/*@fileName 输出栅格数据路径*/
		                    GTRaster2D* source, /*@rdt 栅格数据输出类型，暂支持整型与浮点型*/
							GTRasterDataType rdt);/*@rdt 栅格数据输出类型，暂支持整型与浮点型*/
};

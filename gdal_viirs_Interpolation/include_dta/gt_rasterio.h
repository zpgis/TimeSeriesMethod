/****************************************************************
* Project:  ������Ϣդ�����߼�
* Purpose:  դ���������������
* Author:   ������, fanjf@lreis.ac.cn, 2013��-01��-06��
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved
* ��Ȩ����   (c) �й���ѧԺ�����ѧ����Դ�о���
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


//դ��Ԫ��������
enum GTRasterDataType{
	RDT_Int,	// ����
	RDT_Float,	// ������
	RDT_Double  // ˫������
};

/*�����ڴ����ƣ����������ֵ�����޷����м��㣨��MBΪ��λ��*/
#ifndef MEMORY_LIMIT
#	define MEMORY_LIMIT 2048 /*MB*/
#endif

//class __declspec(dllexport) GTRasterIO
class GTS_DLL GTRasterIO
{
public:		
	// ��ȡդ������
	static  GTRaster2D* Read(const char* fileName);	/*@fileName ��ȡդ������·��*/
	// дդ������ΪASCII��ʽ
	static void WriteToASCII(const char* fileName,/*@fileName ���դ������·��*/
		                   GTRaster2D* source,/*@source դ��������Ϣ��*/
						   GTRasterDataType rdt);/*@rdt դ������������ͣ���֧�������븡����*/
	// дդ������ΪTiff��ʽ
	static void WriteToTiff(const char* fileName,/*@fileName ���դ������·��*/
		                    GTRaster2D* source, /*@rdt դ������������ͣ���֧�������븡����*/
							GTRasterDataType rdt);/*@rdt դ������������ͣ���֧�������븡����*/
};

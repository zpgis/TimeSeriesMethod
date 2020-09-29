/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ���ָ̺߳����������������ϵͳ��������
* Author:   ������, fanjf@lreis.ac.cn, 2010��-12��-24��
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* ��Ȩ����   (c) �й���ѧԺ�����ѧ����Դ�о���. ��������Ȩ��
****************************************************************/
#ifndef GTSDTAPORT_H_INCLUDED
#define GTSDTAPORT_H_INCLUDED

/*GT��������ģ��*/
#include "gts_port.h"
#include "gt_datasource.h"

/*GDAL*/
#include "ogr_spatialref.h"
#include "gdalwarper.h"
#include "cpl_string.h"
#include "ogr_core.h"
#include "ogr_api.h"
#include "ogrsf_frmts.h"
#include "gdal_priv.h"
#include "gdal_alg.h"
#include "gdal.h"

/*OpenMP*/
#ifdef _OPENMP
# include <omp.h>
#endif

#define __WINDOWS 0

#ifdef __WINDOWS
#define PLATFORM 0
#else
#define PLATFORM 1
#endif

#ifndef GTS_FREE
#   define GTS_FREE(p) if(p){free(p); p = 0;}
#endif

#ifndef GTS_DELETE
#   define GTS_DELETE(p) if(p){delete p; p = 0;}
#endif

#ifndef GTS_DELETES
#   define GTS_DELETES(p) if(p){delete[] p; p = 0;}
#endif

namespace gts{

/*
 *����shade�Ĳ����ṹ
 */
typedef struct
{
	double nsres;
	double ewres;
	double sin_altRadians;
	double cos_altRadians_mul_z_scale_factor;
	double azRadians;
	double square_z_scale_factor;
}GTHillshadeAlgData;

/*
 *color relief�Ĳ����ṹ
 *�߳�ֵ��ӦRGB��Alphaֵ
 */
typedef struct
{
	double dfVal; 
	int nR;
	int nG;
	int nB;
	int nA;
}GTDEMColorEntry;

/*
 *color relief�Ĳ����ṹ
 *�߳�ֵ��Ӧ��ɫ��ѡ�񷽷�
 */
typedef enum
{
	COLOR_SELECTION_MIN_MAX,
	COLOR_SELECTION_INTERPOLATE,
	COLOR_SELECTION_NEAREST_ENTRY,
	COLOR_SELECTION_EXACT_ENTRY,
}GTDEMColorSelectionMode;

/*
 *�����¶ȵĲ����ṹ
 */
typedef struct
{
	double nsres;
	double ewres;
	double scale;
	int    slopeFormat; /*1Ϊ�Ƕ�, 0Ϊ�ٷֱ�*/
}GTSlopeAlgData;

/*
 *��������Ĳ����ṹ
 */
typedef struct
{
	int bAngleAsAzimuth;/*����Ϊ0*/
}GTAspectAlgData;

/*
 *����һ��ָ��Ĳ����ṹ
 */
typedef struct
{
	double nsres;/*�ϱ��ֱ���*/
	double ewres;/*�ϱ��ֱ���*/
	double scale;/*�ֱ���ת��Ϊm�Ĳ���*/
}GTDEMGeneralAlgData;

/*
 *CURVATURE����
 */
typedef enum
{
	CURVATURE_NORMAL,
	CURVATURE_MEAN,
	CURVATURE_DIFFERENCE,
	CURVATURE_HORIZONTAL,
	CURVATURE_PLAN,
	CURVATURE_PROFILE,
	CURVATURE_ROTOR,
	CURVATURE_HORIZONTAL_EXCESS,
	CURVATURE_VERTICAL_EXCESS,
	CURVATURE_MINIMAL,
	CURVATURE_MAXIMAL,
	CURVATURE_TOTAL_GAUSSIAN,
	CURVATURE_TOTAL_RING,
	CURVATURE_TOTAL_ACCUMULATION
}GTDEMCurvatureType;

/*
 *��������㷨����
 */
typedef enum
{
	NeighborStatistic_Majority,
	NeighborStatistic_Maximum,
	NeighborStatistic_Mean,
	NeighborStatistic_Median,
	NeighborStatistic_Minimum,
	NeighborStatistic_Minority,
	NeighborStatistic_Range,
	NeighborStatistic_STD,
	NeighborStatistic_Sum,
	NeighborStatistic_Varity,
}GTDEMNeighborStatisticType;

}
using namespace gts;

#endif

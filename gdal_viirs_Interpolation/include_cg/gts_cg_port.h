/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  �ռ������������ϵͳ��������
*           ����:�ռ��ϵ/�����ռ����/��������/Buffer/Overlay/͹��/
*           Voronoi/ʸ-դת��/�ռ�任/��-�����ת��/���ཻ�ֽ�/Dissolve 
* Author:   ������, fanjf@lreis.ac.cn 2010��-12��-15��
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved.
* ��Ȩ����   (c) �й���ѧԺ�����ѧ����Դ�о���. ��������Ȩ��
****************************************************************/
#ifndef GTSCGPORT_H_INCLUDED
#define GTSCGPORT_H_INCLUDED

/*GT��������ģ��*/
#include "gts_port.h"
#include "gt_datasource.h"
#include "gt_spatialindex.h"

/*OpenMP*/
#ifdef _OPENMP
# include <omp.h>
#	define OPENMP_THREADS_NUM 8	/*���Ӻ�Ϊ��˼���ʱ������������ܣ�2011-7-18*/
#endif

#ifndef GTS_XOR
#	define GTS_XOR(a, b) a^b
#endif

#ifndef GTS_NXOR
#	define GTS_NXOR(a, b) !(a^b)
#endif

#ifndef GTS_AND
#	define GTS_AND(a, b) a&&b
#endif

#ifndef GTS_OR
#	define GTS_OR(a, b) a||b
#endif

#ifndef GTS_NOT
#	define GTS_NOT(a) !a
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
/*liuyan add at 2011-8-18*/
#define GTS_RADTODGR	(0.0174532925199433)
/*add end*/
namespace gts{

/*�����������Ͷ���*/
typedef enum enumGTBooleanType{
	GT_BOOL_DIFFE  = 1,/*A-B��B-A��ͬ, ���ӵ�һ���ڽ���ڶ����ص�����ɾ��*/
	GT_BOOL_INTEC  = 2,/*���ڼ���, �����������*/
	GT_BOOL_XOR    = 3,/*������в�ͬ�Ĳ���*/
	GT_BOOL_UNION  = 4 /*����Overlay�ͺϲ�*/
};

/*Buffer�������Ͷ���*/
typedef enum enumGTBufferType{
	GT_BUFFER_OUTER  = 1,
	GT_BUFFER_EDGE   = 2
};

typedef bool (*checkCondition)(GTFeature *f1, GTFeature *f2,...);

typedef enum enumGTBufferDissolveType{
	GT_BUFFER_DISSOLVE_NONE			= 0,
	GT_BUFFER_DISSOLVE_ALL			= 1,
	GT_BUFFER_DISSOLVE_LIST			= 2,
	GT_BUFFER_DISSOLVE_INTERSECT	= 3
};

/*Overlay�������Ͷ���*/
typedef enum enumGTOverlayType{
	GT_OVERLAY_ERASE           = 1,
	GT_OVERLAY_IDENTITY        = 2,
	GT_OVERLAY_INTERSECT       = 3,
	GT_OVERLAY_SPATIALJOIN     = 4,
	GT_OVERLAY_SYMMETRICALDIFF = 5,
	GT_OVERLAY_UNION           = 6,
	GT_OVERLAY_UPDAT           = 7
};
/*liuyan add at 2011-8-18*/
struct GTGCP{
	GTRawPoint pt_maporg;
	GTRawPoint pt_georef;
	double	   dz_value;
};
/*add end*/

using namespace gts;

}
#endif

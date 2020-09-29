/****************************************************************
 *
 * Project:  地理信息处理工具集
 * Purpose:  基础宏和系统变量定义
 * Author:   范俊甫, fanjf@lreis.ac.cn 2010年-11月-12日
 *
 ****************************************************************
 * Copyright (c) All rights reserved.
 * 版权所有  (c) 保留所有权利
 ****************************************************************/
#pragma once
#ifndef GTSPORT_H_INCLUDED
#define GTSPORT_H_INCLUDED

#include <string>  //STL
#include <string.h>//c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>
#include <limits.h>
#include <limits>
#include <vector>
#include <iterator>
#include <algorithm>
#include <time.h>

#ifdef _MSC_VER
#   pragma warning(disable: 4996)
#   pragma warning(disable: 4311)
#   pragma warning(disable: 4005)
#   define _SCL_SECURE_NO_WARNING
#   define _CRT_SECURE_NO_DEPRECATE
#endif

#ifdef _MSC_VER
#   define snprintf _snprintf /*VC*/
#else
#   define snprintf snprintf  /*gcc*/
#endif

#ifndef GTS_MAX_DOUBLE
#	define GTS_MAX_DOUBLE DBL_MAX
#endif

#ifndef GTS_MIN_DOUBLE
#	define GTS_MIN_DOUBLE -1.0*GTS_MAX_DOUBLE
#endif

#ifndef DBL_MAX
#   define DBL_MAX 1.7976931348623158e+308
#endif

#ifndef GTS_FREE
#   define GTS_FREE(p)	do { if(p) { free(p); (p) = 0; } } while(0)
#endif

#ifndef GTS_DELETE
#   define GTS_DELETE(p)	do { if(p) { delete (p); (p) = 0; } } while(0)
#endif

#ifndef GTS_DELETES
#   define GTS_DELETES(p)	do { if(p) { delete[] (p); (p) = 0; } } while(0)
#endif

#ifndef GTS_OGRFREE
#	define GTS_OGRFREE CPLFree
#endif

#ifndef TRUE
#	define TRUE 1
#endif

#ifndef FALSE
#	define FALSE 0
#endif

#ifndef true
#	define true 1
#endif

#ifndef false
#	define false 0
#endif

#ifndef null
#	define null 0
#endif

#ifndef NULL
#	define NULL 0
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef byte
typedef unsigned char byte;
#endif

#ifndef GTS_DLL
#if defined(__WIN32__) || defined(_WIN32) || defined(_WINDOWS)//for window c++ compiler
#   define GTS_DLL __declspec(dllexport)
#   define RESTRICT __restrict
#else              //for gcc
#   define GTS_DLL __attribute__ ((visibility("default")))
#   define RESTRICT
#endif
#endif

#ifndef MAX
#  define MAX(a,b)    (((a)>(b)) ? (a) : (b))
#endif

#ifndef MIN
#  define MIN(a,b)    (((a)<(b)) ? (a) : (b))
#endif

#ifndef ABS
#  define ABS(x)      (((x)<0) ? -(x) : (x))
#endif

#ifndef XOR
#  define XOR(a, b) (a)^(b)
#endif

#ifndef NXOR
#  define NXOR(a, b) !((a)^(b))
#endif

#ifdef __cplusplus
#  define GTS_C_START  extern "C" {
#  define GTS_C_END    }
#else
#  define GTS_C_START
#  define GTS_C_END
#endif

#ifndef GTS_PI
#  define GTS_PI   3.14159265358979323846
#endif

#ifndef GTS_PI_2
#  define GTS_PI_2 1.57079632679489661923
#endif

#ifndef GTS_PI_4
#  define GTS_PI_4 0.78539816339744830961566084581988
#endif

#ifndef GTS_2PI
#  define GTS_2PI 6.2831853071795865
#endif

#ifndef DEG2RAD
#  define DEG2RAD(x)	((x)*GTS_PI/180.0)
#endif

#ifndef RAD2DEG
#  define RAD2DEG(x)	((x)*180.0/GTS_PI)
#endif

/*Little endian与Big endian 字节顺序交换的宏定义*/
#ifndef GT_SWAP16
#define GT_SWAP16(x) \
   ((GUInt16)( \
   (((GUInt16)(x) & 0x00ffU) << 8) | \
   (((GUInt16)(x) & 0xff00U) >> 8) ))
#endif

#ifndef GTByte
#  define GTByte unsigned char
#endif

#ifndef GTInt32
#  define GTInt32 unsigned int
#endif

#ifndef GT_SWAP16PTR
#  define GT_SWAP16PTR(x) \
{                                                                 \
	GTByte       byTemp, *_pabyDataT = (GTByte *) (x);            \
	\
	byTemp = _pabyDataT[0];                                       \
	_pabyDataT[0] = _pabyDataT[1];                                \
	_pabyDataT[1] = byTemp;                                       \
}                                                                    
#endif

#ifndef GT_SWAP32
#  define GT_SWAP32(x) \
	((GTInt32)( \
	(((GTInt32)(x) & (GTInt32)0x000000ffUL) << 24) | \
	(((GTInt32)(x) & (GTInt32)0x0000ff00UL) <<  8) | \
	(((GTInt32)(x) & (GTInt32)0x00ff0000UL) >>  8) | \
	(((GTInt32)(x) & (GTInt32)0xff000000UL) >> 24) ))
#endif

#ifndef GT_SWAP32PTR
#  define GT_SWAP32PTR(x) \
{                                                                 \
	GTByte       byTemp, *_pabyDataT = (GTByte *) (x);            \
	\
	byTemp = _pabyDataT[0];                                       \
	_pabyDataT[0] = _pabyDataT[3];                                \
	_pabyDataT[3] = byTemp;                                       \
	byTemp = _pabyDataT[1];                                       \
	_pabyDataT[1] = _pabyDataT[2];                                \
	_pabyDataT[2] = byTemp;                                       \
}                                                                    
#endif

#ifndef GT_SWAP64PTR
#  define GT_SWAP64PTR(x) \
{                                                                 \
	GTByte       byTemp, *_pabyDataT = (GTByte *) (x);            \
	\
	byTemp = _pabyDataT[0];                                       \
	_pabyDataT[0] = _pabyDataT[7];                                \
	_pabyDataT[7] = byTemp;                                       \
	byTemp = _pabyDataT[1];                                       \
	_pabyDataT[1] = _pabyDataT[6];                                \
	_pabyDataT[6] = byTemp;                                       \
	byTemp = _pabyDataT[2];                                       \
	_pabyDataT[2] = _pabyDataT[5];                                \
	_pabyDataT[5] = byTemp;                                       \
	byTemp = _pabyDataT[3];                                       \
	_pabyDataT[3] = _pabyDataT[4];                                \
	_pabyDataT[4] = byTemp;                                       \
}                                                                    
#endif

#ifndef GT_SWAPDOUBLE
#  define GT_SWAPDOUBLE(p) GT_SWAP64PTR(p)
#endif

/*从2个字节按照LSB顺序排列的x地址处获取16位整型*/
#define GT_LSBINT16PTR(x)    ((*(unsigned char*)(x)) | ((*(unsigned char*)((x)+1)) << 8))

/*从4个字节按照LSB顺序排列的x地址处获取32位整型*/
#define GT_LSBINT32PTR(x)    ((*(unsigned char*)(x)) | ((*(unsigned char*)((x)+1)) << 8) | \
	((*(unsigned char*)((x)+2)) << 16) | ((*(unsigned char*)((x)+3)) << 24))

namespace gts {
	/*执行错误类型定义*/
	enum enumGTEXErr {
		GT_Success = 0,
		GT_Log     = 1,
		GT_Warning = 2,
		GT_Failure = 3,
		GT_Fatal   = 4
    };

	/*字节顺序定义, 参考OGR*/
	enum enumGTWKBByteOrder
	{
		/*WKB中, 1为little-endian, 0为big-endian*/
		WKBXDR = 0, /* MSB/Sun/Motoroloa: Most Significant Byte First big-endian*/
		WKBNDR = 1  /* LSB/Intel/Vax: Least Significant Byte First little-endian*/
		/*Network Data Representation (NDR) 
		 *External Data Representation (XDR)*/
	};

	/*面向数据库空间数据操作WKB/WKT错误定义, 参考OGR*/
	enum enumGTDataErr {
		GTERR_NONE                     = 0,
		GTERR_NOT_ENOUGH_DATA          = 1,
		GTERR_NOT_ENOUGH_MEMORY        = 2,
		GTERR_UNSUPPORTED_GEOMETRY_TYPE= 3,
		GTERR_UNSUPPORTED_OPERATION    = 4,
		GTERR_CORRUPT_DATA             = 5,
		GTERR_FAILURE                  = 6,
		GTERR_UNSUPPORTED_SRS          = 7,
		GTERR_INVALID_HANDLE           = 8
	};

	/*WKB数据类型, 参考OGR*/
	enum enumGTWKBGeometryType
	{
		GT_WKBUnknown    = 0,  /**< unknown type, non-standard */
		GT_WKBPoint      = 1,  /**< 0-dimensional geometric object, standard WKB */
		GT_WKBLineString = 2,  /**< 1-dimensional geometric object with linear
							  *  interpolation between Points, standard WKB */
		GT_WKBPolygon    = 3,  /**< planar 2-dimensional geometric object defined
							  *  by 1 exterior boundary and 0 or more interior
							  *  boundaries, standard WKB */
		GT_WKBMultiPoint         = 4, /**< GeometryCollection of Points, standard WKB */
		GT_WKBMultiLineString    = 5, /**< GeometryCollection of LineStrings, standard WKB */
		GT_WKBMultiPolygon       = 6, /**< GeometryCollection of Polygons, standard WKB */
		GT_WKBGeometryCollection = 7, /**< geometric object that is a collection of 1
								        or more geometric objects, standard WKB */
	    GT_WKBNone                  = 100,        /**< non-standard, for pure attribute records */
	    GT_WKBLinearRing            = 101,        /**< non-standard, just for createGeometry()  */
	    GT_WKBPoint25D              = 0x80000001, /**< 2.5D extension as per 99-402 */
	    GT_WKBLineString25D         = 0x80000002, /**< 2.5D extension as per 99-402 */
	    GT_WKBPolygon25D            = 0x80000003, /**< 2.5D extension as per 99-402 */
	    GT_WKBMultiPoint25D         = 0x80000004, /**< 2.5D extension as per 99-402 */
	    GT_WKBMultiLineString25D    = 0x80000005, /**< 2.5D extension as per 99-402 */
	    GT_WKBMultiPolygon25D       = 0x80000006, /**< 2.5D extension as per 99-402 */
	    GT_WKBGeometryCollection25D = 0x80000007  /**< 2.5D extension as per 99-402 */
	};

	/*数据类型定义, 主要针对栅格数据, 与GDALDataType一致*/
	enum enumGTDataType {
		GTD_Unknown   = 0,
		GTD_Byte      = 1,
		GTD_UInt16    = 2,
		GTD_Int16     = 3,
		GTD_UInt32    = 4,
		GTD_Int32     = 5,
		GTD_Float32   = 6,
		GTD_Float64   = 7,
		GTD_CInt16    = 8,/**/
		GTD_CInt32    = 9,
		GTD_CFloat32  = 10,
		GTD_CFloat64  = 11,
		GTD_TypeCount = 12
	};
	
	/*栅格像素排列方式*/
	enum enumGTPixelInterleave {
		GT_PIXINTERL_UNKNOWN  = 0, 
		GT_PIXINTERL_BANDBSQ  = 1, /*Band SeQuential format*/
		GT_PIXINTERL_LINEBIL  = 2, /*Band Interleaved by Line format*/
		GT_PIXINTERL_PIXELBIP = 3  /*Band Interleaved by Pixel format*/
	};

	/*Raster重采样方法, 与GDALResampleAlg一致*/
	enum enumGTRasterResample {
		GTRA_NearestNeighbour = 0, /*! Nearest neighbor (select on one input pixel)*/ 
		GTRA_Bilinear         = 1, /*! Bilinear (2x2 kernel) */                         
		GTRA_Cubic            = 2, /*! Cubic Convolution Approximation (4x4 kernel)*/  
		GTRA_CubicSpline      = 3, /*! Cubic B-Spline Approximation (4x4 kernel)*/     
		GTRA_Lanczos          = 4  /*! Lanczos windowed sinc interpolation (6x6 kernel)*/ 
	};

	/*几何对象类型定义*/
	enum enumGTGeometryType {
		/*组成Feature的主要几何对象*/
		GT_RawPoint        = 0,
		GT_Point           = 1,
		GT_LineString      = 2,
		GT_Polygon         = 3,
		GT_MultiPoint      = 4,
		GT_MultiLineString = 5,
		GT_MultiPolygon    = 6,
		GT_GeometryBag     = 7,
		GT_DynamicPoint    = 8,
		
		/*与上面存在继承关系的几何对象*/
		GT_Line            = 11,
		GT_Elliptic        = 12,
		GT_Circular        = 13,
		GT_Ring            = 14,
		GT_Triangle        = 15,
		GT_Bezier          = 16,

		/*抽象几何类型*/
		GT_Segment            = 21,
		GT_PolyCurve          = 22,
		GT_Curve              = 23,
		GT_GeometryCollection = 24,
		GT_Geometry           = 25,
		GT_Geometry_Unknown   = 26
	};

	/*环的点排列顺序*/
	enum enumGTRingOrientation {
		GT_RO_Degenerated  = 0,
		GT_RO_Counterclock = 1,
		GT_RO_Clockwise    = 2
	};

	/*金字塔重采样方法*/
	enum enumGTPyraResample {
		GT_PYRA_NONE        = 0,
		GT_PYRA_NEAREST     = 1,
		GT_PYRA_GAUSS       = 2,
		GT_PYRA_CUBIC       = 3,
		GT_PYRA_AVERAGE     = 4,
		GT_PYRA_MODE        = 5,
		GT_PYRA_AVEMAGPHASE = 6
	};

	/*表中列的类型*/
	enum enumGTFieldType {
		GT_FD_Int        = 0, //4字节整数
		GT_FD_IntList    = 1, //4字节整数链表
		GT_FD_Double     = 2, //8字节浮点
		GT_FD_DoubleList = 3, //8字节浮点链表
		GT_FD_String     = 4, //字符串
		GT_FD_StringList = 5, //字符串链表
		GT_FD_Blob       = 8, //二进制数据
		GT_FD_Date       = 9, //日期
		GT_FD_Time       = 10,//时间
		GT_FD_DateTime   = 11,//时间和日期
		GT_FD_Geometry   = 12,//几何对象, WKB格式存放
		GT_FD_SmallInt   = 13,//2字节整数
		GT_FD_Float      = 14 //4字节浮点
	};

	struct GTFDDate {
		unsigned short iYear;
		unsigned char  iMonth;
		unsigned char  iDay;
	};

	struct GTFDTime {
		unsigned char iHour;
		unsigned char iMinute;
		unsigned char iSecond;
		unsigned char tzFlag; /*0=unknown, 1=localtime, 100=GMT, 104=GMT+1, 80=GMT-5, etc*/
	};

	struct GTFDDateTime {
		GTFDDate date;
		GTFDTime time;
	};

	struct GTFDBlob {
		int nCount;
		unsigned char* pData; 
	};

	struct GTFDSet {
		int nMarker1;
		int nMarker2;
	};

	/*列的数值的联合体，最大长度分配内存，每次只有一个有效*/
	union unionGTField {
		int      gtInt;
		double   gtDouble;
		char*    gtString;
		GTFDDate gtDate;
		GTFDTime gtTime;
		GTFDDateTime gtDateTime;
		GTFDBlob gtBlob;
		GTFDSet  gtSet;
	};

	/*层的类型*/
	enum enumGTLayerType {
		GT_LAYER_Unknown      = 0,
		GT_LAYER_Feature      = 1,
		GT_LAYER_Raster       = 2,
		GT_LAYER_TIN          = 3,
		GT_LAYER_DEM          = 4,
		GT_LAYER_Measure      = 5,
		GT_LAYER_Graphic      = 6,
		GT_LAYER_Annotation   = 7,
		GT_LAYER_Topology     = 8,
		GT_LAYER_WMS          = 9,
		GT_LAYER_WFS          = 10,
		GT_LAYER_KML          = 11,
		GT_LAYER_Network      = 12,
		GT_LAYER_TopoRelation = 13
	};

	/*数据源驱动类型*/
	enum enumGTDataDriverType {
		GT_DATADRIVER_UNKNOWN       = 0,
		GT_DATADRIVER_GDAL          = 1,
		GT_DATADRIVER_OGR           = 2,
		GT_DATADRIVER_GDOS_MYSQL    = 3,
		GT_DATADRIVER_GDOS_ORACLE   = 4,
		GT_DATADRIVER_GDOS_BEYONDB  = 5,
		GT_DATADRIVER_GDOS_POSTGIS  = 6,
		GT_DATADRIVER_GDOS_ODBC     = 7
	};

	/*数据源驱动操作错误类型*/
	enum enumGTDataSourceErr {
		GT_DS_SUCCESS         = 0,
		GT_DS_UNSURPPORTED    = 1,
		GT_DS_NODATASOURCE    = 2,
		GT_DS_NODATADRIVER    = 3,
		GT_DS_INVALIDPARAM    = 4,
		GT_DS_OGRDRIVERERROR  = 5,
		GT_DS_GDALDRIVERERROR = 6,
		GT_DS_GDOSDRIVERERROR = 7,
		GT_DS_ALREADYOPEN     = 8,
		GT_DS_DBINITERROR     = 9,
		GT_DS_DBCONNERROR     = 10,
		GT_DS_DBQUERYERROR    = 11,
		GT_DS_OUTOFMEMORY     = 12,
		GT_DS_NONZEROREFCNT   = 13,
		GT_DS_NAMECOLLISION   = 14,
		GT_DS_READONLY        = 15,
		GT_DS_EXCEEDLIMIT     = 16,
// 		GT_DS_SOURCENOTOPEN   = 16,
		GT_DS_GENERICERROR    = 100,
	};

	/*地图坐标单位类型*/
	enum enumGTUnitType {
		GT_UNIT_UNKNOWN       = 0,
		/*大地坐标*/
		GT_UNIT_DEGREE        = 1, /*0.0174532925199433*/
		GT_UNIT_RADIAN        = 2, /*1.0*/
		/*投影坐标*/
		GT_UNIT_METER         = 3, /*1.0*/
		GT_UNIT_FOOT          = 4, /*0.3048*/
		GT_UNIT_FOOT_US       = 5, /*0.3048006096012129*/
		GT_UNIT_NAUTICAL_MILE = 6, /*1852.0*/
		GT_UNIT_LINK          = 7, /*0.20116684023368047*/
		GT_UNIT_CHAIN         = 8, /*20.116684023369047*/
		GT_UNIT_ROD           = 9, /*5.02921005842012*/
		GT_UNIT_KILOMETER     = 10 /*0.001*/
	};

	/*空间关系类型定义*/
	enum enumGTSpatialRelation {
		/*对于错误数据的判断结果*/
		GT_SR_NONE    = 0,

		/*类型、个数与坐标完全相等*/
		GT_SR_EQUAL   = 1,

		/*完全分离*/
		GT_SR_DISJOINT = 2,

		/*点为线段端点, 线段端点相同
		*点在多边形边上, 线段与多边形边部分重叠
		*/
		GT_SR_TOUCH   = 3,

		/*点集合之间部分相同, 线线之间部分相同，
		*多边形多边形部分重叠
		*相交不能包含
		*/
		GT_SR_OVERLAP = 4,

		/**/
		GT_SR_CROSS   = 5,
		/**/
		GT_SR_WITHIN  = 6,
		/**/
		GT_SR_CONTAIN = 7,

		/*相交的定义广泛, 包括overlap, within, contain, cross等*/
		GT_SR_INTERSECT = 8
	};

	/*颜色定义, 按照RGB, CMY和HLS三种模式*/
	struct GTColorEntry
	{
		/*gray, red, cyan or hue */
		short      c1;      
		/* green, magenta, or lightness */
		short      c2;      
		/* blue, yellow, or saturation */
		short      c3;      
		/* alpha or blackband */
		short      c4;      
	};

	/*! Types of color interpretations for a GDALColorTable. */
	enum GTPaletteInterp
	{
		/*Grayscale (in GDALColorEntry.c1) */
		GT_COLOR_GRAY = 0,
		/*Red, Green, Blue and Alpha in (in c1, c2, c3 and c4) */
		GT_COLOR_RGB  = 1,
		/*Cyan, Magenta, Yellow and Black (in c1, c2, c3 and c4)*/
		GT_COLOR_CMYK = 2,
		/*Hue, Lightness and Saturation (in c1, c2, and c3) */
		GT_COLOR_HLS  = 3,
		/*Undefined*/
		GT_COLOR_UNKNOWN = 4
	};
}

using namespace gts;

#endif

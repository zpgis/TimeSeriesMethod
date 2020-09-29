/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  栅格坐标转换的定义类
* Author:   范俊甫, fanjf@lreis.ac.cn 2011年-03月-09日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有  (c) 保留所有权利
****************************************************************/
#pragma once
#ifndef GTRASTERWARP_H_INCLUDED
#define GTRASTERWARP_H_INCLUDED

#include "gt_layer.h"
#include "../include_p/gdal/gdalwarper.h"

namespace gts{

/*
 *RasterWarp系列类的基类
 */
class GTS_DLL GTRasterReader
{
public:
	GTRasterReader(void);
	virtual ~GTRasterReader(void);
};

/*栅格数据的直接读取*/
class GTS_DLL GTRasterSimpleReader: public GTRasterReader
{
public:
	GTRasterSimpleReader(GTRasterDataset *pDataset);
	~GTRasterSimpleReader(void);

	static void destroyRasterSimpleReader(GTRasterSimpleReader *poSimpleReader);

	/*读取影像中的数据
	 *nXOff 	从左上角的X偏移量
	 *nYOff 	从左上角的Y偏移量
	 *nXSize 	读取/写入的宽度
	 *nYSize 	读取/写入的高度
	 *pData 	读取/写入的buffer，应该至少包含nBufXSize * nBufYSize * nBandCount 个 eBufType 类型的字节. 
	 *nBufXSize buffer 影像的宽度
	 *nBufYSize buffer 影像的高度
	 *eBufType 	pData 数据类型
	 *nBandCount	读/写的波段数，默认为所有
	 *panBandMap 	波段列表，以1开始，如果 NULL 则从第一个nBandCount开始
	 *nPixelSpace   默认为0，横向的读/写间隔
	 *nLineSpace 	默认为0，纵向的读/写间隔
	 *nBandSpace 	默认为0，波段的读/写间隔 */
	bool readDataByPixels(
		int   nXOff,
		int   nYOff,
		int   nXSize,
		int   nYSize,
		void *pData,
		int   nBufXSize,
		int   nBufYSize,
		enumGTDataType eBufType,
		int   nBandCount  = 0,
		int  *panBandMap  = NULL,
		int   nPixelSpace = 0,
		int   nLineSpace  = 0,
		int   nBandSpace  = 0);

	bool readDataByPixels(
		int   nXOff,
		int   nYOff,
		int   nXSize,
		int   nYSize,
		int   nBand,
		void *pData,
		int   nBufXSize,
		int   nBufYSize,
		enumGTDataType eBufType);

    /* 按照给定的空间范围的大小读取一块数据
     * env为实际的空间范围
	 * nBand为用户想读取的波段，从1开始
	 * dataType为用户指定的数据类型
     * *pData由外部初始化好内存空间,
     * nXSize, nYSize为目标区的像素行列数
	 * bReadOverview为是否从金字塔读取数据
	 * 注意: 对于non-north_up影像, 该方法不适用
     */
    bool readDataByEnvelop(
		GTEnvelop&     env, 
		int            nBand, 
		enumGTDataType dataType, 
		void*          pData, 
		int            nXSize, 
		int            nYSize,
		bool           bReadOverview = true);

	/* 按照给定的空间范围的大小读取一块数据
     * env为实际的空间范围
	 * nBand为用户想读取的波段，从1开始
	 * dataType为用户指定的数据类型
	 * dResolX, dResolY为指定的理想空间分辨率
     * *pData由该函数分配内存空间,外部调用GTS_FREE释放
	 * *pData应该由外部初始化为0
     * nXSize, nYSize返回的为读取的目标区的像素行列数
	 * bReadOverview为是否从金字塔读取数据
	 * 注意: 对于non-north_up影像, 该方法不适用
     */
	bool readDataByEnvelop(
		GTEnvelop&     env, 
		int            nBand, 
		enumGTDataType dataType,
		double         dResolX,
		double         dResolY,
		void**         pData, 
		int&           nXSize, 
		int&           nYSize,
		bool           bReadOverview = true); 

	/* 按照给定的空间范围的大小读取一块数据
     * 给定范围为相对于影像的空间范围, 由两个角点确定
	 * nBand为用户想读取的波段，从1开始
	 * dataType为用户指定的数据类型
	 * dResolX, dResolY为指定的理想空间分辨率
     * *pData由该函数分配内存空间,外部调用GTS_FREE释放
	 * *pData应该由外部初始化为0
     * nXSize, nYSize返回的为读取的目标区的像素行列数
	 * bReadOverview为是否从金字塔读取数据
	 * 针对非正轴影像, 取出后的数据在显示的时候需要将画布旋转
     */
	bool readDataByExtent(
		const GTRawPoint&    pt_leftTop, 
		const GTRawPoint&    pt_rightBottom, 
		int            nBand, 
		enumGTDataType dataType,
		double         dResolX,
		double         dResolY,
		void**         pData, 
		int&           nXSize, 
		int&           nYSize,
		bool           bReadOverview = true); 

	/* 按照给定的空间范围的大小读取一块数据
     * 给定范围为相对于影像的空间范围, 由两个角点确定
	 * nBand为用户想读取的波段，从1开始
	 * dataType为用户指定的数据类型
	 * dResolX, dResolY为指定的理想空间分辨率
     * pData由外部分配内存空间
     * nXSize, nYSizep为Data像素行列数
	 * bReadOverview为是否从金字塔读取数据
	 * 针对非正轴影像, 取出后的数据在显示的时候需要将画布旋转
     */
	bool readDataByExtent(
		const GTRawPoint&    pt_leftTop, 
		const GTRawPoint&    pt_rightBottom, 
		int            nBand, 
		enumGTDataType dataType,
		double         dResolX,
		double         dResolY,
		void*          pData, 
		int            nXSize, 
		int            nYSize,
		bool           bReadOverview = true); 

private:
	GTRasterDataset *poDataset; /*gdal数据集指针 */
};

/*
 *将数据通过仿射变换转换坐标系统之后读取到缓冲区
 */
class GTS_DLL GTRasterWarpReader: public GTRasterReader
{
public:
	/*构造时需要传入GTRasterDataset对象指针, 
	 *目标坐标描述, 重采样方法, 目标数据类型
	 *转换的波段数目和列表(从1开始)
	 */
	GTRasterWarpReader(
		GTRasterDataset *pDataset, 
		const char*      srsDstWKT,          /*目标坐标描述*/
		GDALResampleAlg  resamAlg,           /*重采样方法*/
		enumGTDataType   dstDataType,        /*输出数据类型*/
		int              iBandCount,         /*目标波段数目*/
		int             *iBandList,          /*目标波段列表*/
		double           dCacheLimit = 0.0,  /*内存使用限制, 单位为MB*/
		double          *pdNodata    = 0,    /*目标数据空值标志, 如果非空,长度必须等于iBandCount*/
		GDALProgressFunc progressFunc = 0,
		void            *progressArg = 0);

	~GTRasterWarpReader(void);

	/*释放GTRasterWarpReader对象指针的静态函数, pDataset必须是new显示声明*/
	static void destroyRasterWarpReader(GTRasterWarpReader *pRasterWarp);

	/*获取完全转换之后目标图像的像素宽度*/
	int getDestApprWidth() const { return destApprWidth; }

	/*获取完全转换之后目标图像的像素高度*/
	int getDestApprHeight() const { return destApprHeight; }

	/*获取转换之后目标图像的仿射变换参数*/
	const double* getDestAffineParams() const { return pdfDstGeoTransform; }

	/*是否有效*/
	bool isValid() const { return bValid; }

	/*获取转换之后目标图像的x方向空间分辨率*/
	double getXApprResolution();

	/*获取转换之后目标图像的y方向空间分辨率*/
	double getYApprResolution();
	
	/*获取转换之后目标图像的空间范围*/
	bool getApprSpatialExtent(GTEnvelop &env);

	/*获取转换的目标数据类型*/
	enumGTDataType getDstDataType();

    /*按照给定的空间范围的大小读取一块数据
     *env为实际的空间范围
     *输入*pData必须为空, 返回时像素顺序为从左到右,从上到下
     *(*pData)由内部分配，外部调用GTS_FREE释放
     *nXSize, nYSize为目标区的像素行列数, 
     *由getX(Y)ApprResolution和env大小可以进行计算近似1:1的大小值
     */
    bool readWarpedData(
		const GTEnvelop& env, 
		void**           pData, 
		int              nXSize, 
		int              nYSize);

	void setProgressData(GDALProgressFunc progressFunc, void* progressArg);

private:
	GDALDataset     *poDataset;     /*gdal数据集指针 */
	const char      *pszSrcWKT;     /*原始的坐标定义  */
	const char      *pszDstWKT;     /*目标坐标定义    */
	GDALWarpOptions *psWarpOptions; /*Warp参数结构指针*/

	int    destApprWidth;        /*目标数据块的近似宽度*/
	int    destApprHeight;       /*目标数据块的近似高度*/
	double pdfDstGeoTransform[6];/*目标投影之仿射变换参数*/
	bool   bValid;
};

}
#endif

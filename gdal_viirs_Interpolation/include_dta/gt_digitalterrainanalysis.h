/****************************************************************
* Project:  地理信息栅格处理工具集
* Purpose:  DEM空间分析处理类
* Author:   范俊甫, fanjf@lreis.ac.cn, 2013年-01月-07日
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved
* 版权所有   (c) 中国科学院地理科学与资源研究所
****************************************************************/
#pragma once
#include <cmath>
#include <algorithm>
#include <stack>
#include "gt_rasterio.h"
#include "gts_port.h"
//#include "gt_rasterio.h"

/*无值标识*/
#ifndef NODATA
#	define NODATA -FLT_MAX
#endif

/*判断允许容错*/
#ifndef EPS
#	define EPS 0.00001f
#endif

/*180/pi*/
#ifndef PI180
#	define PI180 57.29577951308233
#endif

/*特征要素标识*/
#ifndef FEATURE_YES
#	define FEATURE_YES 1.0f
#endif

/*非特征要素标识*/
#ifndef FEATURE_NO
#	define FEATURE_NO 0.0f
#endif
/*邻域统计类型*/
enum enumNeighborStatisType{
	MAJORITY = 0, /*众数*/
	MAXIMUM  = 1, /*最大值*/
	MEAN     = 2, /*平均值*/
	MEDIAN   = 3, /*中值*/
	MINIMUM  = 4, /*最小值*/
	MINORITY = 5, /*少数*/
	RANGE    = 6, /*变程*/
	STD      = 7, /*标准差*/
	SUM      = 8, /*总和*/
	VARIETY  = 9  /*变异度：唯一值数目*/
};


class GTS_DLL GTTerrainAnalysisN
{
public:
	GTTerrainAnalysisN(void);
	~GTTerrainAnalysisN(void);

	/*坡度提取
	 *输入DEM数据
	 *输出坡度数据
	*/
	static bool Slope(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);

	/*坡向提取
	 *输入DEM数据
	 *输出坡向数据
	*/
	static bool Aspect(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);

	/*坡度变率提取
	 *输入DEM数据
	 *输出坡度变率数据
	*/
	static bool SlopeRateSOS(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);

	/*坡向变率提取
	 *输入DEM数据
	 *输出坡向变率数据
	*/
	static bool AspectRateSOA(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);
	
	/*全曲率提取
	 *输入DEM数据
	 *输出全曲率数据
	*/
	static bool TotalCurvature(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		
		);

	/*平面曲率提取
	 *输入DEM数据
	 *输出平面曲率数据
	*/
	static bool PlanCurvature(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);

	/*剖面曲率提取
	 *输入DEM数据
	 *输出剖面曲率数据
	*/
	static bool ProfileCurvature(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);

	/*非球面曲率提取
	 *输入DEM数据
	 *输出非球面曲率数据
	*/
	static bool UnsphericityCurvature(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);

	/*曲率差提取
	 *输入DEM数据
	 *输出曲率差数据
	*/
	static bool DifferenceCurvature(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);

	/*最大曲率提取
	 *输入DEM数据
	 *输出最大曲率数据
	*/
	static bool MaxCurvature(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);

	/*最小曲率提取
	 *输入DEM数据
	 *输出最小曲率数据
	*/
	static bool MinCurvature(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);

	/*平均曲率提取
	 *输入DEM数据
	 *输出平均曲率数据
	*/
	static bool MeanCurvature(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo   /*@ResultTo      结果数据源路径*/
		);

	/*全高斯曲率提取
	 *输入DEM数据
	 *输出全高斯曲率数据
	*/
	static bool GaussianCurvature(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*邻域分析－众数
	 *输入DEM数据
	 *输出众数结果数据
	*/
	static bool MajorityNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		int WinSize=3           /*@WinSize       分析窗口大小，默认为3*3的矩形分析窗口*/
		);

	/*邻域分析－最大值
	 *输入DEM数据
	 *输出最大值结果数据
	*/
	static bool MaximumNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		int WinSize=3           /*@WinSize       分析窗口大小，默认为3*3的矩形分析窗口*/
		);

	/*邻域分析－平均值
	 *输入DEM数据
	 *输出平均值结果数据
	*/
	static bool MeanNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		int WinSize=3           /*@WinSize       分析窗口大小，默认为3*3的矩形分析窗口*/
		);

	/*邻域分析－中值
	 *输入DEM数据
	 *输出中值结果数据
	*/
	static bool MedianNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		int WinSize=3           /*@WinSize       分析窗口大小，默认为3*3的矩形分析窗口*/
		);

	/*邻域分析－最小值
	 *输入DEM数据
	 *输出最小值结果数据
	*/
	static bool MinimumNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		int WinSize=3           /*@WinSize       分析窗口大小，默认为3*3的矩形分析窗口*/
		);

	/*邻域分析－少数
	 *输入DEM数据
	 *输出少数结果数据
	*/
	static bool MinorityNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		int WinSize=3           /*@WinSize       分析窗口大小，默认为3*3的矩形分析窗口*/
		);

	/*邻域分析－变程值
	 *输入DEM数据
	 *输出变程值结果数据
	*/
	static bool RangeNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		int WinSize=3           /*@WinSize       分析窗口大小，默认为3*3的矩形分析窗口*/
		);

	/*邻域分析－标准差值
	 *输入DEM数据
	 *输出标准差值结果数据
	*/
	static bool STDNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		int WinSize=3           /*@WinSize       分析窗口大小，默认为3*3的矩形分析窗口*/
		);

	/*邻域分析－总和值
	 *输入DEM数据
	 *输出总和值结果数据
	*/
	static bool SumNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		int WinSize=3           /*@WinSize       分析窗口大小，默认为3*3的矩形分析窗口*/
		);

	/*邻域分析－变异度值
	 *输入DEM数据
	 *输出变异度值结果数据
	*/
	static bool VarityNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		int WinSize=3           /*@WinSize       分析窗口大小，默认为3*3的矩形分析窗口*/
		);

	/*高程偏差值
	 *输入DEM数据
	 *输出高程偏差值结果数据
	*/
	static bool DeviationMean(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*百分位值
	 *输入DEM数据
	 *输出百分位值结果数据
	*/
	static bool ElevationPercentile(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*相对百分位值
	 *输入DEM数据
	 *输出相对百分位值结果数据
	*/
	static bool PercentageRange(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*变异系数
	 *输入DEM数据
	 *输出变异系数结果数据
	*/
	static bool VarianceCoefficient(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*粗糙度
	 *输入DEM数据
	 *输出粗糙度结果数据
	*/
	static bool Roughness(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*切割深度
	 *输入DEM数据
	 *输出切割深度结果数据
	*/
	static bool SurfaceIncision(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*山体阴影提取
	 *输入DEM数据
	 *输出山体阴影数据
	*/
	static bool HillShade(
		const char* SourceFrom,  /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,    /*@ResultTo      结果数据源路径*/
		float Azimuth=315.0f,    /*@Azimuth       太阳方位角 degree*/
		float Altitude=45.0f,    /*@Altitude      太阳高度角 degree*/
		float Z_Factor=1.0f      /*@Z_Factor      Z值的比例因子*/
		);

	/*山肩提取
	 *输入DEM数据
	 *输出山肩结果数据
	*/
	static bool ShoulderSlope(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*陡坡提取
	 *输入DEM数据
	 *输出陡坡结果数据
	*/
	static bool SteepSlope(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);	

	/*缓坡提取
	 *输入DEM数据
	 *输出缓坡结果数据
	*/
	static bool GentleSlope(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);	

	/*山嘴坡提取
	 *输入DEM数据
	 *输出山嘴坡结果数据
	*/
	static bool NoseSlope(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*源头坡提取
	 *输入DEM数据
	 *输出源头坡结果数据
	*/
	static bool HeadSlope(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*背坡提取
	 *输入DEM数据
	 *输出源头坡结果数据
	*/
	static bool BackSlope(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*等值线提取
	 *输入DEM数据
	 *输等值线结果数据
	*/
	static bool Contour(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo,   /*@ResultTo      结果数据源路径*/
		float interval            /*@interval      等间距*/
		);

	/*脊点提取
	 *输入DEM数据
	 *输出脊点结果数据
	*/
	static bool RidgePoint(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*谷点提取
	 *输入DEM数据
	 *输出谷点结果数据
	*/
	static bool ValleyPoint(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*鞍部点提取
	 *输入DEM数据
	 *输出鞍部点结果数据
	*/
	static bool SaddlePoint(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*洼地点提取
	 *输入DEM数据
	 *输出洼地点结果数据
	*/
	static bool SinkPoint(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*平地点提取
	 *输入DEM数据
	 *输出平地点结果数据
	*/
	static bool FlatPoint(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*洼地处理
	 *输入DEM数据
	 *输出洼地处理结果数据
	*/
	static bool Fill(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*水流方向
	 *输入洼地处理后的DEM数据
	 *输出水流方向结果数据
	*/
	static bool FlowDirection(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*汇流累计
	 *输入水流方向数据
	 *输出汇流累计结果数据
	*/
	static bool FlowAccumulation(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*河网水系
	 *输入汇流累计及水流方向数据
	 *输出河网水系结果数据
	*/
	static bool Stream(
		const char* SourceFrom1, /*@SourceFrom1    目标数据源路径1汇流累计*/
		const char* SourceFrom2, /*@SourceFrom2    目标数据源路径2水流方向*/
		const char* ResultTo,    /*@ResultTo      结果数据源路径*/
		const float Threshhold   /*@Threshhold    河网阈值*/
		);

	/*流域分割
	*输入水流方向及河网水系数据
	*输出流域分割结果数据
	*/
	static bool Watershed(
		const char* SourceFrom1, /*@SourceFrom1    目标数据源路径1水流方向*/
		const char* SourceFrom2, /*@SourceFrom2    目标数据源路径2河网水系*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*沟谷节点
	 *输入河网水系和流向数据
	 *输出沟谷节点结果数据
	*/
	static bool GullyRunoff(
		const char* SourceFrom1, /*@SourceFrom1    目标数据源路径1*/
		const char* SourceFrom2, /*@SourceFrom2    目标数据源路径2*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*沟谷源点
	 *输入河网水系和流向数据
	 *输出沟谷节点结果数据
	*/
	static bool GullySourceNode(
		const char* SourceFrom1, /*@SourceFrom1    目标数据源路径1*/
		const char* SourceFrom2, /*@SourceFrom2    目标数据源路径2*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*流域出水口
	 *输入河网水系和流向数据
	 *输出沟谷节点结果数据
	*/
	static bool Outlet(
		const char* SourceFrom1, /*@SourceFrom1    目标数据源路径1*/
		const char* SourceFrom2, /*@SourceFrom2    目标数据源路径2*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);
	
	/*单位汇水面积
	 *输入流向和汇流累积数据
	 *输出单位汇水面积结果数据
	*/
	static bool SpecificCatchmentArea(
		const char* SourceFrom1, /*@SourceFrom1    目标数据源路径1*/
		const char* SourceFrom2, /*@SourceFrom2    目标数据源路径2*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*输沙能力指数
	 *输入单位汇水面积和坡度数据
	 *输出地形湿度指数结果数据
	*/
	static bool SedimentTransportCapacityIndex(
		const char* SourceFrom1, /*@SourceFrom1    目标数据源路径1*/
		const char* SourceFrom2, /*@SourceFrom2    目标数据源路径2*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*水流强度指数
	 *输入单位汇水面积和坡度数据
	 *输出地形湿度指数结果数据
	*/
	static bool StreamPowerIndex(
		const char* SourceFrom1, /*@SourceFrom1    目标数据源路径1*/
		const char* SourceFrom2, /*@SourceFrom2    目标数据源路径2*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*地形湿度指数
	 *输入单位汇水面积和坡度数据
	 *输出地形湿度指数结果数据
	*/
	static bool TopographicWetnessIndex(
		const char* SourceFrom1, /*@SourceFrom1    目标数据源路径1*/
		const char* SourceFrom2, /*@SourceFrom2    目标数据源路径2*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*点点通视
	 *输入原始DEM数据
	 *输入起始点坐标及离地面高度数据
	 *输入目标点坐标及离地面高度数据
	 *输出点点通视结果
	*/
	static bool PointtoPointVisible(
		const char* SourceFrom, /*@SourceFrom  数据源路径*/
		float xCoorD0,     /*@xCoorD0  起始点X坐标*/
		float yCoorD0,     /*@yCoorD0  起始点Y坐标*/
		float offSet0,     /*@offSet0  起始点离地面高度*/
		float xCoorD1,     /*@xCoorD1  目标点X坐标*/
		float yCoorD1,     /*@yCoorD1  目标点Y坐标*/
		float offSet1      /*@offSet1  目标点离地面高度*/
		);

	/*可视域
	 *输入原始DEM数据
	 *输入起始点坐标及离地面高度数据
	 *输出可视域结果
	*/
	static bool Viewshed(
		const char* SourceFrom, /*@SourceFrom  数据源路径*/
		float xCoorD0,     /*@xCoorD0  起始点X坐标*/
		float yCoorD0,     /*@yCoorD0  起始点Y坐标*/
		float offSet0,     /*@offSet0  起始点离地面高度*/
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*等差分级
	 *输入原始DEM数据
	 *输入分级级数
	 *输出等级分级结果数据
	*/
	static bool EqualIntervalClassify(
		const char* SourceFrom, /*@SourceFrom  目标数据源路径*/
		const int ClassCount, /*@ClassCount   等差分级级数*/     
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*标准差分级
	 *输入原始DEM数据
	 *输出标准差分级结果数据
	*/
	static bool StandardDevClassify(
		const char* SourceFrom, /*@SourceFrom    目标数据源路径*/   
		const int Std_Dev,      /*@ClassCount   标准差倍数的倒数*/     
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*自然裂点分级
	 *输入原始DEM数据
	 *输入分级级数
	 *输出等级分级结果数据
	*/
	static bool NaturalBreaksClassify(
		const char* SourceFrom, /*@SourceFrom  目标数据源路径*/
		const int ClassCount,   /*@ClassCount   自然裂点分级级数*/     
		const char* ResultTo    /*@ResultTo      结果数据源路径*/
		);

	/*****************************************************************************
	* @brief	计算地形起伏度（3*3窗口大小，仅支持GTiff数据格式）。
	* @param	[in]
	*			SourceFrom			输入DEM数据路径
	*			ResultTo			结果数据源路径
	* @param	[out]
	* @author	范俊甫,fanjf@lreis.ac.cn,2013-06-20
	*******************************************************************************/
	static bool ReliefDegree( const char* SourceFrom, const char* ResultTo );

	private:
		/*获取数组数据的频率数－众数和少数*/
		static float __getFrequence(
			GTRaster2D* sourcedata,
			int row,
			int col,
			int winsize,
			enumNeighborStatisType enst
			);

		/*获取单元的流向单元*/
		static bool __findNext(
			GTRaster2D *source,
			int row, 
			int col, 
			int *r, 
			int *c );
	
		/*获取单元的流向单元*/
		static bool __isDirectionCell(
			GTRaster2D *source,
			int row, 
			int col
			);

		static void __setWatershedCell(
            GTRaster2D *sourcedir,
            GTRaster2D *sourcestm,
            GTRaster2D *sourcerlt,
            int row,                
            int col);

		/*获取栅格数据的平均值*/
		static float __getmeanvalue(
			GTRaster2D *source);

		/*获取栅格数据的标准差*/
		static float __getstd_devvalue(
			GTRaster2D *source);

		/*计算是否可视*/
		static bool __isVisible(
			GTRaster2D* source, 
			int row0,
			int col0,
			float elevation0,
			int row1,
			int col1);
};


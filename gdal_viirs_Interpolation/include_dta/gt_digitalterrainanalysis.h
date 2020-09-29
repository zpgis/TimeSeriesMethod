/****************************************************************
* Project:  ������Ϣդ�����߼�
* Purpose:  DEM�ռ����������
* Author:   ������, fanjf@lreis.ac.cn, 2013��-01��-07��
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved
* ��Ȩ����   (c) �й���ѧԺ�����ѧ����Դ�о���
****************************************************************/
#pragma once
#include <cmath>
#include <algorithm>
#include <stack>
#include "gt_rasterio.h"
#include "gts_port.h"
//#include "gt_rasterio.h"

/*��ֵ��ʶ*/
#ifndef NODATA
#	define NODATA -FLT_MAX
#endif

/*�ж������ݴ�*/
#ifndef EPS
#	define EPS 0.00001f
#endif

/*180/pi*/
#ifndef PI180
#	define PI180 57.29577951308233
#endif

/*����Ҫ�ر�ʶ*/
#ifndef FEATURE_YES
#	define FEATURE_YES 1.0f
#endif

/*������Ҫ�ر�ʶ*/
#ifndef FEATURE_NO
#	define FEATURE_NO 0.0f
#endif
/*����ͳ������*/
enum enumNeighborStatisType{
	MAJORITY = 0, /*����*/
	MAXIMUM  = 1, /*���ֵ*/
	MEAN     = 2, /*ƽ��ֵ*/
	MEDIAN   = 3, /*��ֵ*/
	MINIMUM  = 4, /*��Сֵ*/
	MINORITY = 5, /*����*/
	RANGE    = 6, /*���*/
	STD      = 7, /*��׼��*/
	SUM      = 8, /*�ܺ�*/
	VARIETY  = 9  /*����ȣ�Ψһֵ��Ŀ*/
};


class GTS_DLL GTTerrainAnalysisN
{
public:
	GTTerrainAnalysisN(void);
	~GTTerrainAnalysisN(void);

	/*�¶���ȡ
	 *����DEM����
	 *����¶�����
	*/
	static bool Slope(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);

	/*������ȡ
	 *����DEM����
	 *�����������
	*/
	static bool Aspect(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);

	/*�¶ȱ�����ȡ
	 *����DEM����
	 *����¶ȱ�������
	*/
	static bool SlopeRateSOS(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);

	/*���������ȡ
	 *����DEM����
	 *��������������
	*/
	static bool AspectRateSOA(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);
	
	/*ȫ������ȡ
	 *����DEM����
	 *���ȫ��������
	*/
	static bool TotalCurvature(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		
		);

	/*ƽ��������ȡ
	 *����DEM����
	 *���ƽ����������
	*/
	static bool PlanCurvature(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);

	/*����������ȡ
	 *����DEM����
	 *���������������
	*/
	static bool ProfileCurvature(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);

	/*������������ȡ
	 *����DEM����
	 *�����������������
	*/
	static bool UnsphericityCurvature(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);

	/*���ʲ���ȡ
	 *����DEM����
	 *������ʲ�����
	*/
	static bool DifferenceCurvature(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);

	/*���������ȡ
	 *����DEM����
	 *��������������
	*/
	static bool MaxCurvature(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);

	/*��С������ȡ
	 *����DEM����
	 *�����С��������
	*/
	static bool MinCurvature(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);

	/*ƽ��������ȡ
	 *����DEM����
	 *���ƽ����������
	*/
	static bool MeanCurvature(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo   /*@ResultTo      �������Դ·��*/
		);

	/*ȫ��˹������ȡ
	 *����DEM����
	 *���ȫ��˹��������
	*/
	static bool GaussianCurvature(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*�������������
	 *����DEM����
	 *��������������
	*/
	static bool MajorityNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		int WinSize=3           /*@WinSize       �������ڴ�С��Ĭ��Ϊ3*3�ľ��η�������*/
		);

	/*������������ֵ
	 *����DEM����
	 *������ֵ�������
	*/
	static bool MaximumNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		int WinSize=3           /*@WinSize       �������ڴ�С��Ĭ��Ϊ3*3�ľ��η�������*/
		);

	/*���������ƽ��ֵ
	 *����DEM����
	 *���ƽ��ֵ�������
	*/
	static bool MeanNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		int WinSize=3           /*@WinSize       �������ڴ�С��Ĭ��Ϊ3*3�ľ��η�������*/
		);

	/*�����������ֵ
	 *����DEM����
	 *�����ֵ�������
	*/
	static bool MedianNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		int WinSize=3           /*@WinSize       �������ڴ�С��Ĭ��Ϊ3*3�ľ��η�������*/
		);

	/*�����������Сֵ
	 *����DEM����
	 *�����Сֵ�������
	*/
	static bool MinimumNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		int WinSize=3           /*@WinSize       �������ڴ�С��Ĭ��Ϊ3*3�ľ��η�������*/
		);

	/*�������������
	 *����DEM����
	 *��������������
	*/
	static bool MinorityNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		int WinSize=3           /*@WinSize       �������ڴ�С��Ĭ��Ϊ3*3�ľ��η�������*/
		);

	/*������������ֵ
	 *����DEM����
	 *������ֵ�������
	*/
	static bool RangeNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		int WinSize=3           /*@WinSize       �������ڴ�С��Ĭ��Ϊ3*3�ľ��η�������*/
		);

	/*�����������׼��ֵ
	 *����DEM����
	 *�����׼��ֵ�������
	*/
	static bool STDNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		int WinSize=3           /*@WinSize       �������ڴ�С��Ĭ��Ϊ3*3�ľ��η�������*/
		);

	/*����������ܺ�ֵ
	 *����DEM����
	 *����ܺ�ֵ�������
	*/
	static bool SumNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		int WinSize=3           /*@WinSize       �������ڴ�С��Ĭ��Ϊ3*3�ľ��η�������*/
		);

	/*��������������ֵ
	 *����DEM����
	 *��������ֵ�������
	*/
	static bool VarityNeighborStatistic(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		int WinSize=3           /*@WinSize       �������ڴ�С��Ĭ��Ϊ3*3�ľ��η�������*/
		);

	/*�߳�ƫ��ֵ
	 *����DEM����
	 *����߳�ƫ��ֵ�������
	*/
	static bool DeviationMean(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*�ٷ�λֵ
	 *����DEM����
	 *����ٷ�λֵ�������
	*/
	static bool ElevationPercentile(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*��԰ٷ�λֵ
	 *����DEM����
	 *�����԰ٷ�λֵ�������
	*/
	static bool PercentageRange(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*����ϵ��
	 *����DEM����
	 *�������ϵ���������
	*/
	static bool VarianceCoefficient(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*�ֲڶ�
	 *����DEM����
	 *����ֲڶȽ������
	*/
	static bool Roughness(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*�и����
	 *����DEM����
	 *����и���Ƚ������
	*/
	static bool SurfaceIncision(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*ɽ����Ӱ��ȡ
	 *����DEM����
	 *���ɽ����Ӱ����
	*/
	static bool HillShade(
		const char* SourceFrom,  /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,    /*@ResultTo      �������Դ·��*/
		float Azimuth=315.0f,    /*@Azimuth       ̫����λ�� degree*/
		float Altitude=45.0f,    /*@Altitude      ̫���߶Ƚ� degree*/
		float Z_Factor=1.0f      /*@Z_Factor      Zֵ�ı�������*/
		);

	/*ɽ����ȡ
	 *����DEM����
	 *���ɽ��������
	*/
	static bool ShoulderSlope(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*������ȡ
	 *����DEM����
	 *������½������
	*/
	static bool SteepSlope(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);	

	/*������ȡ
	 *����DEM����
	 *������½������
	*/
	static bool GentleSlope(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);	

	/*ɽ������ȡ
	 *����DEM����
	 *���ɽ���½������
	*/
	static bool NoseSlope(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*Դͷ����ȡ
	 *����DEM����
	 *���Դͷ�½������
	*/
	static bool HeadSlope(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*������ȡ
	 *����DEM����
	 *���Դͷ�½������
	*/
	static bool BackSlope(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*��ֵ����ȡ
	 *����DEM����
	 *���ֵ�߽������
	*/
	static bool Contour(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo,   /*@ResultTo      �������Դ·��*/
		float interval            /*@interval      �ȼ��*/
		);

	/*������ȡ
	 *����DEM����
	 *�������������
	*/
	static bool RidgePoint(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*�ȵ���ȡ
	 *����DEM����
	 *����ȵ�������
	*/
	static bool ValleyPoint(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*��������ȡ
	 *����DEM����
	 *���������������
	*/
	static bool SaddlePoint(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*�ݵص���ȡ
	 *����DEM����
	 *����ݵص�������
	*/
	static bool SinkPoint(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*ƽ�ص���ȡ
	 *����DEM����
	 *���ƽ�ص�������
	*/
	static bool FlatPoint(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*�ݵش���
	 *����DEM����
	 *����ݵش���������
	*/
	static bool Fill(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*ˮ������
	 *�����ݵش�����DEM����
	 *���ˮ������������
	*/
	static bool FlowDirection(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*�����ۼ�
	 *����ˮ����������
	 *��������ۼƽ������
	*/
	static bool FlowAccumulation(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*����ˮϵ
	 *��������ۼƼ�ˮ����������
	 *�������ˮϵ�������
	*/
	static bool Stream(
		const char* SourceFrom1, /*@SourceFrom1    Ŀ������Դ·��1�����ۼ�*/
		const char* SourceFrom2, /*@SourceFrom2    Ŀ������Դ·��2ˮ������*/
		const char* ResultTo,    /*@ResultTo      �������Դ·��*/
		const float Threshhold   /*@Threshhold    ������ֵ*/
		);

	/*����ָ�
	*����ˮ�����򼰺���ˮϵ����
	*�������ָ�������
	*/
	static bool Watershed(
		const char* SourceFrom1, /*@SourceFrom1    Ŀ������Դ·��1ˮ������*/
		const char* SourceFrom2, /*@SourceFrom2    Ŀ������Դ·��2����ˮϵ*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*���Ƚڵ�
	 *�������ˮϵ����������
	 *������Ƚڵ�������
	*/
	static bool GullyRunoff(
		const char* SourceFrom1, /*@SourceFrom1    Ŀ������Դ·��1*/
		const char* SourceFrom2, /*@SourceFrom2    Ŀ������Դ·��2*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*����Դ��
	 *�������ˮϵ����������
	 *������Ƚڵ�������
	*/
	static bool GullySourceNode(
		const char* SourceFrom1, /*@SourceFrom1    Ŀ������Դ·��1*/
		const char* SourceFrom2, /*@SourceFrom2    Ŀ������Դ·��2*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*�����ˮ��
	 *�������ˮϵ����������
	 *������Ƚڵ�������
	*/
	static bool Outlet(
		const char* SourceFrom1, /*@SourceFrom1    Ŀ������Դ·��1*/
		const char* SourceFrom2, /*@SourceFrom2    Ŀ������Դ·��2*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);
	
	/*��λ��ˮ���
	 *��������ͻ����ۻ�����
	 *�����λ��ˮ����������
	*/
	static bool SpecificCatchmentArea(
		const char* SourceFrom1, /*@SourceFrom1    Ŀ������Դ·��1*/
		const char* SourceFrom2, /*@SourceFrom2    Ŀ������Դ·��2*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*��ɳ����ָ��
	 *���뵥λ��ˮ������¶�����
	 *�������ʪ��ָ���������
	*/
	static bool SedimentTransportCapacityIndex(
		const char* SourceFrom1, /*@SourceFrom1    Ŀ������Դ·��1*/
		const char* SourceFrom2, /*@SourceFrom2    Ŀ������Դ·��2*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*ˮ��ǿ��ָ��
	 *���뵥λ��ˮ������¶�����
	 *�������ʪ��ָ���������
	*/
	static bool StreamPowerIndex(
		const char* SourceFrom1, /*@SourceFrom1    Ŀ������Դ·��1*/
		const char* SourceFrom2, /*@SourceFrom2    Ŀ������Դ·��2*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*����ʪ��ָ��
	 *���뵥λ��ˮ������¶�����
	 *�������ʪ��ָ���������
	*/
	static bool TopographicWetnessIndex(
		const char* SourceFrom1, /*@SourceFrom1    Ŀ������Դ·��1*/
		const char* SourceFrom2, /*@SourceFrom2    Ŀ������Դ·��2*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*���ͨ��
	 *����ԭʼDEM����
	 *������ʼ�����꼰�����߶�����
	 *����Ŀ������꼰�����߶�����
	 *������ͨ�ӽ��
	*/
	static bool PointtoPointVisible(
		const char* SourceFrom, /*@SourceFrom  ����Դ·��*/
		float xCoorD0,     /*@xCoorD0  ��ʼ��X����*/
		float yCoorD0,     /*@yCoorD0  ��ʼ��Y����*/
		float offSet0,     /*@offSet0  ��ʼ�������߶�*/
		float xCoorD1,     /*@xCoorD1  Ŀ���X����*/
		float yCoorD1,     /*@yCoorD1  Ŀ���Y����*/
		float offSet1      /*@offSet1  Ŀ��������߶�*/
		);

	/*������
	 *����ԭʼDEM����
	 *������ʼ�����꼰�����߶�����
	 *�����������
	*/
	static bool Viewshed(
		const char* SourceFrom, /*@SourceFrom  ����Դ·��*/
		float xCoorD0,     /*@xCoorD0  ��ʼ��X����*/
		float yCoorD0,     /*@yCoorD0  ��ʼ��Y����*/
		float offSet0,     /*@offSet0  ��ʼ�������߶�*/
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*�Ȳ�ּ�
	 *����ԭʼDEM����
	 *����ּ�����
	 *����ȼ��ּ��������
	*/
	static bool EqualIntervalClassify(
		const char* SourceFrom, /*@SourceFrom  Ŀ������Դ·��*/
		const int ClassCount, /*@ClassCount   �Ȳ�ּ�����*/     
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*��׼��ּ�
	 *����ԭʼDEM����
	 *�����׼��ּ��������
	*/
	static bool StandardDevClassify(
		const char* SourceFrom, /*@SourceFrom    Ŀ������Դ·��*/   
		const int Std_Dev,      /*@ClassCount   ��׼����ĵ���*/     
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*��Ȼ�ѵ�ּ�
	 *����ԭʼDEM����
	 *����ּ�����
	 *����ȼ��ּ��������
	*/
	static bool NaturalBreaksClassify(
		const char* SourceFrom, /*@SourceFrom  Ŀ������Դ·��*/
		const int ClassCount,   /*@ClassCount   ��Ȼ�ѵ�ּ�����*/     
		const char* ResultTo    /*@ResultTo      �������Դ·��*/
		);

	/*****************************************************************************
	* @brief	�����������ȣ�3*3���ڴ�С����֧��GTiff���ݸ�ʽ����
	* @param	[in]
	*			SourceFrom			����DEM����·��
	*			ResultTo			�������Դ·��
	* @param	[out]
	* @author	������,fanjf@lreis.ac.cn,2013-06-20
	*******************************************************************************/
	static bool ReliefDegree( const char* SourceFrom, const char* ResultTo );

	private:
		/*��ȡ�������ݵ�Ƶ����������������*/
		static float __getFrequence(
			GTRaster2D* sourcedata,
			int row,
			int col,
			int winsize,
			enumNeighborStatisType enst
			);

		/*��ȡ��Ԫ������Ԫ*/
		static bool __findNext(
			GTRaster2D *source,
			int row, 
			int col, 
			int *r, 
			int *c );
	
		/*��ȡ��Ԫ������Ԫ*/
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

		/*��ȡդ�����ݵ�ƽ��ֵ*/
		static float __getmeanvalue(
			GTRaster2D *source);

		/*��ȡդ�����ݵı�׼��*/
		static float __getstd_devvalue(
			GTRaster2D *source);

		/*�����Ƿ����*/
		static bool __isVisible(
			GTRaster2D* source, 
			int row0,
			int col0,
			float elevation0,
			int row1,
			int col1);
};


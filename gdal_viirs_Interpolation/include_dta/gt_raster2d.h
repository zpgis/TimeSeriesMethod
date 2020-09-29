/****************************************************************
* Project:  地理信息栅格处理工具集
* Purpose:  栅格信息管理类
* Author:   范俊甫, fanjf@lreis.ac.cn, 2013年-01月-06日
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved
* 版权所有   (c) 中国科学院地理科学与资源研究所
****************************************************************/
#pragma once

#include <iostream>
using namespace std;

const float eps = 0.0001f; /*判断允许容错*/
const float PI = 3.141593f;

/*//投影单位转换值(含义为1米等于多少其他单位),未启用
const float UNIT_FOOT=0.3048f;
const float UNIT_FOOT_US=0.30480f;
const float UNIT_NAUTICAL_MILE=1852.0f;
const float UNIT_LINK=0.201167f;
const float UNIT_CHAIN=20.116684f;
const float UNIT_ROD =5.029210f;
const float UNIT_KILOMETER=0.001f;
//大地坐标单位转换值(含义为1弧度等于多少其他单位)
const float UNIT_DEGREE=0.017453f;*/

enum referenceType/*栅格空间参考类型 */
{
	ProjectRef,
	GeographicRef,
	UnknownRef
};

/*//地图坐标单位类型，未启用
enum enumGTUnitType {
	GT_UNIT_UNKNOWN       = 0,
	//大地坐标
	GT_UNIT_DEGREE        = 1, //0.0174532925199433
	GT_UNIT_RADIAN        = 2, //1.0
	投影坐标
	GT_UNIT_METER         = 3, //1.0
	GT_UNIT_FOOT          = 4, //0.3048
	GT_UNIT_FOOT_US       = 5, //0.3048006096012129
	GT_UNIT_NAUTICAL_MILE = 6, //1852.0
	GT_UNIT_LINK          = 7, //0.20116684023368047
	GT_UNIT_CHAIN         = 8, //20.116684023369047
	GT_UNIT_ROD           = 9, //5.02921005842012
	GT_UNIT_KILOMETER     = 10 //0.001
};*/

struct GTRaster2D
{
public:

    GTRaster2D(
			   float xTopLeft,/*左上角x坐标*/
		       float yTopLeft,/*左上角y坐标*/
			   float cellSize,/*栅格单元大小，假设x、y方向相同*/
			   int nRow, /*行数*/
			   int nCol,/*列数*/
			   float noDataValue)/*无值标识*/
    {
        this->_xTopLeft = xTopLeft;
        this->_yTopLeft = yTopLeft;
        this->_cellSize = cellSize;
        this->_nRow = nRow;
        this->_nCol = nCol;
        this->_noDataValue = noDataValue;
        this->_dataPtr = new float[nRow * nCol];

        this->SetNoDataValueCount(0);        

		_isEmpty = true;
		_semimajor = 6371.009f;
    }

    ~GTRaster2D(void)
    {
        if (this->_dataPtr != NULL)
        {
            delete[] this->_dataPtr;
        }
    }

    // 获取左上角X坐标
    float XTopLeft() const
    {
        return _xTopLeft;
    }

    // 获取左上角Y坐标
    float YTopLeft() const
    {
        return _yTopLeft;
    }

    // 获取单元大小尺寸
    float CellSize() const
    {
        return _cellSize;
    }

    // 获取无效值类型
    float NoDataValue() const
    {
        return _noDataValue;
    }
	//空间参考系统
	const char* SpatialRef() const
	{
		return _spatialRef.c_str();
	}    
	
	// 设置空间参考系统
	void SetSpatialRef(const char* strRef)
	{
		_spatialRef = string(strRef);
	}

	//空间参考类型
	referenceType spatialRefType() const
	{
		return _spatialRefType;
	}

	// 设置空间参考系统类型
	void SetspatialRefType(referenceType refType)
	{
		_spatialRefType = refType;
	}

	// 设置椭球体长半轴值
	void SetSimiMajor(float SetSimiMajorValue)
	{
		_semimajor = SetSimiMajorValue;
	}
	//设置转换单位值
	void SetConvetUnit(float convertunit)
	{
		_converunit=convertunit;
	}

	//获取投影空间参考下的单元大小值,统一为米（由于高程单位国际默认为米）
	float GetProjectCellSize()
	{
		if(_spatialRefType== ProjectRef) 
		{
			return _cellSize/_converunit;
		}
		else if( _spatialRefType== GeographicRef)
		{
			float pointX1=_yTopLeft/_converunit;
			float pointY1=_xTopLeft/_converunit;
			float pointY2=(_xTopLeft+_cellSize)/_converunit;
			float pointX2=(_yTopLeft-_cellSize)/_converunit;


			double dL = (pointX2 - pointX1)*PI/180.0;
			double d1 =  cos(pointY2*PI/180.0)*sin(dL);
			double d2 = (cos(pointY1*PI/180.0)*sin(pointY2*PI/180.0) - 
				sin(pointY1*PI/180.0)*cos(pointY2*PI/180.0)*cos(dL));
			double d3 = (sin(pointY1*PI/180.0)*sin(pointY2*PI/180.0) + 
				cos(pointY1*PI/180.0)*cos(pointY2*PI/180.0)*cos(dL));
			d1 = d1*d1;
			d2 = d2*d2;
			d1 = sqrt(d1 + d2);
			double dB = atan2(d1, d3);
			return (float)dB*_semimajor*1000;
		}
		else//unknown
		{
			return _cellSize;
		}
	}

    // 获取无效值个数
    int GetNoDataValueCount() const
    {
        return _noDataValueCount;
    }

    // 设置无效值个数
    void SetNoDataValueCount(int count)
    {
        _noDataValueCount = count;
    }

    // 获取数据指针
    float* DataPtr() const
    {
        return _dataPtr;
    }

    // 得到栅格最小值
    float GetMinValue() const
    {
        return _minValue;
    }

    // 设置栅格最小值
    void SetMinValue(const float v)
    {
        _minValue = v;
    }

    // 得到栅格最大值
    float GetMaxValue() const
    {
        return _maxValue;
    }

    // 设置栅格最大值
    void SetMaxValue(float v)
    {
        _maxValue = v;
    }

    // 获取行数
    int NRow() const
    {
        return _nRow;
    }

    // 获取列数
    int NCol() const
    {
        return _nCol;
    }

    // 根据下标获取高程值
    float GetValue(int row, int col)
    {
        if (IsValidIndex(row, col))
        {
            return _dataPtr[col + NCol() * row];
        }
        return NoDataValue();
    }

	// 根据下标设置高程值
    void SetValue(int row, int col, float v)
    {
        if (IsValidIndex(row, col))
        {
            _dataPtr[col + NCol() * row] = v;

			if (isNoData(v))
			{
				SetNoDataValueCount(GetNoDataValueCount() + 1);
			}else 
			{
				if (_isEmpty)
				{
					_minValue = _maxValue = v;
					_isEmpty = false;

				}else
				{
					if (_minValue > v)
					{
						_minValue = v;
					}

					if (_maxValue < v)
					{
						_maxValue = v;
					}
				}
	        }
	    }
	}

    // 得到给定位置、给定窗口大小内的平均高程，winSize必须为奇数
    inline float GetMeanValueInWindow(int row, int col, int winSize)
    {
		float sumValue = 0.0f;
		int count = 0;
		for (int r = row - winSize / 2; r <= row + winSize / 2; r++)
		{
			for (int c = col - winSize / 2; c <= col + winSize / 2; c++)
			{
				if (!IsValidIndex(r, c) ) continue;
				
				if (isNoData(GetValue(r, c))) continue;
				
				sumValue += GetValue(r, c);
				count++;
			}
		}

		if (count != 0)
		{
			return sumValue / count;
		}

        return NoDataValue();	// TODO
    }

	// 得到给定位置、给定窗口大小内的高程标准差，winSize必须为奇数
	inline float GetStandValueInWindow( int row, int col, int winSize )
	{
		float meanValue = GetMeanValueInWindow(row, col, winSize);

		float sumValue = 0.0f;
		int count = 0;
		for (int r = row - winSize / 2; r <= row + winSize / 2; r++)
		{
			for (int c = col - winSize / 2; c <= col + winSize / 2; c++)
			{
				if (!IsValidIndex(r, c) ) continue;

				if (isNoData(GetValue(r, c))) continue;

				sumValue += (GetValue(r, c) - meanValue) * (GetValue(r, c) - meanValue);
				count++;
			}
		}

		if (count != 0)
		{
			return sqrt(sumValue / count);
		}

		return NoDataValue();

	}

    // 判断下标是否为有效值
    bool IsValidIndex(int row, int col)
    {
        if (row < 0 || row >= NRow() || col < 0 || col >= NCol())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    // 根据地理坐标获取高程值
    float GetElevByCoord( float xCoord, float yCoord )
    {
        int col = ( int ) ( ( xCoord - XTopLeft() ) / CellSize() );

        int row = ( int ) ( ( YTopLeft() - yCoord ) / CellSize() );

        float e = GetValue( row, col );

        return e;
    }

    // 根据x坐标计算列号
    inline int GetColByXCoord( float xCoord )
    {
        return ( int ) ( ( xCoord - XTopLeft() ) / CellSize() );
    }

    // 根据y坐标计算行号
    inline int GetRowByYCoord( float yCoord )
    {
        return ( int ) ( ( YTopLeft() - yCoord) / CellSize() );
    }

    // 根据列号获取x坐标
    inline float GetXCoordByCol( int col )
    {
        return col * CellSize() + XTopLeft();
    }

    // 根据行号获取y坐标
    inline float GetYCoordByRow( int row )
    {
        return YTopLeft() - row * CellSize() ;
    }

    // 判断是否为无效值
    inline bool isNoData(float v)
    {
        if (fabs(v - NoDataValue()) < eps)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

	// 判断窗口内是否有无效值，注意winSize必须为奇数
	inline bool hasNodataInWindow(int row, int col, int winSize)
	{
		int topRow = row + winSize / 2;
		int bottomRow = row - winSize / 2;

		int LeftCol = col - winSize / 2;
		int RightCol = col + winSize / 2;

		for (int r = bottomRow; r <= topRow; r++)
		{
			for (int c = LeftCol; c <= RightCol; c++)
			{
				if (!IsValidIndex(r, c)) return true;

				if (isNoData(GetValue(r, c))) return true;				
			}
		}

		return false;
	}

    // 初始化栅格值
    void initValue(float v)
    {		
        for (int row = 0; row < NRow(); row++)
        {
            for(int col = 0; col < NCol(); col++)
            {
                SetValue(row, col, v);
            }
        }

		_minValue = _maxValue = v;
    }

	// 计算某点的3*3坡度
	float GetSlope(int row, int col)
	{
		if (!IsValidIndex(row, col))	return NoDataValue();

		if (isNoData(GetValue(row, col)))	return NoDataValue();

		float h[9] = { 0 };

		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				int currentRow = r + row - 1;
				int currentCol = c + col - 1;

				if (IsValidIndex(currentRow, currentCol))
				{
					float v = GetValue(currentRow, currentCol);
					if (!isNoData(v))
					{
						h[c + r*3] = v;
					}
				}
			}
		}		

		/*
		_________________________________
		_______h0________h1_______h2____
		_______h3________h4_______h5____
		_______h6________h7_______h8____
		________________________________
		*/ 

		float fx = (h[8] + 2 * h[5] + h[2] - h[6] - 2 * h[3] - h[0]) / ( 8 * CellSize() );
		float fy = (h[2] + 2 * h[1] + h[0] - h[6] - 2 * h[7] - h[8]) / ( 8 * CellSize() );

		float slope = atan( sqrt( fx*fx + fy*fy) ) * ( PI / 180);

		return slope;			
	}

private:

    float _xTopLeft;

    float _yTopLeft;

    float _cellSize;

    int _nRow;

    int _nCol;

    float _noDataValue;

    int _noDataValueCount;

    float* _dataPtr;

    float _minValue;

    float _maxValue;

	bool _isEmpty;

	std::string _spatialRef;

	referenceType _spatialRefType;

	float _semimajor;//椭球体长半轴

	float _converunit;//单位转换值，如果是大地坐标，则是1度等于多少其他单位；如果是投影坐标，则是1米等于多少其他单位


};
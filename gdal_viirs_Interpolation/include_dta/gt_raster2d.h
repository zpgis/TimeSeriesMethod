/****************************************************************
* Project:  ������Ϣդ�����߼�
* Purpose:  դ����Ϣ������
* Author:   ������, fanjf@lreis.ac.cn, 2013��-01��-06��
*
****************************************************************
* Copyright (c) IGSNRR, CAS, China. All rights reserved
* ��Ȩ����   (c) �й���ѧԺ�����ѧ����Դ�о���
****************************************************************/
#pragma once

#include <iostream>
using namespace std;

const float eps = 0.0001f; /*�ж������ݴ�*/
const float PI = 3.141593f;

/*//ͶӰ��λת��ֵ(����Ϊ1�׵��ڶ���������λ),δ����
const float UNIT_FOOT=0.3048f;
const float UNIT_FOOT_US=0.30480f;
const float UNIT_NAUTICAL_MILE=1852.0f;
const float UNIT_LINK=0.201167f;
const float UNIT_CHAIN=20.116684f;
const float UNIT_ROD =5.029210f;
const float UNIT_KILOMETER=0.001f;
//������굥λת��ֵ(����Ϊ1���ȵ��ڶ���������λ)
const float UNIT_DEGREE=0.017453f;*/

enum referenceType/*դ��ռ�ο����� */
{
	ProjectRef,
	GeographicRef,
	UnknownRef
};

/*//��ͼ���굥λ���ͣ�δ����
enum enumGTUnitType {
	GT_UNIT_UNKNOWN       = 0,
	//�������
	GT_UNIT_DEGREE        = 1, //0.0174532925199433
	GT_UNIT_RADIAN        = 2, //1.0
	ͶӰ����
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
			   float xTopLeft,/*���Ͻ�x����*/
		       float yTopLeft,/*���Ͻ�y����*/
			   float cellSize,/*դ��Ԫ��С������x��y������ͬ*/
			   int nRow, /*����*/
			   int nCol,/*����*/
			   float noDataValue)/*��ֵ��ʶ*/
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

    // ��ȡ���Ͻ�X����
    float XTopLeft() const
    {
        return _xTopLeft;
    }

    // ��ȡ���Ͻ�Y����
    float YTopLeft() const
    {
        return _yTopLeft;
    }

    // ��ȡ��Ԫ��С�ߴ�
    float CellSize() const
    {
        return _cellSize;
    }

    // ��ȡ��Чֵ����
    float NoDataValue() const
    {
        return _noDataValue;
    }
	//�ռ�ο�ϵͳ
	const char* SpatialRef() const
	{
		return _spatialRef.c_str();
	}    
	
	// ���ÿռ�ο�ϵͳ
	void SetSpatialRef(const char* strRef)
	{
		_spatialRef = string(strRef);
	}

	//�ռ�ο�����
	referenceType spatialRefType() const
	{
		return _spatialRefType;
	}

	// ���ÿռ�ο�ϵͳ����
	void SetspatialRefType(referenceType refType)
	{
		_spatialRefType = refType;
	}

	// ���������峤����ֵ
	void SetSimiMajor(float SetSimiMajorValue)
	{
		_semimajor = SetSimiMajorValue;
	}
	//����ת����λֵ
	void SetConvetUnit(float convertunit)
	{
		_converunit=convertunit;
	}

	//��ȡͶӰ�ռ�ο��µĵ�Ԫ��Сֵ,ͳһΪ�ף����ڸ̵߳�λ����Ĭ��Ϊ�ף�
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

    // ��ȡ��Чֵ����
    int GetNoDataValueCount() const
    {
        return _noDataValueCount;
    }

    // ������Чֵ����
    void SetNoDataValueCount(int count)
    {
        _noDataValueCount = count;
    }

    // ��ȡ����ָ��
    float* DataPtr() const
    {
        return _dataPtr;
    }

    // �õ�դ����Сֵ
    float GetMinValue() const
    {
        return _minValue;
    }

    // ����դ����Сֵ
    void SetMinValue(const float v)
    {
        _minValue = v;
    }

    // �õ�դ�����ֵ
    float GetMaxValue() const
    {
        return _maxValue;
    }

    // ����դ�����ֵ
    void SetMaxValue(float v)
    {
        _maxValue = v;
    }

    // ��ȡ����
    int NRow() const
    {
        return _nRow;
    }

    // ��ȡ����
    int NCol() const
    {
        return _nCol;
    }

    // �����±��ȡ�߳�ֵ
    float GetValue(int row, int col)
    {
        if (IsValidIndex(row, col))
        {
            return _dataPtr[col + NCol() * row];
        }
        return NoDataValue();
    }

	// �����±����ø߳�ֵ
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

    // �õ�����λ�á��������ڴ�С�ڵ�ƽ���̣߳�winSize����Ϊ����
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

	// �õ�����λ�á��������ڴ�С�ڵĸ̱߳�׼�winSize����Ϊ����
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

    // �ж��±��Ƿ�Ϊ��Чֵ
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

    // ���ݵ��������ȡ�߳�ֵ
    float GetElevByCoord( float xCoord, float yCoord )
    {
        int col = ( int ) ( ( xCoord - XTopLeft() ) / CellSize() );

        int row = ( int ) ( ( YTopLeft() - yCoord ) / CellSize() );

        float e = GetValue( row, col );

        return e;
    }

    // ����x��������к�
    inline int GetColByXCoord( float xCoord )
    {
        return ( int ) ( ( xCoord - XTopLeft() ) / CellSize() );
    }

    // ����y��������к�
    inline int GetRowByYCoord( float yCoord )
    {
        return ( int ) ( ( YTopLeft() - yCoord) / CellSize() );
    }

    // �����кŻ�ȡx����
    inline float GetXCoordByCol( int col )
    {
        return col * CellSize() + XTopLeft();
    }

    // �����кŻ�ȡy����
    inline float GetYCoordByRow( int row )
    {
        return YTopLeft() - row * CellSize() ;
    }

    // �ж��Ƿ�Ϊ��Чֵ
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

	// �жϴ������Ƿ�����Чֵ��ע��winSize����Ϊ����
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

    // ��ʼ��դ��ֵ
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

	// ����ĳ���3*3�¶�
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

	float _semimajor;//�����峤����

	float _converunit;//��λת��ֵ������Ǵ�����꣬����1�ȵ��ڶ���������λ�������ͶӰ���꣬����1�׵��ڶ���������λ


};
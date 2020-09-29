#pragma once

#ifndef  DIFFERENCEHISTOGRAM_H
#define DIFFERENCEHISTOGRAM_H

class DifferenceHistogram {
public:
	double startValue;		// ������ʼֵ�� ����Ϊǰ�պ�
	double totalValue;  // �������ܵ���Ԫֵ
	int pixelCount;		// �������� ��Ԫ�ĸ���
	DifferenceHistogram(double startValue_):startValue(startValue_), totalValue(0.0), pixelCount(0){}
};


class Real_Simulate {
public:
	float real;
	float simulate;
};

#endif // ! DIFFERENCEHISTOGRAM_H


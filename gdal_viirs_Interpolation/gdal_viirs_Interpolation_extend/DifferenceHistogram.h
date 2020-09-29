#pragma once

#ifndef  DIFFERENCEHISTOGRAM_H
#define DIFFERENCEHISTOGRAM_H

class DifferenceHistogram {
public:
	double startValue;		// 区间起始值， 区间为前闭后开
	double totalValue;  // 该区间总的像元值
	int pixelCount;		// 该区间内 像元的个数
	DifferenceHistogram(double startValue_):startValue(startValue_), totalValue(0.0), pixelCount(0){}
};


class Real_Simulate {
public:
	float real;
	float simulate;
};

#endif // ! DIFFERENCEHISTOGRAM_H


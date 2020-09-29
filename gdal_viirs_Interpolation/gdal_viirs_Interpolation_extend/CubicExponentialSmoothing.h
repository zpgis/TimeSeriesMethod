#pragma once
#ifndef CUBICEXPONENTIALSMOOTHING_H
#define CUBICEXPONENTIALSMOOTHING_H
#include <vector>
#include <iomanip>
namespace CubicExponentialSmoothingSpace
{

	/*
	 * 采用三次指数平滑法对7月份的数据进行插补，现有7月份之前的6个月，7月份之后的6个月， 
	 *  先使用前6个月对其正向插补，再使用后6个月的数据逆向插补，最终取两者的平均值
	 */
	class CubicExponentialSmoothing {
	public:
		CubicExponentialSmoothing(std::vector<double>& rawData, double alpha) {
			this->alpha = alpha;
			std::vector<double>* s1 = getSmoothValues(rawData);
			std::vector<double>* s2 = getSmoothValues(*s1);
			std::vector<double>* s3 = getSmoothValues(*s2);	

			this->calculateParams(*s1, *s2, *s3);
			delete s1;
			delete s2;
			delete s3;
		}
		~CubicExponentialSmoothing() = default;
		/*
		 * 预测公式  y = a + b*T + c*T*T
		 */
		double prediction() {
			return a + b * 1 + c * 1 * 1;
		}
	private:
		/*
		 * 获取一次平滑值
		 */
		std::vector<double>* getSmoothValues(std::vector<double>& rawData) {
			std::vector<double>* res = new std::vector<double>;

			for (int i = 0; i < rawData.size(); i++) {
				double pre = 0;
				if (i == 0) {
					pre = (1 - alpha) * (rawData[0] + rawData[1] + rawData[2]) / 3 + alpha * rawData[i];
				}
				else {
					pre = alpha * rawData[i] + (1 - alpha) * rawData[i];
				}
				res->push_back(pre);
			}
			return res;
		}
		/*
		 * 计算三次平滑预测公式的参数
		 */
		void calculateParams( std::vector<double>& s1, std::vector<double>& s2, std::vector<double>& s3) {
			int num = s1.size() - 1;
			this->a = 3 * s1[num] - 3 * s2[num] + s3[num];
			this->b = alpha / (2 * (1 - alpha) * (1 - alpha)) * ((6 - 5 * alpha) * s1[num] - (2 * (5 - 4 * alpha)) * s2[num] + (4 - 3 * alpha) * s3[num]);
			this->c = alpha * alpha / (2 * (1 - alpha) * (1 - alpha)) * (s1[num] - 2 * s2[num] + s3[num]);
		}	
	private:
		double a;
		double b;
		double c;
		double alpha;
	};

	/*
	 *  传进来的数值是  12个月的数据，   7月份之前的6个月，7月份之后的6个月， 
	 *  先使用前6个月对其正向插补，再使用后6个月的数据逆向插补，最终取两者的平均值
	 */
	double cubicExponentialSmoothingValue(std::vector<double>& xData, std::vector<double>& yData) {
		std::vector<double> prevData;
		std::vector<double> nextData;  // 后6个月的数据要反向存储

		for (size_t i = 0; i < 6 ; i++)
		{
			prevData.push_back(yData[i]);
		}

		for (size_t i = xData.size() - 1; i >=6; i--)
		{
			nextData.push_back(yData[i]);
		}

		CubicExponentialSmoothing smoothPrev(prevData, 0.3);
		CubicExponentialSmoothing smoothNext(nextData, 0.3);
		if (smoothPrev.prediction() != smoothNext.prediction()) {

			std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);
			//std::cout << "smoothPrev.prediction() = " << smoothPrev.prediction() << " smoothNext.prediction() = " << smoothNext.prediction() << std::endl;
		}
		return (smoothPrev.prediction() + smoothNext.prediction()) / 2;
	}

}
#endif
#pragma once

#ifndef  GRAYFORECASTMODEL_H
#define GRAYFORECASTMODEL_H
#include <vector>
#include <Eigen\Dense>
#include <cmath>
#include <numeric>
#include <iostream>
namespace GFMpace {

	// 使用灰色预测模型进行预测
	class GrayForecastModel {
	public:
		GrayForecastModel(std::vector<double>& rawData) {

			Eigen::MatrixXd* U = getMatrixU(rawData);
			double a = (*U)(0, 0);
			double u = (*U)(1, 0);
			int size = rawData.size();
			// 预测值的累加序列
			std::vector<double> forecastAccumulation{ rawData.front() };

			for (size_t i = 1; i < size + 1; i++)
			{
				double value = (forecastAccumulation.front() - u / a) * exp(-a * (i)) + u / a;
				forecastAccumulation.push_back(value);
			}

			std::vector<double> forecastValues(forecastAccumulation.size()); // 原始序列的预测值
			forecastValues[0] = forecastAccumulation[0];

			for (size_t i = 1; i < forecastAccumulation.size(); i++)
			{
				forecastValues[i] = forecastAccumulation[i] - forecastAccumulation[i - 1];
			}
			// 预测值

			// 采用后验差法进行检验
			std::vector<double> errors;  // 残差   真实值 - 预测值
			std::vector<double> relativeErrors;	// 相对误差    残差除以实际值
			for (size_t i = 1; i < rawData.size(); i++)
			{
				double err = rawData[i] - forecastValues[i];
				double relativeErr = err / rawData[i];
				errors.push_back(err);
				relativeErrors.push_back(relativeErr);
			}

			double xAverage = this->myAccumulate(rawData.begin(), rawData.end()) / rawData.size(); // 求和之后除以元素的个数得到平均值
			double s1 = 0;   // 方差
			for (const auto& d : rawData) {
				s1 += pow(d - xAverage, 2);
			}
			s1 = s1 / rawData.size();

			double eAverage = this->myAccumulate(errors.begin(), errors.end()) / errors.size(); // 残差的均值
			double s2 = 0; // 残差的方差
			for (const auto& d : errors) {
				s2 += pow(d - eAverage, 2);
			}
			s2 = s2 / errors.size();
			// 验差比C  c = s2 / s1
			double c = s2 / s1;  // 如果c 小于等于0.35 那么符合条件
			bool flag = true;
			for (size_t i = 0; i < errors.size(); i++)
			{
				if (abs(errors[i] - eAverage) >= 0.6745 * sqrt(s1)) {
					flag = false;
				}
			}
			if (c <= 0.35 && flag) {
				this->forecastValue = forecastValues.back();
			}
			else {
				this->forecastValue = 0;
			}

			delete U;
		}
		~GrayForecastModel() = default;
		double SinglePointInterp() {
			return this->forecastValue;
		}
	private:
		double forecastValue;	// 预测值， 本次预测只预测一个值

	private:
		/*
		 * 求和
		 */
		double myAccumulate(std::vector<double>::iterator first, std::vector<double>::iterator last) {
			double sum = 0;
			for (auto iter = first; iter != last; iter++)
			{
				sum += *iter;
			}
			return sum;
		}
		/*
		 * 构建矩阵B
		 */
		Eigen::MatrixXd* createMatrixB(const std::vector<double>& sequence) {
		/*	std::cout << "createMatrixB" << std::endl;
			std::cout << "sequence.size() = " << sequence.size() << std::endl;*/
			Eigen::MatrixXd* matrixXd = new Eigen::MatrixXd(sequence.size() - 1, 2);

			for (size_t i = 0; i < sequence.size() - 1; i++)
			{
				(*matrixXd)(i, 0) = -(sequence[i] + sequence[i + 1]) / 2;
				(*matrixXd)(i, 1) = 1;
			}
			//std::cout << "matrixXd" << std::endl;
			//std::cout << *matrixXd << std::endl;
			return matrixXd;
		}
		/*
		 * 构建矩阵Y
		 */
		Eigen::MatrixXd* createMatrixY(const std::vector<double>& rawData) {

			Eigen::MatrixXd* matrixXd = new Eigen::MatrixXd(rawData.size() - 1, 1);

			for (size_t i = 0; i < rawData.size() - 1; i++)
			{
				(*matrixXd)(i, 0) = rawData[i + 1];
			}
			return matrixXd;
		}

		/*
		 * 生成矩阵U， U = 〖(B^T B)〗^(-1) B^T Y^T
		 */
		Eigen::MatrixXd* getMatrixU(std::vector<double>& rawData) {
			std::vector<double>* sequence = generateAccumulateSeq(rawData);
			//std::cout << "累加序列" << std::endl;
		/*	for (size_t i = 0; i < sequence->size(); i++)
			{
				std::cout << sequence->at(i) << std::endl;
			}*/

			Eigen::MatrixXd* B = createMatrixB(*sequence);
			Eigen::MatrixXd* Y = createMatrixY(rawData);

		/*	std::cout << "B = " << *B << std::endl;
			std::cout << " Y=" << *Y << std::endl;*/


			Eigen::MatrixXd transposeB = B->transpose();
			Eigen::MatrixXd U = (transposeB * *B).inverse() * transposeB * *Y;
			//std::cout << " U =" << U << std::endl;
			Eigen::MatrixXd* res = new Eigen::MatrixXd(2, 1);
			*res << U(0, 0), U(1, 0);
			//std::cout << "a = " << U(0, 0) << " b=" << U(1, 0) << std::endl;
			delete sequence;
			delete B;
			delete Y;
			return res;
		}
		/*
		 * 获取累加序列
		 */
		std::vector<double>* generateAccumulateSeq(std::vector<double>& data) {
			std::vector<double>* sequence = new std::vector<double>;
			sequence->push_back(data.front());

			for (size_t i = 1; i < data.size(); i++)
			{
				double newData = sequence->at(i - 1) + data[i];
				sequence->push_back(newData);
			}
			return sequence;

			//return nullptr;
		}

	};
	/*
	 *  传进来的数值是  12个月的数据，   7月份之前的6个月，7月份之后的6个月，
	 *  先使用前6个月对其正向插补，再使用后6个月的数据逆向插补，最终取两者的平均值
	 */
	double getGrayForecastModel(std::vector<double>& xData, std::vector<double>& yData) {
		std::vector<double> prevData;
		std::vector<double> nextData;  // 后6个月的数据要反向存储

		for (size_t i = 0; i < 6; i++)
		{
			prevData.push_back(yData[i]);
		}

		for (size_t i = xData.size() - 1; i >= 6; i--)
		{
			nextData.push_back(yData[i]);
		}
		GrayForecastModel gfm1(prevData);
		GrayForecastModel gfm2(nextData);

		return (gfm1.SinglePointInterp() + gfm2.SinglePointInterp() ) / 2;
	}
}
#endif // ! GRAYFORECASTMODEL_H


#pragma once

#ifndef  GRAYFORECASTMODEL_H
#define GRAYFORECASTMODEL_H
#include <vector>
#include <Eigen\Dense>
#include <cmath>
#include <numeric>
#include <iostream>
namespace GFMpace {

	// ʹ�û�ɫԤ��ģ�ͽ���Ԥ��
	class GrayForecastModel {
	public:
		GrayForecastModel(std::vector<double>& rawData) {

			Eigen::MatrixXd* U = getMatrixU(rawData);
			double a = (*U)(0, 0);
			double u = (*U)(1, 0);
			int size = rawData.size();
			// Ԥ��ֵ���ۼ�����
			std::vector<double> forecastAccumulation{ rawData.front() };

			for (size_t i = 1; i < size + 1; i++)
			{
				double value = (forecastAccumulation.front() - u / a) * exp(-a * (i)) + u / a;
				forecastAccumulation.push_back(value);
			}

			std::vector<double> forecastValues(forecastAccumulation.size()); // ԭʼ���е�Ԥ��ֵ
			forecastValues[0] = forecastAccumulation[0];

			for (size_t i = 1; i < forecastAccumulation.size(); i++)
			{
				forecastValues[i] = forecastAccumulation[i] - forecastAccumulation[i - 1];
			}
			// Ԥ��ֵ

			// ���ú������м���
			std::vector<double> errors;  // �в�   ��ʵֵ - Ԥ��ֵ
			std::vector<double> relativeErrors;	// ������    �в����ʵ��ֵ
			for (size_t i = 1; i < rawData.size(); i++)
			{
				double err = rawData[i] - forecastValues[i];
				double relativeErr = err / rawData[i];
				errors.push_back(err);
				relativeErrors.push_back(relativeErr);
			}

			double xAverage = this->myAccumulate(rawData.begin(), rawData.end()) / rawData.size(); // ���֮�����Ԫ�صĸ����õ�ƽ��ֵ
			double s1 = 0;   // ����
			for (const auto& d : rawData) {
				s1 += pow(d - xAverage, 2);
			}
			s1 = s1 / rawData.size();

			double eAverage = this->myAccumulate(errors.begin(), errors.end()) / errors.size(); // �в�ľ�ֵ
			double s2 = 0; // �в�ķ���
			for (const auto& d : errors) {
				s2 += pow(d - eAverage, 2);
			}
			s2 = s2 / errors.size();
			// ����C  c = s2 / s1
			double c = s2 / s1;  // ���c С�ڵ���0.35 ��ô��������
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
		double forecastValue;	// Ԥ��ֵ�� ����Ԥ��ֻԤ��һ��ֵ

	private:
		/*
		 * ���
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
		 * ��������B
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
		 * ��������Y
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
		 * ���ɾ���U�� U = ��(B^T B)��^(-1) B^T Y^T
		 */
		Eigen::MatrixXd* getMatrixU(std::vector<double>& rawData) {
			std::vector<double>* sequence = generateAccumulateSeq(rawData);
			//std::cout << "�ۼ�����" << std::endl;
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
		 * ��ȡ�ۼ�����
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
	 *  ����������ֵ��  12���µ����ݣ�   7�·�֮ǰ��6���£�7�·�֮���6���£�
	 *  ��ʹ��ǰ6���¶�������岹����ʹ�ú�6���µ���������岹������ȡ���ߵ�ƽ��ֵ
	 */
	double getGrayForecastModel(std::vector<double>& xData, std::vector<double>& yData) {
		std::vector<double> prevData;
		std::vector<double> nextData;  // ��6���µ�����Ҫ����洢

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


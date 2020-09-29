#include <iostream>
#include <string>
#include "SADataExec.h"
#include <vector>
#include <algorithm>
#include "addtest.h"
#include <omp.h>
#include <Eigen\Dense>
#include <map>
#include <string>
#include <fstream>
#include <ogr_feature.h>
#include "DifferenceHistogram.h"
typedef Eigen::Matrix<int, 3, 3> Matrix3i;
using namespace gdal_viirs_Interpolation;
using namespace std;

// ��ֵ�˲� 
void midFilter() {
#pragma region vcm2019
	//string src = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2019\\";
	//vector<string> srcFiles {"SVDNB_npp_20190501-20190531_75N060E_vcmcfg_v10_c201906130930.avg_rade9h.tif", "SVDNB_npp_20190601-20190630_75N060E_vcmcfg_v10_c201907091100.avg_rade9h.tif", "SVDNB_npp_20190701-20190731_75N060E_vcmcfg_v10_c201908090900.avg_rade9h.tif"};
	//string dst = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2019\\filter\\";
	//vector<string> dstFiles{ "SVDNB_npp_20190501-20190531_75N060E_vcmcfg_v10_c201906130930.avg_rade9h_filter.tif", "SVDNB_npp_20190601-20190630_75N060E_vcmcfg_v10_c201907091100.avg_rade9h_filter.tif", "SVDNB_npp_20190701-20190731_75N060E_vcmcfg_v10_c201908090900.avg_rade9h_filter.tif" };

	//for (size_t i = 0; i < srcFiles.size(); i++)
	//{
	//	SADataExec::viirs_Denoising_MedianFlitering_5P5((src+srcFiles[i]).c_str(), (dst + dstFiles[i]).c_str(), 285);
	//}
#pragma endregion

#pragma region vcm_2018_75N180W
	//string src = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018_75N180W\\";
	//vector<string> srcNames {"SVDNB_npp_20180401-20180430_75N180W_vcmcfg_v10_c201805021400.avg_rade9h.tif",
	//	"SVDNB_npp_20180501-20180531_75N180W_vcmcfg_v10_c201806061100.avg_rade9h.tif", 
	//	"SVDNB_npp_20180601-20180630_75N180W_vcmcfg_v10_c201904251200.avg_rade9h.tif", 
	//	"SVDNB_npp_20180701-20180731_75N180W_vcmcfg_v10_c201812111300.avg_rade9h.tif",
	//"SVDNB_npp_20180801-20180831_75N180W_vcmcfg_v10_c201809070900.avg_rade9h.tif"};
	//string dst = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018_75N180W\\filter\\";
	//for (size_t i = 0; i < srcNames.size(); i++)
	//{
	//	SADataExec::viirs_Denoising_MedianFlitering_5P5((src+ srcNames[i]).c_str(), (dst + srcNames[i]).c_str(), 397.15);
	//}


#pragma endregion
#pragma region vcm_2018_75N60W
	string src = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018_75N60W\\";
	vector<string> srcNames{ /*"SVDNB_npp_20180401-20180430_75N060W_vcmcfg_v10_c201805021400.avg_rade9h.tif",*/
		"SVDNB_npp_20180501-20180531_75N060W_vcmcfg_v10_c201806061100.avg_rade9h.tif",
		"SVDNB_npp_20180601-20180630_75N060W_vcmcfg_v10_c201904251200.avg_rade9h.tif",
		"SVDNB_npp_20180701-20180731_75N060W_vcmcfg_v10_c201812111300.avg_rade9h.tif",
	"SVDNB_npp_20180801-20180831_75N060W_vcmcfg_v10_c201809070900.avg_rade9h.tif" };
	string dst = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018_75N60W\\filter\\";
	for (size_t i = 0; i < srcNames.size(); i++)
	{
		SADataExec::viirs_Denoising_MedianFlitering_5P5((src + srcNames[i]).c_str(), (dst + srcNames[i]).c_str(), 397.15);
	}
#pragma endregion
}
// ֱ��ͼͳ��
void histogram() {
	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\"};
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Difference_0719\\";

	// vcm 201807�·�  ����  ·�����
	vector<string> strNames{ "haozhou.tif", "fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif" };
	string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Difference_0719\\Interpolation_results_of_8_methods_in_each_city";

	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ͳ�Ʊ��\\6������8�ַ�������ּ����ֱ��ͼ����ͳ��_0720.csv", std::ios::app);
	if (f.is_open()) {
		f << "6������8�ֲ岹��������ֵ�ּ��Ƚ�" << endl;
	}
	// �岹���
	//vector<double> gaps = { 0.36, 1, 1.92, 5.03, 9.7, 14.37, 20.59, 26.81, 33.04, 42.38 };//�Ϻ�
//	vector<double> gaps = {0.33, 0.5, 1.08, 1.83, 4.09, 7.84, 13.85, 19.86, 27.37, 43.89};// COllin
	//vector<double> gaps = {0.31, 0.56, 0.68, 0.93, 1.45, 2.5, 4.67, 9.11, 18.25, 37}; // fuyang
	// ����ֵ�ּ�չʾ
	//vector<double> gaps{0, 0.54, 1.9, 3.67, 5.7, 8.01, 11, 14.26, 18.74, 24.3}; // �����븷��
	vector<double> gaps{0, 2, 10, 30};// Collin and El paos
	//vector<double> gaps{0, 1.3, 7.8, 16.9, 31.2, 52, 91, 147, 205.5, 257.5};// Shanghai
	for (size_t i = 0; i < regions.size(); i++)
	{
		f << regions[i] << endl;

		map<string, vector<Real_Simulate>> map;
		int length = 0;
		for (size_t j = 0; j < methods.size(); j++)
		{
			f << methods[j] << endl;
			string simulatePath = filePath + regions[i]/*+ "noexception\\final_" */ + methods[j];
			int validPixelCount = 0;
			//string realPath = srcPath + strNames[i];
			//vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(), simulatePath.c_str());
			vector<DifferenceHistogram>* hists = SADataExec::viirs_histogram(simulatePath.c_str(), gaps, &validPixelCount);
			
			for (auto iter = hists->begin(); iter != hists->end(); ++iter) {
				auto tmp = iter + 1;
				double right = 1000;
				if (tmp != hists->end()) {
					right = tmp->startValue;
				}

				f << "[" << iter->startValue << "��" << right << ")," << 1.0 * iter->pixelCount / validPixelCount
					<<"," << validPixelCount << endl;
			}
		}

	}
	f.close();


#pragma region ����������֮����Ϻ��е�8�ַ�����ֱ��ͼ
	//vector<string> strNames{ "BezierCurve_Shanghai.tif", "CESmoothing_Shanghai.tif", "GFM_Shanghai.tif", "LSM_Curve_Shanghai.tif", "LSM_Curve_Three_Shanghai.tif",
	//"LSM_Shanghai.tif", "Spline3_Shanghai.tif", "Hermite_right_Shanghai.tif" };
	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\noexception397_2\\";
	////vector<int> gaps{ 0,2,5,10, 30, 100 };
	//vector<double> gaps;
	//for (int i = 0; i <= 500; i = i+1) {
	//	gaps.push_back(i);
	//}
	//// ��ֱ��ͼ����д�뵽csv��
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\�岹�����ֱ��ͼ����_�Ϻ�_2.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "�Ϻ���8�ֲ岹��ʽ�Ĳ���ּ��Ƚ�" << endl;
	//	f << "��ֵ����_ǰ��0.5" << "," << "��������Ԫ������, ��������Ԫ�ľ�ֵ" << endl;
	//}
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	int validPixelCount = 0;
	//	vector<DifferenceHistogram>* hists = SADataExec::viirs_histogram((filePath + strNames[i]).c_str(), gaps, &validPixelCount);

	//	double totalPixelValue = 0;
	//	f << strNames[i] << endl;
	//	for (auto iter = hists->begin(); iter != hists->end(); ++iter) {
	//		f << iter->startValue << "," << 1.0 * iter->pixelCount << "," 
	//			<<( (iter->pixelCount == 0)?0: iter->totalValue / iter->pixelCount )<< endl;
	//		totalPixelValue += iter->totalValue;
	//	}
	//	f << "ƽ������ֵΪ " << totalPixelValue / validPixelCount << endl;
	//}

	//f.close();
#pragma endregion

#pragma region ԭʼvcm7�·�ֱ��ͼ���� �Ϻ� �人 פ���
	//vector<string> strNames{ "Shanghai.tif"/*, "Wuhan.tif", "Zhumadian.tif"*/ };

	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\";
	//vector<double> gaps { 0, 1, 2,3,4, 5,6,7,8,9, 10,12,14,16,18, 20 ,24, 28, 40, 60, 100};
	////for (int i = 0; i < 300; i++) {
	////	gaps.push_back(i);
	////}
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ԭʼvcm7�·�ֱ��ͼ����_0710.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "��ֵ_��ǰȡ��" << "," << "��������Ԫ������" << endl;
	//}
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	int validPixelCount = 0;
	//	vector<DifferenceHistogram>* hists = SADataExec::viirs_histogram((filePath + strNames[i]).c_str(), gaps, &validPixelCount);

	//	f << strNames[i] << endl;
	//	for (auto iter = hists->begin(); iter != hists->end(); ++iter) {
	//		auto tmp = iter+1;
	//		double right = 1000;
	//		if (tmp != hists->end()) {
	//			right = tmp->startValue;
	//		}

	//		f << "[" << iter->startValue << "��"<< right << ")," << iter->pixelCount  << endl;
	//	}
	//}

	//f.close();
#pragma endregion

#pragma region ����������֮����人 �е�8�ַ�����ֱ��ͼ
	//vector<string> strNames{ "BezierCurve_Wuhan.tif", "CESmoothing_Wuhan.tif", "GFM_Wuhan.tif", 
	//	"LSM_Curve_Wuhan.tif", "LSM_Curve_Three_Wuhan.tif",
	//	"LSM_Wuhan.tif", "Spline3_Wuhan.tif", "Hermite_right_Wuhan.tif" };
	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\\Wuhan\\DifferenceAbsolute\\";
	////vector<int> gaps{ 0,1,2,5,10, 30 };
	//vector<double> gaps;
	//for (int i = 0; i < 20; i++) {
	//	gaps.push_back(i);
	//}
	//// ��ֱ��ͼ����д�뵽csv��
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\�岹�����ֱ��ͼ����_�人.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "�人����8�ֲ岹��ʽ�Ĳ���ּ��Ƚ�" << endl;
	//	f << "��ֵ����_ǰ�պ�" << "," << "��������Ԫ������, ��������Ԫ�ľ�ֵ" << endl;
	//}
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	int validPixelCount = 0;
	//	vector<DifferenceHistogram>* hists = SADataExec::viirs_histogram((filePath + strNames[i]).c_str(), gaps, &validPixelCount);

	//	double totalPixelValue = 0;
	//	f << strNames[i] << endl;
	//	for (auto iter = hists->begin(); iter != hists->end(); ++iter) {
	//		f << iter->startValue << "," << iter->pixelCount << ","
	//			<< 1.0 * iter->totalValue / iter->pixelCount << endl;
	//		totalPixelValue += iter->totalValue;
	//	}
	//	f << "ƽ������ֵΪ, " << totalPixelValue / validPixelCount << endl;
	//}

	//f.close();
#pragma endregion

#pragma region ��������֮���פ��� �е�8�ַ�����ֱ��ͼ
	//vector<string> strNames{ "BezierCurve_zhumadian_mid.tif", "CESmoothing_zhumadian_mid.tif", "GFM_zhumadian_mid.tif",
	//	"LSM_Curve_zhumadian_mid.tif", "LSM_Curve_Three_zhumadian_mid.tif",
	//	"LSM_zhumadian_mid.tif", "Spline3_zhumadian_mid.tif", "Hermite_right_zhumadian_mid.tif" };
	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\\zhumadian\\";
	////vector<double> gaps{ 0,0.5,5,10,20 };
	//vector<double> gaps;
	//for (int i = 0; i < 50; i++) {
	//	gaps.push_back(i);
	//}
	//// ��ֱ��ͼ����д�뵽csv��
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\�岹�����ֱ��ͼ����_פ���.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "פ�����8�ֲ岹��ʽ�Ĳ���ּ��Ƚ�" << endl;
	//	f << "��ֵ,ǰ��0.5��������" << "," << "��������Ԫ������, ��������Ԫ�ľ�ֵ" << endl;
	//}
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	int validPixelCount = 0;
	//	vector<DifferenceHistogram>* hists = SADataExec::viirs_histogram((filePath + strNames[i]).c_str(), gaps, &validPixelCount);

	//	double totalPixelValue = 0;
	//	f << strNames[i] << endl;
	//	for (auto iter = hists->begin(); iter != hists->end(); ++iter) {
	//		f << iter->startValue << "," << 1.0 * iter->pixelCount  << ","
	//			<< iter->totalValue / iter->pixelCount << endl;
	//		totalPixelValue += iter->totalValue;
	//	}
	//	f << "ƽ������ֵΪ, " << totalPixelValue / validPixelCount << endl;
	//}

	//f.close();
#pragma endregion
}
//ɢ��ͼ
void scatterDiagram() {
#pragma region ʱ������
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ͳ�Ʊ��\\ʱ������Ԥ��ɢ��ͼͳ��_0923_3.csv", std::ios::app);
	if (f.is_open()) {
		
	}
	vector<string> months{ "07", "08", "09", "10", "11", "12" };
	for (size_t i = 0; i < months.size(); i++)
	{
		string realPath = "F:\\04-dissertation\\02-VIIRS\\�õ�����ش���\\ʱ������\\city_world_raster\\Shanghai\\2018" + months[i]+".tif";
		string simulatePath = "F:\\04-dissertation\\02-VIIRS\\�õ�����ش���\\ʱ������\\city_world_raster\\ts_result\\2018" + months[i] + "_Shanghai_95.tif";
		vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(), simulatePath.c_str());
		f << "2018" + months[i] + "_Shanghai_95" << endl;
		f << "���," << "vcm" << "," << "ts" << endl;
		int index = 1;
		for (auto p : *hists) {
			f << index++ << ",";
			f << p.real << ",";
			f << p.simulate << endl;

		}
	}

	vector<int> v;
	bool flog = false;
	f.close();
#pragma endregion

#pragma region ����ȱʧ�������� 4������ ��vcmsl�ĶԱ�
//	//�岹������·��
//	vector<string> methods{ "bezier","CESmoothing", "GFM", "Hermite", "LSM",
//
//"LSM_Curve", "LSM_Curve_Three", "Spline3" };
//	vector<string> regions{ "֣����.tif","������.tif", "������.tif", "������.tif" };
//	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\201806_high_latitude_cities_simulation\\";
//
//	// vcm 201806�·�  ����  ·�����
//	//vector<string> strNames{"Bozhou.tif", "Fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif"};
//	//string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\vcmsl\\";
//
//	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ͳ�Ʊ��\\4������8�ַ���vcmslɢ��ͼ����ͳ��_0723.csv", std::ios::app);
//	if (f.is_open()) {
//		f << "4������8�ֲ岹�����岹�����vcmsl���ݱȽ�" << endl;
//	}
//
//	for (size_t i = 0; i < regions.size(); i++)
//	{
//		f << regions[i] << endl;
//
//		map<string, vector<Real_Simulate>> map;
//		int length = 0;
//		for (size_t j = 0; j < methods.size(); j++)
//		{
//
//			string simulatePath = filePath + methods[j] + "\\" + regions[i];
//			string realPath = filePath + "VCMSL\\" + regions[i];
//			vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(), simulatePath.c_str());
//
//			map.insert(pair<string, vector<Real_Simulate>>(methods[j], *hists));
//			length = hists->size();
//		}
//		// �������
//		f << endl;
//		f << "���,vcmsl" << ",";
//		for (auto p : map) {
//			f << p.first << ",";
//		}
//		f << endl;
//		// �����������
//		for (size_t i = 0; i < length; i++)
//		{
//			f << i << ",";
//			bool first = true;
//			for (auto p : map) {
//				if (first) {
//					if (p.second[i].real < 0) {
//						f << 0 << ",";
//						first = false;
//					}else{
//						f << p.second[i].real << ",";
//						first = false;
//					}
//			
//				}
//				if (p.second[i].simulate < 0) {
//					f << 0 << ",";
//				}
//				else {
//					f << p.second[i].simulate << ",";
//				}
//			
//			}
//			f << endl;
//		}
//
//	}
//	f.close();

#pragma endregion

#pragma region 6������vcm  ��vcmsl�ĶԱ�
	///*vector<string> strNames{ "Bozhou.tif", "Fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif" };
	//string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\";

	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ͳ�Ʊ��\\6������_vcm_vcmsl_ɢ��ͼ����ͳ��_0721.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "6������ vcm�� vcmsl �Ƚ�" << endl;
	//}

	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	string vcm = srcPath + strNames[i];
	//	string vcmsl = srcPath + "vcmsl\\" + strNames[i];
	//	vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(vcm.c_str(), vcmsl.c_str());

	//	f << strNames[i] << endl;
	//	f << "���,vcm,vcmsl" << endl;
	//	for (size_t j = 0; j < hists->size(); j++)
	//	{
	//		f << j + 1 << "," << hists->at(j).real << "," << hists->at(j).simulate << endl;
	//	}
	//}
	//f.close();*/
#pragma endregion

#pragma region   ȫ����6�������� vcmsl�Ƚ�
	  //�岹������·��
//	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
//"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
//	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
//	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city_0719\\";
//
//	// vcm 201807�·�  ����  ·�����
//	vector<string> strNames{"Bozhou.tif", "Fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif"};
//	string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\vcmsl\\";
//
//	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ͳ�Ʊ��\\6������8�ַ���vcmslɢ��ͼ����ͳ��_0721.csv", std::ios::app);
//	if (f.is_open()) {
//		f << "6������8�ֲ岹�����岹�����vcmsl���ݱȽ�" << endl;
//	}
//
//	for (size_t i = 0; i < regions.size(); i++)
//	{
//		f << regions[i] << endl;
//
//		map<string, vector<Real_Simulate>> map;
//		int length = 0;
//		for (size_t j = 0; j < methods.size(); j++)
//		{
//
//			string simulatePath = filePath + regions[i] + "noexception\\final_" + methods[j];
//			string realPath = srcPath + strNames[i];
//			vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(), simulatePath.c_str());
//
//			map.insert(pair<string, vector<Real_Simulate>>(methods[j], *hists));
//			length = hists->size();
//		}
//		// �������
//		f << endl;
//		f << "���,vcmsl" << ",";
//		for (auto p : map) {
//			f << p.first << ",";
//		}
//		f << endl;
//		// �����������
//		for (size_t i = 0; i < length; i++)
//		{
//			f << i << ",";
//			bool first = true;
//			for (auto p : map) {
//				if (first) {
//					f << p.second[i].real << ",";
//					first = false;
//				}
//
//				f << p.second[i].simulate << ",";
//			}
//			f << endl;
//		}
//
//	}
//	f.close();
#pragma endregion
#pragma region   ȫ����6�������� vcm�Ƚ�
	 //  �岹������·��
//	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
//"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
//	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
//	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city\\";
//
//	// vcm 201807�·�  ����  ·�����
//	vector<string> strNames{"haozhou.tif", "fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif"};
//	string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\";
//
//	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ͳ�Ʊ��\\6������8�ַ���ɢ��ͼ����ͳ��_0712.csv", std::ios::app);
//	if (f.is_open()) {
//		f << "6������8�ֲ岹�����岹�������ʵ���ݱȽ�" << endl;
//	}
//
//	for (size_t i = 0; i < regions.size(); i++)
//	{
//		f << regions[i] << endl;
//
//		map<string, vector<Real_Simulate>> map;
//		int length = 0;
//		for (size_t j = 0; j < methods.size(); j++)
//		{
//
//			string simulatePath = filePath + regions[i] + "noexception\\test_" + methods[j];
//			string realPath = srcPath + strNames[i];
//			vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(), simulatePath.c_str());
//
//			map.insert(pair<string, vector<Real_Simulate>>(methods[j], *hists));
//			length = hists->size();
//		}
//		// �������
//		f << endl;
//		f << "���,vcm" << ",";
//		for (auto p : map) {
//			f << p.first << ",";
//		}
//		f << endl;
//		// �����������
//		for (size_t i = 0; i < length; i++)
//		{
//			f << i << ",";
//			bool first = true;
//			for (auto p : map) {
//				if (first) {
//					f << p.second[i].real << ",";
//					first = false;
//				}
//
//				f << p.second[i].simulate << ",";
//			}
//			f << endl;
//		}
//
//	}
//	f.close();
#pragma endregion
#pragma region �Ϻ�
	/*vector<string> strNames{ "BezierCurve_Shanghai.tif", "CESmoothing_Shanghai.tif", "GFM_Shanghai.tif", "LSM_Curve_Shanghai.tif", "LSM_Curve_Three_Shanghai.tif",
"LSM_Shanghai.tif", "Spline3_Shanghai.tif", "Hermite_right_Shanghai.tif" };
	string simulatePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\noexception397_2\\";

	string realPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\Shanghai.tif";
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ȥ���쳣�Ĳ岹�����ɢ��ͼ����_�Ϻ�.csv", std::ios::app);
	if (f.is_open()) {
		f << "�Ϻ���8�ֲ岹�����岹�������ʵ���ݱȽ�" << endl;
		f << "���" << "��ʵ����" << "," << "�岹����" << endl;
	}
	for (size_t i = 0; i < strNames.size(); i++)
	{
		vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(),(simulatePath + strNames[i]).c_str() );

		f << strNames[i] << endl;
		int index = 1;
		for (auto iter = hists->begin(); iter != hists->end(); ++iter) {
			f << (index++) << "," << iter->real << "," << iter->simulate << endl;
		}
	}

	f.close();*/
#pragma endregion

#pragma region �人��
	//vector<string> strNames{ /*"BezierCurve_Wuhan.tif", "CESmoothing_Wuhan.tif", "GFM_Wuhan.tif", 
	//	"LSM_Curve_Wuhan.tif", "LSM_Curve_Three_Wuhan.tif",
	//	"LSM_Wuhan.tif", "Spline3_Wuhan.tif",*/ "Hermite_right_Wuhan.tif" };
	////vector<string> strNames{  "LSM_Wuhan.tif" };
	//string simulatePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Wuhan\\noexception\\";

	//string realPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\noexception\\Wuhan.tif";
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ȥ���쳣�Ĳ岹�����ɢ��ͼ����_�人3.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "�人��8�ֲ岹�����岹�������ʵ���ݱȽ�" << endl;
	//	f << "���," << "��ʵ����" << "," << "�岹����" << endl;
	//}
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(), (simulatePath + strNames[i]).c_str());

	//	f << strNames[i] << endl;
	//	int index = 1;
	//	for (auto iter = hists->begin(); iter != hists->end(); ++iter) {
	//		f << (index++) << "," << iter->real << "," << iter->simulate << endl;
	//	}
	//}

	//f.close();
#pragma endregion
#pragma region פ�����
	//vector<string> strNames{ /*"BezierCurve_zhumadian_mid.tif", "CESmoothing_zhumadian_mid.tif", "GFM_zhumadian_mid.tif",
	//	"LSM_Curve_zhumadian_mid.tif", "LSM_Curve_Three_zhumadian_mid.tif",
	//	"LSM_zhumadian_mid.tif", "Spline3_zhumadian_mid.tif",*/ "Hermite_right_zhumadian_mid.tif" };
	//string simulatePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\zhumadian\\noexception2\\";

	//string realPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\Zhumadian.tif";
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ȥ���쳣�Ĳ岹�����ɢ��ͼ����_פ���_2.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "פ�����8�ֲ岹�����岹�������ʵ���ݱȽ�" << endl;
	//	f << "���" << "��ʵ����" << "," << "�岹����" << endl;
	//}
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(), (simulatePath + strNames[i]).c_str());

	//	f << strNames[i] << endl;
	//	int index = 1;
	//	for (auto iter = hists->begin(); iter != hists->end(); ++iter) {
	//		f << (index++) << "," << iter->real << "," << iter->simulate << endl;
	//	}
	//}

	//f.close();
#pragma endregion
}

// ƽ���������ֵ
void averageDiffValue() {
	//  �岹������·��
	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city\\";

	// vcm 201807�·�  ����  ·�����
	vector<string> strNames{ "haozhou.tif", "fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif" };
	string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\";

	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ͳ�Ʊ��\\6������8�ַ���ƽ���������ֵͳ��_0714.csv", std::ios::app);
	if (f.is_open()) {
		f << "6������8�ֲ岹�����岹�������ʵ���ݱȽ�" << endl;
	}

	for (size_t i = 0; i < regions.size(); i++)
	{
		f << regions[i] << endl;

		map<string, double> map;
		int pixelCount = 0;
		for (size_t j = 0; j < methods.size(); j++)
		{

			string simulatePath = filePath + regions[i] + "noexception\\test_" + methods[j];
			string realPath = srcPath + strNames[i];
			vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(), simulatePath.c_str());

			pixelCount = RegionDataHanle::totalCount(realPath.c_str());
			double diff = 0;
			for (auto m : *hists)
			{
				diff += abs(m.real - m.simulate);
			}
			diff = diff / pixelCount;
			map.insert(pair<string, double>(methods[j], diff));

		}
		f << "��Ԫ����Ϊ," << pixelCount << endl;
		// �������
		for (auto p : map) {
			f << p.first << ",";
		}
		f << endl;
		for (auto p : map) {
			f << p.second << ",";
		}
		f << endl;


	}
	f.close();
}
// �岹
void interpolation_75N60E() {
#pragma region ��7�·ݽ��в岹

	//int rslt = -1;
	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\";
	////6.VIIRS vcm�¶�����-ʱ�����в岹
	//string srsFile1 = filePath + "SVDNB_npp_20180101-20180131_75N060E_vcmcfg_v10_c201805221252.avg_rade9h.tif";
	//string srsFile2 = filePath + "SVDNB_npp_20180201-20180228_75N060E_vcmcfg_v10_c201803012000.avg_rade9h.tif";
	//string srsFile3 = filePath + "SVDNB_npp_20180301-20180331_75N060E_vcmcfg_v10_c201804022005.avg_rade9h.tif";
	//string srsFile4 = filePath + "SVDNB_npp_20180401-20180430_75N060E_vcmcfg_v10_c201805021400.avg_rade9h.tif";
	//string srsFile5 = filePath + "SVDNB_npp_20180501-20180531_75N060E_vcmcfg_v10_c201806061100.avg_rade9h.tif";
	//string srsFile6 = filePath + "SVDNB_npp_20180601-20180630_75N060E_vcmcfg_v10_c201904251200.avg_rade9h.tif";

	//string srsFile7 = filePath + "SVDNB_npp_20180701-20180731_75N060E_vcmcfg_v10_c201812111300.avg_rade9h.tif";

	//string srsFile8 = filePath + "SVDNB_npp_20180801-20180831_75N060E_vcmcfg_v10_c201809070900.avg_rade9h.tif";
	//string srsFile9 = filePath + "SVDNB_npp_20180901-20180930_75N060E_vcmcfg_v10_c201810250900.avg_rade9h.tif";
	//string srsFile10 = filePath + "SVDNB_npp_20181001-20181031_75N060E_vcmcfg_v10_c201811131000.avg_rade9h.tif";
	//string srsFile11 = filePath + "SVDNB_npp_20181101-20181130_75N060E_vcmcfg_v10_c201812081230.avg_rade9h.tif";
	//string srsFile12 = filePath + "SVDNB_npp_20181201-20181231_75N060E_vcmcfg_v10_c201902122100.avg_rade9h.tif";
	//string srsFile13 = filePath + "SVDNB_npp_20190101-20190131_75N060E_vcmcfg_v10_c201905201300.avg_rade9h.tif";



	//string dstFile1 = filePath + "dstInterpolation\\LSM.tif";
	//rslt = SADataExec::viirs_Interpolation_LSM(srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile1.c_str());

	//string dstFile2 = filePath + "dstInterpolation\\LSM_Curve_test_debug.tif";
	//rslt = SADataExec::viirs_Interpolation_LSM_Curve(srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile2.c_str());

	//string dstFile3 = filePath + "dstInterpolation\\LSM_Curve_Three_test_debug.tif";
	//rslt = SADataExec::viirs_Interpolation_LSM_Curve_Three(srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile3.c_str());

	/*string dstFile4 = filePath + "dstInterpolation\\Spline3.tif";
	rslt = SADataExec::viirs_Interpolation_Spline3(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile4.c_str());

	string dstFile5 = filePath + "dstInterpolation\\BezierCurve.tif";
	rslt = SADataExec::viirs_Interpolation_BezierCurve(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile5.c_str());


	string dstFile7 = filePath + "dstInterpolation\\CESmoothing.tif";
	rslt = SADataExec::viirs_Interpolation_CES(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile7.c_str());

	string dstFile8 = filePath + "dstInterpolation\\GFM.tif";
	rslt = SADataExec::viirs_Interpolation_GFM(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile8.c_str());*/
	//string dstFile6 = filePath + "dstInterpolation\\Hermite_right_debug.tif";
	//rslt = SADataExec::viirs_Interpolation_Hermite(srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile6.c_str());

#pragma endregion 
	int rslt = -1;
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\";
	//6.VIIRS vcm�¶�����-ʱ�����в岹
	string srsFile0 = filePath + "SVDNB_npp_20171201-20171231_75N060E_vcmcfg_v10_c201801021747.cf_cvg.tif";
	string srsFile1 = filePath + "SVDNB_npp_20180101-20180131_75N060E_vcmcfg_v10_c201805221252.avg_rade9h.tif";
	string srsFile2 = filePath + "SVDNB_npp_20180201-20180228_75N060E_vcmcfg_v10_c201803012000.avg_rade9h.tif";
	string srsFile3 = filePath + "SVDNB_npp_20180301-20180331_75N060E_vcmcfg_v10_c201804022005.avg_rade9h.tif";
	string srsFile4 = filePath + "SVDNB_npp_20180401-20180430_75N060E_vcmcfg_v10_c201805021400.avg_rade9h.tif";
	string srsFile5 = filePath + "SVDNB_npp_20180501-20180531_75N060E_vcmcfg_v10_c201806061100.avg_rade9h.tif";

	string srsFile6 = filePath + "SVDNB_npp_20180601-20180630_75N060E_vcmcfg_v10_c201904251200.avg_rade9h.tif";

	string srsFile7 = filePath + "SVDNB_npp_20180701-20180731_75N060E_vcmcfg_v10_c201812111300.avg_rade9h.tif";
	string srsFile8 = filePath + "SVDNB_npp_20180801-20180831_75N060E_vcmcfg_v10_c201809070900.avg_rade9h.tif";
	string srsFile9 = filePath + "SVDNB_npp_20180901-20180930_75N060E_vcmcfg_v10_c201810250900.avg_rade9h.tif";
	string srsFile10 = filePath + "SVDNB_npp_20181001-20181031_75N060E_vcmcfg_v10_c201811131000.avg_rade9h.tif";
	string srsFile11 = filePath + "SVDNB_npp_20181101-20181130_75N060E_vcmcfg_v10_c201812081230.avg_rade9h.tif";
	string srsFile12 = filePath + "SVDNB_npp_20181201-20181231_75N060E_vcmcfg_v10_c201902122100.avg_rade9h.tif";

	string dstPath = filePath + "dstInterpolation_0721_6\\";

	/*string dstFile1 = dstPath + "LSM.tif";
	rslt = SADataExec::viirs_Interpolation_LSM(srsFile0.c_str(), srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(),  dstFile1.c_str());

	string dstFile2 = dstPath + "LSM_Curve.tif";
	rslt = SADataExec::viirs_Interpolation_LSM_Curve(srsFile0.c_str(), srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), dstFile2.c_str());

	string dstFile3 = dstPath + "LSM_Curve_Three.tif";
	rslt = SADataExec::viirs_Interpolation_LSM_Curve_Three(srsFile0.c_str(), srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(),  dstFile3.c_str());*/

	//string dstFile4 = dstPath + "Spline3.tif";
	//rslt = SADataExec::viirs_Interpolation_Spline3(srsFile0.c_str(), srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(),  dstFile4.c_str());

	//string dstFile5 = dstPath + "BezierCurve.tif";
	//rslt = SADataExec::viirs_Interpolation_BezierCurve(srsFile0.c_str(), srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(),  dstFile5.c_str());


	//string dstFile7 = dstPath + "CESmoothing.tif";
	//rslt = SADataExec::viirs_Interpolation_CES(srsFile0.c_str(), srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), dstFile7.c_str());

	//string dstFile8 = dstPath + "GFM.tif";
	//rslt = SADataExec::viirs_Interpolation_GFM(srsFile0.c_str(), srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), dstFile8.c_str());
	string dstFile6 = dstPath + "Hermite2.tif";
	rslt = SADataExec::viirs_Interpolation_Hermite(srsFile0.c_str(), srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(),  dstFile6.c_str());

#pragma region 
}

void interpolation_75N180W() {
	int rslt = -1;
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018_75N180W\\";
	//6.VIIRS vcm�¶�����-ʱ�����в岹
	string srsFile1 = filePath + "SVDNB_npp_20180101-20180131_75N180W_vcmcfg_v10_c201805221252.avg_rade9h.tif";
	string srsFile2 = filePath + "SVDNB_npp_20180201-20180228_75N180W_vcmcfg_v10_c201803012000.avg_rade9h.tif";
	string srsFile3 = filePath + "SVDNB_npp_20180301-20180331_75N180W_vcmcfg_v10_c201804022005.avg_rade9h.tif";
	string srsFile4 = filePath + "SVDNB_npp_20180401-20180430_75N180W_vcmcfg_v10_c201805021400.avg_rade9h.tif";
	string srsFile5 = filePath + "SVDNB_npp_20180501-20180531_75N180W_vcmcfg_v10_c201806061100.avg_rade9h.tif";
	string srsFile6 = filePath + "SVDNB_npp_20180601-20180630_75N180W_vcmcfg_v10_c201904251200.avg_rade9h.tif";

	string srsFile7 = filePath + "SVDNB_npp_20180701-20180731_75N180W_vcmcfg_v10_c201812111300.avg_rade9h.tif";

	string srsFile8 = filePath + "SVDNB_npp_20180801-20180831_75N180W_vcmcfg_v10_c201809070900.avg_rade9h.tif";
	string srsFile9 = filePath + "SVDNB_npp_20180901-20180930_75N180W_vcmcfg_v10_c201810250900.avg_rade9h.tif";
	string srsFile10 = filePath + "SVDNB_npp_20181001-20181031_75N180W_vcmcfg_v10_c201811131000.avg_rade9h.tif";
	string srsFile11 = filePath + "SVDNB_npp_20181101-20181130_75N180W_vcmcfg_v10_c201812081230.avg_rade9h.tif";
	string srsFile12 = filePath + "SVDNB_npp_20181201-20181231_75N180W_vcmcfg_v10_c201902122100.avg_rade9h.tif";
	string srsFile13 = filePath + "SVDNB_npp_20190101-20190131_75N180W_vcmcfg_v10_c201905201300.avg_rade9h.tif";

	string dstFile1 = filePath + "dstInterpolation_7-9\\LSM.tif";
	rslt = SADataExec::viirs_Interpolation_LSM(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile1.c_str());

	string dstFile2 = filePath + "dstInterpolation_7-9\\LSM_Curve.tif";
	rslt = SADataExec::viirs_Interpolation_LSM_Curve(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile2.c_str());

	string dstFile3 = filePath + "dstInterpolation_7-9\\LSM_Curve_Three.tif";
	rslt = SADataExec::viirs_Interpolation_LSM_Curve(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile3.c_str());

	string dstFile4 = filePath + "dstInterpolation_7-9\\Spline3.tif";
	rslt = SADataExec::viirs_Interpolation_Spline3(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile4.c_str());

	string dstFile5 = filePath + "dstInterpolation_7-9\\BezierCurve.tif";
	rslt = SADataExec::viirs_Interpolation_BezierCurve(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile5.c_str());


	string dstFile7 = filePath + "dstInterpolation_7-9\\CESmoothing.tif";
	rslt = SADataExec::viirs_Interpolation_CES(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile7.c_str());

	string dstFile8 = filePath + "dstInterpolation_7-9\\GFM.tif";
	rslt = SADataExec::viirs_Interpolation_GFM(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile8.c_str());
	string dstFile6 = filePath + "dstInterpolation_7-9\\Hermite.tif";
	rslt = SADataExec::viirs_Interpolation_Hermite(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile6.c_str());
}

void interpolation_75N60W() {
	int rslt = -1;
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018_75N60W\\";
	//6.VIIRS vcm�¶�����-ʱ�����в岹
	string srsFile1 = filePath + "SVDNB_npp_20180101-20180131_75N060W_vcmcfg_v10_c201805221252.avg_rade9h.tif";
	string srsFile2 = filePath + "SVDNB_npp_20180201-20180228_75N060W_vcmcfg_v10_c201803012000.avg_rade9h.tif";
	string srsFile3 = filePath + "SVDNB_npp_20180301-20180331_75N060W_vcmcfg_v10_c201804022005.avg_rade9h.tif";
	string srsFile4 = filePath + "SVDNB_npp_20180401-20180430_75N060W_vcmcfg_v10_c201805021400.avg_rade9h.tif";
	string srsFile5 = filePath + "SVDNB_npp_20180501-20180531_75N060W_vcmcfg_v10_c201806061100.avg_rade9h.tif";
	string srsFile6 = filePath + "SVDNB_npp_20180601-20180630_75N060W_vcmcfg_v10_c201904251200.avg_rade9h.tif";

	string srsFile7 = filePath + "SVDNB_npp_20180701-20180731_75N060W_vcmcfg_v10_c201812111300.avg_rade9h.tif";

	string srsFile8 = filePath + "SVDNB_npp_20180801-20180831_75N060W_vcmcfg_v10_c201809070900.avg_rade9h.tif";
	string srsFile9 = filePath + "SVDNB_npp_20180901-20180930_75N060W_vcmcfg_v10_c201810250900.avg_rade9h.tif";
	string srsFile10 = filePath + "SVDNB_npp_20181001-20181031_75N060W_vcmcfg_v10_c201811131000.avg_rade9h.tif";
	string srsFile11 = filePath + "SVDNB_npp_20181101-20181130_75N060W_vcmcfg_v10_c201812081230.avg_rade9h.tif";
	string srsFile12 = filePath + "SVDNB_npp_20181201-20181231_75N060W_vcmcfg_v10_c201902122100.avg_rade9h.tif";
	string srsFile13 = filePath + "SVDNB_npp_20190101-20190131_75N060W_vcmcfg_v10_c201905201300.avg_rade9h.tif";

	string dstFile1 = filePath + "dstInterpolation_7-7\\LSM.tif";
	rslt = SADataExec::viirs_Interpolation_LSM(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile1.c_str());

	//string dstFile2 = filePath + "dstInterpolation_7-7\\LSM_Curve.tif";
	//rslt = SADataExec::viirs_Interpolation_LSM_Curve(srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile2.c_str());

	//string dstFile3 = filePath + "dstInterpolation_7-7\\LSM_Curve_Three.tif";
	//rslt = SADataExec::viirs_Interpolation_LSM_Curve(srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile3.c_str());

	//string dstFile4 = filePath + "dstInterpolation_7-7\\Spline3.tif";
	//rslt = SADataExec::viirs_Interpolation_Spline3(srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile4.c_str());

	//string dstFile5 = filePath + "dstInterpolation_7-7\\BezierCurve.tif";
	//rslt = SADataExec::viirs_Interpolation_BezierCurve(srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile5.c_str());


	//string dstFile7 = filePath + "dstInterpolation_7-7\\CESmoothing.tif";
	//rslt = SADataExec::viirs_Interpolation_CES(srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile7.c_str());

	//string dstFile8 = filePath + "dstInterpolation_7-7\\GFM.tif";
	//rslt = SADataExec::viirs_Interpolation_GFM(srsFile1.c_str(),
	//	srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
	//	srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile8.c_str());
	string dstFile6 = filePath + "dstInterpolation_7-7\\Hermite.tif";
	rslt = SADataExec::viirs_Interpolation_Hermite(srsFile1.c_str(),
		srsFile2.c_str(), srsFile3.c_str(), srsFile4.c_str(), srsFile5.c_str(), srsFile6.c_str(), srsFile7.c_str(), srsFile8.c_str(),
		srsFile9.c_str(), srsFile10.c_str(), srsFile11.c_str(), srsFile12.c_str(), srsFile13.c_str(), dstFile6.c_str());
}
// ͳ���쳣ֵ
void handleException_Shanghai() {

	////vector<string> strNames{ "BezierCurve_Shanghai.tif", "CESmoothing_Shanghai.tif", "GFM_Shanghai.tif", "LSM_Curve_Shanghai.tif", "LSM_Curve_Three_Shanghai.tif",
	////	"LSM_Shanghai.tif", "Spline3_Shanghai.tif", "Hermite_Shanghai.tif" };
	//vector<string> strNames{ "Hermite_right_Shanghai.tif" };
	//vector<int> lowExceptions;
	//vector<int> highExceptions;
	//vector<double> lowExceptionAverage;
	//vector<double> highExceptionAverage;

	//vector<vector<double>> low;
	//vector<vector<double>> high;
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\";

	//	int lowExceptionCount = 0;
	//	int highExceptionCount = 0;
	//	double lowExceptionValue = 0;
	//	double highExceptionValue = 0;

	//	vector<double> lowValues;
	//	vector<double> highValues;
	//	string srcPath = filePath + strNames[i];
	//	string dstPath = filePath + "noexception397_2\\" + strNames[i];
	//	RegionDataHanle::RecordAndHandleException(srcPath.c_str(), dstPath.c_str(), &lowExceptionCount, &highExceptionCount,
	//		&lowExceptionValue, &highExceptionValue, &lowValues, &highValues);

	//	low.push_back(lowValues);
	//	high.push_back(highValues);
	//	lowExceptions.push_back(lowExceptionCount);
	//	highExceptions.push_back(highExceptionCount);
	//	lowExceptionAverage.push_back(1.0 * lowExceptionValue );
	//	highExceptionAverage.push_back(1.0 * highExceptionValue );
	//	/*if (lowExceptionCount == 0 || lowExceptionValue == 0) {
	//		lowExceptionAverage.push_back(0.0);
	//	}
	//	else {
	//		lowExceptionAverage.push_back(1.0 * lowExceptionValue / lowExceptionCount);
	//	}

	//	if (highExceptionCount == 0 || highExceptionValue == 0) {
	//		highExceptionAverage.push_back(0.0);
	//	}
	//	else {
	//		highExceptionAverage.push_back(1.0 * highExceptionValue / highExceptionCount);
	//	}*/
	//}
}
void handleException_Wuhan() {

	////vector<string> strNames{ "BezierCurve_Wuhan.tif", "CESmoothing_Wuhan.tif", "GFM_Wuhan.tif", 
	////	"LSM_Curve_Wuhan.tif", "LSM_Curve_Three_Wuhan.tif",
	////	"LSM_Wuhan.tif", "Spline3_Wuhan.tif", "Hermite_Wuhan.tif" };
	//vector<string> strNames{"Hermite_right_Wuhan.tif"};
	//vector<int> lowExceptions;
	//vector<int> highExceptions;
	//vector<double> lowExceptionAverage;
	//vector<double> highExceptionAverage;
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Wuhan\\";

	//	int lowExceptionCount = 0;
	//	int highExceptionCount = 0;
	//	double lowExceptionValue = 0;
	//	double highExceptionValue = 0;

	//	vector<double> lowValues;
	//	vector<double> highValues;
	//	string srcPath = filePath + strNames[i];
	//	string dstPath = filePath + "noexception\\" + strNames[i];
	//	RegionDataHanle::RecordAndHandleException(srcPath.c_str(), dstPath.c_str(), &lowExceptionCount, &highExceptionCount,
	//		&lowExceptionValue, &highExceptionValue, &lowValues, &highValues);


	//	lowExceptions.push_back(lowExceptionCount);
	//	highExceptions.push_back(highExceptionCount);

	//	if (lowExceptionCount == 0 || lowExceptionValue == 0) {
	//		lowExceptionAverage.push_back(0.0);
	//	}
	//	else {
	//		lowExceptionAverage.push_back(1.0 * lowExceptionValue / lowExceptionCount);
	//	}

	//	if (highExceptionCount == 0 || highExceptionValue == 0) {
	//		highExceptionAverage.push_back(0.0);
	//	}
	//	else {
	//		highExceptionAverage.push_back(1.0 * highExceptionValue / highExceptionCount);
	//	}
	//}
}
void handleException_Zhumadian() {

	////vector<string> strNames{ "BezierCurve_zhumadian_mid.tif", "CESmoothing_zhumadian_mid.tif", "GFM_zhumadian_mid.tif",
	////	"LSM_Curve_zhumadian_mid.tif", "LSM_Curve_Three_zhumadian_mid.tif",
	////	"LSM_zhumadian_mid.tif", "Spline3_zhumadian_mid.tif", "Hermite_zhumadian_mid.tif" };
	//vector<string> strNames{ "Wuhan.tif" };
	//vector<int> lowExceptions;
	//vector<int> highExceptions;
	//vector<double> lowExceptionAverage;
	//vector<double> highExceptionAverage;
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\";
	//	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\zhumadian\\";

	//	int lowExceptionCount = 0;
	//	int highExceptionCount = 0;
	//	double lowExceptionValue = 0;
	//	double highExceptionValue = 0;
	//	vector<double> lowValues;
	//	vector<double> highValues;
	//	string srcPath = filePath + strNames[i];
	//	string dstPath = filePath + "noexception\\" + strNames[i];
	//	RegionDataHanle::RecordAndHandleException(srcPath.c_str(), dstPath.c_str(), &lowExceptionCount, &highExceptionCount,
	//		&lowExceptionValue, &highExceptionValue, &lowValues, &highValues);


	//	lowExceptions.push_back(lowExceptionCount);
	//	highExceptions.push_back(highExceptionCount);

	//	if (lowExceptionCount == 0 || lowExceptionValue == 0) {
	//		lowExceptionAverage.push_back(0.0);
	//	}
	//	else {
	//		lowExceptionAverage.push_back(1.0 * lowExceptionValue / lowExceptionCount);
	//	}
	//	
	//	if (highExceptionCount == 0 || highExceptionValue == 0) {
	//		highExceptionAverage.push_back(0.0);
	//	}
	//	else {
	//		highExceptionAverage.push_back(1.0 * highExceptionValue / highExceptionCount);
	//	}
	//}
}

// ͳ�Ʋ������쳣ֵ
void handleException() {

	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif", 
	"LSM_Three.tif", "LSM_Two.tif", "Spline.tif"};
	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city\\";
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ͳ�Ʊ��\\6������8�ַ����쳣ֵͳ��_0719.csv", std::ios::app);
	if (f.is_open()) {
		f << "�岹����," << "С��0ֵ������" << "," << "С��0ֵ����Ԫ��ֵ," << "���������ֵ����Ԫ����," << "���������ֵ����Ԫֵ��ֵ" << endl;
	}
	vector<double> thresholds{ 83.85,83.85, 196.52,196.52,  434.57 , 434.57 };

	for (size_t i = 0; i < regions.size(); i++)
	{
		f << regions[i] << endl;
		
		for (size_t j = 0; j < methods.size(); j++)
		{
			f << methods[j] << ",";

			int lowExceptionCount = 0;
			int highExceptionCount = 0;
			double lowExceptionValue = 0;
			double highExceptionValue = 0;

			string srcPath = filePath + regions[i] + methods[j];
			string dstPath = filePath + regions[i]+ "noexception\\final_" + methods[j];
			RegionDataHanle::RecordAndHandleException(srcPath.c_str(), dstPath.c_str(), &lowExceptionCount, &highExceptionCount,
				&lowExceptionValue, &highExceptionValue, thresholds[i]);

			f << lowExceptionCount << "," << (lowExceptionCount == 0 ? 0: (lowExceptionValue / lowExceptionCount)) << "," << highExceptionCount << "," << (highExceptionCount == 0 ?0: highExceptionValue / highExceptionCount )<< endl;
		}
		
	}
}
// ����ͳ���Ϻ� �人 פ�����ܵ���Ԫ����
void totalCount() {
#pragma region ����ԭʼ��vcm����  �Ϻ� �人 פ��� ����Ԫ����
	//string shStr = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\BezierCurve_Shanghai.tif";
	//string whStr = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Wuhan\\BezierCurve_Wuhan.tif";
	//string zmdStr = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\zhumadian\\BezierCurve_zhumadian_mid.tif";

	//int shCount = RegionDataHanle::totalCount(shStr.c_str());
	//int whCount = RegionDataHanle::totalCount(whStr.c_str());
	//int zmdCount = RegionDataHanle::totalCount(zmdStr.c_str());
#pragma endregion

	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\DifferenceAbsolute\\BezierCurve_Shanghai.tif";
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\Shanghai.tif";
	int shCount = RegionDataHanle::totalCount(filePath.c_str());
}

// ͳ��ͼ����Ԫֵ���ܺ� 
void calculateImageValueSum() {
	// ͳ�Ƹ�����������
	vector<string> methods{ "bezier","CESmoothing", "GFM", "Hermite", "LSM","LSM_Curve", "LSM_Curve_Three", "Spline3", "VCMSL" };
	
	vector<string> regions{ "������.tif", "��ɽ��.tif", "�޳���.tif", "��ˮ��.tif", "�γ���.tif",
	"������.tif", "������.tif", "������.tif","΢ɽ��.tif","��̨��.tif","������.tif","��ɽ��.tif","��ׯ��.tif", 
	"ƽ����.tif","������.tif","������.tif","����.tif", "������.tif", "������.tif", "�Ӷ���.tif", "۰����.tif", 
	"��ˮ��.tif","������.tif","������.tif", "��Ȫ��.tif", "������.tif", "������.tif","ԭ����.tif","��Ұ��.tif", 
	"�����.tif","��ԫ��.tif","�ӽ���.tif", "������.tif","������.tif", "������.tif", "��ԭ��.tif","������.tif",
	"�ܳǻ�����.tif","��ˮ��.tif", "�Ͻ���.tif", "�ݼ���.tif", "��Ĳ��.tif","������.tif","������.tif","������.tif",
	"��֣��.tif","�Ƿ���.tif" };
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\201806_high_latitude_cities_simulation\\";

	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ͳ�Ʊ��\\��������������Ԫֵ����_0724.csv", std::ios::app);
	
	f << "����" << "," << "����Ԫֵ" << "��Ԫ����"  << "ƽ����Ԫֵ"<< endl;

	for (size_t i = 0; i < methods.size(); i++)
	{
		f << methods[i] << endl;

		for (size_t j = 0; j < regions.size(); j++)
		{
			string dstPath = filePath  + methods[i] + "\\" + regions[j];

			double min = 1000, max = -1;
			int sum = RegionDataHanle::totalDNValue(dstPath.c_str(), &min, &max);
			int count = RegionDataHanle::totalCount(dstPath.c_str());

			f << regions[j] << "," << sum << "," << count << "," << 1.0 *sum / count << endl;
		}
	}
	f.close();
#pragma region ͳ�ƽ��в岹��� 3������  8�������� ����Ӱ�����Ԫֵ�ܺ�
	////vector<string> strNames{ "BezierCurve_Shanghai.tif", "CESmoothing_Shanghai.tif", "GFM_Shanghai.tif", "LSM_Curve_Shanghai.tif", "LSM_Curve_Three_Shanghai.tif",
	////"LSM_Shanghai.tif", "Spline3_Shanghai.tif", "Hermite_Shanghai.tif" };
	////string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\";

	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\����Ԫֵ����.csv", std::ios::app);
	//
	////f << "�岹����" << "," << "����Ԫֵ" << "," << "��Сֵ" << "," << "���ֵ" << endl;
	////vector<string> strNames{ "BezierCurve_Wuhan.tif", "CESmoothing_Wuhan.tif", "GFM_Wuhan.tif",
	////"LSM_Curve_Wuhan.tif", "LSM_Curve_Three_Wuhan.tif",
	////"LSM_Wuhan.tif", "Spline3_Wuhan.tif", "Hermite_Wuhan.tif" };
	////string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Wuhan\\";
	////f << "�人" << endl;

	////vector<string> strNames{ "BezierCurve_zhumadian_mid.tif", "CESmoothing_zhumadian_mid.tif", "GFM_zhumadian_mid.tif",
	////"LSM_Curve_zhumadian_mid.tif", "LSM_Curve_Three_zhumadian_mid.tif",
	////"LSM_zhumadian_mid.tif", "Spline3_zhumadian_mid.tif", "Hermite_zhumadian_mid.tif" };
	////string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\zhumadian\\";
	////f << "פ�����" << endl;

	//vector<string> strNames{ "Shanghai.tif", "Wuhan.tif", "Zhumadian.tif" };
	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\";
	//f << "ԭʼ���ݵ�ֵ" << endl;
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	string dstPath = filePath+ strNames[i];
	//	double min = 1000, max = -1;
	//	int sum = RegionDataHanle::totalDNValue(dstPath.c_str(), &min, &max);

	//	f << strNames[i] << "," << sum << "," << min << "," << max << endl;
	//}
	//f.close();
#pragma endregion

#pragma region Hermite��ֵ ������һ�飬
		
	/*string sh = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\noexception397_2\\Hermite_right_Shanghai.tif";
	string wh = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Wuhan\\noexception\\Hermite_right_Wuhan.tif";
	string zmd = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\zhumadian\\noexception2\\Hermite_right_zhumadian_mid.tif";
	
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\����Ԫֵ����.csv", std::ios::app);
	f << "Hermite ��ֵ" << endl;
	f << "�岹����" << "," << "����Ԫֵ" << "," << "��Сֵ" << "," << "���ֵ" << "," << "��Ч��Ԫֵ����" << endl;

	double min = 1000, max = -1;

	int sum = RegionDataHanle::totalDNValue(sh.c_str(), &min, &max);
	int count = RegionDataHanle::totalCount(sh.c_str());
	f << "�Ϻ�" << "," << sum << "," << min << "," << max << "," << count << endl;

	min = 1000, max = -1;

	sum = RegionDataHanle::totalDNValue(wh.c_str(), &min, &max);
	count = RegionDataHanle::totalCount(wh.c_str());
	f << "�人" << "," << sum << "," << min << "," << max << "," << count << endl;

	min = 1000, max = -1;

	sum = RegionDataHanle::totalDNValue(zmd.c_str(), &min, &max);
	count = RegionDataHanle::totalCount(zmd.c_str());
	f << "פ�����" << "," << sum << "," << min << "," << max << "," << count << endl;

	f.close();*/
#pragma endregion 

#pragma region ͳ�� �岹ͼ���� ԭʼ��������֮���ͼ��� ��Ԫֵ�ܺ�
	/*string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\DifferenceAbsolute\\";

	vector<string> strNames{ "BezierCurve_Shanghai.tif", "CESmoothing_Shanghai.tif", "GFM_Shanghai.tif", "LSM_Curve_Shanghai.tif", "LSM_Curve_Three_Shanghai.tif",
	"LSM_Shanghai.tif", "Spline3_Shanghai.tif", "Hermite_Shanghai.tif" };
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\�������ֵ��Ԫֵ����.csv", std::ios::app);
	
	f << "�Ϻ�" << endl;
	f << "�岹����" << "," << "����Ԫֵ" << "," << "��Сֵ" << "," << "���ֵ" << "," << "��Ч��Ԫֵ����" << endl;
	for (size_t i = 0; i < strNames.size(); i++)
	{
		string dstPath = filePath+ strNames[i];
		double min = 1000, max = -1;
		int sum = RegionDataHanle::totalDNValue(dstPath.c_str(), &min, &max);
		int count = RegionDataHanle::totalCount(dstPath.c_str());
		f << strNames[i] << "," << sum << "," << min << "," << max << "," << count <<  endl;
	}
	f.close();*/
#pragma endregion

#pragma region ��ȫ������ѡ�����ĳ���
	/*string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\";

	vector<string> strNames{ "Bezar.tif", "Dallas.tif", "delhi.tif", "dibai.tif",
	"Elpaso.tif", "fuyang.tif", "guangzhou.tif","hangzhou.tif", "haozhou.tif", 
	"Harris.tif", "kailuo.tif","Lubbock.tif", "marion.tif", "mengmai.tif", "Mexico.tif",
	"midland.tif", "mobile.tif", "Mumbai.tif", "Orange.tif", "Shanghai.tif","shenzhen.tif",
	"Wuhan.tif", "Xindeli.tif", "Zhumadian.tif", "loving.tif", "denton.tif", 
	"Oklahoma.tif", "travis.tif", "Collin.tif"};
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ȫ����ѡ���ĳ��еĻ�����Ϣ.csv", std::ios::app);

	
	f << "�岹����" << "," << "����Ԫֵ" << "," << "��Сֵ" << "," << "���ֵ" << "," << "��Ч��Ԫֵ����" << endl;
	for (size_t i = 0; i < strNames.size(); i++)
	{
		string dstPath = filePath+ strNames[i];
		double min = 1000, max = -1;
		double sum = RegionDataHanle::totalDNValue(dstPath.c_str(), &min, &max);
		int count = RegionDataHanle::totalCount(dstPath.c_str());
		f << strNames[i] << "," << sum << "," << min << "," << max << "," << count <<  endl;
	}
	f.close();*/

#pragma endregion

#pragma region ͳ�ƽ��в岹��� 6������  8�������� ����Ӱ�����Ԫֵ�ܺ�
//vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
//"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
//vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city\\";
//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\ͳ�Ʊ��\\6������8�ַ�����Ԫֵ�ܺ�ͳ��_0714.csv", std::ios::app);
//if (f.is_open()) {
//	f << "�岹����" << "," << "����Ԫֵ" << "," << "��Сֵ" << "," << "���ֵ" << endl;
//}
//
//for (size_t i = 0; i < regions.size(); i++)
//{
//	f << regions[i] << endl;
//
//	for (size_t j = 0; j < methods.size(); j++)
//	{
//		f << methods[j] << ",";
//
//		string dstPath = filePath + regions[i] + "noexception\\" + methods[j];
//		double min = 1000, max = -1;
//		double sum = RegionDataHanle::totalDNValue(dstPath.c_str(), &min, &max);
//
//		f << sum << "," << min << "," << max << endl;		
//	}
//
//}
//f.close();
#pragma endregion
}

// �������ֵӰ��vcm��ʵ������岹֮�����������õ��ľ���ֵ ����һ��Ӱ��
void difference() {
	// vcm 201807�·�  ����  ·�����
	vector<string> strNames{ "haozhou.tif", "fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif" };
	string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\";

	//  �岹������·��
	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city\\";
	string dstPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Difference_0719\\";
	for (size_t i = 0; i < regions.size(); i++)
	{

		for (size_t j = 0; j < methods.size(); j++)
		{

			string simulatePath = filePath + regions[i] + "noexception\\final_" + methods[j];
			string realPath = srcPath + strNames[i];
			//vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(), simulatePath.c_str());
			
			RegionDataHanle::differenceImage(realPath.c_str(), simulatePath.c_str(), (dstPath + regions[i] + methods[j]).c_str());

		}
	}
#pragma region �Ϻ�
	//string srcPath1 = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\Shanghai.tif";
	//string srcPath2 = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\noexception397_2\\";
	//string dstPath  = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\DifferenceAbsolute\\";

	////vector<string> strNames{ "BezierCurve_Shanghai.tif", "CESmoothing_Shanghai.tif", "GFM_Shanghai.tif", "LSM_Curve_Shanghai.tif", "LSM_Curve_Three_Shanghai.tif",
	////"LSM_Shanghai.tif", "Spline3_Shanghai.tif", "Hermite_Shanghai.tif" };

	//vector<string> strNames{ "Hermite_right_Shanghai.tif" };

	//for (size_t i = 0; i < strNames.size(); i++)
	//{

	//	RegionDataHanle::differenceImage(srcPath1.c_str(), (srcPath2 + strNames[i]).c_str(), (dstPath + strNames[i]).c_str());
	//}
#pragma endregion

#pragma region �人��
	//string srcPath1 = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\Wuhan.tif";
	//string srcPath2 = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Wuhan\\noexception\\";
	//string dstPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Wuhan\\DifferenceAbsolute\\";

	////vector<string> strNames{ "BezierCurve_Wuhan.tif", "CESmoothing_Wuhan.tif", "GFM_Wuhan.tif",
	////	"LSM_Curve_Wuhan.tif", "LSM_Curve_Three_Wuhan.tif",
	////	"LSM_Wuhan.tif", "Spline3_Wuhan.tif", "Hermite_Wuhan.tif" };
	//vector<string> strNames{"Hermite_right_Wuhan.tif" };

	//for (size_t i = 0; i < strNames.size(); i++)
	//{

	//	RegionDataHanle::differenceImage(srcPath1.c_str(), (srcPath2 + strNames[i]).c_str(), (dstPath + strNames[i]).c_str());
	//}
#pragma endregion

#pragma region פ�����
	//string srcPath1 = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\Zhumadian.tif";
	//string srcPath2 = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\zhumadian\\noexception2\\";
	//string dstPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\zhumadian\\DifferenceAbsolute\\";

	//vector<string> strNames{  "Hermite_right_zhumadian_mid.tif" };
	///*vector<string> strNames{ "BezierCurve_zhumadian_mid.tif", "CESmoothing_zhumadian_mid.tif", "GFM_zhumadian_mid.tif",
	//"LSM_Curve_zhumadian_mid.tif", "LSM_Curve_Three_zhumadian_mid.tif",
	//"LSM_zhumadian_mid.tif", "Spline3_zhumadian_mid.tif", "Hermite_zhumadian_mid.tif" };*/
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	RegionDataHanle::differenceImage(srcPath1.c_str(), (srcPath2 + strNames[i]).c_str(), (dstPath + strNames[i]).c_str());
	//}
#pragma endregion
}

int main(int argc, char** argv) {
	CPLSetConfigOption("GDAL_DATA", "D:\\Program Files\\gdal\\data");
	CPLSetConfigOption("GDAL_DATA", "D:\\Program Files\\gdal\\data\\");
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	scatterDiagram();
	//calculateImageValueSum();
	//scatterDiagram();
	//calculateImageValueSum();
	//totalCount();
	//midFilter();	
	//interpolation_75N60E();
	//interpolation_75N180W();
	//interpolation_75N60W();
	//handleException_Shanghai();
	//handleException_Wuhan();
	//handleException_Zhumadian();

	//totalCount();
	//handleException();
	//calculateImageValueSum();

	//interpolation_75N60E();

	//difference();
	//calculateImageValueSum();
	//totalCount();
	//histogram();
	//calculateImageValueSum();
	//scatterDiagram();
	//handleException_Zhumadian();
	//calculateImageValueSum();
	//scatterDiagram();
	//calculateImageValueSum();
	//averageDiffValue();
	//difference();
	//handleException();
	//histogram();
	//scatterDiagram();
	//interpolation_75N60E();
	//difference();
	system("pause");
	return 0;
}
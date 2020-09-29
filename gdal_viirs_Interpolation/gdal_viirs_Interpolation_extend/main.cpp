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

// 中值滤波 
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
// 直方图统计
void histogram() {
	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\"};
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Difference_0719\\";

	// vcm 201807月份  数据  路径相关
	vector<string> strNames{ "haozhou.tif", "fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif" };
	string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Difference_0719\\Interpolation_results_of_8_methods_in_each_city";

	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\统计表格\\6个城市8种方法差异分级结果直方图数据统计_0720.csv", std::ios::app);
	if (f.is_open()) {
		f << "6个城市8种插补方法差异值分级比较" << endl;
	}
	// 插补结果
	//vector<double> gaps = { 0.36, 1, 1.92, 5.03, 9.7, 14.37, 20.59, 26.81, 33.04, 42.38 };//上海
//	vector<double> gaps = {0.33, 0.5, 1.08, 1.83, 4.09, 7.84, 13.85, 19.86, 27.37, 43.89};// COllin
	//vector<double> gaps = {0.31, 0.56, 0.68, 0.93, 1.45, 2.5, 4.67, 9.11, 18.25, 37}; // fuyang
	// 差异值分级展示
	//vector<double> gaps{0, 0.54, 1.9, 3.67, 5.7, 8.01, 11, 14.26, 18.74, 24.3}; // 亳州与阜阳
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

				f << "[" << iter->startValue << "，" << right << ")," << 1.0 * iter->pixelCount / validPixelCount
					<<"," << validPixelCount << endl;
			}
		}

	}
	f.close();


#pragma region 经过这做差之后的上海市的8种方法的直方图
	//vector<string> strNames{ "BezierCurve_Shanghai.tif", "CESmoothing_Shanghai.tif", "GFM_Shanghai.tif", "LSM_Curve_Shanghai.tif", "LSM_Curve_Three_Shanghai.tif",
	//"LSM_Shanghai.tif", "Spline3_Shanghai.tif", "Hermite_right_Shanghai.tif" };
	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\noexception397_2\\";
	////vector<int> gaps{ 0,2,5,10, 30, 100 };
	//vector<double> gaps;
	//for (int i = 0; i <= 500; i = i+1) {
	//	gaps.push_back(i);
	//}
	//// 将直方图数据写入到csv中
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\插补结果的直方图数据_上海_2.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "上海市8种插补方式的差异分级比较" << endl;
	//	f << "数值区间_前后0.5" << "," << "区间内像元的数量, 区间内像元的均值" << endl;
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
	//	f << "平均差异值为 " << totalPixelValue / validPixelCount << endl;
	//}

	//f.close();
#pragma endregion

#pragma region 原始vcm7月份直方图数据 上海 武汉 驻马店
	//vector<string> strNames{ "Shanghai.tif"/*, "Wuhan.tif", "Zhumadian.tif"*/ };

	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\";
	//vector<double> gaps { 0, 1, 2,3,4, 5,6,7,8,9, 10,12,14,16,18, 20 ,24, 28, 40, 60, 100};
	////for (int i = 0; i < 300; i++) {
	////	gaps.push_back(i);
	////}
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\原始vcm7月份直方图数据_0710.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "数值_向前取整" << "," << "区间内像元的数量" << endl;
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

	//		f << "[" << iter->startValue << "，"<< right << ")," << iter->pixelCount  << endl;
	//	}
	//}

	//f.close();
#pragma endregion

#pragma region 经过这做差之后的武汉 市的8种方法的直方图
	//vector<string> strNames{ "BezierCurve_Wuhan.tif", "CESmoothing_Wuhan.tif", "GFM_Wuhan.tif", 
	//	"LSM_Curve_Wuhan.tif", "LSM_Curve_Three_Wuhan.tif",
	//	"LSM_Wuhan.tif", "Spline3_Wuhan.tif", "Hermite_right_Wuhan.tif" };
	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\\Wuhan\\DifferenceAbsolute\\";
	////vector<int> gaps{ 0,1,2,5,10, 30 };
	//vector<double> gaps;
	//for (int i = 0; i < 20; i++) {
	//	gaps.push_back(i);
	//}
	//// 将直方图数据写入到csv中
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\插补结果的直方图数据_武汉.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "武汉市市8种插补方式的差异分级比较" << endl;
	//	f << "数值区间_前闭后开" << "," << "区间内像元的数量, 区间内像元的均值" << endl;
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
	//	f << "平均差异值为, " << totalPixelValue / validPixelCount << endl;
	//}

	//f.close();
#pragma endregion

#pragma region 经过做差之后的驻马店 市的8种方法的直方图
	//vector<string> strNames{ "BezierCurve_zhumadian_mid.tif", "CESmoothing_zhumadian_mid.tif", "GFM_zhumadian_mid.tif",
	//	"LSM_Curve_zhumadian_mid.tif", "LSM_Curve_Three_zhumadian_mid.tif",
	//	"LSM_zhumadian_mid.tif", "Spline3_zhumadian_mid.tif", "Hermite_right_zhumadian_mid.tif" };
	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\\zhumadian\\";
	////vector<double> gaps{ 0,0.5,5,10,20 };
	//vector<double> gaps;
	//for (int i = 0; i < 50; i++) {
	//	gaps.push_back(i);
	//}
	//// 将直方图数据写入到csv中
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\插补结果的直方图数据_驻马店.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "驻马店市8种插补方式的差异分级比较" << endl;
	//	f << "数值,前后0.5四舍五入" << "," << "区间内像元的数量, 区间内像元的均值" << endl;
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
	//	f << "平均差异值为, " << totalPixelValue / validPixelCount << endl;
	//}

	//f.close();
#pragma endregion
}
//散点图
void scatterDiagram() {
#pragma region 时间序列
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\统计表格\\时间序列预测散点图统计_0923_3.csv", std::ios::app);
	if (f.is_open()) {
		
	}
	vector<string> months{ "07", "08", "09", "10", "11", "12" };
	for (size_t i = 0; i < months.size(); i++)
	{
		string realPath = "F:\\04-dissertation\\02-VIIRS\\用到的相关代码\\时间序列\\city_world_raster\\Shanghai\\2018" + months[i]+".tif";
		string simulatePath = "F:\\04-dissertation\\02-VIIRS\\用到的相关代码\\时间序列\\city_world_raster\\ts_result\\2018" + months[i] + "_Shanghai_95.tif";
		vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(realPath.c_str(), simulatePath.c_str());
		f << "2018" + months[i] + "_Shanghai_95" << endl;
		f << "编号," << "vcm" << "," << "ts" << endl;
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

#pragma region 北方缺失数据区域 4个城市 与vcmsl的对比
//	//插补结果相关路径
//	vector<string> methods{ "bezier","CESmoothing", "GFM", "Hermite", "LSM",
//
//"LSM_Curve", "LSM_Curve_Three", "Spline3" };
//	vector<string> regions{ "郑州市.tif","新乡市.tif", "临沂市.tif", "济宁市.tif" };
//	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\201806_high_latitude_cities_simulation\\";
//
//	// vcm 201806月份  数据  路径相关
//	//vector<string> strNames{"Bozhou.tif", "Fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif"};
//	//string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\vcmsl\\";
//
//	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\统计表格\\4个城市8种方法vcmsl散点图数据统计_0723.csv", std::ios::app);
//	if (f.is_open()) {
//		f << "4个城市8种插补方法插补结果与vcmsl数据比较" << endl;
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
//		// 输出标题
//		f << endl;
//		f << "序号,vcmsl" << ",";
//		for (auto p : map) {
//			f << p.first << ",";
//		}
//		f << endl;
//		// 输出具体内容
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

#pragma region 6个城市vcm  与vcmsl的对比
	///*vector<string> strNames{ "Bozhou.tif", "Fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif" };
	//string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\";

	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\统计表格\\6个城市_vcm_vcmsl_散点图数据统计_0721.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "6个城市 vcm与 vcmsl 比较" << endl;
	//}

	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	string vcm = srcPath + strNames[i];
	//	string vcmsl = srcPath + "vcmsl\\" + strNames[i];
	//	vector<Real_Simulate>* hists = RegionDataHanle::viirs_scatter(vcm.c_str(), vcmsl.c_str());

	//	f << strNames[i] << endl;
	//	f << "序号,vcm,vcmsl" << endl;
	//	for (size_t j = 0; j < hists->size(); j++)
	//	{
	//		f << j + 1 << "," << hists->at(j).real << "," << hists->at(j).simulate << endl;
	//	}
	//}
	//f.close();*/
#pragma endregion

#pragma region   全部的6个城市与 vcmsl比较
	  //插补结果相关路径
//	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
//"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
//	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
//	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city_0719\\";
//
//	// vcm 201807月份  数据  路径相关
//	vector<string> strNames{"Bozhou.tif", "Fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif"};
//	string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\vcmsl\\";
//
//	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\统计表格\\6个城市8种方法vcmsl散点图数据统计_0721.csv", std::ios::app);
//	if (f.is_open()) {
//		f << "6个城市8种插补方法插补结果与vcmsl数据比较" << endl;
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
//		// 输出标题
//		f << endl;
//		f << "序号,vcmsl" << ",";
//		for (auto p : map) {
//			f << p.first << ",";
//		}
//		f << endl;
//		// 输出具体内容
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
#pragma region   全部的6个城市与 vcm比较
	 //  插补结果相关路径
//	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
//"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
//	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
//	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city\\";
//
//	// vcm 201807月份  数据  路径相关
//	vector<string> strNames{"haozhou.tif", "fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif"};
//	string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\";
//
//	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\统计表格\\6个城市8种方法散点图数据统计_0712.csv", std::ios::app);
//	if (f.is_open()) {
//		f << "6个城市8种插补方法插补结果与真实数据比较" << endl;
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
//		// 输出标题
//		f << endl;
//		f << "序号,vcm" << ",";
//		for (auto p : map) {
//			f << p.first << ",";
//		}
//		f << endl;
//		// 输出具体内容
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
#pragma region 上海
	/*vector<string> strNames{ "BezierCurve_Shanghai.tif", "CESmoothing_Shanghai.tif", "GFM_Shanghai.tif", "LSM_Curve_Shanghai.tif", "LSM_Curve_Three_Shanghai.tif",
"LSM_Shanghai.tif", "Spline3_Shanghai.tif", "Hermite_right_Shanghai.tif" };
	string simulatePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\noexception397_2\\";

	string realPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\Shanghai.tif";
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\去除异常的插补结果的散点图数据_上海.csv", std::ios::app);
	if (f.is_open()) {
		f << "上海市8种插补方法插补结果与真实数据比较" << endl;
		f << "序号" << "真实数据" << "," << "插补数据" << endl;
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

#pragma region 武汉市
	//vector<string> strNames{ /*"BezierCurve_Wuhan.tif", "CESmoothing_Wuhan.tif", "GFM_Wuhan.tif", 
	//	"LSM_Curve_Wuhan.tif", "LSM_Curve_Three_Wuhan.tif",
	//	"LSM_Wuhan.tif", "Spline3_Wuhan.tif",*/ "Hermite_right_Wuhan.tif" };
	////vector<string> strNames{  "LSM_Wuhan.tif" };
	//string simulatePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Wuhan\\noexception\\";

	//string realPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\noexception\\Wuhan.tif";
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\去除异常的插补结果的散点图数据_武汉3.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "武汉市8种插补方法插补结果与真实数据比较" << endl;
	//	f << "序号," << "真实数据" << "," << "插补数据" << endl;
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
#pragma region 驻马店市
	//vector<string> strNames{ /*"BezierCurve_zhumadian_mid.tif", "CESmoothing_zhumadian_mid.tif", "GFM_zhumadian_mid.tif",
	//	"LSM_Curve_zhumadian_mid.tif", "LSM_Curve_Three_zhumadian_mid.tif",
	//	"LSM_zhumadian_mid.tif", "Spline3_zhumadian_mid.tif",*/ "Hermite_right_zhumadian_mid.tif" };
	//string simulatePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\zhumadian\\noexception2\\";

	//string realPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\Zhumadian.tif";
	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\去除异常的插补结果的散点图数据_驻马店_2.csv", std::ios::app);
	//if (f.is_open()) {
	//	f << "驻马店市8种插补方法插补结果与真实数据比较" << endl;
	//	f << "序号" << "真实数据" << "," << "插补数据" << endl;
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

// 平均差异绝对值
void averageDiffValue() {
	//  插补结果相关路径
	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city\\";

	// vcm 201807月份  数据  路径相关
	vector<string> strNames{ "haozhou.tif", "fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif" };
	string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\";

	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\统计表格\\6个城市8种方法平均差异绝对值统计_0714.csv", std::ios::app);
	if (f.is_open()) {
		f << "6个城市8种插补方法插补结果与真实数据比较" << endl;
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
		f << "像元数量为," << pixelCount << endl;
		// 输出标题
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
// 插补
void interpolation_75N60E() {
#pragma region 对7月份进行插补

	//int rslt = -1;
	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\";
	////6.VIIRS vcm月度数据-时间序列插补
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
	//6.VIIRS vcm月度数据-时间序列插补
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
	//6.VIIRS vcm月度数据-时间序列插补
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
	//6.VIIRS vcm月度数据-时间序列插补
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
// 统计异常值
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

// 统计并处理异常值
void handleException() {

	vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif", 
	"LSM_Three.tif", "LSM_Two.tif", "Spline.tif"};
	vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city\\";
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\统计表格\\6个城市8种方法异常值统计_0719.csv", std::ios::app);
	if (f.is_open()) {
		f << "插补方法," << "小于0值的数量" << "," << "小于0值的像元均值," << "大于最大阈值的像元数量," << "大于最大阈值的像元值均值" << endl;
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
// 用来统计上海 武汉 驻马店的总的像元个数
void totalCount() {
#pragma region 计算原始的vcm数据  上海 武汉 驻马店 的像元个数
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

// 统计图像像元值的总和 
void calculateImageValueSum() {
	// 统计各个区的亮度
	vector<string> methods{ "bezier","CESmoothing", "GFM", "Hermite", "LSM","LSM_Curve", "LSM_Curve_Three", "Spline3", "VCMSL" };
	
	vector<string> regions{ "兖州区.tif", "梁山县.tif", "邹城市.tif", "泗水县.tif", "任城区.tif",
	"金乡县.tif", "汶上县.tif", "曲阜市.tif","微山县.tif","鱼台县.tif","嘉祥县.tif","兰山区.tif","罗庄区.tif", 
	"平邑县.tif","临沭县.tif","沂南县.tif","费县.tif", "蒙阴县.tif", "兰陵县.tif", "河东区.tif", "郯城县.tif", 
	"沂水县.tif","莒南县.tif","新乡县.tif", "凤泉区.tif", "封丘县.tif", "红旗区.tif","原阳县.tif","牧野区.tif", 
	"获嘉县.tif","长垣县.tif","延津县.tif", "卫滨区.tif","卫辉市.tif", "辉县市.tif", "中原区.tif","二七区.tif",
	"管城回族区.tif","金水区.tif", "上街区.tif", "惠济区.tif", "中牟县.tif","巩义市.tif","荥阳市.tif","新密市.tif",
	"新郑市.tif","登封市.tif" };
	string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\201806_high_latitude_cities_simulation\\";

	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\统计表格\\北方各区县总像元值汇总_0724.csv", std::ios::app);
	
	f << "区域" << "," << "总像元值" << "像元个数"  << "平均像元值"<< endl;

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
#pragma region 统计进行插补后的 3个城市  8个方法的 各个影像的像元值总和
	////vector<string> strNames{ "BezierCurve_Shanghai.tif", "CESmoothing_Shanghai.tif", "GFM_Shanghai.tif", "LSM_Curve_Shanghai.tif", "LSM_Curve_Three_Shanghai.tif",
	////"LSM_Shanghai.tif", "Spline3_Shanghai.tif", "Hermite_Shanghai.tif" };
	////string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\";

	//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\总像元值汇总.csv", std::ios::app);
	//
	////f << "插补方法" << "," << "总像元值" << "," << "最小值" << "," << "最大值" << endl;
	////vector<string> strNames{ "BezierCurve_Wuhan.tif", "CESmoothing_Wuhan.tif", "GFM_Wuhan.tif",
	////"LSM_Curve_Wuhan.tif", "LSM_Curve_Three_Wuhan.tif",
	////"LSM_Wuhan.tif", "Spline3_Wuhan.tif", "Hermite_Wuhan.tif" };
	////string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Wuhan\\";
	////f << "武汉" << endl;

	////vector<string> strNames{ "BezierCurve_zhumadian_mid.tif", "CESmoothing_zhumadian_mid.tif", "GFM_zhumadian_mid.tif",
	////"LSM_Curve_zhumadian_mid.tif", "LSM_Curve_Three_zhumadian_mid.tif",
	////"LSM_zhumadian_mid.tif", "Spline3_zhumadian_mid.tif", "Hermite_zhumadian_mid.tif" };
	////string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\zhumadian\\";
	////f << "驻马店市" << endl;

	//vector<string> strNames{ "Shanghai.tif", "Wuhan.tif", "Zhumadian.tif" };
	//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\vcm_2018\\month7_orginaldata\\";
	//f << "原始数据的值" << endl;
	//for (size_t i = 0; i < strNames.size(); i++)
	//{
	//	string dstPath = filePath+ strNames[i];
	//	double min = 1000, max = -1;
	//	int sum = RegionDataHanle::totalDNValue(dstPath.c_str(), &min, &max);

	//	f << strNames[i] << "," << sum << "," << min << "," << max << endl;
	//}
	//f.close();
#pragma endregion

#pragma region Hermite插值 又做了一遍，
		
	/*string sh = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\noexception397_2\\Hermite_right_Shanghai.tif";
	string wh = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Wuhan\\noexception\\Hermite_right_Wuhan.tif";
	string zmd = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\zhumadian\\noexception2\\Hermite_right_zhumadian_mid.tif";
	
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\总像元值汇总.csv", std::ios::app);
	f << "Hermite 插值" << endl;
	f << "插补地区" << "," << "总像元值" << "," << "最小值" << "," << "最大值" << "," << "有效像元值个数" << endl;

	double min = 1000, max = -1;

	int sum = RegionDataHanle::totalDNValue(sh.c_str(), &min, &max);
	int count = RegionDataHanle::totalCount(sh.c_str());
	f << "上海" << "," << sum << "," << min << "," << max << "," << count << endl;

	min = 1000, max = -1;

	sum = RegionDataHanle::totalDNValue(wh.c_str(), &min, &max);
	count = RegionDataHanle::totalCount(wh.c_str());
	f << "武汉" << "," << sum << "," << min << "," << max << "," << count << endl;

	min = 1000, max = -1;

	sum = RegionDataHanle::totalDNValue(zmd.c_str(), &min, &max);
	count = RegionDataHanle::totalCount(zmd.c_str());
	f << "驻马店市" << "," << sum << "," << min << "," << max << "," << count << endl;

	f.close();*/
#pragma endregion 

#pragma region 统计 插补图像与 原始数据做差之后的图像的 像元值总和
	/*string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Shanghai\\DifferenceAbsolute\\";

	vector<string> strNames{ "BezierCurve_Shanghai.tif", "CESmoothing_Shanghai.tif", "GFM_Shanghai.tif", "LSM_Curve_Shanghai.tif", "LSM_Curve_Three_Shanghai.tif",
	"LSM_Shanghai.tif", "Spline3_Shanghai.tif", "Hermite_Shanghai.tif" };
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\差异绝对值像元值汇总.csv", std::ios::app);
	
	f << "上海" << endl;
	f << "插补方法" << "," << "总像元值" << "," << "最小值" << "," << "最大值" << "," << "有效像元值个数" << endl;
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

#pragma region 从全球中挑选出来的城市
	/*string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\";

	vector<string> strNames{ "Bezar.tif", "Dallas.tif", "delhi.tif", "dibai.tif",
	"Elpaso.tif", "fuyang.tif", "guangzhou.tif","hangzhou.tif", "haozhou.tif", 
	"Harris.tif", "kailuo.tif","Lubbock.tif", "marion.tif", "mengmai.tif", "Mexico.tif",
	"midland.tif", "mobile.tif", "Mumbai.tif", "Orange.tif", "Shanghai.tif","shenzhen.tif",
	"Wuhan.tif", "Xindeli.tif", "Zhumadian.tif", "loving.tif", "denton.tif", 
	"Oklahoma.tif", "travis.tif", "Collin.tif"};
	ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\全球挑选出的城市的基本信息.csv", std::ios::app);

	
	f << "插补方法" << "," << "总像元值" << "," << "最小值" << "," << "最大值" << "," << "有效像元值个数" << endl;
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

#pragma region 统计进行插补后的 6个城市  8个方法的 各个影像的像元值总和
//vector<string> methods{ "Bezier.tif","Exponent.tif", "GFM.tif", "Hermite.tif", "LSM.tif",
//"LSM_Three.tif", "LSM_Two.tif", "Spline.tif" };
//vector<string> regions{ "bozhou\\","fuyang\\", "Collin\\", "Elpaso\\", "Harris\\","Shanghai\\" };
//string filePath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\Interpolation_results_of_8_methods_in_each_city\\";
//ofstream f("F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\统计表格\\6个城市8种方法像元值总和统计_0714.csv", std::ios::app);
//if (f.is_open()) {
//	f << "插补方法" << "," << "总像元值" << "," << "最小值" << "," << "最大值" << endl;
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

// 差异绝对值影像，vcm真实数据与插补之后的数据做差，得到的绝对值 构成一幅影像
void difference() {
	// vcm 201807月份  数据  路径相关
	vector<string> strNames{ "haozhou.tif", "fuyang.tif", "Collin.tif", "Elpaso.tif", "Harris.tif", "Shanghai.tif" };
	string srcPath = "F:\\04-dissertation\\02-VIIRS\\viirs_data\\interpolation_rslt\\city_world_vcm_201807\\";

	//  插补结果相关路径
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
#pragma region 上海
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

#pragma region 武汉市
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

#pragma region 驻马店市
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
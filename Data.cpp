#include "Data.h"

Data::Data()
{
	
	x_line = 0;
	beginRow = 0;
	endRow = 0;
	invalid_value = 0;
	showRownums = 0;
	Rownums = 0;
	Colnums = 0;
	insert_point = 0;
	x_line_name = "";
	filepath = "";
	filter_way = "median-filter";
	
}

Data::~Data()
{
	alldata.clear();
	Nomdata.clear();
	gain_para.clear();
	chart_map.clear();
	flip_chart_map.clear();
	filter_result.clear();
}

void Data::clearalldata()
{
	alldata.clear();
	Nomdata.clear();
	gain_para.clear();
	chart_map.clear();
	flip_chart_map.clear();
	filter_result.clear();
}

void Data::curve_processing()
{
	if (filter_way == "median-filter") {
		median_filter();//计算出中值基线
		baseline_correction();//基线校正
		return;
	}
	baseline_correction();

}

void Data::baseline_correction()
{
	int& Row_nums = showRownums;
	for (auto it = chart_map.begin(); it != chart_map.end(); it++)
	{
		QString line_name;
		QVector<float> temp_data = Nomdata.value(it.key());//获取初始数据
		QVector<float> filter_data;
		if (filter_result.count(it.key() + "-avg"))
		{
			line_name = it.key() + "-adjust";
			filter_data = filter_result.value(it.key() + "-avg");
		}
		else
		{
			line_name = it.key() + "-" + filter_way + "-adjust";
			filter_data = filter_result.value(filter_way);
		}
			
		QVector<float> result_data;
		for (int i = 0; i < Row_nums; i++)
		{
			result_data.push_back(temp_data[i] - filter_data[i] + 1);
		}
		filter_result.insert(std::move(line_name), std::move(result_data));
	}
}

float Data::median_number(const int& begin, const int& end, const QString& name)
{
	QVector<float> temp;
	for (int i = begin; i < end; i++) {
		temp.push_back(Nomdata.value(name)[i]);
	}
	std::sort(temp.begin(), temp.end());
	return std::move(temp[(end - begin) / 2]);
}

void Data::median_filter()
{
	if (insert_point < 1) insert_point = 1;
	int& Row_nums = showRownums;
	int interdv2 = insert_point / 2;
	for (auto it = chart_map.begin(); it != chart_map.end(); it++) {
		QVector<float> temp_data;
		for (int i = 0; i < Row_nums; i++) {
			if (i < interdv2) {
				temp_data.push_back(median_number(i, i + interdv2, it.key()));
				continue;
			}
			if (i + interdv2 > Row_nums) {
				temp_data.push_back(median_number(i- interdv2, Row_nums, it.key()));
				continue;
			}
			temp_data.push_back(median_number(i - interdv2, i + interdv2, it.key()));
		}
		
		QString line_name = it.key() + "-avg";//"IMP-avg"
		filter_result.insert(std::move(line_name), std::move(temp_data));
	}
	
}

void Data::gain_process()
{
	for (auto it = chart_map.begin(); it != chart_map.end(); it++)
	{
		QString line_name;
		const int& _adjust = gain_para.value(it.key())[0];
		const int& _gain = gain_para.value(it.key())[1];
		QVector<float> temp_data;
		if (filter_result.count(it.key() + "-adjust"))
		{
			line_name = it.key() + "-gain";
			temp_data = filter_result.value(it.key() + "-adjust");
		}
		else
		{
			line_name = it.key() + "-" + filter_way + "-gain";
			temp_data = filter_result.value(it.key() + "-" + filter_way + "-adjust");
		}
		
		const int& size = temp_data.size();
		//找到校正后曲线最大值
		QMap<float, int> sort_map;
		for (int i = 0; i < size; i++) {
			sort_map.insert(temp_data[i], i);
		}
		//增益
		int gain_size = size * (double)(_adjust) * 0.01;
		auto it_map = sort_map.end()-1;
		for (int i = 0; i < gain_size; i++) {
			if (it_map.key() < 1) break;
			const int& local = it_map.value();
			temp_data[local] = temp_data[local] * _gain;
			if (temp_data[local] > 1.5) 
				temp_data[local] = 1.5;
			it_map--;
		}
		filter_result.insert(line_name, temp_data);
	}
}
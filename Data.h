#pragma once

#include <QObject>
#include <qstringlist.h>
#include <qvector.h>
#include <qvariant.h>
#include <qdebug.h>
#include <qpoint.h>




class Data  : public QObject
{
	Q_OBJECT

public:
	Data();
	~Data();
	int x_line;
	int beginRow;//开始行
	int endRow;//结束行
	float invalid_value;
	int showRownums;//显示数据行数
	int Rownums;//行数
	int Colnums;//列数
	int insert_point;
	QString x_line_name;
	QString filepath;//文件路径
	QVector<QStringList> alldata;//初始数据
	QMap<QString,QVector<float>> Nomdata;//存放x轴以及勾选的归一化的数据
	QMap<QString, QVector<int>> gain_para;//曲线的增益参数
	QMap<QString, int> chart_map;//存放勾选的线以及对应列名字
	QMap<QString, int> flip_chart_map;
	QString filter_way;
	QStringList process_result;//存放基线的处理结果
	void clearalldata();
	void curve_processing();//基线处理主函数
	void baseline_correction();//基线校正
	void median_filter();//中值滤波基线计算
	void gain_process();
	float median_number(const int& begin, const int& end, const QString& col);
	QMap<QString, QVector<float>> filter_result;//存放曲线值以及对应名称

};

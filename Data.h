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
	int beginRow;//��ʼ��
	int endRow;//������
	float invalid_value;
	int showRownums;//��ʾ��������
	int Rownums;//����
	int Colnums;//����
	int insert_point;
	QString x_line_name;
	QString filepath;//�ļ�·��
	QVector<QStringList> alldata;//��ʼ����
	QMap<QString,QVector<float>> Nomdata;//���x���Լ���ѡ�Ĺ�һ��������
	QMap<QString, QVector<int>> gain_para;//���ߵ��������
	QMap<QString, int> chart_map;//��Ź�ѡ�����Լ���Ӧ������
	QMap<QString, int> flip_chart_map;
	QString filter_way;
	QStringList process_result;//��Ż��ߵĴ�����
	void clearalldata();
	void curve_processing();//���ߴ���������
	void baseline_correction();//����У��
	void median_filter();//��ֵ�˲����߼���
	void gain_process();
	float median_number(const int& begin, const int& end, const QString& col);
	QMap<QString, QVector<float>> filter_result;//�������ֵ�Լ���Ӧ����

};

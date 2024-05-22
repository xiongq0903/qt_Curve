#pragma once

#include <QWidget>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qtableview.h>
#include <qstandarditemmodel.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qvector.h>
#include <qbarseries.h>
#include <QtCharts>
#include <qmap.h>

#pragma execution_character_set("utf-8")

class Lithologymapwidget  : public QWidget
{
	Q_OBJECT

public:
	Lithologymapwidget(QWidget *parent = nullptr);
	~Lithologymapwidget();
	int rownums;
	int colnums = 0;
	QString file_path;
	QLineEdit* filename;
	QPushButton* btn_file_select;
	QPushButton* btn_litmap_affirm;
	QGridLayout* my_layout;
	QTableView* tableview;
	QVector<QStringList> litmap_data;
	QVector<QPointF> data_rect;//直接存储矩形的宽和高
	QMap<int, QPushButton*> btn_bar_map;
	QMap<int, QString> bar_color;

	float min;//岩性图x轴范围 最小值
	float max;//最大值


private:
	void data2table();
	void add_btn();
	void get_data_bar();

signals:
	void btn_litmap_affirm_clicked();

private slots:
	void btn_file_select_clicked();
};

#pragma once

#include <QWidget>
#include "ui_Operatewidget.h"
#include <qmap.h>
#include <qlabel.h>
#include <qvector.h>
#include <qstandarditemmodel.h>
#include <qlineseries.h>
#include <QtCharts>

#pragma execution_character_set("utf-8")

class Operatewidget : public QWidget
{
	Q_OBJECT

public:
	Operatewidget(QWidget *parent = nullptr);
	~Operatewidget();
	int insert_point;
	int adjust;
	int gain;
	QString draw_name;
	QWidget* draw_widget;
	QListView* listView;
	QMap<QString, QSpinBox*> my_spin;
	QMap<QString, QLineSeries*> filter_result_series;
	QComboBox* baseline_select;
	QString filter_way;
	QStringList find_check_listview();
	QLineEdit* name_edit;
	void add_QSpinBox(const QVector<QString>& line_name);
	void add_listview();

private:
	void btn_customize_clicked();
	Ui::OperatewidgetClass ui;

signals:
	void btn_draw_clicked();
	void btn_drag_clicked();
	void btn_affirm_clicked();
	void btn_true_clicked();
	void btn_calculate_cliked();
	void btn_clearlistview_clicked();
};

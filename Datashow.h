#pragma once

#include <QMainWindow>
#include "ui_Datashow.h"
#include "Data.h"
#include <QStandardItemModel>
#include <qcheckbox.h>
#include <qmessagebox.h>


#include <sys/utime.h>
#include <qdebug.h>

#pragma execution_character_set("utf-8")

class Datashow : public QMainWindow
{
	Q_OBJECT

public:
	Datashow();
	~Datashow();
	bool firstshow = true;
	std::shared_ptr<Data> data_object;
	QVector<QModelIndex*> data_changed;
	void btn_reset_clicked();
	void data2table();
	void clearalldata();

signals:
	void btn_begin();

private slots:
	void btn_confirm_cliked();
	

private:
	int title_line;
	int jump_line;
	int end_line;
	int x_line;
	float invalid_value;
	bool get_axis();
	QVector<QCheckBox*> my_check;
	QVector<QCheckBox*> my_flip_check;
	void clear_checkbox();
	void showdata();
	Ui::DatashowClass ui;
	
};

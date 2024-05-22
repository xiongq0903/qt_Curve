#pragma once

#include <QWidget>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qstandarditemmodel.h>

#pragma execution_character_set("utf-8")

class Clearwidget  : public QWidget
{
	Q_OBJECT

public:
	Clearwidget(QWidget* parent = nullptr);
	~Clearwidget();
	QListView* cw_listview;
	QPushButton* btn_clear_cw;
	QPushButton* btn_clearall_cw;

signals:
	void btn_clear_cw_clicked();
	void btn_clearall_cw_clicked();

};

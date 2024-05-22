#pragma once

#include <QDialog>
#include "ui_Styletwidget.h"

class Styletwidget : public QDialog
{
	Q_OBJECT

public:
	Styletwidget(QWidget *parent = nullptr);
	~Styletwidget();
	int color;
	int width;
	int type;
	QComboBox* select_curve;
	QString select_line_name;

signals:
	void btn_styleaffirm_clicked();

private:
	Ui::StyletwidgetClass ui;
};

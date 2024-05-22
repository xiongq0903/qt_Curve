#include "Styletwidget.h"

Styletwidget::Styletwidget(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	select_curve = ui.cob_curve;

	ui.cob_color->addItem("Qt::white",3);
	ui.cob_color->addItem("Qt::black", 2);
	ui.cob_color->addItem("Qt::red", 7);
	ui.cob_color->addItem("Qt::darkRed", 13);
	ui.cob_color->addItem("Qt::green", 8);
	ui.cob_color->addItem("Qt::darkGreen",14);
	ui.cob_color->addItem("Qt::blue", 9);
	ui.cob_color->addItem("Qt::darkBlue", 15);
	ui.cob_color->addItem("Qt::cyan", 10);
	ui.cob_color->addItem("Qt::yellow", 12);

	ui.cob_linetype->addItem("Qt::SolidLine", 1);
	ui.cob_linetype->addItem("Qt::DashLine", 2);
	ui.cob_linetype->addItem("Qt::DotLine", 3);
	ui.cob_linetype->addItem("Qt::DashDotLine", 4);
	ui.cob_linetype->addItem("Qt::DashDotDotLine",5);
	ui.cob_linetype->addItem("Qt::CustomDashLine", 6);

	ui.cob_width->addItem("1-Pix", 1);
	ui.cob_width->addItem("2-Pix", 2);
	ui.cob_width->addItem("3-Pix", 3);
	ui.cob_width->addItem("4-Pix", 4);
	ui.cob_width->addItem("5-Pix", 5);
	ui.cob_width->addItem("6-Pix", 6);


	ui.cob_color->setCurrentIndex(0);
	ui.cob_linetype->setCurrentIndex(0);
	ui.cob_width->setCurrentIndex(0);
	connect(ui.pushButton, &QPushButton::clicked, this, [=]() {
		select_line_name = ui.cob_curve->currentText();
		color = ui.cob_color->currentData().toInt();
		width = ui.cob_width->currentData().toInt();
		type = ui.cob_linetype->currentData().toInt();
		emit btn_styleaffirm_clicked();
		});
}

Styletwidget::~Styletwidget()
{
}

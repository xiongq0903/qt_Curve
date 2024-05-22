#include "Operatewidget.h"

Operatewidget::Operatewidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	draw_widget = new QWidget();
	filter_way = "median_filter";
	listView = ui.listView;
	QStandardItemModel* model = new QStandardItemModel();
	ui.listView->setModel(model);

	draw_widget->setMaximumSize(QSize(200, 300));
	draw_widget->setWindowFlags(Qt::Dialog);
	draw_widget->setWindowTitle("自定义基线");
	QGridLayout* temp_widfet_layout = new QGridLayout(draw_widget);
	draw_widget->setLayout(temp_widfet_layout);
	QPushButton* btn_darw = new QPushButton(draw_widget);
	QPushButton* btn_drag = new QPushButton(draw_widget);
	QPushButton* btn_true = new QPushButton(draw_widget);
	name_edit = new QLineEdit(draw_widget);
	QLabel* lab_name = new QLabel(draw_widget);
	btn_darw->setText("绘制");
	btn_drag->setText("拖动");
	btn_true->setText("确定");
	lab_name->setText("基线名称:");
	temp_widfet_layout->addWidget(btn_darw, 0, 0, 1, 2);
	temp_widfet_layout->addWidget(lab_name, 1, 0);
	temp_widfet_layout->addWidget(name_edit, 1, 1);
	temp_widfet_layout->addWidget(btn_drag, 2, 0, 1, 2);
	temp_widfet_layout->addWidget(btn_true, 3, 0, 1, 2);
	connect(btn_darw, &QPushButton::clicked, this, [=]() {
		this->hide();
		draw_widget->hide();
		emit btn_draw_clicked();
		});

	connect(btn_drag, &QPushButton::clicked, this, [=]() {
		this->hide();
		draw_widget->hide();
		emit btn_drag_clicked();
		});

	connect(btn_true, &QPushButton::clicked, this, [=]() {
		draw_widget->hide();
		emit btn_true_clicked();
		});
	ui.insert_point->setEnabled(false);
	baseline_select = ui.baseline_select;
	ui.baseline_select->addItem("median-filter");
	ui.baseline_select->setCurrentIndex(10);
	connect(ui.baseline_select, &QComboBox::currentTextChanged, this, [=](QString text) {
		if (text == "median-filter") 
			ui.insert_point->setEnabled(true);
		else
			ui.insert_point->setEnabled(false);
		});
	connect(ui.btn_calculate, &QPushButton::clicked, this, [=]() {
		//基线类型
		filter_way = ui.baseline_select->currentText();
		insert_point = ui.insert_point->value();//内插点数
		emit btn_calculate_cliked();
		});
	connect(ui.btn_affirm, &QPushButton::clicked, this, [=]() {
		emit btn_affirm_clicked();
		});
	connect(ui.btn_clearlistview, &QPushButton::clicked, this, [=]() {
		emit btn_clearlistview_clicked();
		});
	connect(ui.btn_customize, &QPushButton::clicked, this, &Operatewidget::btn_customize_clicked);
	
}

void Operatewidget::add_QSpinBox(const QVector<QString>& line_name)
{
	while (QLayoutItem* item = ui.para_layout->takeAt(0))
	{
		if (QWidget* widget = item->widget())
			widget->deleteLater();
		delete item;
	}
	int index = 0;
	for (const QString& name : line_name) {
		QLabel *label = new QLabel(this);
		label->setText(name + "调整");
		label->setAlignment(Qt::AlignRight);
		ui.para_layout->addWidget(label, index, 0);
		QSpinBox* spin1 = new QSpinBox(this);
		my_spin.insert(name + "-adjust", spin1);
		ui.para_layout->addWidget(spin1, index, 1);
		QLabel *label2 = new QLabel(this);
		label2->setText(name + "增益");
		label2->setAlignment(Qt::AlignRight);
		ui.para_layout->addWidget(label2, index, 2);
		QSpinBox* spin2 = new QSpinBox(this);
		my_spin.insert(name + "-gain", spin2);
		ui.para_layout->addWidget(spin2, index, 3);
		index++;
	}

}

void Operatewidget::add_listview()
{
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.listView->model());
	model->clear();
	for (auto it = filter_result_series.begin(); it != filter_result_series.end(); it++) 
	{
		QStandardItem* Item = new QStandardItem();
		Item->setCheckable(true);
		Item->setCheckState(Qt::Checked);
		Item->setText(it.key());
		Item->setEditable(false);
		Item->setCheckState(Qt::Unchecked);
		model->appendRow(Item);
	}
	
}

QStringList Operatewidget::find_check_listview()
{
	QStringList check_line;
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.listView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem* temp_item = model->item(i);
		if (temp_item->checkState() == Qt::Checked)
			check_line.append(temp_item->text());
	}
	return std::move(check_line);
}

void Operatewidget::btn_customize_clicked()
{
	name_edit->setText("");
	draw_widget->show();

}

Operatewidget::~Operatewidget()
{
	for (auto& data : my_spin.toStdMap())
	{
		delete data.second;
	}
}

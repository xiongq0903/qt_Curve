#include "Lithologymapwidget.h"

Lithologymapwidget::Lithologymapwidget(QWidget *parent)
	: QWidget(parent)
{
	filename = new QLineEdit();
	btn_file_select = new QPushButton();
	btn_litmap_affirm = new QPushButton();
	my_layout = new QGridLayout();
	tableview = new QTableView();
	//my_bar = new QBarSeries();
	//my_barset = new QBarSet("");
	QLabel* label = new QLabel();
	this->setLayout(my_layout);
	label->setText("文件地址：");
	//label->setAlignment(Qt::AlignRight);
	btn_file_select->setText("选择文件");
	btn_litmap_affirm->setText("确定");
	tableview->setMinimumSize(400, 400);
	my_layout->addWidget(label, 0, 0, 1, 2);
	my_layout->addWidget(filename, 0, 1, 1, 4);
	my_layout->addWidget(btn_file_select, 1, 1, 1, 3);
	my_layout->addWidget(tableview, 0, 5, 6, 6);
	min = 0;
	max = 0;
	QStandardItemModel* model = new QStandardItemModel();
	tableview->setModel(model);

	connect(btn_file_select, &QPushButton::clicked, this, &Lithologymapwidget::btn_file_select_clicked);
	connect(btn_litmap_affirm, &QPushButton::clicked, this, [=]() {
		emit btn_litmap_affirm_clicked();
		});

}

void Lithologymapwidget::btn_file_select_clicked()
{
	file_path = QFileDialog::getOpenFileName(this, "choose open file", "", "*.txt");
	if (file_path.isEmpty())
	{
		QMessageBox::warning(this,
			tr("error"),
			tr("plese choose file!"),
			QMessageBox::Ok);
		return;
	}
	filename->setText(file_path);

	QFile data_litmap(file_path);
	QString dataline;
	QStringList data;
	rownums = 0;
	data_litmap.open(QIODevice::ReadOnly);
	if (!data_litmap.isOpen()) {
		QMessageBox::warning(this,
			tr("error"),
			tr("file open failed!"),
			QMessageBox::Ok);
		return;
	}
	if (data_litmap.size() != 0)
	{
		QTextStream in(&data_litmap);
		while (!in.atEnd()) {
			++rownums;
			dataline = in.readLine();
			data = dataline.split(QRegExp("\\s+"));
			if (data.size() > colnums) colnums = data.size();
			litmap_data.push_back(data);
		}
	}
	data_litmap.close();
	data2table();
	get_data_bar();
	add_btn();
}

void Lithologymapwidget::data2table()
{
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(tableview->model());
	model->clear();
	model->setColumnCount(colnums);
	model->setRowCount(rownums);
	for (int i = 0; i < rownums; i++)
	{
		QStringList& temp = litmap_data[i];
		for (int j = 0; j < temp.size(); j++) {
			model->setData(model->index(i, j), temp[j]);
		}
	}
}

void Lithologymapwidget::get_data_bar()
{
	min = litmap_data[1][0].toFloat();
	max = litmap_data[rownums - 1][1].toFloat();
	for (int i = 1; i < rownums; i++)
	{
		if (i + 1 > rownums) break;
		float dif = litmap_data[i][1].toFloat() - litmap_data[i][0].toFloat();
		const int& value = litmap_data[i][2].toInt();
		if (!btn_bar_map.contains(value))
		{
			QPushButton* btn = new QPushButton();
			btn->setFixedSize(100, 50);
			btn->setText(QString::number(value));
			btn->setStyleSheet("background-color:#00ffff;");
			bar_color.insert(btn->text().toInt(), "#00ffff");
			connect(btn, &QPushButton::clicked, this, [=]() {
				QColor color = QColorDialog::getColor(Qt::red, this, tr("颜色对话框"),QColorDialog::ShowAlphaChannel);
				btn->setStyleSheet("background-color:" + color.name() + ";");
				bar_color[btn->text().toInt()] = color.name();
				});
			btn_bar_map.insert(value, btn);
		}
		data_rect.push_back(QPointF(litmap_data[i][1].toFloat() - litmap_data[i][0].toFloat(), litmap_data[i][2].toFloat()));
		
	}
}

void Lithologymapwidget::add_btn()
{
	int i = 0, j = 0;
	for (auto it = btn_bar_map.begin(); it != btn_bar_map.end(); it++)
	{
		QPushButton* temp_btn = it.value();
		my_layout->addWidget(temp_btn, i / 5 + 2, i % 5, 1, 1);
		++i;
	}
	my_layout->addWidget(btn_litmap_affirm, --i / 5 + 3, 1, 1, 3);
}

Lithologymapwidget::~Lithologymapwidget()
{}

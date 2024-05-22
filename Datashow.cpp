#include "Datashow.h"

Datashow::Datashow()
{
	ui.setupUi(this);
	data_object = std::make_shared<Data>();
	title_line = 0;
	jump_line = 0;
	end_line = 0;
	x_line = 0;
	invalid_value = 0;
	ui.btn_begin->setEnabled(false);
	ui.btn_keeprevise->setEnabled(false);
	ui.tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
	QStandardItemModel* model = new QStandardItemModel(this);
	ui.tableView->setModel(model);
	connect(ui.tableView, &QTableView::doubleClicked, this, [=](QModelIndex topLeft) {
		QModelIndex* temp_data = new QModelIndex(topLeft);
		data_changed.push_back(std::move(temp_data));
		ui.btn_keeprevise->setEnabled(true);
		});
	connect(ui.btn_confirm, &QPushButton::clicked, this, &Datashow::btn_confirm_cliked);
	connect(ui.btn_begin, &QPushButton::clicked, [=](){
		if (!get_axis()) {
			QMessageBox::warning(this,
				tr("error"),
				tr("please Select axis!"),
				QMessageBox::Ok);
			return;
		}
		emit btn_begin();
		});
	connect(ui.btn_reset, &QPushButton::clicked, this, &Datashow::btn_reset_clicked);

	connect(ui.btn_keeprevise, &QPushButton::clicked, this, [=]() {
		int x, y;
		for (int i = 0; i < data_changed.size(); i++)
		{
			x = data_changed[i]->row() + jump_line;
			y = data_changed[i]->column();
			data_object->alldata[x][y] = ui.tableView->model()->data(*data_changed[i]).toString();
		}
		for (auto& data : data_changed)
		{
			delete data;
		}
		data_changed.clear();
		});
}

Datashow::~Datashow()
{
	for (auto& data : data_changed)
	{
		delete data;
	}
	for (auto& data : my_check)
	{
		delete data;
	}
	for (auto& data : my_flip_check)
	{
		delete data;
	}
}

void Datashow::clearalldata()
{
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.tableView->model());
	model->clear();
	data_object->clearalldata();
	data_changed.clear();
	my_check.clear();
	my_flip_check.clear();
	data_changed.clear();
}

void Datashow::showdata()
{
	//double time_Start = (double)clock();//显示数据加载时间
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.tableView->model());
	model->clear();
	model->setColumnCount(data_object->Colnums);
	model->setRowCount(end_line - jump_line);
	for (int i = 0; i < end_line - jump_line; i++) {
		QStringList& temp = data_object->alldata[i + jump_line];
		for (int j = 0; j < temp.size(); j++) {
			model->setData(model->index(i, j), temp[j]);
		}
	}
	//double end_Start = (double)clock();
	//qDebug() << "openfile time" << (end_Start - time_Start) / 1000 << 's';
}

bool Datashow::get_axis()
{
	//获取勾选的数据轴信息
	int index = 0;
	for (auto check : my_check)
	{
		++index;
		if (index == x_line)
			++index;
		if (check->isChecked())
		{
			data_object->chart_map.insert(check->text(), index);
		}
	}
	int index_flip = 0;
	for (auto check : my_flip_check)
	{
		++index_flip;
		if (index_flip == x_line)
			++index_flip;
		if (check->isChecked())
		{
			data_object->flip_chart_map.insert(check->text() + "-R", index_flip);
		}
	}
	if (data_object->chart_map.isEmpty() && data_object->flip_chart_map.isEmpty())
		return false;
	return true;
	
}

void Datashow::data2table()
{
	end_line = data_object->Rownums;
	showdata();
}

void Datashow::clear_checkbox()
{
	while (QLayoutItem* item = ui.layout_show->takeAt(1))
	{
		if (QWidget* widget = item->widget())
			widget->deleteLater();
		delete item;
	}
	while (QLayoutItem* item = ui.layout_reverse->takeAt(1))
	{
		if (QWidget* widget = item->widget())
			widget->deleteLater();
		delete item;
	}
	my_check.clear();
	my_flip_check.clear();
	data_object->chart_map.clear();
	data_object->flip_chart_map.clear();
}

void Datashow::btn_confirm_cliked()
{
	title_line = ui.title_line->text().toInt();
	end_line = ui.end_line->text().toInt();
	jump_line = ui.jump_line->text().toInt();
	x_line = ui.x_line->text().toInt();
	invalid_value = ui.invalid_value->text().toFloat();

	ui.title_line->setEnabled(false);
	ui.jump_line->setEnabled(false);
	ui.x_line->setEnabled(false);
	ui.end_line->setEnabled(false);
	ui.invalid_value->setEnabled(false);

	int y_max = data_object->Rownums;
	int x_max = data_object->Colnums;
	if (title_line > y_max || jump_line > y_max || end_line > y_max || x_line > x_max || x_line <=0) {
		QMessageBox::warning(this,
			tr("error"),
			tr("参数错误!"),
			QMessageBox::Ok);
		btn_reset_clicked();
		return;
	}

	data_object->endRow = end_line;
	data_object->x_line = x_line;
	data_object->beginRow = jump_line;
	data_object->showRownums = end_line - jump_line;
	data_object->invalid_value = invalid_value;

	QString title_x = data_object->alldata[title_line - 1][x_line - 1];
	data_object->x_line_name = title_x;
	
	clear_checkbox();
	
	for (int i = 0; i < data_object->alldata[title_line-1].size(); i++) {
		if (i == x_line - 1)
			continue;
		QCheckBox* check = new QCheckBox(this);
		check->setText(data_object->alldata[title_line-1][i]);
		my_check.push_back(check);
		ui.layout_show->addWidget(check);
	}
	for (int i = 0; i < data_object->alldata[title_line - 1].size(); i++) {
		if (i == x_line - 1)
			continue;
		QCheckBox* check = new QCheckBox(this);
		check->setText(data_object->alldata[title_line - 1][i]);
		my_flip_check.push_back(check);
		ui.layout_reverse->addWidget(check);
	}

	//存储x轴的数据
	
	if (!data_object->Nomdata.contains(title_x))
	{
		QVector<float> temp_data;
		for (int i = data_object->beginRow; i < data_object->endRow; i++)
		{
			temp_data.push_back(data_object->alldata[i][x_line-1].toFloat());
		}
		data_object->Nomdata.insert(title_x, std::move(temp_data));
	}
	showdata();
	ui.btn_begin->setEnabled(true);
}

void Datashow::btn_reset_clicked()
{
	ui.title_line->clear();
	ui.jump_line->clear();
	ui.x_line->clear();
	ui.end_line->clear();
	ui.invalid_value->clear();
	
	ui.title_line->setEnabled(true);
	ui.jump_line->setEnabled(true);
	ui.x_line->setEnabled(true);
	ui.end_line->setEnabled(true);
	ui.invalid_value->setEnabled(true);
	ui.btn_begin->setEnabled(false);

	title_line = 0;
	end_line = data_object->Rownums;
	jump_line = 0;
	invalid_value = 0;
	my_check.clear();
	my_flip_check.clear();
	clear_checkbox();
	showdata();
}





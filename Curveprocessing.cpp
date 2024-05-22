#include "Curveprocessing.h"


Curveprocessing::Curveprocessing(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //初始化
    data_widget = std::make_unique<Datashow>();
    style_widget = std::make_unique<Styletwidget>();
    operate_widget = std::make_unique<Operatewidget>();
    clear_widget = std::make_unique<Clearwidget>();
    litmap_widget = std::make_unique<Lithologymapwidget>();
    data_widget->setWindowTitle("编辑设置");
    operate_widget->setWindowTitle("操作");
    //清除曲线窗口设置
    
    //ui.my_chart->setChart(ui.my_chart->chart_data);
    ui.my_chart->setRenderHint(QPainter::Antialiasing);//抗锯齿
    ui.btn_edit->setEnabled(false);
    ui.btn_clear->setEnabled(false);
    ui.btn_operate->setEnabled(false);
    ui.btn_setstyle->setEnabled(false);
    //ui.btn_lithologymap->setEnabled(false);
    ui.btn_keepdata->setEnabled(false);
    ui.btn_keeppara->setEnabled(false);
    fileDir = QCoreApplication::applicationDirPath();
    filePath = QCoreApplication::applicationDirPath();
    x_line_last = -1;

    connect(ui.btn_openfile, &QPushButton::clicked, this, &Curveprocessing::btn_openfile_clicked);
    connect(ui.btn_edit, &QPushButton::clicked, this, &Curveprocessing::btn_edit_clicked);
    connect(data_widget.get(), &Datashow::btn_begin, this, &Curveprocessing::btn_begin_clicked);
    connect(ui.my_chart, &myqchartview::sendpos, this, &Curveprocessing::pos_show);
    connect(ui.btn_operate, &QPushButton::clicked, this, &Curveprocessing::btn_operate_clicked);
    connect(operate_widget.get(), &Operatewidget::btn_calculate_cliked, this, &Curveprocessing::btn_calculate_cliked);
    connect(operate_widget.get(), &Operatewidget::btn_affirm_clicked, this, &Curveprocessing::btn_affirm_clicked);
    connect(ui.btn_clear, &QPushButton::clicked, this, &Curveprocessing::btn_clear_clicked);
    connect(operate_widget.get(), &Operatewidget::btn_draw_clicked, this, &Curveprocessing::btn_draw_clicked);
    connect(operate_widget.get(), &Operatewidget::btn_drag_clicked, this, &Curveprocessing::btn_drag_clicked);
    connect(operate_widget.get(), &Operatewidget::btn_true_clicked, this, &Curveprocessing::btn_true_clicked);
    connect(ui.btn_keeppara, &QPushButton::clicked, this, &Curveprocessing::btn_keeppara_clicked);
    connect(ui.btn_keepdata, &QPushButton::clicked, this, &Curveprocessing::btn_keepdata_clicked);
    connect(ui.btn_setstyle, &QPushButton::clicked, this, &Curveprocessing::btn_setstyle_clicked);
    connect(clear_widget.get(), &Clearwidget::btn_clear_cw_clicked, this, &Curveprocessing::btn_clear_cw_clicked);
    connect(clear_widget.get(), &Clearwidget::btn_clearall_cw_clicked, this, &Curveprocessing::btn_clearall_cw_clicked);
    connect(style_widget.get(), &Styletwidget::btn_styleaffirm_clicked, this, &Curveprocessing::btn_styleaffirm_clicked);
    connect(operate_widget.get(), &Operatewidget::btn_clearlistview_clicked, this, &Curveprocessing::btn_clearlistview_clicked);
    connect(ui.btn_lithologymap, &QPushButton::clicked, this, &Curveprocessing::btn_lithologymap_clicked);
    connect(litmap_widget.get(), &Lithologymapwidget::btn_litmap_affirm_clicked, this, &Curveprocessing::btn_litmap_affirm_clicked);
    this->setMouseTracking(true);
    ui.centralWidget->setMouseTracking(true);
    ui.my_chart->setMouseTracking(true);
    
    
}

Curveprocessing::~Curveprocessing()
{
    for (auto& series : series_set.toStdMap())
    {
        delete series.second;
    }
}

void Curveprocessing::mouseMoveEvent(QMouseEvent* event)
{
    //setMouseTracking(true);
    ui.x_global->setText(QString::number(event->x()));
    ui.y_global->setText(QString::number(event->y()));
    //ui.y_global->setText(QString(event->y()));
}

void Curveprocessing::btn_openfile_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, "choose open file", "","*.txt");
    if (filePath.isEmpty())
    {
        QMessageBox::warning(this,
            tr("error"),
            tr("plese choose file!"),
            QMessageBox::Ok);
        return;
    }
    Curveprocessing::read_data(filePath);
}

void Curveprocessing::btn_edit_clicked()
{
    data_widget->show();
}

void Curveprocessing::btn_operate_clicked()
{
    operate_widget->show();
    //增益参数
    std::shared_ptr<Data> my_data(data_widget->data_object);
    QVector<QString> line_name;
    for (auto it = my_data->chart_map.begin(); it != my_data->chart_map.end(); ++it) {
        line_name.push_back(it.key());
    }
    for (auto it = my_data->flip_chart_map.begin(); it != my_data->flip_chart_map.end(); ++it) {
        line_name.push_back(it.key());
    }
    operate_widget->add_QSpinBox(std::move(line_name));
    

}

void Curveprocessing::read_data(const QString& filepath)
{
    std::shared_ptr<Data> my_data(data_widget->data_object);
    if (my_data->filepath == filepath) {
        data_widget->show();
        return;
    }//打开重复文件则直接退出
    data_widget->btn_reset_clicked();
    data_widget->clearalldata();
    QFile datafile(filepath);
    QString linedata;
    QStringList data;
    int colnums = 0;
    int rownums = 0;
    datafile.open(QIODevice::ReadOnly);
    if (!datafile.isOpen()) {
        QMessageBox::warning(this,
            tr("error"),
            tr("file open failed!"),
            QMessageBox::Ok);
        return;
    }
    if (datafile.size() != 0) {
        QTextStream in(&datafile);
        while (!in.atEnd())//一行一行一直读，直至读取失败
        {
            ++rownums;
            linedata = in.readLine();//读取一行存到data里
            data = linedata.split(QRegExp("\\s+"));
            data.removeAll("");
            if (data.size() > colnums)
                colnums = data.size();
            my_data->alldata.push_back(std::move(data));
            data.clear();//清空，便于存放下一行数据
        }
        datafile.close();
        
    }
    my_data->Colnums = std::move(colnums);
    my_data->Rownums = std::move(rownums);
    my_data->filepath = filepath;
    data_widget->data2table();
    data_widget->show();
}

Max_Min Curveprocessing::get_Max_Min(const int& Col)
{
    std::shared_ptr<Data> my_data(data_widget->data_object);
    int& begin = my_data->beginRow;
    int& end = my_data->endRow;

    float min = my_data->alldata[begin][Col].toFloat();
    float max = min;
    float& invalid_value = my_data->invalid_value;
    for (int i = begin; i < end; ++i) {
        if (my_data->alldata[i][Col].toFloat() == invalid_value)
            continue;
        if (min > my_data->alldata[i][Col].toFloat())
            min = my_data->alldata[i][Col].toFloat();
        if (max < my_data->alldata[i][Col].toFloat())
            max = my_data->alldata[i][Col].toFloat();
    }
    return std::move(Max_Min({ min, max, max - min }));
}

void Curveprocessing::btn_begin_clicked()
{
    data_widget->hide();
    ui.btn_edit->setEnabled(true);
    ui.btn_operate->setEnabled(true);
    ui.btn_clear->setEnabled(true);
    ui.btn_setstyle->setEnabled(true);
    ui.btn_keepdata->setEnabled(true);
    ui.btn_keeppara->setEnabled(true);
    //绘制曲线
    std::shared_ptr<Data> my_data(data_widget->data_object);
    int& begin = my_data->beginRow;
    int& end = my_data->endRow;
    int& x_line = my_data->x_line;
    float& invalid_value = my_data->invalid_value;
    QString& x_line_name = my_data->x_line_name;

    //存储x轴最大最小值
    if (x_line_last != x_line)
    {
        Max_Min mm_x = get_Max_Min(x_line - 1);
        ui.my_chart->min = std::move(mm_x.min);
        ui.my_chart->dif = std::move(mm_x.dif);
        x_line_last = x_line;
    }
    for (auto iter = my_data->chart_map.begin(); iter != my_data->chart_map.end(); ++iter) {

        if (series_set.contains(iter.key()))
            continue;
        QLineSeries *series = new QLineSeries();
        series->setUseOpenGL(true);
        int& y_line = iter.value();
        //归一化处理
        Max_Min mm = get_Max_Min(y_line-1);
        QVector<float> temp_y;
        for (int j = begin; j < end; ++j) {
            float y = my_data->alldata[j][y_line - 1].toFloat();
            if (y == invalid_value)
            {
                y = my_data->alldata[j - 1][y_line - 1].toFloat();
            }
            const float& x = my_data->Nomdata.value(x_line_name)[j - begin];
            series->append(x, (y - mm.min)/mm.dif);
            temp_y.push_back((y - mm.min) / mm.dif);
        }
        my_data->Nomdata.insert(iter.key(), std::move(temp_y));//保存归一化数据
        series->setName(iter.key());
        ui.my_chart->chart_data->addSeries(series);
        series_set.insert(iter.key(), series);
    }
    for (auto iter = my_data->flip_chart_map.begin(); iter != my_data->flip_chart_map.end(); ++iter) {
        if (series_set.contains(iter.key()))
            continue;
        QLineSeries* series = new QLineSeries();
        series->setUseOpenGL(true);
        int& y_line = iter.value();
        Max_Min mm = get_Max_Min(y_line - 1);//归一化处理
        QVector<float> temp_y;
        for (int j = begin; j < end; ++j) {
            float y = my_data->alldata[j][y_line - 1].toFloat();
            if (y == invalid_value)
            {
                y = my_data->alldata[j - 1][y_line - 1].toFloat();
            }
            const float& x = my_data->Nomdata.value(x_line_name)[j - begin];
            series->append(x, -(y - mm.min) / mm.dif + 1);
            temp_y.push_back(-(y - mm.min) / mm.dif + 1);
        }
        my_data->Nomdata.insert(iter.key(), std::move(temp_y));//保存归一化数据
        series->setName(iter.key());
        ui.my_chart->chart_data->addSeries(series);
        series_set.insert(iter.key(), series);
    }
    ui.my_chart->chart_data->createDefaultAxes();// （没有这个就不显示）基于已添加到图表的 series 来创建默认的坐标轴
}

void Curveprocessing::btn_calculate_cliked()
{
    std::shared_ptr<Data> my_data(data_widget->data_object);
    int ins_poi = operate_widget->insert_point;
    QString filter_way = operate_widget->baseline_select->currentText();
    my_data->filter_way = std::move(filter_way);
    my_data->insert_point = std::move(ins_poi);
    get_gain_para();//获取设定的增益参数
    my_data->curve_processing();//基线计算
    my_data->baseline_correction();//基线校正
    my_data->gain_process();//增益曲线

    //绘制曲线但不显示
    const QVector<float>& x_temp_data = my_data->Nomdata.value(my_data->x_line_name);
    int size = x_temp_data.size();
    for (auto it = my_data->filter_result.begin(); it != my_data->filter_result.end(); ++it)
    {
        if (it.key() == my_data->x_line_name)
            continue;
        QVector<float>& temp_data = it.value();
        QLineSeries* series = new QLineSeries();
        series->setUseOpenGL(true);
        for (int j = 0; j < size; ++j) {
            float x = x_temp_data[j];
            float y = temp_data[j];
            series->append(std::move(x), std::move(y));
        }
        series->setName(it.key());
        operate_widget->filter_result_series.insert(it.key(), std::move(series));//加入到处理结果
        //series_set.insert(it.key(), series);
    }
    operate_widget->add_listview();//加入到操作页面
}

void Curveprocessing::pos_show(const QPoint& pos)
{
    QPoint pos_my_chart = ui.my_chart->pos();
    ui.x_widget->setText(QString::number(pos.x()));
    ui.y_widget->setText(QString::number(pos.y()));
    ui.x_global->setText(QString::number(pos.x() + pos_my_chart.x()));
    ui.y_global->setText(QString::number(pos.y() + pos_my_chart.y()));
}

void Curveprocessing::btn_affirm_clicked()
{
    operate_widget->hide();
    QStringList check_name = operate_widget->find_check_listview();
    for (int i = 0; i < check_name.size(); ++i)
    {
        QLineSeries* series = operate_widget->filter_result_series.value(check_name[i]);
        series_set.insert(check_name[i], series);
        ui.my_chart->chart_data->addSeries(series);
    }
    ui.my_chart->chart_data->createDefaultAxes();
}

void Curveprocessing::get_gain_para()
{
    std::shared_ptr<Data> my_data(data_widget->data_object);
    my_data->gain_para.clear();
    QSpinBox* temp_box = nullptr;
    for (auto it = my_data->chart_map.begin(); it != my_data->chart_map.end(); ++it)
    {
        QVector<int> temp;
        temp_box = operate_widget->my_spin.value(it.key() + "-adjust");
        temp.push_back(temp_box->value());
        temp_box = operate_widget->my_spin.value(it.key() + "-gain");
        temp.push_back(temp_box->value());
        my_data->gain_para.insert(it.key(), std::move(temp));
    }
}

void Curveprocessing::btn_clear_clicked()
{
    clear_widget->show();
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(clear_widget->cw_listview->model());
    model->clear();
    std::shared_ptr<Data> my_data(data_widget->data_object);
    for (auto it = series_set.begin(); it != series_set.end(); ++it)
    {
        QStandardItem* Item = new QStandardItem();
        Item->setCheckable(true);
        Item->setCheckState(Qt::Checked);
        Item->setText(it.key());
        Item->setEditable(false);
        model->appendRow(Item);
    }
}

void Curveprocessing::btn_clear_cw_clicked()
{
    std::shared_ptr<Data> my_data(data_widget->data_object);
    QStandardItemModel* temp_model = qobject_cast<QStandardItemModel*>(clear_widget->cw_listview->model());
    int rowcount = temp_model->rowCount();
    for (int i = 0;; ++i)
    {
        if (i >= rowcount)
            break;
        QStandardItem* temp_item = temp_model->item(i);
        if (temp_item->checkState() == Qt::Checked)
        {
            const QString& line_name = temp_item->text();
            auto item = series_set.find(line_name);
            QLineSeries* temp_series = item.value();
            ui.my_chart->chart_data->removeSeries(temp_series);
            series_set.erase(item);
            my_data->chart_map.remove(line_name);
            my_data->flip_chart_map.remove(line_name);
            //my_data->filter_result.remove(line_name);
            temp_model->removeRow(i--);
            --rowcount;
        }
    }
    clear_widget->close();
}

void Curveprocessing::btn_clearall_cw_clicked()
{
    std::shared_ptr<Data> my_data(data_widget->data_object);
    for (auto it = series_set.begin(); it != series_set.end(); it++)
    {
        ui.my_chart->chart_data->removeSeries(it.value());
    }
    series_set.clear();
    my_data->chart_map.clear();
    my_data->flip_chart_map.clear();
    clear_widget->close();
}

void Curveprocessing::btn_clearlistview_clicked()
{
    std::shared_ptr<Data> my_data(data_widget->data_object);
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(operate_widget->listView->model());
    model->clear();
    operate_widget->filter_result_series.clear();
    my_data->filter_result.clear();
}

void Curveprocessing::btn_draw_clicked()
{
    ui.my_chart->draw_begin = true;
    //绘制时需要关闭opengl渲染，否则无法点击曲线
    for (auto& series : series_set.toStdMap())
    {
        series.second->setUseOpenGL(false);
    }
    ui.my_chart->create_draw_line();

    //右键结束绘制
    connect(ui.my_chart, &myqchartview::stop_draw, this, [=]() {
        //将曲线加入到集合
        //QList<QPointF> temp_list_point = ui.my_chart->draw_baseline->points();
        if (ui.my_chart->draw_baseline->points().size() <= 1)
        {
            QMessageBox::warning(this,
                tr("error"),
                tr("请正确绘制基线!"),
                QMessageBox::Ok);
            ui.my_chart->draw_baseline->clear();
            ui.my_chart->draw_begin = true;
            ui.my_chart->baselinenum--;
            return;
        }
        operate_widget->show();
        operate_widget->draw_widget->show();
        operate_widget->draw_name = ui.my_chart->draw_name;
        operate_widget->name_edit->setText(ui.my_chart->draw_name);
        series_set.insert(ui.my_chart->draw_name, ui.my_chart->draw_baseline);
        for (auto& series : series_set.toStdMap())
        {
            series.second->setUseOpenGL(true);
        }
        });

}

void Curveprocessing::btn_drag_clicked()
{
    ui.my_chart->drag_begin = true;
    QString& baseline_name = operate_widget->draw_name;
    ui.my_chart->series_points = series_set.value(baseline_name)->points();
    float& min = ui.my_chart->min;
    float& dif = ui.my_chart->dif;
    for (int i = 0; i < ui.my_chart->series_points.size(); ++i)
    {
        const float& x = ui.my_chart->series_points[i].x();
        ui.my_chart->series_points[i].setX((x - min) / dif);
    }
    ui.my_chart->draw_baseline = series_set.value(baseline_name);
    connect(ui.my_chart, &myqchartview::stop_drag, this, [=]() {
        operate_widget->show();
        operate_widget->draw_widget->show();
        ui.my_chart->drag_begin = false;
        });

    //1.左键摁下，计算最接近的点
    //2.设置该点随鼠标移动
    //3.松开左键结束
}

void Curveprocessing::btn_true_clicked()
{
    const QString& baseline_name = operate_widget->name_edit->text();
    QLineSeries* temp = series_set.value(operate_widget->draw_name);
    series_set.remove(operate_widget->draw_name);
    ui.my_chart->chart_data->removeSeries(temp);

    std::shared_ptr<Data> my_data(data_widget->data_object);
    operate_widget->baseline_select->addItem(baseline_name);

    //将Qlineseries转化成QVector<float>并且进行线性插值
    const QList<QPointF>& temp_list = temp->points();
    QVector<float> temp_vector;
    const QVector<float>& temp_x = my_data->Nomdata.value(my_data->x_line_name);
    int index = 0;
    float x, y;
    float x1, y1, x2, y2;
    for (int i = 0; i < my_data->showRownums; ++i)
    {
        x = temp_x[i];
        if (x > temp_list[index + 1].x())
        {
            if (index != temp_list.size() - 2)
                ++index;
        } 
        x1 = temp_list[index].x();
        y1 = temp_list[index].y();
        x2 = temp_list[index + 1].x();
        y2 = temp_list[index + 1].y();
        y = (y2 - y1) * (x - x1) / (x2 - x1) + y1;
        temp_vector.push_back(std::move(y));
    }
    my_data->filter_result.insert(baseline_name, std::move(temp_vector));
    operate_widget->filter_result_series.insert(baseline_name, temp);

}

void Curveprocessing::btn_keeppara_clicked()
{
    QString para_file_name = filePath;
    para_file_name.insert(filePath.size() - 4, "-para");
    const QString& save_fileName = QFileDialog::getSaveFileName(this,
        tr("保存参数文件"),
        para_file_name,
        tr("Parameter Files (*.txt)"));
    QFile datafile(save_fileName);
    datafile.open(QIODevice::WriteOnly);
    if (!datafile.isOpen()) {
        QMessageBox::warning(this,
            tr("error"),
            tr("file save failed!"),
            QMessageBox::Ok);
        return;
    }
    QTextStream out(&datafile);
    std::shared_ptr<Data> my_data(data_widget->data_object);
    QString adjust, gain;
    for (auto it = my_data->chart_map.begin(); it != my_data->chart_map.end(); it++)
    {
        adjust = QString::number(my_data->gain_para.value(it.key())[0]);
        gain = QString::number(my_data->gain_para.value(it.key())[1]);
        out << (it.key() + "-adjust:" + adjust + "   " + it.key() + "-gain:" + gain) << Qt::endl;
    }
    for (auto it = my_data->flip_chart_map.begin(); it != my_data->flip_chart_map.end(); it++)
    {
        adjust = QString::number(my_data->gain_para.value(it.key())[0]);
        gain = QString::number(my_data->gain_para.value(it.key())[1]);
        out << (it.key() + "-adjust:" + adjust + "   " + it.key() + "-gain:" + gain) << Qt::endl;
    }
}

void Curveprocessing::btn_keepdata_clicked()
{
    QString para_file_name = filePath;
    para_file_name.insert(filePath.size() - 4, "-data");
    qDebug() << para_file_name;
    QString save_fileName = QFileDialog::getSaveFileName(this,
        tr("保存数据文件"),
        para_file_name,
        tr("Parameter Files (*.txt)"));
    QFile datafile(save_fileName);
    datafile.open(QIODevice::WriteOnly);
    if (!datafile.isOpen()) {
        QMessageBox::warning(this,
            tr("error"),
            tr("file save failed!"),
            QMessageBox::Ok);
        return;
    }
    //三部分数据 x轴数据 原始曲线数据（归一化）处理后曲线数据
    QTextStream out(&datafile);
    std::shared_ptr<Data> my_data(data_widget->data_object);
    QStringList filter_line_name_list;
    QStringList check_line_name;
    for (auto it = my_data->Nomdata.begin(); it != my_data->Nomdata.end(); it++)
    {
        out << it.key() << "     ";
        if (it.key() != my_data->x_line_name)
            check_line_name.push_back(it.key());
    }
    for (auto it = my_data->filter_result.begin(); it != my_data->filter_result.end(); it++)
    {
        out << it.key()<<"     ";
        filter_line_name_list.push_back(it.key());
    }
    out << Qt::endl;
    for (int i = 0; i < my_data->showRownums; i++) {
        out << my_data->Nomdata.value(my_data->x_line_name)[i] << "     ";//第一部分
        for (int j = 0; j < check_line_name.size(); j++)
        {
            out << my_data->Nomdata.value(check_line_name[j])[i] << "      ";//第二部分
        }
        for (int k = 0; k < filter_line_name_list.size(); k++)
        {
            out << my_data->filter_result.value(filter_line_name_list[k])[i]<<"      ";//第三部分
        }
        out << Qt::endl;
    }
}

void Curveprocessing::btn_setstyle_clicked()
{
    style_widget->select_curve->clear();
    for (auto it = series_set.begin(); it != series_set.end(); it++)
    {
        style_widget->select_curve->addItem(it.key());
    }
    style_widget->show();
}

void Curveprocessing::btn_styleaffirm_clicked()
{
    //style_widget->close();
    QLineSeries* temp_series = series_set.value(style_widget->select_line_name);
    
    temp_series->setPen(QPen(QColor(Qt::GlobalColor(style_widget->color)), style_widget->width, Qt::PenStyle(style_widget->type)));
}

void Curveprocessing::btn_lithologymap_clicked()
{
    const QString& text = ui.btn_lithologymap->text();
    if ("关闭岩性图" == text)
    {
        ui.my_chart->bar_begin = false;
        ui.my_chart->update();
        ui.btn_lithologymap->setText("岩性图");
        QMessageBox::information(this,
            tr("提示"),
            tr("岩性图已关闭"),
            QMessageBox::Ok);
    }
    else
    {
        ui.btn_lithologymap->setText("关闭岩性图");
        litmap_widget->show();
    }
}

void Curveprocessing::btn_litmap_affirm_clicked()
{
    litmap_widget->close();
    ui.my_chart->data_rect = litmap_widget->data_rect;
    ui.my_chart->bar_color = litmap_widget->bar_color;
    ui.my_chart->min_bar = litmap_widget->min;
    ui.my_chart->max_bar = litmap_widget->max;
    ui.my_chart->bar_begin = true;
}



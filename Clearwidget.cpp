#include "Clearwidget.h"

Clearwidget::Clearwidget(QWidget* parent)
	: QWidget(parent)
{
    this->setMaximumSize(QSize(400, 400));
    this->setWindowTitle("清除曲线");
    QVBoxLayout* cw_layout = new QVBoxLayout();
    this->setLayout(cw_layout);
    cw_listview = new QListView();
    btn_clear_cw = new QPushButton();
    btn_clearall_cw = new QPushButton();
    btn_clear_cw->setText("清除");
    btn_clearall_cw->setText("全部清除");
    cw_layout->addWidget(cw_listview);
    cw_layout->addWidget(btn_clear_cw);
    cw_layout->addWidget(btn_clearall_cw);
    QStandardItemModel* model = new QStandardItemModel();
    cw_listview->setModel(model);

    connect(btn_clear_cw, &QPushButton::clicked, this, [=]() {
        emit btn_clear_cw_clicked();
        });

    connect(btn_clearall_cw, &QPushButton::clicked, this, [=]() {
        emit btn_clearall_cw_clicked();
        });
}

Clearwidget::~Clearwidget()
{
}

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Curveprocessing.h"
#include "Datashow.h"
#include "myqchartview.h"

#include <qfiledialog.h>
#include <qstring.h>
#include <qdebug.h>
#include <string.h>
#include <qmessagebox.h>
#include <qtablewidget.h>
#include <qfile.h>
#include <qregexp.h>
#include <QtCharts>
#include <memory>
#include "Operatewidget.h"
#include "Styletwidget.h"
#include "Clearwidget.h"
#include "Lithologymapwidget.h"

#pragma execution_character_set("utf-8")

typedef struct Max_Min {
    float min;
    float max;
    float dif;
}Max_Min;


class Curveprocessing : public QMainWindow
{
    Q_OBJECT

public:
    Curveprocessing(QWidget *parent = nullptr);
    ~Curveprocessing();
    QMultiMap<QString, QLineSeries*> series_set;//已经存在的曲线

private:
    int x_line_last;
    QString fileDir;
    QString filePath;
    std::unique_ptr<Datashow> data_widget;
    std::unique_ptr<Styletwidget> style_widget;
    std::unique_ptr<Operatewidget> operate_widget;
    std::unique_ptr<Clearwidget> clear_widget;
    std::unique_ptr<Lithologymapwidget> litmap_widget;
    void read_data(const QString& filepath);
    Max_Min get_Max_Min(const int& Col);

private slots:
    void btn_openfile_clicked();
    void btn_edit_clicked();
    void btn_begin_clicked();
    void btn_operate_clicked();
    void btn_calculate_cliked();
    void btn_affirm_clicked();
    void btn_clear_clicked();
    void btn_draw_clicked();
    void btn_drag_clicked();
    void btn_true_clicked();
    void btn_keeppara_clicked();
    void btn_keepdata_clicked();
    void btn_setstyle_clicked();
    void btn_styleaffirm_clicked();
    void btn_clear_cw_clicked();
    void btn_clearall_cw_clicked();
    void btn_clearlistview_clicked();
    void btn_lithologymap_clicked();
    void btn_litmap_affirm_clicked();
    void pos_show(const QPoint& pos);
    void get_gain_para();

protected:
    void mouseMoveEvent(QMouseEvent* event)override;
    

private:
    Ui::CurveprocessingClass ui;
};

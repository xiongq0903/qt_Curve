#pragma once

#include <QChartView>
#include <qdebug.h>
#include <qlineseries.h>
#include <qvalueaxis.h>
#include <qpainter.h>

QT_CHARTS_USE_NAMESPACE

class myqchartview  : public QChartView
{
	Q_OBJECT

public:
	myqchartview(QWidget *parent);
	~myqchartview();
	//std::unique_ptr<QVector<QRect>> data_rect;
	bool draw_begin = false;//绘制开始
	bool drag_begin = false;//拖拽开始
	bool bar_begin = false;//开始绘制岩性图
	int baselinenum = 1;
	int index;
	float min;
	float dif;
	QString draw_name;
	QChart* chart_data;
	QList<QPointF> series_points;
	QLineSeries* draw_baseline;
	void create_draw_line();
	void begin_bar();

	//std::unique_ptr<QVector<QPointF>> data_rect_ptr;
	//std::unique_ptr<QMap<int, QString>> bar_color_ptr;
	QVector<QPointF> data_rect;
	QMap<int, QString> bar_color;
	float min_bar;
	float max_bar;
	

protected:
	 void mouseMoveEvent(QMouseEvent* event)override;
	 void mousePressEvent(QMouseEvent* event)override;
	 void mouseReleaseEvent(QMouseEvent* event)override;
	 void enterEvent(QEvent* event)override;
	 void leaveEvent(QEvent* event)override;
	 void paintEvent(QPaintEvent* event)override;

private:
	inline float distance(const QPointF& x, const QPointF& y);

signals:
	void stop_draw();
	void stop_drag();
	void close_point_index(int index);
	void sendpos(QPoint nPos);
};

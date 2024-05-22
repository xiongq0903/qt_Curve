#include "myqchartview.h"

myqchartview::myqchartview(QWidget *parent)
	: QChartView(parent)
{
	chart_data = new QChart();
	index = -1;
	this->setMouseTracking(true);
	
	this->setChart(chart_data);
	//connect(this, &myqchartview::close_point_index, this, &myqchartview::point_follow);
	
}

myqchartview::~myqchartview()
{
	delete chart_data;
}

void myqchartview::create_draw_line()
{
	draw_name.clear();
	draw_name = "baseline-" + QString::number(baselinenum);
	draw_baseline = new QLineSeries();
	chart_data->addSeries(draw_baseline);
	draw_baseline->attachAxis(chart_data->axisX());
	draw_baseline->attachAxis(chart_data->axisY());
	draw_baseline->setName(draw_name);
}

void myqchartview::mouseMoveEvent(QMouseEvent * event)
{
	if (drag_begin && index >= 0)
	{
		QPointF temp = this->chart()->mapToValue(event->pos());
		draw_baseline->replace(index, temp);
	}
	QPoint point;
	point.setX(event->x());
	point.setY(event->y());
	emit sendpos(point);

}

void myqchartview::mousePressEvent(QMouseEvent* event)
{
	QPointF temp = this->chart()->mapToValue(event->pos());
	if (event->button() == Qt::LeftButton && draw_begin)
	{
		draw_baseline->append(std::move(temp));
	}
	if (event->button() == Qt::LeftButton && drag_begin)
	{
		temp.setX((temp.x() - min) / dif);
		for (int i = 0; i < series_points.size(); i++) {
			//qDebug() << temp << "    " << series_points.at(i) << "    " << 100 * distance(temp, series_points.at(i));
			if ((100 * distance(temp, series_points.at(i))) <= 0.1)
			{
				index = i;
				this->setCursor(Qt::SizeVerCursor);
				break;
			}
		}
	}
}

void myqchartview::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton && draw_begin)
	{
		this->setCursor(Qt::ArrowCursor);
		baselinenum++;
		draw_begin = false;
		emit stop_draw();
	}
	if (event->button() == Qt::RightButton && drag_begin)
	{
		drag_begin = false;
		emit stop_drag();
	}

	if (event->button() == Qt::LeftButton && drag_begin && index >= 0)
	{
		QPointF temp = this->chart()->mapToValue(event->pos());
		temp.setX((temp.x() - min) / dif);
		series_points[index].setX(temp.x());
		series_points[index].setY(temp.y());
		this->setCursor(Qt::ArrowCursor);
		index = -1;
	}


}

void myqchartview::enterEvent(QEvent* event)
{
	if (draw_begin || drag_begin)
		this->setCursor(Qt::CrossCursor);
}

void myqchartview::leaveEvent(QEvent* event)
{
	if (draw_begin || drag_begin)
		this->setCursor(Qt::ArrowCursor);
}

float myqchartview::distance(const QPointF& x, const QPointF& y)
{
	return pow(x.x() - y.x(), 2) + pow(x.y() - y.y(), 2);
}

void myqchartview::begin_bar()
{
	update();
}

void myqchartview::paintEvent(QPaintEvent* event)
{
	QtCharts::QChartView::paintEvent(event);
	if (bar_begin)
	{
		QPainter p(viewport());
		QBrush brush;
		QPen pen;

		float x = chart_data->plotArea().topLeft().x();//x初始值设置为原点坐标
		float x_axis_length = chart_data->plotArea().width();//记录当前图中x轴的实际长度
		float y = chart_data->plotArea().topLeft().y();
		float dif = max_bar - min_bar;
		for (int i = 0; i < data_rect.size(); i++)
		{
			const QPointF& rect = data_rect[i];
			brush.setColor(QColor(bar_color.value(rect.y())));
			brush.setStyle(Qt::SolidPattern);
			pen.setColor(QColor(bar_color.value(rect.y())));
			p.setBrush(brush);
			p.setPen(pen);

			//绘制矩形
			float alpha = rect.x() / dif;
			p.drawRect(QRectF(x, y + 100 - rect.y() * 10, x_axis_length * alpha, rect.y() * 10));
			x += (x_axis_length * alpha);
		}
	}
}


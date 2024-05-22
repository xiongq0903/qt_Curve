#pragma once

#include <QAbstractSeries>

class CustomBarSeries  : public QAbstractSeries
{
	Q_OBJECT

public:
	CustomBarSeries(QObject *parent);
	~CustomBarSeries();
};

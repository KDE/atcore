/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2017 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2017-2018 Chris Rizzitello <rizzitello@kde.org>
*/

#include "plotwidget.h"
#include <QChart>

PlotWidget::PlotWidget(QWidget *parent)
    : QWidget(parent)
    , _chart(new QChartView(this))
    , _axisX(new QDateTimeAxis(this))
    , _axisY(new QValueAxis(this))
    , m_maximumPoints(120)
{
    _axisX->setTickCount(3);
    _axisX->setFormat(QStringLiteral("hh:mm:ss"));
    _axisY->setLabelFormat(QStringLiteral("%d"));
    _axisY->setTitleText(tr("Temp."));
    _axisY->setRange(0, 3e2);

    _axisX->setRange(QDateTime::currentDateTime().addSecs(-120), QDateTime::currentDateTime());

    _chart->chart()->addAxis(_axisY, Qt::AlignLeft);
    _chart->chart()->addAxis(_axisX, Qt::AlignBottom);

    _chart->setRenderHint(QPainter::Antialiasing);
    if (palette().text().color().value() >= QColor(Qt::lightGray).value()) {
        _chart->chart()->setTheme(QChart::ChartThemeDark);
    }

    auto mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_chart);
    setLayout(mainLayout);
}

void PlotWidget::addPlot(const QString &name)
{
    plot _newPlot;
    _newPlot.setName(name);
    _chart->chart()->addSeries(_newPlot.serie());
    _newPlot.serie()->attachAxis(_axisY);
    _newPlot.serie()->attachAxis(_axisX);
    _plots.insert(name, _newPlot);
}

void PlotWidget::removePlot(const QString &name)
{
    _chart->chart()->removeSeries(_plots[name].serie());
    _plots.remove(name);
}

void PlotWidget::appendPoint(const QString &name, float value)
{
    if (_plots[name].serie()->count() > m_maximumPoints) {
        _plots[name].serie()->remove(0);
    }
    _plots[name].pushPoint(value);
    update();
}

void PlotWidget::update()
{
    _axisX->setRange(QDateTime::currentDateTime().addSecs(-120), QDateTime::currentDateTime());
}

QStringList PlotWidget::plots()
{
    return _plots.keys();
}

void PlotWidget::setMaximumPoints(const int newMax)
{
    m_maximumPoints = std::max(newMax, 0);
}

void PlotWidget::setMaximumTemperature(const uint maxTemp)
{
    _axisY->setRange(0, maxTemp);
}

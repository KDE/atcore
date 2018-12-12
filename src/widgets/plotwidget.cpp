/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Patrick José Pereira - patrickjp@kde.org
            Tomaz Canabrava <tcanabrava@kde.org>
            Lays Rodrigues <lays.rodrigues@kde.org>
            Chris Rizzitello <rizzitello@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "plotwidget.h"
#include <QChart>

PlotWidget::PlotWidget(QWidget *parent) :
    QWidget(parent)
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

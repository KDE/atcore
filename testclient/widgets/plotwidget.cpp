/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Patrick José Pereira - patrickelectric@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 3 of
    the License or any later version accepted by the membership of
    KDE e.V. (or its successor approved by the membership of KDE
    e.V.), which shall act as a proxy defined in Section 14 of
    version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QChart>

#include "plotwidget.h"

PlotWidget::PlotWidget(QWidget *parent) :
    QWidget(parent)
{
    _chart = new QChartView;
    if (palette().text().color().value() >= QColor(Qt::lightGray).value()) {
        _chart->chart()->setTheme(QChart::ChartThemeDark);
    }
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_chart);
    this->setLayout(mainLayout);

    newPlot(tr("Actual Bed"));
    newPlot(tr("Target Bed"));
    // The extruder need to be added after some signal emitted (ExtruderCountChanged)
    newPlot(tr("Actual Ext.1"));
    newPlot(tr("Target Ext.1"));
    update();
}

void PlotWidget::newPlot(QString name)
{
    // Create a new plot
    _name2Index[name] = _plots.size();
    plot _newPlot;
    _newPlot.setName(name);
    _plots.append(_newPlot);
}

void PlotWidget::deletePlot(QString name)
{
    _plots.remove(_name2Index[name]);
    _name2Index[name] = -1;
}

void PlotWidget::appendPoint(QString name, float value)
{
    _plots[_name2Index[name]].pushPoint(value);
}

void PlotWidget::update()
{
    static bool firstTimeCheck = true;
    static QDateTimeAxis *axisX = new QDateTimeAxis;
    static QValueAxis *axisY = new QValueAxis;

    // After already executed, update time axis
    if (firstTimeCheck == false) {
        _chart->chart()->axisX()->setRange(QDateTime::currentDateTime().addSecs(-120), QDateTime::currentDateTime());
        return;
    }

    firstTimeCheck = false;

    axisX->setTickCount(3);
    axisX->setFormat(QStringLiteral("hh:mm:ss"));

    axisY->setLabelFormat(QStringLiteral("%d"));
    axisY->setTitleText(tr("Temp."));

    _chart->chart()->addAxis(axisY, Qt::AlignLeft);
    _chart->chart()->addAxis(axisX, Qt::AlignBottom);

    _chart->chart()->axisY()->setRange(0, 3e2);
    _chart->chart()->axisX()->setRange(QDateTime::currentDateTime().addSecs(-120), QDateTime::currentDateTime());

    for (auto plot : _plots) {
        _chart->chart()->addSeries(plot.serie());
        plot.serie()->attachAxis(axisY);
        plot.serie()->attachAxis(axisX);
    }

    _chart->setRenderHint(QPainter::Antialiasing);
}

PlotWidget::~PlotWidget()
{
}

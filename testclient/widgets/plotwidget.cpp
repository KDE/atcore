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
#include <KLocalizedString>

PlotWidget::PlotWidget(QWidget *parent) :
    QWidget(parent),
    _chart(new QChartView()),
    _axisX(new QDateTimeAxis()),
    _axisY(new QValueAxis())
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_chart);
    setLayout(mainLayout);

    _axisX->setTickCount(3);
    _axisX->setFormat(QStringLiteral("hh:mm:ss"));
    _axisY->setLabelFormat(QStringLiteral("%d"));
    _axisY->setTitleText(i18n("Temp."));

    _chart->chart()->addAxis(_axisY, Qt::AlignLeft);
    _chart->chart()->addAxis(_axisX, Qt::AlignBottom);
    _chart->chart()->axisY()->setRange(0, 3e2);
    _chart->chart()->axisX()->setRange(QDateTime::currentDateTime().addSecs(-120), QDateTime::currentDateTime());
    _chart->setRenderHint(QPainter::Antialiasing);
    if (palette().text().color().value() >= QColor(Qt::lightGray).value()) {
        _chart->chart()->setTheme(QChart::ChartThemeDark);
    }

    newPlot(i18n("Actual Bed"));
    newPlot(i18n("Target Bed"));
    // The extruder need to be added after some signal emitted (ExtruderCountChanged)
    newPlot(i18n("Actual Ext.1"));
    newPlot(i18n("Target Ext.1"));

}

void PlotWidget::newPlot(const QString &name)
{
    _name2Index[name] = _plots.size();
    plot _newPlot;
    _newPlot.setName(name);
    _chart->chart()->addSeries(_newPlot.serie());
    _newPlot.serie()->attachAxis(_axisY);
    _newPlot.serie()->attachAxis(_axisX);
    _plots.append(_newPlot);
}

void PlotWidget::deletePlot(const QString &name)
{
    _plots.remove(_name2Index[name]);
    _name2Index.remove(name);
}

void PlotWidget::appendPoint(const QString &name, float value)
{
    _plots[_name2Index[name]].pushPoint(value);
}

void PlotWidget::update()
{
    _chart->chart()->axisX()->setRange(QDateTime::currentDateTime().addSecs(-120), QDateTime::currentDateTime());
}

PlotWidget::~PlotWidget()
{
}

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

#pragma once

#include <QChartView>
#include <QWidget>
#include <QtCharts>

class PlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlotWidget(QWidget *parent = nullptr);
    ~PlotWidget();

    /**
     * @brief Create a new plot
     *
     * @param name p_name: plot name
     */
    void newPlot(const QString &name);

    /**
     * @brief Delete plot with name
     *
     * @param name p_name: name
     */
    void deletePlot(const QString &name);

    /**
     * @brief Append point to plot
     *
     * @param name p_name: plot name
     * @param value p_value: value
     */
    void appendPoint(const QString &name, float value);

    /**
     * @brief Update plot list, need to run after ALL plots added
     *
     */
    void update();
private:
    class plot
    {
    public:
        explicit plot() : _series(new QLineSeries())
        {
        };

        ~plot()
        {
            // Series will be deleted with chart
        };

        void pushPoint(float value)
        {
            QDateTime now = QDateTime::currentDateTime();
            _series->append(now.toMSecsSinceEpoch(), value);
        }

        void setName(const QString &name)
        {
            _name = name;
            _series->setName(_name);

            //Add 3 initial points to plot
            QDateTime now = QDateTime::currentDateTime();
            _series->append(now.toMSecsSinceEpoch() - 2 * 60e3, 0.0);
            _series->append(now.toMSecsSinceEpoch() - 60e3, 0.0);
            _series->append(now.toMSecsSinceEpoch(), 0.0);
        }

        QLineSeries *serie() const
        {
            return _series;
        }

        QString name() const
        {
            return _name;
        }

    private:
        QLineSeries *_series;
        QString _name;
    };
    QChartView *_chart;
    QDateTimeAxis *_axisX;
    QValueAxis *_axisY;

    QHash<QString, int> _name2Index;
    QVector<plot> _plots;
};

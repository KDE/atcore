/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Patrick José Pereira <patrickelectric@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <QObject>
#include "katcore_export.h"

class TemperaturePrivate;

class KATCORE_EXPORT Temperature : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float bedTemperature READ bedTemperature WRITE setBedTemperature NOTIFY bedTemperatureChanged);
    Q_PROPERTY(float bedTargetTemperature READ bedTargetTemperature WRITE setBedTargetTemperature NOTIFY bedTargetTemperatureChanged);
    Q_PROPERTY(float extruderTemperature READ extruderTemperature WRITE setExtruderTemperature NOTIFY extruderTemperatureChanged);
    Q_PROPERTY(float extruderTargetTemperature READ extruderTargetTemperature WRITE setExtruderTargetTemperature NOTIFY extruderTargetTemperatureChanged);

public:
    Temperature(QObject *parent = nullptr);
    float bedTemperature() const;
    float bedTargetTemperature() const;
    float extruderTemperature() const;
    float extruderTargetTemperature() const;
    void decodeTemp(const QByteArray &msg);

public slots:
    void setBedTemperature(float temp);
    void setBedTargetTemperature(float temp);
    void setExtruderTemperature(float temp);
    void setExtruderTargetTemperature(float temp);

signals:
    void bedTemperatureChanged(float temp);
    void bedTargetTemperatureChanged(float temp);
    void extruderTemperatureChanged(float temp);
    void extruderTargetTemperatureChanged(float temp);

private:
    TemperaturePrivate *d;
};

/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Patrick José Pereira <patrickelectric@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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

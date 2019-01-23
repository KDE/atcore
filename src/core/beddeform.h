/* AtCore
    Copyright (C) <2019>

    Authors:
        Chris Rizzitello <rizzitello@kde.org>

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

#include "atcore_export.h"

/**
 * @brief The Bed Deform class
 *
 * Read and hold data for the Bed Deformation of the printer
 */

class ATCORE_EXPORT BedDeform : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList bedDeformationGrid READ bedDeformationGrid NOTIFY dataChanged)
public:
    /**
     * @brief BedDeform Create a new bedDeform object
     * @param parent: Objects parent
     */
    explicit BedDeform(QObject *parent = nullptr);
    /**
     * @brief decodeDeform Decode the deform list from the provided lines
     * @param rawData: raw returns containing deform data
     */
    void decodeDeform(const QStringList &rawData);
    /**
     * @brief get bed Deform info
     * @return: bed Deform grid, Variant Format: QList<QList<double>>
     *  The Inner QList<double> contains,
     *  one row worth of offsets for the deformation grid.
     *  the outer list is a list of rows data.
     *  For a grid with 2x3 proble points the size of the inner list will be 2 and the outer list 3
     * example: QVariant(QVariantList, (QVariant(double, 0.25), QVariant(double, 0.225))
     *          ,QVariant(QVariantList, (QVariant(double, 0.045), QVariant(double, 0.01))
     *          , QVariant(QVariantList, (QVariant(double, -0.168), QVariant(double, -0.24)))
     */
    QVariantList bedDeformationGrid();
signals:
    void dataChanged(const QVariantList &data);
private:
    struct BedDeformPrivate;
    BedDeformPrivate *d;
};

/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2019 Chris Rizzitello <rizzitello@kde.org>
*/

#pragma once

#include <QObject>
#include <QVariant>

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

    ~BedDeform();
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

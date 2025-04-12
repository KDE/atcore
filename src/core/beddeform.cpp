/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2019 Chris Rizzitello <rizzitello@kde.org>
*/

#include "beddeform.h"
#include <QRegularExpressionMatch>

struct BedDeform::BedDeformPrivate {
    /** bedData Beddeform data*/
    QVariantList bedData;
    /** Regex to capture lines containing valid data */
    static const QRegularExpression lineRegEx;
    /** Regex to capture Data from in lines*/
    static const QRegularExpression valueRegEx;
};
/**
 * @brief Line with the pattern of  digit(s) a single space then '+' or '-'.
 *  Examples:
 *   0 -0.155 +5.123 -4.567
 *   10 +8.901 -2.345 +6.789
 */
const QRegularExpression BedDeform::BedDeformPrivate::lineRegEx = QRegularExpression(QStringLiteral(R"((?:\d+\s(\+|\-)))"));
/**
 * @brief Numeric value with a decimal (maybe negative)
 *    example Input : 0 +0.005 -1.110, +1.040
 *    captured values from above line: 0.005, -1.110, 1.04
 */
const QRegularExpression BedDeform::BedDeformPrivate::valueRegEx = QRegularExpression(QStringLiteral(R"((?<value>\-?\d+\.?\d+))"));

BedDeform::BedDeform(QObject *parent)
    : QObject(parent)
    , d(new BedDeformPrivate)
{
}

BedDeform::~BedDeform()
{
    delete d;
}
void BedDeform::decodeDeform(const QStringList &rawData)
{
    d->bedData.clear();
    QVariantList coreList;
    for (const QString &line : rawData) {
        QRegularExpressionMatch lineCheck = d->lineRegEx.match(line);
        if (lineCheck.hasMatch()) {
            QRegularExpressionMatchIterator valueCheck = d->valueRegEx.globalMatch(line);
            while (valueCheck.hasNext()) {
                coreList.append(valueCheck.next().captured(QStringLiteral("value")).toDouble());
            }
            d->bedData.push_back(coreList);
            coreList.clear();
        }
    }
    Q_EMIT dataChanged(d->bedData);
}

QVariantList BedDeform::bedDeformationGrid()
{
    return d->bedData;
}

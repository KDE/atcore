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
#include <QRegularExpressionMatch>
#include "beddeform.h"

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
    emit dataChanged(d->bedData);
}

QVariantList BedDeform::bedDeformationGrid()
{
    return d->bedData;
}

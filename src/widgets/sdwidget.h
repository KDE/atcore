/* AtCore Test Client
    Copyright (C) <2018>
    Author: Chris Rizzitello - rizzitello@kde.org

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

#include <QListWidget>
#include <QWidget>

#include "atcorewidgets_export.h"
/**
 * @brief The SdWidget class
 * Provide basic Sd card actions. Requires Fw Support.
 */
class ATCOREWIDGETS_EXPORT SdWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief SdWidget
     * @param parent
     */
    SdWidget(QWidget *parent = nullptr);

    /**
     * @brief update the list of files on the card.
     * @param fileList
     */
    void updateFilelist(const QStringList &fileList);
signals:
    /**
     * @brief request a list of files on the card.
     */
    void requestSdList();

    /**
     * @brief User has selected to print a file for the sd card.
     * @param fileName: file to print.
     */
    void printSdFile(const QString &fileName);

    /**
     * @brief User has selected to delete a file for the sd card.
     * @param fileName: file to delete
     */
    void deleteSdFile(const QString &fileName);

private:
    QListWidget *listSdFiles = nullptr;
};

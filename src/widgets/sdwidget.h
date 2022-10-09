/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018, 2020 Chris Rizzitello <rizzitello@kde.org>
*/

#pragma once

#include <QWidget>

#include "atcorewidgets_export.h"

class QListWidget;
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
    explicit SdWidget(QWidget *parent = nullptr);
    ~SdWidget() = default;

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

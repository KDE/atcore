/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018, 2020 Chris Rizzitello <rizzitello@kde.org>
*/

#pragma once

#include <QWidget>

#include "atcorewidgets_export.h"

class QPlainTextEdit;
class QTemporaryFile;

/**
 * @brief The LogWidget will display the log for the connected atcore. Create with a pointer to a new QTemporaryFile.
 */
class ATCOREWIDGETS_EXPORT LogWidget : public QWidget
{
    Q_OBJECT
public:
    LogWidget(QTemporaryFile *tempFile, QWidget *parent = nullptr);
    LogWidget(QWidget *parent = nullptr);

    /**
     * @brief Add a message to the log. Should always also be connected to AtCore::atcoreMessage;
     * @param  msg: Message
     */
    void appendLog(const QString &msg);

    /**
     * @brief Connect to AtCore::receivedMessage
     * @param  bmsg: Message
     */
    void appendRLog(const QByteArray &bmsg);

    /**
     * @brief Connect to AtCore::pushedCommand
     * @param  bmsg: Message
     */
    void appendSLog(const QByteArray &bmsg);

    /**
     * @brief test if last line of log is string
     * @param string: line to test for
     * @return true if last line starts with
     */
    bool endsWith(const QString &string);

private:
    void initialize();

    /**
     * @brief Return string with actual time
     * @return QString
     */
    QString getTime();

    /**
     * @brief save Log pressed.
     */
    void savePressed();

    /**
     * @brief Append text in temporary file
     * @param text
     */
    void writeTempFile(const QString &text);

    /**
     * @brief flush unwritten strings to temp file
     */
    void flushTemp();
    QStringList unsyncedStrings;
    QTemporaryFile *logFile = nullptr;
    QPlainTextEdit *textLog = nullptr;
};

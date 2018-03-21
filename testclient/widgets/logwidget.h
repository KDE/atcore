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

#include <QPlainTextEdit>
#include <QTemporaryFile>
#include <QWidget>

/* Usage:
 *
 * Create a instance of the log widget.
 */

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    LogWidget(QTemporaryFile *tempFile, QWidget *parent = nullptr);

    /**
     * @brief Add in logger normal type message
     *
     * @param  msg: Message
     */
    void appendLog(const QString &msg);

    /**
     * @brief Add in logger received type message
     *
     * @param  bmsg: Message
     */
    void appendRLog(const QByteArray &bmsg);

    /**
     * @brief Add in logger send type message
     *
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
    /**
     * @brief Return string with actual time
     *
     * @return QString
     */
    QString getTime();

    /**
     * @brief save Log pressed.
     */
    void savePressed();

    /**
     * @brief Append text in temporary file
     *
     * @param text
     */
    void writeTempFile(QString text);

    QTemporaryFile *logFile = nullptr;
    QPlainTextEdit *textLog = nullptr;
};

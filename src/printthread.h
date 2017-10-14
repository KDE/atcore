/* AtCore
   Copyright (C) <2017>

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

#include <QTextStream>
#include <QFile>

#include "atcore.h"

class PrintThreadPrivate;
/**
 * @brief The PrintThread class
 * A Thread for running a print job
 *
 * see AtCore::print() for example of how to create a print thread.
 *
 */
class ATCORE_EXPORT PrintThread : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Create a new Print Thread
     * @param parent: Parent of the tread
     * @param fileName: gcode File to print
     */
    PrintThread(AtCore *parent, QString fileName);
signals:
    /**
    * @brief Print job has finished
    */
    void finished();

    /**
     * @brief A command has caused an error
     * @param err: the offending command
     */
    void error(QString err);

    /**
     * @brief The print job's progress has changed
     */
    void printProgressChanged(float);

    /**
     * @brief the next command of the job
     * @param comm: Command to be sent next
     */
    void nextCommand(const QString &comm);

    /**
     * @brief Printer state was changed
     * @param state: new state
     */
    void stateChanged(const AtCore::STATES &state);

public slots:
    /**
     * @brief start the print thread
     */
    void start();
private slots:
    /**
     * @brief process the current job
     */
    void processJob();

    /**
     * @brief Set printer state
     * @param state: the new printer state
     */
    void setState(const AtCore::STATES &state);

private:
    /**
     * @brief parse the next line
     */
    void nextLine();

    /**
     * @brief end the print
     */
    void endPrint();
    PrintThreadPrivate *d;
};

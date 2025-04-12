/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2017-2018, 2020 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2018 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2018 Leandro Santiago <leandrosansilva@gmail.com>
*/

#pragma once

#include <QFile>

#include "atcore.h"

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
    PrintThread(AtCore *parent, const QString &fileName);
    ~PrintThread();
Q_SIGNALS:
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
    void printProgressChanged(const float);

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

public Q_SLOTS:
    /**
     * @brief start the print thread
     */
    void start();
private Q_SLOTS:
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

    /**
     * @brief injectCommand Attempt to inject a Command from the currently printing file.
     *
     * One of the following on a line that starts with ';-' \n
     * example line ;-Message: Hello \n
     *
     * - Pause: ppc\n
     *   Pause the print job and then run the comma-separated commands after pausing the job.\n
     *     + ppc: A comma-separated list of Commands to send after pause. ex(G91, G0 Z1, G90, G1 X0 Y195)\n
     *\n
     * - Extruder %Temperature:newTemp,extnum,wait \n
     *   Set extruder temperature. \n
     *     + newTemp: new target temperature. \n
     *     + extnum: Extruder number you want to Heat. Starting at 0. \n
     *     + wait: ignore commands until the target is reached. [true | false] \n
     *\n
     * - Bed %Temperature: newTemp,wait \n
     *   Set the bed temperature. \n
     *      + newTemp: new target temperature \n
     *      + wait: ignore commands until the target is reached. [true | false] \n
     *\n
     * - Fan Speed:newSpeed, fanNum \n
     *   Set the Fan speed. \n
     *     + newSpeed: new fan speed. \n
     *     + fanNum: Fan number. Starting at 0.\n
     *\n
     * - Print Speed:newSpeed \n
     *   Set the printer speed. \n
     *    + newSpeed: the print speed. 100= movement speed defined in file. \n
     *\n
     * - Flow Rate:newRate \n
     *   Set the flow rate \n
     *    + newRate: the flow rate. 100 = flow rate defined in file. \n
     *\n
     * - Message:message \n
     *   Show a message the printer's LCD \n
     *    + message: the message to print. \n
     *\n
     * - Command:command \n
     *   Inject your own command. Command are sent as is. Be sure your line is correct. \n
     *    + command: Commands to inject \n
     */
    void injectCommand(QString &command);

    /**
     * @brief d: Private storage for the thread
     */
    class PrintThreadPrivate;
    PrintThreadPrivate *d;
};

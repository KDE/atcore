/* AtCore Unit Tests
 * SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 * SPDX-FileCopyrightText: 2017-2019 Chris Rizzitello <rizzitello@kde.org>
 * SPDX-FileCopyrightText: 2018 Patrick José Pereira <patrickjp@kde.org>
 */

#include <QObject>

#include "../src/core/gcodecommands.h"

class GCodeTests : public QObject
{
    Q_OBJECT
private slots:
    bool testGCodeNeedsArg(GCode::GCommands code);
    void command_G0();
    void command_G1();
    void command_G28();
    void command_G32();
    void command_G90();
    void command_G91();
    void command_unsupportedG();

    bool testMCodeNeedsArg(GCode::MCommands code);
    void command_M20();
    void command_M21();
    void command_M22();
    void command_M23();
    void command_M24();
    void command_M25();
    void command_M26();
    void command_M27();
    void command_M28();
    void command_M29();
    void command_M30();
    void command_M84();
    void command_M104();
    void command_M105();
    void command_M106();
    void command_M107();
    void command_M109();
    void command_M112();
    void command_M114();
    void command_M115();
    void command_M116();
    void command_M117();
    void command_M119();
    void command_M140();
    void command_M155();
    void command_M190();
    void command_M220();
    void command_M221();
    void command_unsupportedM();
};

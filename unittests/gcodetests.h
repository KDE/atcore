/*
    This file is part of the KDE project

    Copyright (C) 2017 Chris Rizzitello <rizzitello@kde.org>

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
#include <QObject>

#include "../src/core/gcodecommands.h"

class GCodeTests: public QObject
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
    void command_M190();
    void command_M220();
    void command_M221();
    void command_unsupportedM();
};

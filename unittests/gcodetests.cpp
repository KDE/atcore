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
#include <algorithm>

#include "gcodetests.h"

bool GCodeTests::testGCodeNeedsArg(GCode::GCommands code)
{
    return GCode::toCommand(code) == GCode::commandRequiresArgument.arg(QStringLiteral("G"), QString::number(code));
}

void GCodeTests::command_G0()
{
    QVERIFY(GCode::toCommand(GCode::G0) == QStringLiteral("G0"));
    QVERIFY(GCode::toCommand(GCode::G0, QStringLiteral("50")) == QStringLiteral("G0 50"));
}

void GCodeTests::command_G1()
{
    QVERIFY(GCode::toCommand(GCode::G1) == QStringLiteral("G1"));
    QVERIFY(GCode::toCommand(GCode::G1, QStringLiteral("50")) == QStringLiteral("G1 50"));
}

void GCodeTests::command_G28()
{
    QVERIFY(GCode::toCommand(GCode::G28) == QStringLiteral("G28"));
    QVERIFY(GCode::toCommand(GCode::G28, QStringLiteral("Y")) == QStringLiteral("G28 Y"));
}

void GCodeTests::command_G32()
{
    QVERIFY(GCode::toCommand(GCode::G32) == QStringLiteral("G32 S1"));
}

void GCodeTests::command_G90()
{
    QVERIFY(GCode::toCommand(GCode::G90) == QStringLiteral("G90"));
}

void GCodeTests::command_G91()
{
    QVERIFY(GCode::toCommand(GCode::G91) == QStringLiteral("G91"));
}

void GCodeTests::command_unsupportedG()
{
    QVERIFY(GCode::toCommand(GCode::G2) == GCode::commandNotSupported);
}

bool GCodeTests::testMCodeNeedsArg(GCode::MCommands code)
{
    return GCode::toCommand(code) == GCode::commandRequiresArgument.arg(QStringLiteral("M"), QString::number(code));
}

void GCodeTests::command_M84()
{
    QVERIFY(GCode::toCommand(GCode::M84) == QStringLiteral("M84"));
    QVERIFY(GCode::toCommand(GCode::M84, QStringLiteral("10")) == QStringLiteral("M84 S10"));
}

void GCodeTests::command_M104()
{
    QVERIFY(testMCodeNeedsArg(GCode::M104));
    QVERIFY(GCode::toCommand(GCode::M104, QStringLiteral("100")) == QStringLiteral("M104 S100"));
    QVERIFY(GCode::toCommand(GCode::M104, QStringLiteral("3"), QStringLiteral("100")) == QStringLiteral("M104 P3 S100"));
}

void GCodeTests::command_M105()
{
    QVERIFY(GCode::toCommand(GCode::M105) == QStringLiteral("M105"));
}

void GCodeTests::command_M106()
{
    QVERIFY(GCode::toCommand(GCode::M106) == QStringLiteral("M106"));
    QVERIFY(GCode::toCommand(GCode::M106, QStringLiteral("100")) == QStringLiteral("M106 S100"));
    QVERIFY(GCode::toCommand(GCode::M106, QStringLiteral("3"), QStringLiteral("100")) == QStringLiteral("M106 P3 S100"));
}

void GCodeTests::command_M107()
{
    QVERIFY(GCode::toCommand(GCode::M107) == QStringLiteral("M107"));
}

void GCodeTests::command_M109()
{
    QVERIFY(testMCodeNeedsArg(GCode::M109));
    QVERIFY(GCode::toCommand(GCode::M109, QStringLiteral("100")) == QStringLiteral("M109 S100"));
}

void GCodeTests::command_M112()
{
    QVERIFY(GCode::toCommand(GCode::M112) == QStringLiteral("M112"));
}

void GCodeTests::command_M114()
{
    QVERIFY(GCode::toCommand(GCode::M114) == QStringLiteral("M114"));
}

void GCodeTests::command_M115()
{
    QVERIFY(GCode::toCommand(GCode::M115) == QStringLiteral("M115"));
}

void GCodeTests::command_M116()
{
    QVERIFY(GCode::toCommand(GCode::M116) == QStringLiteral("M116"));
}

void GCodeTests::command_M117()
{
    QVERIFY(testMCodeNeedsArg(GCode::M117));
    QVERIFY(GCode::toCommand(GCode::M117, QStringLiteral("100")) == QStringLiteral("M117 100"));
}

void GCodeTests::command_M119()
{
    QVERIFY(GCode::toCommand(GCode::M119) == QStringLiteral("M119"));
}

void GCodeTests::command_M140()
{
    QVERIFY(testMCodeNeedsArg(GCode::M140));
    QVERIFY(GCode::toCommand(GCode::M140, QStringLiteral("100")) == QStringLiteral("M140 S100"));
}

void GCodeTests::command_M190()
{
    QVERIFY(testMCodeNeedsArg(GCode::M190));
    QVERIFY(GCode::toCommand(GCode::M190, QStringLiteral("100")) == QStringLiteral("M190 S100"));
}

void GCodeTests::command_M220()
{
    QVERIFY(testMCodeNeedsArg(GCode::M220));;
    QVERIFY(GCode::toCommand(GCode::M220, QStringLiteral("100")) == QStringLiteral("M220 S100"));
}

void GCodeTests::command_M221()
{
    QVERIFY(testMCodeNeedsArg(GCode::M221));;
    QVERIFY(GCode::toCommand(GCode::M221, QStringLiteral("100")) == QStringLiteral("M221 S100"));
}

void GCodeTests::command_unsupportedM()
{
    QVERIFY(GCode::toCommand(GCode::M999) == GCode::commandNotSupported);
}

QTEST_MAIN(GCodeTests)

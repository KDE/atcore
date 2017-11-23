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
#include <QtTest>
#include <QObject>

#include "../src/gcodecommands.h"

class GCodeTests: public QObject
{
    Q_OBJECT
private slots:
    void command_G0();
    void command_G1();
    void command_G28();
    void command_G32();
    void command_G90();
    void command_G91();
    void command_unsupportedG();

    void string_G0();
    void string_G1();
    void string_G2();
    void string_G3();
    void string_G4();
    void string_G10();
    void string_G11();
    void string_G20();
    void string_G21();
    void string_G28();
    void string_G29();
    void string_G30();
    void string_G31();
    void string_G32();
    void string_G33();
    void string_G90();
    void string_G91();
    void string_G92();
    void string_G100();
    void string_G130();
    void string_G131();
    void string_G132();
    void string_G133();
    void string_G161();
    void string_G162();

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

    void string_M0();
    void string_M1();
    void string_M2();
    void string_M6();
    void string_M17();
    void string_M18();
    void string_M20();
    void string_M21();
    void string_M22();
    void string_M23();
    void string_M24();
    void string_M25();
    void string_M26();
    void string_M27();
    void string_M28();
    void string_M29();
    void string_M30();
    void string_M31();
    void string_M32();
    void string_M33();
    void string_M34();
    void string_M36();
    void string_M42();
    void string_M48();
    void string_M70();
    void string_M72();
    void string_M73();
    void string_M80();
    void string_M81();
    void string_M82();
    void string_M83();
    void string_M84();
    void string_M85();
    void string_M92();
    void string_M93();
    void string_M98();
    void string_M99();
    void string_M101();
    void string_M103();
    void string_M104();
    void string_M105();
    void string_M106();
    void string_M107();
    void string_M108();
    void string_M109();
    void string_M110();
    void string_M111();
    void string_M112();
    void string_M114();
    void string_M115();
    void string_M116();
    void string_M117();
    void string_M119();
    void string_M120();
    void string_M121();
    void string_M122();
    void string_M126();
    void string_M127();
    void string_M130();
    void string_M131();
    void string_M132();
    void string_M133();
    void string_M134();
    void string_M135();
    void string_M140();
    void string_M141();
    void string_M143();
    void string_M144();
    void string_M150();
    void string_M163();
    void string_M164();
    void string_M190();
    void string_M200();
    void string_M201();
    void string_M202();
    void string_M203();
    void string_M204();
    void string_M205();
    void string_M206();
    void string_M207();
    void string_M208();
    void string_M209();
    void string_M212();
    void string_M218();
    void string_M220();
    void string_M221();
    void string_M226();
    void string_M231();
    void string_M232();
    void string_M240();
    void string_M250();
    void string_M251();
    void string_M280();
    void string_M300();
    void string_M301();
    void string_M302();
    void string_M303();
    void string_M304();
    void string_M305();
    void string_M306();
    void string_M320();
    void string_M321();
    void string_M322();
    void string_M323();
    void string_M340();
    void string_M350();
    void string_M351();
    void string_M355();
    void string_M360();
    void string_M361();
    void string_M362();
    void string_M363();
    void string_M364();
    void string_M365();
    void string_M366();
    void string_M370();
    void string_M371();
    void string_M372();
    void string_M373();
    void string_M374();
    void string_M375();
    void string_M380();
    void string_M381();
    void string_M400();
    void string_M401();
    void string_M402();
    void string_M404();
    void string_M405();
    void string_M406();
    void string_M407();
    void string_M408();
    void string_M420();
    void string_M450();
    void string_M451();
    void string_M452();
    void string_M453();
    void string_M460();
    void string_M500();
    void string_M501();
    void string_M502();
    void string_M503();
    void string_M540();
    void string_M550();
    void string_M551();
    void string_M552();
    void string_M553();
    void string_M554();
    void string_M555();
    void string_M556();
    void string_M557();
    void string_M558();
    void string_M559();
    void string_M560();
    void string_M561();
    void string_M562();
    void string_M563();
    void string_M564();
    void string_M565();
    void string_M566();
    void string_M567();
    void string_M568();
    void string_M569();
    void string_M570();
    void string_M571();
    void string_M573();
    void string_M574();
    void string_M575();
    void string_M577();
    void string_M578();
    void string_M579();
    void string_M580();
    void string_M600();
    void string_M605();
    void string_M665();
    void string_M666();
    void string_M667();
    void string_M851();
    void string_M906();
    void string_M907();
    void string_M908();
    void string_M911();
    void string_M912();
    void string_M913();
    void string_M928();
    void string_M997();
    void string_M998();
    void string_M999();
};

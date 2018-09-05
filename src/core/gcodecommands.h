/* AtCore
    Copyright (C) <2016>

    Authors:
        Lays Rodrigues <lays.rodrigues@kde.org>
        Tomaz Canabrava <tcanabrava@kde.org>
        Patrick José Pereira <patrickjp@kde.org>
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

#include <QString>
#include <QObject>

#include "atcore_export.h"
/**
 * @brief The GCode class
 * Provides Descriptions and Commands strings for G and M Commands
 */
class ATCORE_EXPORT GCode
{
    Q_GADGET
    friend class GCodeTests;
public:
    /**
     * @brief The GCommands enum
     */
    enum GCommands {
        G0, G1, G2, G3, G4,
        G10 = 10, G11,
        G20 = 20, G21, G22, G23, G28 = 28, G29,
        G30, G31, G32, G33,
        G90 = 90, G91, G92,
        G100 = 100,
        G130 = 130, G131, G132, G133,
        G161 = 161, G162
    };
    Q_ENUM(GCommands)

    /**
     * @brief The MCommands enum
     */
    enum MCommands {
        M0, M1, M2, M6 = 6,
        M17 = 17, M18,
        M20 = 20, M21, M22, M23, M24, M25, M26, M27, M28, M29,
        M30, M31, M32, M33, M34, M36 = 36, M37, M38,
        M40 = 40, M41, M42, M43, M48 = 48,
        M70 = 70, M72 = 72, M73,
        M80 = 80, M81, M82, M83, M84, M85,
        M92 = 92, M93, M98 = 98, M99,
        M101 = 101, M102, M103, M104, M105, M106, M107, M108, M109,
        M110, M111, M112, M113, M114, M115, M116, M117, M118, M119,
        M120, M121, M122, M123, M124, M126 = 126, M127, M128, M129,
        M130, M131, M132, M133, M134, M135, M136,
        M140 = 140, M141, M142, M143, M144, M146 = 146, M149 = 149,
        M150 = 150,
        M160 = 160, M163 = 163, M164 = 164,
        M190 = 190, M191,
        M200 = 200, M201, M202, M203, M204, M205, M206, M207, M208, M209,
        M210, M211, M212, M218 = 218,
        M220 = 220, M221, M222, M223, M224, M225, M226, M227, M228, M229,
        M230, M231, M232,
        M240 = 240, M241, M245 = 245, M246,
        M250 = 250, M251,
        M280 = 280,
        M300 = 300, M301, M302, M303, M304, M305, M306,
        M320 = 320, M321, M322, M323,
        M340 = 340,
        M350 = 350, M351, M355 = 355,
        M360 = 360, M361, M362, M363, M364, M365, M366,
        M370 = 370, M371, M372, M373, M374, M375,
        M380 = 380, M381,
        M400 = 400, M401, M402, M404 = 404, M405, M406, M407, M408,
        M420 = 420, M421,
        M450 = 450, M451, M452, M453,
        M460 = 460,
        M500 = 500, M501, M502, M503,
        M540 = 540,
        M550 = 550, M551, M552, M553, M554, M555, M556, M557, M558, M559,
        M560, M561, M562, M563, M564, M565, M566, M567, M568, M569,
        M570, M571, M572, M573, M574, M575, M577 = 577, M578, M579,
        M580 = 580, M581, M582, M583, M584,
        M600 = 600, M605 = 605, M665 = 665, M666 = 666, M667, M668,
        M700 = 700, M701, M702, M703,
        M710 = 710,
        M800 = 800, M801,
        M851 = 851,
        M906 = 906, M907, M908,
        M910 = 910, M911, M912, M913,
        M928 = 928,
        M997 = 997, M998, M999
    };
    Q_ENUM(MCommands)

    /**
     * @brief Return Description of command \p gcode
     * @param gcode: Command to describe
     * @return description of GCommand
     */
    static QString description(GCommands gcode);

    /**
     * @brief Return Description of command \p gcode
     * @param gcode: Command to describe
     * @return description of MCommand
     */
    static QString description(MCommands gcode);
    /**
     * @brief Convert GCode::GCommands to command
     * @param gcode: GCode::GCommands
     * @param value1: Value of argument
     * @return Command String to send to printer
     */
    static QString toCommand(GCommands gcode, const QString &value1 = QString());

    /**
     * @brief Convert GCode::MCommands to command
     * @param gcode: GCode::MCommands
     * @param value1: Value of argument 1
     * @param value2: Value of argument 2
     * @return Command String to send to printer
     */
    static QString toCommand(MCommands gcode, const QString &value1 = QString(), const QString &value2 = QString());
protected:
    static const QString commandRequiresArgument;
    static const QString commandNotSupported;
};

#include "gcodecommands.h"

namespace GCode {

namespace GCommands {

    QString toString(Enum gcode)
    {
        switch (gcode) {
        case G0://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
            return QString("G0: Rapid linear move");
        case G1://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
            return QString("G1: Linear move");
        case G2://Sprinter - Marlin - Repetier - Smoothie
            return QString("G2: Controlled Arc Move clockwise");
        case G3://Sprinter - Marlin - Repetier - Smoothie
            return QString("G3: Controlled Arc Move counterclockwise");
        case G4://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
            return QString("G4: Dwell");
        case G10://Marlin - Repetier > 0.92 - Smoothie
            return QString("G10: Retract");
        case G11://Marlin - Repetier > 0.92 - Smoothie
            return QString("G11: Unretract");
        case G20://Teacup - Sprinter - Repetier - Smoothie - RepRap Firmware
            return QString("G20: Set units to inches");
        case G21://Teacup - Sprinter - Repetier - Smoothie - RepRap Firmware
            return QString("G21: Set units to millimiters");
        case G28://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
            return QString("G28: Move to Origin Home");
        case G29://Marlin - Repetier 0.91.7
            return QString("G29: Detailed Z-Probe");
        case G30:// Marlin - Repetier - Smoothie - RepRap Firmware
            return QString("G30: Single Z-Probe");
        case G31://Repetier 0.91.7 - Smoothie - RepRap Firmware - Marlin
            return QString("G31: Set or report current probe status / Dock Z Probe sled for Marlin");
        case G32://Repetier 0.92.8 - Smoothie - RepRap Firmware - Marlin
            return QString("G32: Probe Z and calculate Z plane(Bed Leveling)/ UnDoc Z Probe sled for Marlin");
        case G33://Repetier 0.92.8
            return QString("G33: Measure/List/Adjust Distortion Matrix");
        case G90://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
            return QString("G90: Set to absolute positioning");
        case G91://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
            return QString("G91: Set to relative positioning");
        case G92://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
            return QString("G92: Set position");
        case G100://Repetier 0.92
            return QString("G100: Calibrate floor  or rod radius");
        case G130://MakerBot
            return QString("G130: Set digital potentiometer value");
        case G131://Repetier 0.91
            return QString("G131: Recase Move offset");
        case G132://Repetier 0.91
            return QString("G132: Calibrate endstops offsets");
        case G133://Repetier 0.91
            return QString("G133: Measure steps to top");
        case G161://Teacup - MakerBot
            return QString("G161: Home axis to minimum");
        case G162://Teacup - MakerBot
            return QString("G162: Home axis to maximum");
        default:
            return QString("GCommand not supported!");
        }
    }

    QString toCommand(Enum gcode, QChar &c)
    {
        switch (gcode) {
            case G28: {
                if(c.isNull())
                    return QString("G28");
                else
                    return QString("G28 %1").arg(c.toUpper());
            }
        case G32:
             return QString("G32 S1");
        case G90:
            return QString("G90");
        case G91:
            return QString("G91");
        default:
            return QString("Not implemented or not supported!");

        }
    }

}

namespace MCommands {

}

}

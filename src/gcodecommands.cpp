#include "gcodecommands.h"

namespace GCode {

namespace Firmwares {

    QString toString(Enum firmware)
    {
        switch (firmware) {
        case Repetier:
            return QString("Repetier Firmware");
        case Marlin:
            return QString("Marlin");
        case Teacup:
            return QString("Teacup");
        case RepRapFirmware:
            return QString("RepRap Firmware");
        case MakerBot:
            return QString("MakerBot Firmware");
        case Sprinter:
            return QString("Sprinter");
        case Sjfw:
            return QString("Sjfw");
        case Sailfish:
            return QString("Sailfish");
        case Smoothie:
            return QString("Smoothieware");
        default:
            return QString("Firmware not listed");
        }
    }
}

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

        QString toString(Enum gcode)
        {
            switch (gcode) {
                case M0://Marlin - Teacup - RepRap Firmware
                    return QString("M0: Stop or unconditional stop");
                case M1://Marlin - RepRap Firmware
                    return QString("M1: Sleep or unconditional stop");
                case M2://Teacup - Maker Bot
                    return QString("M2: Program End");
                case M6://Teacup
                    return QString("M6: Tool Change");
                case M17://Teacup - Marlin - Smoothie
                    return QString("M17: Enable/power all steppers motors");
                case M18://Teacup - Marlin(M84) - Smoothie -RepRap Firmware
                    return QString("M18: Disable all steppers motors");
                case M20://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M20: List SDCard");
                case M21://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M21: Initialize SDCard");
                case M22://Teacup - Sprinter - Marlin - Repetier - RepRap Firmware
                    return QString("M22: Release SDCard");
                case M23:////Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M23: Select SD file");
                case M24://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M24: Start/resume SD print");
                case M25://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M25: Pause SD print");
                case M26://Sprinter - Marlin - Repetier - Smoothie(abort) - RepRap Firmware
                    return QString("M26: Set SD position");
                case M27://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M27: Report SD print status");
                case M28://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M28: Begin write to SD card");
                case M29:// Sprinter - Marlin - Repetier - RepRap Firmware
                    return QString("M29: Stop writing to SD card");
                case M30://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M30: Delete a file on the SD card");
                case M31://Marlin
                    return QString("M31: Output time since last M109 or SD card start to serial");
                case M32://Marlin - Smoothie - RepRap Firmware
                    return QString("M32: Select file and start SD prin");
                case M33://Marlin
                    return QString("M33: Get the long name for an SD card file or folder");
                case M34://Marlin
                    return QString("M34: Set SD file sorting options");
                case M36://RepRap Firmware
                    return QString("M36: Return file information");
                case M42://Sprinter - Marlin - Repetier - RepRap Firmware
                    return QString("M42: Switch I/O pin");
                case M48://Marlin
                    return QString("M48: Measure Z-Probe repeatabiliy");
                case M70://MakerBot
                    return QString("M70: Display message");
                case M72://MakerBot
                    return QString("M72: Play a tone or song");
                case M73://MakerBot
                    return QString("M73: Set build percentage");
                case M80://Teacup(automatic) - Sprinter - Marlin - Repetier - RepRap Firmware
                    return QString("M80: ATX Power On");
                case M81://Teacup(automatic) - Sprinter - Marlin - Repetier  - RepRap Firmware
                    return QString("M81: ATX Power Off");
                case M82://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M82: Set extruder to absolute mode");
                case M83://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M83: Set extruder to relative mode");
                case M84://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M84: Stop idle hold");
                case M85://Sprinter - Marlin - Repetier
                    return QString("M85: Set Inactivity shutdown timer");
                case M92:// Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M92: Set axis steps per unit");
                case M93:// Sprinter
                    return QString("M93: Send axis steps per unit");
                case M98: //RepRap Firmware
                    return QString("M98: Call Macro/Subprogram");
                case M99://RepRap Firmware
                    return QString("M99: Return from Macro/Subprogram");
                case M101://Teacup
                    return QString("M101:Turn extruder 1 on Forward, Undo Retraction");
                case M103://Teacup
                    return QString("M103: Turn all extruders off - Extruder Retraction");
                case M104://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
                    return QString("M104: Set Extruder Temperature");
                case M105://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
                    return QString("M105: Get Extruder Temperature");
                case M106://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M106: Fan On");
                case M107:// Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M107: Fan Off");
                case M108://Marlin
                    return QString("M108: Cancel Heating");
                case M109:// Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
                    return QString("M109: Set Extruder Temperature and Wait");
                case M110:// Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M110: Set Current Line Number");
                case M111://Teacup -  Marlin - Repetier - RepRap Firmware
                    return QString("M111: Set Debug Level");
                case M112://Teacup - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M112: Emergency Stop");
                case M114://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
                    return QString("M114: Get Current Position");
                case M115://Teacup - Sprinter - Marlin - Repetier - RepRap Firmware
                    return QString("M115: Get Firmware Version and Capabilities");
                case M116://Teacup - Repetier - RepRap Firmware - MakerBot
                    return QString("M116: Wait");
                case M117:// Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M117: Display Message");
                case M119://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M119: Get Endstop Status");
                case M120://Marlin - Smoothie - RepRap Firmware
                    return QString("M120: Push for Smoothie and RepRap Firmware / Enable Endstop detection for Marlin");
                case M121://Marlin - Smoothie - RepRap Firmware
                    return QString("M121: Pop for Smoothie and RepRap Firmware / Disable Endstop detection for Marlin");
                case M122://RepRap Firmware
                    return QString("M122: Diagnose");
                case M126://Marlin - MakerBot
                    return QString("M126: Open valve");
                case M127://Marlin - MakerBot
                    return QString("M127: Close valve");
                case M130://Teacup
                    return QString("M130: Set PID P value");
                case M131://Teacup
                    return QString("M131: Set PID I value");
                case M132://Teacup - MakerBot
                    return QString("M132: Set PID D value");
                case M133://Teacup - MakerBot
                    return QString("M133: Set PID I limit value");
                case M134://Teacup - MakerBot
                    return QString("M134: Write PID values to EEPROM");
                case M135://RepRap Firmware - MakerBot
                    return QString("M135: Set PID sample interval");
                case M140://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
                    return QString("M140: Set Bed Temperature - Fast");
                case M141://RepRap Firmware
                    return QString("M141: Set Chamber Temperature - Fast");
                case M143://RepRap Firmware
                    return QString("M143: Maximum hot-end temperature");
                case M144://RepRap Firmware
                    return QString("M144: Stand by your bed");
                case M150://Marlin
                    return QString("M150: Set display color");
                case M163://Repetier > 0.92
                    return QString("M163: Set weight of mixed material");
                case M164://Repetier > 0.92
                    return QString("M164: Store weights");
                case M190://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M190: Wait for bed temperaure to reach target temp");
                case M200://Marlin - Repetier - Smoothie
                    return QString("M200: Set filament diameter");
                case M201://Sprinter - Marlin - Repetier - RepRap Firmware
                    return QString("M201: Set max printing acceleration");
                case M202://Marlin - Repetier
                    return QString("M202: Set max travel acceleration");
                case M203://Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M203: Set maximum feedrate");
                case M204://Sprinter - Marlin - Repetier - Smoothie
                    return QString("M204: Set default acceleration");
                case M205://Sprinter - Marlin - Repetier - Smoothie
                    return QString("M205: Advanced settings");
                case M206://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M206: Offset axes for Sprinter, Marlin, Smoothie, RepRap Firmware / Set eeprom value for Repetier");
                case M207://Marlin - Smoothie
                    return QString("M207: Set retract length ");
                case M208://Marlin - Smoothie
                    return QString("M208: Set unretract length");
                case M209://Marlin - Repetier
                    return QString("M209: Enable automatic retract");
                case M212://Marlin
                    return QString("M212: Set Bed Level Sensor Offset");
                case M218://Marlin
                    return QString("M218: Set Hotend Offset");
                case M220://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M220: Set speed factor override percentage");
                case M221://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M221: Set extrude factor override percentage");
                case M226://Marlin - Repetier
                    return QString("M226: Wait for pin state");
                case M231://Repetier
                    return QString("M231: Set OPS parameter ");
                case M232://Repetier
                    return QString("M232: Read and reset max. advance values");
                case M240: //Marlin
                    return QString("M240: Trigger camera ");
                case M250://Marlin
                    return QString("M250: Set LCD contrast");
                case M251://Repetier
                    return QString("M251: Measure Z steps from homing stop (Delta printers) ");
                case M280://Marlin
                    return QString("M280: Set servo position ");
                case M300://Marlin - Repetier - RepRap Firmware - MakerBot
                    return QString("M300: Play beep sound ");
                case M301://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M301: Set PID parameters");
                case M302://Marlin - Repetier > 0.92 - RepRap Firmware
                    return QString("M302: Allow cold extrudes ");
                case M303://Sprinter - Marlin - Repetier - Smoothie
                    return QString("M303: Run PID tuning ");
                case M304://Marlin - RepRap Firmware
                    return QString("M304: Set PID parameters - Bed ");
                case M305:// Smoothie - RepRap Firmware
                    return QString("M305: Set thermistor and ADC parameters ");
                case M306:// Smoothie
                    return QString("M306: set home offset calculated from toolhead position ");
                case M320://Repeier
                    return QString("M320: Activate autolevel (Repetier) ");
                case M321://Repetier
                    return QString("M321: Deactivate autolevel (Repetier) ");
                case M322://Repetier
                    return QString("M322: Reset autolevel matrix (Repetier) ");
                case M323://Repetier
                    return QString("M323: Distortion correction on/off (Repetier) ");
                case M340://Repetier
                    return QString(" M340: Control the servos ");
                case M350://Marlin - Repetier - RepRap Firmware
                    return QString("M350: Set microstepping mode ");
                case M351://Marlin
                    return QString("M351: Toggle MS1 MS2 pins directly ");
                case M355://Repetier > 0.92.2
                    return QString("M355: Turn case lights on/off ");
                case M360://Repetier > 9.92.2
                    return QString("M360: Report firmware configuration ");
                case M361://Smoothie
                    return QString("M361: Move to Theta 90 degree position ");
                case M362://Smoothie
                    return QString("M362: Move to Psi 0 degree position ");
                case M363://Smoothie
                    return QString("M363: Move to Psi 90 degree position ");
                case M364://Smoothie
                    return QString("M364: Move to Psi + Theta 90 degree position");
                case M365://Smoothie
                    return QString("M365: SCARA scaling factor ");
                case M366://Smoothie
                    return QString("M366: SCARA convert trim ");
                case M370://Smoothie
                    return QString("M370: Morgan manual bed level - clear map ");
                case M371://Smoothie
                    return QString("M371: Move to next calibration position ");
                case M372://Smoothie
                    return QString("M372: Record calibration value, and move to next position ");
                case M373://Smoothie
                    return QString("M373: End bed level calibration mode ");
                case M374://Smoothie
                    return QString("M374: Save calibration grid ");
                case M375://Smoothie
                    return QString("M375: Display matrix / Load Matrix ");
                case M380://Marlin
                    return QString("M380: Activate solenoid ");
                case M381://Marlin
                    return QString("M381: Disable all solenoids ");
                case M400://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M400: Wait for current moves to finish ");
                case M401://Marlin
                    return QString("M401: Lower z-probe ");
                case M402://Marlin
                    return QString("M402: Raise z-probe");
                case M404://Marlin - RepRap Firmware
                    return QString("M404: Filament width and nozzle diameter ");
                case M405://Marlin
                    return QString("M405: Filament Sensor on ");
                case M406://Marlin
                    return QString("M406: Filament Sensor off");
                case M407://Marlin - RepRap Firmware
                    return QString("M407: Display filament diameter ");
                case M408://RepRap Firmware
                    return QString("M408: Report JSON-style response ");
                case M420:
                    return QString(" M420: Enable/Disable Mesh Leveling (Marlin)");
                case M450://Repetier
                    return QString("M450: Report Printer Mode ");
                case M451://Repetier
                    return QString("M451: Select FFF Printer Mode");
                case M452://Repetier
                    return QString("M452: Select Laser Printer Mode ");
                case M453://Repetier
                    return QString("M453: Select CNC Printer Mode ");
                case M460://Repetier
                    return QString("M460: Define temperature range for thermistor controlled fan ");
                case M500://Sprinter - Marlin - Repetier - RepRap Firmware
                    return QString("M500: Store parameters in EEPROM");
                case M501://Sprinter - Marlin - Repetier - RepRap Firmware
                    return QString("M501: Read parameters from EEPROM ");
                case M502://Sprinter - Marlin - Repetier - RepRap Firmware
                    return QString("M502: Revert to the default 'factory settings'.");
                case M503://Sprinter - Marlin - RepRap Firmware
                    return QString("M503: Print settings ");
                case M540://Marlin
                    return QString("M540: Enable/Disable 'Stop SD Print on Endstop Hit' ");
                case M550://RepRap Firmware
                    return QString("M550: Set Name ");
                case M551://RepRap Firmware
                    return QString("M551: Set Password ");
                case M552://RepRap Firmware
                    return QString("M552: Set IP address ");
                case M553://RepRap Firmware
                    return QString(" M553: Set Netmask ");
                case M554://RepRap Firmware
                    return QString("M554: Set Gateway ");
                case M555://RepRap Firmware
                    return QString("M555: Set compatibility ");
                case M556://RepRap Firmware
                    return QString("M556: Axis compensation ");
                case M557://Smoothie - RepRap Firmware
                    return QString("M557: Set Z probe point ");
                case M558://RepRap Firmware
                    return QString("M558: Set Z probe type ");
                case M559://RepRap Firmware
                    return QString("M559: Upload configuration file ");
                case M560://RepRap Firmware
                    return QString("M560: Upload web page file ");
                case M561://Smoothie - RepRap Firmware
                    return QString("M561: Set Identity Transform ");
                case M562://RepRap Firmware
                    return QString("M562: Reset temperature fault ");
                case M563://RepRap Firmware
                    return QString("M563: Define or remove a tool ");
                case M564://RepRap Firmware
                    return QString("M564: Limit axes ");
                case M565://Smoothie
                    return QString("M565: Set Z probe offset ");
                case M566://RepRap Firmware
                    return QString("M566: Set allowable instantaneous speed change ");
                case M567://RepRap Firmware
                    return QString("M567: Set tool mix ratio");
                case M568://RepRap Firmware
                    return QString("M568: Turn off/on tool mix ratio");
                case M569://RepRap Firmware
                    return QString("M569: Set axis direction and enable values ");
                case M570://RepRap Firmware
                    return QString("M570: Set heater timeout ");
                case M571://RepRap Firmware
                    return QString("M571: Set output on extrude ");
                case M573://RepRap Firmware
                    return QString("M573: Report heater PWM ");
                case M574://RepRap Firmware
                    return QString(" M574: Set endstop configuration ");
                case M575://RepRap Firmware
                    return QString("M575: Set serial comms parameters ");
                case M577://RepRap Firmware
                    return QString("M577: Wait until endstop is triggered ");
                case M578://RepRap Firmware
                    return QString("M578: Fire inkjet bits ");
                case M579://RepRap Firmware
                    return QString("M579: Scale Cartesian axes ");
                case M580://RepRap Firmware
                    return QString("M580: Select Roland ");
                case M600://Marlin
                    return QString("M600: Filament change pause");
                case M605://Marlin
                    return QString("M605: Set dual x-carriage movement mode ");
                case M665://Marlin - Smoothie - RepRap Firmware
                    return QString("M665: Set delta configuration ");
                case M666://Marlin - Repetier - Smoothie - RepRap Firmware
                    return QString("M666: Set delta endstop adjustment ");
                case M667://RepRap Firmware
                    return QString("M667: Select CoreXY mode ");
                case M851://Marlin
                    return QString("M851: Set Z-Probe Offset ");
                case M906://RepRap Firmware
                    return QString("M906: Set motor currents");
                case M907://Marlin - Repetier - Smoothie
                    return QString("M907: Set digital trimpot motor ");
                case M908://Marlin - Repetier > 0.92
                    return QString("M908: Control digital trimpot directly");
                case M911://RepRap Firmware
                    return QString("M911: Set power monitor threshold voltages ");
                case M912://RepRap Firmware
                    return QString("M912: Set electronics temperature monitor adjustment");
                case M913://RepRap Firmware
                    return QString("M913: Set motor percentage of normal current ");
                case M928://Marlin
                    return QString("M928: Start SD logging ");
                case M997://RepRap Firmware
                    return QString("M997: Perform in-application firmware update ");
                case M998://RepRap Firmware
                    return QString("M998: Request resend of line");
                case M999://Marlin - Smoothie - RepRap Firmware
                    return QString(" M999: Restart after being stopped by error ");
            default:
                return QString("Not implemented or not supported!");

            }
        }

        QString toCommand(Enum gcode, const QString &value){

            switch (gcode) {
            case M104: {
                if(!value.isEmpty())
                    return QString("M104 S%1").arg(value);
                else
                    return QString("ERROR! M104: It's obligatory have an argument");
            }
            case M105:
                return QString("M105");
            case M106:
                return QString("M106");
            case M107:
                return QString("M107");
            case M112:
                return QString("M112");
            case M114:
                return QString("M114");
            case M115:
                return QString("M115");
            case M116:
                return QString("M116");
            case M117: {
                if(!value.isEmpty())
                    return QString("M117 %1").arg(value);
                else
                    return QString("ERROR! M117: It's obligatory have an argument");
            }
            case M119:
                return QString("M119");
            case M140: {
                if(!value.isEmpty())
                    return QString("M140 S%1").arg(value);
                else
                    return QString("ERROR! M140: It's obligatory have an argument");
            }
            case M220: {
                if(!value.isEmpty())
                    return QString("M220 S%1").arg(value);
                else
                    return QString("ERROR! M220: It's obligatory have an argument");
            }
            case M221: {
                if(!value.isEmpty())
                    return QString("M221 S%1").arg(value);
                else
                    return QString("ERROR! M221: It's obligatory have an argument");
            }
            default:
                return QString("Not supported or implemented!");
            }
        }
    }
}

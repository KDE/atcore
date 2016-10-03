#include "gcodecommands.h"
#include <QObject>

namespace GCode
{

QString toString(Firmwares firmware)
{
    switch (firmware) {
    case Repetier:
        return QStringLiteral("Repetier");
    case Marlin:
        return QStringLiteral("Marlin");
    case Teacup:
        return QStringLiteral("Teacup");
    case RepRapFirmware:
        return QStringLiteral("RepRap");
    case MakerBot:
        return QStringLiteral("MakerBot");
    case Sprinter:
        return QStringLiteral("Sprinter");
    case Sjfw:
        return QStringLiteral("Sjfw");
    case Sailfish:
        return QStringLiteral("Sailfish");
    case Smoothie:
        return QStringLiteral("Smoothieware");
    default:
        return QObject::tr("Firmware not listed");
    }
}

QString toString(GCommands gcode)
{
    switch (gcode) {
    case G0://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("G0: Rapid linear move");
    case G1://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("G1: Linear move");
    case G2://Sprinter - Marlin - Repetier - Smoothie
        return QObject::tr("G2: Controlled Arc Move clockwise");
    case G3://Sprinter - Marlin - Repetier - Smoothie
        return QObject::tr("G3: Controlled Arc Move counterclockwise");
    case G4://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("G4: Dwell");
    case G10://Marlin - Repetier > 0.92 - Smoothie
        return QObject::tr("G10: Retract");
    case G11://Marlin - Repetier > 0.92 - Smoothie
        return QObject::tr("G11: Unretract");
    case G20://Teacup - Sprinter - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("G20: Set units to inches");
    case G21://Teacup - Sprinter - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("G21: Set units to millimiters");
    case G28://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("G28: Move to Origin Home");
    case G29://Marlin - Repetier 0.91.7
        return QObject::tr("G29: Detailed Z-Probe");
    case G30:// Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("G30: Single Z-Probe");
    case G31://Repetier 0.91.7 - Smoothie - RepRap Firmware - Marlin
        return QObject::tr("G31: Set or report current probe status / Dock Z Probe sled for Marlin");
    case G32://Repetier 0.92.8 - Smoothie - RepRap Firmware - Marlin
        return QObject::tr("G32: Probe Z and calculate Z plane(Bed Leveling)/ UnDoc Z Probe sled for Marlin");
    case G33://Repetier 0.92.8
        return QObject::tr("G33: Measure/List/Adjust Distortion Matrix");
    case G90://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("G90: Set to absolute positioning");
    case G91://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("G91: Set to relative positioning");
    case G92://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("G92: Set position");
    case G100://Repetier 0.92
        return QObject::tr("G100: Calibrate floor or rod radius");
    case G130://MakerBot
        return QObject::tr("G130: Set digital potentiometer value");
    case G131://Repetier 0.91
        return QObject::tr("G131: Recase Move offset");
    case G132://Repetier 0.91
        return QObject::tr("G132: Calibrate endstops offsets");
    case G133://Repetier 0.91
        return QObject::tr("G133: Measure steps to top");
    case G161://Teacup - MakerBot
        return QObject::tr("G161: Home axis to minimum");
    case G162://Teacup - MakerBot
        return QObject::tr("G162: Home axis to maximum");
    default:
        return QObject::tr("GCommand not supported!");
    }
}

QString toCommand(GCommands gcode, QChar &c)
{
    switch (gcode) {
    case G28: {
        if (c.isNull()) {
            return QStringLiteral("G28");
        } else {
            return QStringLiteral("G28 %1").arg(c.toUpper());
        }
    }
    case G32:
        return QStringLiteral("G32 S1");
    case G90:
        return QStringLiteral("G90");
    case G91:
        return QStringLiteral("G91");
    default:
        return QObject::tr("Not implemented or not supported!");

    }
}

QString toString(MCommands gcode)
{
    switch (gcode) {
    case M0://Marlin - Teacup - RepRap Firmware
        return QObject::tr("M0: Stop or unconditional stop");
    case M1://Marlin - RepRap Firmware
        return QObject::tr("M1: Sleep or unconditional stop");
    case M2://Teacup - Maker Bot
        return QObject::tr("M2: Program End");
    case M6://Teacup
        return QObject::tr("M6: Tool Change");
    case M17://Teacup - Marlin - Smoothie
        return QObject::tr("M17: Enable/power all steppers motors");
    case M18://Teacup - Marlin(M84) - Smoothie -RepRap Firmware
        return QObject::tr("M18: Disable all steppers motors");
    case M20://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M20: List SDCard");
    case M21://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M21: Initialize SDCard");
    case M22://Teacup - Sprinter - Marlin - Repetier - RepRap Firmware
        return QObject::tr("M22: Release SDCard");
    case M23:////Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M23: Select SD file");
    case M24://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M24: Start/resume SD print");
    case M25://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M25: Pause SD print");
    case M26://Sprinter - Marlin - Repetier - Smoothie(abort) - RepRap Firmware
        return QObject::tr("M26: Set SD position");
    case M27://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M27: Report SD print status");
    case M28://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M28: Begin write to SD card");
    case M29:// Sprinter - Marlin - Repetier - RepRap Firmware
        return QObject::tr("M29: Stop writing to SD card");
    case M30://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M30: Delete a file on the SD card");
    case M31://Marlin
        return QObject::tr("M31: Output time since last M109 or SD card start to serial");
    case M32://Marlin - Smoothie - RepRap Firmware
        return QObject::tr("M32: Select file and start SD prin");
    case M33://Marlin
        return QObject::tr("M33: Get the long name for an SD card file or folder");
    case M34://Marlin
        return QObject::tr("M34: Set SD file sorting options");
    case M36://RepRap Firmware
        return QObject::tr("M36: Return file information");
    case M42://Sprinter - Marlin - Repetier - RepRap Firmware
        return QObject::tr("M42: Switch I/O pin");
    case M48://Marlin
        return QObject::tr("M48: Measure Z-Probe repeatabiliy");
    case M70://MakerBot
        return QObject::tr("M70: Display message");
    case M72://MakerBot
        return QObject::tr("M72: Play a tone or song");
    case M73://MakerBot
        return QObject::tr("M73: Set build percentage");
    case M80://Teacup(automatic) - Sprinter - Marlin - Repetier - RepRap Firmware
        return QObject::tr("M80: ATX Power On");
    case M81://Teacup(automatic) - Sprinter - Marlin - Repetier  - RepRap Firmware
        return QObject::tr("M81: ATX Power Off");
    case M82://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M82: Set extruder to absolute mode");
    case M83://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M83: Set extruder to relative mode");
    case M84://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M84: Stop idle hold");
    case M85://Sprinter - Marlin - Repetier
        return QObject::tr("M85: Set Inactivity shutdown timer");
    case M92:// Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M92: Set axis steps per unit");
    case M93:// Sprinter
        return QObject::tr("M93: Send axis steps per unit");
    case M98: //RepRap Firmware
        return QObject::tr("M98: Call Macro/Subprogram");
    case M99://RepRap Firmware
        return QObject::tr("M99: Return from Macro/Subprogram");
    case M101://Teacup
        return QObject::tr("M101:Turn extruder 1 on Forward, Undo Retraction");
    case M103://Teacup
        return QObject::tr("M103: Turn all extruders off - Extruder Retraction");
    case M104://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("M104: Set Extruder Temperature");
    case M105://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("M105: Get Extruder Temperature");
    case M106://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M106: Fan On");
    case M107:// Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M107: Fan Off");
    case M108://Marlin
        return QObject::tr("M108: Cancel Heating");
    case M109:// Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("M109: Set Extruder Temperature and Wait");
    case M110:// Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M110: Set Current Line Number");
    case M111://Teacup -  Marlin - Repetier - RepRap Firmware
        return QObject::tr("M111: Set Debug Level");
    case M112://Teacup - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M112: Emergency Stop");
    case M114://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("M114: Get Current Position");
    case M115://Teacup - Sprinter - Marlin - Repetier - RepRap Firmware
        return QObject::tr("M115: Get Firmware Version and Capabilities");
    case M116://Teacup - Repetier - RepRap Firmware - MakerBot
        return QObject::tr("M116: Wait");
    case M117:// Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M117: Display Message");
    case M119://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M119: Get Endstop Status");
    case M120://Marlin - Smoothie - RepRap Firmware
        return QObject::tr("M120: Push for Smoothie and RepRap Firmware / Enable Endstop detection for Marlin");
    case M121://Marlin - Smoothie - RepRap Firmware
        return QObject::tr("M121: Pop for Smoothie and RepRap Firmware / Disable Endstop detection for Marlin");
    case M122://RepRap Firmware
        return QObject::tr("M122: Diagnose");
    case M126://Marlin - MakerBot
        return QObject::tr("M126: Open valve");
    case M127://Marlin - MakerBot
        return QObject::tr("M127: Close valve");
    case M130://Teacup
        return QObject::tr("M130: Set PID P value");
    case M131://Teacup
        return QObject::tr("M131: Set PID I value");
    case M132://Teacup - MakerBot
        return QObject::tr("M132: Set PID D value");
    case M133://Teacup - MakerBot
        return QObject::tr("M133: Set PID I limit value");
    case M134://Teacup - MakerBot
        return QObject::tr("M134: Write PID values to EEPROM");
    case M135://RepRap Firmware - MakerBot
        return QObject::tr("M135: Set PID sample interval");
    case M140://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware - MakerBot
        return QObject::tr("M140: Set Bed Temperature - Fast");
    case M141://RepRap Firmware
        return QObject::tr("M141: Set Chamber Temperature - Fast");
    case M143://RepRap Firmware
        return QObject::tr("M143: Maximum hot-end temperature");
    case M144://RepRap Firmware
        return QObject::tr("M144: Stand by your bed");
    case M150://Marlin
        return QObject::tr("M150: Set display color");
    case M163://Repetier > 0.92
        return QObject::tr("M163: Set weight of mixed material");
    case M164://Repetier > 0.92
        return QObject::tr("M164: Store weights");
    case M190://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M190: Wait for bed temperaure to reach target temp");
    case M200://Marlin - Repetier - Smoothie
        return QObject::tr("M200: Set filament diameter");
    case M201://Sprinter - Marlin - Repetier - RepRap Firmware
        return QObject::tr("M201: Set max printing acceleration");
    case M202://Marlin - Repetier
        return QObject::tr("M202: Set max travel acceleration");
    case M203://Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M203: Set maximum feedrate");
    case M204://Sprinter - Marlin - Repetier - Smoothie
        return QObject::tr("M204: Set default acceleration");
    case M205://Sprinter - Marlin - Repetier - Smoothie
        return QObject::tr("M205: Advanced settings");
    case M206://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M206: Offset axes for Sprinter, Marlin, Smoothie, RepRap Firmware / Set eeprom value for Repetier");
    case M207://Marlin - Smoothie
        return QObject::tr("M207: Set retract length ");
    case M208://Marlin - Smoothie
        return QObject::tr("M208: Set unretract length");
    case M209://Marlin - Repetier
        return QObject::tr("M209: Enable automatic retract");
    case M212://Marlin
        return QObject::tr("M212: Set Bed Level Sensor Offset");
    case M218://Marlin
        return QObject::tr("M218: Set Hotend Offset");
    case M220://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M220: Set speed factor override percentage");
    case M221://Teacup - Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M221: Set extrude factor override percentage");
    case M226://Marlin - Repetier
        return QObject::tr("M226: Wait for pin state");
    case M231://Repetier
        return QObject::tr("M231: Set OPS parameter ");
    case M232://Repetier
        return QObject::tr("M232: Read and reset max. advance values");
    case M240: //Marlin
        return QObject::tr("M240: Trigger camera ");
    case M250://Marlin
        return QObject::tr("M250: Set LCD contrast");
    case M251://Repetier
        return QObject::tr("M251: Measure Z steps from homing stop (Delta printers) ");
    case M280://Marlin
        return QObject::tr("M280: Set servo position ");
    case M300://Marlin - Repetier - RepRap Firmware - MakerBot
        return QObject::tr("M300: Play beep sound ");
    case M301://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M301: Set PID parameters");
    case M302://Marlin - Repetier > 0.92 - RepRap Firmware
        return QObject::tr("M302: Allow cold extrudes ");
    case M303://Sprinter - Marlin - Repetier - Smoothie
        return QObject::tr("M303: Run PID tuning ");
    case M304://Marlin - RepRap Firmware
        return QObject::tr("M304: Set PID parameters - Bed ");
    case M305:// Smoothie - RepRap Firmware
        return QObject::tr("M305: Set thermistor and ADC parameters ");
    case M306:// Smoothie
        return QObject::tr("M306: set home offset calculated from toolhead position ");
    case M320://Repeier
        return QObject::tr("M320: Activate autolevel (Repetier) ");
    case M321://Repetier
        return QObject::tr("M321: Deactivate autolevel (Repetier) ");
    case M322://Repetier
        return QObject::tr("M322: Reset autolevel matrix (Repetier) ");
    case M323://Repetier
        return QObject::tr("M323: Distortion correction on/off (Repetier) ");
    case M340://Repetier
        return QObject::tr("M340: Control the servos ");
    case M350://Marlin - Repetier - RepRap Firmware
        return QObject::tr("M350: Set microstepping mode ");
    case M351://Marlin
        return QObject::tr("M351: Toggle MS1 MS2 pins directly ");
    case M355://Repetier > 0.92.2
        return QObject::tr("M355: Turn case lights on/off ");
    case M360://Repetier > 9.92.2
        return QObject::tr("M360: Report firmware configuration ");
    case M361://Smoothie
        return QObject::tr("M361: Move to Theta 90 degree position ");
    case M362://Smoothie
        return QObject::tr("M362: Move to Psi 0 degree position ");
    case M363://Smoothie
        return QObject::tr("M363: Move to Psi 90 degree position ");
    case M364://Smoothie
        return QObject::tr("M364: Move to Psi + Theta 90 degree position");
    case M365://Smoothie
        return QObject::tr("M365: SCARA scaling factor ");
    case M366://Smoothie
        return QObject::tr("M366: SCARA convert trim ");
    case M370://Smoothie
        return QObject::tr("M370: Morgan manual bed level - clear map ");
    case M371://Smoothie
        return QObject::tr("M371: Move to next calibration position ");
    case M372://Smoothie
        return QObject::tr("M372: Record calibration value, and move to next position ");
    case M373://Smoothie
        return QObject::tr("M373: End bed level calibration mode ");
    case M374://Smoothie
        return QObject::tr("M374: Save calibration grid ");
    case M375://Smoothie
        return QObject::tr("M375: Display matrix / Load Matrix ");
    case M380://Marlin
        return QObject::tr("M380: Activate solenoid ");
    case M381://Marlin
        return QObject::tr("M381: Disable all solenoids ");
    case M400://Sprinter - Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M400: Wait for current moves to finish ");
    case M401://Marlin
        return QObject::tr("M401: Lower z-probe ");
    case M402://Marlin
        return QObject::tr("M402: Raise z-probe");
    case M404://Marlin - RepRap Firmware
        return QObject::tr("M404: Filament width and nozzle diameter ");
    case M405://Marlin
        return QObject::tr("M405: Filament Sensor on ");
    case M406://Marlin
        return QObject::tr("M406: Filament Sensor off");
    case M407://Marlin - RepRap Firmware
        return QObject::tr("M407: Display filament diameter ");
    case M408://RepRap Firmware
        return QObject::tr("M408: Report JSON-style response ");
    case M420:
        return QObject::tr("M420: Enable/Disable Mesh Leveling (Marlin)");
    case M450://Repetier
        return QObject::tr("M450: Report Printer Mode ");
    case M451://Repetier
        return QObject::tr("M451: Select FFF Printer Mode");
    case M452://Repetier
        return QObject::tr("M452: Select Laser Printer Mode ");
    case M453://Repetier
        return QObject::tr("M453: Select CNC Printer Mode ");
    case M460://Repetier
        return QObject::tr("M460: Define temperature range for thermistor controlled fan ");
    case M500://Sprinter - Marlin - Repetier - RepRap Firmware
        return QObject::tr("M500: Store parameters in EEPROM");
    case M501://Sprinter - Marlin - Repetier - RepRap Firmware
        return QObject::tr("M501: Read parameters from EEPROM ");
    case M502://Sprinter - Marlin - Repetier - RepRap Firmware
        return QObject::tr("M502: Revert to the default 'factory settings'.");
    case M503://Sprinter - Marlin - RepRap Firmware
        return QObject::tr("M503: Print settings ");
    case M540://Marlin
        return QObject::tr("M540: Enable/Disable 'Stop SD Print on Endstop Hit' ");
    case M550://RepRap Firmware
        return QObject::tr("M550: Set Name ");
    case M551://RepRap Firmware
        return QObject::tr("M551: Set Password ");
    case M552://RepRap Firmware
        return QObject::tr("M552: Set IP address ");
    case M553://RepRap Firmware
        return QObject::tr("M553: Set Netmask ");
    case M554://RepRap Firmware
        return QObject::tr("M554: Set Gateway ");
    case M555://RepRap Firmware
        return QObject::tr("M555: Set compatibility ");
    case M556://RepRap Firmware
        return QObject::tr("M556: Axis compensation ");
    case M557://Smoothie - RepRap Firmware
        return QObject::tr("M557: Set Z probe point ");
    case M558://RepRap Firmware
        return QObject::tr("M558: Set Z probe type ");
    case M559://RepRap Firmware
        return QObject::tr("M559: Upload configuration file ");
    case M560://RepRap Firmware
        return QObject::tr("M560: Upload web page file ");
    case M561://Smoothie - RepRap Firmware
        return QObject::tr("M561: Set Identity Transform ");
    case M562://RepRap Firmware
        return QObject::tr("M562: Reset temperature fault ");
    case M563://RepRap Firmware
        return QObject::tr("M563: Define or remove a tool ");
    case M564://RepRap Firmware
        return QObject::tr("M564: Limit axes ");
    case M565://Smoothie
        return QObject::tr("M565: Set Z probe offset ");
    case M566://RepRap Firmware
        return QObject::tr("M566: Set allowable instantaneous speed change ");
    case M567://RepRap Firmware
        return QObject::tr("M567: Set tool mix ratio");
    case M568://RepRap Firmware
        return QObject::tr("M568: Turn off/on tool mix ratio");
    case M569://RepRap Firmware
        return QObject::tr("M569: Set axis direction and enable values ");
    case M570://RepRap Firmware
        return QObject::tr("M570: Set heater timeout ");
    case M571://RepRap Firmware
        return QObject::tr("M571: Set output on extrude ");
    case M573://RepRap Firmware
        return QObject::tr("M573: Report heater PWM ");
    case M574://RepRap Firmware
        return QObject::tr("M574: Set endstop configuration ");
    case M575://RepRap Firmware
        return QObject::tr("M575: Set serial comms parameters ");
    case M577://RepRap Firmware
        return QObject::tr("M577: Wait until endstop is triggered ");
    case M578://RepRap Firmware
        return QObject::tr("M578: Fire inkjet bits ");
    case M579://RepRap Firmware
        return QObject::tr("M579: Scale Cartesian axes ");
    case M580://RepRap Firmware
        return QObject::tr("M580: Select Roland ");
    case M600://Marlin
        return QObject::tr("M600: Filament change pause");
    case M605://Marlin
        return QObject::tr("M605: Set dual x-carriage movement mode ");
    case M665://Marlin - Smoothie - RepRap Firmware
        return QObject::tr("M665: Set delta configuration ");
    case M666://Marlin - Repetier - Smoothie - RepRap Firmware
        return QObject::tr("M666: Set delta endstop adjustment ");
    case M667://RepRap Firmware
        return QObject::tr("M667: Select CoreXY mode ");
    case M851://Marlin
        return QObject::tr("M851: Set Z-Probe Offset ");
    case M906://RepRap Firmware
        return QObject::tr("M906: Set motor currents");
    case M907://Marlin - Repetier - Smoothie
        return QObject::tr("M907: Set digital trimpot motor ");
    case M908://Marlin - Repetier > 0.92
        return QObject::tr("M908: Control digital trimpot directly");
    case M911://RepRap Firmware
        return QObject::tr("M911: Set power monitor threshold voltages ");
    case M912://RepRap Firmware
        return QObject::tr("M912: Set electronics temperature monitor adjustment");
    case M913://RepRap Firmware
        return QObject::tr("M913: Set motor percentage of normal current ");
    case M928://Marlin
        return QObject::tr("M928: Start SD logging ");
    case M997://RepRap Firmware
        return QObject::tr("M997: Perform in-application firmware update ");
    case M998://RepRap Firmware
        return QObject::tr("M998: Request resend of line");
    case M999://Marlin - Smoothie - RepRap Firmware
        return QObject::tr("M999: Restart after being stopped by error ");
    default:
        return QObject::tr("Not implemented or not supported!");

    }
}

QString toCommand(MCommands gcode, const QString &value)
{

    switch (gcode) {
    case M104: {
        if (!value.isEmpty()) {
            return QStringLiteral("M104 S%1").arg(value);
        } else {
            return QObject::tr("ERROR! M104: It's obligatory have an argument");
        }
    }
    case M105:
        return QStringLiteral("M105");
    case M106:
        return QStringLiteral("M106");
    case M107:
        return QStringLiteral("M107");
    case M112:
        return QStringLiteral("M112");
    case M114:
        return QStringLiteral("M114");
    case M115:
        return QStringLiteral("M115");
    case M116:
        return QStringLiteral("M116");
    case M117: {
        if (!value.isEmpty()) {
            return QStringLiteral("M117 %1").arg(value);
        } else {
            return QObject::tr("ERROR! M117: It's obligatory have an argument");
        }
    }
    case M119:
        return QStringLiteral("M119");
    case M140: {
        if (!value.isEmpty()) {
            return QStringLiteral("M140 S%1").arg(value);
        } else {
            return QObject::tr("ERROR! M140: It's obligatory have an argument");
        }
    }
    case M220: {
        if (!value.isEmpty()) {
            return QStringLiteral("M220 S%1").arg(value);
        } else {
            return QObject::tr("ERROR! M220: It's obligatory have an argument");
        }
    }
    case M221: {
        if (!value.isEmpty()) {
            return QStringLiteral("M221 S%1").arg(value);
        } else {
            return QObject::tr("ERROR! M221: It's obligatory have an argument");
        }
    }
    default:
        return QObject::tr("Not supported or implemented!");
    }
}
}

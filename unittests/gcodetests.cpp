#include "gcodetests.h"
#include "../src/gcodecommands.h"
#include <algorithm>

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
    QVERIFY(GCode::toCommand(GCode::G2) == QObject::tr("Not implemented or not supported!"));
}

void GCodeTests::string_G0()
{
    QVERIFY(GCode::toString(GCode::G0) == QObject::tr("G0: Rapid linear move"));
}

void GCodeTests::string_G1()
{
    QVERIFY(GCode::toString(GCode::G1) == QObject::tr("G1: Linear move"));
}

void GCodeTests::string_G2()
{
    QVERIFY(GCode::toString(GCode::G2) == QObject::tr("G2: Controlled Arc Move clockwise"));
}

void GCodeTests::string_G3()
{
    QVERIFY(GCode::toString(GCode::G3) == QObject::tr("G3: Controlled Arc Move counterclockwise"));
}

void GCodeTests::string_G4()
{
    QVERIFY(GCode::toString(GCode::G4) == QObject::tr("G4: Dwell"));
}

void GCodeTests::string_G10()
{
    QVERIFY(GCode::toString(GCode::G10) == QObject::tr("G10: Retract"));
}

void GCodeTests::string_G11()
{
    QVERIFY(GCode::toString(GCode::G11) == QObject::tr("G11: Unretract"));
}

void GCodeTests::string_G20()
{
    QVERIFY(GCode::toString(GCode::G20) == QObject::tr("G20: Set units to inches"));
}

void GCodeTests::string_G21()
{
    QVERIFY(GCode::toString(GCode::G21) == QObject::tr("G21: Set units to millimeters"));
}

void GCodeTests::string_G28()
{
    QVERIFY(GCode::toString(GCode::G28) == QObject::tr("G28: Move to Origin Home"));
}

void GCodeTests::string_G29()
{
    QVERIFY(GCode::toString(GCode::G29) == QObject::tr("G29: Detailed Z-Probe"));
}

void GCodeTests::string_G30()
{
    QVERIFY(GCode::toString(GCode::G30) == QObject::tr("G30: Single Z-Probe"));
}

void GCodeTests::string_G31()
{
    QVERIFY(GCode::toString(GCode::G31) == QObject::tr("G31: Set or report current probe status / Dock Z Probe sled for Marlin"));
}

void GCodeTests::string_G32()
{
    QVERIFY(GCode::toString(GCode::G32) == QObject::tr("G32: Probe Z and calculate Z plane(Bed Leveling)/ UnDoc Z Probe sled for Marlin"));
}

void GCodeTests::string_G33()
{
    QVERIFY(GCode::toString(GCode::G33) == QObject::tr("G33: Measure/List/Adjust Distortion Matrix"));
}

void GCodeTests::string_G90()
{
    QVERIFY(GCode::toString(GCode::G90) == QObject::tr("G90: Set to absolute positioning"));
}

void GCodeTests::string_G91()
{
    QVERIFY(GCode::toString(GCode::G91) == QObject::tr("G91: Set to relative positioning"));
}

void GCodeTests::string_G92()
{
    QVERIFY(GCode::toString(GCode::G92) == QObject::tr("G92: Set position"));
}

void GCodeTests::string_G100()
{
    QVERIFY(GCode::toString(GCode::G100) == QObject::tr("G100: Calibrate floor or rod radius"));
}

void GCodeTests::string_G130()
{
    QVERIFY(GCode::toString(GCode::G130) == QObject::tr("G130: Set digital potentiometer value"));
}

void GCodeTests::string_G131()
{
    QVERIFY(GCode::toString(GCode::G131) == QObject::tr("G131: Recase Move offset"));
}

void GCodeTests::string_G132()
{
    QVERIFY(GCode::toString(GCode::G132) == QObject::tr("G132: Calibrate endstops offsets"));
}

void GCodeTests::string_G133()
{
    QVERIFY(GCode::toString(GCode::G133) == QObject::tr("G133: Measure steps to top"));
}

void GCodeTests::string_G161()
{
    QVERIFY(GCode::toString(GCode::G161) == QObject::tr("G161: Home axis to minimum"));
}

void GCodeTests::string_G162()
{
    QVERIFY(GCode::toString(GCode::G162) == QObject::tr("G162: Home axis to maximum"));
}

void GCodeTests::command_M104()
{
    QVERIFY(GCode::toCommand(GCode::M104) == QStringLiteral("ERROR! M104: It's obligatory to have an argument"));
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
    QVERIFY(GCode::toCommand(GCode::M117) == QStringLiteral("ERROR! M117: It's obligatory to have an argument"));
    QVERIFY(GCode::toCommand(GCode::M117, QStringLiteral("100")) == QStringLiteral("M117 100"));
}

void GCodeTests::command_M119()
{
    QVERIFY(GCode::toCommand(GCode::M119) == QStringLiteral("M119"));
}

void GCodeTests::command_M140()
{
    QVERIFY(GCode::toCommand(GCode::M140) == QStringLiteral("ERROR! M140: It's obligatory to have an argument"));
    QVERIFY(GCode::toCommand(GCode::M140, QStringLiteral("100")) == QStringLiteral("M140 S100"));
}

void GCodeTests::command_M220()
{
    QVERIFY(GCode::toCommand(GCode::M220) == QStringLiteral("ERROR! M220: It's obligatory to have an argument"));
    QVERIFY(GCode::toCommand(GCode::M220, QStringLiteral("100")) == QStringLiteral("M220 S100"));
}

void GCodeTests::command_M221()
{
    QVERIFY(GCode::toCommand(GCode::M221) == QStringLiteral("ERROR! M221: It's obligatory to have an argument"));
    QVERIFY(GCode::toCommand(GCode::M221, QStringLiteral("100")) == QStringLiteral("M221 S100"));
}

void GCodeTests::command_unsupportedM()
{
    QVERIFY(GCode::toCommand(GCode::M999) == QObject::tr("Not supported or implemented!"));
}

void GCodeTests::string_M0()
{
    QVERIFY(GCode::toString(GCode::M0) == QObject::tr("M0: Stop or unconditional stop"));
}

void GCodeTests::string_M1()
{
    QVERIFY(GCode::toString(GCode::M1) == QObject::tr("M1: Sleep or unconditional stop"));
}
void GCodeTests::string_M2()
{
    QVERIFY(GCode::toString(GCode::M2) == QObject::tr("M2: Program End"));
}

void GCodeTests::string_M6()
{
    QVERIFY(GCode::toString(GCode::M6) == QObject::tr("M6: Tool Change"));
}

void GCodeTests::string_M17()
{
    QVERIFY(GCode::toString(GCode::M17) == QObject::tr("M17: Enable/power all steppers motors"));
}

void GCodeTests::string_M18()
{
    QVERIFY(GCode::toString(GCode::M18) == QObject::tr("M18: Disable all steppers motors"));
}

void GCodeTests::string_M20()
{
    QVERIFY(GCode::toString(GCode::M20) == QObject::tr("M20: List SDCard"));
}

void GCodeTests::string_M21()
{
    QVERIFY(GCode::toString(GCode::M21) == QObject::tr("M21: Initialize SDCard"));
}

void GCodeTests::string_M22()
{
    QVERIFY(GCode::toString(GCode::M22) == QObject::tr("M22: Release SDCard"));
}

void GCodeTests::string_M23()
{
    QVERIFY(GCode::toString(GCode::M23) == QObject::tr("M23: Select SD file"));
}

void GCodeTests::string_M24()
{
    QVERIFY(GCode::toString(GCode::M24) == QObject::tr("M24: Start/resume SD print"));
}

void GCodeTests::string_M25()
{
    QVERIFY(GCode::toString(GCode::M25) == QObject::tr("M25: Pause SD print"));
}

void GCodeTests::string_M26()
{
    QVERIFY(GCode::toString(GCode::M26) == QObject::tr("M26: Set SD position"));
}

void GCodeTests::string_M27()
{
    QVERIFY(GCode::toString(GCode::M27) == QObject::tr("M27: Report SD print status"));
}

void GCodeTests::string_M28()
{
    QVERIFY(GCode::toString(GCode::M28) == QObject::tr("M28: Begin write to SD card"));
}

void GCodeTests::string_M29()
{
    QVERIFY(GCode::toString(GCode::M29) == QObject::tr("M29: Stop writing to SD card"));
}

void GCodeTests::string_M30()
{
    QVERIFY(GCode::toString(GCode::M30) == QObject::tr("M30: Delete a file on the SD card"));
}

void GCodeTests::string_M31()
{
    QVERIFY(GCode::toString(GCode::M31) == QObject::tr("M31: Output time since last M109 or SD card start to serial"));
}

void GCodeTests::string_M32()
{
    QVERIFY(GCode::toString(GCode::M32) == QObject::tr("M32: Select file and start SD print"));
}

void GCodeTests::string_M33()
{
    QVERIFY(GCode::toString(GCode::M33) == QObject::tr("M33: Get the long name for an SD card file or folder"));

}

void GCodeTests::string_M34()
{
    QVERIFY(GCode::toString(GCode::M34) == QObject::tr("M34: Set SD file sorting options"));
}

void GCodeTests::string_M36()
{
    QVERIFY(GCode::toString(GCode::M36) == QObject::tr("M36: Return file information"));
}

void GCodeTests::string_M42()
{
    QVERIFY(GCode::toString(GCode::M42) == QObject::tr("M42: Switch I/O pin"));
}

void GCodeTests::string_M48()
{
    QVERIFY(GCode::toString(GCode::M48) == QObject::tr("M48: Measure Z-Probe repeatability"));
}

void GCodeTests::string_M70()
{
    QVERIFY(GCode::toString(GCode::M70) == QObject::tr("M70: Display message"));
}

void GCodeTests::string_M72()
{
    QVERIFY(GCode::toString(GCode::M72) == QObject::tr("M72: Play a tone or song"));
}

void GCodeTests::string_M73()
{
    QVERIFY(GCode::toString(GCode::M73) == QObject::tr("M73: Set build percentage"));
}

void GCodeTests::string_M80()
{
    QVERIFY(GCode::toString(GCode::M80) == QObject::tr("M80: ATX Power On"));
}

void GCodeTests::string_M81()
{
    QVERIFY(GCode::toString(GCode::M81) == QObject::tr("M81: ATX Power Off"));
}

void GCodeTests::string_M82()
{
    QVERIFY(GCode::toString(GCode::M82) == QObject::tr("M82: Set extruder to absolute mode"));
}

void GCodeTests::string_M83()
{
    QVERIFY(GCode::toString(GCode::M83) == QObject::tr("M83: Set extruder to relative mode"));
}

void GCodeTests::string_M84()
{
    QVERIFY(GCode::toString(GCode::M84) == QObject::tr("M84: Stop idle hold"));
}

void GCodeTests::string_M85()
{
    QVERIFY(GCode::toString(GCode::M85) == QObject::tr("M85: Set Inactivity shutdown timer"));
}

void GCodeTests::string_M92()
{
    QVERIFY(GCode::toString(GCode::M92) == QObject::tr("M92: Set axis steps per unit"));
}

void GCodeTests::string_M93()
{
    QVERIFY(GCode::toString(GCode::M93) == QObject::tr("M93: Send axis steps per unit"));
}

void GCodeTests::string_M98()
{
    QVERIFY(GCode::toString(GCode::M98) == QObject::tr("M98: Call Macro/Subprogram"));
}

void GCodeTests::string_M99()
{
    QVERIFY(GCode::toString(GCode::M99) == QObject::tr("M99: Return from Macro/Subprogram"));
}

void GCodeTests::string_M101()
{
    QVERIFY(GCode::toString(GCode::M101) == QObject::tr("M101: Turn extruder 1 on Forward, Undo Retraction"));
}

void GCodeTests::string_M103()
{
    QVERIFY(GCode::toString(GCode::M103) == QObject::tr("M103: Turn all extruders off - Extruder Retraction"));
}

void GCodeTests::string_M104()
{
    QVERIFY(GCode::toString(GCode::M104) == QObject::tr("M104: Set Extruder Temperature"));
}

void GCodeTests::string_M105()
{
    QVERIFY(GCode::toString(GCode::M105) == QObject::tr("M105: Get Extruder Temperature"));
}

void GCodeTests::string_M106()
{
    QVERIFY(GCode::toString(GCode::M106) == QObject::tr("M106: Fan On"));
}

void GCodeTests::string_M107()
{
    QVERIFY(GCode::toString(GCode::M107) == QObject::tr("M107: Fan Off"));
}

void GCodeTests::string_M108()
{
    QVERIFY(GCode::toString(GCode::M108) == QObject::tr("M108: Cancel Heating"));
}

void GCodeTests::string_M109()
{
    QVERIFY(GCode::toString(GCode::M109) == QObject::tr("M109: Set Extruder Temperature and Wait"));
}

void GCodeTests::string_M110()
{
    QVERIFY(GCode::toString(GCode::M110) == QObject::tr("M110: Set Current Line Number"));
}

void GCodeTests::string_M111()
{
    QVERIFY(GCode::toString(GCode::M111) == QObject::tr("M111: Set Debug Level"));
}

void GCodeTests::string_M112()
{
    QVERIFY(GCode::toString(GCode::M112) == QObject::tr("M112: Emergency Stop"));
}

void GCodeTests::string_M114()
{
    QVERIFY(GCode::toString(GCode::M114) == QObject::tr("M114: Get Current Position"));
}

void GCodeTests::string_M115()
{
    QVERIFY(GCode::toString(GCode::M115) == QObject::tr("M115: Get Firmware Version and Capabilities"));
}

void GCodeTests::string_M116()
{
    QVERIFY(GCode::toString(GCode::M116) == QObject::tr("M116: Wait"));
}

void GCodeTests::string_M117()
{
    QVERIFY(GCode::toString(GCode::M117) == QObject::tr("M117: Display Message"));
}

void GCodeTests::string_M119()
{
    QVERIFY(GCode::toString(GCode::M119) == QObject::tr("M119: Get Endstop Status"));
}

void GCodeTests::string_M120()
{
    QVERIFY(GCode::toString(GCode::M120) == QObject::tr("M120: Push for Smoothie and RepRap Firmware / Enable Endstop detection for Marlin"));
}

void GCodeTests::string_M121()
{
    QVERIFY(GCode::toString(GCode::M121) == QObject::tr("M121: Pop for Smoothie and RepRap Firmware / Disable Endstop detection for Marlin"));
}

void GCodeTests::string_M122()
{
    QVERIFY(GCode::toString(GCode::M122) == QObject::tr("M122: Diagnose"));
}

void GCodeTests::string_M126()
{
    QVERIFY(GCode::toString(GCode::M126) == QObject::tr("M126: Open valve"));
}

void GCodeTests::string_M127()
{
    QVERIFY(GCode::toString(GCode::M127) == QObject::tr("M127: Close valve"));
}

void GCodeTests::string_M130()
{
    QVERIFY(GCode::toString(GCode::M130) == QObject::tr("M130: Set PID P value"));
}

void GCodeTests::string_M131()
{
    QVERIFY(GCode::toString(GCode::M131) == QObject::tr("M131: Set PID I value"));
}

void GCodeTests::string_M132()
{
    QVERIFY(GCode::toString(GCode::M132) == QObject::tr("M132: Set PID D value"));
}

void GCodeTests::string_M133()
{
    QVERIFY(GCode::toString(GCode::M133) == QObject::tr("M133: Set PID I limit value"));
}

void GCodeTests::string_M134()
{
    QVERIFY(GCode::toString(GCode::M134) == QObject::tr("M134: Write PID values to EEPROM"));
}

void GCodeTests::string_M135()
{
    QVERIFY(GCode::toString(GCode::M135) == QObject::tr("M135: Set PID sample interval"));
}

void GCodeTests::string_M140()
{
    QVERIFY(GCode::toString(GCode::M140) == QObject::tr("M140: Set Bed Temperature - Fast"));
}

void GCodeTests::string_M141()
{
    QVERIFY(GCode::toString(GCode::M141) == QObject::tr("M141: Set Chamber Temperature - Fast"));
}

void GCodeTests::string_M143()
{
    QVERIFY(GCode::toString(GCode::M143) == QObject::tr("M143: Maximum hot-end temperature"));
}

void GCodeTests::string_M144()
{
    QVERIFY(GCode::toString(GCode::M144) == QObject::tr("M144: Stand by your bed"));
}

void GCodeTests::string_M150()
{
    QVERIFY(GCode::toString(GCode::M150) == QObject::tr("M150: Set display color"));
}

void GCodeTests::string_M163()
{
    QVERIFY(GCode::toString(GCode::M163) == QObject::tr("M163: Set weight of mixed material"));
}

void GCodeTests::string_M164()
{
    QVERIFY(GCode::toString(GCode::M164) == QObject::tr("M164: Store weights"));
}

void GCodeTests::string_M190()
{
    QVERIFY(GCode::toString(GCode::M190) == QObject::tr("M190: Wait for bed temperature to reach target temp"));
}

void GCodeTests::string_M200()
{
    QVERIFY(GCode::toString(GCode::M200) == QObject::tr("M200: Set filament diameter"));
}

void GCodeTests::string_M201()
{
    QVERIFY(GCode::toString(GCode::M201) == QObject::tr("M201: Set max printing acceleration"));
}

void GCodeTests::string_M202()
{
    QVERIFY(GCode::toString(GCode::M202) == QObject::tr("M202: Set max travel acceleration"));
}

void GCodeTests::string_M203()
{
    QVERIFY(GCode::toString(GCode::M203) == QObject::tr("M203: Set maximum feedrate"));
}

void GCodeTests::string_M204()
{
    QVERIFY(GCode::toString(GCode::M204) == QObject::tr("M204: Set default acceleration"));
}

void GCodeTests::string_M205()
{
    QVERIFY(GCode::toString(GCode::M205) == QObject::tr("M205: Advanced settings"));
}

void GCodeTests::string_M206()
{
    QVERIFY(GCode::toString(GCode::M206) == QObject::tr("M206: Offset axes for Sprinter, Marlin, Smoothie, RepRap Firmware / Set eeprom value for Repetier"));
}

void GCodeTests::string_M207()
{
    QVERIFY(GCode::toString(GCode::M207) == QObject::tr("M207: Set retract length"));
}

void GCodeTests::string_M208()
{
    QVERIFY(GCode::toString(GCode::M208) == QObject::tr("M208: Set unretract length"));
}

void GCodeTests::string_M209()
{
    QVERIFY(GCode::toString(GCode::M209) == QObject::tr("M209: Enable automatic retract"));
}

void GCodeTests::string_M212()
{
    QVERIFY(GCode::toString(GCode::M212) == QObject::tr("M212: Set Bed Level Sensor Offset"));
}

void GCodeTests::string_M218()
{
    QVERIFY(GCode::toString(GCode::M218) == QObject::tr("M218: Set Hotend Offset"));
}

void GCodeTests::string_M220()
{
    QVERIFY(GCode::toString(GCode::M220) == QObject::tr("M220: Set speed factor override percentage"));
}

void GCodeTests::string_M221()
{
    QVERIFY(GCode::toString(GCode::M221) == QObject::tr("M221: Set extrude factor override percentage"));
}

void GCodeTests::string_M226()
{
    QVERIFY(GCode::toString(GCode::M226) == QObject::tr("M226: Wait for pin state"));
}

void GCodeTests::string_M231()
{
    QVERIFY(GCode::toString(GCode::M231) == QObject::tr("M231: Set OPS parameter"));
}

void GCodeTests::string_M232()
{
    QVERIFY(GCode::toString(GCode::M232) == QObject::tr("M232: Read and reset max. advance values"));
}

void GCodeTests::string_M240()
{
    QVERIFY(GCode::toString(GCode::M240) == QObject::tr("M240: Trigger camera"));
}

void GCodeTests::string_M250()
{
    QVERIFY(GCode::toString(GCode::M250) == QObject::tr("M250: Set LCD contrast"));
}

void GCodeTests::string_M251()
{
    QVERIFY(GCode::toString(GCode::M251) == QObject::tr("M251: Measure Z steps from homing stop (Delta printers)"));
}

void GCodeTests::string_M280()
{
    QVERIFY(GCode::toString(GCode::M280) == QObject::tr("M280: Set servo position"));
}

void GCodeTests::string_M300()
{
    QVERIFY(GCode::toString(GCode::M300) == QObject::tr("M300: Play beep sound"));
}

void GCodeTests::string_M301()
{
    QVERIFY(GCode::toString(GCode::M301) == QObject::tr("M301: Set PID parameters"));
}

void GCodeTests::string_M302()
{
    QVERIFY(GCode::toString(GCode::M302) == QObject::tr("M302: Allow cold extrudes "));
}

void GCodeTests::string_M303()
{
    QVERIFY(GCode::toString(GCode::M303) == QObject::tr("M303: Run PID tuning"));
}

void GCodeTests::string_M304()
{
    QVERIFY(GCode::toString(GCode::M304) == QObject::tr("M304: Set PID parameters - Bed"));
}

void GCodeTests::string_M305()
{
    QVERIFY(GCode::toString(GCode::M305) == QObject::tr("M305: Set thermistor and ADC parameters"));
}

void GCodeTests::string_M306()
{
    QVERIFY(GCode::toString(GCode::M306) == QObject::tr("M306: set home offset calculated from toolhead position"));
}

void GCodeTests::string_M320()
{
    QVERIFY(GCode::toString(GCode::M320) == QObject::tr("M320: Activate autolevel (Repetier)"));
}

void GCodeTests::string_M321()
{
    QVERIFY(GCode::toString(GCode::M321) == QObject::tr("M321: Deactivate autolevel (Repetier)"));
}

void GCodeTests::string_M322()
{
    QVERIFY(GCode::toString(GCode::M322) == QObject::tr("M322: Reset autolevel matrix (Repetier)"));
}

void GCodeTests::string_M323()
{
    QVERIFY(GCode::toString(GCode::M323) == QObject::tr("M323: Distortion correction on/off (Repetier)"));
}

void GCodeTests::string_M340()
{
    QVERIFY(GCode::toString(GCode::M340) == QObject::tr("M340: Control the servos"));
}

void GCodeTests::string_M350()
{
    QVERIFY(GCode::toString(GCode::M350) == QObject::tr("M350: Set microstepping mode"));
}

void GCodeTests::string_M351()
{
    QVERIFY(GCode::toString(GCode::M351) == QObject::tr("M351: Toggle MS1 MS2 pins directly"));
}

void GCodeTests::string_M355()
{
    QVERIFY(GCode::toString(GCode::M355) == QObject::tr("M355: Turn case lights on/off"));
}

void GCodeTests::string_M360()
{
    QVERIFY(GCode::toString(GCode::M360) == QObject::tr("M360: Report firmware configuration"));
}

void GCodeTests::string_M361()
{
    QVERIFY(GCode::toString(GCode::M361) == QObject::tr("M361: Move to Theta 90 degree position"));
}

void GCodeTests::string_M362()
{
    QVERIFY(GCode::toString(GCode::M362) == QObject::tr("M362: Move to Psi 0 degree position"));
}
void GCodeTests::string_M363()
{
    QVERIFY(GCode::toString(GCode::M363) == QObject::tr("M363: Move to Psi 90 degree position"));
}

void GCodeTests::string_M364()
{
    QVERIFY(GCode::toString(GCode::M364) == QObject::tr("M364: Move to Psi + Theta 90 degree position"));
}

void GCodeTests::string_M365()
{
    QVERIFY(GCode::toString(GCode::M365) == QObject::tr("M365: SCARA scaling factor"));
}

void GCodeTests::string_M366()
{
    QVERIFY(GCode::toString(GCode::M366) == QObject::tr("M366: SCARA convert trim"));
}

void GCodeTests::string_M370()
{
    QVERIFY(GCode::toString(GCode::M370) == QObject::tr("M370: Morgan manual bed level - clear map"));
}

void GCodeTests::string_M371()
{
    QVERIFY(GCode::toString(GCode::M371) == QObject::tr("M371: Move to next calibration position"));
}

void GCodeTests::string_M372()
{
    QVERIFY(GCode::toString(GCode::M372) == QObject::tr("M372: Record calibration value, and move to next position"));
}

void GCodeTests::string_M373()
{
    QVERIFY(GCode::toString(GCode::M373) == QObject::tr("M373: End bed level calibration mode"));
}

void GCodeTests::string_M374()
{
    QVERIFY(GCode::toString(GCode::M374) == QObject::tr("M374: Save calibration grid"));
}

void GCodeTests::string_M375()
{
    QVERIFY(GCode::toString(GCode::M375) == QObject::tr("M375: Display matrix / Load Matrix"));
}

void GCodeTests::string_M380()
{
    QVERIFY(GCode::toString(GCode::M380) == QObject::tr("M380: Activate solenoid"));
}

void GCodeTests::string_M381()
{
    QVERIFY(GCode::toString(GCode::M381) == QObject::tr("M381: Disable all solenoids"));
}

void GCodeTests::string_M400()
{
    QVERIFY(GCode::toString(GCode::M400) == QObject::tr("M400: Wait for current moves to finish"));
}

void GCodeTests::string_M401()
{
    QVERIFY(GCode::toString(GCode::M401) == QObject::tr("M401: Lower z-probe"));
}

void GCodeTests::string_M402()
{
    QVERIFY(GCode::toString(GCode::M402) == QObject::tr("M402: Raise z-probe"));
}

void GCodeTests::string_M404()
{
    QVERIFY(GCode::toString(GCode::M404) == QObject::tr("M404: Filament width and nozzle diameter"));
}

void GCodeTests::string_M405()
{
    QVERIFY(GCode::toString(GCode::M405) == QObject::tr("M405: Filament Sensor on"));
}

void GCodeTests::string_M406()
{
    QVERIFY(GCode::toString(GCode::M406) == QObject::tr("M406: Filament Sensor off"));
}

void GCodeTests::string_M407()
{
    QVERIFY(GCode::toString(GCode::M407) == QObject::tr("M407: Display filament diameter"));
}

void GCodeTests::string_M408()
{
    QVERIFY(GCode::toString(GCode::M408) == QObject::tr("M408: Report JSON-style response"));
}

void GCodeTests::string_M420()
{
    QVERIFY(GCode::toString(GCode::M420) == QObject::tr("M420: Enable/Disable Mesh Leveling (Marlin)"));
}

void GCodeTests::string_M450()
{
    QVERIFY(GCode::toString(GCode::M450) == QObject::tr("M450: Report Printer Mode"));
}

void GCodeTests::string_M451()
{
    QVERIFY(GCode::toString(GCode::M451) == QObject::tr("M451: Select FFF Printer Mode"));
}

void GCodeTests::string_M452()
{
    QVERIFY(GCode::toString(GCode::M452) == QObject::tr("M452: Select Laser Printer Mode"));
}

void GCodeTests::string_M453()
{
    QVERIFY(GCode::toString(GCode::M453) == QObject::tr("M453: Select CNC Printer Mode"));
}

void GCodeTests::string_M460()
{
    QVERIFY(GCode::toString(GCode::M460) == QObject::tr("M460: Define temperature range for thermistor controlled fan"));
}

void GCodeTests::string_M500()
{
    QVERIFY(GCode::toString(GCode::M500) == QObject::tr("M500: Store parameters in EEPROM"));
}

void GCodeTests::string_M501()
{
    QVERIFY(GCode::toString(GCode::M501) == QObject::tr("M501: Read parameters from EEPROM"));
}

void GCodeTests::string_M502()
{
    QVERIFY(GCode::toString(GCode::M502) == QObject::tr("M502: Revert to the default 'factory settings'."));
}

void GCodeTests::string_M503()
{
    QVERIFY(GCode::toString(GCode::M503) == QObject::tr("M503: Print settings "));
}

void GCodeTests::string_M540()
{
    QVERIFY(GCode::toString(GCode::M540) == QObject::tr("M540: Enable/Disable 'Stop SD Print on Endstop Hit'"));
}

void GCodeTests::string_M550()
{
    QVERIFY(GCode::toString(GCode::M550) == QObject::tr("M550: Set Name"));
}

void GCodeTests::string_M551()
{
    QVERIFY(GCode::toString(GCode::M551) == QObject::tr("M551: Set Password"));
}

void GCodeTests::string_M552()
{
    QVERIFY(GCode::toString(GCode::M552) == QObject::tr("M552: Set IP address"));
}

void GCodeTests::string_M553()
{
    QVERIFY(GCode::toString(GCode::M553) == QObject::tr("M553: Set Netmask"));
}

void GCodeTests::string_M554()
{
    QVERIFY(GCode::toString(GCode::M554) == QObject::tr("M554: Set Gateway"));
}

void GCodeTests::string_M555()
{
    QVERIFY(GCode::toString(GCode::M555) == QObject::tr("M555: Set compatibility"));
}

void GCodeTests::string_M556()
{
    QVERIFY(GCode::toString(GCode::M556) == QObject::tr("M556: Axis compensation"));
}

void GCodeTests::string_M557()
{
    QVERIFY(GCode::toString(GCode::M557) == QObject::tr("M557: Set Z probe point"));
}

void GCodeTests::string_M558()
{
    QVERIFY(GCode::toString(GCode::M558) == QObject::tr("M558: Set Z probe type"));
}

void GCodeTests::string_M559()
{
    QVERIFY(GCode::toString(GCode::M559) == QObject::tr("M559: Upload configuration file"));
}

void GCodeTests::string_M560()
{
    QVERIFY(GCode::toString(GCode::M560) == QObject::tr("M560: Upload web page file"));
}

void GCodeTests::string_M561()
{
    QVERIFY(GCode::toString(GCode::M561) == QObject::tr("M561: Set Identity Transform"));
}

void GCodeTests::string_M562()
{
    QVERIFY(GCode::toString(GCode::M562) == QObject::tr("M562: Reset temperature fault"));
}

void GCodeTests::string_M563()
{
    QVERIFY(GCode::toString(GCode::M563) == QObject::tr("M563: Define or remove a tool"));
}

void GCodeTests::string_M564()
{
    QVERIFY(GCode::toString(GCode::M564) == QObject::tr("M564: Limit axes"));
}

void GCodeTests::string_M565()
{
    QVERIFY(GCode::toString(GCode::M565) == QObject::tr("M565: Set Z probe offset"));
}

void GCodeTests::string_M566()
{
    QVERIFY(GCode::toString(GCode::M566) == QObject::tr("M566: Set allowable instantaneous speed change"));
}

void GCodeTests::string_M567()
{
    QVERIFY(GCode::toString(GCode::M567) == QObject::tr("M567: Set tool mix ratio"));
}

void GCodeTests::string_M568()
{
    QVERIFY(GCode::toString(GCode::M568) == QObject::tr("M568: Turn off/on tool mix ratio"));
}

void GCodeTests::string_M569()
{
    QVERIFY(GCode::toString(GCode::M569) == QObject::tr("M569: Set axis direction and enable values"));
}

void GCodeTests::string_M570()
{
    QVERIFY(GCode::toString(GCode::M570) == QObject::tr("M570: Set heater timeout"));
}

void GCodeTests::string_M571()
{
    QVERIFY(GCode::toString(GCode::M571) == QObject::tr("M571: Set output on extrude"));
}

void GCodeTests::string_M573()
{
    QVERIFY(GCode::toString(GCode::M573) == QObject::tr("M573: Report heater PWM"));
}

void GCodeTests::string_M574()
{
    QVERIFY(GCode::toString(GCode::M574) == QObject::tr("M574: Set endstop configuration"));
}

void GCodeTests::string_M575()
{
    QVERIFY(GCode::toString(GCode::M575) == QObject::tr("M575: Set serial comms parameters"));
}

void GCodeTests::string_M577()
{
    QVERIFY(GCode::toString(GCode::M577) == QObject::tr("M577: Wait until endstop is triggered"));
}

void GCodeTests::string_M578()
{
    QVERIFY(GCode::toString(GCode::M578) == QObject::tr("M578: Fire inkjet bits"));
}

void GCodeTests::string_M579()
{
    QVERIFY(GCode::toString(GCode::M579) == QObject::tr("M579: Scale Cartesian axes"));
}

void GCodeTests::string_M580()
{
    QVERIFY(GCode::toString(GCode::M580) == QObject::tr("M580: Select Roland"));
}

void GCodeTests::string_M600()
{
    QVERIFY(GCode::toString(GCode::M600) == QObject::tr("M600: Filament change pause"));
}

void GCodeTests::string_M605()
{
    QVERIFY(GCode::toString(GCode::M605) == QObject::tr("M605: Set dual x-carriage movement mode"));
}

void GCodeTests::string_M665()
{
    QVERIFY(GCode::toString(GCode::M665) == QObject::tr("M665: Set delta configuration"));
}

void GCodeTests::string_M666()
{
    QVERIFY(GCode::toString(GCode::M666) == QObject::tr("M666: Set delta endstop adjustment"));
}

void GCodeTests::string_M667()
{
    QVERIFY(GCode::toString(GCode::M667) == QObject::tr("M667: Select CoreXY mode"));
}

void GCodeTests::string_M851()
{
    QVERIFY(GCode::toString(GCode::M851) == QObject::tr("M851: Set Z-Probe Offset"));
}

void GCodeTests::string_M906()
{
    QVERIFY(GCode::toString(GCode::M906) == QObject::tr("M906: Set motor currents"));
}

void GCodeTests::string_M907()
{
    QVERIFY(GCode::toString(GCode::M907) == QObject::tr("M907: Set digital trimpot motor"));
}

void GCodeTests::string_M908()
{
    QVERIFY(GCode::toString(GCode::M908) == QObject::tr("M908: Control digital trimpot directly"));
}

void GCodeTests::string_M911()
{
    QVERIFY(GCode::toString(GCode::M911) == QObject::tr("M911: Set power monitor threshold voltages"));
}

void GCodeTests::string_M912()
{
    QVERIFY(GCode::toString(GCode::M912) == QObject::tr("M912: Set electronics temperature monitor adjustment"));
}

void GCodeTests::string_M913()
{
    QVERIFY(GCode::toString(GCode::M913) == QObject::tr("M913: Set motor percentage of normal current"));
}

void GCodeTests::string_M928()
{
    QVERIFY(GCode::toString(GCode::M928) == QObject::tr("M928: Start SD logging"));
}

void GCodeTests::string_M997()
{
    QVERIFY(GCode::toString(GCode::M997) == QObject::tr("M997: Perform in-application firmware update"));
}

void GCodeTests::string_M998()
{
    QVERIFY(GCode::toString(GCode::M998) == QObject::tr("M998: Request resend of line"));
}

void GCodeTests::string_M999()
{
    QVERIFY(GCode::toString(GCode::M999) == QObject::tr("M999: Restart after being stopped by error"));
}

QTEST_MAIN(GCodeTests)

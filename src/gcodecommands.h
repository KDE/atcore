#pragma once

//QtIncludes
#include <QString>

namespace GCode{

    namespace GCommands {
        enum Enum{
            G0,
            G1,
            G2,
            G3,
            G4,
            G10,
            G11,
            G20,
            G21,
            G22,
            G23,
            G28,
            G29,
            G30,
            G31,
            G32,
            G33,
            G90,
            G91,
            G92,
            G100,
            G130,
            G131,
            G132,
            G133,
            G161,
            G162
        };
        QString toString( GCode::GCommands::Enum gcode);
        QString toCommand(QChar& c = QLatin1Char());
    }

    namespace MCommands {
        enum Enum{

        };
    }
}

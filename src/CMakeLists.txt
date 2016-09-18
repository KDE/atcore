
set(AtCoreLib_SRCS
    atcore.cpp
    seriallayer.cpp
    gcodecommands.cpp
    protocollayer.cpp
)

add_library(AtCoreLib STATIC ${AtCoreLib_SRCS})
target_link_libraries(AtCoreLib Qt5::Core Qt5::SerialPort)

include_directories(..)

set(RepetierPlugin_SRCS repetierplugin.cpp)
add_library(repetier SHARED ${RepetierPlugin_SRCS})
target_link_libraries(repetier Qt5::Core KF5::AtCore)

set(GrblPlugin_SRCS grblplugin.cpp)
add_library(grbl SHARED ${GrblPlugin_SRCS})
target_link_libraries(grbl Qt5::Core KF5::AtCore)

set(TeacupPlugin_SRCS teacupplugin.cpp)
add_library(teacup SHARED ${TeacupPlugin_SRCS})
target_link_libraries(teacup Qt5::Core KF5::AtCore)

set(MarlinPlugin_SRCS marlinplugin.cpp)
add_library(marlin SHARED ${MarlinPlugin_SRCS})
target_link_libraries(marlin Qt5::Core KF5::AtCore)

set(SprinterPlugin_SRCS sprinterplugin.cpp)
add_library(sprinter SHARED ${SprinterPlugin_SRCS})
target_link_libraries(sprinter Qt5::Core KF5::AtCore)

set(AprinterPlugin_SRCS aprinterplugin.cpp)
add_library(aprinter SHARED ${AprinterPlugin_SRCS})
target_link_libraries(aprinter Qt5::Core KF5::AtCore)

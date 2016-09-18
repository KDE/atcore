include_directories(..)

set(RepetierPlugin_SRCS repetierplugin.cpp)
add_library(repetier SHARED ${RepetierPlugin_SRCS})
target_link_libraries(repetier Qt5::Core AtCoreLib)
add_dependencies(repetier AtCoreLib)

set(GrblPlugin_SRCS grblplugin.cpp)
add_library(grbl SHARED ${GrblPlugin_SRCS})
target_link_libraries(grbl Qt5::Core AtCoreLib)
add_dependencies(grbl AtCoreLib)

include_directories(..)

set(RepetierPlugin_SRCS repetierplugin.cpp)
add_library(repetier ${RepetierPlugin_SRCS})
target_link_libraries(repetier Qt5::Core)
add_dependencies(repetier AtCoreLib)

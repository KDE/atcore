project(AtCore)

cmake_minimum_required(VERSION 3.5)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDECompilerSettings)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)

set(CMAKE_AUTOMOC ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Test
    Quick
    Widgets
    SerialPort
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(testclient)

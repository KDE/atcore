project(AtCore)

# 3.0 is needed to support CMAKE_AUTOUIC
cmake_minimum_required(VERSION 3.0)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDECompilerSettings)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(FeatureSummary)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_HOME_DIRECTORY}/doc/Doxyfile"
        WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
        COMMENT "Building user's documentation into doxyDoc build dir..."
    )
endif()

add_definitions(-DQT_NO_CAST_FROM_ASCII
                -DQT_NO_URL_CAST_FROM_STRING)

set(CMAKE_AUTOMOC ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

find_package(KF5 REQUIRED COMPONENTS
    Solid
)

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

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)


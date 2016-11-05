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
include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMPackageConfigHelpers)
include(ECMGenerateHeaders)
find_package(Doxygen)

set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "AtCore" URL "https://projects.kde.org/projects/atcore")

set(PROJECT_VERSION "16.08.0")
set(KF5_DEP_VERSION "5.24.0") # handled by release scripts
set(REQUIRED_QT_VERSION 5.4.0)

add_definitions(-DQT_NO_CAST_FROM_ASCII
                -DQT_NO_URL_CAST_FROM_STRING)

set(CMAKE_AUTOMOC ON)

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

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(testclient)
add_subdirectory(doc)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)


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
include(CMakePackageConfigHelpers)
include(ECMGenerateHeaders)
find_package(Doxygen)

set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "AtCore" URL "https://projects.kde.org/projects/atcore")

set(PROJECT_VERSION "16.08.0")
set(KF5_DEP_VERSION "5.24.0") # handled by release scripts
set(REQUIRED_QT_VERSION 5.4.0)

add_definitions(-DQT_NO_CAST_FROM_ASCII
                -DQT_NO_URL_CAST_FROM_STRING)

set(CMAKE_AUTOMOC ON)
option(BUILD_TEST_GUI "Build the Test Gui" ON)

if(BUILD_TEST_GUI)
    add_subdirectory(testclient)
endif()

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Test
    SerialPort
)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX ATCORE
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/atcore_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5AtCoreConfigVersion.cmake"
    SOVERSION 1
)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(doc)

set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5AtCore")
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KF5AtCoreConfig.cmake.in"
                              "${CMAKE_CURRENT_BINARY_DIR}/KF5AtCoreConfig.cmake"
                              INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5AtCoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5AtCoreConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT
     KF5AtCoreTargets
     DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
     FILE KF5AtCoreTargets.cmake
     NAMESPACE KF5::
     COMPONENT Devel
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/atcore_version.h"
    DESTINATION "${KF5_INCLUDE_INSTALL_DIR}"
    COMPONENT Devel
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)


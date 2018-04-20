project(AtCore)

# 3.0 is needed to support CMAKE_AUTOUIC
cmake_minimum_required(VERSION 3.0)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(FeatureSummary)
include(GenerateExportHeader)
include(ECMSetupVersion)
include(CMakePackageConfigHelpers)
include(ECMGenerateHeaders)

option(BUILD_GUI "Build the Test Gui")
option(BUILD_DOCS "Build and Install Documents (Requires Doxygen)") 
option(BUILD_TESTS "Build and Run Unittests")

set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra modules and scripts for CMake" URL "git://anongit.kde.org/extra-cmake-modules")

set(PROJECT_VERSION "1.0.70")
set(KF5_DEP_VERSION "5.24.0") # handled by release scripts
set(REQUIRED_QT_VERSION 5.4.0)

IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
  FIND_PACKAGE(Git)
  IF(GIT_FOUND)
    EXECUTE_PROCESS(
      COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      OUTPUT_VARIABLE "GIT_VERSION"
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    MESSAGE( STATUS "Git Commit: ${GIT_VERSION}" )
    add_definitions( -DGIT_REVISION="${GIT_VERSION}")
  ENDIF(GIT_FOUND)
ENDIF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)

add_definitions(-DQT_NO_CAST_FROM_ASCII
                -DQT_NO_URL_CAST_FROM_STRING)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    SerialPort
)
include(ECMPoQmTools)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX ATCORE
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/atcore_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/AtCoreConfigVersion.cmake"
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
endif()

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

# Set default build type to be release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Turn warnings on by default
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# Set debug mode to use -g
set(CMAKE_CXX_FLAGS_DEBUG "-g")
# Use optimization
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_AUTOMOC ON)

add_subdirectory(src)

#optional Parts.
if(BUILD_GUI)
    add_subdirectory(testclient)
endif()

if (BUILD_TESTS)
    add_subdirectory(unittests)
endif()

if (BUILD_DOCS)
    add_subdirectory(doc)
endif()

set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/AtCore")
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/AtCoreConfig.cmake.in"
                              "${CMAKE_CURRENT_BINARY_DIR}/AtCoreConfig.cmake"
                              INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/AtCoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/AtCoreConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT
     AtCoreTargets
     DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
     FILE AtCoreTargets.cmake
     NAMESPACE AtCore::
     COMPONENT Devel
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/atcore_version.h"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/AtCore/"
    COMPONENT Devel
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

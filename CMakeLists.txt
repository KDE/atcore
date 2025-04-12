# SPDX-FileCopyrightText: AtCore Authors
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.16)

project(AtCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM 6.10 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(CMakePackageConfigHelpers)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(FeatureSummary)
include(GenerateExportHeader)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEInstallDirs)
include(KDECMakeSettings)

option(BUILD_GUI "Build the Test Gui")
option(BUILD_DOCS "Build and Install Documents (Requires Doxygen)")
option(BUILD_TESTS "Build and Run Unittests")
option(DEPLOY_PLUGINS_WITH_BINARY "Deploy Plugins to bin/plugins (Use for win/mac)")

include(KDEClangFormat)
file(GLOB_RECURSE
    ALL_CLANG_FORMAT_SOURCE_FILES
    *.cpp
    *.h
)

kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

set_package_properties(
    ECM PROPERTIES
    TYPE REQUIRED
    DESCRIPTION "Extra modules and scripts for CMake"
    URL "git://anongit.kde.org/extra-cmake-modules")

set(PROJECT_VERSION "1.0.72")
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/AtCore")

# Generate version check file.
ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX ATCORE
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/atcore_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/AtCoreConfigVersion.cmake"
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Get the version from git if it's a git repository
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

set(QT_MIN_VERSION 6.7)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    SerialPort
    Charts
    Widgets
)

include(ECMPoQmTools)

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/poqm")
    ecm_install_po_files_as_qm(poqm)
endif()

# Set default build type to be release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX OR CMAKE_COMPILER_IS_CLANG)
    # Turn warnings on by default
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    # Set debug mode to use -g
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    # Use optimization
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

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

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/AtCoreConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/atcore_version.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/AtCore/"
    COMPONENT Devel
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

# SPDX-FileCopyrightText: AtCore Authors
# SPDX-License-Identifier: BSD-3-Clause
set(widgetsLIB_SRCS
    about.cpp
    axiscontrol.cpp
    commandwidget.cpp
    logwidget.cpp
    movementwidget.cpp
    plotwidget.cpp
    printwidget.cpp
    sdwidget.cpp
    statuswidget.cpp
    temperaturewidget.cpp
    profilemanager.cpp
)

add_library(AtCoreWidgets ${widgetsLIB_SRCS})
target_link_libraries(AtCoreWidgets AtCore::AtCore Qt::Core Qt::Widgets Qt::Charts)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/AtCore")
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/AtCoreWidgetsConfig.cmake.in"
                              "${CMAKE_CURRENT_BINARY_DIR}/AtCoreWidgetsConfig.cmake"
                              INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/AtCoreWidgetsConfig.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

generate_export_header(AtCoreWidgets BASE_NAME atcorewidgets)

add_library(AtCore::AtCoreWidgets ALIAS AtCoreWidgets)

target_include_directories(AtCoreWidgets INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/AtCore>")

set_target_properties(AtCoreWidgets PROPERTIES VERSION ${ATCORE_VERSION_STRING}
    SOVERSION ${ATCORE_SOVERSION}
    EXPORT_NAME AtCoreWidgets
)

ecm_generate_headers(ATCOREWIDGETS_CamelCase_HEADERS
    HEADER_NAMES
    About
    AxisControl
    CommandWidget
    LogWidget
    MovementWidget
    PlotWidget
    PrintWidget
    SdWidget
    StatusWidget
    TemperatureWidget
    ProfileManager
    REQUIRED_HEADERS ATCOREWIDGETS_HEADERS
)

ecm_create_qm_loader(widgetsLib_SRCS atcore_qt)

install(FILES
    ${ATCOREWIDGETS_CamelCase_HEADERS}
    ${ATCOREWIDGETS_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/AtCore
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/atcorewidgets_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/AtCore COMPONENT Devel
)

install(
    TARGETS
        AtCoreWidgets
    EXPORT
        AtCoreTargets
    ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)


ecm_generate_pri_file(
    BASE_NAME AtCoreWidgets
    LIB_NAME AtCoreWidgets
    DEPS "Qt5Core Qt5Widgets Qt5Charts"
    FILENAME_VAR PRI_FILENAME
    INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/AtCore
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

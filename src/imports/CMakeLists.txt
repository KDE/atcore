# SPDX-FileCopyrightText: AtCore Authors
# SPDX-License-Identifier: BSD-3-Clause

project(atcore-imports)
include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(atcoreextensionplugin SHARED atcoreextensionplugin.cpp)

target_link_libraries (
    atcoreextensionplugin
    AtCore
    Qt::Core
    Qt::Qml
)

install(TARGETS atcoreextensionplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/atcore)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/atcore)

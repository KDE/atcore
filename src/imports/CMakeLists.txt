project(atcore-imports)
include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set (
    atcore_qml_bridge_LIB_SRCS
    atcoreextensionplugin.cpp
)

find_package(Qt5 REQUIRED COMPONENTS
    Qml
)

add_library(atcoreextensionplugin SHARED ${atcore_qml_bridge_LIB_SRCS})

target_link_libraries (
    atcoreextensionplugin
    AtCore
    Qt5::Core
    Qt5::Qml
)

install(TARGETS atcoreextensionplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/atcore)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/atcore)

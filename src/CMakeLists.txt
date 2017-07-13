add_subdirectory(plugins)

configure_file(
    atcore_default_folders.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/atcore_default_folders.h
)

set(AtCoreLib_SRCS
    atcore.cpp
    seriallayer.cpp
    gcodecommands.cpp
    ifirmware.cpp
    temperature.cpp
    printthread.cpp
)

add_library(KF5AtCore SHARED ${AtCoreLib_SRCS})
target_link_libraries(KF5AtCore Qt5::Core Qt5::SerialPort)

generate_export_header(KF5AtCore BASE_NAME AtCore)
add_library(KF5::AtCore ALIAS KF5AtCore)

target_include_directories(KF5AtCore PUBLIC "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/AtCore>")

set_target_properties(KF5AtCore PROPERTIES VERSION ${ATCORE_VERSION_STRING}
    SOVERSION ${ATCORE_SOVERSION}
    EXPORT_NAME AtCore
)

ecm_generate_headers(ATCORE_HEADERS
    HEADER_NAMES
    AtCore
    GCodeCommands
    IFirmware
    SerialLayer
    Temperature
    REQUIRED_HEADERS ATCORE_HEADERS
)
ecm_create_qm_loader(AtCoreLib_SRCS atcore_qt)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/atcore_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/atcore_default_folders.h
    ${ATCORE_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/AtCore COMPONENT Devel
)

install(TARGETS KF5AtCore EXPORT KF5AtCoreTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME AtCore LIB_NAME AtCore DEPS "Qt5Core Qt5SerialPort" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/AtCore)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

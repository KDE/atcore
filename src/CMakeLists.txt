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
)

add_library(KF5AtCore SHARED ${AtCoreLib_SRCS})
target_link_libraries(KF5AtCore Qt5::Core Qt5::SerialPort)

generate_export_header(KF5AtCore BASE_NAME KAtCore)
add_library(KF5::AtCore ALIAS KF5AtCore)

target_include_directories(KF5AtCore PUBLIC "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KAtCore>")

set_target_properties(KF5AtCore PROPERTIES VERSION ${ATCORE_VERSION_STRING}
    SOVERSION ${ATCORE_SOVERSION}
    EXPORT_NAME AtCore
)

ecm_generate_headers(KATCORE_HEADERS
    HEADER_NAMES
    AtCore
    GCodeCommands
    IFirmware
    SerialLayer
    Temperature
    REQUIRED_HEADERS KATCORE_HEADERS
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/katcore_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/atcore_default_folders.h
    ${KATCORE_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KAtCore COMPONENT Devel
)

install(TARGETS KF5AtCore EXPORT KF5AtCoreTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KAtCore LIB_NAME KAtCore DEPS "Qt5Core Qt5SerialPort" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KAtCore)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

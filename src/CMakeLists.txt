add_subdirectory(plugins)
add_subdirectory(widgets)

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

add_library(AtCore SHARED ${AtCoreLib_SRCS})
target_link_libraries(AtCore Qt5::Core Qt5::SerialPort)

generate_export_header(AtCore BASE_NAME atcore)
add_library(AtCore::AtCore ALIAS AtCore)

target_include_directories(AtCore INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/AtCore>")

set_target_properties(AtCore PROPERTIES VERSION ${ATCORE_VERSION_STRING}
    SOVERSION ${ATCORE_SOVERSION}
    EXPORT_NAME AtCore
)

ecm_generate_headers(ATCORE_CamelCase_HEADERS
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
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/AtCore COMPONENT Devel
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/atcore_default_folders.h
    ${ATCORE_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/AtCore COMPONENT Devel
)

install(FILES
    ${ATCORE_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/AtCore
)

install(TARGETS AtCore EXPORT AtCoreTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME AtCore LIB_NAME AtCore DEPS "Qt5Core Qt5SerialPort" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/AtCore)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

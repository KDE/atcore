add_subdirectory(plugins)

set(AtCoreLib_SRCS
    atcore.cpp
    seriallayer.cpp
    gcodecommands.cpp
    ifirmware.h
)

add_library(KF5AtCore SHARED ${AtCoreLib_SRCS})
target_link_libraries(KF5AtCore Qt5::Core Qt5::SerialPort)

generate_export_header(KF5AtCore BASE_NAME KAtCore)
add_library(KF5::AtCore ALIAS KF5AtCore)

target_include_directories(KF5AtCore PUBLIC "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/atcore>")

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
    REQUIRED_HEADERS KATCORE_HEADERS
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/katcore_export.h
    ${KATCORE_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KAtCore COMPONENT Devel
)

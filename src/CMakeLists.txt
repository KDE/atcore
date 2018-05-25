add_subdirectory(core)
add_subdirectory(plugins)
add_subdirectory(widgets)

install(
    EXPORT
        AtCoreTargets
    DESTINATION
        "${CMAKECONFIG_INSTALL_DIR}"
    FILE
        AtCoreTargets.cmake
    NAMESPACE
        AtCore::
    COMPONENT
        Devel
)

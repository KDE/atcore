# SPDX-FileCopyrightText: AtCore Authors
# SPDX-License-Identifier: BSD-3-Clause

include_directories(../src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (NOT APPLE)
    add_executable(atcore-gui ${AtCoreTestClient_SRCS} icons.qrc)
    install(TARGETS atcore-gui RUNTIME DESTINATION ${KDE_INSTALL_BINDIR})
    install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/atcore-gui.png"
        DESTINATION share/icons/hicolor/64x64/apps/
        RENAME org.kde.atcore.png
    )
    install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/org.kde.atcore.desktop"
        DESTINATION ${KDE_INSTALL_APPDIR}
    )
    install(FILES org.kde.atcore.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
    set(MACOSX_BUNDLE_DISPLAY_NAME "atcore-gui")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.AtCore")
    set(MACOSX_BUNDLE_BUNDLE_NAME "atcore-gui")
    set(MACOSX_BUNDLE_DISPLAY_NAME "atcore-gui")
    set(MACOSX_BUNDLE_ICON_FILE atcore-gui.icns)
    set(MACOSX_BUNDLE_INFO_STRING "AtCore - Test Client")
    set(MACOSX_BUNDLE_COPYRIGHT "2016-2017 The AtCore Authors")
    set(APPICON ${CMAKE_CURRENT_SOURCE_DIR}/atcore-gui.icns)
    set_source_files_properties(${APPICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(atcore-gui MACOSX_BUNDLE ${AtCoreTestClient_SRCS} icons.qrc ${APPICON})
    install(TARGETS atcore-gui BUNDLE DESTINATION bin)
endif()

target_sources(atcore-gui PRIVATE
    main.cpp
    mainwindow.cpp
)

ecm_create_qm_loader(atcore-gui atcore_qt)

target_link_libraries(atcore-gui AtCore::AtCoreWidgets AtCore::AtCore Qt::Widgets Qt::Charts)

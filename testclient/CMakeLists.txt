set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(../src)
add_subdirectory(widgets)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(AtCoreTestClient_SRCS
    main.cpp
    mainwindow.cpp
)

ecm_create_qm_loader(AtCoreTestClient_SRCS atcore_qt)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Test
    Widgets
    SerialPort
)
if (NOT APPLE)
    add_executable(AtCoreTest ${AtCoreTestClient_SRCS} icons.qrc)

	install(TARGETS AtCoreTest RUNTIME DESTINATION bin)

	install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/AtCoreTest.png"
        DESTINATION share/pixmaps)

	install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/AtCoreTest.desktop"
        DESTINATION share/applications)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
    set(MACOSX_BUNDLE_DISPLAY_NAME "AtCore")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.AtCore")
    set(MACOSX_BUNDLE_BUNDLE_NAME "AtCoreTest")
    set(MACOSX_BUNDLE_DISPLAY_NAME "AtCoreTest")
    set(MACOSX_BUNDLE_ICON_FILE AtCoreTest.icns)
    set(MACOSX_BUNDLE_INFO_STRING "AtCore - Test Client")
    set(MACOSX_BUNDLE_COPYRIGHT "2016-2017 The AtCore Authors")
    set(APPICON ${CMAKE_CURRENT_SOURCE_DIR}/AtCoreTest.icns)
    set_source_files_properties(${APPICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(AtCoreTest MACOSX_BUNDLE ${AtCoreTestClient_SRCS} icons.qrc ${APPICON})
    install(TARGETS AtCoreTest BUNDLE DESTINATION bin)
endif()

target_link_libraries(AtCoreTest AtCoreTestWidgets AtCore::AtCore Qt5::Widgets Qt5::Charts)




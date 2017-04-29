set(CMAKE_AUTOUIC ON)

include_directories(../src)
add_subdirectory(widgets)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(AtCoreTestClient_SRCS
    main.cpp
    mainwindow.cpp
)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Test
    Widgets
    SerialPort
)

find_package(KF5 REQUIRED COMPONENTS
    Solid
    I18n
    XmlGui
    ConfigWidgets
)

add_executable(AtCoreTest ${AtCoreTestClient_SRCS})

target_link_libraries(AtCoreTest AtCoreTestWidgets KF5::AtCore Qt5::Widgets KF5::Solid Qt5::Charts KF5::XmlGui KF5::ConfigWidgets KF5::I18n)

install(TARGETS AtCoreTest RUNTIME DESTINATION bin)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/AtCoreTest.menu"
    RENAME AtCoreTest
    DESTINATION share/menu)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/AtCoreTest.png"
    DESTINATION share/pixmaps)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/AtCoreTest.desktop"
    DESTINATION share/applications)

install(FILES atcoreui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/atcore)

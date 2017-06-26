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
if (NOT APPLE)
	add_executable(AtCoreTest ${AtCoreTestClient_SRCS})

	install(TARGETS AtCoreTest RUNTIME DESTINATION bin)

	install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/AtCoreTest.png"
        DESTINATION share/pixmaps)

	install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/AtCoreTest.desktop"
        DESTINATION share/applications)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
    add_executable(AtCoreTest MACOSX_BUNDLE ${AtCoreTestClient_SRCS})
    install(TARGETS AtCoreTest BUNDLE DESTINATION bin)
endif()

target_link_libraries(AtCoreTest AtCoreTestWidgets KF5::AtCore Qt5::Widgets Qt5::Charts)




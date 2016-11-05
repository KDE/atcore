set(CMAKE_AUTOUIC ON)

include_directories(../src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(AtCoreTestClient_SRCS
    main.cpp
    mainwindow.cpp
)

add_executable(AtCoreTest ${AtCoreTestClient_SRCS})

target_link_libraries(AtCoreTest KF5::AtCore Qt5::Widgets KF5::Solid)

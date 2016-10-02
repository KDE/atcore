set(CMAKE_AUTOUIC ON)

find_package(KF5Solid REQUIRED NO_MODULE)

MESSAGE("Include dirs: "  ${Solid_INCLUDE_DIRS} ${Solid_VERSION_MAJOR} )

include_directories(../src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(AtCoreTestClient_SRCS
    main.cpp
    mainwindow.cpp
)

add_executable(AtCoreTest ${AtCoreTestClient_SRCS})
add_dependencies(AtCoreTest AtCoreLib)

target_link_libraries(AtCoreTest AtCoreLib Qt5::Widgets KF5::Solid)

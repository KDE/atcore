include_directories(../src)

set(AtCoreTestClient_SRCS
    main.cpp
    mainwindow.cpp
)

add_executable(AtCoreTest ${AtCoreTestClient_SRCS})
add_dependencies(AtCoreTest AtCoreLib)

target_link_libraries(AtCoreTest AtCoreLib Qt5::Widgets)

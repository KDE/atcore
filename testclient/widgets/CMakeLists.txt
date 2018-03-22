set(widgets_SRCS
    about.cpp
    axiscontrol.cpp
    commandwidget.cpp
    logwidget.cpp
    plotwidget.cpp
    printwidget.cpp
    statuswidget.cpp
    )

add_library(AtCoreTestWidgets STATIC ${widgets_SRCS})
if (APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
endif()
target_link_libraries(AtCoreTestWidgets Qt5::Core Qt5::Widgets Qt5::Charts)

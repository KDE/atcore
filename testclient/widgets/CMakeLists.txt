set(widgets_SRCS
    plotwidget.cpp
    axiscontrol.cpp
    about.cpp
    )

add_library(AtCoreTestWidgets STATIC ${widgets_SRCS})
if (APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
endif()
target_link_libraries(AtCoreTestWidgets Qt5::Core Qt5::Widgets Qt5::Charts)

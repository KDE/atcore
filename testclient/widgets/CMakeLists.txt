set(widgets_SRCS
    plotwidget.cpp
    )

add_library(AtCoreTestWidgets STATIC ${widgets_SRCS})

target_link_libraries(AtCoreTestWidgets Qt5::Core Qt5::Widgets Qt5::Charts)

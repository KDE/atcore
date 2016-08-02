project (kps)

cmake_minimum_required( VERSION 3.5)

set (CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

find_package(Qt5 REQUIRED COMPONENTS
                Core
                Test
                Quick
                Widgets
                SerialPort
)

set(kps_SRCS
    src/main.cpp
    src/mainwindow.cpp
    src/seriallayer.cpp
)

add_executable(kps ${kps_SRCS})
target_link_libraries(kps Qt5::Core Qt5::Quick Qt5::Widgets Qt5::SerialPort)


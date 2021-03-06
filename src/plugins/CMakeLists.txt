# SPDX-FileCopyrightText: AtCore Authors
# SPDX-License-Identifier: BSD-3-Clause

set(RepetierPlugin_SRCS repetierplugin.cpp)
add_library(repetier SHARED ${RepetierPlugin_SRCS})
target_link_libraries(repetier Qt::Core AtCore::AtCore)

set(GrblPlugin_SRCS grblplugin.cpp)
add_library(grbl SHARED ${GrblPlugin_SRCS})
target_link_libraries(grbl Qt::Core AtCore::AtCore)

set(TeacupPlugin_SRCS teacupplugin.cpp)
add_library(teacup SHARED ${TeacupPlugin_SRCS})
target_link_libraries(teacup Qt::Core AtCore::AtCore)

set(MarlinPlugin_SRCS marlinplugin.cpp)
add_library(marlin SHARED ${MarlinPlugin_SRCS})
target_link_libraries(marlin Qt::Core AtCore::AtCore)

set(SprinterPlugin_SRCS sprinterplugin.cpp)
add_library(sprinter SHARED ${SprinterPlugin_SRCS})
target_link_libraries(sprinter Qt::Core AtCore::AtCore)

set(AprinterPlugin_SRCS aprinterplugin.cpp)
add_library(aprinter SHARED ${AprinterPlugin_SRCS})
target_link_libraries(aprinter Qt::Core AtCore::AtCore)


set(SmoothiePlugin_SRCS smoothieplugin.cpp)
add_library(smoothie SHARED ${SmoothiePlugin_SRCS})
target_link_libraries(smoothie Qt::Core AtCore::AtCore)

if(DEPLOY_PLUGINS_WITH_BINARY)
    install(
    TARGETS
        repetier
        grbl
        teacup
        marlin
        sprinter
        aprinter
        smoothie
    DESTINATION
        bin/plugins
    )

else (NOT DEPLOY_PLUGINS_WITH_BINARY)
    install(
    TARGETS
        repetier
        grbl
        teacup
        marlin
        sprinter
        aprinter
        smoothie
    DESTINATION
        ${KDE_INSTALL_PLUGINDIR}/AtCore
    )
endif()

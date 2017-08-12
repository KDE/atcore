# Helper macro TEST used to created rules to build, link, install and run tests
macro(TEST NAME FILE)
    add_executable(${NAME} ${FILE} )
    target_link_libraries(
        ${NAME}
        AtCore
        Qt5::Core Qt5::Test
        )

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        # Run test using wine
        if(WINE_PROGRAM)
            add_test(
                NAME ${NAME}
                COMMAND "$<TARGET_FILE:${NAME}>"
                WORKING_DIRECTORY ${WINDOWS_STAGING_TESTS}
                )
            # Set WINEPATH (%PATH%) to WINDOWS_STAGING_TESTS allowing wine to find dlls
            # WINEDEBUG=-all is used to avoid anoying winde debug outputs
            set_tests_properties(${NAME} PROPERTIES ENVIRONMENT "WINEPATH=${WINDOWS_STAGING_TESTS};WINEDEBUG=-all")
            set_tests_properties(${NAME} PROPERTIES DEPENDS PrepareTests)
        endif()
    else()
        add_test(NAME ${NAME} COMMAND  $<TARGET_FILE:${NAME}>)
    endif()
endmacro()

TEST(AtCoreTests atcoretests.cpp)
TEST(GcodeTests gcodetests.cpp)
TEST(TemperatureTests temperaturetests.cpp)

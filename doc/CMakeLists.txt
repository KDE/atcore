if(DOXYGEN_FOUND)
    add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_HOME_DIRECTORY}/doc/Doxyfile"
        WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
        COMMENT "Building user's documentation into doxyDoc build dir..."
    )
endif()

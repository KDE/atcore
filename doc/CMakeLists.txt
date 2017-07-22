add_custom_target(doc ALL COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building user's documentation into doxyDoc build dir..."
)

install (DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/doc/html" DESTINATION share/doc/atcore )


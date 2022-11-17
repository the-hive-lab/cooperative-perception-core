set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR})
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

configure_file(Doxyfile.in Doxyfile)

add_custom_target(cooperative_perception_coreDocumentation
  COMMAND Doxygen::doxygen ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  COMMAND sphinx-build
    -b html
    -Dbreathe_projects.cooperative_perception_core=${CMAKE_CURRENT_BINARY_DIR}/xml
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/sphinx
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

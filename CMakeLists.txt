project(MudClient CXX C)
cmake_minimum_required(VERSION 2.6)
add_definitions("-Wall -O2 -g")

find_package(Qt4 REQUIRED)
set(CMAKE_INSTALL_PREFIX ".")
add_subdirectory(src)

find_package(Doxygen)

if (DOXYGEN_FOUND)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE}
                    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
  set_property(DIRECTORY APPEND PROPERTY
               ADDITIONAL_MAKE_CLEAN_FILES doc)
endif (DOXYGEN_FOUND)

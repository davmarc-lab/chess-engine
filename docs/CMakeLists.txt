# TODO 
cmake_minimum_required(VERSION 3.12)
project(livedoc LANGUAGES NONE) # No C++ compiler needed just for docs

# 1. Find Doxygen on the system
find_package(Doxygen REQUIRED)

# 2. Define paths to your Doxyfile and where it lives
set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/live_doxy")

# 3. Create a custom target named 'doc' that runs Doxygen
add_custom_target(livedoc
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/build/docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_IN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation using Doxyfile..."
    VERBATIM
)

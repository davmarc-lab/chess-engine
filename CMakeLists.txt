cmake_minimum_required(VERSION 3.23)

project(chess-engine)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Redirect all executables to build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Redirect all static/import libraries to build/lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Redirect all shared libraries (DLLs/SOs) to build/lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Subdirectories
add_subdirectory(Engine)
add_subdirectory(App)
add_subdirectory(Test)

# ===== DOCS =====
find_package(Doxygen)

if (DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/docs/api
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/docs/api

        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile

        COMMAND doxybook2
            --input  ${CMAKE_CURRENT_BINARY_DIR}/docs/xml
            --output ${CMAKE_CURRENT_SOURCE_DIR}/docs/api
            --config ${CMAKE_CURRENT_SOURCE_DIR}/doxybook_config.json

        COMMAND mkdocs build --config-file ${CMAKE_CURRENT_SOURCE_DIR}/mkdocs.yml

        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating documentation pipeline..."
        VERBATIM
    )

    message(STATUS "Documentation target 'docs' is available.")
else()
    message(WARNING "Doxygen not found. Documentation target will not be created.")
endif()

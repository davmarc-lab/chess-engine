cmake_minimum_required(VERSION 3.14)

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
add_subdirectory(Graphics)

# ===== DOCS =====
find_package(Doxygen)

if (DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/docs_md/api
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/docs_md/api

        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile

        COMMAND doxybook2
            --input  ${CMAKE_CURRENT_BINARY_DIR}/docs/xml
            --output ${CMAKE_CURRENT_SOURCE_DIR}/docs_md/api
            --config ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxybook_config_local.json

        COMMAND mkdocs build --config-file ${CMAKE_CURRENT_SOURCE_DIR}/docs/mkdocs.yml

        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating documentation pipeline..."
        VERBATIM
    )

    add_custom_target(docs_gh
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/docs_md/api
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/docs_md/api

        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile

        COMMAND doxybook2
            --input  ${CMAKE_CURRENT_BINARY_DIR}/docs/xml
            --output ${CMAKE_CURRENT_SOURCE_DIR}/docs_md/api
            --config ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxybook_config_gh.json

        COMMAND mkdocs build --config-file ${CMAKE_CURRENT_SOURCE_DIR}/docs/mkdocs.yml

        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating documentation pipeline..."
        VERBATIM
    )
else()
    message(WARNING "Doxygen not found. Documentation target will not be created.")
endif()

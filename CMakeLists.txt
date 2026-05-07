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

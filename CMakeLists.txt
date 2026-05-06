cmake_minimum_required(VERSION 3.23)

project(chess-engine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(Engine)
add_subdirectory(App)
add_subdirectory(Test)

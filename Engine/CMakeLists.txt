# Collect all source files recursively
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")

# Define the library
add_library(Engine SHARED ${ENGINE_SOURCES})

# Public headers are in 'include', private ones in 'src'
target_include_directories(Engine 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

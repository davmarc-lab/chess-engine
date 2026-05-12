project(common LANGUAGES CXX)

file(GLOB_RECURSE COMMON_SOURCES "src/*.cpp")

add_library(common STATIC ${COMMON_SOURCES})

# Public headers are in 'include', private ones in 'src'
target_include_directories(common 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


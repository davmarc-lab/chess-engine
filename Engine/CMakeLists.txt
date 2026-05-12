project(engine LANGUAGES CXX)

file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")

add_library(engine STATIC ${ENGINE_SOURCES})

target_include_directories(engine PRIVATE common)

target_link_libraries(engine PRIVATE common)

# Public headers are in 'include', private ones in 'src'
target_include_directories(engine 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

file(GLOB_RECURSE APP_SOURCES "src/*.cpp")

add_executable(App ${APP_SOURCES})

# Link the Engine library
target_link_libraries(App PRIVATE Engine)

# Include directories for the App's own headers
target_include_directories(App PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


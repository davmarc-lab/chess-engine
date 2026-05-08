project(app LANGUAGES CXX)

file(GLOB_RECURSE APP_SOURCES "src/*.cpp")

add_executable(app ${APP_SOURCES})

target_link_libraries(app PRIVATE engine)
target_link_libraries(app PRIVATE graphics)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

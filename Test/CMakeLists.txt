project(test LANGUAGES CXX)

file(GLOB_RECURSE TEST_SOURCES "src/*.cpp")

add_executable(test ${TEST_SOURCES})

target_link_libraries(test PRIVATE engine)

target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

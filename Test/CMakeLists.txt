file(GLOB_RECURSE TEST_SOURCES "src/*.cpp")

add_executable(Test ${TEST_SOURCES})

# Link against the Engine to test its functionality
target_link_libraries(Test PRIVATE Engine)

target_include_directories(Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Optional: Enable CTest
# enable_testing()
# add_test(NAME BaseEngineTest COMMAND TestRunner)

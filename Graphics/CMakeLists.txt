project(graphics LANGUAGES CXX)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE GRAPHICS_SOURCES "src/*.cpp")

include(FetchContent)

# --- Helper Function for Find-or-Fetch ---
# This looks for the package on the system first. 
# If not found, it falls back to downloading it.
macro(find_or_fetch_dependency name repo tag)
    find_package(${name} QUIET)
    if(NOT ${name}_FOUND)
        message(STATUS "${name} not found on system. Fetching from ${repo}...")
        FetchContent_Declare(
            ${name}
            GIT_REPOSITORY ${repo}
            GIT_TAG ${tag}
        )
        FetchContent_MakeAvailable(${name})
    else()
        message(STATUS "Found ${name} on system.")
    endif()
endmacro()

# --- Check/Fetch GLFW ---

unset(GLFW_LIBRARY_TYPE CACHE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Wayland support
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)

find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "Fetching glfw from source...")
    find_or_fetch_dependency(glfw https://github.com/glfw/glfw.git 3.4) 
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(glfw)

    set(GLFW_LIB_TARGET glfw)
else()
    message(STATUS "Found glfw3 on system.")
    set(GLFW_LIB_TARGET glfw)
endif()

# --- Check/Fetch GLM ---
find_or_fetch_dependency(glm https://github.com/g-truc/glm.git 1.0.3)

# --- Freetype2 ---
find_or_fetch_dependency(freetype https://github.com/freetype/freetype.git VER-2-13-2)

set(FT_DISABLE_ZLIB  ON  CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON  CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG   ON  CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(freetype)

# --- Fetch glad2 ---
find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(VENV_DIR "${CMAKE_BINARY_DIR}/venv")
set(GLAD_GEN_DIR "${CMAKE_BINARY_DIR}/generated/glad")
set(GLAD_SOURCES "${GLAD_GEN_DIR}/src/gl.c")
set(GLAD_INCLUDE_DIR "${GLAD_GEN_DIR}/include")

# Adjust Python path based on OS
if(WIN32)
    set(VENV_PYTHON "${VENV_DIR}/Scripts/python.exe")
else()
    set(VENV_PYTHON "${VENV_DIR}/bin/python")
endif()

# Create Virtual Env and Generate glad2
if(NOT EXISTS ${VENV_PYTHON})
    message(STATUS "Creating Python virtual environment...")
    execute_process(COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_DIR})
endif()

add_custom_command(
    OUTPUT ${GLAD_SOURCES}
    COMMAND ${VENV_PYTHON} -m pip install glad2
    COMMAND ${VENV_PYTHON} -m glad 
        --out-path "${GLAD_GEN_DIR}" 
        --api "gl:core=4.6" 
        --extensions "GL_KHR_debug,GL_KHR_parallel_shader_compile,GL_EXT_debug_label,GL_ARB_direct_state_access,GL_ARB_shading_language_include"
        c 
    COMMENT "Generating GLAD 2 loader (OpenGL 4.6 Core)..."
)

# Create a library for the generated GLAD files
add_library(glad_lib STATIC ${GLAD_SOURCES})
target_include_directories(glad_lib PUBLIC ${GLAD_INCLUDE_DIR})

# --- 4. Executable & Linking ---
add_library(${PROJECT_NAME} STATIC 
    ${GRAPHICS_SOURCES}
    $<TARGET_OBJECTS:glad_lib>
)

# Prevent GLFW from including system OpenGL headers
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${GLFW_LIB_TARGET}
    glm::glm
    freetype
    glad_lib
    ${CMAKE_DL_LIBS}
)

# macOS specific framework linking
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit")
endif()
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


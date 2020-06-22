cmake_minimum_required(VERSION 3.16)
include(ExternalProject)

project(OpenGL_Practice)

set(CMAKE_CXX_STANDARD 14)
add_executable(OpenGL_Practice main.cpp)

# GLEW
find_package(OpenGL REQUIRED)
set(GLEW_DIR dependencies/glew)
ExternalProject_Add(
        GLEW
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/glew"
        URL https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0.zip/download
        SOURCE_SUBDIR "build/cmake"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${OpenGL_Practice_SOURCE_DIR}/${GLEW_DIR}"
)
find_library(GLEW_LIB
        NAMES glew32
        PATHS ${OpenGL_Practice_SOURCE_DIR}/${GLEW_DIR}/lib
        REQUIRED)
include_directories(${PROJECT_NAME} ${GLEW_DIR}/include)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIB})
add_dependencies(${PROJECT_NAME} GLEW)
# GLEW END

# GLFW
set(dir_glfw dependencies/glfw)
ExternalProject_Add(
        GLFW
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/glfw"
        URL https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.zip
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${OpenGL_Practice_SOURCE_DIR}/${dir_glfw}"
)
find_library(GLFW_LIB
        NAMES glfw3
        PATHS ${OpenGL_Practice_SOURCE_DIR}/${dir_glfw}/lib
        REQUIRED)

include_directories(${PROJECT_NAME} ${dir_glfw}/include)
target_link_libraries(${PROJECT_NAME} ${GLFW_LIB})
add_dependencies(${PROJECT_NAME} GLFW)
# GLFW END

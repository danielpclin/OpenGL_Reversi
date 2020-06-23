cmake_minimum_required(VERSION 3.16)
include(ExternalProject)

project(OpenGL_Practice)

set(CMAKE_CXX_STANDARD 14)
if(APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
endif()
add_executable(OpenGL_Practice src/main.cpp)


# GLEW
find_package(OpenGL REQUIRED)
set(GLEW_DIR dependencies/glew)
ExternalProject_Add(
        GLEW
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/glew"
        URL "https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0.zip/download"
        SOURCE_SUBDIR "build/cmake"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/${GLEW_DIR}"
)
add_library(GLEW_LIB STATIC IMPORTED REQUIRED)
if (WIN32)
    add_definitions(-DGLEW_STATIC)
    set_target_properties(GLEW_LIB PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/${GLEW_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glew32${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
    set_target_properties(GLEW_LIB PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/${GLEW_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}GLEW${CMAKE_STATIC_LIBRARY_SUFFIX})
endif ()
include_directories(${PROJECT_NAME} ${GLEW_DIR}/include)
target_link_libraries(${PROJECT_NAME} GLEW_LIB ${OPENGL_LIBRARIES})
add_dependencies(${PROJECT_NAME} GLEW)
# GLEW END

# GLFW
set(GLFW_DIR dependencies/glfw)
ExternalProject_Add(
        GLFW
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/glfw"
        URL "https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.zip"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/${GLFW_DIR}"
)
add_library(GLFW_LIB STATIC IMPORTED REQUIRED)
set_target_properties(GLFW_LIB PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/${GLFW_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX})
include_directories(${PROJECT_NAME} ${GLFW_DIR}/include)
target_link_libraries(${PROJECT_NAME} GLFW_LIB)
add_dependencies(${PROJECT_NAME} GLFW)
# GLFW END

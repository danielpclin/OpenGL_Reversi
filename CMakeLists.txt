cmake_minimum_required(VERSION 3.16)
include(ExternalProject)

project(OpenGL_Reversi)

set(CMAKE_CXX_STANDARD 14)


add_executable(OpenGL_Reversi src/main.cpp src/Agent.h src/Board.cpp src/Board.h src/PlayerAgent.cpp src/PlayerAgent.h src/Reversi.cpp src/Reversi.h src/Reversi_env.cpp src/Reversi_env.h src/ComputerAgent.cpp src/ComputerAgent.h src/Shader.h)

if(APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
endif()

# MINGW THREAD
#https://github.com/meganz/mingw-std-threads/archive/1.0.0.zip
set(MINGW_THREAD_DIR dependencies/mingw_thread)
ExternalProject_Add(
        MINGW_THREAD
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/mingw_thread"
        GIT_REPOSITORY "https://github.com/meganz/mingw-std-threads.git"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/${MINGW_THREAD_DIR}"
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR> ${PROJECT_SOURCE_DIR}/${MINGW_THREAD_DIR}/include
)
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()
include_directories(${PROJECT_NAME} ${MINGW_THREAD_DIR}/include)
add_dependencies(${PROJECT_NAME} MINGW_THREAD)
# END

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

# GLM
set(GLM_DIR dependencies/glm)
ExternalProject_Add(
        GLM
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/glm"
        URL "https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/${GLM_DIR}"
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/glm ${PROJECT_SOURCE_DIR}/${GLM_DIR}/include/glm
)
include_directories(${PROJECT_NAME} ${GLM_DIR}/include)
add_dependencies(${PROJECT_NAME} GLM)
# GLM END

# FREETYPE
set(FREETYPE_DIR dependencies/freetype)
ExternalProject_Add(
        FREETYPE
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/freetype"
        URL "https://download.savannah.gnu.org/releases/freetype/freetype-2.10.2.tar.gz"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/${FREETYPE_DIR}"
)
add_library(FREETYPE_LIB STATIC IMPORTED REQUIRED)
set_target_properties(FREETYPE_LIB PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/${FREETYPE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}freetype${CMAKE_STATIC_LIBRARY_SUFFIX})
include_directories(${PROJECT_NAME} ${FREETYPE_DIR}/include/freetype2)
target_link_libraries(${PROJECT_NAME} FREETYPE_LIB)
add_dependencies(${PROJECT_NAME} FREETYPE)
# FREETYPE END

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

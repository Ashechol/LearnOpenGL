cmake_minimum_required(VERSION 3.26)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include)

set(glad ${PROJECT_SOURCE_DIR}/glad.c)
set(Shader_Path ${PROJECT_SOURCE_DIR}/Shaders/)
set(Resources_Path ${PROJECT_SOURCE_DIR}/Resources/)

configure_file(Include/Config.h.in ../Include/Config.h)

if (${APPLE})
    link_directories(Libs/M1)
elseif (${WIN32})
    link_directories(Libs)
endif ()

#add_subdirectory("Source/1. Getting Started")

add_library(Core Include/Shader.cpp Include/Camera.cpp Include/Model.cpp glad.c)

add_executable(Renderer Source/Renderer.cpp)

target_link_libraries(Renderer Core glfw3)
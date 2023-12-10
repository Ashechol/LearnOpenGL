cmake_minimum_required(VERSION 3.26)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include)

if (${APPLE})
    link_directories(Libs/M1)
elseif (${WIN32})
    link_directories(Libs)
endif ()

add_library(Shader Include/Shader.cpp glad.c)

add_executable(HelloWindow Source/HelloWindow.cpp glad.c)
add_executable(HelloTriangle Source/HelloTriangle.cpp glad.c)
add_executable(HelloShader Source/HelloShader.cpp glad.c)

if (${APPLE})
    target_link_libraries(HelloWindow "-framework Cocoa")
    target_link_libraries(HelloWindow "-framework IOKit")
    target_link_libraries(HelloWindow "-framework OpenGL")

    target_link_libraries(HelloTriangle "-framework Cocoa")
    target_link_libraries(HelloTriangle "-framework IOKit")
    target_link_libraries(HelloTriangle "-framework OpenGL")

    target_link_libraries(Shaders "-framework Cocoa")
    target_link_libraries(Shaders "-framework IOKit")
    target_link_libraries(Shaders "-framework OpenGL")
endif ()

target_link_libraries(HelloWindow glfw3)
target_link_libraries(HelloTriangle glfw3)
target_link_libraries(HelloShader glfw3 Shader)
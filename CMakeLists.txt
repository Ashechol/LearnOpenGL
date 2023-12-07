cmake_minimum_required(VERSION 3.26)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include)

if (${APPLE})
    link_directories(Libs/M1)
elseif (${WIN32})
    link_directories(Libs)
endif ()

add_executable(HelloWindow Source/HelloWindow.cpp glad.c)
add_executable(HelloTriangle Source/HelloTriangle.cpp glad.c)

if (${APPLE})
    target_link_libraries(HelloWindow "-framework Cocoa")
    target_link_libraries(HelloWindow "-framework IOKit")
    target_link_libraries(HelloWindow "-framework OpenGL")
endif ()

target_link_libraries(HelloWindow glfw3)
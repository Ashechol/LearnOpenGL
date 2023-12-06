cmake_minimum_required(VERSION 3.26)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include)

if (${APPLE})
    link_directories(Libs/M1)
elseif (${WIN32})
    link_directories(Libs)
endif ()

add_executable(${PROJECT_NAME} main.cpp)

if (${APPLE})
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()

target_link_libraries(${PROJECT_NAME} glfw3)
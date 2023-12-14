cmake_minimum_required(VERSION 3.26)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

add_executable(HelloWindow HelloWindow.cpp ${glad})
add_executable(HelloTriangle HelloTriangle.cpp ${glad})
add_executable(HelloShader HelloShader.cpp ${glad})
add_executable(HelloTexture HelloTexture.cpp ${glad})
add_executable(Hello3D Hello3D.cpp ${glad})

if (${APPLE})
    target_link_libraries(HelloWindow "-framework Cocoa")
    target_link_libraries(HelloWindow "-framework IOKit")
    target_link_libraries(HelloWindow "-framework OpenGL")

    target_link_libraries(HelloTriangle "-framework Cocoa")
    target_link_libraries(HelloTriangle "-framework IOKit")
    target_link_libraries(HelloTriangle "-framework OpenGL")

    target_link_libraries(HelloShader "-framework Cocoa")
    target_link_libraries(HelloShader "-framework IOKit")
    target_link_libraries(HelloShader "-framework OpenGL")

    target_link_libraries(HelloTexture "-framework Cocoa")
    target_link_libraries(HelloTexture "-framework IOKit")
    target_link_libraries(HelloTexture "-framework OpenGL")

    target_link_libraries(Hello3D "-framework Cocoa")
    target_link_libraries(Hello3D "-framework IOKit")
    target_link_libraries(Hello3D "-framework OpenGL")
endif ()

target_link_libraries(HelloWindow glfw3)
target_link_libraries(HelloTriangle glfw3)
target_link_libraries(HelloShader glfw3 Shader)
target_link_libraries(HelloTexture glfw3 Shader)
target_link_libraries(Hello3D glfw3 Shader)
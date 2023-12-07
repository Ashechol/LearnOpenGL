// Created by ash.gong on 2023/12/7.

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 三角形三个点的坐标
float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
};

int main()
{
    // vertex buffer objects
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    std::cout << VBO << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    return 0;
}


// Created by ash.gong on 2023/12/7.

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// // 三角形三个点的坐标
// float vertices[] = {
//         -0.5f, -0.5f, 0.0f,
//          0.5f, -0.5f, 0.0f,
//          0.0f,  0.5f, 0.0f
// };

float vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};

// // 矩形四个点的坐标
// float vertices[] = {
//         0.5f, 0.5f, 0.0f,   // 右上角
//         0.5f, -0.5f, 0.0f,  // 右下角
//         -0.5f, -0.5f, 0.0f, // 左下角
//         -0.5f, 0.5f, 0.0f   // 左上角
// };

// unsigned int indices[] = {
//         0, 1, 3,    // 第一个三角形
//         1, 2, 3     // 第二个三角形
// };

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "HelloTriangle", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        std::cout << "Failed to create window" << std::endl;
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, &FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        return EXIT_FAILURE;
    }

#ifdef __APPLE__
        glViewport(0, 0, 1600, 1200);   // Retina 显示器下，宽高翻倍
#else
    glViewport(0, 0, 800, 600);
#endif

    // Vertex Array Objects, Vertex Buffer Objects, Element(Index) Buffer Objects
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // // 通常情况没有解绑的必要，因为每次配置新的 VAO 和 VBO 时都会绑定对应的。
    // glBindBuffer(GL_ARRAY_BUFFER, 0);   // 解绑 VBO
    // glBindVertexArray(0);               // 解绑 VAO


    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader newShader("Simple.vert",
                     "Simple.frag");

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 设置清空屏幕颜色缓冲时的颜色，状态设置函数
        glClear(GL_COLOR_BUFFER_BIT);           // 清空屏幕颜色缓冲，状态使用函数

        newShader.Use();
        glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES,  6, GL_UNSIGNED_INT, nullptr);
        newShader.SetFloat("offsetX", sin((float) glfwGetTime() * 1.5f) * 0.5f);
        newShader.SetFloat("offsetY", cos((float) glfwGetTime() * 1.5f) * 0.5f);
        newShader.SetFloat("time", (float) glfwGetTime());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
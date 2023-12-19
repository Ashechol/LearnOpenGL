// Created by ash.gong on 2023/12/13.

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Config.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float vertices[] = {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
        0, 1, 3,    // 第一个三角形
        1, 2, 3     // 第二个三角形
};

unsigned int SetTexture(const char* filename, int format);

glm::mat4 ChangeTransform();

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

    Shader sProgram("Texture.vert", "Texture.frag");

    // Vertex Array Objects, Vertex Buffer Objects, Element(Index) Buffer Objects
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // // 通常情况没有解绑的必要，因为每次配置新的 VAO 和 VBO 时都会绑定对应的。
    // glBindBuffer(GL_ARRAY_BUFFER, 0);   // 解绑 VBO
    // glBindVertexArray(0);               // 解绑 VAO


    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    unsigned int texture0 = SetTexture("container.jpg", GL_RGB);
    unsigned int texture1 = SetTexture("awesomeface.png", GL_RGBA);

    sProgram.Use();
    // glUniform1i(glGetUniformLocation(sProgram.ID, "texture0"), 0);
    // glUniform1i(glGetUniformLocation(sProgram.ID, "texture1"), 1);
    sProgram.SetInt("texture0", 0);
    sProgram.SetInt("texture1", 1);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 设置清空屏幕颜色缓冲时的颜色，状态设置函数
        glClear(GL_COLOR_BUFFER_BIT);           // 清空屏幕颜色缓冲，状态使用函数

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO);

        unsigned int transform = glGetUniformLocation(sProgram.ID, "transform");
        glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(ChangeTransform()));

        sProgram.Use();
        glDrawElements(GL_TRIANGLES,  6, GL_UNSIGNED_INT, 0);

        transform = glGetUniformLocation(sProgram.ID, "transform");
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5, 0.5, 0));
        float s = 0.35f * sin(glfwGetTime()) + 0.65f;
        trans = glm::scale(trans, glm::vec3(s, s, s));
        glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans));
        glDrawElements(GL_TRIANGLES,  6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

unsigned int SetTexture(const char* filename, int format)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    std::string path = RESOURCES_PATH;
    unsigned char *data = stbi_load((path + filename).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

glm::mat4 ChangeTransform()
{
    glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::translate(trans, glm::vec3(0.2, 0.2, 0));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    return trans;
}



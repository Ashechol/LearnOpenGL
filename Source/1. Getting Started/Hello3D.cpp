// Created by ash.gong on 2023/12/14.

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Shader.h"
#include "Config.h"
#include "Model.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window, Camera& cam);
void MouseCallBack(GLFWwindow* window, double posX, double posY);

glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};

inline glm::mat4 Projection(float width, float height)
{
    return glm::perspective(glm::radians(45.f), width / height, 0.1f, 100.f);
}

inline glm::mat4 View(glm::vec3 translate = glm::vec3(0.f, 0.f, 0.f))
{
    return glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -3.f) + translate);
}

inline glm::mat4 Model(glm::vec3 translate, glm::vec3 axis, float angle)
{
    glm::mat4 mat(1.f);
    mat = glm::translate(mat, translate);
    return glm::rotate(mat, glm::radians(angle), axis);
}

Camera camera;

float deltaTime = 0.f;
float lastFrameTime = 0.f;

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, &FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, &MouseCallBack);

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

    Shader sProgram("Box.vert", "Box.frag");

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Cube), Cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // // 通常情况没有解绑的必要，因为每次配置新的 VAO 和 VBO 时都会绑定对应的。
    // glBindBuffer(GL_ARRAY_BUFFER, 0);   // 解绑 VBO
    // glBindVertexArray(0);               // 解绑 VAO

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST); // 开启深度测试

    unsigned int texture0 = Shader::GenerateTexture("container.jpg", GL_RGB);
    unsigned int texture1 = Shader::GenerateTexture("awesomeface.png", GL_RGBA);

    sProgram.Use();
    sProgram.SetInt("texture0", 0);
    sProgram.SetInt("texture1", 1);
    sProgram.SetMatrix4("projection", Projection(800, 600));
    sProgram.SetMatrix4("view", View());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glBindVertexArray(VAO);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        auto currentFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // 清空颜色缓存和深度缓存

        sProgram.Use();

        for (int i = 0; i < 10; ++i)
        {
            float angle = (float) glfwGetTime() * 50.f;
            glm::vec3 axis = glm::vec3(1.f, 0.5f, 0.f);
            sProgram.SetMatrix4("model", Model(cubePositions[i], axis, angle + i * 25));
            sProgram.SetMatrix4("view", camera.View());
            glDrawArrays(GL_TRIANGLES,  0, 36);
        }

        ProcessInput(window, camera);

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

void ProcessInput(GLFWwindow* window, Camera& cam)
{
    glm::vec3 rightward = glm::cross(cam.forward, cam.upward);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.Movement(MovementDirection::Rightward, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.Movement(MovementDirection::Leftward, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.Movement(MovementDirection::Forward, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.Movement(MovementDirection::Backward, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void MouseCallBack(GLFWwindow* window, double posX, double posY)
{
    auto x = static_cast<float>(posX);
    auto y = static_cast<float>(posY);
    camera.Look(x, y);
}
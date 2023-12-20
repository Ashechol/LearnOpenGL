// Created by ash.gong on 2023/12/20.

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCallBack(GLFWwindow* window, double posX, double posY);

float deltaTime = 0.f;
float lastFrameTime = 0.f;

double lastMouseX, lastMouseY;

Camera camera;

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

int main()
{
#pragma region Window Initialization

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    // macos 需要开启向前版本兼容
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
    glfwSetCursorPosCallback(window, &MouseCallBack);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        return EXIT_FAILURE;
    }

#pragma endregion

#ifdef __APPLE__
        glViewport(0, 0, 1600, 1200);   // Retina 显示器下，宽高翻倍
#else
    glViewport(0, 0, 800, 600);
#endif

    Shader shader("Lighting.vert", "Lighting.frag");
    shader.Use();
    shader.SetMatrix4("projection", Projection(800, 600));
    shader.SetMatrix4("view", camera.View());
    shader.SetMatrix4("model", Model(glm::vec3( 0.0f,  0.0f,  0.0f), glm::vec3(1.f, 0.f, 0.f), 0.f));

#pragma region Render Loop

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    unsigned int cubeVAO = Model::LoadModel(sizeof(Cube), Cube);
    unsigned int lightVAO = Model::LoadModel(sizeof(Cube), Cube);

    while (!glfwWindowShouldClose(window))
    {
        auto currentFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        shader.SetMatrix4("view", camera.View());
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
        ProcessInput(window);
    }

#pragma endregion

    glfwTerminate();

    return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    if (!window) return;

#ifdef __APPLE__
    glViewport(0, 0, width * 2, height * 2); // Retina 显示器下，宽高翻倍
#else
    glViewport(0, 0, width, height);
#endif
}

bool mouseRightClicked = false;
void ProcessInput(GLFWwindow* window)
{
    glm::vec3 rightward = glm::cross(camera.forward, camera.upward);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Movement(MovementDirection::Rightward, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Movement(MovementDirection::Leftward, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Movement(MovementDirection::Forward, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Movement(MovementDirection::Backward, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        mouseRightClicked = true;
        glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        mouseRightClicked = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void MouseCallBack(GLFWwindow* window, double posX, double posY)
{
    if (!mouseRightClicked || !window)
        return;

    double deltaX = camera.mouseSensitivity * (posX - lastMouseX);
    double deltaY = -camera.mouseSensitivity * (posY - lastMouseY);
    lastMouseX = posX;
    lastMouseY = posY;

    auto x = static_cast<float>(deltaX);
    auto y = static_cast<float>(deltaY);
    camera.Look(x, y);
}
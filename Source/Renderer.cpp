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

    Shader lightShader("Lighting.vert", "Lighting.frag");
    lightShader.Use();
    lightShader.SetMatrix4("projection", Camera::Projection(800, 600));

    Shader boxShader("Box.vert", "Box.frag");
    boxShader.Use();
    boxShader.SetMatrix4("projection", Camera::Projection(800, 600));
    boxShader.SetVec4("color", glm::vec4(1.f, 0.5f, 0.31f, 1.f));

#pragma region Render Loop

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    unsigned int cubeVBO = Model::Load(sizeof(Cube), Cube);

    Model cube(cubeVBO);
    Model lightCube(cubeVBO);

    lightCube.position = glm::vec3(1.2f, 1.0f, 2.0f);
    lightCube.scale = glm::vec3(0.2f);

    while (!glfwWindowShouldClose(window))
    {
        auto currentFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // cube.position = glm::vec3(1, 3, 3);

        lightCube.position.x = 3 * sin(currentFrameTime);
        lightCube.position.z = 3 * cos(currentFrameTime);

        lightShader.Use();
        lightShader.SetMatrix4("view", camera.View());
        lightShader.SetMatrix4("model", lightCube.GetModelMatrix());
        lightCube.Render();

        boxShader.Use();
        boxShader.SetMatrix4("view", camera.View());
        boxShader.SetVec3("lightColor", glm::vec3(1.f));
        boxShader.SetVec3("lightPos", lightCube.position);
        boxShader.SetVec3("viewPos", camera.position);

        glm::mat4 modelMat = cube.GetModelMatrix();
        boxShader.SetMatrix4("model", modelMat);
        boxShader.SetMatrix3("normalMatrix", Model::GetNormalMatrix(modelMat));

        cube.Render();

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
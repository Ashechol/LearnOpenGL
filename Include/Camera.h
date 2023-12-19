// Created by ash.gong on 2023/12/14.

#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum class MovementDirection
{
    Forward,
    Backward,
    Leftward,
    Rightward,
};

const glm::vec3 WorldUpward(0.f, 1.f, 0.f);

class Camera
{
public:

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 rightward;
    glm::vec3 upward;

    float speed = 5.f;
    float mouseSensitivity = 0.1f;

    float pitch = 0.f;
    float yaw =  -90.0f;
    // float roll;

    Camera();
    glm::mat4 View();

    void UpdateCamera();

    void Movement(MovementDirection direction, float deltaTime);
    void Look(float posX, float posY, bool constraintPitch = true);

private:

    bool _isInitMouse = true;
    float _lastMouseX, _lastMouseY;
};


#endif //LEARNOPENGL_CAMERA_H

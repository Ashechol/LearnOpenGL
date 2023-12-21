// Created by ash.gong on 2023/12/14.

#include "Camera.h"

Camera::Camera() :
        position (glm::vec3(0.f, 0.f, 3.f)),
        forward  (glm::vec3(0.f, 0.f, -1.f)),
        upward   (glm::vec3(0.f, 1.f, 0.f)),
        rightward(glm::cross(forward, WorldUpward))
{}

glm::mat4 Camera::View() const
{
    return glm::lookAt(position, position + forward, upward);
}

void Camera::Movement(MovementDirection direction, float deltaTime)
{
    float deltaSpeed = speed * deltaTime;
    switch (direction)
    {
        case MovementDirection::Forward:
            position += forward * deltaSpeed;
            break;
        case MovementDirection::Backward:
            position -= forward * deltaSpeed;
            break;
        case MovementDirection::Rightward:
            position += rightward * deltaSpeed;
            break;
        case MovementDirection::Leftward:
            position -= rightward * deltaSpeed;
            break;
    }
}

void Camera::Look(float deltaX, float deltaY, bool constraintPitch)
{
    yaw += deltaX;
    pitch += deltaY;

    if (constraintPitch)
    {
        if (pitch > 89.f)
            pitch = 89.f;
        else if (pitch < -89.f)
            pitch = -89.f;
    }

    UpdateCamera();
}

void Camera::UpdateCamera()
{
    glm::vec3 newForward;
    newForward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newForward.y = sin(glm::radians(pitch));
    newForward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    forward = glm::normalize(newForward);
    rightward = glm::normalize(glm::cross(forward, WorldUpward));
    upward = glm::normalize(glm::cross(rightward, forward));
}

glm::mat4 Camera::Projection(float width, float height)
{
    return glm::perspective(glm::radians(45.f), width / height, 0.1f, 100.f);
}



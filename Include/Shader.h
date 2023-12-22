// Created by ash.gong on 2023/12/9.

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexFileName, const char* fragmentFileName);

    void Use() const;

    static unsigned int GenerateTexture(const char* filename, int format);

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetMatrix3(const std::string& name, glm::mat3 matrix) const;
    void SetMatrix4(const std::string& name, glm::mat4 matrix) const;
    void SetVec3(const std::string &name, glm::vec3 color) const;
    void SetVec4(const std::string& name, glm::vec4 color) const;
};

#endif

// Created by ash.gong on 2023/12/20.

#include "Model.h"
#include "glad/glad.h"

Model::Model(int size, const float *vertices) :
        position(glm::vec3(0)),
        eulerRot(glm::vec3(0)),
        scale(glm::vec3(1))
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定 VAO 和 VBO 之后，才能给 Buffer 传值，以及设置 VertexAttribPointer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // 每个顶点大小为 5，前 3 个值表示坐标，后 2 个值表示 uv
    int stride = 6 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr); // (void*)(0)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); // (void*)(0)
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    // 设置好了顶点属性，还需要启用它们
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // glEnableVertexAttribArray(2);
}

Model::Model(unsigned int VBO) :
        VBO(VBO),
        position(glm::vec3(0)),
        eulerRot(glm::vec3(0)),
        scale(glm::vec3(1))
{
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int stride = 6 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr); // (void*)(0)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

Model::~Model()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Model::Render() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,  0, 36);
}

//TODO: 可以用脏标记来防止重复计算矩阵
glm::mat4 Model::GetModelMatrix() const
{
    glm::mat4 model(1);

    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, eulerRot.x, glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, eulerRot.y, glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, eulerRot.z, glm::vec3(0.f, 0.f, 1.f));

    return model;
}

glm::mat3 Model::GetNormalMatrix(glm::mat4 model)
{
    glm::mat3 result(model);
    result = glm::transpose(glm::inverse(result));

    return result;
}

/// 加载模型到显存上
/// \param size 模型数据大小
/// \param vertices 模型顶点数组
/// \return VBO ID
unsigned int Model::Load(int size, const float *vertices)
{
    unsigned int modelVBO;

    glGenBuffers(1, &modelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    return modelVBO;
}

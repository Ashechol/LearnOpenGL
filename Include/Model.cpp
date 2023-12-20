// Created by ash.gong on 2023/12/20.

#include "Model.h"
#include "glad/glad.h"

unsigned int Model::LoadModel(const int size, const float* vertices)
{
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定 VAO 和 VBO 之后，才能给 Buffer 传值，以及设置 VertexAttribPointer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // 每个顶点大小为 5，前 3 个值表示坐标，后 2 个值表示 uv
    int stride = 5 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    // 设置好了顶点属性，还需要启用它们
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    return VAO;
}

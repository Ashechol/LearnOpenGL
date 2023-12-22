#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
// layout (location = 2) in vec3 inTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 Normal;
out vec4 FragPos;
// out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

    Normal = normalMatrix * normal; 
    FragPos = model * vec4(position, 1.0);

    // TexCoord = inTexCoord;
}
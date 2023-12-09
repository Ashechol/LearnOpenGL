#version 330 core
out vec4 FragColor;

in vec4 vertexColor;

uniform float alpha;

void main()
{
    FragColor = vec4(vertexColor.x, vertexColor.y, alpha, 1.0f);
}
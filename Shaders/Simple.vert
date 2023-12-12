#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色

uniform float offsetX, offsetY;
uniform float value;

const float PI = 3.14159265359;

float func(float value, float offset)
{
    return cos(value * PI + PI * offset) * 0.5 + 0.5;
}

void main()
{
    gl_Position = vec4(aPos.x + offsetX, -aPos.y + offsetY, aPos.z, 1.0);

    if (aColor.r > 0)
        ourColor = vec3(func(value, 0), func(value, 1), aColor.b);
    else if (aColor.g > 0)
        ourColor = vec3(aColor.r, func(value, 0), func(value, 1));
    else
        ourColor = vec3(func(value, 1), aColor.g, func(value, 0));
}
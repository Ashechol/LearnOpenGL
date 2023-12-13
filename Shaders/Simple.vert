#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色

uniform float offsetX, offsetY;
uniform float time;

const float PI = 3.14159265359;

float easeInOutQuart(float x)
{
    return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
}

float func(float start, float end, float duration)
{
    float t = time / duration - floor(time / duration);

    // Linear
    // return (1-t) * start + t * end;

    // if (start < end)
    //     return 0.5 * cos(PI * t + PI) + 0.5;
    // else
    //     return 0.5  * cos(PI * t) + 0.5;

    return (1-easeInOutQuart(t)) * start + easeInOutQuart(t) * end;
}

vec3 GetColor(int pos)
{
    int n = int(floor(time / 5)) % 3;

    if (pos == 0)
    {
        if (n == 0)
            return vec3(func(1, 0, 5), func(0, 1, 5), 0);
        else if (n == 1)
            return vec3(0, func(1, 0, 5), func(0, 1, 5));
        else
            return vec3(func(0, 1, 5), 0, func(1, 0 , 5));
    }
    else if (pos == 1)
    {
        if (n == 0)
            return vec3(0, func(1, 0, 5), func(0, 1, 5));
        else if (n == 1)
            return vec3(func(0, 1, 5), 0, func(1, 0, 5));
        else
            return vec3(func(1, 0, 5), func(0, 1, 5), 0);
    }
    else
    {
        if (n == 0)
            return vec3(func(0, 1, 5), 0, func(1, 0, 5));
        else if (n == 1)
            return vec3(func(1, 0, 5), func(0, 1, 5), 0);
        else
            return vec3(0, func(1, 0, 5), func(0, 1, 5));
    }

    return vec3(0, 0, 0);
}

void main()
{
    gl_Position = vec4(aPos.x + offsetX, -aPos.y + offsetY, aPos.z, 1.0);
    gl_Position = vec4(aPos, 1.0);

    if (aColor.r > 0)
        ourColor = GetColor(0);
    else if (aColor.g > 0)
        ourColor = GetColor(1);
    else
        ourColor = GetColor(2);
}
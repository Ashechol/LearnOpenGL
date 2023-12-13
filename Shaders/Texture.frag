#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    // FragColor = texture(ourTexture, TexCoord);
    // FragColor = texture(texture1, TexCoord);
    // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);

    vec2 TexCoordInverse = vec2(-TexCoord.x, TexCoord.y);

    FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoordInverse), 0.2);
}
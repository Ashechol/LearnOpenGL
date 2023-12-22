#version 330 core

in vec3 Normal;
in vec4 FragPos;
// in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 color;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

// uniform sampler2D texture0;
// uniform sampler2D texture1;

void main()
{
    // FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);
    
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - vec3(FragPos));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - vec3(FragPos));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = lightColor * specularStrength * spec;

    FragColor = color * vec4(ambient + diffuse + specular, 1.0);
}
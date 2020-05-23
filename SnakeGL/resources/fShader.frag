#version 330 core
out vec4 FragColor;

in vec3 VertexColor;
in vec3 Normal;
in vec2 TexCoords;

in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 result = (diffuse + ambient) * VertexColor;
    FragColor = vec4(result, 1.0);
}

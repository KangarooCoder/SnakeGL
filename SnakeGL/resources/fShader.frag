#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    FragColor = vec4(vec3(texture(texture1, TexCoords)), 1.0);
}

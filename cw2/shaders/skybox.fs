#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox1;

void main()
{
    vec4 night = texture(skybox1, TexCoords);
    FragColor = night;
}

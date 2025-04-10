#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox1;
uniform samplerCube skybox2;
uniform float blendFactor; // 0.0 for skybox1, 1.0 for skybox2

void main()
{
    vec4 night = texture(skybox1, TexCoords);
    vec4 day = texture(skybox2, TexCoords);
    FragColor = mix(day, night, blendFactor);
}

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{
    // Sample the depth map texture and display it as grayscale
    float depth = texture(depthMap, TexCoords).r;
    FragColor = vec4(vec3(depth), 1.0);
}

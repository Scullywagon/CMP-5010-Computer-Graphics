#version 330 core

layout(location = 0) in vec3 aPos;      // Position of the vertex
layout(location = 1) in vec2 aTexCoords; // Texture coordinates

out vec2 TexCoords; // Pass the texture coordinates to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // Transform the vertex position to clip space
    TexCoords = aTexCoords; // Pass the texture coordinates to the fragment shader
}

#version 330 core
layout (location = 0) in vec3 aPos; // Position attribute
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); // Get the world-space position of the vertex
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = vec2(aTexCoords.x, aTexCoords.y);
    gl_Position = projection * view * vec4(FragPos, 1.0); // Apply transformations
}

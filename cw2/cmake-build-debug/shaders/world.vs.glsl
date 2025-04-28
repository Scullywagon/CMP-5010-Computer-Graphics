#version 330 core
layout (location = 0) in vec3 aPos; // Position attribute
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceModel;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform mat4 lightSpaceMatrix; 

void main()
{
    mat4 mod = instanceModel;
    vec3 pos = aPos;
    FragPos = vec3(mod * vec4(pos, 1.0)); // Get the world-space position of the vertex
    Normal = mat3(transpose(inverse(mod))) * aNormal;
    TexCoords = vec2(aTexCoords.x, aTexCoords.y);
    fragPosLightSpace = lightSpaceMatrix * mod * vec4(FragPos, 1.0); 
    gl_Position = projection * view * vec4(FragPos, 1.0); // Apply transformations
}

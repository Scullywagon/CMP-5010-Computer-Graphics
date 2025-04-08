#version 330 core
layout (location = 0) in vec3 aPos; // Position attribute
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform mat4 lightSpaceMatrix; 

uniform vec3 offsets[1000];
uniform bool isInstanced = false;

void main()
{
    vec3 pos = aPos;
    if (isInstanced) {
        vec3 offset = offsets[gl_InstanceID];
        pos += offset;
    }
    FragPos = vec3(model * vec4(pos, 1.0)); // Get the world-space position of the vertex
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = vec2(aTexCoords.x, aTexCoords.y);
    fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0); 
    gl_Position = projection * view * vec4(FragPos, 1.0); // Apply transformations
}

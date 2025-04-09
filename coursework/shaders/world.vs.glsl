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

uniform vec4 offsets[1000]; 
uniform bool isInstanced;

void main()
{
    mat4 mod = model;
    vec3 pos = aPos;
    if (isInstanced) {
        vec3 offset = offsets[gl_InstanceID].xyz;
        float rotation = offsets[gl_InstanceID].w;

        mat4 rotationMatrix = mat4(1.0); // Identity matrix
        float cosTheta = cos(rotation);
        float sinTheta = sin(rotation);

        rotationMatrix[0][0] = cosTheta;
        rotationMatrix[0][2] = sinTheta;
        rotationMatrix[2][0] = -sinTheta;
        rotationMatrix[2][2] = cosTheta;

        pos = vec3(rotationMatrix * vec4(pos, 1.0));

        pos += offset;
    }
    FragPos = vec3(mod * vec4(pos, 1.0)); // Get the world-space position of the vertex
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = vec2(aTexCoords.x, aTexCoords.y);
    fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0); 
    gl_Position = projection * view * vec4(FragPos, 1.0); // Apply transformations
}

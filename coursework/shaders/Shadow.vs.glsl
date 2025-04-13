#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 instanceModel;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

uniform bool isInstanced;

void main()
{
    mat4 mod = model;
    vec3 pos = aPos;
    if (isInstanced) {
        mod = instanceModel;
    }

    gl_Position = lightSpaceMatrix * mod * vec4(pos, 1.0);
}  

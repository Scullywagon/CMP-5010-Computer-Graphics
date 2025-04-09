#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

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

    gl_Position = lightSpaceMatrix * mod * vec4(pos, 1.0);
}  

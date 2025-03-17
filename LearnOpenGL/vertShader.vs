#version 330 core
layout (location = 0) in vec3 aPos;    // position
layout (location = 1) in vec2 aTexCoord; // texture coordinates

out vec2 TexCoord;  // pass texture coordinates to fragment shader

// Unifrom vars are constant
uniform mat4 model; // local space -> world space
uniform mat4 view; // word space -> camera space
uniform mat4 projection; // camera space -> clip space (after perspective division)

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // determines the final position of the vertex
    //TexCoord = aTexCoord;  // passes texture coordinates to fragment shader
}

#include "Shader.h"
using namespace std;

/*
 * This Program: 
 * - Reads the vertex and fragment shader files
 * - Compiles the shaders
 * - Links the shaders
 * - Deletes the shaders
 */
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    cout << "creating shader" << endl;
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;

    try 
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream; // char arrays
        // read the files into the streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close the files
        vShaderFile.close();
        fShaderFile.close();
        // convert the streams into strings
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (exception e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }

    const char* vShaderCode = vertexCode.c_str(); // convert the strings into char arrays
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment; // inits vertex and fragment shader IDs
                                   //
    vertex = glCreateShader(GL_VERTEX_SHADER); // create the vertex shader
    glShaderSource(vertex, 1, &vShaderCode, NULL); // attach the shader source code to the shader object
    glCompileShader(vertex); // compile the shader

    fragment = glCreateShader(GL_FRAGMENT_SHADER); // create the fragment shader
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    unsigned int ID = glCreateProgram(); // create a shader program
    glAttachShader(ID, vertex); // attach the vertex shader to the shader program
    glAttachShader(ID, fragment); // attach the fragment shader to the shader program
    glLinkProgram(ID); // link the shader program

    glDeleteShader(vertex); // delete the shaders
    glDeleteShader(fragment);
    cout << "created shader" << endl;
}

// To activate the shader
void Shader::use()
{
    glUseProgram(ID);
}

// The following functions set the values of the shader uniforms
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

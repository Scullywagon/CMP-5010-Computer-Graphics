#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << endl;
        }
    }
}


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

    // the following opens the files individually
    // converts both to string stream
    try 
    {
        vShaderFile.open(vertexPath);
        if (!vShaderFile.is_open()) {
            cout << "ERROR::SHADER::VERTEX_FILE_NOT_SUCCESFULLY_OPENED" << endl;
        }
        fShaderFile.open(fragmentPath);
        if (!fShaderFile.is_open()) {
            cout << "ERROR::SHADER::FRAGMENT_FILE_NOT_SUCCESFULLY_OPENED" << endl;
        }
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
    catch (exception &e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }



    const char* vShaderCode = vertexCode.c_str(); // convert the strings into char arrays
    const char* fShaderCode = fragmentCode.c_str();

    // Open GL creates shaders and returns ID's -- these can then be queried by the program
    unsigned int vertex, fragment; // inits vertex and fragment shader IDs

    vertex = glCreateShader(GL_VERTEX_SHADER); // create the vertex shader and return its ID
    glShaderSource(vertex, 1, &vShaderCode, NULL); // attach the shader source code to the shader object
    glCompileShader(vertex); // compile the shader
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER); // create the fragment shader
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    this->ID = glCreateProgram(); // create a shader program
    glAttachShader(this->ID, vertex); // attach the vertex shader to the shader program
    glAttachShader(this->ID, fragment); // attach the fragment shader to the shader program
    glLinkProgram(this->ID); // link the shader program
    checkCompileErrors(this->ID, "PROGRAM");

    glDeleteShader(vertex); // delete the shaders
    glDeleteShader(fragment);
    cout << "created shader" << endl;
}

// To activate the shader
void Shader::use()
{
    glUseProgram(this->ID);
}

// mat$ is a 4x4 matrix -> used for 3D transformations
void Shader::setMat4(const std::string &name,  const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

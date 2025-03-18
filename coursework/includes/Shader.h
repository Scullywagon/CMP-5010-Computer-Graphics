#include <GL/glew.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

#ifndef SHADER_H
#define SHADER_H

class Shader
{
  public:
    // Program ID -- the GLuint
    unsigned int ID;
    // Reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::exception &e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"
                      << std::endl;
        }

        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        unsigned int vertex,
            fragment; // holds the id of the vertex and fragment shaders

        vertex = glCreateShader(GL_VERTEX_SHADER); // create the vertex shader
        glShaderSource(vertex, 1, &vShaderCode,
                       NULL);    // attach the vertex shader to the source code
        glCompileShader(vertex); // compile the vertex shader
        checkCompileErrors(vertex, "VERTEX");

        fragment =
            glCreateShader(GL_FRAGMENT_SHADER); // create the fragment shader
        glShaderSource(fragment, 1, &fShaderCode,
                       NULL); // attach the fragment shader to the source code
        glCompileShader(fragment); // compile the fragment shader
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();     // create the program
        glAttachShader(ID, vertex); // attach the vertex shader to the program
        glAttachShader(ID,
                       fragment); // attach the fragment shader to the program
        glLinkProgram(ID);        // link the program
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);   // delete the vertex shader
        glDeleteShader(fragment); // delete the fragment shader
    }
    // To activate the shader
    void use()
    {
        glUseProgram(ID);
    }

    // utility uniform functions
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string &name, float x, float y, float z,
                 float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                           &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                           &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                           &mat[0][0]);
    }

    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout
                    << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                    << "\n"
                    << infoLog
                    << "\n -- "
                       "--------------------------------------------------- -- "
                    << std::endl;
            }
            else
            {
                std::cout << "Shader compiled successfully" << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout
                    << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                    << infoLog
                    << "\n -- "
                       "--------------------------------------------------- -- "
                    << std::endl;
            }
            else
            {
                std::cout << "Program linked successfully" << std::endl;
            }
        }
    }
};

#endif

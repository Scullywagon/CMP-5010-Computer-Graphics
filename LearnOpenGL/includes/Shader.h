#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // Program ID -- the GLuint
    unsigned int ID;
    // Reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // To activate the shader
    void use();
    // not sure yet
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

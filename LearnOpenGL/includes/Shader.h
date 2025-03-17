#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    void setMat4(const std::string &name,  const glm::mat4 &mat) const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

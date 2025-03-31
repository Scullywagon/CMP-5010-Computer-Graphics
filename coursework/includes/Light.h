#ifndef LIGHT_H
#define LIGHT_H

#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

struct Sun
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct Light
{
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#endif

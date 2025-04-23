#ifndef LIGHT_H
#define LIGHT_H

#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "glm/gtc/quaternion.hpp"
#include <glm/gtc/type_ptr.hpp>

struct WorldLight 
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    WorldLight(glm::vec3 direction, glm::vec3 ambient,
              glm::vec3 diffuse, glm::vec3 specular)
        : direction(direction), ambient(ambient), diffuse(diffuse),
          specular(specular)
    {
    }
};

struct Light
{
    int index;

    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
          glm::vec3 specular, float constant, float linear, float quadratic)
        : position(position), ambient(ambient), diffuse(diffuse),
          specular(specular), constant(constant), linear(linear),
          quadratic(quadratic)
    {
    }
};

#endif

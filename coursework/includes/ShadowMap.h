#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include "Shader.h"
#include <glm/glm.hpp>

class ShadowMap
{
  public:
    Shader shader;
    unsigned int depthMapFBO, depthMap;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    glm::vec3 sunPos = glm::vec3(0.6f, -0.6f, 0.4f) * -500.0f;

    glm::mat4 lightProjection =
        glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.0f, 10000.0f);
    glm::mat4 lightView =
        glm::lookAt(sunPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    ShadowMap(glm::vec3 lightDir)
        : shader("shaders/Shadow.vs.glsl", "shaders/Shadow.fs.glsl")
    {
        glGenFramebuffers(1, &depthMapFBO);
        unsigned int depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
                     SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void use()
    {
        shader.use();
        shader.setInt("diffuseTexture", 0);
        shader.setInt("shadowMap", 1);
    }
};

#endif

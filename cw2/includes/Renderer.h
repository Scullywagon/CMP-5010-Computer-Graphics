#ifndef RENDERER_H
#define RENDERER_H

#include "Model.h"
#include "Scene.h"
#include "Shader.h"

class Renderer
{
  public:
    Scene *scene;
    Shader *shader;
    Shader *depthShader;

    glm::mat4 projection;
    glm::mat4 view;

    glm::mat4 lightSpaceMatrix;
    glm::mat4 lightView;
    glm::vec3 lightPos;
    glm::mat4 lightProjection =
        glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 0.01f, 2000.0f);

    unsigned int depthMapFBO, depthMap;
    unsigned int instanceVBO;

    bool depthMode = false;

    // inits the renderer with the active stuff
    Renderer(Scene *scene);
    // dont know if this is needed
    void init();
    // render the scene each update
    void update();

  private:
    void generateDepthMap();
    void renderScene();
    void renderDepth();
    void drawModel(Model *model, vector<glm::mat4 *> &translations);
    void drawDepth(Model *model, vector<glm::mat4 *> &translations);
    void drawMesh(Mesh &mesh, int amount);
    void drawFloor();
};

#endif

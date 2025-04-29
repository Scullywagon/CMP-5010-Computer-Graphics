#include "Renderer.h"
#include "Constants.h"

Renderer::Renderer(Scene *scene)
{
    this->scene = scene;
    generateDepthMap();
    /*scene->player->camera->fov*/
}

void Renderer::init()
{
    glGenBuffers(1, &instanceVBO);
    this->shader =
        new Shader("./shaders/world.vs.glsl", "./shaders/world.fs.glsl");
    this->depthShader =
        new Shader("./shaders/Shadow.vs.glsl", "./shaders/Shadow.fs.glsl");
    projection = this->scene->cam.GetProjectionMatrix();
    // view = scene->player->camera->getViewMatrix();
    cout << "Renderer initialized" << endl;
}

void Renderer::generateDepthMap()
{
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);

    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Constants::SHADOW_WIDTH,
                 Constants::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
                    GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach depth texture to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depthMap, 0);
    glDrawBuffer(GL_NONE); // no color
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    lightPos = scene->worldLight.direction * -1000.0f;
    cout << lightPos.x << " " << lightPos.y << endl;
    lightProjection =
        glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 0.01f, 2000.0f);
    lightView =
        glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
}
int ticks = 0;
void Renderer::update()
{
    renderDepth();
    view = scene->cam.GetViewMatrix();
    projection = scene->cam.GetProjectionMatrix();
    scene->skybox.render(view, projection);
    renderScene();
    ticks++;
}

void Renderer::renderScene()
{
    this->shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setVec3("cameraPos", scene->cam.Position);
    shader->setMat4("model", glm::mat4(1.0f));
    shader->setVec3("sunDir", scene->worldLight.direction);
    shader->setVec3("sunAmbient", scene->worldLight.ambient);
    shader->setVec3("sunDiffuse", scene->worldLight.diffuse);
    shader->setVec3("sunSpecular", scene->worldLight.specular);
    shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    shader->setBool("isLight", false);
    int lightCount = 0;
    for (Light *light : scene->lights)
    {
        string element = "lights[" + std::to_string(lightCount) + "]";
        shader->setVec3(element + ".position", light->position);
        shader->setVec3(element + ".ambient", light->ambient);
        shader->setVec3(element + ".diffuse", light->diffuse);
        shader->setVec3(element + ".specular", light->specular);
        shader->setFloat(element + ".constant", light->constant);
        shader->setFloat(element + ".linear", light->linear);
        shader->setFloat(element + ".quadratic", light->quadratic);
        lightCount++;
    }
    shader->setInt("numLights", lightCount);

    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    shader->setInt("depthMap", 9);

    // Floor.render()
    for (auto &pair : scene->translations)
    {
        Model *model = pair.first;
        if (model->isLight == true)
        {
            shader->setBool("isLight", true);
            shader->setVec3("outputColor", model->outColor);
        }
        vector<glm::mat4 *> &translations = pair.second;
        drawModel(model, translations);
        shader->setBool("isLight", false);
    }
    drawFloor();
}

void Renderer::renderDepth()
{
    depthMode = true;
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete: " << status << std::endl;
    glViewport(0, 0, Constants::SHADOW_WIDTH, Constants::SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    depthShader->use();
    depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    for (auto &pair : scene->translations)
    {
        Model *model = pair.first;
        vector<glm::mat4 *> &translations = pair.second;
        drawModel(model, translations);
    }
    drawFloor();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
    depthMode = false;
}

void Renderer::drawModel(Model *model, vector<glm::mat4 *> &translations)
{
    if (translations.empty())
    {
        cout << "No translations for model" << endl;
        return;
    }

    vector<glm::mat4> translationsVec;

    for (glm::mat4 *translation : translations)
    {
        translationsVec.push_back(*translation);
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, translationsVec.size() * sizeof(glm::mat4),
                 translationsVec.data(), GL_DYNAMIC_DRAW);

    for (Mesh &mesh : model->meshes)
    {
        drawMesh(mesh, translations.size());
    }
}

void Renderer::drawMesh(Mesh &mesh, int amount)
{
    if (!depthMode)
    {
        if (!mesh.textures.empty())
        {
            const std::string &type = mesh.textures[0].type;
            GLuint texID = mesh.textures[0].id;

            glActiveTexture(GL_TEXTURE0);
            shader->setInt("material.diffuse", 0);
            shader->setFloat("material.shininess", mesh.shininess);
            glBindTexture(GL_TEXTURE_2D, texID);
        }
        else
        {
            shader->setFloat("material.shininess", 3.0f);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    // Bind VAO and set up instanced attributes
    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    for (GLuint i = 0; i < 4; ++i)
    {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                              (void *)(sizeof(glm::vec4) * i));
        glVertexAttribDivisor(3 + i, 1);
    }

    // Draw instanced
    glDrawElementsInstanced(GL_TRIANGLES,
                            static_cast<GLsizei>(mesh.indices.size()),
                            GL_UNSIGNED_INT, 0, amount);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

// patch spaghetti code
void Renderer::drawFloor()
{
    vector<glm::mat4> translations;
    translations.push_back(glm::mat4(1.0f));

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(glm::mat4),
                 translations.data(), GL_DYNAMIC_DRAW);

    drawMesh(scene->floor.meshes[0], translations.size());
}

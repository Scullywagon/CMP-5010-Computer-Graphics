#include "Renderer.h"
#include "Constants.h"

Renderer::Renderer(Scene *scene)
{
    this->scene = scene;
    /*scene->player->camera->fov*/
    projection = glm::perspective(glm::radians(90.0f),
                                  (float)Constants::SCREEN_WIDTH /
                                      (float)Constants::SCREEN_HEIGHT,
                                  Constants::NEAR_PLANE, Constants::FAR_PLANE);
    // view = scene->player->camera->getViewMatrix();
    view = this->scene->cam.GetViewMatrix(); ////////////
    cout << "Renderer initialized" << endl;
}

void Renderer::init()
{
    glGenBuffers(1, &instanceVBO);
    this->shader =
        new Shader("./shaders/world.vs.glsl", "./shaders/world.fs.glsl");
}

void Renderer::update()
{
    //    renderDepth();
    renderScene();
}

void Renderer::renderScene()
{
    this->shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setVec3("cameraPos", scene->cam.Position);
    shader->setMat4("model", glm::mat4(1.0f));

    // Floor.render()
    for (auto &pair : scene->translations)
    {
        Model *model = pair.first;
        vector<glm::mat4> &translations = pair.second;
        draw(model, translations);
    }
}

void Renderer::renderDepth()
{
    // glm::vec3 lp = scene->worldLight->direction * -1000.0f;
    glm::vec3 lp = glm::vec3(0.0f, 100.0f, 0.0f);
    if (this->lightPos != lp)
    {
        this->lightPos = lp;
        lightSpaceMatrix =
            lightProjection * glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f),
                                          glm::vec3(0.0f, 1.0f, 0.0f));
    }
    depthShader->use();
    depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    for (auto &pair : scene->translations)
    {
        Model *model = pair.first;
        vector<glm::mat4> &translations = pair.second;
        draw(model, translations);
    }
}

void Renderer::draw(Model *model, std::vector<glm::mat4> &translations)
{
    if (translations.empty())
    {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(glm::mat4),
                 translations.data(), GL_DYNAMIC_DRAW);

    for (Mesh &mesh : model->meshes)
    {
        drawMesh(mesh, translations.size());
    }
}

void Renderer::drawMesh(Mesh &mesh, int amount)
{
    shader->use();

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

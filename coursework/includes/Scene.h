#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Circus.h"
#include "CollisionManager.h"
#include "Cube.h"
#include "FerrisWheel.h"
#include "Floor.h"
#include "Light.h"
#include "Mesh.h"
#include "Model.h"
#include "ParentCamera.h"
#include "PersonCamera.h"
#include "Shader.h"
#include "ShadowMap.h"
#include "Skybox.h"
#include "Terrain.h"
#include "glm/detail/type_mat.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

struct Scene
{
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;

    ShadowMap *shadowMap;

    CollisionManager *collisionManager;
    FerrisWheel *ferrisWheel;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 theView;

    Skybox skybox;
    Floor floor;
    Terrain terrain;
    Sun sun;
    Circus circus;

    vector<Light *> lights;

    vector<ParentCamera *> cameras;
    ParentCamera *camera;
    int index = 0;

    bool enableRotation = false;
    Shader shader;

    Scene(int SCREEN_WIDTH, int SCREEN_HEIGHT)
        : shader("shaders/world.vs.glsl", "shaders/world.fs.glsl"),
          terrain(Terrain(shader))
    {
        this->SCREEN_WIDTH = SCREEN_WIDTH;
        this->SCREEN_HEIGHT = SCREEN_HEIGHT;

        shadowMap = new ShadowMap(sun.direction);
        collisionManager = new CollisionManager();
        ferrisWheel = new FerrisWheel();

        cameras.push_back(new Camera());
        cameras.push_back(new PersonCamera());
        for (Cart &cart : ferrisWheel->carts)
        {
            std::cout << "Adding camera" << std::endl;
            cameras.push_back(cart.camera);
        }
        camera = cameras[index];

        projection = glm::perspective(
            camera->fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,
            10000.0f);
        view = camera->GetViewMatrix();

        worldShader();
        setupCollisions();
        setupLights();
    }

    void setupCollisions()
    {
        collisionManager->add(ferrisWheel->stand.boundingBox);
        collisionManager->add(ferrisWheel->wheel.boundingBox);
        collisionManager->add(ferrisWheel->booth->booth->boundingBox);
        for (auto &cart : ferrisWheel->carts)
        {
            collisionManager->add(cart.cart.boundingBox);
        }

        collisionManager->add(circus.tent->boundingBox);
        collisionManager->add(circus.eye->boundingBox);
        collisionManager->add(circus.stage->boundingBox);
        collisionManager->add(circus.props->boundingBox);
        collisionManager->add(circus.sign->boundingBox);

        collisionManager->addPlayer(camera);
        collisionManager->sortItems();
    }

    void setupLights()
    {
        for (Cart &cart : ferrisWheel->carts)
        {
            lights.push_back(cart.lamp->light);
        }
        lights.push_back(circus.light);
        for (OilLamp *lamp : circus.lamps)
        {
            lights.push_back(lamp->light);
        }
    }

    void activateRotation()
    {
        enableRotation = !enableRotation;
    }

    void worldShader()
    {
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        shader.setVec3("sunDir", sun.direction);
        shader.setVec3("sunAmbient", sun.ambient);
        shader.setVec3("sunDiffuse", sun.diffuse);
        shader.setVec3("sunSpecular", sun.specular);
        shader.setVec3("cameraPos", camera->Position);
        shader.setBool("isInstanced", false);

        int index = 0;
        for (Light *light : lights)
        {
            std::string lightName = "lights[" + std::to_string(index) + "].";
            shader.setVec3(lightName + "position", light->position);
            shader.setVec3(lightName + "ambient", light->ambient);
            shader.setVec3(lightName + "diffuse", light->diffuse);
            shader.setVec3(lightName + "specular", light->specular);
            shader.setFloat(lightName + "constant", light->constant);
            shader.setFloat(lightName + "linear", light->linear);
            shader.setFloat(lightName + "quadratic", light->quadratic);
            light->index = index;
            index++;
        }
        shader.setInt("numLights", index);
    }

    float cameraTicker = 0.0f;
    void changeCamera(float currentFrame)
    {
        float deltaTime = currentFrame - cameraTicker;
        if (deltaTime < 1.0f)
        {
            return;
        }
        index = (index + 1) % cameras.size();
        camera = cameras[index];
        collisionManager->changePlayer(camera);
        cameraTicker = currentFrame;
    }

    //    Model *t = new Model("assets/fortuneTeller/fortuneTeller.obj", 0.5);

    void use(float deltaTime)
    {
        view = camera->GetViewMatrix();
        projection = glm::perspective(
            camera->fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,
            10000.0f);

        shadowMap->bind();
        glClear(GL_DEPTH_BUFFER_BIT);

        shadowMap->shader.use();
        shadowMap->shader.setMat4("lightSpaceMatrix",
                                  shadowMap->lightSpaceMatrix);
        ferrisWheel->drawDepth(shadowMap->shader);
        terrain.draw(shadowMap->shader);

        circus.drawDepth(shadowMap->shader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glCullFace(GL_BACK);
        skybox.use(view, projection);
        worldShader();
        shader.setMat4("lightSpaceMatrix", shadowMap->lightSpaceMatrix);

        glActiveTexture(GL_TEXTURE9);
        glBindTexture(GL_TEXTURE_2D, shadowMap->depthMap);
        shader.setInt("depthMap", 9);

        floor.use(shader);
        terrain.draw(shader);
        ferrisWheel->draw(shader);
        circus.draw(shader);

        sun.move(deltaTime);
        shadowMap->updateSunPos(sun.direction);
        skybox.blendFactor = sun.direction.y * 0.5f + 0.5f;
        shader.setBool("isLight", true);
        shader.setVec3("outputColor", glm::vec3(0.6f, 0.6f, 0.6f));
        sun.draw(shader);
        shader.setBool("isLight", false);

        if (enableRotation == true)
        {
            ferrisWheel->rotate(deltaTime);
        }

        if (index < 2)
        {
            collisionManager->check();
            camera->move();
        }
    }
};

#endif

#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "CollisionManager.h"
#include "FerrisWheel.h"
#include "Floor.h"
#include "Light.h"
#include "Mesh.h"
#include "Model.h"
#include "ParentCamera.h"
#include "PersonCamera.h"
#include "Shader.h"
#include "Skybox.h"
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
#include <sstream>
#include <string>
#include <vector>

struct Scene
{
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;

    CollisionManager *collisionManager;
    FerrisWheel *ferrisWheel;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection;
    glm::mat4 view;

    Skybox skybox;
    Floor floor;
    Sun sun = {{0.6f, -0.6f, 0.4f},
               {0.2f, 0.2f, 0.2f},
               {0.9f, 0.9f, 0.9f},
               {0.6f, 0.6f, 0.6f}};

    vector<ParentCamera *> cameras;
    ParentCamera *camera;
    int index = 0;

    bool enableRotation = false;
    Shader shader;
    Scene(int SCREEN_WIDTH, int SCREEN_HEIGHT)
        : shader("shaders/world.vs.glsl", "shaders/world.fs.glsl")
    {
        this->SCREEN_WIDTH = SCREEN_WIDTH;
        this->SCREEN_HEIGHT = SCREEN_HEIGHT;

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
    }

    void setupCollisions()
    {
        collisionManager->add(ferrisWheel->stand.boundingBox);
        collisionManager->add(ferrisWheel->wheel.boundingBox);

        for (auto &cart : ferrisWheel->carts)
        {
            collisionManager->add(cart.cart.boundingBox);
        }
        collisionManager->addPlayer(camera);
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

    void use(float deltaTime)
    {
        view = camera->GetViewMatrix();
        projection = glm::perspective(
            camera->fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,
            10000.0f);

        skybox.use(view, projection);
        worldShader();
        floor.use(shader);
        ferrisWheel->draw(shader, model);

        if (enableRotation == true)
        {
            ferrisWheel->rotate(deltaTime);
        }

        if (index < 2)
        {
            collisionManager->check();
            camera->move();
        }

        collisionManager->sortBound();
    }
};

#endif

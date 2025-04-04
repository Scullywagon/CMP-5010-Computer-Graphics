#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
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
    Sun sun = {{0.6f, -0.6f, 0.4f},
               {0.5f, 0.5f, 0.5f},
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
        initQuad();
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
        collisionManager->sortItems();
    }

    void activateRotation()
    {
        enableRotation = !enableRotation;
    }

    void worldShader()
    {
        shader.use();
        shader.setInt("depthMap", 10);
        glActiveTexture(GL_TEXTURE10); // Activate texture unit 10
        glBindTexture(GL_TEXTURE_2D, shadowMap->depthMap);
        shader.setMat4("lightSpaceMatrix", shadowMap->lightSpaceMatrix);
        shader.setInt("depthMap", 0);
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

    float quadVertices[20] = {
        // positions        // texture coordinates
        -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f  // top-right
    };

    Shader *depthMapVisualizerShader;
    unsigned int quadVAO, quadVBO;
    // Create and bind VAO and VBO
    void initQuad()
    {
        depthMapVisualizerShader =
            new Shader("shaders/depthTest.vs", "shaders/depthTest.fs");
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)(3 * sizeof(float)));
        glBindVertexArray(0);
    }
    void use(float deltaTime)
    {
        // view = shadowMap->lightView;
        // projection = shadowMap->lightProjection;
        view = camera->GetViewMatrix();
        projection = glm::perspective(
            camera->fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,
            10000.0f);

        shadowMap->use();
        floor.use(shadowMap->shader);
        ferrisWheel->draw(shadowMap->shader, model);

        depthMapVisualizerShader->use();
        depthMapVisualizerShader->setInt(
            "depthMap", 0); // Set the depth map as the shader's uniform

        // Bind the depth map texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(
            GL_TEXTURE_2D,
            shadowMap
                ->depthMap); // The depth map texture generated from shadowMap

        // Render the quad with the depth map texture
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
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
    }
};

#endif

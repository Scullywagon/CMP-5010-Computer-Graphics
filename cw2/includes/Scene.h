#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "CollisionManager.h"
#include "Entities/FerrisWheel.h"
#include "Entity.h"
#include "Floor.h"
#include "Light.h"
#include "Model.h"
#include "OrthographicCamera.h"
#include "Skybox.h"
#include <iostream>
#include <unordered_map>

using namespace std;

struct Scene
{
    // Asset management
    unordered_map<string, Model *> assets;
    unordered_map<Model *, vector<glm::mat4 *>> translations;

    vector<Entity *> entities;
    vector<Light *> lights;
    CollisionManager *collisionManager;

    // World
    Skybox skybox;
    Floor floor;
    WorldLight worldLight;
    Stand *stand;

    // Player
    Camera Player;
    OrthographicCamera overview;
    ParentCamera &cam = Player;

    Scene();
    void init();
    void update(float dt);
    void playerActivate();

  private:
    void testModels(string name);
    void addEntities(Entity &e);
    void generateTerrain();
};

#endif

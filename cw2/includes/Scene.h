#ifndef SCENE_H
#define SCENE_H

#include "Model.h"
#include <iostream>
#include <unordered_map>
#include "Skybox.h"
#include "Camera.h"

using namespace std;

struct Scene
{
    unordered_map<string, Model *> assets;
    unordered_map<Model *, vector<glm::mat4>> translations;

    Camera cam;

    Scene();

    void init();
    void update();
};

#endif

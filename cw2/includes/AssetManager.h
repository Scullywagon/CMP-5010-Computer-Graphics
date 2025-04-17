#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <iostream>
#include "Model.h"

class AssetManager
{
    // name and model asociated
    unordred_map<std::string, *Model> assets;
    unordred_map<*Model, vector<glm::mat4>> shaders;
};

#endif

#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.h"
#include "AssetManager.h"

class Renderer
{
  public:
    Scene *scene;
    AssetManager *assetManager;

    // inits the renderer with the active stuff
    Renderer(Scene *scene);
    // dont know if this is needed
    void init();
    // render the scene each update
    void update();
};

#endif

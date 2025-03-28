#ifndef FERRISWHEEL_H
#define FERRISWHEEL_H

#include "Model.h"
#include <vector>

struct Cart
{
    Model cart;
    glm::vec3 position;
};

struct FerrisWheel
{
    Model stand;
    Model wheel;
    vector<Cart> carts;
};

#endif

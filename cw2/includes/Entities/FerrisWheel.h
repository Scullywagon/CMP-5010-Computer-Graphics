#ifndef FERRISWHEEL_H
#define FERRISWHEEL_H

#include "Entity.h"

class Stand : public Entity
{
  public:
    Stand(Model *model);
    void init() override;
    void update() override;

  private:
};

#endif

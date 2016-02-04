#ifndef SCENE_H
#define SCENE_H

#include "commons.h"

class Scene{
public:
  View* view;
  Object* object;

  //Parent
  Window* parentWindow;

  Scene(Window* window);
};

#endif

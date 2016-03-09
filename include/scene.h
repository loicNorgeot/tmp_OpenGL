#ifndef SCENE_H
#define SCENE_H

#include "commons.h"

class Scene{
public:
  View* view;
  std::vector<Object*> objects;
  Object* active;
  Background* background;
  Axis* axis;

  //Parent
  Window* parentWindow;
  bool selected;
  glm::vec3 center;

  Scene(Window* window);
  void render();
};

#endif

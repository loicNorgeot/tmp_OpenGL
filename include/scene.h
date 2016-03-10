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
  bool ground;
  glm::vec3 center;
  float scale;

  Scene(Window* window);
  void addObject(Object* o);
  void addGround();
  void render();
};

#endif

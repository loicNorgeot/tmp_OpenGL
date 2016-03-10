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
  bool ground=false;
  glm::vec3 center;
  float scale = 0.5f;

  Scene(Window* window);
  void addObject(Object* o);
  void addGround();
  void render();
};

#endif

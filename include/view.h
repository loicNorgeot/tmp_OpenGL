#ifndef VIEW_H
#define VIEW_H

#include "commons.h"

class View{
public:
  float     fov;
  int       w, h;
  float     min, max;

  glm::vec3 cam;
  glm::vec3 look;
  glm::vec3 up;

  glm::mat4 VIEW;
  glm::mat4 PROJ;

  //Parent
  Scene* parentScene;

  View(Scene * scene);
  void update();
};

#endif

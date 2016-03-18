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
  glm::vec3 right;
  bool      ortho;
  float     zoom;

  glm::mat4 VIEW;
  glm::mat4 PROJ;

  //Parent
  Scene* parentScene;

  View(Scene * scene);
  void update();

  void upView();
  void frontView();
  void sideView();
  void toogleOrtho();
};

#endif

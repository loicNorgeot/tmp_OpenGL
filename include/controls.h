#ifndef CONTROLS_H
#define CONTROLS_H

#include "commons.h"

class Controls{
 public:
  glm::vec3 buttons;
  glm::vec2 oldPos;
  glm::vec2 pos;

  GLenum render;
  GLenum cull;

  Window * parentWindow;

  Controls(Window * window);
  void init();
};

#endif

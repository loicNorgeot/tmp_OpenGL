#ifndef CONTROLS_H
#define CONTROLS_H

#include "commons.h"

class Controls{
 public:
  glm::vec3 buttons;
  glm::vec2 oldPos;
  glm::vec2 pos;

  
  GLenum cull;
  bool   animate;
  bool   ortho;

  int    structure;
  int    lighting;
  int    colors;

  Window * parentWindow;

  Controls(Window * window);
  void init();
};

#endif

#ifndef WINDOW_H
#define WINDOW_H

#include "commons.h"

class Window{
 public:
  GLFWwindow* window;
  Controls*   controls;
  Scene*      scene;
  CONTEXT*    parentContext;
  Shader*     shader;
  font*       FONT;
  CONTROLS    TYPE;

  Window(CONTROLS type, MODE windowMode, int resX=640, int resY=480, int posX=0, int posY=0);
  void addObject(Object * o);
  void addGround();
  void render();
  void swap();
  bool shouldStop();
};

#endif

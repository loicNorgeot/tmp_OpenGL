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

  Window(MODE windowMode, int resX, int resY, int posX, int posY);
  void addObject(Object * o);
  void addGround();
  void render();
  void swap();
  bool shouldStop();
};

#endif

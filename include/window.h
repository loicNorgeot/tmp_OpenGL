#ifndef WINDOW_H
#define WINDOW_H

#include "commons.h"

class Window{
 public:
  GLFWwindow* window;
  Controls*   controls;
  Scene*      scene;
  CONTEXT *   parentContext;

  Window(CONTEXT * context, MODE windowMode, int resX, int resY, int posX, int posY);
  void init();
  void render();
  void swap();
};

#endif

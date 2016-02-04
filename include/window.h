#ifndef WINDOW_H
#define WINDOW_H

#include "commons.h"

class Window{
 public:
  GLFWwindow* window;
  Controls*   controls;
  Scene*      scene;
  CONTEXT *   parentContext;

  Window(CONTEXT * context, int resX, int resY);
  void init();
};

#endif

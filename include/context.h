#ifndef CONTEXT_H
#define CONTEXT_H

#include "commons.h"

class CONTEXT{
public:
  Window* window;
  Shader* shader;

  CONTEXT(MODE mode);
  void init();
  void render();
  bool shouldStop();
  int stop();
};

#endif
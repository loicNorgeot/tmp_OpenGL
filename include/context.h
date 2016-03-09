#ifndef CONTEXT_H
#define CONTEXT_H

#include "commons.h"

class CONTEXT{
public:
  Window* window;
  std::vector<Window*> windows;
  Shader* shader;

  CONTEXT(MODE mode);
  void render();
  bool shouldStop();
  int stop();
};

#endif

#ifndef AXIS_H
#define AXIS_H
#include "commons.h"

class Axis{
public:
  Axis(Scene* scene);
  void createGeometry();
  void createAndBindBuffers();
  void render();
  std::vector<float>  vertices;
  std::vector<float>  colors;
  GLuint              mBuffer;
  GLuint              cBuffer;
  GLuint              VAO;
  glm::mat4	          MODEL;
  Scene* parentScene;
};

#endif // AXIS_H

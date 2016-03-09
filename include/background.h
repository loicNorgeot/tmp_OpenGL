#ifndef BACKGOUND_H
#define BACKGOUND_H
#include "commons.h"

class Background{
public:

  Background(Scene* scene);
  void createGeometry();
  void createAndBindBuffers();
  void render();

  int nbVertices;
  std::vector<float>  vertices;
  std::vector<float>  normals;
  std::vector<float>  colors;
  std::vector<int>    indTri;
  GLuint              mBuffer;
  GLuint              nBuffer;
  GLuint              cBuffer;
  GLuint              iBuffer;
  GLuint              VAO;
  glm::mat4	          MODEL;
  Scene* parentScene;
};

#endif // BACKGOUND_H

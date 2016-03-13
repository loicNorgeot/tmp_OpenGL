#ifndef OBJECT_H
#define OBJECT_H

#include "commons.h"

class Object{
public:
  int nbVertices;
  glm::vec3 pickingColor;
  int       id;

  std::vector<float>  vertices;
  std::vector<float>  normals;
  std::vector<float>  colors;
  std::vector<int>    refTri, refTet, refVert;
  std::vector<int>    indTri, indTet;
  std::vector<int>    adjacent;

  std::string         meshfile;
  std::string         name;

  GLuint              mBuffer;
  GLuint              nBuffer;
  GLuint              cBuffer;
  GLuint              iBuffer;
  GLuint              VAO;

  float               scale;
  glm::vec3           translation;
  glm::mat4	          MODEL;

  bool selected;
  glm::vec3 size;

  //Parent
  Scene* parentScene;

  Object();
  Object(char * mesh_path);
  Object(Object* object);
  void init(Scene* scene);
  void computeID(int _id);

  void createGeometry();
  void createGeometry(char * mesh_path);
  void computeBoundingBox();
  void scaleAndTranslate();
  void createAndBindBuffers();

  void render();
  void pickingRender();
};

#endif

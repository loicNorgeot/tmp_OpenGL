#ifndef OBJECT_H
#define OBJECT_H

#include "commons.h"

class Object{
public:
  int nbVertices;
	
  std::vector<float>  vertices;
  std::vector<float>  normals;
  std::vector<float>  colors;
  std::vector<int>    indices;

  GLuint              mBuffer;
  GLuint              nBuffer;
  GLuint              cBuffer;
  GLuint              iBuffer;
  GLuint              VAO;

  float               scale;
  glm::vec3           translation;
  glm::mat4	      MODEL;

  //Parent
  Scene* parentScene;

  Object(Scene* scene);
  Object(Scene* scene, char * mesh_path);
  void createGeometry();
  void createGeometry(char * mesh_path);
  void computeBoundingBox();
  void scaleAndTranslate();
  void createAndBindBuffers();
  void render();
};

typedef struct {
  double    c[3];
  int       ref;
} Point;
//typedef Point * pPoint;
typedef struct {
  int       v[3],ref;
} Tria;
//typedef Tria * pTria;
typedef struct {
  double    n[3];
} Normal;
//typedef Normal * pNormal;
typedef struct {
  int inds[2];
} NormalAtVertex;

#endif

#include "background.h"

Background::Background(Scene* scene){
  parentScene = scene;
  createGeometry();
  createAndBindBuffers();
  MODEL = glm::mat4(1);
}

void Background::createGeometry(){
  vertices = std::vector<float>{
    1.0, 0,  1.0,
    -1.0, 0,  1.0,
    -1.0,  0, -1.0,
    1.0,  0,  -1.0
  };
  for(int i = 0 ; i < vertices.size() ; i++)
    vertices[i] = 0.5f * vertices[i];
  nbVertices = vertices.size()/3;
  indTri = std::vector<int>{
    0, 1, 2,
    2, 3, 0
  };
  colors = std::vector<float>{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f
  };
  normals = std::vector<float>{
    0, 1, 0,
    0, 1, 0
  };
}

void Background::createAndBindBuffers(){
  createVAO(&VAO);
  createBuffer(&mBuffer, &vertices);
  createBuffer(&nBuffer, &normals);
  createBuffer(&cBuffer, &colors);
  createBuffer(&iBuffer, &indTri);

  int shaderID = parentScene->parentWindow->shader->ID;
  glUseProgram(shaderID);
  glBindVertexArray(VAO);
  bindBuffer(0, shaderID, mBuffer, "vertex_position");
  bindBuffer(1, shaderID, nBuffer, "vertex_normal");
  bindBuffer(2, shaderID, cBuffer, "vertex_color");
  bindIndicesBuffer(iBuffer);
}


void Background::render(){
  int ID = parentScene->parentWindow->shader->ID;
  glUseProgram(ID);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glm::mat4 MVP = parentScene->view->PROJ * parentScene->view->VIEW * MODEL;

  //Uniform sending
  send(ID, MVP, 	                              "MVP");
  send(ID, MODEL, 	                            "M");
  send(ID, parentScene->view->VIEW,             "V");
  send(ID, 1, "uLighting");
  send(ID, 3,   "uColor");
  send(ID, 0,"uStructure");
  send(ID, glm::vec3(1,1,1),                  "objectColor");
  send(ID, 0,                                   "uSecondPass");
  send(ID, 0, "picking");

  //Drawing
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indTri.size(), GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);
}

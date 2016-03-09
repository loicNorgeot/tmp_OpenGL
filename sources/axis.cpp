#include "axis.h"


Axis::Axis(Scene* scene){
  parentScene = scene;
  createGeometry();
  createAndBindBuffers();
  MODEL = glm::mat4(1);
}

void Axis::createGeometry(){
  vertices = std::vector<float>{
    0.0, 0.0,  0.0,
    1.0, 0.0,  0.0,
    0.0, 0.0,  0.0,
    0.0, 1.0,  0.0,
    0.0, 0.0,  0.0,
    0.0, 0.0,  1.0
  };
  for(int i = 0 ; i < vertices.size() ; i++)
    vertices[i] = 0.5f * vertices[i];
}

void Axis::createAndBindBuffers(){
  createVAO(&VAO);
  createBuffer(&mBuffer, &vertices);
  int shaderID = parentScene->parentWindow->parentContext->shader->ID;
  glUseProgram(shaderID);
  glBindVertexArray(VAO);
  bindBuffer(0, shaderID, mBuffer, "vertex_position");
}


void Axis::render(){
  int ID = parentScene->parentWindow->parentContext->shader->ID;
  glUseProgram(ID);
  glDisable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
  glm::mat4 M   = glm::scale(MODEL, glm::vec3(parentScene->view->zoom));
  glm::mat4 MVP = parentScene->view->PROJ * parentScene->view->VIEW * M;
  glLineWidth(2);
  //Uniform sending
  send(ID, MVP, 	                              "MVP");
  send(ID, M, 	                            "M");
  send(ID, parentScene->view->VIEW,             "V");
  send(ID, 0, "uLighting");
  send(ID, 0,   "uColor");
  send(ID, 0,"uStructure");
  send(ID, glm::vec3(1,1,1),                  "objectColor");
  send(ID, 0,                                   "uSecondPass");
  send(ID, 0, "picking");

  //Drawing
  int w,h;
  glfwGetWindowSize(parentScene->parentWindow->window, &w, &h);
  glViewport(0,0,w/6,h/6);
  glBindVertexArray(VAO);
  send(ID, glm::vec3(1,0,0),                  "objectColor");
  glDrawArrays(GL_LINES, 0, 2);
  send(ID, glm::vec3(0,1,0),                  "objectColor");
  glDrawArrays(GL_LINES, 2, 4);
  send(ID, glm::vec3(0,0,1),                  "objectColor");
  glDrawArrays(GL_LINES, 4, 6);
  glBindVertexArray(0);
  glEnable(GL_DEPTH_TEST);
  glViewport(0,0,w,h);
}

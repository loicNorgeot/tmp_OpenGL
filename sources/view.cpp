#include "view.h"

View::View(Scene* scene){
  parentScene = scene;
  fov = glm::radians(45.0f);
  min = 0.1f;
  max = 100.0f;
  cam = glm::vec3(1);
  look = glm::vec3(0,0,0);
  up = glm::normalize(glm::vec3(-1,1,-1));
  update();
}
void View::update(){
  glfwGetWindowSize(parentScene->parentWindow->window, &w, &h);
  PROJ = glm::perspective(fov, (float)w / (float)h, min, max);
  VIEW = glm::lookAt(cam, look, up);
}

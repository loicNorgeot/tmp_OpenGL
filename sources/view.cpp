#include "view.h"

View::View(Scene* scene){
  parentScene = scene;
  fov = glm::radians(45.0f);
  min = 0.005f;
  max = 20.0f;
  cam   = glm::normalize(glm::vec3(1));
  look  = glm::vec3(0,0,0);//-cam
  up    = glm::normalize(glm::vec3(-1,1,-1));
  right = glm::normalize(glm::vec3(1,0,-1));
  zoom = 1.0;
  update();
}
void View::update(){
  glfwGetWindowSize(parentScene->parentWindow->window, &w, &h);
  if(cam.y<0)
    cam.y=0;
  cam = zoom * glm::normalize(cam);
  if(parentScene->parentWindow->controls->ortho){
    float f = 0.5f * zoom;
    PROJ = glm::ortho(-f*(float)w/(float)h,f*(float)w/(float)h,-f,f,min,max);
  }
  else
    PROJ = glm::perspective(fov, (float)w / (float)h, min, max);
  VIEW = glm::lookAt(cam, look, up);
}

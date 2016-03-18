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
  ortho = false;
  zoom = 1.0;
  if(parentScene->parentWindow->TYPE == GALERY){
    ortho = true;
    upView();
  }
  update();
}
void View::update(){
  glfwGetWindowSize(parentScene->parentWindow->window, &w, &h);
  if(parentScene->parentWindow->TYPE != MANIPULATION && cam.y<0)
    cam.y=0;
  cam = zoom * glm::normalize(cam);
  if(ortho){
    float f = 0.5f * zoom;
    PROJ = glm::ortho(-f*(float)w/(float)h,f*(float)w/(float)h,-f,f,min,max);
  }
  else
    PROJ = glm::perspective(fov, (float)w / (float)h, min, max);
  VIEW = glm::lookAt(cam, look, up);
}

void View::upView(){
  cam = glm::vec3(0,1,0);
  right = glm::vec3(1,0,0);
  up = glm::vec3(0,0,-1);
  update();
}
void View::frontView(){
  cam = glm::vec3(0,0,1);
  right = glm::vec3(1,0,0);
  up = glm::vec3(0,1,0);
  update();
}
void View::sideView(){
  cam = glm::vec3(1,0,0);
  right = glm::vec3(0,0,-1);
  up = glm::vec3(0,1,0);
  update();
}
void View::toogleOrtho(){
  ortho = !ortho;
  update();
}
  void ortho();

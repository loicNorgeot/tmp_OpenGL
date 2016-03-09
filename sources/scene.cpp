#include "scene.h"

Scene::Scene(Window* window){
  parentWindow = window;
  view = new View(this);
  background = new Background(this);
  axis = new Axis(this);
  selected = true;
  center = glm::vec3(0);
}

void Scene::render(){
  glfwPollEvents();
  glClearColor(0.1,0.1,0.1,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  background->render();
  for(int i = 0 ; i < objects.size() ; i++)
    objects[i]->render();
  axis->render();
}

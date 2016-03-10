#include "scene.h"

Scene::Scene(Window* window){
  parentWindow = window;
  view = new View(this);
  axis = new Axis(this);
  selected = true;
  center = glm::vec3(0);
}
void Scene::addObject(Object* o){
  objects.push_back(o);
  o->init(this);
}
void Scene::addGround(){
  ground = true;
  background = new Background(this);
}
void Scene::render(){
  if(ground)
    background->render();
  for(int i = 0 ; i < objects.size() ; i++)
    objects[i]->render();
  axis->render();
}

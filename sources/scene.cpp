#include "scene.h"

Scene::Scene(Window* window){
  scale = 0.5f;
  ground = false;
  parentWindow = window;
  view = new View(this);
  axis = new Axis(this);
  selected = true;
  center = glm::vec3(0);
  parentWindow->controls->lighting = 2;
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
  for(int i = 0 ; i < objects.size() ; i++){
    objects[i]->render();
    if(objects[i]->selected)
      parentWindow->FONT->render(objects[i]->name, 10, 10, 0.5, glm::vec3(1,1,0.5));
  }
  axis->render();
}

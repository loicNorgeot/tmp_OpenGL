#include "scene.h"

Scene::Scene(Window* window){
  parentWindow = window;
  view = new View(this);
}

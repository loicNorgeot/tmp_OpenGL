#include "scene.h"

Scene::Scene(Window* window){
  parentWindow = window;
  view = new View(this);
  //object = new Object(this);
  object = new Object(this,"/home/norgeot/Bureau/3DASSETS/NumeRO/248.mesh");
}

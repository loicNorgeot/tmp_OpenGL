#include "commons.h"

int main(int argc, char** argv){
  context = new CONTEXT();

  //Single mesh
  if(argc==2){
    Window* window = new Window(FULLSCREEN, 1366,  768, 0,   0);
    window->addObject( new Object(argv[1]) );
  }

  else if(argc>2){
    //Fenêtre de gauche: cube
    Window* leftWindow = new Window(WINDOWED, 366,  768, 0,   0);
    for(int i = 1 ; i < argc ; i++){
      Object* obj = new Object(argv[i]);
      leftWindow->addObject(obj);
      obj->MODEL = glm::translate(obj->MODEL, glm::vec3(0,0,0.25f*i));
    }

    //Fenêtre de droite : objets
    Window* rightWindow = new Window(WINDOWED, 1000, 768, 366, 0);
    rightWindow->addObject(new Object(argv[1]));
    rightWindow->addGround();
  }

  else
    exit(-1);

  while( ! context->shouldStop() )
    context->render();

  return context->stop();
}






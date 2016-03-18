#include "commons.h"

int main(int argc, char** argv){
  context = new CONTEXT();

  //Cube
  if(argc==1){
    Window* window = new Window(MANIPULATION, FULLSCREEN);
    window->addObject( new Object() );
  }

  //Single mesh
  else if(argc==2){
    Window* window = new Window(MANIPULATION, FULLSCREEN);
    window->addObject( new Object(argv[1]) );
  }

  else if(argc>2){
    //Fenêtre de gauche: galerie des objets disponibles
    Window* leftWindow = new Window(GALERY, WINDOWED, 366,  766, 0,   0);
    for(int i = 1 ; i < argc ; i++){
      Object* obj = new Object(argv[i]);
      leftWindow->addObject(obj);
      obj->MODEL = glm::translate(obj->MODEL, glm::vec3(0,0,0.25f*i));
    }

    //Fenêtre de droite : "paillasse de manipulation"
    Window* rightWindow = new Window(RECONSTRUCTION, WINDOWED, 1000, 766, 366, 0);
    rightWindow->addObject(new Object(argv[1]));
    rightWindow->addGround();
  }

  else
    exit(-1);

  while( ! context->shouldStop() )
    context->render();

  return context->stop();
}






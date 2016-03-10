#include "commons.h"

CONTEXT * context;

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
    leftWindow->addObject( new Object() );
    leftWindow->addObject(new Object(argv[1]));
    leftWindow->addObject(new Object(argv[2]));

    //Fenêtre de droite : objets
    Window* rightWindow = new Window(WINDOWED, 1000, 768, 366, 0);
    for(int i = 1 ; i < argc ; i++)
      rightWindow->addObject( new Object(argv[i]) );
    rightWindow->addGround();


  }

  else
    exit(-1);

  while( ! context->shouldStop() )
    context->render();

  return context->stop();
}






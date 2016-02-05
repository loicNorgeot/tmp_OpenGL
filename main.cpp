#include "ogl.h"

CONTEXT * context;

int main(int argc, char** argv){

  if(argc>2){
    std::cout << "Usage : cube [file.mesh]" << std::endl;
    exit(-1);
  }

  initGLFW();

  context = new CONTEXT();

  initGLEW();

  context->init();

  if(argc==1)
    context->window->scene->object = new Object(context->window->scene);
  else if(argc==2)
    context->window->scene->object = new Object(context->window->scene, argv[1]);

  while( ! context->shouldStop() ){
    context->render();
  }

  return context->stop();
}






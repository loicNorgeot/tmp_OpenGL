#include "commons.h"

CONTEXT * context;

int main(int argc, char** argv){

  if(argc>3){
    std::cout << "Usage : cube [file.mesh] [-fs]" << std::endl;
    exit(-1);
  }

  context = new CONTEXT( (argc == 3 && std::string(argv[2]).compare("-fs") == 0)? FULLSCREEN : WINDOWED);

  if(argc==1)
    context->window->scene->object = new Object(context->window->scene);
  else
    context->window->scene->object = new Object(context->window->scene, argv[1]);

  while( ! context->shouldStop() ){
    context->render();
  }

  return context->stop();
}






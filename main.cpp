#include "commons.h"

CONTEXT * context;

int main(int argc, char** argv){
  context = new CONTEXT( (argc == 4 && std::string(argv[3]).compare("-fs") == 0)? FULLSCREEN : WINDOWED);

  if( argc == 1 )
    context->window->scene->object = new Object(context->window->scene);
  else if( argc == 2 )
    context->window->scene->object = new Object(context->window->scene, argv[1]);
  else if( argc == 3 )
    context->window->scene->object = new Object(context->window->scene, argv, argc);
  else
    exit(-1);

  while( ! context->shouldStop() )
    context->render();

  return context->stop();
}






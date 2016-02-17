#include "commons.h"

/*
~/Bureau/3DASSETS/final/tmp/A_canthare.o.mesh  ~/Bureau/3DASSETS/final/tmp/B_3351.o.mesh  ~/Bureau/3DASSETS/final/tmp/C_3354.o.mesh  ~/Bureau/3DASSETS/final/tmp/D_3354a.o.mesh  ~/Bureau/3DASSETS/final/tmp/E_3257.o.mesh  ~/Bureau/3DASSETS/final/tmp/F_3354.o.mesh
*/

CONTEXT * context;

int main(int argc, char** argv){
  context = new CONTEXT( (argc == 4 && std::string(argv[3]).compare("-fs") == 0)? FULLSCREEN : WINDOWED);

  if( argc == 1 )
    context->window->scene->object = new Object(context->window->scene);
  else if( argc == 2 )
    context->window->scene->object = new Object(context->window->scene, argv[1]);
  else if( argc > 2 )
    context->window->scene->object = new Object(context->window->scene, argv, argc);
  else
    exit(-1);

  while( ! context->shouldStop() )
    context->render();

  return context->stop();
}






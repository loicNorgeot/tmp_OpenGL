#include "commons.h"

/*
~/Bureau/3DASSETS/final/tmp/A_canthare.o.mesh  ~/Bureau/3DASSETS/final/tmp/B_3351.o.mesh  ~/Bureau/3DASSETS/final/tmp/C_3354.o.mesh  ~/Bureau/3DASSETS/final/tmp/D_3354a.o.mesh  ~/Bureau/3DASSETS/final/tmp/E_3257.o.mesh  ~/Bureau/3DASSETS/final/tmp/F_3354.o.mesh
*/

CONTEXT * context;

int main(int argc, char** argv){
  context = new CONTEXT( (argc == 4 && std::string(argv[3]).compare("-fs") == 0)? FULLSCREEN : WINDOWED);

  int i = 0;

  for(Window* win : context->windows){
    context->window = win;
    //Cube
    if( argc == 1 )
      context->window->scene->objects.push_back( new Object(context->window->scene) );

    //single .mesh file
    else if( argc == 2 )
      context->window->scene->objects.push_back( new Object(context->window->scene, argv[1]) );

    //multiple .mesh files
    else if( argc > 2 ){
      if(i==1)
        context->window->scene->objects.push_back( new Object(context->window->scene) );
      else
        for(int i = 1 ; i < argc ; i++)
          context->window->scene->objects.push_back( new Object(context->window->scene, argv[i]) );
    }
    else
      exit(-1);
    i++;
  }

  while( ! context->shouldStop() )
    context->render();

  return context->stop();
}






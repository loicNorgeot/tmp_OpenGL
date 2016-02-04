#include "ogl.h"

CONTEXT * context;

int main(int argc, char** argv){

  initGLFW();

  context = new CONTEXT();

  initGLEW();

  context->init();
  context->shader->load("/home/norgeot/dev/tmp_OpenGL/shaders/shader.vert", "/home/norgeot/dev/tmp_OpenGL/shaders/shader.frag");
  //context->shader->load("/home/norgeot/dev/tmp_OpenGL/shaders/smooth.vert", "/home/norgeot/dev/tmp_OpenGL/shaders/smooth.frag");

  while( ! context->shouldStop() ){
    context->render();
  }

  return context->stop();
}






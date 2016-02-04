#ifndef OGL_H
#define OGL_H

#include "commons.h"

void initGLFW(){
  if(!glfwInit()){
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(-1);
  }
}

void initGLEW(){
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err){
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    exit(-1);
  }
}

#endif


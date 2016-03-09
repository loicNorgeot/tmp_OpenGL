#include "context.h"

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

CONTEXT::CONTEXT(MODE mode){
  context = this;
  initGLFW();
  windows.push_back(new Window(this, mode, 1000, 768, 366, 0));
  windows.push_back(new Window(this, mode, 366,  768, 0,   0));
  windows.push_back(new Window(this, mode, 366,  768, 0,   0));
  shader = new Shader(this);
  initGLEW();
  for(Window* win : windows){
    window = win;
    glfwMakeContextCurrent(win->window);
    win->init();
  }
  shader->load(shadersPath + "shader.vert", shadersPath + "shader.frag", shadersPath + "shader.functions");
}
bool CONTEXT::shouldStop(){
  bool goesOn = glfwGetKey(window->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window->window) == 0;
  return !goesOn;
}
void CONTEXT::render(){
  window->render();
  window->swap();
}
int CONTEXT::stop(){
  for(Window* win : windows)
    glfwDestroyWindow(win->window);
  glfwTerminate();
  return 0;
}

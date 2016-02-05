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
  window = new Window(this, mode, 1024, 620);
  shader = new Shader(this);
  initGLEW();
  window->init();
  shader->load(shadersPath + "shader.vert", shadersPath + "shader.frag", shadersPath + "shader.functions");
}
void CONTEXT::init(){}
bool CONTEXT::shouldStop(){
  bool goesOn = glfwGetKey(window->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window->window) == 0;
  return !goesOn;
}
void CONTEXT::render(){
  glfwPollEvents();
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  window->scene->object->render();
  glfwSwapBuffers(window->window);
}
int CONTEXT::stop(){
  glfwDestroyWindow(window->window);
  glfwTerminate();
  return 0;
}

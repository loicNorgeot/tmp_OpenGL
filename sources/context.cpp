#include "context.h"

CONTEXT::CONTEXT(){
  window = new Window(this, 1024, 620);
  shader = new Shader(this);
}
void CONTEXT::init(){
  window->init();
}
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

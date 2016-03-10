#include "context.h"

CONTEXT * context;

void CONTEXT::addWindow(Window* w){
  windows.push_back(w);
}

void CONTEXT::render(){
  for(Window* win : windows){
    glfwMakeContextCurrent(win->window);
    win->render();
    win->swap();
  }
  glfwPollEvents();
}

bool CONTEXT::shouldStop(){
  bool goesOn = glfwGetKey(window->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window->window) == 0;
  return !goesOn;
}
int CONTEXT::stop(){
  for(Window* win : windows)
    glfwDestroyWindow(win->window);
  glfwTerminate();
  return 0;
}

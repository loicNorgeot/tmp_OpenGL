#include "window.h"

Window::Window(CONTEXT * context, MODE windowMode, int resX, int resY, int posX=0, int posY=0){
  parentContext = context;
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint( GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  if(windowMode == FULLSCREEN)
    window = glfwCreateWindow(mode->width, mode->height, "TEST", glfwGetPrimaryMonitor(), NULL);
  else if(windowMode == WINDOWED)
    window = glfwCreateWindow(resX, resY, "TEST", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSetWindowPos(window,posX, posY);

  if(window==NULL){
    std::cout << "Can't create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }
}

void Window::init(){
  controls = new Controls(this);
  scene    = new Scene(this);
}
void Window::render(){
  scene->render();
  glFlush();
}
void Window::swap(){
  glfwMakeContextCurrent(window);
  glfwSwapBuffers(window);
  glfwSwapInterval(1);
}


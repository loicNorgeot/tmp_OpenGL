#include "window.h"

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

Window::Window(MODE windowMode, int resX, int resY, int posX=0, int posY=0){
  shader = new Shader(this);
  initGLFW();
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
  if(windowMode != FULLSCREEN)
    glfwSetWindowPos(window,posX, posY);

  if(window==NULL){
    std::cout << "Can't create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  initGLEW();

  shader->load(shadersPath + "shader.vert", shadersPath + "shader.frag", shadersPath + "shader.functions");
  controls = new Controls(this);
  scene    = new Scene(this);
  context->window = this;
  FONT = new font(this);
  parentContext->addWindow(this);
}
void Window::addObject(Object * o){
  glfwMakeContextCurrent(window);
  scene->addObject(o);
}
void Window::addGround(){
  glfwMakeContextCurrent(window);
  scene->addGround();
}
void Window::render(){
  glClearColor(0.1,0.1,0.1,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  scene->render();
}
void Window::swap(){
  glfwSwapBuffers(window);
  glfwSwapInterval(1);
}
bool Window::shouldStop(){
  if( glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS || glfwWindowShouldClose(window)==1 ){
    glfwDestroyWindow(window);
    for(int i = 0 ; i < context->windows.size() ; i++)
      if(context->windows[i] == this)
        context->windows.erase(context->windows.begin() + i);
    //Set context indow
    return true;
  }
  else
    return false;
}



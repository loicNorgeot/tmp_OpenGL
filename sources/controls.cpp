#include "controls.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(action == GLFW_PRESS){
    if(key == GLFW_KEY_ESCAPE)
      glfwSetWindowShouldClose(window, GL_TRUE);

    if(key == GLFW_KEY_R){
      GLenum render = context->window->controls->render;
      GLenum tmp;
      switch(render){
      case(GL_POINT):
	tmp = GL_LINE;break;
      case(GL_LINE):
	tmp = GL_FILL;break;
      case(GL_FILL):
	tmp = GL_POINT;break;
      }
      context->window->controls->render = tmp;
    }

    if(key == GLFW_KEY_C){
      if(context->window->controls->cull == GL_BACK)
	context->window->controls->cull = GL_FRONT;
      else
	context->window->controls->cull = GL_BACK;
    }

    if(key == GLFW_KEY_S){
      context->window->controls->shader++;
      context->window->controls->shader = context->window->controls->shader % 5;
    }

    if(key == GLFW_KEY_Q){
      context->window->controls->animate = !context->window->controls->animate;
    }
    if(key == GLFW_KEY_KP_5){
      context->window->controls->ortho = !context->window->controls->ortho;
      context->window->scene->view->update();
    }
    /*
    if(key == GLFW_KEY_F){
      const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
      int width, height;
      glfwGetWindowSize(window, &width, &height);

      if(width != mode->width){
	glfwSetWindowSize(window, mode->width, mode->height);
	glfwSetWindowPos(window, 0, 0);
      }
      else{
	glfwSetWindowSize(window, 640, 480);
	glfwSetWindowPos(window, 1200, 400);
      }
    }
    */
  }
}

glm::vec3 getArcballVector(GLFWwindow* window, glm::vec2 pos){
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float X  =  2.0f * (float)(pos.x / width ) - 1.0f;
  float Y  =  2.0f * (float)(pos.y / height) - 1.0f;
  glm::vec3 P(X,-Y,0);
  std::cout << glm::length(P) << std::endl;
  if(glm::length(P) <= 1)
    P.z = std::sqrt(1 - glm::length(P));
  else
    P = glm::normalize(P);
  return P;
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){  

  glm::vec2 pos(xpos, ypos);

  /*
  glm::vec2 lastPos = context->window->controls->oldPos;
  if( (pos.x != lastPos.x || pos.y != lastPos.y) && context->window->controls->buttons[0]){
    glm::vec3 va = getArcballVector(window, lastPos);
    glm::vec3 vb = getArcballVector(window, pos);
    glm::vec3 axis = glm::cross(va, vb);
    float angle = std::acos(std::min(1.0f, glm::dot(va, vb)));

    glm::mat4 camera2object = glm::inverse(context->window->scene->view->VIEW * context->window->scene->object->MODEL);
    axis = glm::mat3(camera2object) * axis;

    //glm::vec3 center = glm::vec3( (context->window->scene->object->MODEL)[3] );
    context->window->scene->object->MODEL = 
      //glm::translate(glm::mat4(1), center) *
      glm::rotate(glm::mat4(1), glm::radians(angle), axis) *
      //glm::translate(glm::mat4(1), -center) *
      context->window->scene->object->MODEL
      ;
  }
  */
  
  if(context->window->controls->buttons[0]){
    glm::quat xRot = glm::angleAxis(0.01f * (float)(xpos - context->window->controls->oldPos.x), glm::normalize(glm::vec3(-1,1,-1)));
    glm::quat yRot = glm::angleAxis(-0.01f * (float)(ypos - context->window->controls->oldPos.y), glm::normalize(glm::vec3(-1,0,1)));
    glm::vec3 center = glm::vec3( (context->window->scene->object->MODEL)[3] );
    context->window->scene->object->MODEL = 
      glm::translate(glm::mat4(1), center) *
      glm::toMat4(xRot * yRot) *
      glm::translate(glm::mat4(1), -center) *
      context->window->scene->object->MODEL
      ;
  }
  

  if(context->window->controls->buttons[1]){
    glm::vec3 xTrans = -0.0025f * (float)(xpos - context->window->controls->oldPos.x) * glm::normalize(glm::vec3(-1,0,1));
    glm::vec3 yTrans = -0.0025f * (float)(ypos - context->window->controls->oldPos.y) * context->window->scene->view->up;
    context->window->scene->object->MODEL = glm::translate(glm::mat4(1) , xTrans + yTrans) * context->window->scene->object->MODEL;
  }
  

  context->window->controls->oldPos = pos;
}


void error_callback(int error, const char* description){
  fputs(description, stderr);
  exit(-1);
}
void window_size_callback(GLFWwindow* window, int width, int height){
  context->window->scene->view->update();
  glViewport(0, 0, width, height);
}
void drop_callback(GLFWwindow* window, int count, const char** paths){
  for (int i = 0;  i < count;  i++)
    context->window->scene->object = new Object(context->window->scene, (char *)(paths[i]));// << std::endl;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
  int isPressed = (action == GLFW_PRESS);
  switch(button){
  case GLFW_MOUSE_BUTTON_LEFT:
    context->window->controls->buttons[0] = isPressed;break;
  case GLFW_MOUSE_BUTTON_MIDDLE:
    context->window->controls->buttons[1] = isPressed;break;
  case GLFW_MOUSE_BUTTON_RIGHT:
    context->window->controls->buttons[2] = isPressed;break;
  }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  context->window->scene->view->cam *= 1 - yoffset/10;
  context->window->scene->view->update();
}


Controls::Controls(Window * window){
  parentWindow = window;
  render = GL_FILL;
  cull   = GL_BACK;
  animate = false;
  ortho   = false;
  shader  = 0;
  init();
}

void Controls::init(){
  GLFWwindow * w = parentWindow->window;

  glfwSetKeyCallback(w, key_callback);

  glfwSetMouseButtonCallback(w, mouse_button_callback);
  glfwSetCursorPosCallback(w, cursor_pos_callback);
  glfwSetScrollCallback(w, scroll_callback);

  glfwSetDropCallback(w, drop_callback);
  
  glfwSetWindowSizeCallback(w, window_size_callback);
  glfwSetErrorCallback(error_callback);

  glfwSetInputMode(w, GLFW_STICKY_KEYS, GL_TRUE);
}


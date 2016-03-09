#include "controls.h"

void setActiveWindow(GLFWwindow* window){
  for(Window* win : context->windows)
    if(win->window == window){
      context->window = win;
    }
  glfwMakeContextCurrent(context->window->window);
}
Controls::Controls(Window * window){
  parentWindow = window;
  cull    = GL_BACK;
  animate = false;
  ortho   = false;
  structure = 0;
  colors    = 0;
  lighting  = 0;
  init();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  setActiveWindow(window);
  if(action == GLFW_PRESS){
    if(key == GLFW_KEY_ESCAPE)
      glfwSetWindowShouldClose(window, GL_TRUE);

    if(key == GLFW_KEY_R){
      context->window->controls->structure++;
      context->window->controls->structure = context->window->controls->structure % 4;
    }

    if(key == GLFW_KEY_H){
      if(context->window->controls->cull == GL_BACK)
        context->window->controls->cull = GL_FRONT;
      else
        context->window->controls->cull = GL_BACK;
    }

    if(key == GLFW_KEY_C){
      context->window->controls->colors++;
      context->window->controls->colors = context->window->controls->colors % 3;
    }

    if(key == GLFW_KEY_L){
      context->window->controls->lighting++;
      context->window->controls->lighting = context->window->controls->lighting % 3;
    }

    if(key == GLFW_KEY_Q){
      context->window->controls->animate = !context->window->controls->animate;
    }

    //Views
    if(key == GLFW_KEY_KP_1){
      context->window->scene->view->cam = glm::vec3(0,0,1);
      context->window->scene->view->right = glm::vec3(1,0,0);
      context->window->scene->view->up = glm::vec3(0,1,0);
      context->window->scene->view->update();
    }
    if(key == GLFW_KEY_KP_3){
      context->window->scene->view->cam = glm::vec3(1,0,0);
      context->window->scene->view->right = glm::vec3(0,0,-1);
      context->window->scene->view->up = glm::vec3(0,1,0);
      context->window->scene->view->update();
    }
    if(key == GLFW_KEY_KP_7){
      context->window->scene->view->cam = glm::vec3(0,1,0);
      context->window->scene->view->right = glm::vec3(1,0,0);
      context->window->scene->view->up = glm::vec3(0,0,-1);
      context->window->scene->view->update();
    }
    if(key == GLFW_KEY_KP_5){
      context->window->controls->ortho = !context->window->controls->ortho;
      context->window->scene->view->update();
    }

    /*
    if( key == GLFW_KEY_UP ){
      for(int i = 0 ; i < context->window->scene->objects.size() ; i++){
        if(context->window->scene->active == context->window->scene->objects[i]){
          if(i<context->window->scene->objects.size()-1)
            context->window->scene->active = context->window->scene->objects[i+1];
          else
            context->window->scene->active = context->window->scene->objects[0];
          for(Object* obj : context->window->scene->objects)
            obj->selected = false;
          context->window->scene->active->selected = true;
          break;
        }
      }
    }
    else if(  key == GLFW_KEY_DOWN ){
      for(int i = 0 ; i < context->window->scene->objects.size() ; i++){
        if(context->window->scene->active == context->window->scene->objects[i]){
          if(i>0)
            context->window->scene->active = context->window->scene->objects[i-1];
          else
            context->window->scene->active = context->window->scene->objects.back();
          break;
          for(Object* obj : context->window->scene->objects)
            obj->selected = false;
          context->window->scene->active->selected = true;
        }
      }
    }
    */
  }
}
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){
  setActiveWindow(window);
  glm::vec2 pos(xpos, ypos);
  glm::vec2  old   = context->window->controls->oldPos;
  View*      view  = context->window->scene->view;
  glm::mat4* M     = &context->window->scene->active->MODEL;
  glm::vec3* cam   = &view->cam;
  glm::vec3* up    = &view->up;
  glm::vec3* right = &view->right;
  //Clic gauche: rotation
  if(context->window->controls->buttons[0]){
    if(context->window->scene->selected){
      glm::quat qRot =  glm::angleAxis(0.01f * (float)(pos.y - old.y), *right) *
                        glm::angleAxis(0.01f * (float)(pos.x - old.x), glm::vec3(0,1,0));
      glm::vec3 center = glm::vec3(0);//context->window->scene->center;
      glm::mat4 rot = glm::inverse(glm::translate(glm::mat4(1), center) * glm::toMat4(qRot) * glm::translate(glm::mat4(1), -center));
      *cam   = glm::vec3( rot * glm::vec4(*cam,0) );
      *right = glm::vec3( rot * glm::vec4(*right,1) );
      *up    = glm::cross(*right, -*cam);
      view->update();
    }
    else{
      glm::quat rotX, rotY;
      glm::mat4 rot;
      if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
          rot = glm::toMat4(glm::angleAxis(0.0025f * (float)(pos.y - old.y), glm::vec3(1,0,0)));
        else if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
          rot = glm::toMat4(glm::angleAxis(0.0025f * (float)(pos.y - old.y), glm::vec3(0,0,1)));
        else{
          rotX = glm::angleAxis(0.0025f * (float)(pos.x - old.x), glm::normalize(*up));
          rotY = glm::angleAxis(0.0025f * (float)(pos.y - old.y), glm::normalize(*right));
          rot = glm::toMat4(rotX * rotY);
        }
      }
      else{
        rotX = glm::angleAxis(0.01f * (float)(pos.x - old.x), glm::vec3(0,1,0));
        rotY = glm::angleAxis(0.0f, glm::normalize(*right));
        rot = glm::toMat4(rotX * rotY);
      }
      glm::vec3 center = glm::vec3((context->window->scene->active->MODEL)[3]);
      *M = glm::translate(glm::mat4(1), center) * rot * glm::translate(glm::mat4(1), -center) * *M;
    }
  }
  //Clic du milieu on translate
  if(context->window->controls->buttons[1]){
    if(context->window->scene->selected){
      glm::vec3 trX = 0.0025f * (float)(pos.x - old.x) * glm::normalize(*right);
      glm::vec3 trY = -0.0025f  * (float)(pos.y - old.y) * glm::normalize(*up);
      glm::vec3 tr = trX + trY;
      tr[1] = 0;
      context->window->scene->center = context->window->scene->center + tr;
      for(Object* obj : context->window->scene->objects)
        obj->MODEL[3] = glm::translate(glm::mat4(1), tr) * obj->MODEL[3];
      context->window->scene->background->MODEL[3] = glm::translate(glm::mat4(1), tr) * context->window->scene->background->MODEL[3];
      view->update();
    }
    else{
      glm::vec3 tr;
      if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        tr = -0.00075f  * (float)(pos.y - old.y) * glm::vec3(0,1,0);
      }
      else{
        glm::vec3 trX = 0.0025f * (float)(pos.x - old.x) * glm::normalize(*right);
        glm::vec3 trY = -0.0025f  * (float)(pos.y - old.y) * glm::normalize(*up);
        tr = trX + trY;
        tr[1] = 0;
      }
      *M = glm::translate(glm::mat4(1) , tr) * *M;
    }
  }
  //Clic droit on sélectionne
  if(context->window->controls->buttons[2]){
    /*
    float dY = ypos - context->window->controls->oldPos.y;
    float dX = xpos - context->window->controls->oldPos.x;
    if(context->window->scene->active->mBuffer != -1){
      std::vector<float> * pVertices = &(context->window->scene->active->vertices);
      std::vector<float> * pNormals  = &(context->window->scene->active->normals);
      for(int i = 0 ; i < pVertices->size() ; i+=3){
        glm::vec3 pos((*pVertices)[i], (*pVertices)[i+1], (*pVertices)[i+2]);
        glm::vec3 nor((*pNormals)[i], (*pNormals)[i+1], (*pNormals)[i+2]);
        pos += 0.00025f * dY * nor;
        for(int j = 0 ; j < 3 ; j++)
          (*pVertices)[i+j] = pos[j];
      }
      updateBuffer(context->window->scene->active->mBuffer, pVertices);
    }

    if(context->window->scene->active->cBuffer != -1){
      std::vector<float> * pColors = &(context->window->scene->active->colors);
      for(int i = 0 ; i < pColors->size() ; i+=3){
        glm::vec3 col( (*pColors)[i], (*pColors)[i+1], (*pColors)[i+2]);
        col = glm::rotate(col, 0.01f * dX, glm::vec3(1,1,1));
        for(int j = 0 ; j < 3 ; j++)
          (*pColors)[i+j] = col[j];
      }
      updateBuffer(context->window->scene->active->cBuffer, pColors);
    }
    */
  }
  context->window->controls->oldPos = pos;
}
void error_callback(int error, const char* description){
  fputs(description, stderr);
  exit(-1);
}
void window_size_callback(GLFWwindow* window, int width, int height){
  setActiveWindow(window);
  context->window->scene->view->update();
  glViewport(0, 0, width, height);
}
void drop_callback(GLFWwindow* window, int count, const char** paths){
  setActiveWindow(window);
  for (int i = 0;  i < count;  i++)
    context->window->scene->active = new Object(context->window->scene, (char *)(paths[i]));// << std::endl;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
  setActiveWindow(window);
  int isPressed = (action == GLFW_PRESS);
  switch(button){
  case GLFW_MOUSE_BUTTON_LEFT:
    context->window->controls->buttons[0] = isPressed;break;
  case GLFW_MOUSE_BUTTON_MIDDLE:
    context->window->controls->buttons[1] = isPressed;break;
  case GLFW_MOUSE_BUTTON_RIGHT:
    context->window->controls->buttons[2] = isPressed;break;
  }

  if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
    unsigned char pixel[3];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0 ; i < context->window->scene->objects.size() ; i++){
      context->window->scene->objects[i]->pickingRender();
    }
    double xpos, ypos;
    glfwGetCursorPos( context->window->window, &xpos, &ypos);
    glReadPixels(xpos,viewport[3]-ypos,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);
    int id = 255*255*int(pixel[2]) + 255*int(pixel[1]) + int(pixel[0]);
    std::cout << "Picked object = " << id << std::endl;

    Object* obj = NULL;
    for(Object* o : context->window->scene->objects){
      if(o->id == id-1)
        obj = o;
      else
        o->selected = false;
    }
    if (obj==NULL){
      for(Object* o : context->window->scene->objects){
        o->selected = false;
        context->window->scene->active = NULL;
      }
      context->window->scene->selected = true;
    }
    else{
      if( ! obj->selected ){
        obj->selected = true;
        context->window->scene->selected = false;
        context->window->scene->active = obj;
      }
      else{
        obj->selected = false;
        context->window->scene->selected = true;
        context->window->scene->active = NULL;
      }
    }
  }
}
void window_focus_callback(GLFWwindow* window, int focused){
  if (focused){
    setActiveWindow(window);
    for(Window* win : context->windows)
      if(win->window == window)
        std::cout << win->scene->objects.size() << std::endl;
  }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    setActiveWindow(window);
    if(yoffset > 0){
        context->window->scene->view->zoom *= 1 - 0.05;
        context->window->scene->view->update();
    }
    if(yoffset < 0){
        context->window->scene->view->zoom *= 1 + 0.05;
        context->window->scene->view->update();
    }
}

void Controls::init(){
  setActiveWindow(parentWindow->window);
  GLFWwindow * w = parentWindow->window;

  glfwSetKeyCallback(w, key_callback);

  glfwSetMouseButtonCallback(w, mouse_button_callback);
  glfwSetCursorPosCallback(w, cursor_pos_callback);
  glfwSetScrollCallback(w, scroll_callback);

  glfwSetDropCallback(w, drop_callback);

  glfwSetWindowSizeCallback(w, window_size_callback);
  glfwSetErrorCallback(error_callback);

  glfwSetWindowFocusCallback(w, window_focus_callback);

  glfwSetInputMode(w, GLFW_STICKY_KEYS, GL_TRUE);
}

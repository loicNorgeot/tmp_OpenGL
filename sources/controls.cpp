#include "controls.h"

#include <glm/gtx/intersect.hpp>
#include <glm/gtx/vector_angle.hpp>

glm::mat4 tmpMODEL;

void setActiveWindow(GLFWwindow* window){
  for(Window* win : context->windows)
    if(win->window == window){
      context->window = win;
    }
  glfwMakeContextCurrent(context->window->window);
}
bool intersects(Window* window, glm::vec3 center, glm::vec3 normal, glm::vec2 cursor, glm::vec3 &intersection){
  View* view = window->scene->view;

  int       w = view->w;
  int       h = view->h;

  glm::vec2 norm_pos(((float)cursor.x/((float)w*0.5f) - 1) * (float)w/(float)h, 1.0f - (float)cursor.y/((float)h*0.5f));

  float     fov             = view->fov;
  float     tan             = tanf(fov * 0.5f);
  glm::vec2 fov_coordinates = tan * norm_pos;
  float     near            = view->min;
  float     far             = view->max;

  glm::vec3 near_point(fov_coordinates.x * near, fov_coordinates.y * near, -near);
  glm::vec3 far_point( fov_coordinates.x * far,  fov_coordinates.y * far,  -far);

  //World coordinates
  glm::mat4 inv             = glm::inverse(view->VIEW);
  near_point                = view->cam + glm::vec3( inv * glm::vec4(near_point, 1) );
  far_point                 = view->cam + glm::vec3( inv * glm::vec4(far_point,  1) );
  glm::vec3 ray             = glm::normalize(far_point - near_point);

  float      intersectDist;
  bool intersectionExists = glm::intersectRayPlane(view->cam, ray, center, normal, intersectDist);
  if(intersectionExists)
    intersection = view->cam + intersectDist * ray;
  return intersectionExists;
}
float orientedAngle(glm::vec3 pt1, glm::vec3 pt2, glm::vec3 center, glm::vec3 axis){
  return glm::orientedAngle( glm::normalize(glm::vec3(center.x - pt1.x, center.y, center.z - pt1.z)),
                             glm::normalize(glm::vec3(center.x - pt2.x, center.y, center.z - pt2.z)),
                             axis);
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

//Fonctions de GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(action == GLFW_PRESS){

    //Clipping plane
    if(key == GLFW_KEY_P){
      for(int i = 0 ; i < context->window->scene->objects.size() ; i++){
        Object* o = context->window->scene->objects[i];
        if(o->selected){
          o->clipped = !o->clipped;
        }
      }
    }

    //Open in a new window
    if(key == GLFW_KEY_N){
      for(int i = 0 ; i < context->window->scene->objects.size() ; i++){
        Object* o = context->window->scene->objects[i];
        if(o->selected){
          Window* win = new Window(MANIPULATION, WINDOWED, 500, 500, 0, 0);
          win->addObject(new Object(o));
        }
      }
    }

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


    if(key == GLFW_KEY_X){
      if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        for(int i = 0 ; i < context->window->scene->objects.size() ; i++){
          Object* o = context->window->scene->objects[i];
          if(o->selected){
            std::cout << "Cutting " << o->meshfile << std::endl;
            glfwSetClipboardString(window, o->meshfile.c_str());
            tmpMODEL = o->MODEL;
            context->window->scene->objects.erase(context->window->scene->objects.begin()+i);
          }
        }
      }
    }
    if(key == GLFW_KEY_C){
      if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        for(Object* o : context->window->scene->objects){
          if(o->selected){
            std::cout << "Copying " << o->meshfile << std::endl;
            glfwSetClipboardString(window, o->meshfile.c_str());
            tmpMODEL = o->MODEL;
          }
        }
      }
      else{
        context->window->controls->colors++;
        context->window->controls->colors = context->window->controls->colors % 3;
      }
    }
    if(key == GLFW_KEY_V){
      if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        std::cout << "Pasting " << glfwGetClipboardString(window) << std::endl;
        context->window->addObject(new Object((char*)glfwGetClipboardString(window)));
        glm::vec3 inter;
        double x,y;
        glfwGetCursorPos(context->window->window, &x, &y);
        context->window->scene->objects.back()->MODEL = tmpMODEL;
        context->window->scene->objects.back()->selected = true;
        context->window->scene->active = context->window->scene->objects.back();
        context->window->scene->selected = false;
        if(intersects(context->window, glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec2(x,y), inter))
          context->window->scene->objects.back()->MODEL[3] = glm::vec4(inter,1);
        else
          context->window->scene->objects.back()->MODEL[3] = glm::vec4(0,0,0,1);
        for(int i = 0 ; i < context->window->scene->objects.size() ; i++)
          context->window->scene->objects[i]->computeID(i+1);
      }
    }



    if(key == GLFW_KEY_L){
      context->window->controls->lighting++;
      context->window->controls->lighting = context->window->controls->lighting % 3;
    }

    if(key == GLFW_KEY_Q){
      context->window->controls->animate = !context->window->controls->animate;
    }

    //Views
    if(key == GLFW_KEY_KP_1)
      context->window->scene->view->frontView();
    if(key == GLFW_KEY_KP_3)
      context->window->scene->view->sideView();
    if(key == GLFW_KEY_KP_7)
      context->window->scene->view->upView();
    if(key == GLFW_KEY_KP_5)
      context->window->scene->view->toogleOrtho();

    if(key == GLFW_KEY_S){
      for(Object* obj : context->window->scene->objects){
        if(obj->selected)
          obj->MODEL = glm::scale(obj->MODEL, glm::vec3(-1,1,1));
      }
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
  glm::vec2 pos(xpos, ypos);

  glm::vec2  old   = context->window->controls->oldPos;
  View*      view  = context->window->scene->view;
  glm::mat4* M     = &context->window->scene->active->MODEL;
  glm::vec3* cam   = &view->cam;
  glm::vec3* up    = &view->up;
  glm::vec3* right = &view->right;
  CONTROLS   type  = context->window->TYPE;
  //Clic gauche: rotation
  if(context->window->controls->buttons[0]){
    glm::vec2 diff = pos - old;
    if(context->window->scene->selected){
      glm::quat qRot;
      if(type == MANIPULATION)
        qRot =  glm::angleAxis(0.01f * (float)(diff.y), *right) *
                glm::angleAxis(0.01f * (float)(diff.x), *up);
      else
        qRot =  glm::angleAxis(0.01f * (float)(diff.y), *right) *
                glm::angleAxis(0.01f * (float)(diff.x), glm::vec3(0,1,0));
      glm::vec3 center = glm::vec3(0);//context->window->scene->center;
      glm::mat4 rot = glm::inverse(glm::translate(glm::mat4(1), center) * glm::toMat4(qRot) * glm::translate(glm::mat4(1), -center));
      *cam   = glm::vec3( rot * glm::vec4(*cam,0) );
      *right = glm::vec3( rot * glm::vec4(*right,1) );
      *up    = glm::cross(*right, -*cam);
      view->update();
    }
    else if (type!=MANIPULATION){
      glm::quat rotX, rotY;
      glm::mat4 rot;
      if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
          rot = glm::toMat4(glm::angleAxis(0.0025f * (float)(diff.y), glm::vec3(1,0,0)));
        else if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
          rot = glm::toMat4(glm::angleAxis(0.0025f * (float)(diff.y), glm::vec3(0,0,1)));
        else{
          rotX = glm::angleAxis(0.0025f * (float)(diff.x), glm::normalize(*up));
          rotY = glm::angleAxis(0.0025f * (float)(diff.y), glm::normalize(*right));
          rot = glm::toMat4(rotX * rotY);
        }
      }
      else{
        glm::vec3 inter, oldInter;
        if(intersects(context->window, glm::vec3(0,(*M)[3][1],0), glm::vec3(0,1,0), pos, inter) && intersects(context->window, glm::vec3(0,(*M)[3][1],0), glm::vec3(0,1,0), old, oldInter)){
          float angle = orientedAngle(oldInter, inter, glm::vec3((*M)[3]), glm::vec3(0,1,0));
          rot = glm::toMat4(glm::angleAxis(angle, glm::vec3(0,1,0)));
        }

        //Est ce qu'on dépasse?
        glm::vec3 s  = context->window->scene->active->size;
        glm::vec3 sc = context->window->scene->center;
        std::vector<glm::vec3> corners;
        glm::vec3 bounds = glm::vec3(0.5,0,0.5);
        corners.push_back(glm::vec3(*M * glm::vec4( glm::vec3(-s.x,0,-s.z),1)));
        corners.push_back(glm::vec3(*M * glm::vec4( glm::vec3(+s.x,0,-s.z),1)));
        corners.push_back(glm::vec3(*M * glm::vec4( glm::vec3(+s.x,0,+s.z),1)));
        corners.push_back(glm::vec3(*M * glm::vec4( glm::vec3(-s.x,0,+s.z),1)));
        for(glm::vec3 c : corners){
          //glm::vec3 newPos = c - sc;
          glm::vec3 center = glm::vec3((context->window->scene->active->MODEL)[3])+sc;
          glm::vec3 newPos = glm::vec3( glm::translate(glm::mat4(1), center) * rot * glm::translate(glm::mat4(1), -center) * glm::vec4(c,1) )-sc;
          if(newPos.x > bounds.x || newPos.x < -bounds.x || newPos.z > bounds.z || newPos.z < -bounds.z)
            rot = glm::mat4(1);
        }

      }
      glm::vec3 center = glm::vec3((context->window->scene->active->MODEL)[3]);
      *M = glm::translate(glm::mat4(1), center) * rot * glm::translate(glm::mat4(1), -center) * *M;
    }
  }
  //Clic du milieu on translate
  if(context->window->controls->buttons[1]){
    if(context->window->scene->selected){
      glm::vec3 tr, inter, oldInter;
      if(intersects(context->window, glm::vec3(0,0,0), glm::vec3(0,1,0), pos, inter) && intersects(context->window, glm::vec3(0,0,0), glm::vec3(0,1,0), old, oldInter))
        tr = inter-oldInter;
      if(type == GALERY)
        tr.x = 0;
      context->window->scene->center = context->window->scene->center + tr;
      for(Object* obj : context->window->scene->objects)
        obj->MODEL[3] = glm::translate(glm::mat4(1), tr) * obj->MODEL[3];
      if(context->window->scene->ground)
        context->window->scene->background->MODEL[3] = glm::translate(glm::mat4(1), tr) * context->window->scene->background->MODEL[3];
      view->update();
    }
    else{
      glm::vec3 tr;
      if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        glm::vec3 inter,oldInter;
        if(intersects(context->window, glm::vec3((*M)[3]), glm::vec3(view->cam.x,0,view->cam.z), pos, inter) && intersects(context->window, glm::vec3((*M)[3]), glm::vec3(view->cam.x,0,view->cam.z), old, oldInter))
            tr = inter - oldInter;//oldInter - inter;//-0.00075f  * (float)(pos.y - old.y) * glm::vec3(0,1,0);
      }
      else{
        glm::vec3 inter, oldInter;
        if(intersects(context->window, glm::vec3(0,0,0), glm::vec3(0,1,0), pos, inter) && intersects(context->window, glm::vec3(0,0,0), glm::vec3(0,1,0), old, oldInter))
          tr = inter-oldInter;
        if(context->window == context->windows[0])
          tr.x = 0;
      }

      //Est ce qu'on dépasse?
      glm::vec3 s  = context->window->scene->active->size;
      glm::vec3 sc = context->window->scene->center;
      std::vector<glm::vec3> corners;
      glm::vec3 bounds = glm::vec3(0.5,0,0.5);
      corners.push_back(glm::vec3(*M * glm::vec4( glm::vec3(-s.x,0,-s.z),1)));
      corners.push_back(glm::vec3(*M * glm::vec4( glm::vec3(+s.x,0,-s.z),1)));
      corners.push_back(glm::vec3(*M * glm::vec4( glm::vec3(+s.x,0,+s.z),1)));
      corners.push_back(glm::vec3(*M * glm::vec4( glm::vec3(-s.x,0,+s.z),1)));
      bool goesOut = false;
      for(glm::vec3 c : corners){
        glm::vec3 newPos = c - sc + tr;
        if(newPos.x > bounds.x || newPos.x < -bounds.x)
          tr.x = 0;
        if(newPos.z > bounds.z || newPos.z < -bounds.z)
          tr.z = 0;
      }
      *M = glm::translate(glm::mat4(1) , tr) * *M;
    }
  }
  //Clic droit on change les couleurs
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
  context->window->scene->view->update();
  glViewport(0, 0, width, height);
}
void drop_callback(GLFWwindow* window, int count, const char** paths){
  for (int i = 0;  i < count;  i++)
    context->window->scene->active = new Object((char *)(paths[i]));// << std::endl;
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
      if(o->id == id-1){
        std::cout << o->meshfile << std::endl;
        obj = o;
      }
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
void cursor_enter_callback(GLFWwindow* window, int entered){
  if (entered){
    for(Window* win : context->windows)
      if(win->window == window){
        context->window = win;
      }
    glfwMakeContextCurrent(context->window->window);
  }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
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
  GLFWwindow * w = parentWindow->window;

  glfwSetKeyCallback(w, key_callback);

  glfwSetMouseButtonCallback(w, mouse_button_callback);
  glfwSetCursorPosCallback(w, cursor_pos_callback);
  glfwSetScrollCallback(w, scroll_callback);

  glfwSetDropCallback(w, drop_callback);

  glfwSetWindowSizeCallback(w, window_size_callback);
  glfwSetErrorCallback(error_callback);

  glfwSetCursorEnterCallback(w, cursor_enter_callback);

  glfwSetInputMode(w, GLFW_STICKY_KEYS, GL_TRUE);
}

#ifndef COMMONS_H
#define COMMONS_H

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <iostream>
#include <fstream>

#include "utils.h"

//Debugging
void print(glm::vec3 v);
void print(glm::mat4 m);
void glError(std::string name);

//GPU buffers and bindings
#pragma GCC diagnostic ignored "-Wwrite-strings"
int  initProgram(int ID);
void createBuffer(GLuint *pBuffer, std::vector<float> *data);
void createBuffer(GLuint *pBuffer, std::vector<short> *data);
void createBuffer(GLuint *pBuffer, std::vector<int>   *data);
void createBuffer(GLuint *pBuffer, std::vector<unsigned int>   *data);
void bindBuffer(int attrib, int ID, GLuint buffer, char* name);
void bindIndicesBuffer(GLuint buffer);
void freeBuffer();
void send(int ID, float     f, 	char* name);
void send(int ID, glm::vec3 v, 	char* name);
void send(int ID, glm::mat4 &m, char* name);

class Controls;
class Shader;
class Window;
class Object;
class View;
class Scene;
class CONTEXT;

extern CONTEXT * context;

#include "context.h"
#include "scene.h"
#include "shader.h"
#include "view.h"
#include "object.h"
#include "window.h"
#include "controls.h"

#endif

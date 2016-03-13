#ifndef UTILS_H
#define UTILS_H

#include "commons.h"
#include <sstream>

//Debugging
void print(glm::vec3 v);
void print(glm::mat4 m);
void glError(std::string name);

//GPU buffers and bindings
#pragma GCC diagnostic ignored "-Wwrite-strings"
void createVAO(GLuint* pVAO);
void updateBuffer(GLuint  pBuffer, std::vector<float> *data);
void createBuffer(GLuint *pBuffer, std::vector<float> *data);
void createBuffer(GLuint *pBuffer, std::vector<short> *data);
void createBuffer(GLuint *pBuffer, std::vector<int>   *data);
void createBuffer(GLuint *pBuffer, std::vector<unsigned int>   *data);
void bindBuffer(int attrib, int ID, GLuint buffer, char* name);
void bindIndicesBuffer(GLuint buffer);
void freeBuffer();
void send(int ID, float     f, 	char* name);
void send(int ID, int       i, 	char* name);
void send(int ID, glm::vec3 v, 	char* name);
void send(int ID, glm::mat4 &m, char* name);


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif

#include "utils.h"

//DEBUGGING
void print(glm::vec3 v){
  std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

void print(glm::mat4 m){
  for( int i = 0 ; i < 4 ; i++ )
    std::cout << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << std::endl;
}

void glError(std::string name){
  GLenum error = glGetError();
  if(error != GL_NO_ERROR){
    printf("%s : %d\n", name.c_str(), error);
    glfwTerminate();
    exit(-1);
  }
}


//BUFFER OPERATIONS
void    createBuffer(GLuint *pBuffer, std::vector<float> *data){
  glGenBuffers( 1,               pBuffer);
  glBindBuffer( GL_ARRAY_BUFFER, *pBuffer);
  glBufferData( GL_ARRAY_BUFFER, sizeof(float) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}

void    createBuffer(GLuint *pBuffer, std::vector<int> *data){
  glGenBuffers( 1,               pBuffer);
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pBuffer);
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}

void    createBuffer(GLuint *pBuffer, std::vector<short> *data){
  glGenBuffers( 1,               pBuffer);
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pBuffer);
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}

void createVAO(GLuint* pVAO){
  glGenVertexArrays(1, pVAO);
  glBindVertexArray(*pVAO);
}

void    bindBuffer(int attrib, int ID, GLuint buffer, char* name){
  if(buffer!=-1){
    glBindBuffer( GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray( attrib );
    glVertexAttribPointer( attrib, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
    glBindAttribLocation(ID, attrib, name);
  }
}

void    bindIndicesBuffer(GLuint buffer){
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer);
}

void send(int ID, glm::mat4 &m, char* name){
  int id = glGetUniformLocation(ID, name);
  glUniformMatrix4fv(id, 1, GL_FALSE, &m[0][0]);
}

void send(int ID, glm::vec3 v, char* name){
  int id = glGetUniformLocation(ID, name);
  glUniform3f(id, v.x, v.y, v.z);
}

void send(int ID, float f, char* name){
  int id = glGetUniformLocation(ID, name);
  glUniform1f(id, f);
}

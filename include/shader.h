#ifndef SHADER_H
#define SHADER_H

#include "commons.h"

class Shader
{
public:
  GLuint      ID;
  CONTEXT * parentContext;
  Shader(CONTEXT * context){
    ID = 0;
    parentContext = context;
  }
  void 		load(const char * vert_path, const char * frag_path);
};

#endif

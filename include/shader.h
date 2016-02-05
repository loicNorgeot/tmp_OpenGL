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
  void 		load(std::string vert_path, std::string frag_path, std::string functions_file_path);
};

#endif

#ifndef SHADER_H
#define SHADER_H

#include "commons.h"

class Shader
{
public:
  GLuint      ID;
  Window* parentWindow;
  Shader(Window* window){
    ID = 0;
    parentWindow = window;
  }
  void 		load(std::string vert_path, std::string frag_path, std::string functions_file_path);
};

#endif

#ifndef TEXT_H
#define TEXT_H

#include "commons.h"

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

/// State information relevant to a character
struct Character {
    GLuint TextureID;    // ID handle of the glyph texture
    glm::ivec2 Size;     // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;      // Horizontal offset to advance to next glyph
};

class font{
 public:
  GLuint VAO, VBO;
  Shader* shader;
  Window* parentWindow;
  std::map<GLchar, Character> Characters;
  font(Window* window);
  void render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

#endif

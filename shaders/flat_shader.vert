#version 120

attribute vec3 vertex_position;
attribute vec3 vertex_normal;
attribute vec3 vertex_color;
varying vec3   vertex_pos;
varying vec3   vertex_nor;
varying vec3   vertex_col;

uniform mat4 MVP;

void main(){
  vec4 pos = MVP * vec4(vertex_position, 1);
  gl_Position = pos;
  vertex_pos = vertex_position;
}




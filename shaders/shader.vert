#version 330 core

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_color;

out vec3 color;
out vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 MVP;

void main(){
  gl_Position = MVP * vec4(vertex_position, 1);
  color = vertex_color;
  normal = vertex_normal;
}



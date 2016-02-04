#version 330 core

in vec3 color;
in vec3 normal;

uniform vec3 col;

out vec3 out_color;

void main(){
  out_color = color;
}


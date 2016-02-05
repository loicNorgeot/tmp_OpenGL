#version 330 core

in vec3 frag_position;
in vec3 frag_color;
in vec3 frag_normal;

//Render options:
// 0 - simple color, no shading
// 1 - vertex color, no shading
// 2 - normal color, no shading
// 3 - simple color, flat shading
uniform int  shader;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform vec3 col;

out vec3 out_color;

vec3 light(mat4 light_matrix, vec3 mater_color, int light);

void main(){

  if(shader == 0){
    out_color = col;
  }
     
  if(shader == 1){
    out_color = frag_color;
  }

  if(shader == 2){
    out_color = vec3( abs(frag_normal.x), abs(frag_normal.y), abs(frag_normal.z) );
  }

  if(shader == 3){
    mat4 LIGHT = mat4(vec4(-10,10,10,0), vec4(1,1,1,0), vec4(0.15,0.7,0.7,0), vec4(300,5,1,0));
    out_color = light(LIGHT, col, 0);   
  }

  if(shader == 4){
    mat4 LIGHT = mat4(vec4(-10,10,10,0), vec4(1,1,1,0), vec4(0.15,0.7,0.7,0), vec4(300,5,1,0));
    out_color = light(LIGHT, frag_normal, 1);
  }
}

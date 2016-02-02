// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
using namespace glm;


////////////////////////////////////////////////////////////////////////////////////////////////////////
// SHADERS
////////////////////////////////////////////////////////////////////////////////////////////////////////

class Shader
{
public:
  	GLuint      ID;

  	Shader(){ID = 0;}
	//std::string openShader(std::string path);
	//void 		compileShader(GLuint id, const char * code);
	int 		compileShader(std::string path, GLenum shaderType);
	int 		compileShader(GLuint vertID, GLuint fragID);
  	void 		load(std::string vert_path, std::string frag_path);
};
/*
std::string Shader::openShader(std::string path){
	std::string code = "";
    std::ifstream stream(path.c_str(), std::ios::in);
    if(stream.is_open()){
      std::string Line = "";
      while(getline(stream, Line))
        code += "\n" + Line;
      stream.close();
    }
    else{
		exit(144);
	}
	return code;
}

void Shader::compileShader(GLuint id, const char * code){
    char const * pSource = code;
    glShaderSource(id, 1, &pSource , NULL);
    glCompileShader(id);
    
	int logLength;
	GLint Result = GL_FALSE;
	
    glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> Error(logLength);
    glGetShaderInfoLog(id, logLength, NULL, &Error[0]);
    fprintf(stdout, "%s\n", &Error[0]);
}
*/
int Shader::compileShader(GLuint vertID, GLuint fragID){
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vertID);
    glAttachShader(ProgramID, fragID);
    glLinkProgram(ProgramID);

	int logLength;
	GLint Result = GL_FALSE;
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> Error( std::max(logLength, int(1)) );
    glGetProgramInfoLog(ProgramID, logLength, NULL, &Error[0]);
    fprintf(stdout, "%s\n", &Error[0]);

    glDeleteShader(vertID);
    glDeleteShader(fragID);
	
	return ProgramID;
}




int Shader::compileShader(std::string path, GLenum shaderType){
	GLuint id = glCreateShader(shaderType);
	//Reading the code
	std::string code;
    std::ifstream stream(path.c_str(), std::ios::in);
    if(stream.is_open()){
      std::string Line = "";
      while(getline(stream, Line))
        code += "\n" + Line;
      stream.close();
    }
	//Compiling the shader
	GLint Result = GL_FALSE;
	int logLength;
	printf("Compiling shader : %s\n", path.c_str());
	char const * pSource = code.c_str();
	glShaderSource(id, 1, &pSource , NULL);
	glCompileShader(id);
	//Verifying status
	glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> errorMessage(logLength);
	glGetShaderInfoLog(id, logLength, NULL, &errorMessage[0]);
	fprintf(stdout, "%s\n", &errorMessage[0]);
	//Return shader program id
	return id;
}




void Shader::load(std::string vert_path, std::string frag_path){
    GLuint vertID = compileShader(vert_path, GL_VERTEX_SHADER);
    GLuint fragID = compileShader(frag_path, GL_FRAGMENT_SHADER);
	printf("Shaders ID = %d %d \n", vertID, fragID);
	
    ID = compileShader(vertID, fragID);
}






int main(){
// Initialise GLFW
if( !glfwInit() )
{
    fprintf( stderr, "Failed to initialize GLFW\n" );
    return -1;
}
glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


// Open a window and create its OpenGL context
GLFWwindow* window; // (In the accompanying source code, this variable is global)
window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
if( window == NULL ){
   fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
    glfwTerminate();
    return -1;
}
glfwMakeContextCurrent(window); // Initialize GLEW



glewExperimental=true; // Needed in core profile
if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
}




GLuint VertexArrayID;
glGenVertexArrays(1, &VertexArrayID);
glBindVertexArray(VertexArrayID);



glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


Shader* shader1 = new Shader();
shader1->load("/Users/guestaccount/dev/Minimal_OPENGL/shaders/shader.vert", "/Users/guestaccount/dev/Minimal_OPENGL/shaders/shader.frag");


// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};
GLuint vertexbuffer;
glGenBuffers(1, &vertexbuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);



//int loc = glGetFragDataLocation( shader1->ID, "col" );
//glBindFragDataLocationExt( shader1->ID, loc, "col" );


do{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader1->ID);
   

   // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
      (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
	
    glfwSwapBuffers(window);
    glfwPollEvents();
	
	

} // Check if the ESC key was pressed or the window was closed
while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
}



/*
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "simple_shader_frag.h"
#include "simple_shader_vert.h"

#include <vector>
#include <iostream>
#include <fstream>

class Shader;
class Object;

Shader* shader1;

void print(glm::vec3 v){
	std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}
void print(glm::mat4 m){
	for( int i = 0 ; i < 4 ; i++ )
		std::cout << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << std::endl;
}












////////////////////////////////////////////////////////////////////////////////////////////////////////
// OBJECT
////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic ignored "-Wwrite-strings"

int  initProgram(int ID);
void createBuffer(GLuint *pBuffer, std::vector<float> *data);
void createBuffer(GLuint *pBuffer, std::vector<short> *data);
void createBuffer(GLuint *pBuffer, std::vector<int>   *data);
void bindBuffer(int attrib, int ID, GLuint buffer, char* name);
void bindIndicesBuffer(GLuint buffer);
void freeBuffer();
void draw(int ID, int s, int mBuffer, int nBuffer, int iBuffer, int cBuffer = -1);
void send(int ID, float     f, 	char* name);
void send(int ID, glm::vec3 v, 	char* name);
void send(int ID, glm::mat4 &m, char* name);

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
void    bindBuffer(int attrib, int ID, GLuint buffer, char* name){
	if(buffer!=-1){
    	glEnableVertexAttribArray( attrib );
    	glBindBuffer( GL_ARRAY_BUFFER, buffer);
    	glVertexAttribPointer( attrib, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
		glBindAttribLocation(ID, attrib, name);
	}
}
void    bindIndicesBuffer(GLuint buffer){
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer);
}
void    freeBuffer(){
    for(int i = 0 ; i < 5 ; i++)
        glDisableVertexAttribArray(i);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
int     initProgram(int ID){
    glUseProgram(ID);
    for(int i = 0 ; i < 5 ; i++)
        glEnableVertexAttribArray(5);
    return ID;
}
void    draw(   int ID,         //Shader ID
                int s,          //Number of vertices
                int mBuffer,    //Vertexs BUffer
                int nBuffer,    //Normals Buffer
                int iBuffer,    //Indices Buffer
                int cBuffer     //Colors Buffer
            ){
	initProgram(ID);
    bindBuffer(0, ID, mBuffer, "vertex_position");
	bindBuffer(1, ID, nBuffer, "vertex_normal");
	bindBuffer(2, ID, cBuffer, "vertex_color");
    bindIndicesBuffer(iBuffer);
    glDrawElements(GL_TRIANGLES, s, GL_UNSIGNED_INT, (void*)0);
    freeBuffer();
}
void send(int ID, glm::mat4 &m, char* name){
	glGetUniformLocation(	ID, name);
    glUniformMatrix4fv( 	ID, 1, GL_FALSE, &m[0][0]);
	
}
void send(int ID, glm::vec3 v, 	char* name){
	glGetUniformLocation(	ID, name);
    glUniform3f(			ID, v.x, v.y, v.z);
}
void send(int ID, float     f, 	char* name){
	glGetUniformLocation(	ID, name);
	glUniform1f( 		 	ID, f);
}



class Object{
public:
	
	int nbVertices;
	
    std::vector<float>  vertices;
    std::vector<float>  normals;
    std::vector<float>  colors;
    std::vector<int>    indices;

    GLuint              mBuffer;
    GLuint              nBuffer;
    GLuint              cBuffer;
    GLuint              iBuffer;
	
	glm::mat4			MODEL;
	
	Object(){
        vertices = std::vector<float>{
            0,0,0,
			100,0,0,
			0,0,-100
        };
		nbVertices = vertices.size()/3;
        indices = std::vector<int>{
            0, 1, 2
        };
		
		colors = {
			0, 0, 0,   
			0, 0, 1,   
			0, 1, 0
	    };
		normals = {
			0, 1,0
	    };
	    createBuffer(&mBuffer, &vertices);
	    createBuffer(&iBuffer, &indices);
	    createBuffer(&nBuffer, &normals);
	    createBuffer(&cBuffer, &colors);
	    freeBuffer();
		MODEL = glm::mat4(1.);
	}
	
	void render(){
        //glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
        int shaderID                = shader1->ID;

		glm::mat4 PROJ = glm::perspective(70.0f, (float)1024 / (float)620, 0.1f, 100.f);//glm::radians(45.0f)
		glm::mat4 VIEW = glm::lookAt(	glm::vec3(-5,5,-5), //eye
										glm::vec3(0.,0.,0.), //look
										glm::vec3(0.,1.,0.)  //up
									);
        glm::mat4 MVP               = PROJ * VIEW * MODEL;
		
        //send(shaderID, MODEL, 	"M");
		initProgram(shaderID);
		send(shaderID, MVP, 	"MVP");
		//send(shaderID, VIEW, 	"V");
		
        draw(shaderID, nbVertices, mBuffer, nBuffer, iBuffer, cBuffer);
	}	
};



////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////////////////////////////

void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
        if(key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* initWindow(const int resX, const int resY)
{
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(resX, resY, "TEST", NULL, NULL);

    if(window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controls);
	

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return window;
}



void display( GLFWwindow* window, Object* obj )
{
    while(!glfwWindowShouldClose(window))
    {
        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // Draw stuff
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        obj->render();

        // Update Screen
        glfwSwapBuffers(window);

        // Check for any input, or window movement
        glfwPollEvents();
    }
}


int main(int argc, char** argv)
{
    GLFWwindow* window = initWindow(1024, 620);
    glewExperimental = GL_TRUE;
    glewInit();
	
	// Create and bind a VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	Object* object1 = new Object();
	shader1 = new Shader();
	shader1->load("/Users/guestaccount/dev/Minimal_OPENGL/shaders/shader.vert", "/Users/guestaccount/dev/Minimal_OPENGL/shaders/shader.frag");
    if( NULL != window )
    {
        display( window, object1 );
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

*/





